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
  
  typedef enum lxGFXBufferHint_e{
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
    LUXGFX_BUFFERHINT_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXBufferHint_t;

  typedef enum lxGFXBufferMapType_e{
    LUXGFX_BUFFERMAP_READ,
    LUXGFX_BUFFERMAP_WRITE,
    LUXGFX_BUFFERMAP_READWRITE,

    LUXGFX_BUFFERMAP_WRITEDISCARD,
    LUXGFX_BUFFERMAP_WRITEDISCARDALL,

    LUXGFX_BUFFERMAPS,
    LUXGFX_BUFFERMAP_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXBufferMapType_t;

  typedef struct lxGFXBuffer_s{
    lxGLBuffer_t      vgl;
    lxGFXContextPTR   ctx;
    void*             user;

    void*                 mapped;
    lxGFXBufferMapType_t  maptype;
    uint                  mapstart;
    uint                  maplength;
        
    uint                size;
    uint                used;
    lxGFXBufferHint_t   hint;
  }lxGFXBuffer_t;


  // raises used and returns offset withn padsize from start or -1 on error
  LUX_API uint lxGFXBuffer_alloc(lxGFXBufferPTR buffer, uint needed, uint padsize);

  LUX_API void lxGFXBuffer_apply(lxGFXContextPTR ctx, lxGLBufferTarget_t type, const lxGFXBufferPTR buffer);
  LUX_API void  lxGFXBuffer_applyIndexed(lxGFXContextPTR ctx, lxGLBufferTarget_t type, uint idx, const lxGFXBufferPTR buffer);
  LUX_API void  lxGFXBuffer_applyRanged(lxGFXContextPTR ctx,  lxGLBufferTarget_t type, uint idx, const lxGFXBufferPTR buffer, size_t offset, size_t size);

  // implicitly call lxGFXBuffer_setGL with type used on init
  LUX_API booln lxGFXBuffer_map(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, void**ptr, lxGFXBufferMapType_t type);
  LUX_API booln lxGFXBuffer_mapRange(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, void**ptr, uint from, uint length, lxGFXBufferMapType_t type, booln manualflush, booln unsynch);
  LUX_API booln lxGFXBuffer_flushRange(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, uint from, uint length);
  LUX_API booln lxGFXBuffer_unmap(lxGFXContextPTR ctx, lxGFXBufferPTR buffer);
  LUX_API booln lxGFXBuffer_copy(lxGFXContextPTR ctx, lxGFXBufferPTR dst, uint dstoffset, lxGFXBufferPTR src, uint srcoffset, uint size);

  LUX_API void lxGFXBuffer_deinit(lxGFXContextPTR ctx, lxGFXBufferPTR buffer);
  LUX_API void lxGFXBuffer_reset(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, void* data);
  LUX_API void lxGFXBuffer_init(lxGFXContextPTR ctx, lxGFXBufferPTR buffer, lxGFXBufferHint_t hint, uint size, lxGLBufferTarget_t type, void* data);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE void lxGFXBuffer_apply( lxGFXContextPTR ctx, lxGLBufferTarget_t type, const lxGFXBufferPTR buffer )
  {
    glBindBuffer(type,buffer ? buffer->vgl.id : 0);
  }

  LUX_INLINE void  lxGFXBuffer_applyIndexed(lxGFXContextPTR ctx, lxGLBufferTarget_t type, uint idx, const lxGFXBufferPTR buffer)
  {
    glBindBufferBase(type,idx,buffer ? buffer->vgl.id : 0);
  }

  LUX_INLINE void  lxGFXBuffer_applyRanged(lxGFXContextPTR ctx, lxGLBufferTarget_t type, uint idx, const lxGFXBufferPTR buffer, size_t offset, size_t size)
  {
    LUX_DEBUGASSERT(offset + size < buffer->size);
    glBindBufferRange(type,idx,buffer ? buffer->vgl.id : 0, offset, size);
  }

#ifdef __cplusplus
}
#endif


#endif
