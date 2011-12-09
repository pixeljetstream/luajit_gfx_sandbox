// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxgfx/luxgfx.h>
#include <luxinia/luxgfx/program.h>
#include <luxinia/luxmath/misc.h>
#include <luxinia/luxplatform/debug.h>

#include "state_inl.h"


//////////////////////////////////////////////////////////////////////////

static LUX_INLINE lxGLShaderType_t lxgProgramStage_typeGL(lxgProgramStage_t type)
{
  lxGLShaderType_t types[] = {
    LUXGL_SHADER_VERTEX,
    LUXGL_SHADER_FRAGMENT,
    LUXGL_SHADER_GEOMETRY,
    LUXGL_SHADER_TESSCTRL,
    LUXGL_SHADER_TESSEVAL,
  };

  return types[type];
}

static LUX_INLINE uint32 lxgProgramStage_bit(lxgProgramStage_t type)
{
  return 1<<type;
}

static LUX_INLINE lxGLProgramType_t lxgProgramStage_typeNV(lxgProgramStage_t type)
{
  lxGLProgramType_t types[] = {
    LUXGL_PROGRAM_VERTEX,
    LUXGL_PROGRAM_FRAGMENT,
    LUXGL_PROGRAM_GEOMETRY,
    LUXGL_PROGRAM_TESSCTRL,
    LUXGL_PROGRAM_TESSEVAL,
  };

  return types[type];
}

LUX_API booln lxGLParameterType_isValue(lxGLParameterType_t type)
{
  switch(type){
  case LUXGL_PARAM_FLOAT  :
  case LUXGL_PARAM_FLOAT2 :
  case LUXGL_PARAM_FLOAT3 :
  case LUXGL_PARAM_FLOAT4 :
  case LUXGL_PARAM_INT    :
  case LUXGL_PARAM_INT2   :
  case LUXGL_PARAM_INT3   :
  case LUXGL_PARAM_INT4   :
  case LUXGL_PARAM_UINT   :
  case LUXGL_PARAM_UINT2  :
  case LUXGL_PARAM_UINT3  :
  case LUXGL_PARAM_UINT4  :
  case LUXGL_PARAM_BOOL   :
  case LUXGL_PARAM_BOOL2  :
  case LUXGL_PARAM_BOOL3  :
  case LUXGL_PARAM_BOOL4  :
  case LUXGL_PARAM_MAT2   :
  case LUXGL_PARAM_MAT3   :
  case LUXGL_PARAM_MAT4   :
  case LUXGL_PARAM_MAT2x3 :
  case LUXGL_PARAM_MAT2x4 :
  case LUXGL_PARAM_MAT3x2 :
  case LUXGL_PARAM_MAT3x4 :
  case LUXGL_PARAM_MAT4x2 :
  case LUXGL_PARAM_MAT4x3 :
    return LUX_TRUE;
  default:
    return LUX_FALSE;
  }
}

LUX_API booln lxGLParameterType_isTexture(lxGLParameterType_t type)
{
  switch(type){
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
    return LUX_TRUE;
  default:
    return LUX_FALSE;
  }
}

LUX_API booln lxGLParameterType_isImage(lxGLParameterType_t type)
{
  switch(type){
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
    return LUX_TRUE;
  default:
    return LUX_FALSE;
  }

}


//////////////////////////////////////////////////////////////////////////


static void lxgUpdateFloat1GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform1fv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateFloat2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform2fv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateFloat3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform3fv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateFloat4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform4fv(param->gllocation, param->uniform.count, data);
}

static void lxgUpdateInt1GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform1iv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateInt2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform2iv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateInt3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform3iv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateInt4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform4iv(param->gllocation, param->uniform.count, data);
}

static void lxgUpdateUInt1GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform1uiv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateUInt2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform2uiv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateUInt3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform3uiv(param->gllocation, param->uniform.count, data);
}
static void lxgUpdateUInt4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniform4uiv(param->gllocation, param->uniform.count, data);
}

static void lxgUpdateMat2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix2fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix3fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix4fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat2x3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix2x3fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat2x4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix2x4fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat3x2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix3x2fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat3x4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix3x4fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat4x2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix4x2fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat4x3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformMatrix4x3fv(param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateUint64GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glUniformui64vNV(param->gllocation, param->uniform.count,data);
}
static void lxgUpdateFloat1SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform1fvEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateFloat2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform2fvEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateFloat3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform3fvEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateFloat4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform4fvEXT(param->glid, param->gllocation, param->uniform.count, data);
}

static void lxgUpdateInt1SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform1ivEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateInt2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform2ivEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateInt3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform3ivEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateInt4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform4ivEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateUInt1SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform1uivEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateUInt2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform2uivEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateUInt3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform3uivEXT(param->glid, param->gllocation, param->uniform.count, data);
}
static void lxgUpdateUInt4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniform4uivEXT(param->glid, param->gllocation, param->uniform.count, data);
}

static void lxgUpdateMat2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix2fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix3fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix4fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat2x3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix2x3fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat2x4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix2x4fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat3x2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix3x2fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat3x4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix3x4fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat4x2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix4x2fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateMat4x3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformMatrix4x3fvEXT(param->glid, param->gllocation, param->uniform.count, (GLboolean)param->uniform.transpose, data);
}
static void lxgUpdateUint64SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramUniformui64vNV(param->glid, param->gllocation, param->uniform.count,data);
}

static void lxgUpdateFloat1NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec++){
    glProgramLocalParameter4fARB(param->gltarget,param->gllocation + i,vec[0],0.0f,0.0f,0.0f);
  }
}
static void lxgUpdateFloat2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec+=2){
    glProgramLocalParameter4fARB(param->gltarget,param->gllocation + i,vec[0],vec[1],0.0f,0.0f);
  }
}
static void lxgUpdateFloat3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec+=3){
    glProgramLocalParameter4fARB(param->gltarget,param->gllocation + i,vec[0],vec[1],vec[2],0.0f);
  }
}
static void lxgUpdateFloat4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,param->uniform.count,data);
}

static void lxgUpdateInt1NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  int* vec = (int*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec++){
    glProgramLocalParameterI4iNV(param->gltarget,param->gllocation + i,vec[0],0,0,0);
  }
}
static void lxgUpdateInt2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  int* vec = (int*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec+=2){
    glProgramLocalParameterI4iNV(param->gltarget,param->gllocation + i,vec[0],vec[1],0,0);
  }
}
static void lxgUpdateInt3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  int* vec = (int*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec+=3){
    glProgramLocalParameterI4iNV(param->gltarget,param->gllocation + i,vec[0],vec[1],vec[2],0);
  }
}
static void lxgUpdateInt4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramLocalParametersI4ivNV(param->gltarget,param->gllocation,param->uniform.count,data);
}

