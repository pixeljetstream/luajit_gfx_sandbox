// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/state.h>
#include <luxplatform/debug.h>
#include <luxmath/basetypes.h>
#include <luxmath/misc.h>
#include <string.h>
#include "state_inl.h"

#define CASE_TEST(what)   if( changed & what){
#define CASE_END      }
#define LUXGL_TO_GFX(what)  case LUXGL_##what: return LUXGFX_##what;


//////////////////////////////////////////////////////////////////////////
// RenderFlag

LUX_API flags32 lxgRenderFlag_init()
{
  flags32 rflag = 0;
  rflag |= LUXGFX_RFLAG_COLORWRITE;
  rflag |= LUXGFX_RFLAG_DEPTHWRITE;
  rflag |= LUXGFX_RFLAG_DEPTHTEST;
  rflag |= LUXGFX_RFLAG_CULL;
  rflag |= LUXGFX_RFLAG_CCW;

  return rflag;
}

LUX_API flags32 lxgRenderFlag_sync(lxgContextPTR ctx){
  flags32 renderflag;
  GLboolean state[4];
  GLint   blah[4];
  renderflag = 0;
  if (glIsEnabled(GL_BLEND))
    renderflag |= LUXGFX_RFLAG_BLEND;
  if (glIsEnabled(GL_DEPTH_TEST))
    renderflag |= LUXGFX_RFLAG_DEPTHTEST;
  if (glIsEnabled(GL_CULL_FACE))
    renderflag |= LUXGFX_RFLAG_CULL;
  if (glIsEnabled(GL_STENCIL_TEST))
    renderflag |= LUXGFX_RFLAG_STENCILTEST;
  if (glIsEnabled(GL_LOGIC_OP_MODE))
    renderflag |= LUXGFX_RFLAG_LOGIC;
  glGetBooleanv(GL_DEPTH_WRITEMASK,state);
  if (state[0])
    renderflag |= LUXGFX_RFLAG_DEPTHWRITE;
  glGetBooleanv(GL_COLOR_WRITEMASK,state);
  if (state[0])
    renderflag |= LUXGFX_RFLAG_COLORWRITER;
  if (state[1])
    renderflag |= LUXGFX_RFLAG_COLORWRITEG;
  if (state[2])
    renderflag |= LUXGFX_RFLAG_COLORWRITEB;
  if (state[3])
    renderflag |= LUXGFX_RFLAG_COLORWRITEA;
  glGetBooleanv(GL_STENCIL_WRITEMASK,state);
  if (state[0])
    renderflag |= LUXGFX_RFLAG_STENCILWRITE;

  glGetIntegerv(GL_POLYGON_MODE, blah);
  if (blah[1] != GL_FILL)
    renderflag |= LUXGFX_RFLAG_WIRE;

  glGetIntegerv(GL_CULL_FACE,blah);
  if (blah[0] == GL_FRONT)
    renderflag |= LUXGFX_RFLAG_FRONTCULL;

  glGetIntegerv(GL_FRONT_FACE,blah);
  if (blah[0] == GL_CCW)
    renderflag |= LUXGFX_RFLAG_CCW;

  return renderflag;

}

LUX_API const char* lxgRenderFlag_test(lxgContextPTR ctx)
{
  flags32 flags = ctx->rflag;
  flags32 current = lxgRenderFlag_sync(ctx);
  flags32 changed = current ^ flags;

  CASE_TEST( LUXGFX_RFLAG_CULL )
    return("LUXGFX_RFLAG_CULL");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_CCW)
    return("LUXGFX_RFLAG_CCW");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_DEPTHWRITE )
    return("LUXGFX_RFLAG_DEPTHWRITE");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_DEPTHTEST )
    return("LUXGFX_RFLAG_DEPTHTEST");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_BLEND )
    return("LUXGFX_RFLAG_BLEND");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_WIRE)
    return("LUXGFX_RFLAG_WIRE");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_COLORWRITE)
    return("LUXGFX_RFLAG_COLORWRITE");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_FRONTCULL)
    return("LUXGFX_RFLAG_FRONTCULL");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_STENCILWRITE)
    return("LUXGFX_RFLAG_STENCILWRITE");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_STENCILTEST)
    return("LUXGFX_RFLAG_STENCILTEST");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_LOGIC)
    return("LUXGFX_RFLAG_LOGIC");
  CASE_END;
  return NULL;
}

