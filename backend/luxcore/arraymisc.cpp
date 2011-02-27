// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxcore/arraymisc.h>

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
