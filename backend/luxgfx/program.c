// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/luxgfx.h>
#include <luxgfx/program.h>
#include <luxmath/misc.h>

#include "state_inl.h"


static void lxGFXupdateFloat1(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform1fv(param->vgl.location, param->count, data);
}
static void lxGFXupdateFloat2(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform2fv(param->vgl.location, param->count, data);
}
static void lxGFXupdateFloat3(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform3fv(param->vgl.location, param->count, data);
}
static void lxGFXupdateFloat4(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform4fv(param->vgl.location, param->count, data);
}

static void lxGFXupdateInt1(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform1iv(param->vgl.location, param->count, data);
}
static void lxGFXupdateInt2(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform2iv(param->vgl.location, param->count, data);
}
static void lxGFXupdateInt3(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform3iv(param->vgl.location, param->count, data);
}
static void lxGFXupdateInt4(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform4iv(param->vgl.location, param->count, data);
}

static void lxGFXupdateUint1(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform1uiv(param->vgl.location, param->count, data);
}
static void lxGFXupdateUint2(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform2uiv(param->vgl.location, param->count, data);
}
static void lxGFXupdateUint3(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform3uiv(param->vgl.location, param->count, data);
}
static void lxGFXupdateUint4(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniform4uiv(param->vgl.location, param->count, data);
}

