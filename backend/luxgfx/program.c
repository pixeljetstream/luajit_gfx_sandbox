// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/luxgfx.h>
#include <luxgfx/program.h>
#include <luxmath/misc.h>
#include <luxplatform/debug.h>

#include "state_inl.h"


static void lxgUpdateFloat1GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform1fv(param->gllocation, param->count, data);
}
static void lxgUpdateFloat2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform2fv(param->gllocation, param->count, data);
}
static void lxgUpdateFloat3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform3fv(param->gllocation, param->count, data);
}
static void lxgUpdateFloat4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform4fv(param->gllocation, param->count, data);
}

static void lxgUpdateInt1GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform1iv(param->gllocation, param->count, data);
}
static void lxgUpdateInt2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform2iv(param->gllocation, param->count, data);
}
static void lxgUpdateInt3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform3iv(param->gllocation, param->count, data);
}
static void lxgUpdateInt4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform4iv(param->gllocation, param->count, data);
}

static void lxgUpdateUInt1GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform1uiv(param->gllocation, param->count, data);
}
static void lxgUpdateUInt2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform2uiv(param->gllocation, param->count, data);
}
static void lxgUpdateUInt3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform3uiv(param->gllocation, param->count, data);
}
static void lxgUpdateUInt4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniform4uiv(param->gllocation, param->count, data);
}

static void lxgUpdateMat2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix2fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix3fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix4fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat2x3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix2x3fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat2x4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix2x4fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat3x2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix3x2fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat3x4GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix3x4fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat4x2GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix4x2fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat4x3GLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glUniformMatrix4x3fv(param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static LUX_INLINE void lxgUpdateBufferGLSL(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  if (lxgBuffer_queryUniform(data,ctx,param->gllocation)){
    lxgBuffer_bindIndexed(data,LUXGL_BUFFER_UNIFORM,param->gllocation);
  }
}
static void lxgUpdateFloat1SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform1fv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateFloat2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform2fv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateFloat3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform3fv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateFloat4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform4fv(param->glid, param->gllocation, param->count, data);
}

static void lxgUpdateInt1SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform1iv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateInt2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform2iv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateInt3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform3iv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateInt4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform4iv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateUInt1SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform1uiv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateUInt2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform2uiv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateUInt3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform3uiv(param->glid, param->gllocation, param->count, data);
}
static void lxgUpdateUInt4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniform4uiv(param->glid, param->gllocation, param->count, data);
}

static void lxgUpdateMat2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix2fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix3fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix4fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat2x3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix2x3fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat2x4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix2x4fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat3x2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix3x2fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat3x4SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix3x4fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat4x2SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix4x2fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}
static void lxgUpdateMat4x3SEP(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramUniformMatrix4x3fv(param->glid, param->gllocation, param->count, (GLboolean)param->transpose, data);
}

static void lxgUpdateFloat1NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec++){
    glProgramLocalParameter4fARB(param->gltarget,param->gllocation + i,vec[0],0.0f,0.0f,0.0f);
  }
}
static void lxgUpdateFloat2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec+=2){
    glProgramLocalParameter4fARB(param->gltarget,param->gllocation + i,vec[0],vec[1],0.0f,0.0f);
  }
}
static void lxgUpdateFloat3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec+=3){
    glProgramLocalParameter4fARB(param->gltarget,param->gllocation + i,vec[0],vec[1],vec[2],0.0f);
  }
}
static void lxgUpdateFloat4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,param->count,data);
}

static void lxgUpdateInt1NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  int* vec = (int*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec++){
    glProgramLocalParameterI4iNV(param->gltarget,param->gllocation + i,vec[0],0,0,0);
  }
}
static void lxgUpdateInt2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  int* vec = (int*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec+=2){
    glProgramLocalParameterI4iNV(param->gltarget,param->gllocation + i,vec[0],vec[1],0,0);
  }
}
static void lxgUpdateInt3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  int* vec = (int*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec+=3){
    glProgramLocalParameterI4iNV(param->gltarget,param->gllocation + i,vec[0],vec[1],vec[2],0);
  }
}
static void lxgUpdateInt4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramLocalParametersI4ivNV(param->gltarget,param->gllocation,param->count,data);
}

