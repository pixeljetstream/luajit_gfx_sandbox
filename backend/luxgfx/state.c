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
#define LUXGL_TO_VID(what)  case LUXGL_##what: return LUXGFX_##what;

//////////////////////////////////////////////////////////////////////////
// 
lxGLCompareMode_t g_VGLcmpModes[LUXGL_COMPARES+1] = {
  LUXGL_COMPARE_NEVER,
  LUXGL_COMPARE_LESS,
  LUXGL_COMPARE_EQUAL,
  LUXGL_COMPARE_LEQUAL,
  LUXGL_COMPARE_GREATER,
  LUXGL_COMPARE_NOTEQUAL,
  LUXGL_COMPARE_GEQUAL,
  LUXGL_COMPARE_ALWAYS,
  LUXGL_COMPARE_DONTEXECUTE,
};


LUX_INLINE static lxGFXCompareMode_t lxGFXCompareMode_get(GLenum mode)
{
  switch (mode)
  {
    LUXGL_TO_VID(COMPARE_NEVER);
    LUXGL_TO_VID(COMPARE_LESS);
    LUXGL_TO_VID(COMPARE_EQUAL);
    LUXGL_TO_VID(COMPARE_LEQUAL);
    LUXGL_TO_VID(COMPARE_GREATER);
    LUXGL_TO_VID(COMPARE_NOTEQUAL);
    LUXGL_TO_VID(COMPARE_GEQUAL);
    LUXGL_TO_VID(COMPARE_ALWAYS);
  default:
    LUX_DEBUGASSERT(0);
    return LUXGFX_COMPARE_ILLEGAL;
  }
}

LUX_INLINE static lxGFXLogicOp_t lxGFXLogicOp_get(GLenum mode)
{
  switch (mode)
  {
    LUXGL_TO_VID(LOGICOP_CLEAR);
    LUXGL_TO_VID(LOGICOP_SET);
    LUXGL_TO_VID(LOGICOP_COPY);
    LUXGL_TO_VID(LOGICOP_INVERTED);
    LUXGL_TO_VID(LOGICOP_NOOP);
    LUXGL_TO_VID(LOGICOP_INVERT);
    LUXGL_TO_VID(LOGICOP_AND);
    LUXGL_TO_VID(LOGICOP_NAND);
    LUXGL_TO_VID(LOGICOP_OR);
    LUXGL_TO_VID(LOGICOP_NOR);
    LUXGL_TO_VID(LOGICOP_XOR);
    LUXGL_TO_VID(LOGICOP_EQUIV);
    LUXGL_TO_VID(LOGICOP_AND_REVERSE);
    LUXGL_TO_VID(LOGICOP_AND_INVERTED);
    LUXGL_TO_VID(LOGICOP_OR_REVERSE);
    LUXGL_TO_VID(LOGICOP_OR_INVERTED);
  default:
    LUX_DEBUGASSERT(0);
    return LUXGFX_LOGICOP_ILLEGAL;
  }
}

LUX_INLINE static GLenum VGLStencilMode_get(lxGFXStencilMode_t mode)
{
  static GLenum modes[] = {
    LUXGL_STENCIL_KEEP,
    LUXGL_STENCIL_ZERO,
    LUXGL_STENCIL_REPLACE,
    LUXGL_STENCIL_INCR_SAT,
    LUXGL_STENCIL_DECR_SAT,
    LUXGL_STENCIL_INVERT,
    LUXGL_STENCIL_INCR,
    LUXGL_STENCIL_DECR,
  };
  return modes[mode];
}

LUX_INLINE static GLenum VGLLogicOp_get(lxGFXLogicOp_t mode)
{
  static GLenum modes[] = {
    LUXGL_LOGICOP_CLEAR,
    LUXGL_LOGICOP_SET,
    LUXGL_LOGICOP_COPY,
    LUXGL_LOGICOP_INVERTED,
    LUXGL_LOGICOP_NOOP,
    LUXGL_LOGICOP_INVERT,
    LUXGL_LOGICOP_AND,
    LUXGL_LOGICOP_NAND,
    LUXGL_LOGICOP_OR,
    LUXGL_LOGICOP_NOR,
    LUXGL_LOGICOP_XOR,
    LUXGL_LOGICOP_EQUIV,
    LUXGL_LOGICOP_AND_REVERSE,
    LUXGL_LOGICOP_AND_INVERTED,
    LUXGL_LOGICOP_OR_REVERSE,
    LUXGL_LOGICOP_OR_INVERTED,
    LUXGL_LOGICOP_SET,
  };
  return modes[mode];
}

