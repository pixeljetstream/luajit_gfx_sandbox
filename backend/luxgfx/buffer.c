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


static LUX_INLINE void lxgBuffer_applyDefault(lxgContextPTR ctx, lxgBufferPTR buffer)
{
  if (buffer->vgl.target == LUXGL_BUFFER_VERTEX){
    lxgContext_resetVertexStreams(ctx);
  }
  glBindBuffer(buffer->vgl.target,buffer->vgl.id);
}

//////////////////////////////////////////////////////////////////////////
// lxgBuffer

LUX_API void lxgBuffer_deinit(lxgContextPTR ctx, lxgBufferPTR buffer)
{

  if (buffer->mapped){
    lxgBuffer_applyDefault(ctx,buffer);
    glUnmapBuffer(buffer->vgl.target);
  }

  glDeleteBuffers(1,&buffer->vgl.id);

  buffer->mapped = NULL;
  buffer->vgl.id = 0;
}


LUX_API void lxgBuffer_reset(lxgContextPTR ctx, lxgBufferPTR buffer, void *data)
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

  lxgBuffer_applyDefault(ctx,buffer);
  glBufferData(buffer->vgl.target,buffer->size,data,hinttoGL[buffer->hint]);

  buffer->used = data ? buffer->size : 0;
}

LUX_API void lxgBuffer_init(lxgContextPTR ctx, lxgBufferPTR buffer, lxGLBufferTarget_t type, lxgBufferHint_t hint, uint size, void* data )
{
//  LUX_ASSERT(buffer->glID == 0);

  buffer->ctx = ctx;
  buffer->vgl.target = type;
  buffer->hint = hint;
  buffer->mapped = NULL;
  buffer->size = size;
  buffer->used = 0;

  glGenBuffers(1,&buffer->vgl.id);
  lxgBuffer_reset(ctx,buffer,data);
}

LUX_API uint lxgBuffer_alloc(lxgBufferPTR buffer, uint size, uint padsize)
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

LUX_API booln lxgBuffer_map(lxgContextPTR ctx, lxgBufferPTR buffer, void** ptr, lxgAccessMode_t type)
{
  static const GLenum typetoGL[LUXGFX_ACCESSS] = {
    GL_READ_ONLY,
    GL_WRITE_ONLY,
    GL_READ_WRITE,
    GL_WRITE_ONLY,
    GL_WRITE_ONLY,
  };

  if(buffer->mapped) return LUX_FALSE;

  lxgBuffer_applyDefault(ctx,buffer);

  if (type >= LUXGFX_ACCESS_WRITEDISCARD && ctx->capbits){
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

LUX_API booln lxgBuffer_copy(lxgContextPTR ctx, lxgBufferPTR dst, uint dstoffset, lxgBufferPTR src, uint srcoffset, uint size)
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
    lxgBuffer_apply(ctx,LUXGL_BUFFER_CPYWRITE,dst);
    lxgBuffer_apply(ctx,LUXGL_BUFFER_CPYREAD,src);

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
      srcoffset, dstoffset,
      size);
  }
  else if ((ctx->capbits & LUXGFX_CAP_BUFMAPRANGE) || src != dst){
    // temporarily map both
    void* psrc;
    void* pdst;
    lxgBuffer_mapRange(ctx,src,&psrc,srcoffset,size,LUXGFX_ACCESS_READ,LUX_FALSE,LUX_FALSE);
    lxgBuffer_mapRange(ctx,dst,&pdst,dstoffset,size,LUXGFX_ACCESS_WRITEDISCARD,LUX_FALSE,LUX_FALSE);

    memcpy(pdst,psrc,size);

    // unmap
    lxgBuffer_unmap(ctx,src);
    lxgBuffer_unmap(ctx,dst);
  }
  else{
    // temporarily map
    byte* p;
    booln succ = lxgBuffer_map(ctx,src,(void**)&p,LUXGFX_ACCESS_READWRITE);
    byte* psrc = p+srcoffset;
    byte* pdst = p+dstoffset;

    memcpy(pdst,psrc,size);

    // unmap
    lxgBuffer_unmap(ctx,src);
  }

  return LUX_TRUE;
}

LUX_API booln lxgBuffer_submit(lxgContextPTR ctx, lxgBufferPTR buffer, uint offset, uint size, void *data)
{
  if (LUXGFX_VALIDITY &&(buffer->mapped || offset+size > buffer->size))
    return LUX_FALSE;

  lxgBuffer_applyDefault(ctx,buffer);
  glBufferSubDataARB(buffer->vgl.target,offset,size,data);
  
  return LUX_TRUE;
}

LUX_API booln lxgBuffer_retrieve(lxgContextPTR ctx, lxgBufferPTR buffer, uint offset, uint size, void *data)
{
  if (LUXGFX_VALIDITY && (buffer->mapped || offset+size > buffer->size))
    return LUX_FALSE;

  lxgBuffer_applyDefault(ctx,buffer);
  glGetBufferSubDataARB(buffer->vgl.target,offset,size,data);

  return LUX_TRUE;
}

LUX_API booln lxgBuffer_mapRange(lxgContextPTR ctx, lxgBufferPTR buffer, void**ptr, lxgAccessMode_t type, uint from, uint length,  booln manualflush, booln unsynch)
{
  if (LUXGFX_VALIDITY && (buffer->mapped || from+length > buffer->size))
    return LUX_FALSE;

  lxgBuffer_applyDefault(ctx,buffer);

  if ((ctx->capbits & LUXGFX_CAP_BUFMAPRANGE))
  {
    static const GLbitfield bitfieldsGL[LUXGFX_ACCESSS] = {
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
    static const GLenum typetoGL[LUXGFX_ACCESSS] = {
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

LUX_API booln lxgBuffer_flushRange(lxgContextPTR ctx, lxgBufferPTR buffer, uint from, uint length)
{
  if (!buffer->mapped) return LUX_FALSE;

  lxgBuffer_applyDefault(ctx,buffer);
  glFlushMappedBufferRange(buffer->vgl.target,from,length);

  return LUX_TRUE;
}
LUX_API booln lxgBuffer_unmap(lxgContextPTR ctx, lxgBufferPTR buffer)
{
  if(!buffer->mapped) return LUX_FALSE;

  lxgBuffer_applyDefault(ctx,buffer);
  glUnmapBuffer(buffer->vgl.target);

  buffer->maplength = buffer->mapstart = 0;
  buffer->mapped = NULL;
  buffer->maptype = LUXGFX_ACCESSS;

  return LUX_TRUE;
}

