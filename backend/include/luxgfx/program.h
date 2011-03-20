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

  typedef enum lxgShaderType_e{
    LUXGFX_SHADER_VERTEX,
    LUXGFX_SHADER_FRAGMENT,
    LUXGFX_SHADER_GEOMETRY,
    LUXGFX_SHADER_TESSCTRL,
    LUXGFX_SHADER_TESSEVAL,
    LUXGFX_SHADERS,
  }lxgShaderType_t;

  typedef void (*lxgParmeterUpdate_fn)(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data);

  typedef struct lxgProgramParameter_s{
    lxGLParameter_t         vgl;
    lxgParmeterUpdate_fn  func;
    ushort                  count;
    ushort                  transpose;
    uint                    size;
    const char*             name;
  }lxgProgramParameter_t;

  typedef struct lxgProgramData_s{
    uint                        numParams;
    lxgProgramParameterPTR    parameters;
    uint                        numSampler;
    lxgProgramParameterPTR    samplers;
    uint                        numBuffers;
    lxgProgramParameterPTR    buffer;
  }lxgProgramData_t;

  typedef struct lxgDomainProgram_s{
    lxGLShader_t          vgl;
    lxgProgramDataPTR   data;
    lxgContextPTR       ctx;
  }lxgDomainProgram_t;

  typedef struct lxgProgram_s{
    lxGLProgram_t         vgl;
    flags32               usedProgs;
    lxgDomainProgramPTR  programs[LUXGFX_SHADERS];
    lxgProgramDataPTR   data;
    lxgContextPTR       ctx;
  }lxgProgram_t;


  LUX_API void lxgProgramParameter_initFunc(lxgProgramParameterPTR param);

  LUX_API void lxgDomainProgram_init(lxgContextPTR ctx, lxgDomainProgramPTR stage, lxgShaderType_t type);
  LUX_API void lxgDomainProgram_deinit(lxgContextPTR ctx, lxgDomainProgramPTR stage);
  LUX_API booln lxgDomainProgram_compile(lxgContextPTR ctx, lxgDomainProgramPTR stage, const char *src, int len);
  LUX_API const char* lxgDomainProgram_error(lxgContextPTR ctx, lxgDomainProgramPTR stage, char *buffer, int len);

  LUX_API void  lxgProgram_init(lxgContextPTR ctx, lxgProgramPTR prog);
  LUX_API void  lxgProgram_deinit(lxgContextPTR ctx, lxgProgramPTR prog);
  LUX_API void  lxgProgram_setStage(lxgContextPTR ctx, lxgProgramPTR prog, lxgShaderType_t type, lxgDomainProgramPTR stage);
  LUX_API booln lxgProgram_link(lxgContextPTR ctx, lxgProgramPTR prog);
  LUX_API const char* lxgProgram_log( lxgContextPTR ctx, lxgProgramPTR prog, char* buffer, int len);
  LUX_API void  lxgProgram_feedback(lxgContextPTR ctx, lxgProgramPTR prog);

  LUX_API void lxgProgram_updateParameters(lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, void **data);
  LUX_API void lxgProgram_updateSamplers(lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgTexturePTR *data);
  LUX_API void lxgProgram_updateBuffers(lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data);
  LUX_API void lxgProgram_updateImages( lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgTextureImagePTR *data );

#ifdef __cplusplus
}


#endif



#endif