static void lxgUpdateUInt1NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  uint* vec = (uint*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec++){
    glProgramLocalParameterI4uiNV(param->gltarget,param->gllocation + i,vec[0],0,0,0);
  }
}
static void lxgUpdateUInt2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  uint* vec = (uint*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec+=2){
    glProgramLocalParameterI4uiNV(param->gltarget,param->gllocation + i,vec[0],vec[1],0,0);
  }
}
static void lxgUpdateUInt3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  uint* vec = (uint*)data;
  uint i;
  for (i = 0; i < param->uniform.count; ++i, vec+=3){
    glProgramLocalParameterI4uiNV(param->gltarget,param->gllocation + i,vec[0],vec[1],vec[2],0);
  }
}
static void lxgUpdateUInt4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  glProgramLocalParametersI4uivNV(param->gltarget,param->gllocation,param->uniform.count,data);
}


static void lxgUpdateMat2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->uniform.transpose){
    for (i = 0; i < param->uniform.count; ++i, vec+=4, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[2],vec[3],0.0f,0.0f);
    }
  }
  else{
    for (i = 0; i < param->uniform.count; ++i, vec+=4, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[2],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[3],0.0f,0.0f);
    }
  }
}
static void lxgUpdateMat3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->uniform.transpose){
    for (i = 0; i < param->uniform.count; ++i, vec+=9, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],vec[2],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[3],vec[4],vec[5],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[6],vec[7],vec[8],0.0f);
    }
  }
  else{
    for (i = 0; i < param->uniform.count; ++i, vec+=9, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[3],vec[6],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[4],vec[7],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[5],vec[8],0.0f);
    }
  }
}
static void lxgUpdateMat4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  if (!param->uniform.transpose){
    glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,((int)param->uniform.count) * 4,data);
  }
  else{
    uint i;
    GLuint loc = param->gllocation;
    float* vec = (float*) data;
    for (i = 0; i < param->uniform.count; ++i, vec+=16, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[4],vec[8],vec[12]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[5],vec[9],vec[13]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[6],vec[10],vec[14]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[3],vec[7],vec[11],vec[15]);
    }
  }
}
static void lxgUpdateMat2x3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->uniform.transpose){
    for (i = 0; i < param->uniform.count; ++i, vec+=6, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],vec[2],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[3],vec[4],vec[5],0.0f);
    }
  }
  else {
    for (i = 0; i < param->uniform.count; ++i, vec+=6, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[2],vec[4],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[3],vec[5],0.0f);
    }
  }
}
static void lxgUpdateMat2x4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  if (!param->uniform.transpose){
    glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,((int)param->uniform.count) * 2,data);
  }
  else{
    uint i;
    GLuint loc = param->gllocation;
    float* vec = (float*) data;
    for (i = 0; i < param->uniform.count; ++i, vec+=8, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[2],vec[4],vec[6]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[3],vec[5],vec[7]);
    }
  }
}
static void lxgUpdateMat3x2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->uniform.transpose){
    for (i = 0; i < param->uniform.count; ++i, vec+=6, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[2],vec[3],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[4],vec[5],0.0f,0.0f);
    }
  }
  else{
    for (i = 0; i < param->uniform.count; ++i, vec+=6, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[3],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[4],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[5],0.0f,0.0f);
    }
  }
}
static void lxgUpdateMat3x4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  if (!param->uniform.transpose){
    glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,((int)param->uniform.count) * 3,data);
  }
  else{
    uint i;
    GLuint loc = param->gllocation;
    float* vec = (float*) data;
    for (i = 0; i < param->uniform.count; ++i, vec+=12, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[3],vec[6],vec[9]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[4],vec[7],vec[10]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[5],vec[8],vec[11]);
    }
  }
}
static void lxgUpdateMat4x2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->uniform.transpose){
    for (i = 0; i < param->uniform.count; ++i, vec+=8, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[2],vec[3],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[4],vec[5],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[6],vec[7],0.0f,0.0f);
    }
  }
  else{
    for (i = 0; i < param->uniform.count; ++i, vec+=8, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[4],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[5],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[6],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[3],vec[7],0.0f,0.0f);
    }
  }
}
static void lxgUpdateMat4x3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->uniform.transpose){
    for (i = 0; i < param->uniform.count; ++i, vec+=12, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],vec[2],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[3],vec[4],vec[5],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[6],vec[7],vec[8],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[9],vec[10],vec[11],0.0f);
    }
  }
  else{
    for (i = 0; i < param->uniform.count; ++i, vec+=12, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[4],vec[8],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[5],vec[9],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[6],vec[10],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[3],vec[7],vec[11],0.0f);
    }
  }
}

static void lxgUpdateUint64NV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  uint i;
  const uint64* vec = data;
  for (i = 0; i < param->uniform.count; ++i, vec++){
    glProgramLocalParameterI4uiNV(param->gltarget,param->gllocation + i,(uint)vec[0],(uint)(vec[0] >> 32),0,0);
  }
}


static LUX_INLINE void lxgUpdateBufferGLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  lxgBufferCPTR buffer = *(lxgBufferCPTR*)data;
  if (lxgContext_setProgramBuffer(ctx,param->buffer.unit,buffer)){
    lxgBuffer_bindIndexed((lxgBufferPTR)data,LUXGL_BUFFER_UNIFORM,param->buffer.unit);
  }
}

static LUX_INLINE void lxgUpdateBufferNV(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  lxgBufferCPTR buffer = *(lxgBufferCPTR*)data;
  GLuint domain = param->gltarget - LUXGL_BUFFER_NVPARAM_TESSCTRL;
  if (lxgContext_setProgramBuffer(ctx,param->buffer.unit + domain * LUXGFX_MAX_STAGE_BUFFERS,buffer)){
    lxgBuffer_bindIndexed(buffer,param->gltarget,param->buffer.unit);
  }
}

static void lxgUpdateSubroutine(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  GLuint* LUX_RESTRICT subroutines = ctx->program.subroutines[param->stage];
  lxgSubroutineKey* LUX_RESTRICT keys = (lxgSubroutineKey*)data;

  if (param->subroutine.count == 1 && param->subroutine.last.namekey == keys[0]){
    subroutines[param->gllocation] = param->subroutine.last.glindex;
  }
  else{
    uint n;
    GLuint index = -1;
    for (n = 0; n < param->subroutine.count; n++){
      uint i;
      index = -1;
      for (i = 0; i < param->subroutine.numCompatible; i++){
        if (param->subroutine.compatible[i].namekey == keys[n]){
          index = param->subroutine.compatible[i].glindex;
          break;
        }
      }
      LUX_DEBUGASSERT(index != -1 && "invalid subroutine namekey");

      subroutines[param->gllocation + n] = index;
    }

    param->subroutine.last.namekey = keys[0];
    param->subroutine.last.glindex = index;
  }
  ctx->program.dirtySubroutines |= lxgProgramStage_bit(param->stage);

}

