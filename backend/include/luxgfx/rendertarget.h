// Copyright (C) 2010-2011 Christoph Kubisch
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

  typedef struct lxgFrameBounds_s{
    int               width;
    int               height;
  }lxgFrameBounds_t;

  typedef struct lxgViewDepth_s{
    double            near;
    double            far;
  }lxgViewDepth_t;

  typedef struct lxgViewPort_s{
    booln             scissor;
    lxRectanglei_t    scissorRect;
    lxRectanglei_t    viewRect;
    lxgViewDepth_t    depth;
  }lxgViewPort_t;

  typedef struct lxgViewPortMrt_s{
    uint              numused;
    flags32           scissored;

    lxRectanglef_t    bounds[LUXGFX_MAX_RENDERTARGETS];
    lxRectanglei_t    scissors[LUXGFX_MAX_RENDERTARGETS];
    lxgViewDepth_t    depths[LUXGFX_MAX_RENDERTARGETS];
  }lxgViewPortMrt_t;

  typedef enum lxgRenderTargetType_e{
    LUXGFX_RENDERTARGET_DRAW,
    LUXGFX_RENDERTARGET_READ,
    LUXGFX_RENDERTARGETS
  }lxgRenderTargetType_t;

  typedef struct lxgRenderAssign_s{
    lxgTexturePTR         tex;
    lxgRenderBufferPTR    rbuf;
    uint                  mip;
    uint                  layer;
  }lxgRenderAssign_t;

  typedef enum lxgRenderAssignType_e{
    LUXGFX_RENDERASSIGN_DEPTH,
    LUXGFX_RENDERASSIGN_STENCIL,
    LUXGFX_RENDERASSIGN_COLOR0,
    LUXGFX_RENDERASSIGNS = LUXGFX_RENDERASSIGN_COLOR0 + LUXGFX_MAX_RENDERTARGETS,
  }lxgRenderAssignType_t;

  typedef struct lxgRenderTarget_s{
    GLuint              glid;
    lxgContextPTR       ctx;

    uint                maxidx;
    flags32             dirty;
        
    lxgRenderAssign_t   assigns[LUXGFX_RENDERASSIGNS];

    booln               equalsized;
    lxgFrameBounds_t    bounds;
  }lxgRenderTarget_t;

  typedef struct lxgRenderTargetBlit_s{
    lxVec2i_t           fromStart;
    lxVec2i_t           fromEnd;
    lxVec2i_t           toStart;
    lxVec2i_t           toEnd;
  }lxgRenderTargetBlit_t;
  typedef struct lxgRenderTargetBlit_s* lxgRenderTargetBlitPTR;

  //////////////////////////////////////////////////////////////////////////

  LUX_API void lxgRenderTarget_init(lxgRenderTargetPTR rt, lxgContextPTR ctx);
  LUX_API void lxgRenderTarget_deinit(lxgRenderTargetPTR rt, lxgContextPTR ctx);

  LUX_API void lxgRenderTarget_applyAssigns(lxgRenderTargetPTR rt, lxgRenderTargetType_t mode);

  LUX_API void  lxgRenderTarget_setAssign(lxgRenderTargetPTR rt, uint assigntype, lxgRenderAssignPTR assign);
  LUX_API booln lxgRenderTarget_checkSize(lxgRenderTargetPTR rt);
  LUX_API lxgFrameBoundsCPTR lxgRenderTarget_getBounds(lxgRenderTargetPTR rt);

  //////////////////////////////////////////////////////////////////////////

  LUX_API void  lxgViewPort_sync(lxgViewPortPTR obj, lxgContextPTR ctx);
  LUX_API void  lxgViewPortMrt_sync(lxgViewPortMrtPTR obj, lxgContextPTR ctx);

  //////////////////////////////////////////////////////////////////////////

    // rts may be NULL
  LUX_API void  lxgContext_applyRenderTarget( lxgContextPTR ctx, lxgRenderTargetPTR obj, lxgRenderTargetType_t type);
  LUX_API void  lxgContext_applyRenderTargetDraw( lxgContextPTR ctx, lxgRenderTargetPTR obj, booln setViewport);
  LUX_API void  lxgContext_blitRenderTargets(lxgContextPTR ctx, lxgRenderTargetPTR to, lxgRenderTargetPTR from, 
    lxgRenderTargetBlitPTR update, flags32 mask, booln linearFilter);

  // return the actual scissor state (can be enabled indirectly through viewport != framebounds)
  LUX_API booln lxgContext_applyViewPortRect(lxgContextPTR ctx, lxRectangleiCPTR rect);
  LUX_API booln lxgContext_applyViewPortScissorState(lxgContextPTR ctx, booln state);
  LUX_API booln lxgContext_applyViewPort(lxgContextPTR ctx, lxgViewPortPTR obj);

  LUX_API void  lxgContext_applyViewPortMrt(lxgContextPTR ctx, lxgViewPortMrtPTR obj);
  



#ifdef __cplusplus
}
#endif


#endif