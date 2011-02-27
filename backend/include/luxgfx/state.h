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

  typedef enum lxGFXWinding_e{
    LUXGFX_WIND_CCW,
    LUXGFX_WIND_CW,
    LUXGFX_WIND_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXWinding_t;

  typedef struct lxGFXRasterizer_s{
    lxGFXWinding_t    wind;

    booln       multisample;
    booln       fill;
    booln       depthclamp;

    float       polyoffset;
    float       polyoffsetslope;
  }lxGFXRasterizer_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxGFXCompareMode_e{
    LUXGFX_COMPARE_NEVER,
    LUXGFX_COMPARE_LESS,
    LUXGFX_COMPARE_EQUAL,
    LUXGFX_COMPARE_LEQUAL,
    LUXGFX_COMPARE_GREATER,
    LUXGFX_COMPARE_NOTEQUAL,
    LUXGFX_COMPARE_GEQUAL,
    LUXGFX_COMPARE_ALWAYS,
    LUXGFX_COMPARE_DONTEXECUTE,
    LUXGFX_COMPARE_ILLEGAL,
    LUXGFX_COMPARE_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXCompareMode_t;

  typedef struct lxGFXAlpha_s{
    lxGFXCompareMode_t    func;
    float                 refval;
  }lxGFXAlpha_t;

  typedef struct lxGFXDepth_s{
    lxGFXCompareMode_t    func;
    booln                 clip;
  }lxGFXDepth_t;

  //////////////////////////////////////////////////////////////////////////
  typedef enum lxGFXLogicOp_e{
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
    LUXGFX_LOGICOP_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXLogicOp_t;

  typedef struct lxGFXLogic_s{
    lxGFXLogicOp_t      op;
  }lxGFXLogic_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxGFXStencilMode_s{
    LUXGFX_STENCIL_KEEP,
    LUXGFX_STENCIL_ZERO,
    LUXGFX_STENCIL_REPLACE,
    LUXGFX_STENCIL_INCR_SAT,
    LUXGFX_STENCIL_DECR_SAT,
    LUXGFX_STENCIL_INVERT,
    LUXGFX_STENCIL_INCR,
    LUXGFX_STENCIL_DECR,
    LUXGFX_STENCIL_UNKNOWN,
    LUXGFX_STENCIL_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXStencilMode_t;

  typedef struct lxGFXStencilOp_s
  {
    byte  fail;
    byte  zfail;
    byte  zpass;
    byte  func;
  }lxGFXStencilOp_t;

  typedef struct lxGFXStencil_s{
    uint16      refvalue;
    uint16      mask;

    lxGFXStencilOp_t  ops[2]; // 0 = front 1 = back
  }lxGFXStencil_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxGFXBlendWeight_s{
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

    LUXGFX_BLENDW_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXBlendWeight_t;

  typedef enum lxGFXBlendEquation_s{
    LUXGFX_BLENDE_ADD,
    LUXGFX_BLENDE_SUB,
    LUXGFX_BLENDE_SUB_REV,
    LUXGFX_BLENDE_MIN,
    LUXGFX_BLENDE_MAX,
    LUXGFX_BLENDE_UNKNOWN,

    LUXGFX_BLENDE_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXBlendEquation_t;

  typedef struct lxGFXBlendMode_s{
    unsigned  srcw : 6;
    unsigned  dstw : 6;
    unsigned  equ : 4;
  }lxGFXBlendMode_t;

  typedef struct lxGFXBlend_s{
    lxGFXBlendMode_t    colormode;
    lxGFXBlendMode_t    alphamode;
  }lxGFXBlend_t;

  typedef struct lxGFXBlendMrt_s{
    ushort          individual;
    ushort          numused;
    flags32         enabled;
    lxGFXBlend_t    blends[LUXGFX_MAX_RENDERTARGETS];
  }lxGFXBlendMrt_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxGFXLine_s{
    float       linewidth;
    ushort      linefactor;
    ushort      linestipple;
  }lxGFXLine_t;

  //////////////////////////////////////////////////////////////////////////

  enum lxGFXRenderFlag_e {
    LUXGFX_RFLAG_NONE=      0,
    LUXGFX_RFLAG_FRONTCULL  =   1<<0,     // front faces are culled
    LUXGFX_RFLAG_NODEPTHMASK =    1<<1,     // dont write in depthmask
    LUXGFX_RFLAG_STENCILMASK =    1<<2,
    LUXGFX_RFLAG_ALPHATEST =    1<<3,     // use alphatest
    LUXGFX_RFLAG_NOCULL =     1<<4,       // draw the backfaces or not
    LUXGFX_RFLAG_NOCOLORMASKR = 1<<5,
    LUXGFX_RFLAG_NOCOLORMASKG = 1<<6,
    LUXGFX_RFLAG_NOCOLORMASKB = 1<<7,
    LUXGFX_RFLAG_NOCOLORMASKA = 1<<8,
    LUXGFX_RFLAG_NOCOLORMASK = LUXGFX_RFLAG_NOCOLORMASKR|LUXGFX_RFLAG_NOCOLORMASKG|LUXGFX_RFLAG_NOCOLORMASKB|LUXGFX_RFLAG_NOCOLORMASKA,
    LUXGFX_RFLAG_BLEND =      1<<9,       // blending
    LUXGFX_RFLAG_STENCILTEST =    1<<10,
    LUXGFX_RFLAG_LOGIC =      1<<11,
    LUXGFX_RFLAG_NODEPTHTEST =    1<<12,      // dont make depthtest
    LUXGFX_RFLAG_WIRE =     1<<13,
  };
  typedef flags32 lxGFXRenderFlag_t;

  //////////////////////////////////////////////////////////////////////////


  LUX_API void  lxGFXDepth_init(lxGFXDepthPTR obj);
  LUX_API void  lxGFXDepth_apply(lxGFXContextPTR ctx, const lxGFXDepthPTR obj);
  LUX_API void  lxGFXDepth_sync(lxGFXContextPTR ctx, lxGFXDepthPTR obj);

  LUX_API void  lxGFXLogic_init(lxGFXLogicPTR obj);
  LUX_API void  lxGFXLogic_apply(lxGFXContextPTR ctx, const lxGFXLogicPTR obj);
  LUX_API void  lxGFXLogic_sync(lxGFXContextPTR ctx, lxGFXLogicPTR obj);

  LUX_API void  lxGFXStencil_init(lxGFXStencilPTR obj);
  LUX_API void  lxGFXStencil_apply(lxGFXContextPTR ctx, const lxGFXStencilPTR obj);
  LUX_API void  lxGFXStencil_sync(lxGFXContextPTR ctx, lxGFXStencilPTR obj);

  LUX_API void  lxGFXBlend_init(lxGFXBlendPTR obj);
  LUX_API void  lxGFXBlend_apply(lxGFXContextPTR ctx, const lxGFXBlendPTR obj);
  LUX_API void  lxGFXBlend_sync(lxGFXContextPTR ctx, lxGFXBlendPTR obj);

  LUX_API void  lxGFXBlendMrt_apply(lxGFXContextPTR ctx, lxGFXBlendMrtPTR obj);
  LUX_API void  lxGFXBlendMrt_sync(lxGFXContextPTR ctx, lxGFXBlendMrtPTR obj);

  LUX_API void  lxGFXAlpha_init(lxGFXAlphaPTR obj);
  LUX_API void  lxGFXAlpha_apply(lxGFXContextPTR ctx, const lxGFXAlphaPTR obj);
  LUX_API void  lxGFXAlpha_sync(lxGFXContextPTR ctx, lxGFXAlphaPTR obj);

  LUX_API void  lxGFXLine_init(lxGFXLinePTR   obj);
  LUX_API void  lxGFXLine_apply(lxGFXContextPTR ctx, const lxGFXLinePTR obj);
  LUX_API void  lxGFXLine_sync(lxGFXContextPTR ctx, lxGFXLinePTR obj);

  LUX_API flags32 lxGFXRenderFlag_sync(lxGFXContextPTR ctx);
  LUX_API void    lxGFXRenderFlag_apply(lxGFXContextPTR ctx, flags32 flags, flags32 changed);
  LUX_API const char* lxGFXRenderFlag_test(lxGFXContextPTR ctx);

  LUX_API void lxGFXRasterizer_sync(lxGFXContextPTR ctx, lxGFXRasterizerPTR raster);

#ifdef __cplusplus
}
#endif

#endif