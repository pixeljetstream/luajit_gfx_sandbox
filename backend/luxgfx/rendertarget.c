// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/texture.h>
#include <luxgfx/luxgfx.h>
#include <luxplatform/debug.h>
#include <luxmath/misc.h>
#include <luxcore/scalarmisc.h>

#include "state_inl.h"


LUX_API void lxgRenderTarget_init(lxgContextPTR ctx, lxgRenderTargetPTR rt)
{
  memset(rt,0,sizeof(lxgRenderTarget_t));

  rt->ctx = ctx;

  glGenFramebuffers(1,&rt->vgl.id);
}

LUX_API void lxgRenderTarget_deinit(lxgContextPTR ctx, lxgRenderTargetPTR rt)
{
  glDeleteFramebuffers(1,&rt->vgl.id);
}

LUX_API void lxgRenderTarget_resetAssigns(lxgContextPTR ctx, lxgRenderTargetPTR rt)
{

}



static void lxgRenderTarget_applyAssign(GLenum target, GLenum what,lxgRenderAssignPTR assign)
{
  if (assign->tex){
    lxgTexturePTR tex = assign->tex;
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

void lxgRenderTarget_applyNamedAssign(lxgRenderTargetPTR rt, GLenum what,lxgRenderAssignPTR assign)
{
  GLuint target = rt->vgl.id;

  if (assign->tex){
    lxgTexturePTR tex = assign->tex;
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

LUX_API void lxgRenderTarget_applyAssigns(lxgContextPTR ctx, lxgRenderTargetPTR rt, lxgRenderTargetType_t mode)
{
  GLenum target = l_FBOmode[mode];
  uint i;

  if (!rt->dirty)
    return;

  LUX_DEBUGASSERT(mode < LUXGFX_RENDERTARGETS);

  for (i = 0; i < rt->maxidx; i++){
    if (rt->dirty & (1<<i)){
      lxgRenderTarget_applyAssign(target,l_FBOAssigns[i],&rt->assigns[i]);
    }
  }
  rt->maxidx = 0;
}

LUX_API void  lxgRenderTarget_applyDraw(lxgContextPTR ctx, lxgRenderTargetPTR rt, booln setViewport)
{
  lxgRenderTarget_apply(ctx,LUXGFX_RENDERTARGET_DRAW,rt);
  if (setViewport){
    lxRectanglei_t rect = {0,0,ctx->framebounds.width,ctx->framebounds.height};
    lxgViewPortRect_apply(ctx, &rect);
  }
}

LUX_API void  lxgRenderTarget_apply(lxgContextPTR ctx, lxgRenderTargetType_t mode, lxgRenderTargetPTR rt)
{
  GLenum target = l_FBOmode[mode];
  glBindFramebuffer(target,rt ? rt->vgl.id : 0);

  ctx->rendertargets[mode] = rt;
  if (mode == LUXGFX_RENDERTARGET_DRAW){
    ctx->framebounds = rt ? rt->bounds : ctx->window;
  }
}

LUX_API void lxgRenderTarget_blit(lxgContextPTR ctx, lxgRenderTargetPTR to, lxgRenderTargetPTR from, lxgRenderTargetBlitPTR update, flags32 mask, booln linearFilter)
{
  lxgRenderTarget_checked(ctx,LUXGFX_RENDERTARGET_DRAW,to);
  lxgRenderTarget_checked(ctx,LUXGFX_RENDERTARGET_READ,from);
  glBlitFramebuffer(
    update->fromStart.x,  update->fromStart.y,  update->fromEnd.x,  update->fromEnd.y,
    update->toStart.x,    update->toStart.y,    update->toEnd.x,    update->toEnd.y, mask, linearFilter ? GL_LINEAR : GL_NEAREST);
}

LUX_API void lxgRenderTarget_setAssign(lxgRenderTargetPTR rt, uint idx,lxgRenderAssignPTR assign)
{
  LUX_DEBUGASSERT(idx < LUXGFX_RENDERASSIGNS);
  if (memcmp(&rt->assigns[idx],assign,sizeof(lxgRenderAssign_t)) == 0) return;
  rt->assigns[idx] = *assign;
  rt->dirty |= (1<<idx);
  rt->maxidx = LUX_MAX(rt->maxidx,idx);
}

LUX_API booln lxgRenderTarget_checkSize(lxgRenderTargetPTR rt)
{
  uint i;
  int width = 0;
  int height = 0;
  booln cmp = LUX_FALSE;

  rt->bounds.width = 0;
  rt->bounds.height = 0;
  rt->equalsized = LUX_TRUE;

  for (i = 0; i < LUXGFX_RENDERASSIGNS; i++){
    lxgRenderAssign_t*assign = &rt->assigns[i];
    if (assign->tex){
      lxVec3i_t vec = *lxgTexture_getMipSize(assign->tex,assign->mip);
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
// lxgViewPort


LUX_API void lxgViewPort_sync(lxgContextPTR ctx, lxgViewPortPTR view)
{
  glGetIntegerv(GL_VIEWPORT,&view->viewRect.x);
  glGetDoublev(GL_DEPTH_RANGE,&view->depth.near);
  view->scissor = glIsEnabled(GL_SCISSOR_TEST);
  glGetIntegerv(GL_SCISSOR_BOX,&view->scissorRect.x);
}

LUX_API booln lxgViewPortRect_apply(lxgContextPTR ctx, const lxRectangleiPTR viewRect)
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

LUX_API booln lxgViewPortScissor_applyState(lxgContextPTR ctx, booln state)
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

LUX_API booln lxgViewPort_apply(lxgContextPTR ctx, const lxgViewPortPTR view)
{
  booln res = lxgViewPortRect_apply(ctx,&view->viewRect);
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

LUX_API void   lxgViewPortMrt_apply(lxgContextPTR ctx, const lxgViewPortMrtPTR objmrt)
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

LUX_API void  lxgViewPortMrt_sync(lxgContextPTR ctx, lxgViewPortMrtPTR objmrt)
{
  int i;
  memset(objmrt,0,sizeof(lxgViewPortMrt_t));

  if (! (ctx->capbits & LUXGFX_CAP_SM4)) return;

  for (i = 0; i < ctx->capabilites.drawbuffers; i++){
    glGetIntegeri_v(GL_SCISSOR_BOX,i,&objmrt->scissors[i].x);
    glGetFloati_v(GL_VIEWPORT,i,&objmrt->bounds[i].x);
    glGetDoublei_v(GL_DEPTH_RANGE,i,&objmrt->depths[i].near);
    objmrt->scissored |= glIsEnabledi(i,GL_SCISSOR_TEST) ? (1<<i) : 0;
  }
}
