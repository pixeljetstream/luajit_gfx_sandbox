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

  typedef enum lxgProgramType_e{
    LUXGFX_PROGRAM_GLSL,
    LUXGFX_PROGRAM_ARBNV,
  }lxgProgramType_t;

  typedef enum lxgShaderDomain_e{
    LUXGFX_DOMAIN_VERTEX,
    LUXGFX_DOMAIN_FRAGMENT,
    LUXGFX_DOMAIN_GEOMETRY,
    LUXGFX_DOMAIN_TESSCTRL,
    LUXGFX_DOMAIN_TESSEVAL,
    LUXGFX_SHADERS,
  }lxgProgramDomain_t;

  typedef void (*lxgParmeterUpdate_fn)(lxgContextPTR ctx, lxgProgramParameterPTR param, void* data);

  typedef struct lxgProgramParameter_s{
    lxGLParameter_t         vgl;
    lxgParmeterUpdate_fn    func;
    ushort                  count;
    bool16                  transpose;
    uint                    size;
    const char*             name;
  }lxgProgramParameter_t;

  typedef struct lxgProgramData_s{
    uint                      numParams;
    lxgProgramParameterPTR    parameters;
    uint                      numSampler;
    lxgProgramParameterPTR    samplers;
    uint                      numBuffers;
    lxgProgramParameterPTR    buffer;
    uint                      numImages;
    lxgProgramParameterPTR    images;
  }lxgProgramData_t;

  typedef struct lxgDomainProgram_s{
    lxGLDomainProgram_t   vgl;
    lxgProgramDataPTR     data;
    lxgContextPTR         ctx;
  }lxgDomainProgram_t;

  typedef struct lxgProgram_s{
    lxGLProgram_t         vgl;
    lxgProgramType_t      type;
    flags32               usedProgs;
    lxgDomainProgramPTR   programs[LUXGFX_SHADERS];
    lxgProgramDataPTR     data;
    lxgContextPTR         ctx;
  }lxgProgram_t;

  // GLSL
  LUX_API void lxgProgramParameter_initFunc(lxgProgramParameterPTR param);

  LUX_API void lxgDomainProgram_init(lxgContextPTR ctx, lxgDomainProgramPTR stage, lxgProgramDomain_t type);
  LUX_API void lxgDomainProgram_deinit(lxgContextPTR ctx, lxgDomainProgramPTR stage);
  LUX_API booln lxgDomainProgram_compile(lxgContextPTR ctx, lxgDomainProgramPTR stage, const char *src, int len);
  LUX_API const char* lxgDomainProgram_error(lxgContextPTR ctx, lxgDomainProgramPTR stage, char *buffer, int len);

  LUX_API void  lxgProgram_init(lxgContextPTR ctx, lxgProgramPTR prog);
  LUX_API void  lxgProgram_deinit(lxgContextPTR ctx, lxgProgramPTR prog);
  LUX_API void  lxgProgram_setDomain(lxgContextPTR ctx, lxgProgramPTR prog, lxgProgramDomain_t type, lxgDomainProgramPTR stage);
  LUX_API booln lxgProgram_link(lxgContextPTR ctx, lxgProgramPTR prog);
  LUX_API const char* lxgProgram_log( lxgContextPTR ctx, lxgProgramPTR prog, char* buffer, int len);

  // COMMON
  LUX_API void lxgProgram_updateParameters(lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, void **data);
  LUX_API void lxgProgram_updateSamplers(lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgTexturePTR *data);
  LUX_API void lxgProgram_updateBuffers(lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data);
  LUX_API void lxgProgram_updateImages( lxgContextPTR ctx, lxgProgramPTR prog, uint num, lxgProgramParameterPTR *params, lxgTextureImagePTR *data );

  // NV/ARB PROGRAM
  LUX_API void lxgProgramParameter_initDomainNV(lxgProgramParameterPTR param, lxgProgramDomain_t domain);
  LUX_API void lxgProgramParameter_initFuncNV(lxgProgramParameterPTR param);


#ifdef __cplusplus
}


#endif



#endif