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

static lxGLBufferTarget_t l_default = LUXGL_BUFFER_VERTEX;
static LUX_INLINE void lxgBuffer_bindDefault(lxgBufferPTR buffer)
{
  glBindBuffer(buffer->gltarget,buffer->glid);
}

//////////////////////////////////////////////////////////////////////////
// lxgBuffer

LUX_API void lxgBuffer_deinit(lxgBufferPTR buffer, lxgContextPTR ctx)
{

  if (buffer->mapped){
    lxgBuffer_bindDefault(buffer);
    glUnmapBuffer(buffer->gltarget);
  }

  glDeleteBuffers(1,&buffer->glid);

  buffer->mapped = NULL;
  buffer->glid = 0;
}


LUX_API void lxgBuffer_reset(lxgBufferPTR buffer, void *data)
{
  static const GLenum hinttoGL[] = {
    GL_STATIC_DRAW,
    GL_STATIC_READ,
    GL_STATIC_COPY,
    GL_DYNAMIC_DRAW,
    GL_DYNAMIC_READ,
    GL_DYNAMIC_COPY,
    GL_STREAM_DRAW,
    GL_STREAM_READ,
    GL_STREAM_COPY,
  };
  int index = (buffer->update * 3) + buffer->mode;
  LUX_DEBUGASSERT(index >= 0 && index < 9);

  lxgBuffer_bindDefault(buffer);
  glBufferData(buffer->gltarget,buffer->size,data,hinttoGL[index]);

  buffer->used = data ? buffer->size : 0;
}

LUX_API void lxgBuffer_init(lxgBufferPTR buffer, lxgContextPTR ctx, lxgBufferMode_t mode, lxgBufferUpdate_t update, uint size, void* data )
{
//  LUX_ASSERT(buffer->glID == 0);

  buffer->ctx = ctx;
  buffer->gltarget = l_default;
  buffer->mode = mode;
  buffer->update = update;
  buffer->mapped = NULL;
  buffer->size = size;
  buffer->used = 0;

  glGenBuffers(1,&buffer->glid);
  lxgBuffer_reset(buffer,data);
}