static void lxgUpdateUInt1NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  uint* vec = (uint*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec++){
    glProgramLocalParameterI4uiNV(param->gltarget,param->gllocation + i,vec[0],0,0,0);
  }
}
static void lxgUpdateUInt2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  uint* vec = (uint*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec+=2){
    glProgramLocalParameterI4uiNV(param->gltarget,param->gllocation + i,vec[0],vec[1],0,0);
  }
}
static void lxgUpdateUInt3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  uint* vec = (uint*)data;
  uint i;
  for (i = 0; i < param->count; ++i, vec+=3){
    glProgramLocalParameterI4uiNV(param->gltarget,param->gllocation + i,vec[0],vec[1],vec[2],0);
  }
}
static void lxgUpdateUInt4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  glProgramLocalParametersI4uivNV(param->gltarget,param->gllocation,param->count,data);
}


static void lxgUpdateMat2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->transpose){
    for (i = 0; i < param->count; ++i, vec+=4, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[2],vec[3],0.0f,0.0f);
    }
  }
  else{
    for (i = 0; i < param->count; ++i, vec+=4, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[2],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[3],0.0f,0.0f);
    }
  }
}
static void lxgUpdateMat3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->transpose){
    for (i = 0; i < param->count; ++i, vec+=9, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],vec[2],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[3],vec[4],vec[5],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[6],vec[7],vec[8],0.0f);
    }
  }
  else{
    for (i = 0; i < param->count; ++i, vec+=9, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[3],vec[6],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[4],vec[7],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[5],vec[8],0.0f);
    }
  }
}
static void lxgUpdateMat4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  if (!param->transpose){
    glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,((int)param->count) * 4,data);
  }
  else{
    uint i;
    GLuint loc = param->gllocation;
    float* vec = (float*) data;
    for (i = 0; i < param->count; ++i, vec+=16, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[4],vec[8],vec[12]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[5],vec[9],vec[13]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[6],vec[10],vec[14]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[3],vec[7],vec[11],vec[15]);
    }
  }
}
static void lxgUpdateMat2x3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->transpose){
    for (i = 0; i < param->count; ++i, vec+=6, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],vec[2],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[3],vec[4],vec[5],0.0f);
    }
  }
  else {
    for (i = 0; i < param->count; ++i, vec+=6, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[2],vec[4],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[3],vec[5],0.0f);
    }
  }
}
static void lxgUpdateMat2x4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  if (!param->transpose){
    glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,((int)param->count) * 2,data);
  }
  else{
    uint i;
    GLuint loc = param->gllocation;
    float* vec = (float*) data;
    for (i = 0; i < param->count; ++i, vec+=8, loc+=2){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[2],vec[4],vec[6]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[3],vec[5],vec[7]);
    }
  }
}
static void lxgUpdateMat3x2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->transpose){
    for (i = 0; i < param->count; ++i, vec+=6, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[2],vec[3],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[4],vec[5],0.0f,0.0f);
    }
  }
  else{
    for (i = 0; i < param->count; ++i, vec+=6, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[3],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[4],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[5],0.0f,0.0f);
    }
  }
}
static void lxgUpdateMat3x4NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  if (!param->transpose){
    glProgramLocalParameters4fvEXT(param->gltarget,param->gllocation,((int)param->count) * 3,data);
  }
  else{
    uint i;
    GLuint loc = param->gllocation;
    float* vec = (float*) data;
    for (i = 0; i < param->count; ++i, vec+=12, loc+=3){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[3],vec[6],vec[9]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[4],vec[7],vec[10]);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[5],vec[8],vec[11]);
    }
  }
}
static void lxgUpdateMat4x2NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->transpose){
    for (i = 0; i < param->count; ++i, vec+=8, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[2],vec[3],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[4],vec[5],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[6],vec[7],0.0f,0.0f);
    }
  }
  else{
    for (i = 0; i < param->count; ++i, vec+=8, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[4],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[5],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[6],0.0f,0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[3],vec[7],0.0f,0.0f);
    }
  }
}
static void lxgUpdateMat4x3NV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  float* vec = (float*)data;
  GLuint loc = param->gllocation;
  uint i;
  if (!param->transpose){
    for (i = 0; i < param->count; ++i, vec+=12, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[1],vec[2],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[3],vec[4],vec[5],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[6],vec[7],vec[8],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[9],vec[10],vec[11],0.0f);
    }
  }
  else{
    for (i = 0; i < param->count; ++i, vec+=12, loc+=4){
      glProgramLocalParameter4fARB(param->gltarget,loc + 0,vec[0],vec[4],vec[8],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 1,vec[1],vec[5],vec[9],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 2,vec[2],vec[6],vec[10],0.0f);
      glProgramLocalParameter4fARB(param->gltarget,loc + 3,vec[3],vec[7],vec[11],0.0f);
    }
  }
}
static LUX_INLINE void lxgUpdateBufferNV(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  GLuint domain = param->gltarget - LUXGL_BUFFER_NVPARAM_TESSCTRL;
  if (lxgBuffer_queryUniform(data,ctx,param->gllocation + domain * LUXGFX_MAX_DOMAIN_BUFFERS)){
    lxgBuffer_bindIndexed(data,param->gltarget,param->gllocation);
  }
}


