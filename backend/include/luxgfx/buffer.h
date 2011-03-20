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
  
  typedef enum lxgBufferHint_e{
    LUXGFX_BUFFERHINT_STATIC_DRAW,
    LUXGFX_BUFFERHINT_STATIC_READ,
    LUXGFX_BUFFERHINT_STATIC_COPY,
    LUXGFX_BUFFERHINT_DYNAMIC_DRAW,
    LUXGFX_BUFFERHINT_DYNAMIC_READ,
    LUXGFX_BUFFERHINT_DYNAMIC_COPY,
    LUXGFX_BUFFERHINT_STREAM_DRAW,
    LUXGFX_BUFFERHINT_STREAM_READ,
    LUXGFX_BUFFERHINT_STREAM_COPY,
    LUXGFX_BUFFERHINTS,
  }lxgBufferHint_t;


  typedef struct lxgBuffer_s{
    lxGLBuffer_t          vgl;
    lxgContextPTR         ctx;
    void*                 user;

    void*                 mapped;
    lxgAccessMode_t       maptype;
    uint                  mapstart;
    uint                  maplength;
        
    uint                  size;
    uint                  used;
    lxgBufferHint_t       hint;
  }lxgBuffer_t;


  // raises used and returns offset withn padsize from start or -1 on error
  LUX_API uint lxgBuffer_alloc(lxgBufferPTR buffer, uint needed, uint padsize);

  LUX_API void lxgBuffer_apply(lxgContextPTR ctx, lxGLBufferTarget_t type, const lxgBufferPTR buffer);
  LUX_API void lxgBuffer_applyIndexed(lxgContextPTR ctx, lxGLBufferTarget_t type, uint idx, const lxgBufferPTR buffer);
  LUX_API void lxgBuffer_applyRanged(lxgContextPTR ctx,  lxGLBufferTarget_t type, uint idx, const lxgBufferPTR buffer, size_t offset, size_t size);

  // implicitly call lxgBuffer_setGL with type used on init
  LUX_API booln lxgBuffer_map(lxgContextPTR ctx, lxgBufferPTR buffer, void**ptr, lxgAccessMode_t type);
  LUX_API booln lxgBuffer_mapRange(lxgContextPTR ctx, lxgBufferPTR buffer, void**ptr, uint from, uint length, lxgAccessMode_t type, booln manualflush, booln unsynch);
  LUX_API booln lxgBuffer_flushRange(lxgContextPTR ctx, lxgBufferPTR buffer, uint from, uint length);
  LUX_API booln lxgBuffer_unmap(lxgContextPTR ctx, lxgBufferPTR buffer);
  LUX_API booln lxgBuffer_copy(lxgContextPTR ctx, lxgBufferPTR dst, uint dstoffset, lxgBufferPTR src, uint srcoffset, uint size);

  LUX_API void lxgBuffer_deinit(lxgContextPTR ctx, lxgBufferPTR buffer);
  LUX_API void lxgBuffer_reset(lxgContextPTR ctx, lxgBufferPTR buffer, void* data);
  LUX_API void lxgBuffer_init(lxgContextPTR ctx, lxgBufferPTR buffer, lxgBufferHint_t hint, uint size, lxGLBufferTarget_t type, void* data);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE void lxgBuffer_apply( lxgContextPTR ctx, lxGLBufferTarget_t type, const lxgBufferPTR buffer )
  {
    glBindBuffer(type,buffer ? buffer->vgl.id : 0);
  }

  LUX_INLINE void  lxgBuffer_applyIndexed(lxgContextPTR ctx, lxGLBufferTarget_t type, uint idx, const lxgBufferPTR buffer)
  {
    glBindBufferBase(type,idx,buffer ? buffer->vgl.id : 0);
  }

  LUX_INLINE void  lxgBuffer_applyRanged(lxgContextPTR ctx, lxGLBufferTarget_t type, uint idx, const lxgBufferPTR buffer, size_t offset, size_t size)
  {
    LUX_DEBUGASSERT(offset + size < buffer->size);
    glBindBufferRange(type,idx,buffer ? buffer->vgl.id : 0, offset, size);
  }

#ifdef __cplusplus
}
#endif


#endif
