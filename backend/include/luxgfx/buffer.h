// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGL_BUFFER_H__
#define __LUXLUXGL_BUFFER_H__

#include <luxplatform/luxplatform.h>
#include <luxplatform/debug.h>

#include "glconfig.h"

#ifdef __cplusplus
extern "C"{
#endif
  
  typedef enum lxgBufferMode_e{
    LUXGFX_BUFFERMODE_DRAW,
    LUXGFX_BUFFERMODE_READ,
    LUXGFX_BUFFERMODE_COPY,
    LUXGFX_BUFFERMODES,
  }lxgBufferMode_t;

  typedef enum lxgBufferUpdate_e{
    LUXGFX_BUFFERUPDATE_STATIC,
    LUXGFX_BUFFERUPDATE_DYNAMIC,
    LUXGFX_BUFFERUPDATE_STREAM,
    LUXGFX_BUFFERUPDATES,
  }lxgBufferUpdate_t;

  typedef struct lxgBuffer_s{
    lxGLBufferTarget_t    gltarget;
    GLuint                glid;
    GLuint64              address;
    lxgContextPTR         ctx;
    void*                 user;

    void*                 mapped;
    lxgAccessMode_t       maptype;
    uint                  mapstart;
    uint                  maplength;
        
    uint                  size;
    uint                  used;
    lxgBufferMode_t       mode;
    lxgBufferUpdate_t     update;
  }lxgBuffer_t;


  // raises used and returns offset withn padsize from start or -1 on error
  LUX_API uint lxgBuffer_alloc(lxgBufferPTR buffer, uint needed, uint padsize);

  LUX_API void lxgBuffer_bind(lxgBufferPTR buffer, lxGLBufferTarget_t type);
  LUX_API void lxgBuffer_bindIndexed(lxgBufferPTR buffer, lxGLBufferTarget_t type, uint idx);
  LUX_API void lxgBuffer_bindRanged(lxgBufferPTR buffer, lxGLBufferTarget_t type, uint idx, size_t offset, size_t size);

  // implicitly call lxgBuffer_setGL with type used on init
  LUX_API booln lxgBuffer_map(lxgBufferPTR buffer, void**ptr, lxgAccessMode_t type);
  LUX_API booln lxgBuffer_mapRange(lxgBufferPTR buffer, void**ptr, uint from, uint length, lxgAccessMode_t type, booln manualflush, booln unsynch);
  LUX_API booln lxgBuffer_flushRange(lxgBufferPTR buffer, uint from, uint length);
  LUX_API booln lxgBuffer_unmap(lxgBufferPTR buffer);
  LUX_API booln lxgBuffer_copy(lxgBufferPTR buffer, uint bufferoffset, lxgBufferPTR src, uint srcoffset, uint size);

  LUX_API GLuint64 lxgBuffer_addressNV(lxgBufferPTR buffer);
  LUX_API void lxgBuffer_residentNV(lxgBufferPTR buffer, lxgAccessMode_t mode);
  LUX_API void lxgBuffer_unresidentNV(lxgBufferPTR buffer);

  LUX_API void lxgBuffer_deinit(lxgBufferPTR buffer, lxgContextPTR ctx);
  LUX_API void lxgBuffer_reset(lxgBufferPTR buffer, void* data);
  LUX_API void lxgBuffer_init(lxgBufferPTR buffer, lxgContextPTR ctx, lxgBufferMode_t hint, lxgBufferUpdate_t update, uint size, void* data);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE void lxgBuffer_bind(lxgBufferPTR buffer, lxGLBufferTarget_t type )
  {
    glBindBuffer(type,buffer ? buffer->glid : 0);
  }

  LUX_INLINE void  lxgBuffer_bindIndexed(lxgBufferPTR buffer, lxGLBufferTarget_t type, uint idx )
  {
    glBindBufferBase(type,idx,buffer ? buffer->glid : 0);
  }

  LUX_INLINE void  lxgBuffer_bindRanged( lxgBufferPTR buffer, lxGLBufferTarget_t type, uint idx, size_t offset, size_t size)
  {
    LUX_DEBUGASSERT(offset + size < buffer->size);
    glBindBufferRange(type,idx,buffer ? buffer->glid : 0, offset, size);
  }

  LUX_INLINE void lxgBuffer_unresidentNV(lxgBufferPTR buffer){
    glMakeNamedBufferNonResidentNV(buffer->glid);
  }

#ifdef __cplusplus
}
#endif


#endif
