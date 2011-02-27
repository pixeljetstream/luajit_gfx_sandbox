// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/luxgfx.h>
#include <luxgfx/program.h>
#include <luxmath/misc.h>

#include "state_inl.h"


static void lxgupdateFloat1(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform1fv(param->vgl.location, param->count, data);
}
static void lxgupdateFloat2(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform2fv(param->vgl.location, param->count, data);
}
static void lxgupdateFloat3(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform3fv(param->vgl.location, param->count, data);
}
static void lxgupdateFloat4(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform4fv(param->vgl.location, param->count, data);
}

static void lxgupdateInt1(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform1iv(param->vgl.location, param->count, data);
}
static void lxgupdateInt2(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform2iv(param->vgl.location, param->count, data);
}
static void lxgupdateInt3(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform3iv(param->vgl.location, param->count, data);
}
static void lxgupdateInt4(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform4iv(param->vgl.location, param->count, data);
}

static void lxgupdateUint1(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform1uiv(param->vgl.location, param->count, data);
}
static void lxgupdateUint2(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform2uiv(param->vgl.location, param->count, data);
}
static void lxgupdateUint3(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform3uiv(param->vgl.location, param->count, data);
}
static void lxgupdateUint4(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniform4uiv(param->vgl.location, param->count, data);
}

static void lxgupdateMat2(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix2fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat3(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix3fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat4(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix4fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat2x3(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix2x3fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat2x4(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix2x4fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat3x2(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix3x2fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat3x4(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix3x4fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat4x2(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix4x2fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}
static void lxgupdateMat4x3(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  glUniformMatrix4x3fv(param->vgl.location, param->count, (GLboolean)param->transpose, data);
}

static void lxgupdateSampler(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  lxgTexture_checked(ctx,param->vgl.location,data);
}
static void lxgupdateImage(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  lxgTextureImage_checked(ctx,param->vgl.location,data);
}
static void lxgupdateBuffer(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data){
  if (lxgBuffer_checkedSetUniform(ctx,param->vgl.location,data)){
    lxgBuffer_applyIndexed(ctx,LUXGL_BUFFER_UNIFORM,param->vgl.location,data);
  }
}

LUX_API void lxgProgramParameter_initFunc( lxgProgramParameterPTR param )
{
  param->func = NULL;
  switch(param->vgl.type)
  {
  case LUXGL_PARAM_FLOAT:
    param->func = lxgupdateFloat1; return;
  case LUXGL_PARAM_FLOAT2:
    param->func = lxgupdateFloat2; return;
  case LUXGL_PARAM_FLOAT3:
    param->func = lxgupdateFloat3; return;
  case LUXGL_PARAM_FLOAT4:
    param->func = lxgupdateFloat4; return;
  case LUXGL_PARAM_INT:
    param->func = lxgupdateInt1; return;
  case LUXGL_PARAM_INT2:
    param->func = lxgupdateInt2; return;
  case LUXGL_PARAM_INT3:
    param->func = lxgupdateInt3; return;
  case LUXGL_PARAM_INT4:
    param->func = lxgupdateInt4; return;
  case LUXGL_PARAM_UINT:
    param->func = lxgupdateUint1; return;
  case LUXGL_PARAM_UINT2:
    param->func = lxgupdateUint2; return;
  case LUXGL_PARAM_UINT3:
    param->func = lxgupdateUint3; return;
  case LUXGL_PARAM_UINT4:
    param->func = lxgupdateUint4; return;
  case LUXGL_PARAM_BOOL:
    param->func = lxgupdateFloat1; return;
  case LUXGL_PARAM_BOOL2:
    param->func = lxgupdateFloat2; return;
  case LUXGL_PARAM_BOOL3:
    param->func = lxgupdateFloat3; return;
  case LUXGL_PARAM_BOOL4:
    param->func = lxgupdateFloat4; return;
  case LUXGL_PARAM_MAT2:
    param->func = lxgupdateMat2; return;
  case LUXGL_PARAM_MAT3:
    param->func = lxgupdateMat3; return;
  case LUXGL_PARAM_MAT4:
    param->func = lxgupdateMat4; return;
  case LUXGL_PARAM_MAT2x3:
    param->func = lxgupdateMat2x3; return;
  case LUXGL_PARAM_MAT2x4:
    param->func = lxgupdateMat2x4; return;
  case LUXGL_PARAM_MAT3x2:
    param->func = lxgupdateMat3x2; return;
  case LUXGL_PARAM_MAT3x4:
    param->func = lxgupdateMat3x4; return;
  case LUXGL_PARAM_MAT4x2:
    param->func = lxgupdateMat4x2; return;
  case LUXGL_PARAM_MAT4x3:
    param->func = lxgupdateMat4x3; return;
  case LUXGL_PARAM_BUFFER:
    param->func = lxgupdateBuffer; return;
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
    param->func = lxgupdateSampler; return;

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

    param->func = lxgupdateImage; return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}

LUX_API void lxgProgram_updateParameters( lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, void **data )
{
  uint i;
  glUseProgram(prog->vgl.id);
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func);
    params[i]->func(ctx,params[i],data[i]);
  }
}

LUX_API void lxgProgram_updateSamplers( lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgTexturePTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxgupdateSampler);
    lxgTexture_checked(ctx,params[i]->vgl.location,data[i]);
  }
}

LUX_API void lxgProgram_updateImages( lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgTextureImagePTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxgupdateImage);
    lxgTextureImage_checked(ctx,params[i]->vgl.location,data[i]);
  }
}

LUX_API void lxgProgram_updateBuffers( lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxgupdateBuffer);
    if (lxgBuffer_checkedSetUniform(ctx,params[i]->vgl.location,data[i])){
      lxgBuffer_applyIndexed(ctx,LUXGL_BUFFER_UNIFORM,params[i]->vgl.location,data[i]);
    }
  }
}


