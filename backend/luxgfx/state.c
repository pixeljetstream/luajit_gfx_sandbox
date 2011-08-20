// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxgfx/state.h>
#include <luxinia/luxplatform/debug.h>
#include <luxinia/luxmath/basetypes.h>
#include <luxinia/luxmath/misc.h>
#include <string.h>
#include "state_inl.h"

//////////////////////////////////////////////////////////////////////////

LUX_API void  lxgRasterizer_init(lxgRasterizerPTR obj)
{
  obj->cullfront = LUX_FALSE;
  obj->cull = LUX_TRUE;
  obj->fill = GL_POLYGON;
}

LUX_API void  lxgRasterizer_sync(lxgRasterizerPTR obj, lxgContextPTR ctx)
{
  GLenum mode;
  glGetIntegerv(GL_CULL_FACE_MODE, &mode);
  obj->cullfront = mode == GL_FRONT;
  obj->cull = glIsEnabled(GL_CULL_FACE);
  glGetIntegerv(GL_POLYGON_MODE, &obj->fill);
}

LUX_API void lxgContext_applyRasterizer(lxgContextPTR ctx, lxgRasterizerCPTR obj)
{
  if (obj->cull){
    glEnable(GL_CULL_FACE);
    glCullFace(obj->cullfront ? GL_FRONT : GL_BACK);
  }
  else{
    glDisable(GL_CULL_FACE);
  }
  glPolygonMode(GL_FRONT_AND_BACK,obj->fill);
}


//////////////////////////////////////////////////////////////////////////


LUX_API void lxgColor_init( lxgColorPTR obj )
{
  int i;
  for (i = 0; i < LUXGFX_MAX_RENDERTARGETS; i++){
    obj->write[i][0] = GL_TRUE;
    obj->write[i][1] = GL_TRUE;
    obj->write[i][2] = GL_TRUE;
    obj->write[i][3] = GL_TRUE;
  }
  obj->individual = LUX_FALSE;
}


LUX_API void lxgColor_sync( lxgColorPTR obj, lxgContextPTR ctx )
{
  LUX_DEBUGASSERT(sizeof(obj->write[0][0]) == sizeof(GLboolean));

  lxgColor_init(obj);
  glGetBooleanv(GL_COLOR_WRITEMASK,obj->write[0]);

  if (ctx->capbits & LUXGFX_CAP_SM4){
    int i;
    for (i = 0; i < ctx->capabilites.drawbuffers; i++){
      glGetBooleani_v(GL_COLOR_WRITEMASK,0,obj->write[i]);
    }
  }
}