static void lxgUpdateTextureArray(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  lxgContext_checkedTextures(ctx, (lxgTexturePTR*)data,param->uniform.unit,param->uniform.count);
}
static void lxgUpdateImageArray(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  lxgContext_checkedTextureImages(ctx, (lxgTextureImagePTR*)data,param->uniform.unit,param->uniform.count);
}

static void lxgUpdateTexture(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  lxgContext_checkedTexture(ctx, *(lxgTexturePTR*)data,param->uniform.unit);
}
static void lxgUpdateImage(lxgProgramParameterPTR param, lxgContextPTR ctx, const void* data){
  lxgContext_checkedTextureImage(ctx, *(lxgTextureImagePTR*)data,param->uniform.unit);
}

LUX_API void lxgProgramParameter_initFuncNV( lxgProgramParameterPTR param, lxgProgramStage_t domain )
{
  if (param->type == LUXGL_PARAM_BUFFER){
    static lxGLBufferTarget_t types[LUXGFX_STAGES] = {
      LUXGL_BUFFER_NVPARAM_VERTEX,
      LUXGL_BUFFER_NVPARAM_FRAGMENT,
      LUXGL_BUFFER_NVPARAM_GEOMETRY,
      LUXGL_BUFFER_NVPARAM_TESSCTRL,
      LUXGL_BUFFER_NVPARAM_TESSEVAL,
    };

    param->gltarget = types[domain];
  }
  else{
    static lxGLProgramType_t types[LUXGFX_STAGES] = {
      LUXGL_PROGRAM_VERTEX,
      LUXGL_PROGRAM_FRAGMENT,
      LUXGL_PROGRAM_GEOMETRY,
      LUXGL_PROGRAM_TESSCTRL,
      LUXGL_PROGRAM_TESSEVAL,
    };

    param->gltarget = types[domain];
  }

  param->func = NULL;
  switch(param->type)
  {
  case LUXGL_PARAM_FLOAT:
    param->func = lxgUpdateFloat1NV; return;
  case LUXGL_PARAM_FLOAT2:
    param->func = lxgUpdateFloat2NV; return;
  case LUXGL_PARAM_FLOAT3:
    param->func = lxgUpdateFloat3NV; return;
  case LUXGL_PARAM_FLOAT4:
    param->func = lxgUpdateFloat4NV; return;
  case LUXGL_PARAM_INT:
    param->func = lxgUpdateInt1NV; return;
  case LUXGL_PARAM_INT2:
    param->func = lxgUpdateInt2NV; return;
  case LUXGL_PARAM_INT3:
    param->func = lxgUpdateInt3NV; return;
  case LUXGL_PARAM_INT4:
    param->func = lxgUpdateInt4NV; return;
  case LUXGL_PARAM_UINT:
    param->func = lxgUpdateUInt1NV; return;
  case LUXGL_PARAM_UINT2:
    param->func = lxgUpdateUInt2NV; return;
  case LUXGL_PARAM_UINT3:
    param->func = lxgUpdateUInt3NV; return;
  case LUXGL_PARAM_UINT4:
    param->func = lxgUpdateUInt4NV; return;
  case LUXGL_PARAM_BOOL:
    param->func = lxgUpdateFloat1NV; return;
  case LUXGL_PARAM_BOOL2:
    param->func = lxgUpdateFloat2NV; return;
  case LUXGL_PARAM_BOOL3:
    param->func = lxgUpdateFloat3NV; return;
  case LUXGL_PARAM_BOOL4:
    param->func = lxgUpdateFloat4NV; return;
  case LUXGL_PARAM_MAT2:
    param->func = lxgUpdateMat2NV; return;
  case LUXGL_PARAM_MAT3:
    param->func = lxgUpdateMat3NV; return;
  case LUXGL_PARAM_MAT4:
    param->func = lxgUpdateMat4NV; return;
  case LUXGL_PARAM_MAT2x3:
    param->func = lxgUpdateMat2x3NV; return;
  case LUXGL_PARAM_MAT2x4:
    param->func = lxgUpdateMat2x4NV; return;
  case LUXGL_PARAM_MAT3x2:
    param->func = lxgUpdateMat3x2NV; return;
  case LUXGL_PARAM_MAT3x4:
    param->func = lxgUpdateMat3x4NV; return;
  case LUXGL_PARAM_MAT4x2:
    param->func = lxgUpdateMat4x2NV; return;
  case LUXGL_PARAM_MAT4x3:
    param->func = lxgUpdateMat4x3NV; return;
  case LUXGL_PARAM_BUFFER:
    param->func = lxgUpdateBufferNV; return;
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
    param->func = param->uniform.count == 1 ? lxgUpdateTexture : lxgUpdateTextureArray; return;

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
    param->func = param->uniform.count == 1 ? lxgUpdateImage : lxgUpdateImageArray; return;

  case LUXGL_PARAM_GPU_ADDRESS:
    param->func = lxgUpdateUint64NV; return;

  case LUXGL_PARAM_SUBROUTINE:
    param->stage = domain;
    param->func = lxgUpdateSubroutine; return;

  case LUXGL_PARAM_USER:
    return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}

LUX_API void lxgProgramParameter_initFunc( lxgProgramParameterPTR param)
{
  param->func = NULL;
  switch(param->type)
  {
  case LUXGL_PARAM_FLOAT:
    param->func = lxgUpdateFloat1GLSL; return;
  case LUXGL_PARAM_FLOAT2:
    param->func = lxgUpdateFloat2GLSL; return;
  case LUXGL_PARAM_FLOAT3:
    param->func = lxgUpdateFloat3GLSL; return;
  case LUXGL_PARAM_FLOAT4:
    param->func = lxgUpdateFloat4GLSL; return;
  case LUXGL_PARAM_INT:
    param->func = lxgUpdateInt1GLSL; return;
  case LUXGL_PARAM_INT2:
    param->func = lxgUpdateInt2GLSL; return;
  case LUXGL_PARAM_INT3:
    param->func = lxgUpdateInt3GLSL; return;
  case LUXGL_PARAM_INT4:
    param->func = lxgUpdateInt4GLSL; return;
  case LUXGL_PARAM_UINT:
    param->func = lxgUpdateUInt1GLSL; return;
  case LUXGL_PARAM_UINT2:
    param->func = lxgUpdateUInt2GLSL; return;
  case LUXGL_PARAM_UINT3:
    param->func = lxgUpdateUInt3GLSL; return;
  case LUXGL_PARAM_UINT4:
    param->func = lxgUpdateUInt4GLSL; return;
  case LUXGL_PARAM_BOOL:
    param->func = lxgUpdateFloat1GLSL; return;
  case LUXGL_PARAM_BOOL2:
    param->func = lxgUpdateFloat2GLSL; return;
  case LUXGL_PARAM_BOOL3:
    param->func = lxgUpdateFloat3GLSL; return;
  case LUXGL_PARAM_BOOL4:
    param->func = lxgUpdateFloat4GLSL; return;
  case LUXGL_PARAM_MAT2:
    param->func = lxgUpdateMat2GLSL; return;
  case LUXGL_PARAM_MAT3:
    param->func = lxgUpdateMat3GLSL; return;
  case LUXGL_PARAM_MAT4:
    param->func = lxgUpdateMat4GLSL; return;
  case LUXGL_PARAM_MAT2x3:
    param->func = lxgUpdateMat2x3GLSL; return;
  case LUXGL_PARAM_MAT2x4:
    param->func = lxgUpdateMat2x4GLSL; return;
  case LUXGL_PARAM_MAT3x2:
    param->func = lxgUpdateMat3x2GLSL; return;
  case LUXGL_PARAM_MAT3x4:
    param->func = lxgUpdateMat3x4GLSL; return;
  case LUXGL_PARAM_MAT4x2:
    param->func = lxgUpdateMat4x2GLSL; return;
  case LUXGL_PARAM_MAT4x3:
    param->func = lxgUpdateMat4x3GLSL; return;
  case LUXGL_PARAM_BUFFER:
    param->func = lxgUpdateBufferGLSL; return;
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
    param->func = param->uniform.count == 1 ? lxgUpdateTexture : lxgUpdateTextureArray; return;

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
    param->func = param->uniform.count == 1 ? lxgUpdateImage : lxgUpdateImageArray; return;

  case LUXGL_PARAM_GPU_ADDRESS:
    param->func = lxgUpdateUint64GLSL; return;

  case LUXGL_PARAM_SUBROUTINE:
    param->func = lxgUpdateSubroutine; return;

  case LUXGL_PARAM_USER:
    return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}

LUX_API void lxgProgramParameter_initFuncSEP( lxgProgramParameterPTR param, GLuint progid)
{
  if (param->type != LUXGL_PARAM_USER && 
      param->type != LUXGL_PARAM_SUBROUTINE)
  {
    param->glid = progid;
  }
  
  param->func = NULL;
  switch(param->type)
  {
  case LUXGL_PARAM_FLOAT:
    param->func = lxgUpdateFloat1SEP; return;
  case LUXGL_PARAM_FLOAT2:
    param->func = lxgUpdateFloat2SEP; return;
  case LUXGL_PARAM_FLOAT3:
    param->func = lxgUpdateFloat3SEP; return;
  case LUXGL_PARAM_FLOAT4:
    param->func = lxgUpdateFloat4SEP; return;
  case LUXGL_PARAM_INT:
    param->func = lxgUpdateInt1SEP; return;
  case LUXGL_PARAM_INT2:
    param->func = lxgUpdateInt2SEP; return;
  case LUXGL_PARAM_INT3:
    param->func = lxgUpdateInt3SEP; return;
  case LUXGL_PARAM_INT4:
    param->func = lxgUpdateInt4SEP; return;
  case LUXGL_PARAM_UINT:
    param->func = lxgUpdateUInt1SEP; return;
  case LUXGL_PARAM_UINT2:
    param->func = lxgUpdateUInt2SEP; return;
  case LUXGL_PARAM_UINT3:
    param->func = lxgUpdateUInt3SEP; return;
  case LUXGL_PARAM_UINT4:
    param->func = lxgUpdateUInt4SEP; return;
  case LUXGL_PARAM_BOOL:
    param->func = lxgUpdateFloat1SEP; return;
  case LUXGL_PARAM_BOOL2:
    param->func = lxgUpdateFloat2SEP; return;
  case LUXGL_PARAM_BOOL3:
    param->func = lxgUpdateFloat3SEP; return;
  case LUXGL_PARAM_BOOL4:
    param->func = lxgUpdateFloat4SEP; return;
  case LUXGL_PARAM_MAT2:
    param->func = lxgUpdateMat2SEP; return;
  case LUXGL_PARAM_MAT3:
    param->func = lxgUpdateMat3SEP; return;
  case LUXGL_PARAM_MAT4:
    param->func = lxgUpdateMat4SEP; return;
  case LUXGL_PARAM_MAT2x3:
    param->func = lxgUpdateMat2x3SEP; return;
  case LUXGL_PARAM_MAT2x4:
    param->func = lxgUpdateMat2x4SEP; return;
  case LUXGL_PARAM_MAT3x2:
    param->func = lxgUpdateMat3x2SEP; return;
  case LUXGL_PARAM_MAT3x4:
    param->func = lxgUpdateMat3x4SEP; return;
  case LUXGL_PARAM_MAT4x2:
    param->func = lxgUpdateMat4x2SEP; return;
  case LUXGL_PARAM_MAT4x3:
    param->func = lxgUpdateMat4x3SEP; return;
  case LUXGL_PARAM_BUFFER:
    param->func = lxgUpdateBufferGLSL; return;
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
    param->func = param->uniform.count == 1 ? lxgUpdateTexture : lxgUpdateTextureArray; return;

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
    param->func = param->uniform.count == 1 ? lxgUpdateImage : lxgUpdateImageArray; return;

  case LUXGL_PARAM_GPU_ADDRESS:
    param->func = lxgUpdateUint64SEP; return;

  case LUXGL_PARAM_SUBROUTINE:
    param->func = lxgUpdateSubroutine; return;

  case LUXGL_PARAM_USER:
    return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}


//////////////////////////////////////////////////////////////////////////

LUX_API void lxgProgram_initSEP( lxgProgramPTR prog, lxgContextPTR ctx)
{
  memset(prog,0,sizeof(lxgProgram_t));
  prog->ctx = ctx;
  glGenProgramPipelines(1,&prog->glid);
  prog->type = LUXGFX_PROGRAM_GLSLSEP;
}

LUX_API void lxgProgram_deinitSEP( lxgProgramPTR prog, lxgContextPTR ctx )
{
  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSLSEP);
  glDeleteProgramPipelines(1,&prog->glid);
}

LUX_API void lxgProgram_setSEP( lxgProgramPTR prog, lxgProgramPTR stage )
{
  GLenum typebits[] = {
    GL_VERTEX_SHADER_BIT,
    GL_FRAGMENT_SHADER_BIT,
    GL_GEOMETRY_SHADER_BIT,
    GL_TESS_CONTROL_SHADER_BIT,
    GL_TESS_EVALUATION_SHADER_BIT,
  };

  GLenum usedbits = 0;
  int i;

  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSLSEP);

  for (i = 0; i < LUXGFX_STAGES; i++){
    LUX_DEBUGASSERT(stage->type == LUXGFX_PROGRAM_GLSL);
    LUX_DEBUGASSERT(stage->isSeparable);
    if (stage->usedProgs & lxgProgramStage_bit(i)){
      prog->sepPrograms[i] = stage;
      prog->usedProgs |= lxgProgramStage_bit(i);
      usedbits |= typebits[i];

      prog->hasSubroutines |= stage->hasSubroutines;
      prog->numSubroutines[i] = stage->numSubroutines[i];
    }
  }
  
  glUseProgramStages(prog->glid,usedbits,stage->glid);
}

LUX_API const char* lxgProgram_logSEP( lxgProgramPTR prog, char* buffer, int len)
{
  GLsizei used;
  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSLSEP);
  glGetProgramPipelineInfoLog(prog->glid, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

//////////////////////////////////////////////////////////////////////////

LUX_API void lxgProgram_init( lxgProgramPTR prog, lxgContextPTR ctx)
{
  memset(prog,0,sizeof(lxgProgram_t));
  prog->ctx = ctx;
  prog->glid = glCreateProgram();
  prog->type = LUXGFX_PROGRAM_GLSL;
  prog->isSeparable = LUX_FALSE;
}

LUX_API void lxgProgram_initForSEP( lxgProgramPTR prog, lxgContextPTR ctx)
{
  memset(prog,0,sizeof(lxgProgram_t));
  prog->ctx = ctx;
  prog->glid = glCreateProgram();
  prog->type = LUXGFX_PROGRAM_GLSL;
  prog->isSeparable = LUX_TRUE;
  glProgramParameteri(prog->glid, GL_PROGRAM_SEPARABLE, GL_TRUE);
}


LUX_API void lxgProgram_deinit( lxgProgramPTR prog, lxgContextPTR ctx )
{
  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSL);
  glDeleteProgram(prog->glid);
}

LUX_API void lxgProgram_setStage( lxgProgramPTR prog, lxgProgramStage_t type, lxgStageProgramPTR stage )
{
  LUX_DEBUGASSERT(stage->progtype == prog->type);
  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSL);
  prog->stagePrograms[type] = stage;
  prog->usedProgs |= lxgProgramStage_bit(type);
  glAttachShader(prog->glid,stage->glid);
}

