// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/texture.h>
#include <luxgfx/luxgfx.h>
#include <luxplatform/debug.h>
#include <luxmath/misc.h>
#include <luxcore/scalarmisc.h>

#include "state_inl.h"


LUX_API void lxGFXRenderTarget_init(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt)
{
  memset(rt,0,sizeof(lxGFXRenderTarget_t));

  rt->ctx = ctx;

  glGenFramebuffers(1,&rt->vgl.id);
}

LUX_API void lxGFXRenderTarget_deinit(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt)
{
  glDeleteFramebuffers(1,&rt->vgl.id);
}

LUX_API void lxGFXRenderTarget_resetAssigns(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt)
{

}



static void lxGFXRenderTarget_applyAssign(GLenum target, GLenum what,lxGFXRenderAssignPTR assign)
{
  if (assign->tex){
    lxGFXTexturePTR tex = assign->tex;
    switch (tex->type){
    case LUXGFX_TEXTURE_1D:
      glFramebufferTexture1DEXT(target,what,GL_TEXTURE_1D,tex->vgl.id,assign->mip);
      return;
    case LUXGFX_TEXTURE_2D:
    case LUXGFX_TEXTURE_RECT:
      glFramebufferTexture2DEXT(target,what,tex->vgl.target,tex->vgl.id,assign->mip);
      return;
    case LUXGFX_TEXTURE_3D:
      glFramebufferTexture3DEXT(target,what,GL_TEXTURE_3D,tex->vgl.id,assign->mip,assign->layer);
      return;
    case LUXGFX_TEXTURE_CUBE:
      glFramebufferTextureFaceEXT(target, what, tex->vgl.id,assign->mip, GL_TEXTURE_CUBE_MAP_POSITIVE_X + assign->layer);
      return;
    case LUXGFX_TEXTURE_2DMS:
      glFramebufferTexture2DEXT(target, what, tex->vgl.target,tex->vgl.id,assign->mip);
      return;
    case LUXGFX_TEXTURE_1DARRAY:
    case LUXGFX_TEXTURE_2DARRAY:
    case LUXGFX_TEXTURE_CUBEARRAY:
    case LUXGFX_TEXTURE_2DMSARRAY:
      glFramebufferTextureLayerEXT(target, what, tex->vgl.id,assign->mip,assign->layer);
      return;
    }
  }
  else if (assign->rbuf){
    glFramebufferRenderbufferEXT(target,what,GL_RENDERBUFFER,assign->rbuf->vgl.id);
  }
  else{
    glFramebufferTexture2DEXT(target,what,GL_TEXTURE_2D,0,0);
    glFramebufferRenderbufferEXT(target,what,GL_RENDERBUFFER,0);
  }
}

void lxGFXRenderTarget_applyNamedAssign(lxGFXRenderTargetPTR rt, GLenum what,lxGFXRenderAssignPTR assign)
{
  GLuint target = rt->vgl.id;

  if (assign->tex){
    lxGFXTexturePTR tex = assign->tex;
    switch (tex->type){
    case LUXGFX_TEXTURE_1D:
      glNamedFramebufferTexture1DEXT(target,what,GL_TEXTURE_1D,tex->vgl.id,assign->mip);
      return;
    case LUXGFX_TEXTURE_2D:
    case LUXGFX_TEXTURE_RECT:
      glNamedFramebufferTexture2DEXT(target,what,tex->vgl.target,tex->vgl.id,assign->mip);
      return;
    case LUXGFX_TEXTURE_3D:
      glNamedFramebufferTexture3DEXT(target,what,GL_TEXTURE_3D,tex->vgl.id,assign->mip,assign->layer);
      return;
    case LUXGFX_TEXTURE_CUBE:
      glNamedFramebufferTextureFaceEXT(target, what, tex->vgl.id,assign->mip, GL_TEXTURE_CUBE_MAP_POSITIVE_X + assign->layer);
      return;
    case LUXGFX_TEXTURE_2DMS:
      glNamedFramebufferTexture2DEXT(target, what, tex->vgl.target,tex->vgl.id,assign->mip);
      return;
    case LUXGFX_TEXTURE_1DARRAY:
    case LUXGFX_TEXTURE_2DARRAY:
    case LUXGFX_TEXTURE_CUBEARRAY:
    case LUXGFX_TEXTURE_2DMSARRAY:
      glNamedFramebufferTextureLayerEXT(target, what, tex->vgl.id,assign->mip,assign->layer);
      return;
    }
  }
  else if (assign->rbuf){
    glNamedFramebufferRenderbufferEXT(target,what,GL_RENDERBUFFER,assign->rbuf->vgl.id);
  }
  else{
    glNamedFramebufferTexture2DEXT(target,what,GL_TEXTURE_2D,0,0);
    glNamedFramebufferRenderbufferEXT(target,what,GL_RENDERBUFFER,0);
  }
}