LUX_API void lxgRenderFlag_apply(flags32 flags, lxgContextPTR ctx, flags32 changed){
  if (!changed)
    return;

  CASE_TEST( LUXGFX_RFLAG_CULL )
    if (LUXGFX_RFLAG_CULL & flags)  glEnable(GL_CULL_FACE);
    else      glDisable(GL_CULL_FACE);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_CCW )
    if (LUXGFX_RFLAG_CCW & flags)  glFrontFace(GL_CCW);
    else      glFrontFace(GL_CW);
    CASE_END;
  CASE_TEST( LUXGFX_RFLAG_DEPTHWRITE )
    glDepthMask(!!(LUXGFX_RFLAG_DEPTHWRITE & flags));
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_DEPTHTEST )
    if (LUXGFX_RFLAG_DEPTHTEST & flags) glEnable(GL_DEPTH_TEST);
    else      glDisable(GL_DEPTH_TEST);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_BLEND )
    if (LUXGFX_RFLAG_BLEND & flags) glEnable(GL_BLEND);
    else      glDisable(GL_BLEND);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_LOGIC )
    if (LUXGFX_RFLAG_LOGIC & flags) glEnable(GL_COLOR_LOGIC_OP);
    else      glDisable(GL_COLOR_LOGIC_OP);
    CASE_END;
  CASE_TEST( LUXGFX_RFLAG_WIRE)
    if (LUXGFX_RFLAG_WIRE & flags)  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_COLORWRITE)
    glColorMask(!!(flags & LUXGFX_RFLAG_COLORWRITER),!!(flags & LUXGFX_RFLAG_COLORWRITEG),!!(flags & LUXGFX_RFLAG_COLORWRITEB),!!(flags & LUXGFX_RFLAG_COLORWRITEA));
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_FRONTCULL)
    if (LUXGFX_RFLAG_FRONTCULL & flags) glCullFace(GL_FRONT);
    else      glCullFace(GL_BACK);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_STENCILWRITE)
    glStencilMask( (flags) & 255);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_STENCILTEST)
    if (LUXGFX_RFLAG_STENCILTEST & flags) glEnable(GL_STENCIL_TEST);
    else      glDisable(GL_STENCIL_TEST);
  CASE_END;

  ctx->rflag = flags;
}

#undef CASE_TEST
#undef CASE_END


//////////////////////////////////////////////////////////////////////////
// lxgLogic

LUX_API void  lxgLogic_init(lxgLogicPTR obj)
{
  obj->op = LUXGL_LOGICOP_COPY;
}

LUX_API void  lxgLogic_apply(lxgLogicPTR obj, lxgContextPTR ctx)
{
  glLogicOp(obj->op);

  ctx->logic = *obj;
}

LUX_API void  lxgLogic_sync(lxgLogicPTR obj, lxgContextPTR ctx)
{
  GLint func;
  glGetIntegerv(GL_LOGIC_OP_MODE,&func);
  obj->op = ((GLenum)func);
}


//////////////////////////////////////////////////////////////////////////
// lxgDepth
LUX_API void  lxgDepth_init(lxgDepthPTR obj)
{
  obj->func = LUXGL_COMPARE_LEQUAL;
}

LUX_API void  lxgDepth_sync(lxgDepthPTR state, lxgContextPTR ctx)
{
  GLint func;
  glGetIntegerv(GL_DEPTH_FUNC,&func);
  state->func = (func);
}

LUX_API void  lxgDepth_apply(lxgDepthPTR state, lxgContextPTR ctx)
{
  glDepthFunc((state->func));
  ctx->depth = *state;
}

//////////////////////////////////////////////////////////////////////////
// lxgBlend
LUX_API void  lxgBlend_init(lxgBlendPTR obj)
{
  obj->alphamode.dstw = LUXGL_BLENDW_ZERO;
  obj->alphamode.srcw = LUXGL_BLENDW_ONE;
  obj->alphamode.equ  = LUXGL_BLENDE_ADD;
  obj->colormode.dstw = LUXGL_BLENDW_ZERO;
  obj->colormode.srcw = LUXGL_BLENDW_ONE;
  obj->colormode.equ  = LUXGL_BLENDE_ADD;
}