LUX_API booln lxgProgram_link( lxgProgramPTR prog )
{
  GLint status;
  GLuint id = prog->glid;
  GLint len;

  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSL);

  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
  if (!status) return len;
  return 0;
}

LUX_API const char* lxgProgram_log( lxgProgramPTR prog, char* buffer, int len)
{
  GLsizei used;
  glGetProgramInfoLog(prog->glid, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

LUX_API int lxgProgram_getSubroutineCount(lxgProgramPTR prog, int* namesSize)
{
  static char buffer[256];
  int i;
  int s;
  int num;
  int nameLen;
  int outNum = 0;
  int outNameLen = 0;
  GLint maxNameLen;

  if (!prog->ctx->capbits & LUXGFX_CAP_SM5) return 0;

  for (s = 0; s < LUXGFX_STAGES; s++){
    GLenum gltype = lxgProgramStage_typeGL(s);
    if (! (prog->usedProgs & lxgProgramStage_bit(s))) continue;

    glGetProgramStageiv(prog->glid,gltype,GL_ACTIVE_SUBROUTINES, &num);
    glGetProgramStageiv(prog->glid,gltype,GL_ACTIVE_SUBROUTINE_MAX_LENGTH, &maxNameLen);
    outNum += num;
    for (i = 0; i < num; i++){
      glGetActiveSubroutineName(prog->glid,gltype,i,sizeof(buffer),&nameLen,buffer);
      if (nameLen >= sizeof(buffer) - 1){
        nameLen = maxNameLen - 1;
      }
      outNameLen  += nameLen + 1;
    }
  }

  if (namesSize){
    *namesSize = outNameLen;
  }

  return outNum;
}

LUX_API int lxgProgram_getParameterCount( lxgProgramPTR prog, int* namesSize, int* totalCompatibleSubroutines)
{
  GLint num;
  int maxNameLen;
  int nameLen;
  int outNum = 0;
  int outNameLen = 0;
  int outCompSubs = 0;
  int outSubNamenLen = 0;
  booln sm4 = prog->ctx->capbits & LUXGFX_CAP_SM4;
  booln sm5 = prog->ctx->capbits & LUXGFX_CAP_SM5;
  int i;
  char buffer[256];
    
  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSL);
  
  // all uniforms
  glGetProgramiv(prog->glid,GL_ACTIVE_UNIFORMS,&num);
  glGetProgramiv(prog->glid,GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxNameLen);
  for (i = 0 ; i < num; i++){
    GLsizei sz;
    GLenum  type;
    if (sm4){
      // ignore buffered
      GLint block;
      glGetActiveUniformsiv(prog->glid, 1, &i, GL_UNIFORM_BLOCK_INDEX,&block);
      if (block >= 0){
        continue;
      }
    }
    glGetActiveUniform(prog->glid, i, sizeof(buffer), &nameLen, &sz, &type, buffer);
    if (strstr(buffer,"gl_")){
      continue;
    }

    if (nameLen >= sizeof(buffer) - 1){
      nameLen = maxNameLen - 1;
    }
    outNameLen  += nameLen + 1;
    outNum++;
  }
  if (sm4){
    // add buffers
    glGetProgramiv(prog->glid,GL_ACTIVE_UNIFORM_BLOCKS, &num);
    outNum += num;
    for (i = 0 ; i < num; i++){
      glGetActiveUniformBlockiv(prog->glid,i,GL_UNIFORM_BLOCK_NAME_LENGTH,&nameLen);
      outNameLen += nameLen;
    }
    
  }
  if (sm5){
    // add subroutines
    int s;
    for (s = 0; s < LUXGFX_STAGES; s++){
      GLenum gltype = lxgProgramStage_typeGL(s);
      if (! (prog->usedProgs & lxgProgramStage_bit(s))) continue;
      
      glGetProgramStageiv(prog->glid,gltype,GL_ACTIVE_SUBROUTINE_UNIFORMS, &num);
      outNum += num;
      for (i = 0; i < num; i++){
        GLint numCompatible;
        glGetActiveSubroutineUniformiv(prog->glid, gltype, i, GL_NUM_COMPATIBLE_SUBROUTINES, &numCompatible);
        glGetActiveSubroutineUniformiv(prog->glid, gltype, i, GL_UNIFORM_NAME_LENGTH, &nameLen);
        outNameLen  += nameLen;
        outCompSubs += numCompatible;
      }
    }
  }
  if (namesSize){
    *namesSize = outNameLen;
  }
  if (totalCompatibleSubroutines){
    *totalCompatibleSubroutines = outCompSubs;
  }

  return outNum;
}

LUX_API void lxgProgram_initSubroutineParameters(
   lxgProgramPTR prog, int numParams, lxgProgramParameter_t* params,
   int namesSize, char* namesBuffer, char** subroutineNames,
   int compatibles, lxgProgramSubroutine_t *compatibleData )
{
  int i;
  int c;
  int s;
  GLint *indices;
  lxgProgramSubroutine_t *compatibleCur = compatibleData;
  char* namesOrig = namesBuffer;

  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSL);

  if (!prog->ctx->capbits & LUXGFX_CAP_SM5) return;

  // fill names
  for (s = 0; s < LUXGFX_STAGES; s++){
    GLenum gltype = lxgProgramStage_typeGL(s);
    GLint num;
    GLint written;
    if (! (prog->usedProgs & lxgProgramStage_bit(s))) continue;

    glGetProgramStageiv(prog->glid,gltype,GL_ACTIVE_SUBROUTINES, &num);
    for (i = 0; i < num; i++){
      LUX_DEBUGASSERT(namesSize > 0);

      glGetActiveSubroutineName(prog->glid,gltype,i,namesSize,&written,namesBuffer);
      subroutineNames[i] = namesBuffer;

      namesBuffer += ++written;
      namesSize -= written;
    }
  }
  LUX_DEBUGASSERT(namesSize == 0);

  for (i = 0; i < numParams; i++, params++){
    if (params->type != LUXGL_PARAM_SUBROUTINE) continue;
    LUX_DEBUGASSERT(compatibleCur < compatibleData + compatibles);

    params->subroutine.compatible = compatibleCur;
    indices = (GLint*)compatibleCur;
    glGetActiveSubroutineUniformiv(prog->glid, lxgProgramStage_typeGL(params->stage), params->gllocation, GL_COMPATIBLE_SUBROUTINES, indices);

    // run reverse as we "overlap" with the indices data
    for (c = params->subroutine.numCompatible-1; c >= 0; c--){
      compatibleCur[c].glindex = indices[c];
      compatibleCur[c].namekey = subroutineNames[indices[c]] - namesOrig;
    }

    compatibleCur += params->subroutine.numCompatible;
  }

  LUX_DEBUGASSERT( compatibleCur == compatibleData + compatibles );
}

