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
    bool16              enabled;
    bool16              write;
    lxGLCompareMode_t   func;
  }lxgDepth_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgLogic_s{
    bool32            enabled;
    lxGLLogicOp_t     op;
  }lxgLogic_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxgColorChannel_e{
    LUXGFX_COLOR_RED,
    LUXGFX_COLOR_GREEN,
    LUXGFX_COLOR_BLUE,
    LUXGFX_COLOR_ALPHA,
    LUXGFX_COLORS,
  }lxgColorChannel_t;

  typedef struct lxgColor_s{
    bool32            individual;
    bool8             write[LUXGFX_MAX_RENDERTARGETS][LUXGFX_COLORS];
  }lxgColor_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxgFaceSide_e{
    LUXGFX_FACE_FRONT,
    LUXGFX_FACE_BACK,
    LUXGFX_FACES,
  }lxgFaceSide_t;

  typedef struct lxgStencilOp_s
  {
    lxGLStencilMode_t   fail;
    lxGLStencilMode_t   zfail;
    lxGLStencilMode_t   zpass;
    lxGLCompareMode_t   func;
  }lxgStencilOp_t;

  typedef struct lxgStencil_s{
    bool8         enabled;
    flags32       write;
    flags32       mask;
    uint32        refvalue;

    lxgStencilOp_t  ops[LUXGFX_FACES]; // 0 = front 1 = back
  }lxgStencil_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgBlendMode_s{
    lxGLBlendWeight_t   srcw;
    lxGLBlendWeight_t   dstw;
    lxGLBlendEquation_t equ;
  }lxgBlendMode_t;

  typedef struct lxgBlendStage_s{
    bool32            enabled;
    lxgBlendMode_t    colormode;
    lxgBlendMode_t    alphamode;
  }lxgBlendStage_t;

  typedef struct lxgBlend_s{
    bool32          individual;
    lxgBlendStage_t blends[LUXGFX_MAX_RENDERTARGETS];
  }lxgBlend_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgRasterizer_s{
    bool8           cull;
    bool8           cullfront;
    bool8           ccw;
    enum32          fill;
  }lxgRasterizer_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgRasterState_s{
    lxgRasterizerCPTR   rasterizerObj;
    lxgColorCPTR        colorObj;
    lxgBlendCPTR        blendObj;
    lxgDepthCPTR        depthObj;
    lxgStencilCPTR      stencilObj;
    lxgLogicCPTR        logicObj;

    lxgRasterizer_t   rasterizer;
    lxgColor_t        color;
    lxgBlend_t        blend;
    lxgDepth_t        depth;
    lxgStencil_t      stencil;
    lxgLogic_t        logic;
  }lxgRasterState_t;

  //////////////////////////////////////////////////////////////////////////

  LUX_API void  lxgRasterizer_init(lxgRasterizerPTR obj);
  LUX_API void  lxgRasterizer_sync(lxgRasterizerPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgColor_init(lxgColorPTR obj);
  LUX_API void  lxgColor_sync(lxgColorPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgDepth_init(lxgDepthPTR obj);
  LUX_API void  lxgDepth_sync(lxgDepthPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgLogic_init(lxgLogicPTR obj);
  LUX_API void  lxgLogic_sync(lxgLogicPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgStencil_init(lxgStencilPTR obj);
  LUX_API void  lxgStencil_sync(lxgStencilPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgBlend_init(lxgBlendPTR obj);
  LUX_API void  lxgBlend_sync(lxgBlendPTR obj, lxgContextPTR ctx);

  LUX_API void  lxgContext_applyColor( lxgContextPTR ctx, lxgColorCPTR obj);
  LUX_API void  lxgContext_applyDepth( lxgContextPTR ctx, lxgDepthCPTR obj);
  LUX_API void  lxgContext_applyLogic( lxgContextPTR ctx, lxgLogicCPTR obj);
  LUX_API void  lxgContext_applyStencil( lxgContextPTR ctx, lxgStencilCPTR obj);
  LUX_API void  lxgContext_applyBlend( lxgContextPTR ctx, lxgBlendCPTR obj);
  LUX_API void  lxgContext_applyRasterizer( lxgContextPTR ctx, lxgRasterizerCPTR obj);

  LUX_API void  lxgProgramParameter_stateColor( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj);
  LUX_API void  lxgProgramParameter_stateDepth( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj);
  LUX_API void  lxgProgramParameter_stateLogic( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj);
  LUX_API void  lxgProgramParameter_stateStencil( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj);
  LUX_API void  lxgProgramParameter_stateBlend( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj);
  LUX_API void  lxgProgramParameter_stateRasterizer( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj);


#ifdef __cplusplus
}
#endif

#endif