// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_CONTEXT_H__
#define __LUXLUXGFX_CONTEXT_H__

#include <luxplatform/luxplatform.h>
#include <luxplatform/debug.h>

#include "glconfig.h"
#include "vertex.h"
#include "buffer.h"
#include "texture.h"
#include "state.h"
#include "rendertarget.h"
#include "program.h"

#ifdef __cplusplus
extern "C"{
#endif

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
    LUXGFX_CAP_DEPTHCLAMP = 1<<19,

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
    lxgTexturePTR       textures[LUXGFX_MAX_TEXTURE_IMAGES];
    lxgSamplerCPTR      samplers[LUXGFX_MAX_TEXTURE_IMAGES];
    lxgTextureImageCPTR images[LUXGFX_MAX_RWTEXTURE_IMAGES];
    lxgRenderTargetPTR  rendertargets[LUXGFX_RENDERTARGETS];
    lxgVertexState_t    vertex;
    lxgFeedbackState_t  feedback;
    lxgProgramState_t   program;
    lxgRasterState_t    raster;

    lxgViewPort_t       viewport;
    lxgFrameBounds_t    framebounds;
    lxgFrameBounds_t    window;
    lxgViewPortMrt_t    viewportMRT;
    
    lxgCapabilites_t    capabilites;
  }lxgContext_t;

  //////////////////////////////////////////////////////////////////////////
  LUX_API const char* lxgContext_init(lxgContextPTR ctx);
  LUX_API void  lxgContext_syncRasterStates(lxgContextPTR ctx);
  LUX_API booln lxgContext_checkStates(lxgContextPTR ctx);

  LUX_API void lxgContext_clearVertexState(lxgContextPTR ctx);
  LUX_API void lxgContext_applyVertexAttribs(lxgContextPTR ctx, flags32 attribs, flags32 changed);
  LUX_API void lxgContext_applyVertexAttribsFIXED(lxgContextPTR ctx, flags32 attribs, flags32 changed);

  LUX_API void lxgContext_applyVertexState(lxgContextPTR ctx);
  LUX_API void lxgContext_applyVertexStateFIXED(lxgContextPTR ctx);
  LUX_API void lxgContext_applyVertexStateNV(lxgContextPTR ctx);      //bindless
  LUX_API void lxgContext_applyVertexStateFIXEDNV(lxgContextPTR ctx); //bindless

  LUX_API void lxgContext_setVertexDecl(lxgContextPTR ctx, lxgVertexDeclCPTR decl);
  LUX_API void lxgContext_setVertexDeclStreams(lxgContextPTR ctx, lxgVertexDeclCPTR decl, lxgStreamHostCPTR hosts );
  LUX_API void lxgContext_setVertexStream(lxgContextPTR ctx, uint idx, lxgStreamHostCPTR host);
  LUX_API void lxgContext_invalidateVertexStreams(lxgContextPTR ctx);

  LUX_API void lxgContext_clearFeedbackState(lxgContextPTR ctx);
  LUX_API void lxgContext_applyFeedbackStreams(lxgContextPTR ctx);
  LUX_API void lxgContext_setFeedbackStreams(lxgContextPTR ctx, lxgStreamHostCPTR hosts, int numStreams);
  LUX_API void lxgContext_setFeedbackStream(lxgContextPTR ctx, uint idx, lxgStreamHostCPTR host );
  LUX_API void lxgContext_enableFeedback(lxgContextPTR ctx, lxGLPrimitiveType_t type, int numStreams);
  LUX_API void lxgContext_disableFeedback(lxgContextPTR ctx);
  
  LUX_API void  lxgContext_clearProgramState(lxgContextPTR ctx);
  LUX_API void  lxgContext_applyProgram(  lxgContextPTR ctx, lxgProgramCPTR prog);
  LUX_API void  lxgContext_applyProgramParameters( lxgContextPTR ctx, lxgProgramCPTR prog, uint num, lxgProgramParameterPTR *params, void **data);
  LUX_API void  lxgContext_updateProgramSubroutines( lxgContextPTR ctx, lxgProgramCPTR prog);

    // always apply samplers before textures
  LUX_API void  lxgContext_clearTextureState(lxgContextPTR ctx);
  LUX_API void  lxgContext_setTextureSampler(lxgContextPTR ctx, uint imageunit, flags32 what);
  LUX_API void  lxgContext_changedTextureSampler(lxgContextPTR ctx, uint imageunit, flags32 what);
  LUX_API void  lxgContext_applyTexture( lxgContextPTR ctx, lxgTexturePTR obj, uint imageunit);
  LUX_API void  lxgContext_applyTextures( lxgContextPTR ctx, lxgTexturePTR *texs, uint start, uint num);
  LUX_API void  lxgContext_applySampler( lxgContextPTR ctx, lxgSamplerCPTR obj, uint imageunit);
  LUX_API void  lxgContext_applySamplers( lxgContextPTR ctx, lxgSamplerCPTR *samps, uint start, uint num);
  LUX_API void  lxgContext_applyTextureImages( lxgContextPTR ctx, lxgTextureImageCPTR *imgs, uint start, uint num);
  LUX_API void  lxgContext_applyTextureImage( lxgContextPTR ctx, lxgTextureImageCPTR img, uint imageunit);

  LUX_API void  lxgContext_clearRasterState(lxgContextPTR ctx);
  LUX_API void  lxgContext_applyDepth( lxgContextPTR ctx, lxgDepthCPTR obj);
  LUX_API void  lxgContext_applyLogic( lxgContextPTR ctx, lxgLogicCPTR obj);
  LUX_API void  lxgContext_applyStencil( lxgContextPTR ctx, lxgStencilCPTR obj);
  LUX_API void  lxgContext_applyBlend( lxgContextPTR ctx, lxgBlendCPTR obj);
  LUX_API void  lxgContext_applyColor( lxgContextPTR ctx, lxgColorCPTR obj);
  LUX_API void  lxgContext_applyRasterizer( lxgContextPTR ctx, lxgRasterizerCPTR obj);

  LUX_API void  lxgContext_blitRenderTargets(lxgContextPTR ctx, lxgRenderTargetPTR to, lxgRenderTargetPTR from, 
    lxgRenderTargetBlitPTR update, flags32 mask, booln linearFilter);

  // return the actual scissor state (can be enabled indirectly through viewport != framebounds)
  LUX_API booln lxgContext_applyViewPortRect(lxgContextPTR ctx, lxRectangleiCPTR rect);
  LUX_API booln lxgContext_applyViewPortScissorState(lxgContextPTR ctx, booln state);
  LUX_API booln lxgContext_applyViewPort(lxgContextPTR ctx, lxgViewPortPTR obj);
  
  LUX_API void  lxgContext_applyViewPortMrt(lxgContextPTR ctx, lxgViewPortMrtPTR obj);
  LUX_API void  lxgContext_applyRenderTarget( lxgContextPTR ctx, lxgRenderTargetPTR obj, lxgRenderTargetType_t type);
  LUX_API void  lxgContext_applyRenderTargetDraw( lxgContextPTR ctx, lxgRenderTargetPTR obj, booln setViewport);

  // perform check if same state, prior to setting
  LUX_API void lxgContext_checkedBlend( lxgContextPTR ctx, lxgBlendCPTR obj);
  LUX_API void lxgContext_checkedColor( lxgContextPTR ctx, lxgColorCPTR obj);
  LUX_API void lxgContext_checkedDepth( lxgContextPTR ctx, lxgDepthCPTR obj);
  LUX_API void lxgContext_checkedLogic( lxgContextPTR ctx, lxgLogicCPTR obj);
  LUX_API void lxgContext_checkedStencil( lxgContextPTR ctx, lxgStencilCPTR obj);
  LUX_API void lxgContext_checkedRasterizer( lxgContextPTR ctx, lxgRasterizerCPTR obj);
  LUX_API void lxgContext_checkedTexture( lxgContextPTR ctx, lxgTexturePTR tex, uint imageunit );
  LUX_API void lxgContext_checkedSampler( lxgContextPTR ctx, lxgSamplerCPTR samp, uint imageunit );
  LUX_API void lxgContext_checkedTextureImage( lxgContextPTR ctx, lxgTextureImageCPTR img, uint imageunit);
  LUX_API void lxgContext_checkedTextures( lxgContextPTR ctx, lxgTexturePTR *texs, uint start, uint num );
  LUX_API void lxgContext_checkedSamplers( lxgContextPTR ctx, lxgSamplerCPTR *samps, uint start, uint num );
  LUX_API void lxgContext_checkedTextureImages( lxgContextPTR ctx, lxgTextureImageCPTR* imgs, uint start, uint num );
  LUX_API void lxgContext_checkedRenderFlag( lxgContextPTR ctx, flags32 needed );
  LUX_API void lxgContext_checkedVertexDecl( lxgContextPTR ctx, lxgVertexDeclCPTR decl );
  LUX_API void lxgContext_checkedVertexAttrib( lxgContextPTR ctx, flags32 needed);
  LUX_API void lxgContext_checkedVertexAttribFIXED( lxgContextPTR ctx, flags32 needed);
  LUX_API void lxgContext_checkedRenderTarget( lxgContextPTR ctx, lxgRenderTargetPTR rt, lxgRenderTargetType_t type);
  
  LUX_API void lxgContext_checkedProgram( lxgContextPTR ctx, lxgProgramPTR prog);
  LUX_API void lxgContext_checkedVertex(lxgContextPTR ctx);
  LUX_API void lxgContext_checkedVertexNV(lxgContextPTR ctx);
  LUX_API void lxgContext_checkedVertexFIXED(lxgContextPTR ctx);
  LUX_API void lxgContext_checkedVertexFIXEDNV(lxgContextPTR ctx);

  LUX_API void  lxgContext_checkedViewPortScissor( lxgContextPTR ctx, lxRectangleiCPTR rect);
  LUX_API void  lxgContext_checkedTextureSampler(lxgContextPTR ctx, uint imageunit);
  LUX_API booln lxgContext_setProgramBuffer(lxgContextPTR ctx, uint idx, lxgBufferCPTR buffer);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE void lxgContext_checkedBlend( lxgContextPTR ctx, lxgBlendCPTR obj)
  {
    if (ctx->raster.blendObj != obj)
    {
      lxgContext_applyBlend(ctx, obj);
    }
  }
  LUX_INLINE void lxgContext_checkedDepth( lxgContextPTR ctx, lxgDepthCPTR obj)
  {
    if (ctx->raster.depthObj != obj)
    {
      lxgContext_applyDepth(ctx, obj);
    }
  }
  LUX_INLINE void lxgContext_checkedLogic( lxgContextPTR ctx, lxgLogicCPTR obj)
  {
    if (ctx->raster.logicObj != obj)
    {
      lxgContext_applyLogic(ctx, obj);
    }
  }
  LUX_INLINE void lxgContext_checkedColor( lxgContextPTR ctx, lxgColorCPTR obj)
  {
    if (ctx->raster.colorObj != obj)
    {
      lxgContext_applyColor(ctx, obj);
    }
  }
  LUX_INLINE void lxgContext_checkedRasterizer( lxgContextPTR ctx, lxgRasterizerCPTR obj)
  {
    if (ctx->raster.rasterizerObj != obj)
    {
      lxgContext_applyRasterizer(ctx, obj);
    }
  }
  LUX_INLINE void lxgContext_checkedStencil( lxgContextPTR ctx, lxgStencilCPTR obj)
  {
    if (ctx->raster.stencilObj != obj)
    {
      lxgContext_applyStencil(ctx, obj);
    }
  }
  LUX_INLINE void lxgContext_checkedTexture( lxgContextPTR ctx, lxgTexturePTR tex, uint imageunit )
  {
    LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
    if (ctx->textures[imageunit] != tex){
      lxgContext_applyTexture(ctx, tex,imageunit);
    }
  }
  LUX_INLINE void lxgContext_checkedSampler( lxgContextPTR ctx, lxgSamplerCPTR sampler, uint imageunit )
  {
    LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
    if (ctx->samplers[imageunit] != sampler){
      lxgContext_applySampler(ctx, sampler,imageunit);
    }
  }
  LUX_INLINE void lxgContext_checkedTextureImage( lxgContextPTR ctx, lxgTextureImageCPTR img, uint imageunit) 
  {
    LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
    if (ctx->images[imageunit] != img){
      lxgContext_applyTextureImage(ctx, img,imageunit);
    }
  }

  LUX_INLINE void lxgContext_checkedProgramContext( lxgContextPTR ctx, lxgProgramCPTR prog )
  {
    if (ctx->program.current != prog){
      lxgContext_applyProgram(ctx, prog);
    }
  }

  LUX_INLINE void lxgContext_checkedTextureSampler(lxgContextPTR ctx, uint imageunit)
  {
    lxgTexturePTR  tex = ctx->textures[imageunit];
    lxgSamplerCPTR sampler = ctx->samplers[imageunit];
    LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
    if (tex && sampler && (tex->lastSampler != sampler || sampler->incarnation != tex->lastSamplerIncarnation)){
      lxgContext_changedTextureSampler(ctx,imageunit,LUXGFX_SAMPLERATTRIB_ALL);
    }
  }

  LUX_INLINE booln lxgContext_setProgramBuffer(lxgContextPTR ctx, uint idx, lxgBufferCPTR buffer)
  {
    booln res = ctx->program.uniform[idx] != buffer;
    LUX_DEBUGASSERT(idx < sizeof(ctx->program.uniform)/sizeof(ctx->program.uniform[0]));
    ctx->program.uniform[idx] = buffer;
    return res;
  }
  LUX_INLINE void lxgContext_checkedTextures( lxgContextPTR ctx, lxgTexturePTR *texs, uint start, uint num )
  {
    LUX_ASSUME(num >= 1 && num <= LUXGFX_MAX_TEXTURE_IMAGES);
    LUX_DEBUGASSERT(start + num < LUXGFX_MAX_TEXTURE_IMAGES);
    if (memcmp(&ctx->textures[start],texs,sizeof(lxgTexturePTR )*num)){
      lxgContext_applyTextures(ctx, texs,start,num);
    }
  }
  LUX_INLINE void lxgContext_checkedSamplers( lxgContextPTR ctx, lxgSamplerCPTR *samps, uint start, uint num )
  {
    LUX_ASSUME(num >= 1 && num <= LUXGFX_MAX_TEXTURE_IMAGES);
    LUX_DEBUGASSERT(start + num < LUXGFX_MAX_TEXTURE_IMAGES);
    if (memcmp(&ctx->samplers[start],samps,sizeof(lxgSamplerPTR)*num)){
      lxgContext_applySamplers(ctx, samps,start,num);
    }
  }

  LUX_INLINE void lxgContext_checkedTextureImages( lxgContextPTR ctx, lxgTextureImageCPTR* imgs, uint start, uint num )
  {
    LUX_ASSUME(num >= 1 && num <= LUXGFX_MAX_RWTEXTURE_IMAGES);
    LUX_DEBUGASSERT(start + num < LUXGFX_MAX_RWTEXTURE_IMAGES);
    if (memcmp(&ctx->images[start],imgs,sizeof(lxgTextureImagePTR)*num)){
      lxgContext_applyTextureImages(ctx, imgs,start,num);
    }
  }

  LUX_INLINE void lxgContext_checkedViewPortScissorContext( lxgContextPTR ctx, lxRectangleiCPTR rect){
    if (rect){
      glScissor(rect->x,rect->y,rect->width,rect->height);
      memcpy(&ctx->viewport.scissorRect,rect,sizeof(lxRectanglei_t));
    }
    if ((rect != NULL) != ctx->viewport.scissor){
      lxgContext_applyViewPortScissorState(ctx,(rect != NULL));
    }
  }

  LUX_INLINE void lxgContext_checkedVertexDecl( lxgContextPTR ctx, lxgVertexDeclCPTR decl)
  {
    if (ctx->vertex.decl != decl){
      lxgContext_setVertexDecl(ctx, decl);
    }
  }

  LUX_INLINE void lxgContext_checkedVertexAttrib( lxgContextPTR ctx, flags32 needed){
    flags32 changed = (needed ^ ctx->vertex.active);
    if (changed){
      lxgContext_applyVertexAttribs(ctx, needed,changed);
    }
  }

  LUX_INLINE void lxgContext_checkedVertexAttribFIXED( lxgContextPTR ctx, flags32 needed){
    flags32 changed = (needed ^ ctx->vertex.active);
    if (changed){
      lxgContext_applyVertexAttribsFIXED(ctx, needed,changed);
    }
  }

  LUX_INLINE void lxgContext_checkedRenderTarget( lxgContextPTR ctx, lxgRenderTargetPTR rt, lxgRenderTargetType_t type )
  {
    if (ctx->rendertargets[type] == rt) return;
    lxgContext_applyRenderTarget(ctx, rt,type);
  }

  LUX_INLINE void lxgContext_checkedVertex(lxgContextPTR ctx)
  {
    lxgVertexState_t* vtx = &ctx->vertex;
    flags32 changed = vtx->declchange & vtx->declvalid;
    flags32 streamchanged = vtx->streamchange;
    if (changed || streamchanged ){
      lxgContext_applyVertexState(ctx);
    }
  }

  LUX_INLINE void lxgContext_checkedVertexFIXED(lxgContextPTR ctx)
  {
    lxgVertexState_t* vtx = &ctx->vertex;
    flags32 changed = vtx->declchange & vtx->declvalid;
    flags32 streamchanged = vtx->streamchange;
    if (changed || streamchanged ){
      lxgContext_applyVertexStateFIXED(ctx);
    }
  }

  LUX_INLINE void lxgContext_checkedVertexNV(lxgContextPTR ctx)
  {
    lxgVertexState_t* vtx = &ctx->vertex;
    flags32 changed = vtx->declchange & vtx->declvalid;
    flags32 streamchanged = vtx->streamchange;
    if (changed || streamchanged ){
      lxgContext_applyVertexStateNV(ctx);
    }
  }

  LUX_INLINE void lxgContext_checkedVertexFIXEDNV(lxgContextPTR ctx)
  {
    lxgVertexState_t* vtx = &ctx->vertex;
    flags32 changed = vtx->declchange & vtx->declvalid;
    flags32 streamchanged = vtx->streamchange;
    if (changed || streamchanged ){
      lxgContext_applyVertexStateFIXEDNV(ctx);
    }
  }

  LUX_INLINE void lxgContext_invalidateVertexStreams(lxgContextPTR ctx)
  {
    ctx->vertex.streamvalid = 0;
    ctx->vertex.streamchange = 0;
  }

#ifdef __cplusplus
}
#endif

#endif