static const GLenum l_FBOmode[]={
  GL_DRAW_FRAMEBUFFER_EXT,
  GL_READ_FRAMEBUFFER_EXT,
};

static const GLenum l_FBOAssigns[LUXGFX_RENDERASSIGNS]= { 
  GL_DEPTH_ATTACHMENT_EXT,
  GL_STENCIL_ATTACHMENT_EXT,
  GL_COLOR_ATTACHMENT0_EXT,
  GL_COLOR_ATTACHMENT1_EXT,
  GL_COLOR_ATTACHMENT2_EXT,
  GL_COLOR_ATTACHMENT3_EXT,
  GL_COLOR_ATTACHMENT4_EXT,
  GL_COLOR_ATTACHMENT5_EXT,
  GL_COLOR_ATTACHMENT6_EXT,
  GL_COLOR_ATTACHMENT7_EXT,
  GL_COLOR_ATTACHMENT8_EXT,
  GL_COLOR_ATTACHMENT9_EXT,
  GL_COLOR_ATTACHMENT10_EXT,
  GL_COLOR_ATTACHMENT11_EXT,
  GL_COLOR_ATTACHMENT12_EXT,
  GL_COLOR_ATTACHMENT13_EXT,
  GL_COLOR_ATTACHMENT14_EXT,
  GL_COLOR_ATTACHMENT15_EXT,
};

LUX_API void lxGFXRenderTarget_applyAssigns(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt, lxGFXRenderTargetType_t mode)
{
  GLenum target = l_FBOmode[mode];
  uint i;

  if (!rt->dirty)
    return;

  LUX_DEBUGASSERT(mode < LUXGFX_RENDERTARGETS);

  for (i = 0; i < rt->maxidx; i++){
    if (rt->dirty & (1<<i)){
      lxGFXRenderTarget_applyAssign(target,l_FBOAssigns[i],&rt->assigns[i]);
    }
  }
  rt->maxidx = 0;
}

LUX_API void  lxGFXRenderTarget_applyDraw(lxGFXContextPTR ctx, lxGFXRenderTargetPTR rt, booln setViewport)
{
  lxGFXRenderTarget_apply(ctx,LUXGFX_RENDERTARGET_DRAW,rt);
  if (setViewport){
    lxRectanglei_t rect = {0,0,ctx->framebounds.width,ctx->framebounds.height};
    lxGFXViewPortRect_apply(ctx, &rect);
  }
}

LUX_API void  lxGFXRenderTarget_apply(lxGFXContextPTR ctx, lxGFXRenderTargetType_t mode, lxGFXRenderTargetPTR rt)
{
  GLenum target = l_FBOmode[mode];
  glBindFramebuffer(target,rt ? rt->vgl.id : 0);

  ctx->rendertargets[mode] = rt;
  if (mode == LUXGFX_RENDERTARGET_DRAW){
    ctx->framebounds = rt ? rt->bounds : ctx->window;
  }
}

LUX_API void lxGFXRenderTarget_blit(lxGFXContextPTR ctx, lxGFXRenderTargetPTR to, lxGFXRenderTargetPTR from, lxGFXRenderTargetBlitPTR update, flags32 mask, booln linearFilter)
{
  lxGFXRenderTarget_checked(ctx,LUXGFX_RENDERTARGET_DRAW,to);
  lxGFXRenderTarget_checked(ctx,LUXGFX_RENDERTARGET_READ,from);
  glBlitFramebuffer(
    update->fromStart.x,  update->fromStart.y,  update->fromEnd.x,  update->fromEnd.y,
    update->toStart.x,    update->toStart.y,    update->toEnd.x,    update->toEnd.y, mask, linearFilter ? GL_LINEAR : GL_NEAREST);
}

LUX_API void lxGFXRenderTarget_setAssign(lxGFXRenderTargetPTR rt, uint idx,lxGFXRenderAssignPTR assign)
{
  LUX_DEBUGASSERT(idx < LUXGFX_RENDERASSIGNS);
  if (memcmp(&rt->assigns[idx],assign,sizeof(lxGFXRenderAssign_t)) == 0) return;
  rt->assigns[idx] = *assign;
  rt->dirty |= (1<<idx);
  rt->maxidx = LUX_MAX(rt->maxidx,idx);
}

LUX_API booln lxGFXRenderTarget_checkSize(lxGFXRenderTargetPTR rt)
{
  uint i;
  int width = 0;
  int height = 0;
  booln cmp = LUX_FALSE;

  rt->bounds.width = 0;
  rt->bounds.height = 0;
  rt->equalsized = LUX_TRUE;

  for (i = 0; i < LUXGFX_RENDERASSIGNS; i++){
    lxGFXRenderAssign_t*assign = &rt->assigns[i];
    if (assign->tex){
      lxVec3i_t vec = *lxGFXTexture_getMipSize(assign->tex,assign->mip);
      width  = vec.x;
      height = vec.y;
    }
    else if (assign->rbuf){
      width = assign->rbuf->width;
      height = assign->rbuf->height;
    }
    if (!cmp){
      rt->bounds.width = width;
      rt->bounds.height = height;
      cmp = LUX_TRUE;
    }
    else if (rt->bounds.width != width || rt->bounds.height != height){
      rt->bounds.width  = LUX_MIN(rt->bounds.width, width);
      rt->bounds.height = LUX_MIN(rt->bounds.height,height);
      rt->equalsized = LUX_FALSE;
    }
  }
  return rt->equalsized;
}