LUX_API void  lxgBlend_apply(lxgBlendPTR obj, lxgContextPTR ctx)
{
  if (obj->colormode.equ != obj->alphamode.equ){
    glBlendEquationSeparate((obj->colormode.equ),(obj->alphamode.equ));
  }
  else{
    glBlendEquation((obj->colormode.equ));
  }
  glBlendFuncSeparate((obj->colormode.srcw),(obj->colormode.dstw),
    (obj->alphamode.srcw),(obj->alphamode.dstw));

  ctx->blend = *obj;
}

LUX_API void  lxgBlend_sync(lxgBlendPTR blend, lxgContextPTR ctx)
{
  GLenum rgbsrc;
  GLenum rgbdst;
  GLenum rgbequ;

  GLenum asrc;
  GLenum adst;
  GLenum aequ;

  glGetIntegerv(GL_BLEND_SRC_RGB,(GLint*)&rgbsrc);
  glGetIntegerv(GL_BLEND_DST_RGB,(GLint*)&rgbdst);
  glGetIntegerv(GL_BLEND_EQUATION_RGB,(GLint*)&rgbequ);

  glGetIntegerv(GL_BLEND_SRC_ALPHA,(GLint*)&asrc);
  glGetIntegerv(GL_BLEND_DST_ALPHA,(GLint*)&adst);

  if (ctx->capbits & LUXGFX_CAP_BLENDSEP){
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA,(GLint*)&aequ);
  }
  else{
    aequ = rgbequ;
  }

  blend->colormode.srcw = (rgbsrc);
  blend->colormode.dstw = (rgbdst);
  blend->colormode.equ =  (rgbequ);

  blend->alphamode.srcw = (asrc);
  blend->alphamode.dstw = (adst);
  blend->alphamode.equ =  (aequ);

}

LUX_API void  lxgBlendMrt_apply(lxgBlendMrtPTR objmrt, lxgContextPTR ctx)
{
  int i;

  if (objmrt->enabled && !objmrt->individual){
    lxgBlend_apply(&objmrt->blends[0],ctx);
    return;
  }

  for (i = 0; i < objmrt->numused; i++){
    if (objmrt->enabled & (1<<i)){
      lxgBlendPTR obj = &objmrt->blends[i];

      glEnablei(GL_BLEND,i);
      if (objmrt->individual){
        if (memcmp(&obj->alphamode,&obj->colormode,sizeof(lxgBlendMode_t)) == 0){
          glBlendFunci(i,(obj->colormode.srcw),(obj->colormode.dstw));
          glBlendEquationi(i,(obj->colormode.equ));
        }
        else{
          if (obj->colormode.equ != obj->alphamode.equ){
            glBlendEquationSeparatei(i,(obj->colormode.equ),(obj->alphamode.equ));
          }
          else{
            glBlendEquationi(i,(obj->colormode.equ));
          }

          glBlendFuncSeparatei(i,(obj->colormode.srcw),(obj->colormode.dstw),
            (obj->alphamode.srcw),(obj->alphamode.dstw));
        }
      }
    }
    else{
      glDisablei(GL_BLEND,i);
    }
  }
}

