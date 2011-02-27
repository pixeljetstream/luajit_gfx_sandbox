// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/luxgfx.h>

static lxGLBufferTarget_t l_vglbuffers[] = {
  LUXGL_BUFFER_VERTEX,
  LUXGL_BUFFER_INDEX,
  LUXGL_BUFFER_PIXELREAD,
  LUXGL_BUFFER_PIXELWRITE,
  LUXGL_BUFFER_UNIFORM,
  LUXGL_BUFFER_TEXTURE,
  LUXGL_BUFFER_FEEDBACK,
  LUXGL_BUFFER_CPYWRITE,
  LUXGL_BUFFER_CPYREAD,
  LUXGL_BUFFER_DRAWINDIRECT,
  LUXGL_BUFFER_NVVIDEO,
  LUXGL_BUFFER_NVPARAM_FRAGMENT,
  LUXGL_BUFFER_NVPARAM_VERTEX,
  LUXGL_BUFFER_NVPARAM_GEOMETRY,
  LUXGL_BUFFER_NVPARAM_TESSCTRL,
  LUXGL_BUFFER_NVPARAM_TESSEVAL,
};


static LUX_INLINE void lxGFXBuffer_applyDefault(lxGFXContextPTR ctx, lxGFXBufferPTR buffer)
{
  if (buffer->vgl.target == LUXGL_BUFFER_VERTEX){
    lxGFXContext_resetVertexStreams(ctx);
  }
  glBindBuffer(buffer->vgl.target,buffer->vgl.id);
}

//////////////////////////////////////////////////////////////////////////
// lxGFXBuffer

LUX_API void lxGFXBuffer_deinit(lxGFXContextPTR ctx, lxGFXBufferPTR buffer)
{

  if (buffer->mapped){
    lxGFXBuffer_applyDefault(ctx,buffer);
    glUnmapBuffer(buffer->vgl.target);
  }

  glDeleteBuffers(1,&buffer->vgl.id);

  buffer->mapped = NULL;
  buffer->vgl.id = 0;
}


LUX_API void lxGFXBuffer_reset(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, void *data)
{
  static const GLenum hinttoGL[LUXGFX_BUFFERHINTS] = {
    GL_STREAM_DRAW,
    GL_STREAM_READ,
    GL_STREAM_COPY,
    GL_STATIC_DRAW,
    GL_STATIC_READ,
    GL_STATIC_COPY,
    GL_DYNAMIC_DRAW,
    GL_DYNAMIC_READ,
    GL_DYNAMIC_COPY,
  };
  void* buf = NULL;

  lxGFXBuffer_applyDefault(ctx,buffer);
  glBufferData(buffer->vgl.target,buffer->size,data,hinttoGL[buffer->hint]);

  buffer->used = data ? buffer->size : 0;
}

LUX_API void lxGFXBuffer_init(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, lxGLBufferTarget_t type, lxGFXBufferHint_t hint, uint size, void* data )
{
//  LUX_ASSERT(buffer->glID == 0);

  buffer->ctx = ctx;
  buffer->vgl.target = type;
  buffer->hint = hint;
  buffer->mapped = NULL;
  buffer->size = size;
  buffer->used = 0;

  glGenBuffers(1,&buffer->vgl.id);
  lxGFXBuffer_reset(ctx,buffer,data);
}

LUX_API uint lxGFXBuffer_alloc(lxGFXBufferPTR buffer, uint size, uint padsize)
{ 
  // pad to biggest 
  uint oldused = buffer->used;
  uint oldsize = size;
  uint offset = (size%padsize);
  size += offset ? (padsize-offset) : 0;

  if (buffer->used + size > buffer->size)
    return (uint)-1;

  buffer->used += size;


  return oldused;
}

