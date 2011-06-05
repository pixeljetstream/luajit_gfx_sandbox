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
    LUXGFX_PROGRAM_NONE,
    LUXGFX_PROGRAM_GLSL,
    LUXGFX_PROGRAM_NV,
  }lxgProgramType_t;

  typedef enum lxgShaderDomain_e{
    LUXGFX_DOMAIN_VERTEX,
    LUXGFX_DOMAIN_FRAGMENT,
    LUXGFX_DOMAIN_GEOMETRY,
    LUXGFX_DOMAIN_TESSCTRL,
    LUXGFX_DOMAIN_TESSEVAL,
    LUXGFX_DOMAINS,
  }lxgProgramDomain_t;

  typedef void (*lxgParmeterUpdate_fn)(lxgProgramParameterPTR param, lxgContextPTR ctx, void* data);

  typedef struct lxgProgramParameter_s{
    lxGLParameterType_t   gltype;
    lxgParmeterUpdate_fn  func;
    union{
      GLuint              glid;
      GLenum              gltarget;
    };
    GLuint                gllocation;
    ushort                count;
    bool16                transpose;
    const char*           name;  // only to aid debugging
  }lxgProgramParameter_t;

  typedef struct lxgDomainProgram_s{
    union{
      lxGLShaderType_t    gltype;
      lxGLProgramType_t   gltarget;
    };
    GLuint                glid;
    lxgContextPTR         ctx;
    lxgProgramType_t      progtype;
  }lxgDomainProgram_t;

  typedef struct lxgProgram_s{
    GLuint                glid;
    lxgProgramType_t      type;
    flags32               usedProgs;
    lxgDomainProgramPTR   programs[LUXGFX_DOMAINS];
    lxgContextPTR         ctx;
  }lxgProgram_t;


  // COMMON
  LUX_API void lxgProgram_apply( lxgProgramPTR prog, lxgContextPTR ctx);
  LUX_API void lxgProgram_applyParameters(lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, void **data);
  LUX_API void lxgProgram_applySamplers( lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgTexturePTR *data);
  LUX_API void lxgProgram_applyBuffers(lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data);
  LUX_API void lxgProgram_applyImages( lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgTextureImagePTR *data );

  // GLSL
  LUX_API void lxgProgramParameter_initFunc(lxgProgramParameterPTR param);

  LUX_API void lxgDomainProgram_init(lxgDomainProgramPTR stage, lxgContextPTR ctx, lxgProgramDomain_t type);
  LUX_API void lxgDomainProgram_deinit(lxgDomainProgramPTR stage, lxgContextPTR ctx);
  LUX_API booln lxgDomainProgram_compile(lxgDomainProgramPTR stage, const char *src, int len);
  LUX_API const char* lxgDomainProgram_error(lxgDomainProgramPTR stage, char *buffer, int len);

  LUX_API void  lxgProgram_init(lxgProgramPTR prog, lxgContextPTR ctx);
  LUX_API void  lxgProgram_deinit(lxgProgramPTR prog, lxgContextPTR ctx);
  LUX_API void  lxgProgram_setDomain(lxgProgramPTR prog, lxgProgramDomain_t type, lxgDomainProgramPTR stage);
  LUX_API booln lxgProgram_link(lxgProgramPTR prog);
  LUX_API const char* lxgProgram_log(lxgProgramPTR prog, char* buffer, int len);

  LUX_API void lxgProgram_applyBuffersGLSL(lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data);

  // GLSL SEPERATE or DSA
  LUX_API void lxgProgramParameter_initFuncSEP(lxgProgramParameterPTR param, GLuint progid);

  // NV/ARB PROGRAM
  LUX_API void lxgProgramParameter_initDomainNV(lxgProgramParameterPTR param, lxgProgramDomain_t domain);
  LUX_API void lxgProgramParameter_initFuncNV(lxgProgramParameterPTR param);

  LUX_API void lxgDomainProgram_initNV(lxgDomainProgramPTR stage, lxgContextPTR ctx, lxgProgramDomain_t type);
  LUX_API void lxgDomainProgram_deinitNV(lxgDomainProgramPTR stage, lxgContextPTR ctx);
  LUX_API booln lxgDomainProgram_compileNV(lxgDomainProgramPTR stage, const char *src, int len);
  LUX_API const char* lxgDomainProgram_errorNV(lxgDomainProgramPTR stage, char *buffer, int len);

  LUX_API void  lxgProgram_initNV(lxgProgramPTR prog, lxgContextPTR ctx);
  LUX_API void  lxgProgram_deinitNV(lxgProgramPTR prog, lxgContextPTR ctx);
  LUX_API void  lxgProgram_setDomainNV(lxgProgramPTR prog, lxgProgramDomain_t type, lxgDomainProgramPTR stage);

  LUX_API void lxgProgram_applyBuffersNV(lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data);

  //////////////////////////////////////////////////////////////////////////


  LUX_INLINE void lxgProgram_applyBuffers(lxgProgramPTR prog, lxgContextPTR ctx, uint num, lxgProgramParameterPTR *params, lxgBufferPTR *data )
  {
    if (prog->type != LUXGFX_PROGRAM_NV){
      lxgProgram_applyBuffersGLSL(prog,ctx,num,params,data);
    }
    else{
      lxgProgram_applyBuffersNV(prog,ctx,num,params,data);
    }
  }

#ifdef __cplusplus
}


#endif



#endif