static void lxgUpdateSampler(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  lxgTexture_checked(data,ctx,param->gllocation);
}
static void lxgUpdateImage(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data){
  lxgTextureImage_checked(data,ctx,param->gllocation);
}

LUX_API void lxgProgramParameter_initDomainNV(lxgProgramParameterPTR param, lxgProgramDomain_t domain)
{
  if (param->gltype == LUXGL_PARAM_BUFFER){
    static lxGLBufferTarget_t types[LUXGFX_MAX_DOMAINS] = {
      LUXGL_BUFFER_NVPARAM_VERTEX,
      LUXGL_BUFFER_NVPARAM_FRAGMENT,
      LUXGL_BUFFER_NVPARAM_GEOMETRY,
      LUXGL_BUFFER_NVPARAM_TESSCTRL,
      LUXGL_BUFFER_NVPARAM_TESSEVAL,
    };

    param->gltarget = types[domain];
  }
  else{
    static lxGLProgramType_t types[LUXGFX_MAX_DOMAINS] = {
      LUXGL_PROGRAM_VERTEX,
      LUXGL_PROGRAM_FRAGMENT,
      LUXGL_PROGRAM_GEOMETRY,
      LUXGL_PROGRAM_TESSCTRL,
      LUXGL_PROGRAM_TESSEVAL,
    };

    param->gltarget = types[domain];
  }
}

LUX_API void lxgProgramParameter_initFuncNV( lxgProgramParameterPTR param )
{
  param->func = NULL;
  switch(param->gltype)
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
    param->func = lxgUpdateSampler; return;

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
    param->func = lxgUpdateImage; return;

  case LUXGL_PARAM_SPECIAL:
    return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}

LUX_API void lxgProgramParameter_initFunc( lxgProgramParameterPTR param )
{
  param->func = NULL;
  switch(param->gltype)
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
    param->func = lxgUpdateSampler; return;

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
    param->func = lxgUpdateImage; return;

  case LUXGL_PARAM_SPECIAL:
    return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}

LUX_API void lxgProgramParameter_initFuncSEP( lxgProgramParameterPTR param , GLuint progid)
{
  param->glid = progid;
  param->func = NULL;
  switch(param->gltype)
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
    param->func = lxgUpdateSampler; return;

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
    param->func = lxgUpdateImage; return;

  case LUXGL_PARAM_SPECIAL:
    return;
  }
  LUX_DEBUGASSERT(0 && "illegal parameter type");
}


LUX_API void lxgProgram_applyParameters( lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, void **data )
{
  uint i;
  LUX_DEBUGASSERT(ctx->program == prog);
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func);
    params[i]->func(params[i],ctx,data[i]);
  }
}

LUX_API void lxgProgram_applyBuffersGLSL(lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxgUpdateBufferGLSL);
    lxgUpdateBufferGLSL(params[i],ctx,data[i]);
  }
}

LUX_API void lxgProgram_applyBuffersNV(lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxgUpdateBufferNV);
    lxgUpdateBufferNV(params[i],ctx,data[i]);
  }
}

LUX_API void lxgProgram_applySamplers( lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgTexturePTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxgUpdateSampler);
    lxgTexture_checked(data[i],ctx,params[i]->gllocation);
  }
}

LUX_API void lxgProgram_applyImages( lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgTextureImagePTR *data )
{
  uint i;
  for (i = 0; i < num; ++i){
    LUX_DEBUGASSERT(params[i]->func == lxgUpdateImage);
    lxgTextureImage_checked(data[i],ctx,params[i]->gllocation);
  }
}

//////////////////////////////////////////////////////////////////////////

