// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFXTYPES_H__
#define __LUXLUXGFXTYPES_H__

#include <luxplatform/luxplatform.h>

typedef struct lxgContext_s* LUX_RESTRICT lxgContextPTR;

typedef struct lxgBuffer_s* lxgBufferPTR;
typedef struct lxgStreamHost_s* lxgStreamHostPTR;
typedef struct lxgVertexDecl_s* lxgVertexDeclPTR;
typedef struct lxgVertexPointer_s* lxgVertexPointerPTR;
typedef struct lxgVertexElement_s* lxgVertexElementPTR;
typedef struct lxgVertexState_s* lxgVertexStatePTR;
typedef struct lxgFeedbackState_s* lxgFeedbackStatePTR;

typedef struct lxgTextureImage_s* lxgTextureImagePTR;
typedef struct lxgSampler_s* lxgSamplerPTR;
typedef struct lxgTexture_s* lxgTexturePTR;
typedef struct lxgRenderBuffer_s* lxgRenderBufferPTR;
typedef struct lxgTextureUpdate_s* lxgTextureUpdatePTR;

typedef struct lxgProgramParameter_s* lxgProgramParameterPTR;
typedef struct lxgStageProgram_s* lxgStageProgramPTR;
typedef struct lxgProgram_s* lxgProgramPTR;

typedef struct lxgRenderTarget_s* lxgRenderTargetPTR;
typedef struct lxgViewPort_s* lxgViewPortPTR;
typedef struct lxgViewPortMrt_s* lxgViewPortMrtPTR;
typedef struct lxgFrameBounds_s* lxgFrameBoundsPTR;
typedef struct lxgRenderAssign_s* lxgRenderAssignPTR;

typedef struct lxgBlendMrt_s* lxgBlendMrtPTR;
typedef struct lxgBlend_s* lxgBlendPTR;
typedef struct lxgStencil_s* lxgStencilPTR;
typedef struct lxgLogic_s* lxgLogicPTR;
typedef struct lxgDepth_s* lxgDepthPTR;
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

