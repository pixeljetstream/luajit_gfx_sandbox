// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h


#include <luxinia/luxgfx/context.h>

//////////////////////////////////////////////////////////////////////////
// lxgContext
LUX_API const char* lxgContext_init(lxgContextPTR ctx)
{
  glewInit();
  memset(ctx,0,sizeof(lxgContext_t));

  if (!GLEW_VERSION_2_1){
    return "VERSION_2_1";
  }

  glGetIntegerv( GL_MAX_TEXTURE_UNITS, &ctx->capabilites.texunits );
  ctx->capabilites.teximages = ctx->capabilites.texunits;

  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&ctx->capabilites.texaniso);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&ctx->capabilites.texsize);

  if (GLEW_ARB_depth_clamp || GLEW_VERSION_3_3 || GLEW_NV_depth_clamp){
    ctx->capbits |= LUXGFX_CAP_DEPTHCLAMP;
  }

  {
    ctx->capbits |= LUXGFX_CAP_API2;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&ctx->capabilites.teximages);
    glGetIntegerv(GL_MAX_TEXTURE_COORDS,&ctx->capabilites.texcoords);
    ctx->capabilites.teximages = LUX_MIN(ctx->capabilites.teximages,LUXGFX_MAX_TEXTURE_IMAGES);

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&ctx->capabilites.texvtxunits);
    glGetIntegerv(GL_MAX_DRAW_BUFFERS,&ctx->capabilites.drawbuffers);
    ctx->capabilites.drawbuffers = LUX_MIN(ctx->capabilites.drawbuffers,LUXGFX_MAX_RENDERTARGETS);
  }

  if (GLEW_VERSION_3_2)
  {
    glGetIntegerv(GL_MAX_VIEWPORTS,&ctx->capabilites.viewports);
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS,&ctx->capabilites.texlayers);
    ctx->capbits |= LUXGFX_CAP_API3;
  }

  if (GLEW_VERSION_4_2){
    ctx->capbits |= LUXGFX_CAP_API4;
  }

  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE,&ctx->capabilites.texsize3d);


  if (GLEW_ARB_texture_compression_rgtc){
    ctx->capbits |= LUXGFX_CAP_TEXRGTC;
  }
  if (GLEW_EXT_texture_compression_s3tc){
    ctx->capbits |= LUXGFX_CAP_TEXS3TC;
  }

  {
    float minmax1[2];
    float minmax2[2];
    glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE,minmax1);
    glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE,minmax2);
    ctx->capabilites.pointsize = LUX_MAX(minmax1[0],minmax2[0]);
  }
 
  lxgContext_clearProgramState(ctx);
  lxgContext_clearRasterState(ctx);
  lxgContext_clearTextureState(ctx);
  lxgContext_clearVertexState(ctx);
  lxgContext_clearFeedbackState(ctx);

  lxgContext_syncRasterStates(ctx);

  return NULL;
}


LUX_API void lxgContext_syncRasterStates(lxgContextPTR ctx)
{
  lxgRasterizer_sync(&ctx->raster.rasterizer, ctx );
  lxgColor_sync(&ctx->raster.color, ctx );
  lxgDepth_sync(&ctx->raster.depth, ctx );
  lxgBlend_sync(&ctx->raster.blend, ctx );
  lxgStencil_sync(&ctx->raster.stencil, ctx );
  lxgViewPort_sync(&ctx->viewport, ctx );
}

LUX_API void lxgContext_clearRasterState( lxgContextPTR ctx )
{
  ctx->raster.blendObj = NULL;
  ctx->raster.depthObj = NULL;
  ctx->raster.logicObj = NULL;
  ctx->raster.stencilObj = NULL;
}


LUX_API const char* lxgContext_test(lxgContextPTR ctx)
{
  {
    lxgColorPTR state = &ctx->raster.color;
    lxgColor_t test;
    lxgColor_init( &test);
    lxgColor_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgColor_t))){
      return ("lxgColor_t");
    }
  }

  {
    lxgRasterizerPTR state = &ctx->raster.rasterizer;
    lxgRasterizer_t test;
    lxgRasterizer_init( &test);
    lxgRasterizer_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgRasterizer_t))){
      return ("lxgRasterizer_t");
    }
  }

  {
    lxgDepthPTR state = &ctx->raster.depth;
    lxgDepth_t test;
    lxgDepth_init( &test);
    lxgDepth_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgDepth_t))){
      return ("lxgDepth_t");
    }
  }

  {
    lxgBlendPTR state = &ctx->raster.blend;
    lxgBlend_t test;
    lxgBlend_init( &test);
    lxgBlend_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgBlend_t))){
      return ("lxgBlend_t");
    }
  }

  {
    lxgStencilPTR state = &ctx->raster.stencil;
    lxgStencil_t test;
    lxgStencil_init( &test);
    lxgStencil_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgStencil_t))){
      return ("lxgStencil_t");
    }
  }

  {
    lxgViewPortPTR state = &ctx->viewport;
    lxgViewPort_t test;
    lxgViewPort_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgViewPort_t))){
      return ("VIDViewPort_t");
    }
  }

  return NULL;
}