LUX_API void lxgProgram_init( lxgProgramPTR prog, lxgContextPTR ctx)
{
  memset(prog,0,sizeof(lxgProgram_t));
  prog->ctx = ctx;
  prog->glid = glCreateProgram();
  prog->type = LUXGFX_PROGRAM_GLSL;
}

LUX_API void lxgProgram_deinit( lxgProgramPTR prog, lxgContextPTR ctx )
{
  glDeleteProgram(prog->glid);
}

LUX_API void lxgProgram_setDomain( lxgProgramPTR prog, lxgProgramDomain_t type, lxgDomainProgramPTR stage )
{
  LUX_DEBUGASSERT(stage->progtype == prog->type);
  prog->programs[type] = stage;
  prog->usedProgs |= 1<<type;
  glAttachShader(prog->glid,stage->glid);
}

LUX_API booln lxgProgram_link( lxgProgramPTR prog )
{
  GLint status;
  GLuint id = prog->glid;
  
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (!status) return LUX_FALSE;

  glValidateProgram(id);
  glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
  if (!status) return LUX_FALSE;

  return LUX_TRUE;
}

LUX_API const char* lxgProgram_log( lxgProgramPTR prog, char* buffer, int len)
{
  GLsizei used;
  glGetProgramInfoLog(prog->glid, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

static LUX_INLINE lxgProgram_stateNV(flags32 flags, flags32 changed, lxgDomainProgramPTR domains[LUXGFX_DOMAINS])
{
  if (changed & (1<<LUXGFX_DOMAIN_VERTEX)){
    if (flags & (1<<LUXGFX_DOMAIN_VERTEX)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_VERTEX_PROGRAM_ARB,domains[LUXGFX_DOMAIN_VERTEX]->glid);
      glEnable(GL_VERTEX_PROGRAM_ARB);
    }
    else{
      glDisable(GL_VERTEX_PROGRAM_ARB);
      glBindProgramARB(GL_VERTEX_PROGRAM_ARB,0);
    }
  }
  if (changed & (1<<LUXGFX_DOMAIN_FRAGMENT)){
    if (flags & (1<<LUXGFX_DOMAIN_FRAGMENT)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,domains[LUXGFX_DOMAIN_FRAGMENT]->glid);
      glEnable(GL_FRAGMENT_PROGRAM_ARB);
    }
    else{
      glDisable(GL_FRAGMENT_PROGRAM_ARB);
      glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,0);
    }
  }
  if (changed & (1<<LUXGFX_DOMAIN_GEOMETRY)){
    if (flags & (1<<LUXGFX_DOMAIN_GEOMETRY)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_GEOMETRY_PROGRAM_NV,domains[LUXGFX_DOMAIN_GEOMETRY]->glid);
      glEnable(GL_GEOMETRY_PROGRAM_NV);
    }
    else{
      glDisable(GL_GEOMETRY_PROGRAM_NV);
      glBindProgramARB(GL_GEOMETRY_PROGRAM_NV,0);
    }
  }
  if (changed & (1<<LUXGFX_DOMAIN_TESSCTRL)){
    if (flags & (1<<LUXGFX_DOMAIN_TESSCTRL)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_TESS_CONTROL_PROGRAM_NV,domains[LUXGFX_DOMAIN_TESSCTRL]->glid);
      glEnable(GL_TESS_CONTROL_PROGRAM_NV);
    }
    else{
      glDisable(GL_TESS_CONTROL_PROGRAM_NV);
      glBindProgramARB(GL_TESS_CONTROL_PROGRAM_NV,0);
    }
  }
  if (changed & (1<<LUXGFX_DOMAIN_TESSEVAL)){
    if (flags & (1<<LUXGFX_DOMAIN_TESSEVAL)){
      LUX_DEBUGASSERT(domains);
      glBindProgramARB(GL_TESS_EVALUATION_PROGRAM_NV,domains[LUXGFX_DOMAIN_TESSEVAL]->glid);
      glEnable(GL_TESS_EVALUATION_PROGRAM_NV);
    }
    else{
      glDisable(GL_TESS_EVALUATION_PROGRAM_NV);
      glBindProgramARB(GL_TESS_EVALUATION_PROGRAM_NV,0);
    }
  }
}