//////////////////////////////////////////////////////////////////////////

LUX_API void lxgProgram_init( lxgContextPTR ctx, lxgProgramPTR prog )
{
  prog->ctx = ctx;
  prog->vgl.id = glCreateProgram();
}

LUX_API void lxgProgram_deinit( lxgContextPTR ctx, lxgProgramPTR prog )
{
  glDeleteProgram(prog->vgl.id);
}

LUX_API void lxgProgram_setStage( lxgContextPTR ctx, lxgProgramPTR prog, lxgShaderType_t type, lxgDomainProgramPTR stage )
{
  prog->programs[type] = stage;
  prog->usedProgs = 1<<type;
  glAttachShader(prog->vgl.id,stage->vgl.id);
}

LUX_API booln lxgProgram_link( lxgContextPTR ctx, lxgProgramPTR prog )
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

LUX_API const char* lxgProgram_log( lxgContextPTR ctx, lxgProgramPTR prog, char* buffer, int len)
{
  GLsizei used;
  glGetProgramInfoLog(prog->vgl.id, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

//////////////////////////////////////////////////////////////////////////

LUX_API void lxgDomainProgram_init( lxgContextPTR ctx, lxgDomainProgramPTR stage, lxgShaderType_t type)
{
  stage->ctx = ctx;
  stage->vgl.id = glCreateShader(type);
  stage->vgl.type = type;
}

LUX_API void lxgDomainProgram_deinit( lxgContextPTR ctx, lxgDomainProgramPTR stage )
{
  glDeleteShader(stage->vgl.id);
}

LUX_API booln lxgDomainProgram_compile( lxgContextPTR ctx, lxgDomainProgramPTR stage, const char *src, int len )
{
  GLuint id = stage->vgl.id;
  GLint status;
  glShaderSource(id, 1, &src, &len);
  glCompileShader(id);
  glGetShaderiv(id,GL_COMPILE_STATUS,&status);
  return status;
}

LUX_API const char* lxgDomainProgram_error( lxgContextPTR ctx, lxgDomainProgramPTR stage, char *buffer, int len  )
{
  GLsizei used;
  glGetShaderInfoLog(stage->vgl.id, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