LUX_API void lxgContext_applyColor( lxgContextPTR ctx, lxgColorCPTR obj )
{
  if (!obj->individual){
    glColorMask(obj->write[0][0], obj->write[0][1], obj->write[0][2], obj->write[0][3]);
  }
  else{
    int i;
    for (i = 0; i < ctx->capabilites.drawbuffers; i++){
      glColorMaski(i,obj->write[i][0], obj->write[i][1], obj->write[i][2], obj->write[i][3]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// lxgLogic

LUX_API void  lxgLogic_init(lxgLogicPTR obj)
{
  obj->enabled = LUX_FALSE;
  obj->op = LUXGL_LOGICOP_COPY;
}

LUX_API void  lxgContext_applyLogic( lxgContextPTR ctx, lxgLogicCPTR obj)
{
  if (obj->enabled){
    glEnable(GL_LOGIC_OP);
    glLogicOp(obj->op);
  }
  else{
    glDisable(GL_LOGIC_OP);
  }

  ctx->raster.logic = *obj;
  ctx->raster.logicObj = obj;
}

LUX_API void  lxgLogic_sync( lxgLogicPTR obj, lxgContextPTR ctx)
{
  GLint func;
  glGetIntegerv(GL_LOGIC_OP_MODE,&func);
  obj->enabled = glIsEnabled(GL_LOGIC_OP);
  obj->op = ((GLenum)func);
}


//////////////////////////////////////////////////////////////////////////
// lxgDepth
LUX_API void  lxgDepth_init(lxgDepthPTR obj)
{
  obj->enabled = LUX_TRUE;
  obj->write = LUX_TRUE;
  obj->func = LUXGL_COMPARE_LEQUAL;
}

LUX_API void  lxgDepth_sync( lxgDepthPTR obj, lxgContextPTR ctx )
{
  GLint func;
  GLboolean state;
  glGetIntegerv(GL_DEPTH_FUNC,&func);
  glGetBooleanv(GL_DEPTH_WRITEMASK,&state);
  obj->enabled = glIsEnabled(GL_DEPTH_TEST);
  obj->write = state;
  obj->func = func;
}

LUX_API void  lxgContext_applyDepth( lxgContextPTR ctx, lxgDepthCPTR obj)
{
  if (obj->enabled){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(obj->func);
  }
  else{
    glDisable(GL_DEPTH_TEST);
  }
  glDepthMask(!!obj->write);

  ctx->raster.depth = *obj;
  ctx->raster.depthObj = obj;
}

//////////////////////////////////////////////////////////////////////////
// lxgBlend
static void  lxgBlendStage_init(lxgBlendStage_t* obj)
{
  obj->enabled  = LUX_FALSE;
  obj->alphamode.dstw = LUXGL_BLENDW_ZERO;
  obj->alphamode.srcw = LUXGL_BLENDW_ONE;
  obj->alphamode.equ  = LUXGL_BLENDE_ADD;
  obj->colormode.dstw = LUXGL_BLENDW_ZERO;
  obj->colormode.srcw = LUXGL_BLENDW_ONE;
  obj->colormode.equ  = LUXGL_BLENDE_ADD;
}

static void  lxgContext_applyBlendStage( lxgContextPTR ctx, const lxgBlendStage_t* obj)
{
  if (obj->colormode.equ != obj->alphamode.equ){
    glBlendEquationSeparate((obj->colormode.equ),(obj->alphamode.equ));
  }
  else{
    glBlendEquation((obj->colormode.equ));
  }
  glBlendFuncSeparate((obj->colormode.srcw),(obj->colormode.dstw),
    (obj->alphamode.srcw),(obj->alphamode.dstw));
}

static void  lxgBlendStage_sync( lxgBlendStage_t* obj, lxgContextPTR ctx)
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

  obj->enabled = glIsEnabled(GL_BLEND);

  obj->colormode.srcw = (rgbsrc);
  obj->colormode.dstw = (rgbdst);
  obj->colormode.equ =  (rgbequ);

  obj->alphamode.srcw = (asrc);
  obj->alphamode.dstw = (adst);
  obj->alphamode.equ =  (aequ);

}

LUX_API void lxgBlend_init(lxgBlendPTR obj)
{
  int i;
  for (i = 0; i < LUXGFX_MAX_RENDERTARGETS; i++){
    lxgBlendStage_init(&obj->blends[i]);
  }
  obj->individual = LUX_FALSE;
  obj->separateStages = LUX_FALSE;
}

LUX_API void  lxgContext_applyBlend( lxgContextPTR ctx, lxgBlendCPTR objmrt)
{
  int i;

  if (!objmrt->individual){
    if (objmrt->blends[0].enabled){
      glEnable(GL_BLEND);
      lxgContext_applyBlendStage(ctx, &objmrt->blends[0]);
    }
    else{
      glDisable(GL_BLEND);
    }
  }
  else{
    if (!objmrt->separateStages){
      lxgContext_applyBlendStage(ctx, &objmrt->blends[0]);
    }

    for (i = 0; i < ctx->capabilites.drawbuffers; i++){
      const lxgBlendStage_t* obj = &objmrt->blends[i];
      if (obj->enabled){
        glEnablei(GL_BLEND,i);
        if (objmrt->separateStages){
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

  ctx->raster.blend = *objmrt;
  ctx->raster.blendObj = objmrt;
}

LUX_API void  lxgBlend_sync( lxgBlendPTR obj, lxgContextPTR ctx )
{
  int i;
  lxgBlendStage_t* last = NULL;

  lxgBlend_init(obj);
  lxgBlendStage_sync(&obj->blends[0],ctx);

  if (ctx->capbits & LUXGFX_CAP_SM4){
    for (i = 0; i < ctx->capabilites.drawbuffers; i++){
      lxgBlendStage_t* blend = &obj->blends[i];
      blend->enabled = glIsEnabledi(GL_BLEND,i);

      if (ctx->capbits & LUXGFX_CAP_SM4){
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

      }

      if (last && memcmp(last,blend,sizeof(lxgBlend_t))){
        obj->individual = LUX_TRUE;
        if (last->enabled == blend->enabled){
          obj->separateStages = LUX_TRUE;
        }
      }

      last = blend;
    }
  }
}



//////////////////////////////////////////////////////////////////////////
// lxgStencil
LUX_API void  lxgStencil_init(lxgStencilPTR obj)
{
  obj->ops[0].fail = LUXGL_STENCIL_KEEP;
  obj->ops[0].zfail = LUXGL_STENCIL_KEEP;
  obj->ops[0].zpass = LUXGL_STENCIL_KEEP;
  obj->ops[0].func = LUXGL_COMPARE_ALWAYS;
  obj->ops[1].fail = LUXGL_STENCIL_KEEP;
  obj->ops[1].zfail = LUXGL_STENCIL_KEEP;
  obj->ops[1].zpass = LUXGL_STENCIL_KEEP;
  obj->ops[1].func = LUXGL_COMPARE_ALWAYS;
  obj->refvalue = 0;
  obj->write = (1<<8)-1;
  obj->mask  = (1<<8)-1;
  obj->enabled = LUX_FALSE;
}

LUX_API void lxgStencil_sync( lxgStencilPTR obj, lxgContextPTR ctx)
{
  GLenum val;
  glGetIntegerv(GL_STENCIL_FAIL,(GLint*)&val);
  obj->ops[0].fail = (val);
  glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL,(GLint*)&val);
  obj->ops[0].zfail = (val);
  glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS,(GLint*)&val);
  obj->ops[0].zpass = (val);
  glGetIntegerv(GL_STENCIL_FUNC,(GLint*)&val);
  obj->ops[0].func = (val);

  glGetIntegerv(GL_STENCIL_BACK_FAIL,(GLint*)&val);
  obj->ops[1].fail = (val);
  glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL,(GLint*)&val);
  obj->ops[1].zfail = (val);
  glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS,(GLint*)&val);
  obj->ops[1].zpass = (val);
  glGetIntegerv(GL_STENCIL_BACK_FUNC,(GLint*)&val);
  obj->ops[1].func = (val);

  glGetIntegerv(GL_STENCIL_WRITEMASK,&obj->write);
  glGetIntegerv(GL_STENCIL_VALUE_MASK,&obj->mask);

  obj->enabled = glIsEnabled(GL_STENCIL_TEST);
}

LUX_API void lxgContext_applyStencil( lxgContextPTR ctx, lxgStencilCPTR obj)
{
  if (!obj->enabled){
    glDisable(GL_STENCIL_TEST);
  }
  else{
    glEnable(GL_STENCIL_TEST);
    if (memcmp(&obj->ops[0],&obj->ops[1],sizeof(lxgStencilOp_t))){
      glStencilFuncSeparate(GL_FRONT, obj->ops[0].func,
        obj->refvalue,obj->mask);
      glStencilFuncSeparate(GL_BACK,  obj->ops[1].func,
        obj->refvalue,obj->mask);

      glStencilOpSeparate(GL_FRONT,obj->ops[0].fail,
        (obj->ops[0].zfail),
        (obj->ops[0].zpass));
      glStencilOpSeparate(GL_BACK,  obj->ops[1].fail,
        (obj->ops[1].zfail),
        (obj->ops[1].zpass));
    }
    else{
      glStencilFunc((obj->ops[0].func),obj->refvalue,obj->mask);
      glStencilOp((obj->ops[0].fail),
        (obj->ops[0].zfail),
        (obj->ops[0].zpass));
    }
    glStencilMask( obj->write );
  }

  ctx->raster.stencil = *obj;
  ctx->raster.stencilObj = obj;
}

//////////////////////////////////////////////////////////////////////////

LUX_API void  lxgProgramParameter_stateColor( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj){
  lxgContext_checkedColor(ctx,(lxgColorCPTR)obj);
}
LUX_API void  lxgProgramParameter_stateDepth( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj){
  lxgContext_checkedDepth(ctx,(lxgDepthCPTR)obj);
}
LUX_API void  lxgProgramParameter_stateLogic( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj){
  lxgContext_checkedLogic(ctx,(lxgLogicCPTR)obj);
}
LUX_API void  lxgProgramParameter_stateStencil( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj){
  lxgContext_checkedStencil(ctx,(lxgStencilCPTR)obj);
}
LUX_API void  lxgProgramParameter_stateBlend( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj){
  lxgContext_checkedBlend(ctx,(lxgBlendCPTR)obj);
}
LUX_API void  lxgProgramParameter_stateRasterizer( lxgProgramParameterPTR param, lxgContextPTR ctx, const void* obj){
  lxgContext_checkedRasterizer(ctx,(lxgRasterizerCPTR)obj);
}