LUX_API void  lxgBlendMrt_sync(lxgBlendMrtPTR obj, lxgContextPTR ctx)
{
  int i;
  lxgBlendPTR last = NULL;

  memset(obj,0,sizeof(lxgBlendMrt_t));

  if (! (ctx->capbits & LUXGFX_CAP_SM3)) return;

  for (i = 0; i < ctx->capabilites.drawbuffers; i++){
    if (glIsEnabledi(GL_BLEND,i)){
      obj->enabled |= (1<<i);
    }

    if (ctx->capbits & LUXGFX_CAP_SM4){
      lxgBlendPTR blend = &obj->blends[i];
      GLenum rgbsrc;
      GLenum rgbdst;
      GLenum rgbequ;

      GLenum asrc;
      GLenum adst;
      GLenum aequ;

      glGetIntegeri_v(GL_BLEND_SRC_RGB,i,(GLint*)&rgbsrc);
      glGetIntegeri_v(GL_BLEND_DST_RGB,i,(GLint*)&rgbdst);
      glGetIntegeri_v(GL_BLEND_EQUATION_RGB,i,(GLint*)&rgbequ);

      glGetIntegeri_v(GL_BLEND_SRC_ALPHA,i,(GLint*)&asrc);
      glGetIntegeri_v(GL_BLEND_DST_ALPHA,i,(GLint*)&adst);
      glGetIntegeri_v(GL_BLEND_EQUATION_ALPHA,i,(GLint*)&aequ);

      blend->colormode.srcw = (rgbsrc);
      blend->colormode.dstw = (rgbdst);
      blend->colormode.equ =  (rgbequ);

      blend->alphamode.srcw = (asrc);
      blend->alphamode.dstw = (adst);
      blend->alphamode.equ =  (aequ);

      if (last && memcmp(last,obj,sizeof(lxgBlend_t)))
        obj->individual = LUX_TRUE;

      last = blend;
      
    }

  }
}



//////////////////////////////////////////////////////////////////////////
// lxgStencil
LUX_API void  lxgStencil_init(lxgStencilPTR stencil)
{
  stencil->ops[0].fail = LUXGL_STENCIL_KEEP;
  stencil->ops[0].zfail = LUXGL_STENCIL_KEEP;
  stencil->ops[0].zpass = LUXGL_STENCIL_KEEP;
  stencil->ops[0].func = LUXGL_COMPARE_ALWAYS;
  stencil->ops[1].fail = LUXGL_STENCIL_KEEP;
  stencil->ops[1].zfail = LUXGL_STENCIL_KEEP;
  stencil->ops[1].zpass = LUXGL_STENCIL_KEEP;
  stencil->ops[1].func = LUXGL_COMPARE_ALWAYS;
  stencil->refvalue = 0;
  stencil->mask = (1<<8)-1;
}

LUX_API void lxgStencil_sync(lxgStencilPTR stencil, lxgContextPTR ctx)
{
  GLenum val;
  glGetIntegerv(GL_STENCIL_FAIL,(GLint*)&val);
  stencil->ops[0].fail = (val);
  glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL,(GLint*)&val);
  stencil->ops[0].zfail = (val);
  glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS,(GLint*)&val);
  stencil->ops[0].zpass = (val);
  glGetIntegerv(GL_STENCIL_FUNC,(GLint*)&val);
  stencil->ops[0].func = (val);

  glGetIntegerv(GL_STENCIL_BACK_FAIL,(GLint*)&val);
  stencil->ops[1].fail = (val);
  glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL,(GLint*)&val);
  stencil->ops[1].zfail = (val);
  glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS,(GLint*)&val);
  stencil->ops[1].zpass = (val);
  glGetIntegerv(GL_STENCIL_BACK_FUNC,(GLint*)&val);
  stencil->ops[1].func = (val);
}

LUX_API void lxgStencil_apply(lxgStencilPTR vidstencil, lxgContextPTR ctx)
{
  if (memcmp(&vidstencil->ops[0],&vidstencil->ops[1],sizeof(lxgStencilOp_t))){
    glStencilFuncSeparate(GL_FRONT, vidstencil->ops[0].func,
      vidstencil->refvalue,vidstencil->mask);
    glStencilFuncSeparate(GL_BACK,  vidstencil->ops[1].func,
      vidstencil->refvalue,vidstencil->mask);

    glStencilOpSeparate(GL_FRONT,vidstencil->ops[0].fail,
      (vidstencil->ops[0].zfail),
      (vidstencil->ops[0].zpass));
    glStencilOpSeparate(GL_BACK,  vidstencil->ops[1].fail,
      (vidstencil->ops[1].zfail),
      (vidstencil->ops[1].zpass));
  }
  else{
    glStencilFunc((vidstencil->ops[0].func),vidstencil->refvalue,vidstencil->mask);
    glStencilOp((vidstencil->ops[0].fail),
      (vidstencil->ops[0].zfail),
      (vidstencil->ops[0].zpass));
  }

  ctx->stencil = *vidstencil;
}