LUX_API void lxgProgram_initParameters( lxgProgramPTR prog, int numParams, lxgProgramParameter_t* paramsBuffer, int namesSize, char* namesBuffer)
{
  GLint num;
  GLsizei written;
  booln sm4 = prog->ctx->capbits & LUXGFX_CAP_SM4;
  booln sm5 = prog->ctx->capbits & LUXGFX_CAP_SM5;
  lxgProgramParameter_t* params = paramsBuffer;
  int i;
  char checkBuffer[4];
  booln setTextures = LUX_TRUE;
  booln setImages = LUX_TRUE;
  booln setBuffers = LUX_TRUE;
  glGetProgramiv(prog->glid,GL_ACTIVE_UNIFORMS,&num);

  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_GLSL);

  // first regular uniforms
  for (i = 0 ; i < num; i++){
    GLsizei sz;
    GLenum type;
    if (sm4){
      // discard buffered
      GLint block;
      glGetActiveUniformsiv(prog->glid, 1, &i, GL_UNIFORM_BLOCK_INDEX,&block);
      if (block >= 0){
        continue;
      }
    }
    
    glGetActiveUniform(prog->glid,i,sizeof(checkBuffer),&written,&sz,&type,checkBuffer);
    checkBuffer[sizeof(checkBuffer)- 1] = 0;
    if (strstr(checkBuffer,"gl_")){
      continue;
    }

    LUX_DEBUGASSERT(namesSize > 0);
    LUX_DEBUGASSERT(params < paramsBuffer + numParams);

    glGetActiveUniform(prog->glid,i,namesSize,&written,&sz,&type,namesBuffer);

    if (sz > 1 && strcmp(&namesBuffer[written-3],"[0]")==0){
      // could be nvidia specific: name[0] for arrays
      namesBuffer[written - 3] = 0;
    }
    params->uniform.count = sz;
    params->name = namesBuffer;
    params->type = type;
    params->gllocation = glGetUniformLocation(prog->glid,namesBuffer);

    if ( lxGLParameterType_isTexture(type) || lxGLParameterType_isImage(type)){
      glGetUniformiv(prog->glid, params->gllocation, &params->uniform.unit);
      if (params->uniform.unit != 0){
        if (lxGLParameterType_isTexture(type)){
          setTextures = LUX_FALSE;
        }
        if (lxGLParameterType_isImage(type)){
          setImages = LUX_FALSE;
        }
      }
    }

    namesBuffer += ++written;
    namesSize -= written;
    params++;
  }
  if (sm4){
    // add buffers
    glGetProgramiv(prog->glid,GL_ACTIVE_UNIFORM_BLOCKS, &num);
    for (i = 0 ; i < num; i++){
      LUX_DEBUGASSERT(params < paramsBuffer + numParams);
      LUX_DEBUGASSERT(namesSize > 0);
      params->gllocation = i;
      glGetActiveUniformBlockiv(prog->glid, i, GL_UNIFORM_BLOCK_BINDING, &params->buffer.unit);
      glGetActiveUniformBlockiv(prog->glid, i, GL_UNIFORM_BLOCK_DATA_SIZE, &params->buffer.size);
      glGetActiveUniformBlockName(prog->glid, i, namesSize, &written, namesBuffer);
      params->name = namesBuffer;
      params->type = LUXGL_PARAM_BUFFER;

      if (params->buffer.unit != 0){
        setBuffers = LUX_FALSE;
      }

      namesBuffer += ++written;
      namesSize -= written;
      params++;
    }
  }
  else{
    setBuffers = LUX_FALSE;
  }

  if (sm5){
    // add subroutines
    int s;
    for (s = 0; s < LUXGFX_STAGES; s++){
      GLenum gltype = lxgProgramStage_typeGL(s);
      if (! (prog->usedProgs & lxgProgramStage_bit(s))) continue;
      
      glGetProgramStageiv(prog->glid,gltype,GL_ACTIVE_SUBROUTINE_UNIFORMS, &num);
      prog->stagePrograms[s]->numSubroutines = num;
      prog->hasSubroutines |= num > 0;
      prog->numSubroutines[s] = num;
      for (i = 0 ; i < num; i++){
        GLint num;
        LUX_DEBUGASSERT(params < paramsBuffer + numParams);
        LUX_DEBUGASSERT(namesSize > 0);
        glGetActiveSubroutineUniformiv(prog->glid, gltype, i, GL_NUM_COMPATIBLE_SUBROUTINES, &num);
        params->subroutine.numCompatible = num;
        glGetActiveSubroutineUniformiv(prog->glid, gltype, i, GL_UNIFORM_SIZE, &num);
        params->subroutine.count = num;
        glGetActiveSubroutineUniformName(prog->glid,gltype,i,namesSize, &written, namesBuffer);
        
        if (num > 1 && strcmp(&namesBuffer[written-3],"[0]")==0){
          // could be nvidia specific: name[0] for arrays
          namesBuffer[written - 3] = 0;
        }

        params->type = LUXGL_PARAM_SUBROUTINE;
        params->name = namesBuffer;
        params->gllocation = glGetSubroutineUniformLocation(prog->glid, gltype, namesBuffer);
        params->stage = s;

        namesBuffer += ++written;
        namesSize -= written;
        params++;
      }
    }
  }

  if (prog->isSeparable){
    for (i = 0; i < numParams; i++){
      lxgProgramParameter_initFuncSEP(&paramsBuffer[i],prog->glid);
    }
  }
  else{
    for (i = 0; i < numParams; i++){
      lxgProgramParameter_initFunc(&paramsBuffer[i]);
    }
  }

  if (setImages || setTextures){
    uint tex = 0;
    uint img = 0;
    if (!prog->isSeparable){
      glUseProgram(prog->glid);
      lxGLErrorCheck();
    }
    for (i = 0 ; i < numParams; i++){
      booln doit = LUX_FALSE;
      if (setTextures && lxGLParameterType_isTexture(paramsBuffer[i].type)){
        paramsBuffer[i].uniform.unit = tex;
        tex += paramsBuffer[i].uniform.count;
        doit = LUX_TRUE;
      }
      if (setImages && lxGLParameterType_isImage(paramsBuffer[i].type)){
        paramsBuffer[i].uniform.unit = img;
        img += paramsBuffer[i].uniform.count;
        doit = LUX_TRUE;
      }

      if (doit){
        if (prog->isSeparable){
          glProgramUniform1i(prog->glid,paramsBuffer[i].gllocation,paramsBuffer[i].uniform.unit);
        }
        else{
          glUniform1i(paramsBuffer[i].gllocation,paramsBuffer[i].uniform.unit);
        }
        lxGLErrorCheck();
      }
    }
    if (!prog->isSeparable){
      glUseProgram(0);
    }
  }
  
  if (setBuffers){
    uint buf = 0;
    for (i = 0 ; i < numParams; i++){
      if (paramsBuffer[i].type == LUXGL_PARAM_BUFFER){
        glUniformBlockBinding(prog->glid,paramsBuffer[i].gllocation,buf);
        paramsBuffer[i].buffer.unit = buf++;
      }
    }
  }
  

  LUX_DEBUGASSERT(namesSize == 0);

}