//////////////////////////////////////////////////////////////////////////
// lxGFXViewPort


LUX_API void lxGFXViewPort_sync(lxGFXContextPTR ctx, lxGFXViewPortPTR view)
{
  glGetIntegerv(GL_VIEWPORT,&view->viewRect.x);
  glGetDoublev(GL_DEPTH_RANGE,&view->depth.near);
  view->scissor = glIsEnabled(GL_SCISSOR_TEST);
  glGetIntegerv(GL_SCISSOR_BOX,&view->scissorRect.x);
}

LUX_API booln lxGFXViewPortRect_apply(lxGFXContextPTR ctx, const lxRectangleiPTR viewRect)
{
  glViewport(LUX_ARRAY4UNPACK(&viewRect->x));
  ctx->viewport.viewRect = *viewRect;
  
  if (!ctx->viewport.scissor && !(
    viewRect->x == 0 && 
    viewRect->y == 0 &&
    viewRect->width  == ctx->framebounds.width &&
    viewRect->height == ctx->framebounds.height))
  {
    glEnable(GL_SCISSOR_TEST);
    glScissor(LUX_ARRAY4UNPACK(&viewRect->x));
    return LUX_TRUE;
  }
  else{
    return LUX_FALSE;
  }
  
}

LUX_API booln lxGFXViewPortScissor_applyState(lxGFXContextPTR ctx, booln state)
{
  booln retstate = state;
  if(state){
    glEnable(GL_SCISSOR_TEST);
  }
  else if (!(
    ctx->viewport.viewRect.x == 0 && 
    ctx->viewport.viewRect.y == 0 &&
    ctx->viewport.viewRect.width  == ctx->framebounds.width &&
    ctx->viewport.viewRect.height == ctx->framebounds.height))
  {
    glEnable(GL_SCISSOR_TEST);
    glScissor(LUX_ARRAY4UNPACK(&ctx->viewport.viewRect.x));
    retstate = LUX_TRUE;
  }
  else {
    glDisable(GL_SCISSOR_TEST);
  }
  ctx->viewport.scissor = state;

  return retstate;
}

LUX_API booln lxGFXViewPort_apply(lxGFXContextPTR ctx, const lxGFXViewPortPTR view)
{
  booln res = lxGFXViewPortRect_apply(ctx,&view->viewRect);
  ctx->viewport = *view;
  glDepthRange(view->depth.near,view->depth.far);
  
  if (ctx->viewport.scissor){
    glEnable(GL_SCISSOR_TEST);
    glScissor(LUX_ARRAY4UNPACK(&ctx->viewport.scissorRect.x));
  }
  else{
    glDisable(GL_SCISSOR_TEST);
  }

  return res || ctx->viewport.scissor;
}

LUX_API void   lxGFXViewPortMrt_apply(lxGFXContextPTR ctx, const lxGFXViewPortMrtPTR objmrt)
{
  uint i;
  uint numused = objmrt->numused;

  glViewportArrayv(0,numused,(const float*)objmrt->bounds);
  glScissorArrayv(0,numused,(const int*)objmrt->scissors);
  glDepthRangeArrayv(0,numused,(const double*)objmrt->depths);

  // prep scissor
  for (i = 0; i < objmrt->numused; i++){
    if (objmrt->scissored & (1<<i))
      glEnablei(i,GL_SCISSOR_TEST);
    else
      glDisablei(i,GL_SCISSOR_TEST);
  }
}

LUX_API void  lxGFXViewPortMrt_sync(lxGFXContextPTR ctx, lxGFXViewPortMrtPTR objmrt)
{
  int i;
  memset(objmrt,0,sizeof(lxGFXViewPortMrt_t));

  if (! (ctx->capbits & LUXGFX_CAP_SM4)) return;

  for (i = 0; i < ctx->capabilites.drawbuffers; i++){
    glGetIntegeri_v(GL_SCISSOR_BOX,i,&objmrt->scissors[i].x);
    glGetFloati_v(GL_VIEWPORT,i,&objmrt->bounds[i].x);
    glGetDoublei_v(GL_DEPTH_RANGE,i,&objmrt->depths[i].near);
    objmrt->scissored |= glIsEnabledi(i,GL_SCISSOR_TEST) ? (1<<i) : 0;
  }
}
