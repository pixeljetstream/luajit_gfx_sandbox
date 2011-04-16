// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt


#include <luxgfx/context.h>

//////////////////////////////////////////////////////////////////////////
// lxgContext
LUX_API const char* lxgContext_init(lxgContextPTR ctx)
{
  glewInit();
  memset(ctx,0,sizeof(lxgContext_t));

  if (!GLEW_VERSION_2_0){
    return "ARB_STENCIL_SEPARATE";
  }

  if (!(GLEW_ARB_multitexture || GLEW_VERSION_1_3)){
    return "MULTITEXTURE";
  }
  glGetIntegerv( GL_MAX_TEXTURE_UNITS, &ctx->capabilites.texunits );
  ctx->capabilites.teximages = ctx->capabilites.texunits;

  if (!(GLEW_ARB_texture_env_add || GLEW_EXT_texture_env_add) || 
    !(GLEW_ARB_texture_env_combine || GLEW_EXT_texture_env_combine))
  {
    return "TEXENV_COMBINE|TEXENV_ADD";
  }

  if (!GLEW_EXT_blend_func_separate || GLEW_EXT_blend_minmax || GLEW_EXT_blend_subtract){
    return "BLEND_FUNC_SEP|BLEND_MINMAX|BLEND_SUBTRACT";
  }

  if (!(GLEW_SGIS_generate_mipmap || GLEW_VERSION_1_4)){
    return "TEXTURE_GENERATE_MIPMAP";
  }

  if (!(GLEW_EXT_bgra || GLEW_EXT_abgr)){
    return "BGRA|ABGR";
  }

  if (!(GLEW_VERSION_3_0 || GLEW_EXT_draw_range_elements)){
    return "DRAW_RANGEELEMENTS";
  }

  if (!GLEW_EXT_texture_filter_anisotropic){
    return "TEXTURE_ANISOTROPIC";
  }
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&ctx->capabilites.texaniso);

  if (!GLEW_ARB_texture_compression){
    return "TEXTURE_COMPRESSION";
  }

  if (!GLEW_ARB_texture_cube_map){
    return "TEXTURE_CUBEMAP";
  }
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&ctx->capabilites.texsize);

  if ((GLEW_ARB_texture_env_dot3 || GLEW_EXT_texture_env_dot3) &&
    (GLEW_ARB_texture_env_crossbar || GLEW_NV_texture_env_combine4))
  {
    ctx->capbits |= LUXGFX_CAP_SM0;
  }

  if (GLEW_ARB_vertex_program && GLEW_ARB_shadow){
    ctx->capbits |= LUXGFX_CAP_SM1;
    if (GLEW_ARB_fragment_program){
      ctx->capbits |= LUXGFX_CAP_SM2;
      glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&ctx->capabilites.teximages);
      glGetIntegerv(GL_MAX_TEXTURE_COORDS,&ctx->capabilites.texcoords);
    }
    if (GLEW_ARB_draw_buffers && (GLEW_ARB_texture_float || GLEW_ATI_texture_float) &&
      GLEW_ARB_fragment_shader && GLEW_ARB_vertex_shader)
    {
      glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&ctx->capabilites.texvtxunits);
      glGetIntegerv(GL_MAX_DRAW_BUFFERS,&ctx->capabilites.drawbuffers);

      ctx->capbits |= LUXGFX_CAP_SM2EXT;
      if ((GLEW_NV_fragment_program2 || GLEW_ATI_shader_texture_lod) && 
        ctx->capabilites.texvtxunits>1)
      {
        ctx->capbits |= LUXGFX_CAP_SM3;

        if (GLEW_EXT_gpu_shader4 && GLEW_EXT_texture_integer &&
          GLEW_EXT_texture_buffer_object && GLEW_EXT_texture_array &&
          GLEW_ARB_texture_rg && GLEW_ARB_viewport_array)
        {
          glGetIntegerv(GL_MAX_VIEWPORTS,&ctx->capabilites.viewports);
          glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS,&ctx->capabilites.texlayers);
          ctx->capbits |= LUXGFX_CAP_SM4;
        }
      }
    }
  }

  if (GLEW_VERSION_3_3){
    ctx->capbits |= LUXGFX_CAP_SM4;
  }

  if (GLEW_VERSION_4_1){
    ctx->capbits |= LUXGFX_CAP_SM5;
  }

  if (GLEW_EXT_texture3D){
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE,&ctx->capabilites.texsize3d);
    ctx->capbits |= LUXGFX_CAP_TEX3D;
  }
  if (GLEW_ARB_texture_rectangle || GLEW_NV_texture_rectangle){
    ctx->capbits |= LUXGFX_CAP_TEXRECT;
  }
  if (GLEW_ARB_texture_non_power_of_two){
    ctx->capbits |= LUXGFX_CAP_TEXNP2;
  }
  if (GLEW_ARB_texture_cube_map_array){
    ctx->capbits |= LUXGFX_CAP_TEXCUBEARRAY;
  }
  if (GLEW_ARB_texture_compression_rgtc){
    ctx->capbits |= LUXGFX_CAP_TEXRGTC;
  }
  if (GLEW_EXT_texture_compression_s3tc){
    ctx->capbits |= LUXGFX_CAP_TEXS3TC;
  }

  if (GLEW_ARB_vertex_buffer_object){
    ctx->capbits |= LUXGFX_CAP_VBO;
  }
  if (GLEW_ARB_pixel_buffer_object){
    ctx->capbits |= LUXGFX_CAP_PBO;
  }
  if (GLEW_ARB_uniform_buffer_object){
    ctx->capbits |= LUXGFX_CAP_UBO;
  }
  if (GLEW_ARB_map_buffer_range){
    ctx->capbits |= LUXGFX_CAP_BUFMAPRANGE;
  }
  if (GLEW_ARB_copy_buffer){
    ctx->capbits |= LUXGFX_CAP_BUFCOPY;
  }
  if (GLEW_ARB_occlusion_query){
    ctx->capbits |= LUXGFX_CAP_OCCQUERY;
  }
  if (GLEW_EXT_blend_equation_separate){
    ctx->capbits |= LUXGFX_CAP_BLENDSEP;
  }

  if ((GLEW_EXT_framebuffer_object && GLEW_EXT_packed_depth_stencil) || GLEW_VERSION_3_0 || GLEW_ARB_framebuffer_object){
    ctx->capbits |= LUXGFX_CAP_FBO;
  }
  if ((GLEW_EXT_framebuffer_multisample && GLEW_EXT_framebuffer_blit) || GLEW_VERSION_3_0 || GLEW_ARB_framebuffer_object){
    ctx->capbits |= LUXGFX_CAP_FBOMS;
    glGetIntegerv(GL_MAX_SAMPLES,&ctx->capabilites.fbosamples);
  }
  if (GLEW_ARB_depth_buffer_float){
    ctx->capbits |= LUXGFX_CAP_DEPTHFLOAT;
  }

  if (GLEW_ARB_point_parameters && GLEW_ARB_point_sprite){
    float val1;
    float val2;

    ctx->capbits |= LUXGFX_CAP_POINTSPRITE;

    glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE,&val1);
    glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE,&val2);
    ctx->capabilites.pointsize = LUX_MAX(val1,val2);
  }

  lxgContext_syncStates(ctx);

  return NULL;
}



