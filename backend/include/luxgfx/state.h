// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_STATE_H__
#define __LUXLUXGFX_STATE_H__

#include <luxplatform/luxplatform.h>
#include <luxmath/basetypes.h>

#include "glconfig.h"

#ifdef __cplusplus
extern "C"{
#endif

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgDepth_s{
    lxGLCompareMode_t   func;
  }lxgDepth_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgLogic_s{
    lxGLLogicOp_t     op;
  }lxgLogic_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgStencilOp_s
  {
    lxGLStencilMode_t   fail;
    lxGLStencilMode_t   zfail;
    lxGLStencilMode_t   zpass;
    lxGLCompareMode_t   func;
  }lxgStencilOp_t;

  typedef struct lxgStencil_s{
    uint16      refvalue;
    uint16      mask;

    lxgStencilOp_t  ops[2]; // 0 = front 1 = back
  }lxgStencil_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgBlendMode_s{
    lxGLBlendWeight_t srcw;
    lxGLBlendWeight_t dstw;
    lxGLBlendEquation_t equ;
  }lxgBlendMode_t;

  typedef struct lxgBlend_s{
    lxgBlendMode_t    colormode;
    lxgBlendMode_t    alphamode;
  }lxgBlend_t;

  typedef struct lxgBlendMrt_s{
    uint16          individual;
    uint16          numused;
    flags32         enabled;
    lxgBlend_t      blends[LUXGFX_MAX_RENDERTARGETS];
  }lxgBlendMrt_t;

  //////////////////////////////////////////////////////////////////////////

  enum lxgRenderFlag_e {
    LUXGFX_RFLAG_STENCILWRITE1 = 1<<0,
    LUXGFX_RFLAG_STENCILWRITE2 = 1<<1,
    LUXGFX_RFLAG_STENCILWRITE3 = 1<<2,
    LUXGFX_RFLAG_STENCILWRITE4 = 1<<3,
    LUXGFX_RFLAG_STENCILWRITE5 = 1<<4,
    LUXGFX_RFLAG_STENCILWRITE6 = 1<<5,
    LUXGFX_RFLAG_STENCILWRITE7 = 1<<6,
    LUXGFX_RFLAG_STENCILWRITE8 = 1<<7,
    LUXGFX_RFLAG_STENCILWRITE = LUXGFX_RFLAG_STENCILWRITE1 |
                                LUXGFX_RFLAG_STENCILWRITE2 |
                                LUXGFX_RFLAG_STENCILWRITE3 |
                                LUXGFX_RFLAG_STENCILWRITE4 |
                                LUXGFX_RFLAG_STENCILWRITE5 |
                                LUXGFX_RFLAG_STENCILWRITE6 |
                                LUXGFX_RFLAG_STENCILWRITE7 |
                                LUXGFX_RFLAG_STENCILWRITE8,
    LUXGFX_RFLAG_FRONTCULL  =    1<<8,
    LUXGFX_RFLAG_DEPTHWRITE =    1<<9,
    LUXGFX_RFLAG_CCW =          1<<10,
    LUXGFX_RFLAG_CULL =         1<<11,
    LUXGFX_RFLAG_COLORWRITER =  1<<12,
    LUXGFX_RFLAG_COLORWRITEG =  1<<13,
    LUXGFX_RFLAG_COLORWRITEB =  1<<14,
    LUXGFX_RFLAG_COLORWRITEA =  1<<15,
    LUXGFX_RFLAG_COLORWRITE =   LUXGFX_RFLAG_COLORWRITER |
                                LUXGFX_RFLAG_COLORWRITEG |
                                LUXGFX_RFLAG_COLORWRITEB |
                                LUXGFX_RFLAG_COLORWRITEA,
    LUXGFX_RFLAG_BLEND =        1<<16,
    LUXGFX_RFLAG_STENCILTEST =  1<<17,
    LUXGFX_RFLAG_LOGIC =        1<<18,
    LUXGFX_RFLAG_DEPTHTEST =    1<<19,
    LUXGFX_RFLAG_WIRE =         1<<20,
  };

  //////////////////////////////////////////////////////////////////////////


  LUX_API void  lxgDepth_init(lxgDepthPTR obj);
  LUX_API void  lxgDepth_apply(lxgDepthPTR obj, lxgContextPTR ctx);
  LUX_API void  lxgDepth_sync(lxgDepthPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgLogic_init(lxgLogicPTR obj);
  LUX_API void  lxgLogic_apply(lxgLogicPTR obj, lxgContextPTR ctx);
  LUX_API void  lxgLogic_sync(lxgLogicPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgStencil_init(lxgStencilPTR obj);
  LUX_API void  lxgStencil_apply(lxgStencilPTR obj, lxgContextPTR ctx);
  LUX_API void  lxgStencil_sync(lxgStencilPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgBlend_init(lxgBlendPTR obj);
  LUX_API void  lxgBlend_apply(lxgBlendPTR obj, lxgContextPTR ctx);
  LUX_API void  lxgBlend_sync(lxgBlendPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgBlendMrt_apply(lxgBlendMrtPTR obj, lxgContextPTR ctx);
  LUX_API void  lxgBlendMrt_sync(lxgBlendMrtPTR obj, lxgContextPTR ctx);

  LUX_API flags32 lxgRenderFlag_init();
  LUX_API flags32 lxgRenderFlag_sync(lxgContextPTR ctx);
  LUX_API void    lxgRenderFlag_apply(flags32 flags, lxgContextPTR ctx, flags32 changed);
  LUX_API const char* lxgRenderFlag_test(lxgContextPTR ctx);

#ifdef __cplusplus
}
#endif

#endif