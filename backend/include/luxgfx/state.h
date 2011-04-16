// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_STATE_H__
#define __LUXLUXGFX_STATE_H__

#include <luxplatform/luxplatform.h>
#include <luxmath/basetypes.h>

#define LUXGFX_MAX_TEXTURE_IMAGES     32
#define LUXGFX_MAX_RENDERTARGETS      16
#define LUXGFX_MAX_RWTEXTURE_IMAGES   8
#define LUXGFX_MAX_UNIFORM_BUFFERS    (12*5)

#define LUXGFX_VALIDITY     1

#ifdef __cplusplus
extern "C"{
#endif

  #include "glconfig.h"


  //////////////////////////////////////////////////////////////////////////

  typedef enum lxgWinding_e{
    LUXGFX_WIND_CCW,
    LUXGFX_WIND_CW,
  }lxgWinding_t;

  typedef struct lxgRasterizer_s{
    lxgWinding_t    wind;

    booln       multisample;
    booln       fill;
    booln       depthclamp;

    float       polyoffset;
    float       polyoffsetslope;
  }lxgRasterizer_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgAlpha_s{
    lxgCompareMode_t    func;
    float               refval;
  }lxgAlpha_t;

  typedef struct lxgDepth_s{
    lxgCompareMode_t    func;
    booln               clip;
  }lxgDepth_t;

  //////////////////////////////////////////////////////////////////////////
  typedef enum lxgLogicOp_e{
    LUXGFX_LOGICOP_CLEAR,
    LUXGFX_LOGICOP_SET,
    LUXGFX_LOGICOP_COPY,
    LUXGFX_LOGICOP_INVERTED,
    LUXGFX_LOGICOP_NOOP,
    LUXGFX_LOGICOP_INVERT,
    LUXGFX_LOGICOP_AND,
    LUXGFX_LOGICOP_NAND,
    LUXGFX_LOGICOP_OR,
    LUXGFX_LOGICOP_NOR,
    LUXGFX_LOGICOP_XOR,
    LUXGFX_LOGICOP_EQUIV,
    LUXGFX_LOGICOP_AND_REVERSE,
    LUXGFX_LOGICOP_AND_INVERTED,
    LUXGFX_LOGICOP_OR_REVERSE,
    LUXGFX_LOGICOP_OR_INVERTED,
    LUXGFX_LOGICOP_ILLEGAL,
  }lxgLogicOp_t;

  typedef struct lxgLogic_s{
    lxgLogicOp_t      op;
  }lxgLogic_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxgStencilMode_s{
    LUXGFX_STENCIL_KEEP,
    LUXGFX_STENCIL_ZERO,
    LUXGFX_STENCIL_REPLACE,
    LUXGFX_STENCIL_INCR_SAT,
    LUXGFX_STENCIL_DECR_SAT,
    LUXGFX_STENCIL_INVERT,
    LUXGFX_STENCIL_INCR,
    LUXGFX_STENCIL_DECR,
    LUXGFX_STENCIL_UNKNOWN,
  }lxgStencilMode_t;

  typedef struct lxgStencilOp_s
  {
    lxgStencilMode_t  fail : 8;
    lxgStencilMode_t  zfail : 8;
    lxgStencilMode_t  zpass : 8;
    lxgStencilMode_t  func : 8;
  }lxgStencilOp_t;

  typedef struct lxgStencil_s{
    uint16      refvalue;
    uint16      mask;

    lxgStencilOp_t  ops[2]; // 0 = front 1 = back
  }lxgStencil_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxgBlendWeight_s{
    LUXGFX_BLENDW_ZERO,
    LUXGFX_BLENDW_ONE,
    LUXGFX_BLENDW_RGB_SRC,
    LUXGFX_BLENDW_RGB_DST,
    LUXGFX_BLENDW_A_SRC,
    LUXGFX_BLENDW_A_DST,
    LUXGFX_BLENDW_INVRGB_SRC,
    LUXGFX_BLENDW_INVRGB_DST,
    LUXGFX_BLENDW_INVA_SRC,
    LUXGFX_BLENDW_INVA_DST,
    LUXGFX_BLENDW_UNKNOWN,
  }lxgBlendWeight_t;

  typedef enum lxgBlendEquation_s{
    LUXGFX_BLENDE_ADD,
    LUXGFX_BLENDE_SUB,
    LUXGFX_BLENDE_SUB_REV,
    LUXGFX_BLENDE_MIN,
    LUXGFX_BLENDE_MAX,
    LUXGFX_BLENDE_UNKNOWN,
  }lxgBlendEquation_t;

  typedef struct lxgBlendMode_s{
    lxgBlendWeight_t srcw  : 6;
    lxgBlendWeight_t dstw  : 6;
    lxgBlendEquation_t equ : 4;
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

  typedef struct lxgLine_s{
    float       linewidth;
    ushort      linefactor;
    ushort      linestipple;
  }lxgLine_t;

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
    LUXGFX_RFLAG_ALPHATEST =    1<<10,
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

  LUX_API void  lxgAlpha_init(lxgAlphaPTR obj);
  LUX_API void  lxgAlpha_apply(lxgAlphaPTR obj, lxgContextPTR ctx);
  LUX_API void  lxgAlpha_sync(lxgAlphaPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgLine_init(lxgLinePTR   obj);
  LUX_API void  lxgLine_apply(lxgLinePTR obj, lxgContextPTR ctx);
  LUX_API void  lxgLine_sync(lxgLinePTR obj, lxgContextPTR ctx);

  LUX_API flags32 lxgRenderFlag_init();
  LUX_API flags32 lxgRenderFlag_sync(lxgContextPTR ctx);
  LUX_API void    lxgRenderFlag_apply(flags32 flags, lxgContextPTR ctx, flags32 changed);
  LUX_API const char* lxgRenderFlag_test(lxgContextPTR ctx);

  LUX_API void lxgRasterizer_sync(lxgRasterizerPTR raster, lxgContextPTR ctx);

#ifdef __cplusplus
}
#endif

#endif