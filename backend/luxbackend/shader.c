// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxbackend/shader.h>
#include <luxplatform/debug.h>


//////////////////////////////////////////////////////////////////////////

LUX_API int lxShaderProgram_getUpdateIndex( lxShaderProgram_t* shader, lxStrDictKey namekey )
{
  if (!shader->paramHash){
    uint i;
    for (i = 0; i < shader->numParams; i++){
      if (shader->params[i].namekey == namekey){
        return i;
      }
    }
  }
  else{
    void* data = NULL;
    if (lxContHash_get(shader->paramHash, namekey, &data)){
      return (int)data;
    }
  }

  return -1;
}

//////////////////////////////////////////////////////////////////////////


#define LUX_SHADER_PROPAGATE_SINGLE

LUX_API void lxShaderUpdate_pushData( lxShaderUpdate_t* update, uint num, int* paramIndices, void** data )
{
  lxShaderProgram_t*  shader   = update->shader;
  int                 level    = ++update->level;
  size_t              banksize = sizeof(void*) * update->numParams;
  uint i;
  int min = update->numParams;
  int max = 0;

  LUX_DEBUGASSERT( 0 <= level && level < LUX_SHADER_UPDATELEVELS);
  LUX_DEBUGASSERT( level == 0 && num == update->numParams && "must provide all parameters at baselevel");

  if ( level == 0 ){
    // TODO optimize, if separable shaders are used, check previous sub-shader programs
    memset(update->buildDatas, 0, banksize);
    update->dirtyMinMax[0] = min;
    update->dirtyMinMax[1] = max;
  }
#ifndef LUX_SHADER_PROPAGATE_SINGLE
  else{
    memcpy(update->levelDatas[level], update->levelDatas[level-1], banksize);
  }
#endif

  for (i = 0; i < num; i++){
    int index = paramIndices[i];
    LUX_DEBUGASSERT(index >= 0 && "invalid index, must not be < 0");

    min = LUX_MIN(min,index);
    max = LUX_MAX(max,index);
    
#ifdef LUX_SHADER_PROPAGATE_SINGLE
    {
      int n;
#if 0
      for (n = level; n < LUX_SHADER_UPDATELEVELS; n++){
        update->levelDatas[n][index] = data[i];
      }
#else
      // avoids branching
      for (n = 0; n < LUX_SHADER_UPDATELEVELS; n++){
        update->levelDatas[level+n][index] = data[i];
      }
#endif
    }
#else
    update->levelDatas[level][index] = data[i];
#endif
  }
  update->level = level;
  update->levelMinMax[level][0] = min;
  update->levelMinMax[level][1] = max;
  update->dirtyMinMax[0] = LUX_MIN(update->dirtyMinMax[0], min);
  update->dirtyMinMax[1] = LUX_MAX(update->dirtyMinMax[1], max);
}


LUX_API void lxShaderUpdate_popData( lxShaderUpdate_t* update)
{
  // when we step back from stack, the parameters used in current level
  // must be reset

  int level = update->level;
  LUX_DEBUGASSERT(level >= 0);
  update->dirtyMinMax[0] = LUX_MIN(update->dirtyMinMax[0], update->levelMinMax[level][0]);
  update->dirtyMinMax[1] = LUX_MAX(update->dirtyMinMax[1], update->levelMinMax[level][1]);
  update->level--;
}

static uint LUX_INLINE lxShaderUpdate_appendParam(lxShaderUpdate_t* LUX_RESTRICT update, lxShaderProgram_t* LUX_RESTRICT shader, int paramidx, uint curProgParam, void *data)
{
  lxShaderParameter_t* param = &shader->params[paramidx];
  uint i;
  for (i = 0 ; i < param->progCount; i++){
    update->progParams[curProgParam + i] = &shader->progParams[param->progOffset + i];
    update->progDatas [curProgParam + i] =  data;
  }
  return curProgParam + param->progCount;
}

LUX_API uint lxShaderUpdate_buildProgramParams( lxShaderUpdate_t* update)
{
  lxShaderProgram_t*  shader = update->shader;
  int  level = update->level;
  uint curProgParam = 0;
  int i;
  
  int min = update->dirtyMinMax[0];
  int max = update->dirtyMinMax[1];

  LUX_DEBUGASSERT( 0 <= level && level < LUX_SHADER_UPDATELEVELS);

  for (i = min; i <= max; i++){
    if (update->buildDatas[i] != update->levelDatas[level][i]){
      update->buildDatas[i] = update->levelDatas[level][i];
      curProgParam = lxShaderUpdate_appendParam(update, shader, i, curProgParam, update->levelDatas[level][i]);
    }
  }
  update->dirtyMinMax[0] = update->numParams;
  update->dirtyMinMax[1] = 0;

  return curProgParam;
}