LUX_API void lxgContext_syncStates(lxgContextPTR ctx)
{
  ctx->rflag = lxgRenderFlag_sync(ctx);

  lxgDepth_sync(&ctx->depth, ctx );
  lxgAlpha_sync(&ctx->alpha, ctx );
  lxgBlend_sync(&ctx->blend, ctx );
  lxgStencil_sync(&ctx->stencil, ctx );
  lxgViewPort_sync(&ctx->viewport, ctx );
  lxgRasterizer_sync(&ctx->rasterizer, ctx );
}


LUX_API const char* lxgContext_test(lxgContextPTR ctx)
{
  {
    lxgDepthPTR state = &ctx->depth;
    lxgDepth_t test;
    lxgDepth_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgDepth_t))){
      return ("VIDDepth_t");
    }
  }

  {
    lxgAlphaPTR state = &ctx->alpha;
    lxgAlpha_t test;
    lxgAlpha_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgAlpha_t))){
      return ("VIDAlpha_t");
    }
  }

  {
    lxgBlendPTR state = &ctx->blend;
    lxgBlend_t test;
    lxgBlend_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgBlend_t))){
      return ("VIDBlend_t");
    }
  }

  {
    lxgStencilPTR state = &ctx->stencil;
    lxgStencil_t test;
    lxgStencil_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgStencil_t))){
      return ("VIDStencil_t");
    }
  }

  {
    lxgLinePTR state = &ctx->line;
    lxgLine_t test;
    lxgLine_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgLine_t))){
      return ("VIDLine_t");
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

  {
    lxgRasterizerPTR state = &ctx->rasterizer;
    lxgRasterizer_t test;
    lxgRasterizer_sync( &test, ctx );

    if (memcmp(state,&test,sizeof(lxgRasterizer_t))){
      return ("VIDRasterizer_t");
    }
  }

  return lxgRenderFlag_test(ctx);
}