//////////////////////////////////////////////////////////////////////////

static LUX_INLINE lxgProgram_stateNV(flags32 flags, flags32 changed, const lxgStageProgramCPTR* domains)
{
  if (changed & lxgProgramStage_bit(LUXGFX_STAGE_VERTEX)){
    if (flags & lxgProgramStage_bit(LUXGFX_STAGE_VERTEX)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_VERTEX_PROGRAM_ARB,domains[LUXGFX_STAGE_VERTEX]->glid);
      glEnable(GL_VERTEX_PROGRAM_ARB);
    }
    else{
      glDisable(GL_VERTEX_PROGRAM_ARB);
      glBindProgramARB(GL_VERTEX_PROGRAM_ARB,0);
    }
  }
  if (changed & lxgProgramStage_bit(LUXGFX_STAGE_FRAGMENT)){
    if (flags & lxgProgramStage_bit(LUXGFX_STAGE_FRAGMENT)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,domains[LUXGFX_STAGE_FRAGMENT]->glid);
      glEnable(GL_FRAGMENT_PROGRAM_ARB);
    }
    else{
      glDisable(GL_FRAGMENT_PROGRAM_ARB);
      glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,0);
    }
  }
  if (changed & lxgProgramStage_bit(LUXGFX_STAGE_GEOMETRY)){
    if (flags & lxgProgramStage_bit(LUXGFX_STAGE_GEOMETRY)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_GEOMETRY_PROGRAM_NV,domains[LUXGFX_STAGE_GEOMETRY]->glid);
      glEnable(GL_GEOMETRY_PROGRAM_NV);
    }
    else{
      glDisable(GL_GEOMETRY_PROGRAM_NV);
      glBindProgramARB(GL_GEOMETRY_PROGRAM_NV,0);
    }
  }
  if (changed & lxgProgramStage_bit(LUXGFX_STAGE_TESSCTRL)){
    if (flags & lxgProgramStage_bit(LUXGFX_STAGE_TESSCTRL)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_TESS_CONTROL_PROGRAM_NV,domains[LUXGFX_STAGE_TESSCTRL]->glid);
      glEnable(GL_TESS_CONTROL_PROGRAM_NV);
    }
    else{
      glDisable(GL_TESS_CONTROL_PROGRAM_NV);
      glBindProgramARB(GL_TESS_CONTROL_PROGRAM_NV,0);
    }
  }
  if (changed & lxgProgramStage_bit(LUXGFX_STAGE_TESSEVAL)){
    if (flags & lxgProgramStage_bit(LUXGFX_STAGE_TESSEVAL)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_TESS_EVALUATION_PROGRAM_NV,domains[LUXGFX_STAGE_TESSEVAL]->glid);
      glEnable(GL_TESS_EVALUATION_PROGRAM_NV);
    }
    else{
      glDisable(GL_TESS_EVALUATION_PROGRAM_NV);
      glBindProgramARB(GL_TESS_EVALUATION_PROGRAM_NV,0);
    }
  }
}

