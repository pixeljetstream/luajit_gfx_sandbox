// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_CONTEXT_H__
#define __LUXLUXGFX_CONTEXT_H__

#include <luxplatform/luxplatform.h>
#include <luxplatform/debug.h>

#ifdef __cplusplus
extern "C"{
#endif

  #include "glconfig.h"
  #include "vertex.h"
  #include "buffer.h"
  #include "texture.h"
  #include "state.h"
  #include "rendertarget.h"

  //////////////////////////////////////////////////////////////////////////
  enum lxGFXCapability_e{
    LUXGFX_CAP_POINTSPRITE = 1<<0,
    LUXGFX_CAP_STENCILWRAP = 1<<1,
    LUXGFX_CAP_BLENDSEP = 1<<2,
    LUXGFX_CAP_OCCQUERY = 1<<3,

    LUXGFX_CAP_FBO      = 1<<4,     //+DEPTHSTENCIL
    LUXGFX_CAP_FBOMS    = 1<<5,     // blit,ms
    LUXGFX_CAP_DEPTHFLOAT  = 1<<6,

    LUXGFX_CAP_VBO    = 1<<7,
    LUXGFX_CAP_PBO    = 1<<8,
    LUXGFX_CAP_UBO    = 1<<9,

    LUXGFX_CAP_TEX3D = 1<<10,
    LUXGFX_CAP_TEXRECT = 1<<11,
    LUXGFX_CAP_TEXNP2 = 1<<12,
    LUXGFX_CAP_TEXCUBEARRAY = 1<<13,
    LUXGFX_CAP_TEXS3TC = 1<<14,
    LUXGFX_CAP_TEXRGTC = 1<<15,
    LUXGFX_CAP_TEXRW  = 1<<16,

    LUXGFX_CAP_BUFMAPRANGE = 1<<17,
    LUXGFX_CAP_BUFCOPY = 1<<18,

    LUXGFX_CAP_SM0    = 1<<20,    // DOT3,CROSSBAR,CUBE
    LUXGFX_CAP_SM1    = 1<<21,    // VERTEX,SHADOW
    LUXGFX_CAP_SM2    = 1<<22,    // +FRAGMENT
    LUXGFX_CAP_SM2EXT = 1<<23,    // +DRAWBUFFERS,FLOAT,HLSHADERS
    LUXGFX_CAP_SM3    = 1<<24,    // +NV3/ATI_SHADERLOD
    LUXGFX_CAP_SM4    = 1<<25,    //  GL3.3 TEXINT,TEXBUF,TEXARRAY,UBO,FBOMIX
    //  TEXSAMPLER,XFBO,GS,CUBESAMPLE
    LUXGFX_CAP_SM5    = 1<<26,    //  GL4.0 
  };


  //////////////////////////////////////////////////////////////////////////

  typedef enum lxGFXGPUVendor_e{
    LUXGFX_VENDOR_UNKNOWN,
    LUXGFX_VENDOR_NVIDIA,
    LUXGFX_VENDOR_ATI,
    LUXGFX_VENDOR_INTEL,
    LUXGFX_VENDOR_FORCE_DWORD = 0x7FFFFFFF
  }lxGFXGPUVendor_t;

  typedef enum lxGFXGPUDomain_e{
    LUXGFX_GPUDOMAIN_VERTEX,
    LUXGFX_GPUDOMAIN_FRAGMENT,
    LUXGFX_GPUDOMAIN_GEOMETRY,
    LUXGFX_GPUDOMAIN_TESS_EVAL,
    LUXGFX_GPUDOMAIN_TESS_CTRL,
    LUXGFX_GPUDOMAINS,
    LUXGFX_GPUDOMAIN_FORCE_DWORD = 0x7FFFFFFF
  }lxGFXGPUDomain_t;

  typedef enum lxGFXGPUMode_e{
    LUXGFX_GPUMODE_FIXED,
    LUXGFX_GPUMODE_ASM,
    LUXGFX_GPUMODE_HL,
    LUXGFX_GPUMODE_FORCE_DWORD = 0x7FFFFFFF
  }lxGFXGPUMode_t;


  //////////////////////////////////////////////////////////////////////////
  typedef struct lxGFXCapabilites_s{
    int     texsize;
    int     texsize3d;
    int     texlayers;
    int     texunits;
    int     teximages;
    int     texcoords;
    int     texvtxunits;
    float   texaniso;
    float   pointsize;
    int     drawbuffers;
    int     viewports;
    int     fbosamples;
  }lxGFXCapabilites_t;

  typedef struct lxGFXContext_s{
    flags32               capbits;
    lxGFXVertexState_t    vertex;
    lxGFXTexturePTR       textures[LUXGFX_MAX_TEXTURE_IMAGES];
    lxGFXSamplerPTR       samplers[LUXGFX_MAX_TEXTURE_IMAGES];
    lxGFXRenderTargetPTR  rendertargets[LUXGFX_RENDERTARGETS];
    lxGFXTextureImagePTR  images[LUXGFX_MAX_RWTEXTURE_IMAGES];
    lxGFXBufferPTR        uniform[LUXGFX_MAX_UNIFORM_BUFFERS];

    flags32               rflag;
    uint16                stencilmaskpos;
    uint16                stencilmaskneg;

    lxGFXAlpha_t          alpha;
    lxGFXBlend_t          blend;

    lxGFXDepth_t          depth;
    lxGFXStencil_t        stencil;
    lxGFXLogic_t          logic;

    lxGFXLine_t           line;

    lxGFXRasterizer_t     rasterizer;
    lxGFXViewPort_t       viewport;
    lxGFXFrameBounds_t    framebounds;
    lxGFXFrameBounds_t    window;

    lxGFXBlendMrt_t       blendMRT;
    lxGFXViewPortMrt_t    viewportMRT;
    
    lxGFXCapabilites_t    capabilites;
  }lxGFXContext_t;

  //////////////////////////////////////////////////////////////////////////
  LUX_API const char* lxGFXContext_init(lxGFXContextPTR ctx);
  LUX_API void  lxGFXContext_syncStates(lxGFXContextPTR ctx);
  LUX_API booln lxGFXContext_checkStates(lxGFXContextPTR ctx);
  LUX_API void  lxGFXContext_resetTextures(lxGFXContextPTR ctx);
  LUX_API void  lxGFXContext_resetVertexStreams(lxGFXContextPTR ctx);

  LUX_API void  lxGFXTexture_apply(lxGFXContextPTR ctx, uint imageunit, lxGFXTexturePTR tex);
  LUX_API void  lxGFXSampler_apply(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR self);
  LUX_API void  lxGFXTextures_apply(lxGFXContextPTR ctx, uint start, uint num, lxGFXTexturePTR *texs);
  LUX_API void  lxGFXSamplers_apply(lxGFXContextPTR ctx, uint start, uint num, lxGFXSamplerPTR *samps);
  LUX_API void  lxGFXTextureImage_apply(lxGFXContextPTR ctx, uint imageunit, lxGFXTextureImagePTR tex);
  LUX_API void  lxGFXRenderTarget_apply(lxGFXContextPTR ctx, lxGFXRenderTargetType_t type, lxGFXRenderTargetPTR rt);
  LUX_API void  lxGFXRenderTarget_applyDraw(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt, booln setViewport);

  // perform check if same state, prior to setting
  LUX_API void lxGFXAlpha_checked(lxGFXContextPTR ctx, const lxGFXAlphaPTR );
  LUX_API void lxGFXBlend_checked(lxGFXContextPTR ctx, const lxGFXBlendPTR );
  LUX_API void lxGFXDepth_checked(lxGFXContextPTR ctx, const lxGFXDepthPTR );
  LUX_API void lxGFXLogic_checked(lxGFXContextPTR ctx, const lxGFXLogicPTR );
  LUX_API void lxGFXLine_checked(lxGFXContextPTR ctx, const lxGFXLinePTR );
  LUX_API void lxGFXStencil_checked(lxGFXContextPTR ctx, const lxGFXStencilPTR );
  LUX_API void lxGFXTexture_checked(lxGFXContextPTR ctx, uint imageunit, lxGFXTexturePTR tex);
  LUX_API void lxGFXSampler_checked(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR samp);
  LUX_API void lxGFXTextures_checked(lxGFXContextPTR ctx, uint start, uint num, lxGFXTexturePTR *texs);
  LUX_API void lxGFXSamplers_checked(lxGFXContextPTR ctx, uint start, uint num, lxGFXSamplerPTR *samps);
  LUX_API void lxGFXRenderFlag_checked(lxGFXContextPTR ctx, flags32 needed);
  LUX_API void lxGFXVertexDeclaration_checked(lxGFXContextPTR ctx, const lxGFXVertexDeclPTR decl);
  LUX_API void lxGFXVertexAttrib_checked(lxGFXContextPTR ctx, flags32 needed);
  LUX_API void lxGFXRenderTarget_checked(lxGFXContextPTR ctx, lxGFXRenderTargetType_t type, lxGFXRenderTargetPTR rt);
  LUX_API void lxGFXTextureImage_checked(lxGFXContextPTR ctx, uint imageunit, lxGFXTextureImagePTR img);

  LUX_API void  lxGFXViewPortScissor_checked(lxGFXContextPTR ctx, const lxRectangleiPTR rect);
  LUX_API booln lxGFXBuffer_checkedSetUniform(lxGFXContextPTR ctx, uint idx, lxGFXBufferPTR buffer);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE void lxGFXAlpha_checked(lxGFXContextPTR ctx, const lxGFXAlphaPTR obj)
  {
    if (memcmp(&ctx->alpha,obj,sizeof(lxGFXAlpha_t)))
    {
      lxGFXAlpha_apply(ctx, obj);
    }
  }
  LUX_INLINE void lxGFXBlend_checked(lxGFXContextPTR ctx, const lxGFXBlendPTR obj)
  {
    if (memcmp(&ctx->blend,obj,sizeof(lxGFXBlend_t)))
    {
      lxGFXBlend_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxGFXDepth_checked(lxGFXContextPTR ctx, const lxGFXDepthPTR obj)
  {
    if (memcmp(&ctx->depth,obj,sizeof(lxGFXDepth_t)))
    {
      lxGFXDepth_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxGFXLogic_checked(lxGFXContextPTR ctx, const lxGFXLogicPTR obj)
  {
    if (memcmp(&ctx->logic,obj,sizeof(lxGFXLogic_t)))
    {
      lxGFXLogic_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxGFXLine_checked(lxGFXContextPTR ctx, const lxGFXLinePTR obj)
  {
    if (memcmp(&ctx->line,obj,sizeof(lxGFXLine_t)))
    {
      lxGFXLine_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxGFXStencil_checked(lxGFXContextPTR ctx, const lxGFXStencilPTR obj)
  {
    if (memcmp(&ctx->stencil,obj,sizeof(lxGFXStencil_t)))
    {
      lxGFXStencil_apply(ctx,obj);
    }
  }

  LUX_INLINE void lxGFXTexture_checked(lxGFXContextPTR ctx, uint imageunit, lxGFXTexturePTR tex)
  {
    if (ctx->textures[imageunit] != tex){
      lxGFXTexture_apply(ctx,imageunit,tex);
    }
    else{
      glActiveTexture(GL_TEXTURE0_ARB + imageunit);
    }
  }
  LUX_INLINE void lxGFXSampler_checked(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR sampler)
  {
    if (ctx->samplers[imageunit] != sampler){
      lxGFXSampler_apply(ctx,imageunit,sampler);
    }
  }
  LUX_INLINE void lxGFXTextureImage_checked(lxGFXContextPTR ctx, uint imageunit, lxGFXTextureImagePTR img)
  {
    if (ctx->images[imageunit] != img){
      lxGFXTextureImage_apply(ctx,imageunit,img);
    }
  }
  LUX_INLINE booln lxGFXBuffer_checkedSetUniform(lxGFXContextPTR ctx, uint idx, lxGFXBufferPTR buffer)
  {
    booln res = ctx->uniform[idx] != buffer;
    ctx->uniform[idx] = buffer;
    return res;
  }
  LUX_INLINE void lxGFXTextures_checked(lxGFXContextPTR ctx, uint start, uint num, lxGFXTexturePTR *texs)
  {
    LUX_ASSUME(num >= 1 && num <= LUXGFX_MAX_TEXTURE_IMAGES);
    if (memcmp(ctx->textures,texs,sizeof(lxGFXTexturePTR )*num)){
      lxGFXTextures_apply(ctx,start,num,texs);
    }
  }
  LUX_INLINE void lxGFXSamplers_checked(lxGFXContextPTR ctx, uint start, uint num, lxGFXSamplerPTR *samps)
  {
    LUX_ASSUME(num >= 1 && num <= LUXGFX_MAX_TEXTURE_IMAGES);
    if (memcmp(ctx->textures,samps,sizeof(lxGFXSamplerPTR)*num)){
      lxGFXSamplers_apply(ctx,start,num,samps);
    }
  }
  LUX_INLINE void lxGFXRenderFlag_checked(lxGFXContextPTR ctx, flags32 needed){
    flags32 changed = (needed ^ ctx->rflag);
    if (changed){
      lxGFXRenderFlag_apply(ctx,needed,changed);
    }
  }

  LUX_INLINE void lxGFXViewPortScissor_checked(lxGFXContextPTR ctx, const lxRectangleiPTR rect){
    if (rect){
      glScissor(rect->x,rect->y,rect->width,rect->height);
      memcpy(&ctx->viewport.scissorRect,rect,sizeof(lxRectanglei_t));
    }
    if ((rect != NULL) != ctx->viewport.scissor){
      lxGFXViewPortScissor_applyState(ctx, (rect != NULL));
    }
  }

  LUX_INLINE void lxGFXVertexDeclaration_checked(lxGFXContextPTR ctx, const lxGFXVertexDeclPTR decl)
  {
    if (ctx->vertex.decl != decl){
      lxGFXVertexDecl_apply(ctx,decl);
    }
  }

  LUX_INLINE void lxGFXVertexAttrib_checked(lxGFXContextPTR ctx ,flags32 needed){
    flags32 changed = (needed ^ ctx->vertex.active);
    if (changed){
      lxGFXVertexAttribs_apply(ctx,needed,changed);
    }
  }

  LUX_INLINE void lxGFXRenderTarget_checked(lxGFXContextPTR ctx, lxGFXRenderTargetType_t type, lxGFXRenderTargetPTR rt)
  {
    if (ctx->rendertargets[type] == rt) return;
    lxGFXRenderTarget_apply(ctx,type,rt);
  }

  LUX_INLINE void lxGFXContext_resetTextures(lxGFXContextPTR ctx)
  {
    memset(ctx->textures,0,sizeof(void*)*LUXGFX_MAX_TEXTURE_IMAGES);
  }

  LUX_INLINE void lxGFXContext_resetVertexStreams(lxGFXContextPTR ctx)
  {
    ctx->vertex.streamvalid = 0;
    ctx->vertex.streamchange = 0;
  }

#ifdef __cplusplus
}
#endif

#endif