LUX_INLINE static lxGFXStencilMode_t lxGFXStencilMode_get(GLenum mode)
{
  switch (mode)
  {
    LUXGL_TO_VID(STENCIL_KEEP);
    LUXGL_TO_VID(STENCIL_ZERO);
    LUXGL_TO_VID(STENCIL_REPLACE);
    LUXGL_TO_VID(STENCIL_INCR_SAT);
    LUXGL_TO_VID(STENCIL_DECR_SAT);
    LUXGL_TO_VID(STENCIL_INVERT);
    LUXGL_TO_VID(STENCIL_INCR);
    LUXGL_TO_VID(STENCIL_DECR);
  default:
    LUX_DEBUGASSERT(0);
    return LUXGFX_STENCIL_UNKNOWN;
  }
}

LUX_INLINE static lxGFXBlendWeight_t lxGFXBlendWeight_get(GLenum mode)
{
  switch (mode)
  {
    LUXGL_TO_VID(BLENDW_ZERO);
    LUXGL_TO_VID(BLENDW_ONE);
    LUXGL_TO_VID(BLENDW_RGB_SRC);
    LUXGL_TO_VID(BLENDW_RGB_DST);
    LUXGL_TO_VID(BLENDW_A_SRC);
    LUXGL_TO_VID(BLENDW_A_DST);
    LUXGL_TO_VID(BLENDW_INVRGB_SRC);
    LUXGL_TO_VID(BLENDW_INVRGB_DST);
    LUXGL_TO_VID(BLENDW_INVA_SRC);
    LUXGL_TO_VID(BLENDW_INVA_DST);
  default:
    LUX_DEBUGASSERT(0);
    return LUXGFX_BLENDW_UNKNOWN;
  }
}

LUX_INLINE static lxGFXBlendEquation_t lxGFXBlendEquation_get(GLenum mode)
{
  switch (mode)
  {
    LUXGL_TO_VID(BLENDE_ADD);
    LUXGL_TO_VID(BLENDE_SUB);
    LUXGL_TO_VID(BLENDE_SUB_REV);
    LUXGL_TO_VID(BLENDE_MIN);
    LUXGL_TO_VID(BLENDE_MAX);
  default:
    LUX_DEBUGASSERT(0);
    return LUXGFX_BLENDE_UNKNOWN;
  }
}

//////////////////////////////////////////////////////////////////////////
// RenderFlag

LUX_API flags32 lxGFXRenderFlag_sync(lxGFXContextPTR ctx){
  flags32 renderflag;
  GLboolean state[4];
  GLint   blah[4];
  renderflag = 0;
  if (glIsEnabled(GL_BLEND))
    renderflag |= LUXGFX_RFLAG_BLEND;
  if (glIsEnabled(GL_NORMALIZE))
    renderflag |= LUXGFX_RFLAG_BLEND;
  if (glIsEnabled(GL_ALPHA_TEST))
    renderflag |= LUXGFX_RFLAG_ALPHATEST;
  if (!glIsEnabled(GL_DEPTH_TEST))
    renderflag |= LUXGFX_RFLAG_NODEPTHTEST;
  if (!glIsEnabled(GL_CULL_FACE))
    renderflag |= LUXGFX_RFLAG_NOCULL;
  if (glIsEnabled(GL_STENCIL_TEST))
    renderflag |= LUXGFX_RFLAG_STENCILTEST;
  if (glIsEnabled(GL_LOGIC_OP_MODE))
    renderflag |= LUXGFX_RFLAG_LOGIC;
  glGetBooleanv(GL_DEPTH_WRITEMASK,state);
  if (!state[0])
    renderflag |= LUXGFX_RFLAG_NODEPTHMASK;
  glGetBooleanv(GL_COLOR_WRITEMASK,state);
  if (!state[0])
    renderflag |= LUXGFX_RFLAG_NOCOLORMASKR;
  if (!state[1])
    renderflag |= LUXGFX_RFLAG_NOCOLORMASKG;
  if (!state[2])
    renderflag |= LUXGFX_RFLAG_NOCOLORMASKB;
  if (!state[3])
    renderflag |= LUXGFX_RFLAG_NOCOLORMASKA;
  glGetBooleanv(GL_STENCIL_WRITEMASK,state);
  if (state[0])
    renderflag |= LUXGFX_RFLAG_STENCILMASK;

  glGetIntegerv(GL_POLYGON_MODE, blah);
  if (blah[1] != GL_FILL)
    renderflag |= LUXGFX_RFLAG_WIRE;

  glGetIntegerv(GL_CULL_FACE,blah);
  if (blah[0] == GL_FRONT)
    renderflag |= LUXGFX_RFLAG_FRONTCULL;

  return renderflag;

}