LUX_API GLuint64 lxgBuffer_addressNV(lxgBufferPTR buffer)
{
  lxgBuffer_bindDefault(buffer);
  glGetBufferParameterui64vNV(buffer->gltarget,GL_BUFFER_GPU_ADDRESS_NV,&buffer->address);
  return buffer->address;
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

LUX_API booln lxgBuffer_map(lxgBufferPTR buffer, void** ptr, lxgAccessMode_t type)
{
  static const GLenum typetoGL[LUXGFX_ACCESSS] = {
    GL_READ_ONLY,
    GL_WRITE_ONLY,
    GL_READ_WRITE,
    GL_WRITE_ONLY,
    GL_WRITE_ONLY,
  };

  if(buffer->mapped) return LUX_FALSE;

  lxgBuffer_bindDefault(buffer);

  if (type >= LUXGFX_ACCESS_WRITEDISCARD && buffer->ctx->capbits){
    buffer->mapped = glMapBufferRange(buffer->gltarget,0,buffer->size,GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  }
  else{
    buffer->mapped = glMapBuffer(buffer->gltarget,typetoGL[type]);
  }


  buffer->mapstart = 0;
  buffer->maplength = buffer->size;
  buffer->maptype = type;

  *ptr = buffer->mapped;
  return LUX_TRUE;
}

LUX_API booln lxgBuffer_copy(lxgBufferPTR dst, uint dstoffset, lxgBufferPTR src, uint srcoffset, uint size)
{
  if (LUXGFX_VALIDITY && !(
    (!dst->mapped && !src->mapped) &&
    (dstoffset+size <= dst->size) &&
    (srcoffset+size <= src->size) &&
    (src != dst) ||
    ((src == dst) && (dstoffset+size <= srcoffset || dstoffset >= srcoffset+size))
    ))
    return LUX_FALSE;

  if (dst->ctx->capbits & LUXGFX_CAP_BUFCOPY){
    lxgBuffer_bind(dst,LUXGL_BUFFER_CPYWRITE);
    lxgBuffer_bind(src,LUXGL_BUFFER_CPYREAD);

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
      srcoffset, dstoffset,
      size);
  }
  else if ((dst->ctx->capbits & LUXGFX_CAP_BUFMAPRANGE) || src != dst){
    // temporarily map both
    void* psrc;
    void* pdst;
    lxgBuffer_mapRange(src,&psrc,srcoffset,size,LUXGFX_ACCESS_READ,LUX_FALSE,LUX_FALSE);
    lxgBuffer_mapRange(dst,&pdst,dstoffset,size,LUXGFX_ACCESS_WRITEDISCARD,LUX_FALSE,LUX_FALSE);

    memcpy(pdst,psrc,size);

    // unmap
    lxgBuffer_unmap(src);
    lxgBuffer_unmap(dst);
  }
  else{
    // temporarily map
    byte* p;
    booln succ = lxgBuffer_map(src,(void**)&p,LUXGFX_ACCESS_READWRITE);
    byte* psrc = p+srcoffset;
    byte* pdst = p+dstoffset;

    memcpy(pdst,psrc,size);

    // unmap
    lxgBuffer_unmap(src);
  }

  return LUX_TRUE;
}

LUX_API booln lxgBuffer_submit(lxgBufferPTR buffer, uint offset, uint size, void *data)
{
  if (LUXGFX_VALIDITY &&(buffer->mapped || offset+size > buffer->size))
    return LUX_FALSE;

  lxgBuffer_bindDefault(buffer);
  glBufferSubDataARB(buffer->gltarget,offset,size,data);
  
  return LUX_TRUE;
}

LUX_API booln lxgBuffer_retrieve(lxgBufferPTR buffer, uint offset, uint size, void *data)
{
  if (LUXGFX_VALIDITY && (buffer->mapped || offset+size > buffer->size))
    return LUX_FALSE;

  lxgBuffer_bindDefault(buffer);
  glGetBufferSubDataARB(buffer->gltarget,offset,size,data);

  return LUX_TRUE;
}

LUX_API booln lxgBuffer_mapRange(lxgBufferPTR buffer, void**ptr, lxgAccessMode_t type, uint from, uint length,  booln manualflush, booln unsynch)
{
  if (LUXGFX_VALIDITY && (buffer->mapped || from+length > buffer->size))
    return LUX_FALSE;

  lxgBuffer_bindDefault(buffer);

  if ((buffer->ctx->capbits & LUXGFX_CAP_BUFMAPRANGE))
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

    buffer->mapped = glMapBufferRange(buffer->gltarget,from,length,bitfield);
  }
  else{
    static const GLenum typetoGL[LUXGFX_ACCESSS] = {
      GL_READ_ONLY,
      GL_WRITE_ONLY,
      GL_READ_WRITE,
      GL_WRITE_ONLY,
      GL_WRITE_ONLY,
    };

    buffer->mapped = ((byte*)glMapBuffer(buffer->gltarget,typetoGL[type])) + from;
  }

  buffer->mapstart = from;
  buffer->maplength = length;
  buffer->maptype = type;


  *ptr = buffer->mapped;

  return LUX_TRUE;
}

LUX_API booln lxgBuffer_flushRange(lxgBufferPTR buffer, uint from, uint length)
{
  if (!buffer->mapped) return LUX_FALSE;

  lxgBuffer_bindDefault(buffer);
  glFlushMappedBufferRange(buffer->gltarget,from,length);

  return LUX_TRUE;
}
LUX_API booln lxgBuffer_unmap(lxgBufferPTR buffer)
{
  if(!buffer->mapped) return LUX_FALSE;

  lxgBuffer_bindDefault(buffer);
  glUnmapBuffer(buffer->gltarget);

  buffer->maplength = buffer->mapstart = 0;
  buffer->mapped = NULL;
  buffer->maptype = LUXGFX_ACCESSS;

  return LUX_TRUE;
}

