// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_RENDERTARGET_H__
#define __LUXLUXGFX_RENDERTARGET_H__

#include <luxplatform/luxplatform.h>
#include <luxmath/basetypes.h>

#include "glconfig.h"

#ifdef __cplusplus
extern "C"{
#endif

  typedef struct lxGFXFrameBounds_s{
    int               width;
    int               height;
  }lxGFXFrameBounds_t;

  typedef struct lxGFXViewDepth_s{
    double            near;
    double            far;
  }lxGFXViewDepth_t;

  typedef struct lxGFXViewPort_s{
    booln             scissor;
    lxRectanglei_t    scissorRect;
    lxRectanglei_t    viewRect;
    lxGFXViewDepth_t  depth;
  }lxGFXViewPort_t;

  typedef struct lxGFXViewPortMrt_s{
    uint              numused;
    flags32           scissored;

    lxRectanglef_t    bounds[LUXGFX_MAX_RENDERTARGETS];
    lxRectanglei_t    scissors[LUXGFX_MAX_RENDERTARGETS];
    lxGFXViewDepth_t  depths[LUXGFX_MAX_RENDERTARGETS];
  }lxGFXViewPortMrt_t;

  typedef enum lxGFXRenderTargetType_e{
    LUXGFX_RENDERTARGET_DRAW,
    LUXGFX_RENDERTARGET_READ,
    LUXGFX_RENDERTARGETS
  }lxGFXRenderTargetType_t;

  typedef struct lxGFXRenderAssign_s{
    lxGFXTexturePTR       tex;
    lxGFXRenderBufferPTR  rbuf;
    uint                  mip;
    uint                  layer;
  }lxGFXRenderAssign_t;

  typedef enum lxGFXRenderAssignType_e{
    LUXGFX_RENDERASSIGN_DEPTH,
    LUXGFX_RENDERASSIGN_STENCIL,
    LUXGFX_RENDERASSIGN_COLOR0,
    LUXGFX_RENDERASSIGNS = LUXGFX_RENDERASSIGN_COLOR0 + LUXGFX_MAX_RENDERTARGETS,
  }lxGFXRenderAssignType_t;

  typedef struct lxGFXRenderTarget_s{
    lxGLFrameBuffer_t vgl;
    lxGFXContextPTR   ctx;

    uint          maxidx;
    flags32       dirty;
        
    lxGFXRenderAssign_t assigns[LUXGFX_RENDERASSIGNS];

    booln               equalsized;
    lxGFXFrameBounds_t  bounds;
  }lxGFXRenderTarget_t;

  typedef struct lxGFXRenderTargetBlit_s{
    lxVec2i_t   fromStart;
    lxVec2i_t   fromEnd;
    lxVec2i_t   toStart;
    lxVec2i_t   toEnd;
  }lxGFXRenderTargetBlit_t;
  typedef struct lxGFXRenderTargetBlit_s* lxGFXRenderTargetBlitPTR;

  LUX_API void lxGFXRenderTarget_init(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt);
  LUX_API void lxGFXRenderTarget_deinit(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt);

  LUX_API void lxGFXRenderTarget_applyAssigns(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt, lxGFXRenderTargetType_t mode);


  // to and from may be NULL
  LUX_API void lxGFXRenderTarget_blit(lxGFXContextPTR ctx, lxGFXRenderTargetPTR to, lxGFXRenderTargetPTR from, lxGFXRenderTargetBlitPTR update, flags32 mask, booln linearFilter);

  LUX_API void lxGFXRenderTarget_setAssign(lxGFXRenderTargetPTR rt, uint assigntype, lxGFXRenderAssignPTR assign);
  LUX_API booln lxGFXRenderTarget_checkSize(lxGFXRenderTargetPTR rt);
  LUX_API const lxGFXFrameBoundsPTR lxGFXRenderTarget_getBounds(lxGFXRenderTargetPTR rt);


  // return the actual scissor state (can be enabled indirectly through viewport != framebounds)
  LUX_API booln lxGFXViewPortRect_apply(lxGFXContextPTR ctx, const lxRectangleiPTR rect);
  LUX_API booln lxGFXViewPortScissor_applyState(lxGFXContextPTR ctx, booln state);
  LUX_API booln lxGFXViewPort_apply(lxGFXContextPTR ctx, const lxGFXViewPortPTR obj);
  LUX_API void  lxGFXViewPort_sync(lxGFXContextPTR ctx, lxGFXViewPortPTR obj);

  LUX_API void  lxGFXViewPortMrt_apply(lxGFXContextPTR ctx, const lxGFXViewPortMrtPTR obj);
  LUX_API void  lxGFXViewPortMrt_sync(lxGFXContextPTR ctx, lxGFXViewPortMrtPTR obj);



#ifdef __cplusplus
}
#endif


#endif