static void lxGFXupdateMat2(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix2fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat3(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix3fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat4(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix4fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat2x3(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix2x3fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat2x4(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix2x4fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat3x2(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix3x2fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat3x4(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix3x4fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat4x2(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix4x2fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxGFXupdateMat4x3(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  glUniformMatrix4x3fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}

static void lxGFXupdateSampler(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  lxGFXTexture_checked(ctx,param->vgl.location,data);
}
static void lxGFXupdateImage(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  lxGFXTextureImage_checked(ctx,param->vgl.location,data);
}
static void lxGFXupdateBuffer(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data){
  if (lxGFXBuffer_checkedSetUniform(ctx,param->vgl.location,data)){
    lxGFXBuffer_applyIndexed(ctx,LUXGL_BUFFER_UNIFORM,param->vgl.location,data);
  }
}

LUX_API void lxGFXProgramParameter_initFunc( lxGFXProgramParameterPTR param )
{
  param->func = NULL;
  switch(param->vgl.type)
  {
  case LUXGL_PARAM_FLOAT:
    param->func = lxGFXupdateFloat1; return;
  case LUXGL_PARAM_FLOAT2:
    param->func = lxGFXupdateFloat2; return;
  case LUXGL_PARAM_FLOAT3:
    param->func = lxGFXupdateFloat3; return;
  case LUXGL_PARAM_FLOAT4:
    param->func = lxGFXupdateFloat4; return;
  case LUXGL_PARAM_INT:
    param->func = lxGFXupdateInt1; return;
  case LUXGL_PARAM_INT2:
    param->func = lxGFXupdateInt2; return;
  case LUXGL_PARAM_INT3:
    param->func = lxGFXupdateInt3; return;
  case LUXGL_PARAM_INT4:
    param->func = lxGFXupdateInt4; return;
  case LUXGL_PARAM_UINT:
    param->func = lxGFXupdateUint1; return;
  case LUXGL_PARAM_UINT2:
    param->func = lxGFXupdateUint2; return;
  case LUXGL_PARAM_UINT3:
    param->func = lxGFXupdateUint3; return;
  case LUXGL_PARAM_UINT4:
    param->func = lxGFXupdateUint4; return;
  case LUXGL_PARAM_BOOL:
    param->func = lxGFXupdateFloat1; return;
  case LUXGL_PARAM_BOOL2:
    param->func = lxGFXupdateFloat2; return;
  case LUXGL_PARAM_BOOL3:
    param->func = lxGFXupdateFloat3; return;
  case LUXGL_PARAM_BOOL4:
    param->func = lxGFXupdateFloat4; return;
  case LUXGL_PARAM_MAT2:
    param->func = lxGFXupdateMat2; return;
  case LUXGL_PARAM_MAT3:
    param->func = lxGFXupdateMat3; return;
  case LUXGL_PARAM_MAT4:
    param->func = lxGFXupdateMat4; return;
  case LUXGL_PARAM_MAT2x3:
    param->func = lxGFXupdateMat2x3; return;
  case LUXGL_PARAM_MAT2x4:
    param->func = lxGFXupdateMat2x4; return;
  case LUXGL_PARAM_MAT3x2:
    param->func = lxGFXupdateMat3x2; return;
  case LUXGL_PARAM_MAT3x4:
    param->func = lxGFXupdateMat3x4; return;
  case LUXGL_PARAM_MAT4x2:
    param->func = lxGFXupdateMat4x2; return;
  case LUXGL_PARAM_MAT4x3:
    param->func = lxGFXupdateMat4x3; return;
  case LUXGL_PARAM_BUFFER:
    param->func = lxGFXupdateBuffer; return;
  case LUXGL_PARAM_SAMPLER_1D:
  case LUXGL_PARAM_SAMPLER_2D:
  case LUXGL_PARAM_SAMPLER_3D:
  case LUXGL_PARAM_SAMPLER_CUBE:
  case LUXGL_PARAM_SAMPLER_2DRECT:
  case LUXGL_PARAM_SAMPLER_2DMS:
  case LUXGL_PARAM_SAMPLER_1DARRAY:
  case LUXGL_PARAM_SAMPLER_2DARRAY:
  case LUXGL_PARAM_SAMPLER_CUBEARRAY:
  case LUXGL_PARAM_SAMPLER_2DMSARRAY:
  case LUXGL_PARAM_SAMPLER_BUFFER:

  case LUXGL_PARAM_ISAMPLER_1D:
  case LUXGL_PARAM_ISAMPLER_2D:
  case LUXGL_PARAM_ISAMPLER_3D:
  case LUXGL_PARAM_ISAMPLER_CUBE:
  case LUXGL_PARAM_ISAMPLER_2DRECT:
  case LUXGL_PARAM_ISAMPLER_2DMS:
  case LUXGL_PARAM_ISAMPLER_1DARRAY:
  case LUXGL_PARAM_ISAMPLER_2DARRAY:
  case LUXGL_PARAM_ISAMPLER_CUBEARRAY:
  case LUXGL_PARAM_ISAMPLER_2DMSARRAY:
  case LUXGL_PARAM_ISAMPLER_BUFFER:

  case LUXGL_PARAM_USAMPLER_1D:
  case LUXGL_PARAM_USAMPLER_2D:
  case LUXGL_PARAM_USAMPLER_3D:
  case LUXGL_PARAM_USAMPLER_CUBE:
  case LUXGL_PARAM_USAMPLER_2DRECT:
  case LUXGL_PARAM_USAMPLER_2DMS:
  case LUXGL_PARAM_USAMPLER_1DARRAY:
  case LUXGL_PARAM_USAMPLER_2DARRAY:
  case LUXGL_PARAM_USAMPLER_CUBEARRAY:
  case LUXGL_PARAM_USAMPLER_2DMSARRAY:
  case LUXGL_PARAM_USAMPLER_BUFFER:

  case LUXGL_PARAM_SAMPLER_1D_SHADOW:
  case LUXGL_PARAM_SAMPLER_2D_SHADOW:
  case LUXGL_PARAM_SAMPLER_CUBE_SHADOW:
  case LUXGL_PARAM_SAMPLER_2DRECT_SHADOW:
  case LUXGL_PARAM_SAMPLER_1DARRAY_SHADOW:
  case LUXGL_PARAM_SAMPLER_2DARRAY_SHADOW:
  case LUXGL_PARAM_SAMPLER_CUBEARRAY_SHADOW:
    param->func = lxGFXupdateSampler; return;

  case LUXGL_PARAM_IMAGE_1D:
  case LUXGL_PARAM_IMAGE_2D:
  case LUXGL_PARAM_IMAGE_3D:
  case LUXGL_PARAM_IMAGE_CUBE:
  case LUXGL_PARAM_IMAGE_2DRECT:
  case LUXGL_PARAM_IMAGE_2DMS:
  case LUXGL_PARAM_IMAGE_1DARRAY:
  case LUXGL_PARAM_IMAGE_2DARRAY:
  case LUXGL_PARAM_IMAGE_CUBEARRAY:
  case LUXGL_PARAM_IMAGE_2DMSARRAY:
  case LUXGL_PARAM_IMAGE_BUFFER:

  case LUXGL_PARAM_IIMAGE_1D:
  case LUXGL_PARAM_IIMAGE_2D:
  case LUXGL_PARAM_IIMAGE_3D:
  case LUXGL_PARAM_IIMAGE_CUBE:
  case LUXGL_PARAM_IIMAGE_2DRECT:
  case LUXGL_PARAM_IIMAGE_2DMS:
  case LUXGL_PARAM_IIMAGE_1DARRAY:
  case LUXGL_PARAM_IIMAGE_2DARRAY:
  case LUXGL_PARAM_IIMAGE_CUBEARRAY:
  case LUXGL_PARAM_IIMAGE_2DMSARRAY:
  case LUXGL_PARAM_IIMAGE_BUFFER:

  case LUXGL_PARAM_UIMAGE_1D:
  case LUXGL_PARAM_UIMAGE_2D:
  case LUXGL_PARAM_UIMAGE_3D:
  case LUXGL_PARAM_UIMAGE_CUBE:
  case LUXGL_PARAM_UIMAGE_2DRECT:
  case LUXGL_PARAM_UIMAGE_2DMS:
  case LUXGL_PARAM_UIMAGE_1DARRAY:
  case LUXGL_PARAM_UIMAGE_2DARRAY:
  case LUXGL_PARAM_UIMAGE_CUBEARRAY:
  case LUXGL_PARAM_UIMAGE_2DMSARRAY:
  case LUXGL_PARAM_UIMAGE_BUFFER:

    param->func = lxGFXupdateImage; return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}

LUX_API void lxGFXProgram_updateParameters( lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, void **data )
{
  uint i;
  glUseProgram(prog->vgl.id);
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func);
    params[i]->func(ctx,params[i],data[i]);
  }
}

LUX_API void lxGFXProgram_updateSamplers( lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, lxGFXTexturePTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxGFXupdateSampler);
    lxGFXTexture_checked(ctx,params[i]->vgl.location,data[i]);
  }
}

LUX_API void lxGFXProgram_updateImages( lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, lxGFXTextureImagePTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxGFXupdateImage);
    lxGFXTextureImage_checked(ctx,params[i]->vgl.location,data[i]);
  }
}

LUX_API void lxGFXProgram_updateBuffers( lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, lxGFXBufferPTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxGFXupdateBuffer);
    if (lxGFXBuffer_checkedSetUniform(ctx,params[i]->vgl.location,data[i])){
      lxGFXBuffer_applyIndexed(ctx,LUXGL_BUFFER_UNIFORM,params[i]->vgl.location,data[i]);
    }
  }
}


//////////////////////////////////////////////////////////////////////////

LUX_API void lxGFXProgram_init( lxGFXContextPTR ctx, lxGFXProgramPTR prog )
{
  prog->ctx = ctx;
  prog->vgl.id = glCreateProgram();
}

LUX_API void lxGFXProgram_deinit( lxGFXContextPTR ctx, lxGFXProgramPTR prog )
{
  glDeleteProgram(prog->vgl.id);
}

LUX_API void lxGFXProgram_setStage( lxGFXContextPTR ctx, lxGFXProgramPTR prog, lxGFXShaderType_t type, lxGFXDomainProgramPTR stage )
{
  prog->programs[type] = stage;
  prog->usedProgs = 1<<type;
  glAttachShader(prog->vgl.id,stage->vgl.id);
}

LUX_API booln lxGFXProgram_link( lxGFXContextPTR ctx, lxGFXProgramPTR prog )
{
  GLint status;
  GLuint id = prog->vgl.id;
  
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (!status) return LUX_FALSE;

  glValidateProgram(id);
  glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
  if (!status) return LUX_FALSE;

  return LUX_TRUE;
}

LUX_API const char* lxGFXProgram_log( lxGFXContextPTR ctx, lxGFXProgramPTR prog, char* buffer, int len)
{
  GLsizei used;
  glGetProgramInfoLog(prog->vgl.id, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

//////////////////////////////////////////////////////////////////////////

LUX_API void lxGFXDomainProgram_init( lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage, lxGFXShaderType_t type)
{
  stage->ctx = ctx;
  stage->vgl.id = glCreateShader(type);
  stage->vgl.type = type;
}

LUX_API void lxGFXDomainProgram_deinit( lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage )
{
  glDeleteShader(stage->vgl.id);
}

LUX_API booln lxGFXDomainProgram_compile( lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage, const char *src, int len )
{
  GLuint id = stage->vgl.id;
  GLint status;
  glShaderSource(id, 1, &src, &len);
  glCompileShader(id);
  glGetShaderiv(id,GL_COMPILE_STATUS,&status);
  return status;
}

LUX_API const char* lxGFXDomainProgram_error( lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage, char *buffer, int len  )
{
  GLsizei used;
  glGetShaderInfoLog(stage->vgl.id, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

