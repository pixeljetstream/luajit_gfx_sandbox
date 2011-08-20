// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxscene/shader.h>
#include <luxplatform/debug.h>


//////////////////////////////////////////////////////////////////////////


LUX_API void lxShaderProgram_init( lxShaderProgram_t* shader, lxStrDictPTR dict, lxgProgramPTR program, int numProgParams,lxgProgramParameterPTR* gpuProgParams )
{
  memset(shader,0,sizeof(lxShaderProgram_t));
  shader->program = program;
  shader->dict = dict;
  shader->gpuProgParams = gpuProgParams;
  shader->numProgParams = numProgParams;
}

LUX_API size_t lxShaderProgram_getMemSize( lxShaderProgram_t* shader )
{
  return (sizeof(lxgProgramParameter_t*) * shader->numProgParams) +
         (sizeof(lxShaderParameter_t)    * shader->numParams);
}

LUX_API void lxShaderProgram_initMem( lxShaderProgram_t* shader, size_t size, void* buffer )
{
  byte* mem = (byte*)buffer;

  shader->progParams =  (lxgProgramParameter_t**)mem;
  mem += sizeof(lxgProgramParameter_t*) * shader->numProgParams;

  shader->params =  (lxShaderParameter_t*)mem;
  mem += sizeof(lxShaderParameter_t) * shader->numParams;

  LUX_DEBUGASSERT(mem == ((byte*)buffer)+size);
}


LUX_API uint lxShaderProgram_getParameterCount( lxShaderProgram_t* shader )
{
  uint i;
  uint n;
  uint numGpu = shader->numGpuProgParams;
  uint num = numGpu + shader->numAddProgParams;
  uint unique = 0;

  // compare every name with everyother name
  // count the "last" ones that don't find a match as unique ones
  for (i = 0; i < num; i++){
    lxgProgramParameter_t* pa = i >= numGpu ? shader->addProgParams[i-numGpu] : shader->gpuProgParams[i];
    uint notfound = 1;
    for (n = i+1; n < num; n++){
      lxgProgramParameter_t* pb = n >= numGpu ? shader->addProgParams[n-numGpu] : shader->gpuProgParams[n];
      if (strcmp(pa->name, pb->name) == 0){
        LUX_DEBUGASSERT(pa->type == pb->type);
        notfound = 0;
        break;
      }
    }
    unique += notfound;
  }
  shader->numParams = unique;
  return unique;
}


LUX_API void lxShaderProgram_initParameters( lxShaderProgram_t* shader, lxgProgramParameter_t** customSort)
{
  uint i;
  uint n;
  uint offset = 0;
  uint numGpu = shader->numGpuProgParams;
  uint num = numGpu + shader->numAddProgParams;
  lxShaderParameter_t* LUX_RESTRICT sparam = shader->params;

  // first phase, find unique and add names
  for (i = 0; i < num; i++){
    lxgProgramParameter_t* pa = customSort ? customSort[i] : 
          (i >= numGpu ? shader->addProgParams[i-numGpu] : shader->gpuProgParams[i]);
    uint notfound = 1;

    for (n = i+1; n < num; n++){
      lxgProgramParameter_t* pb = customSort ? customSort[n] : 
          (n >= numGpu ? shader->addProgParams[n-numGpu] : shader->gpuProgParams[n]);
      if (strcmp(pa->name, pb->name) == 0){
        LUX_DEBUGASSERT(pa->type == pb->type);
        notfound = 0;
        break;
      }
    }
    if (notfound){
      sparam->namekey = lxStrDict_getKey(shader->dict,pa->name);
      sparam->type = pa->type;
      sparam->progCount = 0;
      sparam->progOffset = 0;
      sparam++;
    }
  }
  // second phase find compatible
  sparam = shader->params;
  for (i = 0; i < shader->numParams; i++,sparam++){
    sparam->progOffset = offset;
    for (n = 0; n < num; n++){
      lxgProgramParameter_t* pb = customSort ? customSort[n] : 
        (n >= numGpu ? shader->addProgParams[n-numGpu] : shader->gpuProgParams[n]);

      if (strcmp(lxStrDict_getFromKey(shader->dict,sparam->namekey), pb->name) == 0){
        shader->progParams[offset++] = pb;
        break;
      }
    }
    sparam->progCount = offset - sparam->progOffset;
    shader->hasMulti |= sparam->progCount > 1;
  }
}

