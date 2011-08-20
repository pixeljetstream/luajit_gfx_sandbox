// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/arraymisc.h>
#include <luxinia/luxcore/contbitarray.h>

template <class T>
static LUX_INLINE int lxArrayFindOrAddT(T *data, int *inoutCnt, T value, int maxCnt)
{
  int i = 0;
  int cnt = *inoutCnt;
  while (i < cnt){
    if (data[i] == value)
      return i;
    i++;
  }
  if (cnt < maxCnt){
    data[i] = value;
    inoutCnt[0]++;
    return i;
  }
  return -1;
}

LUX_API int lxArrayFindOrAdd8(uint8 *data, int *inoutCnt, uint8 value, int maxCnt)
{
  return lxArrayFindOrAddT<uint8>(data,inoutCnt,value,maxCnt);
}
LUX_API int lxArrayFindOrAdd16(uint16 *data, int *inoutCnt, uint16 value, int maxCnt)
{
  return lxArrayFindOrAddT<uint16>(data,inoutCnt,value,maxCnt);
}
LUX_API int lxArrayFindOrAdd32(uint32 *data, int *inoutCnt, uint32 value, int maxCnt)
{
  return lxArrayFindOrAddT<uint32>(data,inoutCnt,value,maxCnt);
}
LUX_API int lxArrayFindOrAddFloat(float *data, int *inoutCnt, float value, int maxCnt)
{
  return lxArrayFindOrAddT<float32>(data,inoutCnt,value,maxCnt);
}
LUX_API int lxArrayFindOrAddPtr(void **data, int *inoutCnt, void* value, int maxCnt)
{
  return lxArrayFindOrAddT<void*>(data,inoutCnt,value,maxCnt);
}

// index of first item with state
LUX_API int32  lxBitArray_getFirst(const lxBitArray_t *ba, booln state)
{
  const uint32 *start = ba->bits;
  const uint32 *end = start + ba->num32;
  uint32 cur;

  if (state){
    while (start < end){
      if ((cur = *start)){
        int i;
        for (i=0; i < 32; i++){
          if (cur & (1<<i)){
            return (start-ba->bits)*32 + i;
          }
        }
      }
      start++;
    }
  }
  else{
    while (start < end){
      if (!(cur = *start)){
        int i;
        for (i=0; i < 32; i++){
          if (!(cur & (1<<i))){
            return (start-ba->bits)*32 + i;
          }
        }
      }
      start++;
    }
  }

  return -1;
}

