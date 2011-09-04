// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXSCENE_SHADER_H__
#define __LUXSCENE_SHADER_H__

#include <luxinia/luxgfx/program.h>
#include <luxinia/luxcore/contstringmap.h>
#include <luxinia/luxcore/conthash.h>

#ifdef __cplusplus
extern "C"{
#endif

  enum{
    LUX_SHADER_UPDATELEVELS = 4,
    LUX_SHADER_ASSIGNS = 4,
  };

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxShaderParameter_s{
    lxStrDictKey          namekey;
    lxGLParameterType_t   type;
    uint                  progOffset;
    uint                  progCount;
  }lxShaderParameter_t;

  typedef int32 lxShaderIndex;

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

    // returns previous hash
  LUX_API lxContHashPTR  lxShaderProgram_useHash(lxShaderProgram_t* shader,lxContHashPTR hash);
  LUX_API lxShaderIndex  lxShaderProgram_getUpdateIndex(lxShaderProgram_t* shader, lxStrDictKey namekey, lxGLParameterType_t type );

  //////////////////////////////////////////////////////////////////////////
  typedef struct lxShaderAssign_s{
    uint32              shaderID;
    lxShaderIndex*      indices;
  }lxShaderAssign_t;

  typedef struct lxShaderParameterContainer_s{
    uint32                numParams;
    lxStrDictKey*         keys;
    void**                datas;
    lxGLParameterType_t*  types;
  }lxShaderParameterContainer_t;

  typedef struct lxShaderLevel_s{
    uint32              numParams;
    void**              datas;
    lxShaderAssign_t    assigns[LUX_SHADER_ASSIGNS];

    uint32                          numContainers;
    lxShaderParameterContainer_t**  containers;
  }lxShaderLevel_t;
/*
  TODO 
  LUX_API void lxShaderLevel_addContainer();
  LUX_API void lxShaderLevel_remContainer();
  LUX_API void lxShaderLevel_updateContainers();
  LUX_API void lxShaderLevel_buildAssigns();
*/
  //////////////////////////////////////////////////////////////////////////

  typedef struct lxShaderUpdate_s{
    uint                    (*funcBuildProgramParams)( struct lxShaderUpdate_s* update);
    lxShaderProgram_t*      shader;
    uint32                  numParams;
    uint32                  numProgParams;
    int32                   level;

    // must be numParams wide
    void **                 buildDatas;
    void **                 levelDatas[LUX_SHADER_UPDATELEVELS*2];

    int32                   dirtyMinMax[2];
    int32                   levelMinMax[LUX_SHADER_UPDATELEVELS][2];
    // must be numProgParams wide
    lxgProgramParameter_t ** progParams;
    void **                  progDatas;

  }lxShaderUpdate_t;


  LUX_API void    lxShaderUpdate_init(lxShaderUpdate_t* update, lxShaderProgram_t* program);
  LUX_API size_t  lxShaderUpdate_getMemSize(lxShaderUpdate_t* update);
  LUX_API void    lxShaderUpdate_initMem(lxShaderUpdate_t* update, size_t size, void* buffer);

    // data content pointers must be valid until consumption!
  LUX_API void lxShaderUpdate_pushData(lxShaderUpdate_t* update, uint num, lxShaderIndex* paramIndices, void** data);
  LUX_API void lxShaderUpdate_popData(lxShaderUpdate_t* update);

    // sets the progParams and progDatas and returns how many are used
    // use with lxgProgram_applyParameters
  LUX_API uint lxShaderUpdate_buildProgramParams(lxShaderUpdate_t* update);



  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE uint lxShaderUpdate_buildProgramParams(lxShaderUpdate_t* update){
    return update->funcBuildProgramParams(update);
  }

#ifdef __cplusplus
}
#endif

#endif