LUX_API void lxShaderProgram_useHash( lxShaderProgram_t* shader,lxContHashPTR hash )
{
  uint i;
  for (i = 0; i < shader->numParams; i++){
    lxContHash_set(hash, shader->params[i].namekey, (void*)i);
  }

  shader->paramHash = hash;
}


LUX_API lxShaderIndex lxShaderProgram_getUpdateIndex( lxShaderProgram_t* shader, lxStrDictKey namekey, lxGLParameterType_t type )
{
  if (!shader->paramHash){
    uint i;
    for (i = 0; i < shader->numParams; i++){
      if (shader->params[i].namekey == namekey){
        return shader->params[i].type == type ? i : -1;
      }
    }
  }
  else{
    void* data = NULL;
    if (lxContHash_get(shader->paramHash, namekey, &data)){
      uint i = (uint)data;
      return shader->params[i].type == type ? i : -1;
    }
  }

  return -1;
}

//////////////////////////////////////////////////////////////////////////


#define LUX_SHADER_PROPAGATE_SINGLE

LUX_API void lxShaderUpdate_pushData( lxShaderUpdate_t* update, uint num, lxShaderIndex* paramIndices, void** data )
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

static uint LUX_INLINE lxShaderUpdate_appendParamMulti(lxShaderUpdate_t* LUX_RESTRICT update, lxShaderProgram_t* LUX_RESTRICT shader, int paramidx, uint curProgParam, void *data)
{
  lxShaderParameter_t* param = &shader->params[paramidx];
  uint i;
  for (i = 0 ; i < param->progCount; i++){
    update->progParams[curProgParam + i] = shader->progParams[param->progOffset + i];
    update->progDatas [curProgParam + i] = data;
  }
  return curProgParam + param->progCount;
}

static uint lxShaderUpdate_buildProgramParamsMulti( lxShaderUpdate_t* update)
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
      curProgParam = lxShaderUpdate_appendParamMulti(update, shader, i, curProgParam, update->levelDatas[level][i]);
    }
  }
  update->dirtyMinMax[0] = update->numParams;
  update->dirtyMinMax[1] = 0;

  return curProgParam;
}

static uint LUX_INLINE lxShaderUpdate_appendParamSingle(lxShaderUpdate_t* LUX_RESTRICT update, lxShaderProgram_t* LUX_RESTRICT shader, int paramidx, uint curProgParam, void *data)
{
  lxShaderParameter_t* param = &shader->params[paramidx];
  update->progParams[curProgParam] = shader->progParams[param->progOffset];
  update->progDatas [curProgParam] = data;
  return curProgParam + param->progCount;
}

static uint lxShaderUpdate_buildProgramParamsSingle( lxShaderUpdate_t* update)
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
      curProgParam = lxShaderUpdate_appendParamSingle(update, shader, i, curProgParam, update->levelDatas[level][i]);
    }
  }
  update->dirtyMinMax[0] = update->numParams;
  update->dirtyMinMax[1] = 0;

  return curProgParam;
}

LUX_API void lxShaderUpdate_init(lxShaderUpdate_t* update, lxShaderProgram_t* shader)
{
  update->level = 0;
  update->numParams = shader->numParams;
  update->numProgParams = shader->numProgParams;
  update->shader = shader;
  update->funcBuildProgramParams = shader->hasMulti ? lxShaderUpdate_buildProgramParamsMulti : lxShaderUpdate_buildProgramParamsSingle;
}

LUX_API size_t lxShaderUpdate_getMemSize(lxShaderUpdate_t* update)
{
  return (sizeof(void*) + 
          sizeof(void*)*LUX_SHADER_UPDATELEVELS*2) * update->numParams +
         (sizeof(lxgProgramParameter_t*) + 
          sizeof(void*)) * update->numProgParams;
}

LUX_API void lxShaderUpdate_initMem(lxShaderUpdate_t* update, size_t size, void* buffer)
{
  byte* bytes = (byte*)buffer;
  byte* bytesend = bytes + size;
  int i;

  update->buildDatas = (void**)bytes;
  bytes += sizeof(void*) * update->numParams;

  for (i = 0; i < LUX_SHADER_UPDATELEVELS*2; i++){
    update->levelDatas[i] = (void**)bytes;
    bytes += sizeof(void*) * update->numParams;
  }

  update->progParams= (lxgProgramParameter_t**)bytes;
  bytes += sizeof(lxgProgramParameter_t*) * update->numProgParams;
  
  update->progDatas = (void**)bytes;
  bytes += sizeof(void*) * update->numProgParams;

  LUX_DEBUGASSERT(bytes == bytesend);
}

//////////////////////////////////////////////////////////////////////////