LUX_API void  lxgContext_applyProgram( lxgContextPTR ctx, lxgProgramCPTR prog)
{
  lxgProgramType_t type = prog ? prog->type : LUXGFX_PROGRAM_NONE;
  lxgProgramCPTR   oldprog = ctx->program.current;
  lxgProgramType_t oldtype = oldprog ? oldprog->type : LUXGFX_PROGRAM_NONE;
  int i;
  if (type != oldtype){
    switch (oldtype){
    case LUXGFX_PROGRAM_GLSL:
      glUseProgram(0);
      break;
    case LUXGFX_PROGRAM_GLSLSEP:
      glBindProgramPipeline(0);
      break;
    case LUXGFX_PROGRAM_NV:
      lxgProgram_stateNV(0,oldprog->usedProgs,NULL);
      break;
    }
    lxGLErrorCheck();
  }

  switch(type){
    case LUXGFX_PROGRAM_GLSL:
      glUseProgram(prog->glid);
      break;
    case LUXGFX_PROGRAM_GLSLSEP:
      glBindProgramPipeline(prog->glid);
      break;
    case LUXGFX_PROGRAM_NV:
      lxgProgram_stateNV(prog->usedProgs,oldprog->usedProgs ^ prog->usedProgs, prog->stagePrograms);
      break;
  }
  lxGLErrorCheck();
  ctx->program.current = prog;
  ctx->program.dirtySubroutines = 0;
  if (prog && prog->hasSubroutines){
    for (i = 0; i < LUXGFX_STAGES; i++){
      ctx->program.numSubroutines[i] = prog->numSubroutines[i];
      ctx->program.typeSubroutines[i] = type != LUXGFX_PROGRAM_NV ? lxgProgramStage_typeGL(i) : lxgProgramStage_typeNV(i);
    }
  }
}


