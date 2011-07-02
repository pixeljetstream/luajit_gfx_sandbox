// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXBACKEND_SHADER_H__
#define __LUXBACKEND_SHADER_H__

#include <luxgfx/program.h>
#include <luxcore/contstringmap.h>
#include <luxcore/conthash.h>

#if defined(__cplusplus)
extern "C"
{
#endif

  enum{
    LUX_SHADER_UPDATELEVELS = 4,
  };

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxShaderParameter_s{
    lxStrDictKey          namekey;
    lxGLParameterType_t    type;
    uint                  progOffset;
    uint                  progCount;
  }lxShaderParameter_t;

  // for optimal performance group parameters based on the level
  // you typically want to update them

  typedef struct lxShaderProgram_s{
    lxgProgramPTR           program;
    booln                   hasMulti;
    uint                    numParams;
    lxShaderParameter_t*    params;
    uint                    numProgParams;
    lxgProgramParameter_t** progParams;
    lxStrDictPTR            dict;
    lxContHashPTR           paramHash;
    uint                    numGpuProgParams;
    lxgProgramParameter_t** gpuProgParams;
    uint                    numAddProgParams;
    lxgProgramParameter_t** addProgParams;
  }lxShaderProgram_t;

  LUX_API void    lxShaderProgram_init(lxShaderProgram_t* shader, lxStrDictPTR dict, lxgProgramPTR program, int numProgParams,lxgProgramParameterPTR* gpuProgParams);
  LUX_API uint    lxShaderProgram_getParameterCount(lxShaderProgram_t* shader);
  LUX_API size_t  lxShaderProgram_getMemSize(lxShaderProgram_t* shader);
  LUX_API void    lxShaderProgram_initMem(lxShaderProgram_t* shader, size_t size, void* buffer);
  LUX_API void    lxShaderProgram_initParameters(lxShaderProgram_t* shader, lxgProgramParameter_t** optionalSort);
  LUX_API void    lxShaderProgram_useHash(lxShaderProgram_t* shader,lxContHashPTR hash);
  LUX_API int     lxShaderProgram_getUpdateIndex(lxShaderProgram_t* shader, lxStrDictKey namekey, lxGLParameterType_t type );

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxShaderUpdate_s{
    uint                    (*funcBuildProgramParams)( struct lxShaderUpdate_s* update);
    lxShaderProgram_t*      shader;
    uint                    numParams;
    uint                    numProgParams;
    int                     level;

    // must be numParams wide
    void **                 buildDatas;
    void **                 levelDatas[LUX_SHADER_UPDATELEVELS*2];

    int                     dirtyMinMax[2];
    int                     levelMinMax[LUX_SHADER_UPDATELEVELS][2];
    // must be numProgParams wide
    lxgProgramParameter_t ** progParams;
    void **                  progDatas;

  }lxShaderUpdate_t;


  LUX_API void    lxShaderUpdate_init(lxShaderUpdate_t* update, lxShaderProgram_t* program);
  LUX_API size_t  lxShaderUpdate_getMemSize(lxShaderUpdate_t* update);
  LUX_API void    lxShaderUpdate_initMem(lxShaderUpdate_t* update, size_t size, void* buffer);

    // data content pointers must be valid until consumption!
  LUX_API void lxShaderUpdate_pushData(lxShaderUpdate_t* update, uint num, int* paramIndices, void** data);
  LUX_API void lxShaderUpdate_popData(lxShaderUpdate_t* update);

    // sets the progParams and progDatas and returns how many are used
    // use with lxgProgram_applyParameters
  LUX_API uint lxShaderUpdate_buildProgramParams(lxShaderUpdate_t* update);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE uint lxShaderUpdate_buildProgramParams(lxShaderUpdate_t* update){
    return update->funcBuildProgramParams(update);
  }

#if defined(__cplusplus)
}
#endif

#endif

