// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFXTYPES_H__
#define __LUXLUXGFXTYPES_H__

#include <luxplatform/luxplatform.h>

typedef struct lxGFXContext_s* LUX_RESTRICT lxGFXContextPTR;

typedef struct lxGFXBuffer_s* lxGFXBufferPTR;
typedef struct lxGFXStreamHost_s* lxGFXStreamHostPTR;
typedef struct lxGFXVertexDecl_s* lxGFXVertexDeclPTR;
typedef struct lxGFXVertexPointer_s* lxGFXVertexPointerPTR;
typedef struct lxGFXVertexElement_s* lxGFXVertexElementPTR;
typedef struct lxGFXVertexState_s* lxGFXVertexStatePTR;

typedef struct lxGFXTextureImage_s* lxGFXTextureImagePTR;
typedef struct lxGFXSampler_s* lxGFXSamplerPTR;
typedef struct lxGFXTexture_s* lxGFXTexturePTR;
typedef struct lxGFXRenderBuffer_s* lxGFXRenderBufferPTR;
typedef struct lxGFXTextureUpdate_s* lxGFXTextureUpdatePTR;

typedef struct lxGFXProgramParameter_s* lxGFXProgramParameterPTR;
typedef struct lxGFXDomainProgram_s* lxGFXDomainProgramPTR;
typedef struct lxGFXProgram_s* lxGFXProgramPTR;
typedef struct lxGFXProgramData_s* lxGFXProgramDataPTR;

typedef struct lxGFXRenderTarget_s* lxGFXRenderTargetPTR;
typedef struct lxGFXViewPort_s* lxGFXViewPortPTR;
typedef struct lxGFXViewPortMrt_s* lxGFXViewPortMrtPTR;
typedef struct lxGFXFrameBounds_s* lxGFXFrameBoundsPTR;
typedef struct lxGFXRenderAssign_s* lxGFXRenderAssignPTR;

typedef struct lxGFXAlpha_s* lxGFXAlphaPTR;
typedef struct lxGFXLine_s* lxGFXLinePTR;
typedef struct lxGFXAlpha_s* lxGFXAlphaPTR;
typedef struct lxGFXBlendMrt_s* lxGFXBlendMrtPTR;
typedef struct lxGFXBlend_s* lxGFXBlendPTR;
typedef struct lxGFXStencil_s* lxGFXStencilPTR;
typedef struct lxGFXLogic_s* lxGFXLogicPTR;
typedef struct lxGFXDepth_s* lxGFXDepthPTR;
typedef struct lxGFXRasterizer_s* lxGFXRasterizerPTR;

#endif