LUX_INLINE LUX_API void lxgContext_updateProgramSubroutines( lxgContextPTR ctx, lxgProgramCPTR prog){
  lxgProgramState_t* state = &ctx->program;
  int i;
  LUX_DEBUGASSERT(ctx->program.current == prog);
  if (prog->type == LUXGFX_PROGRAM_NV){
    for (i = 0; i < LUXGFX_STAGES; i++){
      if (ctx->program.dirtySubroutines & lxgProgramStage_bit(i)){
        //glProgramSubroutineParametersuivNV(state->typeSubroutines[i],state->numSubroutines[i],state->subroutines[i]);
      }
    }
  }
  else{
    for (i = 0; i < LUXGFX_STAGES; i++){
      if (ctx->program.dirtySubroutines & lxgProgramStage_bit(i)){
        glUniformSubroutinesuiv(state->typeSubroutines[i],state->numSubroutines[i],state->subroutines[i]);
      }
    }
  }
  lxGLErrorCheck();
  ctx->program.dirtySubroutines = 0;
}

LUX_API void lxgContext_clearProgramState( lxgContextPTR ctx )
{
  memset(&ctx->program,0,sizeof(ctx->program));
}

//////////////////////////////////////////////////////////////////////////

LUX_API void lxgProgram_initNV( lxgProgramPTR prog, lxgContextPTR ctx )
{
  memset(prog,0,sizeof(lxgProgram_t));
  prog->ctx = ctx;
  prog->isSeparable = LUX_TRUE;
  prog->type = LUXGFX_PROGRAM_NV;
}

LUX_API void lxgProgram_deinitNV( lxgProgramPTR prog, lxgContextPTR ctx )
{
  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_NV);
}

LUX_API void lxgProgram_setStageNV( lxgProgramPTR prog, lxgProgramStage_t type, lxgStageProgramPTR stage )
{
  LUX_DEBUGASSERT(stage->progtype == prog->type);
  LUX_DEBUGASSERT(prog->type == LUXGFX_PROGRAM_NV);
  prog->stagePrograms[type] = stage;
  prog->usedProgs |= lxgProgramStage_bit(type);
}

//////////////////////////////////////////////////////////////////////////


LUX_API void lxgStageProgram_init( lxgStageProgramPTR stage, lxgContextPTR ctx, lxgProgramStage_t type)
{
  GLenum gltype = lxgProgramStage_typeGL(type);
  memset(stage,0,sizeof(lxgStageProgram_t));

  stage->ctx = ctx;
  stage->glid = glCreateShader(gltype);
  stage->gltype = gltype;
  stage->progtype = LUXGFX_PROGRAM_GLSL;
}

LUX_API void lxgStageProgram_deinit( lxgStageProgramPTR stage, lxgContextPTR ctx )
{
  LUX_DEBUGASSERT(stage->progtype == LUXGFX_PROGRAM_GLSL);
  glDeleteShader(stage->glid);
}

LUX_API int lxgStageProgram_compile( lxgStageProgramPTR stage, const char *src, int len )
{
  GLuint id = stage->glid;
  GLint status;
  GLint errlen;
  LUX_DEBUGASSERT(stage->progtype == LUXGFX_PROGRAM_GLSL);
  glShaderSource(id, 1, &src, &len);
  glCompileShader(id);
  glGetShaderiv(id,GL_COMPILE_STATUS,&status);
  glGetShaderiv(id,GL_INFO_LOG_LENGTH,&errlen);
  return status ? 0 : errlen;
}

LUX_API const char* lxgStageProgram_error( lxgStageProgramPTR stage, char *buffer, int len  )
{
  GLsizei used;
  LUX_DEBUGASSERT(stage->progtype == LUXGFX_PROGRAM_GLSL);
  glGetShaderInfoLog(stage->glid, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

//////////////////////////////////////////////////////////////////////////


LUX_API void lxgStageProgram_initNV( lxgStageProgramPTR stage, lxgContextPTR ctx, lxgProgramStage_t type)
{
  memset(stage,0,sizeof(lxgStageProgram_t));

  stage->ctx = ctx;
  glGenProgramsARB(1,&stage->glid);
  stage->gltarget = lxgProgramStage_typeNV(type);
  stage->progtype = LUXGFX_PROGRAM_NV;
}

LUX_API void lxgStageProgram_deinitNV( lxgStageProgramPTR stage, lxgContextPTR ctx)
{
  LUX_DEBUGASSERT(stage->progtype == LUXGFX_PROGRAM_NV);
  glDeleteProgramsARB(1,&stage->glid);
}

LUX_API int lxgStageProgram_compileNV(lxgStageProgramPTR stage, const char *src, int len)
{
  GLint pos;
  LUX_DEBUGASSERT(stage->progtype == LUXGFX_PROGRAM_NV);
  glBindProgramARB(stage->gltarget,stage->glid);
  glProgramStringARB(stage->gltarget,GL_PROGRAM_FORMAT_ASCII_ARB,len,src);
  glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB,&pos);
  
  return (pos == -1) ? 0 : (int)strlen(glGetString(GL_PROGRAM_ERROR_STRING_ARB))+1;
}
LUX_API const char* lxgStageProgram_errorNV(lxgStageProgramPTR stage, char *buffer, int len)
{
  GLint pos;
  LUX_DEBUGASSERT(stage->progtype == LUXGFX_PROGRAM_NV);
  glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB,&pos);
  if (pos > -1){
    const char* str = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
    strncpy(buffer,str,len);
    return buffer;
  }
  return NULL;
}


