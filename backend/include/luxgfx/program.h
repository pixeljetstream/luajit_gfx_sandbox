// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_PROGRAM_H__
#define __LUXLUXGFX_PROGRAM_H__

#include <luxplatform/luxplatform.h>

#include "glconfig.h"

#ifdef __cplusplus
extern "C"{
#endif

  typedef enum lxGFXShaderType_e{
    LUXGFX_SHADER_VERTEX,
    LUXGFX_SHADER_FRAGMENT,
    LUXGFX_SHADER_GEOMETRY,
    LUXGFX_SHADER_TESSCTRL,
    LUXGFX_SHADER_TESSEVAL,
    LUXGFX_SHADERS,
  }lxGFXShaderType_t;

  typedef void (*lxGFXParmeterUpdate_fn)(lxGFXContextPTR ctx, lxGFXProgramParameterPTR param, void* data);

  typedef struct lxGFXProgramParameter_s{
    lxGLParameter_t         vgl;
    lxGFXParmeterUpdate_fn  func;
    ushort                  count;
    ushort                  transpose;
    uint                    size;
    const char*             name;
  }lxGFXProgramParameter_t;

  typedef struct lxGFXProgramData_s{
    uint                        numParams;
    lxGFXProgramParameterPTR    parameters;
    uint                        numSampler;
    lxGFXProgramParameterPTR    samplers;
    uint                        numBuffers;
    lxGFXProgramParameterPTR    buffer;
  }lxGFXProgramData_t;

  typedef struct lxGFXDomainProgram_s{
    lxGLShader_t          vgl;
    lxGFXProgramDataPTR   data;
    lxGFXContextPTR       ctx;
  }lxGFXDomainProgram_t;

  typedef struct lxGFXProgram_s{
    lxGLProgram_t         vgl;
    flags32               usedProgs;
    lxGFXDomainProgramPTR  programs[LUXGFX_SHADERS];
    lxGFXProgramDataPTR   data;
    lxGFXContextPTR       ctx;
  }lxGFXProgram_t;


  LUX_API void lxGFXProgramParameter_initFunc(lxGFXProgramParameterPTR param);

  LUX_API void lxGFXDomainProgram_init(lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage, lxGFXShaderType_t type);
  LUX_API void lxGFXDomainProgram_deinit(lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage);
  LUX_API booln lxGFXDomainProgram_compile(lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage, const char *src, int len);
  LUX_API const char* lxGFXDomainProgram_error(lxGFXContextPTR ctx, lxGFXDomainProgramPTR stage, char *buffer, int len);

  LUX_API void  lxGFXProgram_init(lxGFXContextPTR ctx, lxGFXProgramPTR prog);
  LUX_API void  lxGFXProgram_deinit(lxGFXContextPTR ctx, lxGFXProgramPTR prog);
  LUX_API void  lxGFXProgram_setStage(lxGFXContextPTR ctx, lxGFXProgramPTR prog, lxGFXShaderType_t type, lxGFXDomainProgramPTR stage);
  LUX_API booln lxGFXProgram_link(lxGFXContextPTR ctx, lxGFXProgramPTR prog);
  LUX_API const char* lxGFXProgram_log( lxGFXContextPTR ctx, lxGFXProgramPTR prog, char* buffer, int len);

  LUX_API void lxGFXProgram_updateParameters(lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, void **data);
  LUX_API void lxGFXProgram_updateSamplers(lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, lxGFXTexturePTR *data);
  LUX_API void lxGFXProgram_updateBuffers(lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, lxGFXBufferPTR *data);
  LUX_API void lxGFXProgram_updateImages( lxGFXContextPTR ctx, lxGFXProgramPTR prog, uint num, lxGFXProgramParameterPTR *params, lxGFXTextureImagePTR *data );

#ifdef __cplusplus
}


#endif



#endif