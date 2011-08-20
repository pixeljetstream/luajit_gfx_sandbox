// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXLUXGFXTYPES_H__
#define __LUXLUXGFXTYPES_H__

#include <luxinia/luxplatform/luxplatform.h>

typedef struct lxgContext_s* LUX_RESTRICT lxgContextPTR;

typedef struct lxgBuffer_s* LUX_RESTRICT lxgBufferPTR;
typedef struct lxgStreamHost_s* LUX_RESTRICT lxgStreamHostPTR;
typedef struct lxgVertexDecl_s* LUX_RESTRICT lxgVertexDeclPTR;
typedef struct lxgFeedbackState_s* LUX_RESTRICT lxgFeedbackStatePTR;

typedef struct lxgTextureImage_s* LUX_RESTRICT lxgTextureImagePTR;
typedef struct lxgSampler_s* LUX_RESTRICT lxgSamplerPTR;
typedef struct lxgTexture_s* LUX_RESTRICT lxgTexturePTR;
typedef struct lxgRenderBuffer_s* LUX_RESTRICT lxgRenderBufferPTR;
typedef struct lxgTextureUpdate_s* LUX_RESTRICT lxgTextureUpdatePTR;

typedef struct lxgProgramParameter_s* LUX_RESTRICT lxgProgramParameterPTR;
typedef struct lxgStageProgram_s* LUX_RESTRICT lxgStageProgramPTR;
typedef struct lxgProgram_s* LUX_RESTRICT lxgProgramPTR;

typedef struct lxgRenderTarget_s* LUX_RESTRICT lxgRenderTargetPTR;
typedef struct lxgViewPort_s* LUX_RESTRICT lxgViewPortPTR;
typedef struct lxgViewPortMrt_s* LUX_RESTRICT lxgViewPortMrtPTR;
typedef struct lxgFrameBounds_s* LUX_RESTRICT lxgFrameBoundsPTR;
typedef struct lxgRenderAssign_s* LUX_RESTRICT lxgRenderAssignPTR;

typedef struct lxgBlend_s* LUX_RESTRICT lxgBlendPTR;
typedef struct lxgStencil_s* LUX_RESTRICT lxgStencilPTR;
typedef struct lxgLogic_s* LUX_RESTRICT lxgLogicPTR;
typedef struct lxgDepth_s* LUX_RESTRICT lxgDepthPTR;
typedef struct lxgColor_s* LUX_RESTRICT lxgColorPTR;
typedef struct lxgRasterizer_s* LUX_RESTRICT lxgRasterizerPTR;


typedef const struct lxgContext_s* LUX_RESTRICT lxgContextCPTR;

typedef const struct lxgBuffer_s* LUX_RESTRICT lxgBufferCPTR;
typedef const struct lxgStreamHost_s* LUX_RESTRICT lxgStreamHostCPTR;
typedef const struct lxgVertexDecl_s* LUX_RESTRICT lxgVertexDeclCPTR;
typedef const struct lxgFeedbackState_s* LUX_RESTRICT lxgFeedbackStateCPTR;
        
typedef const struct lxgTextureImage_s* LUX_RESTRICT lxgTextureImageCPTR;
typedef const struct lxgSampler_s* LUX_RESTRICT lxgSamplerCPTR;
typedef const struct lxgTexture_s* LUX_RESTRICT lxgTextureCPTR;
typedef const struct lxgRenderBuffer_s* LUX_RESTRICT lxgRenderBufferCPTR;
typedef const struct lxgTextureUpdate_s* LUX_RESTRICT lxgTextureUpdateCPTR;
        
typedef const struct lxgProgramParameter_s* LUX_RESTRICT lxgProgramParameterCPTR;
typedef const struct lxgStageProgram_s* LUX_RESTRICT lxgStageProgramCPTR;
typedef const struct lxgProgram_s* LUX_RESTRICT lxgProgramCPTR;
        
typedef const struct lxgRenderTarget_s* LUX_RESTRICT lxgRenderTargetCPTR;
typedef const struct lxgViewPort_s* LUX_RESTRICT lxgViewPortCPTR;
typedef const struct lxgViewPortMrt_s* LUX_RESTRICT lxgViewPortMrtCPTR;
typedef const struct lxgFrameBounds_s* LUX_RESTRICT lxgFrameBoundsCPTR;
typedef const struct lxgRenderAssign_s* LUX_RESTRICT lxgRenderAssignCPTR;
        
typedef const struct lxgBlend_s* LUX_RESTRICT lxgBlendCPTR;
typedef const struct lxgColor_s* LUX_RESTRICT lxgColorCPTR;
typedef const struct lxgStencil_s* LUX_RESTRICT lxgStencilCPTR;
typedef const struct lxgLogic_s* LUX_RESTRICT lxgLogicCPTR;
typedef const struct lxgDepth_s* LUX_RESTRICT lxgDepthCPTR;
typedef const struct lxgRasterizer_s* LUX_RESTRICT lxgRasterizerCPTR;


typedef flags32 lxgRenderFlag_t;

typedef enum lxgAccessMode_e{
  LUXGFX_ACCESS_READ,
  LUXGFX_ACCESS_WRITE,
  LUXGFX_ACCESS_READWRITE,

  LUXGFX_ACCESS_WRITEDISCARD,
  LUXGFX_ACCESS_WRITEDISCARDALL,

  LUXGFX_ACCESSES,
}lxgAccessMode_t;

#endif

