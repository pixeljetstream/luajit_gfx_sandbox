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

  typedef struct lxShaderParameter_s{
    lxGLParameterType_t   type;
    lxStrDictKey          namekey;
    lxStrDictName         name;
    uint                  progOffset;
    uint                  progCount;
  }lxShaderParameter_t;

  // for optimal performance group parameters based on the level
  // you typically want to update them

  typedef struct lxShaderProgram_s{
    uint                    numParams;
    lxShaderParameter_t*    params;
    uint                    numProgParams;
    lxgProgramParameter_t*  progParams;
    lxStrDictPTR            dict;
    lxContHashPTR           paramHash;
  }lxShaderProgram_t;

  LUX_API int  lxShaderProgram_getUpdateIndex(lxShaderProgram_t* shader, lxStrDictKey namekey);

    // all arrays must be numParams wide
  typedef struct lxShaderUpdate_s{
    lxShaderProgram_t*      shader;
    uint                    numParams;
    int                     level;

    void**                  buildDatas;
    void**                  levelDatas[LUX_SHADER_UPDATELEVELS];

    int                     dirtyMinMax[2];
    int                     levelMinMax[LUX_SHADER_UPDATELEVELS][2];
    // must be as wide as numProgParams
    lxgProgramParameter_t** progParams;
    void**                  progDatas;
  }lxShaderUpdate_t;

    // data content pointers must be valid until consumption!
  LUX_API void lxShaderUpdate_pushData(lxShaderUpdate_t* update, uint num, int* paramIndices, void** data);
  LUX_API void lxShaderUpdate_popData(lxShaderUpdate_t* update);

    // sets the progParams and progDatas and returns how many are used
    // use with lxgProgram_applyParameters
  LUX_API uint lxShaderUpdate_buildProgramParams(lxShaderUpdate_t* update);

#if defined(__cplusplus)
}
#endif

#endif

