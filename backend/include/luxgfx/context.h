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
  enum lxgCapability_e{
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

  typedef enum lxgGPUVendor_e{
    LUXGFX_GPUVENDOR_UNKNOWN,
    LUXGFX_GPUVENDOR_NVIDIA,
    LUXGFX_GPUVENDOR_ATI,
    LUXGFX_GPUVENDOR_INTEL,
  }lxgGPUVendor_t;

  typedef enum lxgGPUDomain_e{
    LUXGFX_GPUDOMAIN_VERTEX,
    LUXGFX_GPUDOMAIN_FRAGMENT,
    LUXGFX_GPUDOMAIN_GEOMETRY,
    LUXGFX_GPUDOMAIN_TESS_EVAL,
    LUXGFX_GPUDOMAIN_TESS_CTRL,
    LUXGFX_GPUDOMAINS,
  }lxgGPUDomain_t;

  typedef enum lxgGPUMode_e{
    LUXGFX_GPUMODE_FIXED,
    LUXGFX_GPUMODE_ASM,
    LUXGFX_GPUMODE_HL,
  }lxgGPUMode_t;


  //////////////////////////////////////////////////////////////////////////
  typedef struct lxgCapabilites_s{
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
  }lxgCapabilites_t;

  typedef struct lxgContext_s{
    flags32             capbits;
    lxgVertexState_t    vertex;
    lxgTexturePTR       textures[LUXGFX_MAX_TEXTURE_IMAGES];
    lxgSamplerPTR       samplers[LUXGFX_MAX_TEXTURE_IMAGES];
    lxgRenderTargetPTR  rendertargets[LUXGFX_RENDERTARGETS];
    lxgTextureImagePTR  images[LUXGFX_MAX_RWTEXTURE_IMAGES];
    lxgBufferPTR        uniform[LUXGFX_MAX_UNIFORM_BUFFERS];

    flags32             rflag;
    uint16              stencilmaskpos;
    uint16              stencilmaskneg;

    lxgAlpha_t          alpha;
    lxgBlend_t          blend;

    lxgDepth_t          depth;
    lxgStencil_t        stencil;
    lxgLogic_t          logic;

    lxgLine_t           line;

    lxgRasterizer_t     rasterizer;
    lxgViewPort_t       viewport;
    lxgFrameBounds_t    framebounds;
    lxgFrameBounds_t    window;

    lxgBlendMrt_t       blendMRT;
    lxgViewPortMrt_t    viewportMRT;
    
    lxgCapabilites_t    capabilites;
  }lxgContext_t;

  //////////////////////////////////////////////////////////////////////////
  LUX_API const char* lxgContext_init(lxgContextPTR ctx);
  LUX_API void  lxgContext_syncStates(lxgContextPTR ctx);
  LUX_API booln lxgContext_checkStates(lxgContextPTR ctx);
  LUX_API void  lxgContext_resetTextures(lxgContextPTR ctx);
  LUX_API void  lxgContext_resetVertexStreams(lxgContextPTR ctx);

  LUX_API void  lxgTexture_apply(lxgContextPTR ctx, uint imageunit, lxgTexturePTR tex);
  LUX_API void  lxgSampler_apply(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR self);
  LUX_API void  lxgTextures_apply(lxgContextPTR ctx, uint start, uint num, lxgTexturePTR *texs);
  LUX_API void  lxgSamplers_apply(lxgContextPTR ctx, uint start, uint num, lxgSamplerPTR *samps);
  LUX_API void  lxgTextureImage_apply(lxgContextPTR ctx, uint imageunit, lxgTextureImagePTR tex);
  LUX_API void  lxgRenderTarget_apply(lxgContextPTR ctx, lxgRenderTargetType_t type, lxgRenderTargetPTR rt);
  LUX_API void  lxgRenderTarget_applyDraw(lxgContextPTR ctx, lxgRenderTargetPTR rt, booln setViewport);

  // perform check if same state, prior to setting
  LUX_API void lxgAlpha_checked(lxgContextPTR ctx, const lxgAlphaPTR );
  LUX_API void lxgBlend_checked(lxgContextPTR ctx, const lxgBlendPTR );
  LUX_API void lxgDepth_checked(lxgContextPTR ctx, const lxgDepthPTR );
  LUX_API void lxgLogic_checked(lxgContextPTR ctx, const lxgLogicPTR );
  LUX_API void lxgLine_checked(lxgContextPTR ctx, const lxgLinePTR );
  LUX_API void lxgStencil_checked(lxgContextPTR ctx, const lxgStencilPTR );
  LUX_API void lxgTexture_checked(lxgContextPTR ctx, uint imageunit, lxgTexturePTR tex);
  LUX_API void lxgSampler_checked(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR samp);
  LUX_API void lxgTextures_checked(lxgContextPTR ctx, uint start, uint num, lxgTexturePTR *texs);
  LUX_API void lxgSamplers_checked(lxgContextPTR ctx, uint start, uint num, lxgSamplerPTR *samps);
  LUX_API void lxgRenderFlag_checked(lxgContextPTR ctx, flags32 needed);
  LUX_API void lxgVertexDeclaration_checked(lxgContextPTR ctx, const lxgVertexDeclPTR decl);
  LUX_API void lxgVertexAttrib_checked(lxgContextPTR ctx, flags32 needed);
  LUX_API void lxgVertexAttrib_checkedFIXED(lxgContextPTR ctx ,flags32 needed);
  LUX_API void lxgRenderTarget_checked(lxgContextPTR ctx, lxgRenderTargetType_t type, lxgRenderTargetPTR rt);
  LUX_API void lxgTextureImage_checked(lxgContextPTR ctx, uint imageunit, lxgTextureImagePTR img);

  LUX_API void  lxgViewPortScissor_checked(lxgContextPTR ctx, const lxRectangleiPTR rect);
  LUX_API booln lxgBuffer_checkedSetUniform(lxgContextPTR ctx, uint idx, lxgBufferPTR buffer);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE void lxgAlpha_checked(lxgContextPTR ctx, const lxgAlphaPTR obj)
  {
    if (memcmp(&ctx->alpha,obj,sizeof(lxgAlpha_t)))
    {
      lxgAlpha_apply(ctx, obj);
    }
  }
  LUX_INLINE void lxgBlend_checked(lxgContextPTR ctx, const lxgBlendPTR obj)
  {
    if (memcmp(&ctx->blend,obj,sizeof(lxgBlend_t)))
    {
      lxgBlend_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxgDepth_checked(lxgContextPTR ctx, const lxgDepthPTR obj)
  {
    if (memcmp(&ctx->depth,obj,sizeof(lxgDepth_t)))
    {
      lxgDepth_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxgLogic_checked(lxgContextPTR ctx, const lxgLogicPTR obj)
  {
    if (memcmp(&ctx->logic,obj,sizeof(lxgLogic_t)))
    {
      lxgLogic_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxgLine_checked(lxgContextPTR ctx, const lxgLinePTR obj)
  {
    if (memcmp(&ctx->line,obj,sizeof(lxgLine_t)))
    {
      lxgLine_apply(ctx,obj);
    }
  }
  LUX_INLINE void lxgStencil_checked(lxgContextPTR ctx, const lxgStencilPTR obj)
  {
    if (memcmp(&ctx->stencil,obj,sizeof(lxgStencil_t)))
    {
      lxgStencil_apply(ctx,obj);
    }
  }

  LUX_INLINE void lxgTexture_checked(lxgContextPTR ctx, uint imageunit, lxgTexturePTR tex)
  {
    if (ctx->textures[imageunit] != tex){
      lxgTexture_apply(ctx,imageunit,tex);
    }
    else{
      glActiveTexture(GL_TEXTURE0_ARB + imageunit);
    }
  }
  LUX_INLINE void lxgSampler_checked(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR sampler)
  {
    if (ctx->samplers[imageunit] != sampler){
      lxgSampler_apply(ctx,imageunit,sampler);
    }
  }
  LUX_INLINE void lxgTextureImage_checked(lxgContextPTR ctx, uint imageunit, lxgTextureImagePTR img)
  {
    if (ctx->images[imageunit] != img){
      lxgTextureImage_apply(ctx,imageunit,img);
    }
  }
  LUX_INLINE booln lxgBuffer_checkedSetUniform(lxgContextPTR ctx, uint idx, lxgBufferPTR buffer)
  {
    booln res = ctx->uniform[idx] != buffer;
    ctx->uniform[idx] = buffer;
    return res;
  }
  LUX_INLINE void lxgTextures_checked(lxgContextPTR ctx, uint start, uint num, lxgTexturePTR *texs)
  {
    LUX_ASSUME(num >= 1 && num <= LUXGFX_MAX_TEXTURE_IMAGES);
    if (memcmp(ctx->textures,texs,sizeof(lxgTexturePTR )*num)){
      lxgTextures_apply(ctx,start,num,texs);
    }
  }
  LUX_INLINE void lxgSamplers_checked(lxgContextPTR ctx, uint start, uint num, lxgSamplerPTR *samps)
  {
    LUX_ASSUME(num >= 1 && num <= LUXGFX_MAX_TEXTURE_IMAGES);
    if (memcmp(ctx->textures,samps,sizeof(lxgSamplerPTR)*num)){
      lxgSamplers_apply(ctx,start,num,samps);
    }
  }
  LUX_INLINE void lxgRenderFlag_checked(lxgContextPTR ctx, flags32 needed){
    flags32 changed = (needed ^ ctx->rflag);
    if (changed){
      lxgRenderFlag_apply(ctx,needed,changed);
    }
  }

  LUX_INLINE void lxgViewPortScissor_checked(lxgContextPTR ctx, const lxRectangleiPTR rect){
    if (rect){
      glScissor(rect->x,rect->y,rect->width,rect->height);
      memcpy(&ctx->viewport.scissorRect,rect,sizeof(lxRectanglei_t));
    }
    if ((rect != NULL) != ctx->viewport.scissor){
      lxgViewPortScissor_applyState(ctx, (rect != NULL));
    }
  }

  LUX_INLINE void lxgVertexDeclaration_checked(lxgContextPTR ctx, const lxgVertexDeclPTR decl)
  {
    if (ctx->vertex.decl != decl){
      lxgVertexDecl_apply(ctx,decl);
    }
  }

  LUX_INLINE void lxgVertexAttrib_checked(lxgContextPTR ctx ,flags32 needed){
    flags32 changed = (needed ^ ctx->vertex.active);
    if (changed){
      lxgVertexAttribs_apply(ctx,needed,changed);
    }
  }

  LUX_INLINE void lxgVertexAttrib_checkedFIXED(lxgContextPTR ctx ,flags32 needed){
    flags32 changed = (needed ^ ctx->vertex.active);
    if (changed){
      lxgVertexAttribs_applyFIXED(ctx,needed,changed);
    }
  }

  LUX_INLINE void lxgRenderTarget_checked(lxgContextPTR ctx, lxgRenderTargetType_t type, lxgRenderTargetPTR rt)
  {
    if (ctx->rendertargets[type] == rt) return;
    lxgRenderTarget_apply(ctx,type,rt);
  }

  LUX_INLINE void lxgContext_resetTextures(lxgContextPTR ctx)
  {
    memset(ctx->textures,0,sizeof(void*)*LUXGFX_MAX_TEXTURE_IMAGES);
  }

  LUX_INLINE void lxgContext_resetVertexStreams(lxgContextPTR ctx)
  {
    ctx->vertex.streamvalid = 0;
    ctx->vertex.streamchange = 0;
  }

#ifdef __cplusplus
}
#endif

#endif