LUX_API const char* lxGFXRenderFlag_test(lxGFXContextPTR ctx)
{
  flags32 flags = ctx->rflag;
  flags32 current = lxGFXRenderFlag_sync(ctx);
  flags32 changed = current ^ flags;

  CASE_TEST( LUXGFX_RFLAG_NOCULL )
    return("LUXGFX_RFLAG_NOCULL");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_NODEPTHMASK )
    return("LUXGFX_RFLAG_NODEPTHMASK");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_NODEPTHTEST )
    return("LUXGFX_RFLAG_NODEPTHTEST");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_BLEND )
    return("LUXGFX_RFLAG_BLEND");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_ALPHATEST )
    return("LUXGFX_RFLAG_ALPHATEST");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_WIRE)
    return("LUXGFX_RFLAG_WIRE");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_NOCOLORMASK)
    return("LUXGFX_RFLAG_NOCOLORMASK");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_FRONTCULL)
    return("LUXGFX_RFLAG_FRONTCULL");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_STENCILMASK)
    return("LUXGFX_RFLAG_STENCILMASK");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_STENCILTEST)
    return("LUXGFX_RFLAG_STENCILTEST");
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_LOGIC)
    return("LUXGFX_RFLAG_LOGIC");
  CASE_END;
  return NULL;
}

LUX_API void lxGFXRenderFlag_apply(lxGFXContextPTR ctx, flags32 flags, flags32 changed){
  static lxVector4 white = {1,1,1,1};
  static lxVector4 black = {0,0,0,0};

  if (!changed)
    return;

  CASE_TEST( LUXGFX_RFLAG_NOCULL )
    if (LUXGFX_RFLAG_NOCULL & flags)  glDisable(GL_CULL_FACE);
    else      glEnable(GL_CULL_FACE);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_NODEPTHMASK )
    if (LUXGFX_RFLAG_NODEPTHMASK & flags) glDepthMask(GL_FALSE);
    else      glDepthMask(GL_TRUE);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_NODEPTHTEST )
    if (LUXGFX_RFLAG_NODEPTHTEST & flags) glDisable(GL_DEPTH_TEST);
    else      glEnable(GL_DEPTH_TEST);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_BLEND )
    if (LUXGFX_RFLAG_BLEND & flags) glEnable(GL_BLEND);
    else      glDisable(GL_BLEND);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_LOGIC )
    if (LUXGFX_RFLAG_LOGIC & flags) glEnable(GL_LOGIC_OP);
    else      glDisable(GL_LOGIC_OP);
    CASE_END;
  CASE_TEST( LUXGFX_RFLAG_ALPHATEST )
    if (LUXGFX_RFLAG_ALPHATEST & flags) glEnable(GL_ALPHA_TEST);
    else      glDisable(GL_ALPHA_TEST);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_WIRE)
    if (LUXGFX_RFLAG_WIRE & flags)  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_NOCOLORMASK)
    glColorMask(!(flags & LUXGFX_RFLAG_NOCOLORMASKR),!(flags & LUXGFX_RFLAG_NOCOLORMASKG),!(flags & LUXGFX_RFLAG_NOCOLORMASKB),!(flags & LUXGFX_RFLAG_NOCOLORMASKA));
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_FRONTCULL)
    if (LUXGFX_RFLAG_FRONTCULL & flags) glCullFace(GL_FRONT);
    else      glCullFace(GL_BACK);
  CASE_END;
  CASE_TEST( LUXGFX_RFLAG_STENCILMASK)
    if (LUXGFX_RFLAG_STENCILMASK & flags) glStencilMask(ctx->stencilmaskpos);
    else      glStencilMask(ctx->stencilmaskneg);
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
// lxGFXLine