LUX_API booln lxGFXBuffer_map(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, void** ptr, lxGFXBufferMapType_t type)
{
  static const GLenum typetoGL[LUXGFX_BUFFERMAPS] = {
    GL_READ_ONLY,
    GL_WRITE_ONLY,
    GL_READ_WRITE,
    GL_WRITE_ONLY,
    GL_WRITE_ONLY,
  };

  if(buffer->mapped) return LUX_FALSE;

  lxGFXBuffer_applyDefault(ctx,buffer);

  if (type >= LUXGFX_BUFFERMAP_WRITEDISCARD && ctx->capbits){
    buffer->mapped = glMapBufferRange(buffer->vgl.target,0,buffer->size,GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  }
  else{
    buffer->mapped = glMapBuffer(buffer->vgl.target,typetoGL[type]);
  }


  buffer->mapstart = 0;
  buffer->maplength = buffer->size;
  buffer->maptype = type;

  *ptr = buffer->mapped;
  return LUX_TRUE;
}

LUX_API booln lxGFXBuffer_copy(lxGFXContextPTR ctx, lxGFXBufferPTR dst, uint dstoffset, lxGFXBufferPTR src, uint srcoffset, uint size)
{
  if (LUXGFX_VALIDITY && !(
    (!dst->mapped && !src->mapped) &&
    (dstoffset+size <= dst->size) &&
    (srcoffset+size <= src->size) &&
    (src != dst) ||
    ((src == dst) && (dstoffset+size <= srcoffset || dstoffset >= srcoffset+size))
    ))
    return LUX_FALSE;

  if (ctx->capbits & LUXGFX_CAP_BUFCOPY){
    lxGFXBuffer_apply(ctx,LUXGL_BUFFER_CPYWRITE,dst);
    lxGFXBuffer_apply(ctx,LUXGL_BUFFER_CPYREAD,src);

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
      srcoffset, dstoffset,
      size);
  }
  else if ((ctx->capbits & LUXGFX_CAP_BUFMAPRANGE) || src != dst){
    // temporarily map both
    void* psrc;
    void* pdst;
    lxGFXBuffer_mapRange(ctx,src,&psrc,srcoffset,size,LUXGFX_BUFFERMAP_READ,LUX_FALSE,LUX_FALSE);
    lxGFXBuffer_mapRange(ctx,dst,&pdst,dstoffset,size,LUXGFX_BUFFERMAP_WRITEDISCARD,LUX_FALSE,LUX_FALSE);

    memcpy(pdst,psrc,size);

    // unmap
    lxGFXBuffer_unmap(ctx,src);
    lxGFXBuffer_unmap(ctx,dst);
  }
  else{
    // temporarily map
    byte* p;
    booln succ = lxGFXBuffer_map(ctx,src,(void**)&p,LUXGFX_BUFFERMAP_READWRITE);
    byte* psrc = p+srcoffset;
    byte* pdst = p+dstoffset;

    memcpy(pdst,psrc,size);

    // unmap
    lxGFXBuffer_unmap(ctx,src);
  }

  return LUX_TRUE;
}

LUX_API booln lxGFXBuffer_submit(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, uint offset, uint size, void *data)
{
  if (LUXGFX_VALIDITY &&(buffer->mapped || offset+size > buffer->size))
    return LUX_FALSE;

  lxGFXBuffer_applyDefault(ctx,buffer);
  glBufferSubDataARB(buffer->vgl.target,offset,size,data);
  
  return LUX_TRUE;
}

LUX_API booln lxGFXBuffer_retrieve(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, uint offset, uint size, void *data)
{
  if (LUXGFX_VALIDITY && (buffer->mapped || offset+size > buffer->size))
    return LUX_FALSE;

  lxGFXBuffer_applyDefault(ctx,buffer);
  glGetBufferSubDataARB(buffer->vgl.target,offset,size,data);

  return LUX_TRUE;
}

LUX_API booln lxGFXBuffer_mapRange(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, void**ptr, lxGFXBufferMapType_t type, uint from, uint length,  booln manualflush, booln unsynch)
{
  if (LUXGFX_VALIDITY && (buffer->mapped || from+length > buffer->size))
    return LUX_FALSE;

  lxGFXBuffer_applyDefault(ctx,buffer);

  if ((ctx->capbits & LUXGFX_CAP_BUFMAPRANGE))
  {
    static const GLbitfield bitfieldsGL[LUXGFX_BUFFERMAPS] = {
      GL_MAP_READ_BIT,
      GL_MAP_WRITE_BIT,
      GL_MAP_READ_BIT | GL_MAP_WRITE_BIT,
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT,
    };

    GLbitfield bitfield = bitfieldsGL[type];
    bitfield |= (manualflush ? GL_MAP_FLUSH_EXPLICIT_BIT : 0) | (unsynch ? GL_MAP_UNSYNCHRONIZED_BIT : 0);

    buffer->mapped = glMapBufferRange(buffer->vgl.target,from,length,bitfield);
  }
  else{
    static const GLenum typetoGL[LUXGFX_BUFFERMAPS] = {
      GL_READ_ONLY,
      GL_WRITE_ONLY,
      GL_READ_WRITE,
      GL_WRITE_ONLY,
      GL_WRITE_ONLY,
    };

    buffer->mapped = ((byte*)glMapBuffer(buffer->vgl.target,typetoGL[type])) + from;
  }

  buffer->mapstart = from;
  buffer->maplength = length;
  buffer->maptype = type;


  *ptr = buffer->mapped;

  return LUX_TRUE;
}

LUX_API booln lxGFXBuffer_flushRange(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, uint from, uint length)
{
  if (!buffer->mapped) return LUX_FALSE;

  lxGFXBuffer_applyDefault(ctx,buffer);
  glFlushMappedBufferRange(buffer->vgl.target,from,length);

  return LUX_TRUE;
}
LUX_API booln lxGFXBuffer_unmap(lxGFXContextPTR ctx, lxGFXBufferPTR buffer)
{
  if(!buffer->mapped) return LUX_FALSE;

  lxGFXBuffer_applyDefault(ctx,buffer);
  glUnmapBuffer(buffer->vgl.target);

  buffer->maplength = buffer->mapstart = 0;
  buffer->mapped = NULL;
  buffer->maptype = LUXGFX_BUFFERMAPS;

  return LUX_TRUE;
}