LUX_API void  lxgProgram_apply( lxgProgramPTR prog, lxgContextPTR ctx)
{
  lxgProgramType_t type = prog ? prog->type : LUXGFX_PROGRAM_NONE;
  lxgProgramPTR    oldprog = ctx->program;
  lxgProgramType_t oldtype = oldprog ? oldprog->type : LUXGFX_PROGRAM_NONE;
  if (type != oldtype){
    switch (oldtype){
    case LUXGFX_PROGRAM_GLSL:
      glUseProgram(0); break;
    case LUXGFX_PROGRAM_NV:
      lxgProgram_stateNV(0,oldprog->usedProgs,NULL);
      break;
    }
  }
  else if (type == LUXGFX_PROGRAM_GLSL){
    glUseProgram(prog->glid);
  }
  else{
    lxgProgram_stateNV(prog->usedProgs,oldprog->usedProgs ^ prog->usedProgs, prog->programs);
  }
  
  ctx->program = prog;
}

//////////////////////////////////////////////////////////////////////////

LUX_API void lxgProgram_initNV( lxgProgramPTR prog, lxgContextPTR ctx )
{
  memset(prog,0,sizeof(lxgProgram_t));
  prog->ctx = ctx;
  prog->type = LUXGFX_PROGRAM_NV;
}

LUX_API void lxgProgram_deinitNV( lxgProgramPTR prog, lxgContextPTR ctx )
{

}

LUX_API void lxgProgram_setDomainNV( lxgProgramPTR prog, lxgProgramDomain_t type, lxgDomainProgramPTR stage )
{
  LUX_DEBUGASSERT(stage->progtype == prog->type);
  prog->programs[type] = stage;
  prog->usedProgs |= 1<<type;
}

//////////////////////////////////////////////////////////////////////////

static LUX_INLINE lxGLShaderType_t getDomainType(lxgProgramDomain_t type)
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



LUX_API void lxgDomainProgram_init( lxgDomainProgramPTR stage, lxgContextPTR ctx, lxgProgramDomain_t type)
{
  GLenum gltype = getDomainType(type);
  memset(stage,0,sizeof(lxgDomainProgram_t));

  stage->ctx = ctx;
  stage->glid = glCreateShader(gltype);
  stage->gltype = gltype;
  stage->progtype = LUXGFX_PROGRAM_GLSL;
}

LUX_API void lxgDomainProgram_deinit( lxgDomainProgramPTR stage, lxgContextPTR ctx )
{
  glDeleteShader(stage->glid);
}

LUX_API booln lxgDomainProgram_compile( lxgDomainProgramPTR stage, const char *src, int len )
{
  GLuint id = stage->glid;
  GLint status;
  glShaderSource(id, 1, &src, &len);
  glCompileShader(id);
  glGetShaderiv(id,GL_COMPILE_STATUS,&status);
  return status;
}

LUX_API const char* lxgDomainProgram_error( lxgDomainProgramPTR stage, char *buffer, int len  )
{
  GLsizei used;
  glGetShaderInfoLog(stage->glid, len, &used, buffer);
  return (used > 0) ? buffer : NULL;
}

//////////////////////////////////////////////////////////////////////////

static LUX_INLINE lxGLProgramType_t getDomainTargetNV(lxgProgramDomain_t type)
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

LUX_API void lxgDomainProgram_initNV( lxgDomainProgramPTR stage, lxgContextPTR ctx, lxgProgramDomain_t type)
{
  memset(stage,0,sizeof(lxgDomainProgram_t));

  stage->ctx = ctx;
  glGenProgramsARB(1,&stage->glid);
  stage->gltarget = getDomainTargetNV(type);
  stage->progtype = LUXGFX_PROGRAM_NV;
}

LUX_API void lxgDomainProgram_deinitNV( lxgDomainProgramPTR stage, lxgContextPTR ctx)
{
  glDeleteProgramsARB(1,&stage->glid);
}

LUX_API booln lxgDomainProgram_compileNV(lxgDomainProgramPTR stage, const char *src, int len)
{
  GLint pos;
  glBindProgramARB(stage->gltarget,stage->glid);
  glProgramStringARB(stage->gltarget,GL_PROGRAM_FORMAT_ASCII_ARB,len,src);
  glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB,&pos);
  return (pos == -1);
}
LUX_API const char* lxgDomainProgram_errorNV(lxgDomainProgramPTR stage, char *buffer, int len)
{
  const char* str = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
  GLint pos;
  glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB,&pos);
  if (pos > -1){
    strncpy(buffer,str,len);
    return buffer;
  }
  return NULL;
}