LUX_API void  lxGFXLine_apply(lxGFXContextPTR ctx, const lxGFXLinePTR obj)
{

  glLineWidth(obj->linewidth);
  glPointSize(obj->linewidth);
  if (obj->linestipple){
    if (!ctx->line.linestipple){
      glEnable(GL_LINE_STIPPLE);
    }
    glLineStipple(obj->linefactor,obj->linestipple);
  }
  else if (ctx->line.linestipple){
    glDisable(GL_LINE_STIPPLE);
  }

  ctx->line = *obj;
}

LUX_API void  lxGFXLine_sync(lxGFXContextPTR ctx, lxGFXLinePTR obj)
{
  GLint val;
  glGetIntegerv(GL_LINE_STIPPLE_PATTERN,&val);
  obj->linestipple = glIsEnabled(GL_LINE_STIPPLE) ? val : 0;
  glGetIntegerv(GL_LINE_STIPPLE_REPEAT,&val);
  obj->linefactor = val;
  glGetFloatv(GL_LINE_WIDTH,&obj->linewidth);
}

//////////////////////////////////////////////////////////////////////////
// lxGFXLogic

LUX_API void  lxGFXLogic_init(lxGFXLogicPTR obj)
{

}

LUX_API void  lxGFXLogic_apply(lxGFXContextPTR ctx, const lxGFXLogicPTR obj)
{
  glLogicOp(VGLLogicOp_get(obj->op));

  ctx->logic = *obj;
}

LUX_API void  lxGFXLogic_sync(lxGFXContextPTR ctx, lxGFXLogicPTR obj)
{
  GLint func;
  glGetIntegerv(GL_LOGIC_OP_MODE,&func);
  obj->op = lxGFXLogicOp_get((GLenum)func);
}


//////////////////////////////////////////////////////////////////////////
// lxGFXDepth

LUX_API void  lxGFXDepth_sync(lxGFXContextPTR ctx, lxGFXDepthPTR state)
{
  GLint func;
  glGetIntegerv(GL_DEPTH_FUNC,&func);

  state->clip = LUX_FALSE;
  state->func = lxGFXCompareMode_get(func);
}


LUX_API void  lxGFXDepth_apply(lxGFXContextPTR ctx, const lxGFXDepthPTR state)
{
  glDepthFunc(VGLCompareMode_get(state->func));

  ctx->depth = *state;
}

//////////////////////////////////////////////////////////////////////////
// lxGFXAlpha


LUX_API void  lxGFXAlpha_sync(lxGFXContextPTR ctx, lxGFXAlphaPTR state)
{
  GLenum func;

  glGetIntegerv(GL_ALPHA_TEST_FUNC,(GLint*)&func);
  state->func = lxGFXCompareMode_get(func);

  glGetFloatv(GL_ALPHA_TEST_REF,&state->refval);
}


LUX_API void  lxGFXAlpha_apply(lxGFXContextPTR ctx, const lxGFXAlphaPTR state)
{
  glAlphaFunc(VGLCompareMode_get(state->func),state->refval);

  ctx->alpha = *state;
}

//////////////////////////////////////////////////////////////////////////
// lxGFXBlend

static GLenum l_BlendWeights[] =
{
  LUXGL_BLENDW_ZERO,
  LUXGL_BLENDW_ONE,
  LUXGL_BLENDW_RGB_SRC,
  LUXGL_BLENDW_RGB_DST,
  LUXGL_BLENDW_A_SRC,
  LUXGL_BLENDW_A_DST,
  LUXGL_BLENDW_INVRGB_SRC,
  LUXGL_BLENDW_INVRGB_DST,
  LUXGL_BLENDW_INVA_SRC,
  LUXGL_BLENDW_INVA_DST,
};

static GLenum l_BlendEqu[] =
{
  LUXGL_BLENDE_ADD,
  LUXGL_BLENDE_SUB,
  LUXGL_BLENDE_SUB_REV,
  LUXGL_BLENDE_MIN,
  LUXGL_BLENDE_MAX,
};

//#define BLEND_LAST
LUX_API void  lxGFXBlend_apply(lxGFXContextPTR ctx, const lxGFXBlendPTR obj)
{
#ifdef BLEND_LAST
  enum32 lastequ = (ctx->blend.colormode.equ<<4) | ctx->blend.alphamode.equ;
  enum32 curequ = (obj->colormode.equ<<4) | obj->alphamode.equ;
#endif

  if (memcmp(&obj->alphamode,&obj->colormode,sizeof(lxGFXBlendMode_t)) == 0){
    glBlendFunc(l_BlendWeights[obj->colormode.srcw],l_BlendWeights[obj->colormode.dstw]);
#ifdef BLEND_LAST
    if (lastequ != curequ)
#endif
    {
      glBlendEquation(l_BlendEqu[obj->colormode.equ]);
    }
  }
  else{
#ifdef BLEND_LAST
    if (lastequ != curequ)
#endif
    {
      if (obj->colormode.equ != obj->alphamode.equ){
        glBlendEquationSeparate(l_BlendEqu[obj->colormode.equ],l_BlendEqu[obj->alphamode.equ]);
      }
      else{
        glBlendEquation(l_BlendEqu[obj->colormode.equ]);
      }
    }
    glBlendFuncSeparate(l_BlendWeights[obj->colormode.srcw],l_BlendWeights[obj->colormode.dstw],
      l_BlendWeights[obj->alphamode.srcw],l_BlendWeights[obj->alphamode.dstw]);
  }

  ctx->blend = *obj;
}

LUX_API void  lxGFXBlend_sync(lxGFXContextPTR ctx, lxGFXBlendPTR blend)
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

  blend->colormode.srcw = lxGFXBlendWeight_get(rgbsrc);
  blend->colormode.dstw = lxGFXBlendWeight_get(rgbdst);
  blend->colormode.equ =  lxGFXBlendEquation_get(rgbequ);

  blend->alphamode.srcw = lxGFXBlendWeight_get(asrc);
  blend->alphamode.dstw = lxGFXBlendWeight_get(adst);
  blend->alphamode.equ =  lxGFXBlendEquation_get(aequ);

}

LUX_API void  lxGFXBlendMrt_apply(lxGFXContextPTR ctx, lxGFXBlendMrtPTR objmrt)
{
  int i;

  if (objmrt->enabled && !objmrt->individual){
    lxGFXBlend_apply(ctx,&objmrt->blends[0]);
    return;
  }

  for (i = 0; i < objmrt->numused; i++){
    if (objmrt->enabled & (1<<i)){
      lxGFXBlendPTR obj = &objmrt->blends[i];

      glEnablei(GL_BLEND,i);
      if (objmrt->individual){
        if (memcmp(&obj->alphamode,&obj->colormode,sizeof(lxGFXBlendMode_t)) == 0){
          glBlendFunci(i,l_BlendWeights[obj->colormode.srcw],l_BlendWeights[obj->colormode.dstw]);
          glBlendEquationi(i,l_BlendEqu[obj->colormode.equ]);
        }
        else{
          if (obj->colormode.equ != obj->alphamode.equ){
            glBlendEquationSeparatei(i,l_BlendEqu[obj->colormode.equ],l_BlendEqu[obj->alphamode.equ]);
          }
          else{
            glBlendEquationi(i,l_BlendEqu[obj->colormode.equ]);
          }

          glBlendFuncSeparatei(i,l_BlendWeights[obj->colormode.srcw],l_BlendWeights[obj->colormode.dstw],
            l_BlendWeights[obj->alphamode.srcw],l_BlendWeights[obj->alphamode.dstw]);
        }
      }
    }
    else{
      glDisablei(GL_BLEND,i);
    }
  }
}

LUX_API void  lxGFXBlendMrt_sync(lxGFXContextPTR ctx, lxGFXBlendMrtPTR obj)
{
  int i;
  lxGFXBlendPTR last = NULL;

  memset(obj,0,sizeof(lxGFXBlendMrt_t));

  if (! (ctx->capbits & LUXGFX_CAP_SM3)) return;

  for (i = 0; i < ctx->capabilites.drawbuffers; i++){
    if (glIsEnabledi(GL_BLEND,i)){
      obj->enabled |= (1<<i);
    }

    if (ctx->capbits & LUXGFX_CAP_SM4){
      lxGFXBlendPTR blend = &obj->blends[i];
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

      blend->colormode.srcw = lxGFXBlendWeight_get(rgbsrc);
      blend->colormode.dstw = lxGFXBlendWeight_get(rgbdst);
      blend->colormode.equ =  lxGFXBlendEquation_get(rgbequ);

      blend->alphamode.srcw = lxGFXBlendWeight_get(asrc);
      blend->alphamode.dstw = lxGFXBlendWeight_get(adst);
      blend->alphamode.equ =  lxGFXBlendEquation_get(aequ);

      if (last && memcmp(last,obj,sizeof(lxGFXBlend_t)))
        obj->individual = LUX_TRUE;

      last = blend;
      
    }

  }
}



//////////////////////////////////////////////////////////////////////////
// lxGFXStencil


LUX_API void lxGFXStencil_sync(lxGFXContextPTR ctx, lxGFXStencilPTR stencil)
{
  GLenum val;
  glGetIntegerv(GL_STENCIL_FAIL,(GLint*)&val);
  stencil->ops[0].fail = lxGFXStencilMode_get(val);
  glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL,(GLint*)&val);
  stencil->ops[0].zfail = lxGFXStencilMode_get(val);
  glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS,(GLint*)&val);
  stencil->ops[0].zpass = lxGFXStencilMode_get(val);
  glGetIntegerv(GL_STENCIL_FUNC,(GLint*)&val);
  stencil->ops[0].func = lxGFXCompareMode_get(val);

  glGetIntegerv(GL_STENCIL_BACK_FAIL,(GLint*)&val);
  stencil->ops[1].fail = lxGFXStencilMode_get(val);
  glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL,(GLint*)&val);
  stencil->ops[1].zfail = lxGFXStencilMode_get(val);
  glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS,(GLint*)&val);
  stencil->ops[1].zpass = lxGFXStencilMode_get(val);
  glGetIntegerv(GL_STENCIL_BACK_FUNC,(GLint*)&val);
  stencil->ops[1].func = lxGFXCompareMode_get(val);
}

LUX_API void lxGFXStencil_apply(lxGFXContextPTR ctx, const lxGFXStencilPTR vidstencil)
{
  if (memcmp(&vidstencil->ops[0],&vidstencil->ops[1],sizeof(lxGFXStencilOp_t))){
    glStencilFuncSeparate(GL_FRONT, VGLCompareMode_get((lxGFXCompareMode_t)vidstencil->ops[0].func),
      vidstencil->refvalue,vidstencil->mask);
    glStencilFuncSeparate(GL_BACK,  VGLCompareMode_get((lxGFXCompareMode_t)vidstencil->ops[1].func),
      vidstencil->refvalue,vidstencil->mask);

    glStencilOpSeparate(GL_FRONT,VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[0].fail),
      VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[0].zfail),
      VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[0].zpass));
    glStencilOpSeparate(GL_BACK,  VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[1].fail),
      VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[1].zfail),
      VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[1].zpass));
  }
  else{
    glStencilFunc(VGLCompareMode_get((lxGFXCompareMode_t)vidstencil->ops[0].func),vidstencil->refvalue,vidstencil->mask);
    glStencilOp(VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[0].fail),
      VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[0].zfail),
      VGLStencilMode_get((lxGFXStencilMode_t)vidstencil->ops[0].zpass));
  }

  ctx->stencil = *vidstencil;
}

//////////////////////////////////////////////////////////////////////////
// lxGFXRasterizer


LUX_API void lxGFXRasterizer_sync(lxGFXContextPTR ctx, lxGFXRasterizerPTR raster)
{
  GLint val;
  glGetIntegerv(GL_FRONT_FACE,&val);
  raster->wind = val == GL_CCW ? LUXGFX_WIND_CCW : LUXGFX_WIND_CW;

  raster->multisample = glIsEnabled(GL_MULTISAMPLE_ARB);
  glGetFloatv(GL_POLYGON_OFFSET_FACTOR,&raster->polyoffsetslope);
  glGetFloatv(GL_POLYGON_OFFSET_UNITS,&raster->polyoffset);
}


