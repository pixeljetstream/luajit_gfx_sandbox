// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/contmap.h>

LUX_API void lxContMap_init(lxContMapPTR cv, lxMemoryAllocatorPTR allocator, size_t keysize, size_t valsize)
{
  lxContVector_init(&cv->keys,allocator,keysize);
  lxContVector_init(&cv->values,allocator,valsize);
}

LUX_API booln lxContMap_set(lxContMapPTR cv, void *key, void *val)
{
  if (lxContVector_find(&cv->keys,key) != -1) return LUX_FALSE;

  lxContVector_pushBack(&cv->keys,key);
  lxContVector_pushBack(&cv->values,val);

  return LUX_TRUE;
}
LUX_API booln lxContMap_remove(lxContMapPTR cv, void *key)
{
  int idx = lxContVector_find(&cv->keys,key) ;
  if (idx != -1) return LUX_FALSE;

  lxContVector_removeUnsorted(&cv->keys,idx);
  lxContVector_removeUnsorted(&cv->values,idx);

  return LUX_TRUE;
}
LUX_API booln lxContMap_get(lxContMapCPTR cv, void *key, void**outval)
{
  int idx = lxContVector_find(&cv->keys,key);
  if (idx == -1) return LUX_FALSE;

  *outval = lxContVector_at((lxContVectorPTR)&cv->values,idx);
  return LUX_TRUE;
}

LUX_API void  lxContMap_clear(lxContMapPTR cv)
{
  lxContVector_clear(&cv->keys);
  lxContVector_clear(&cv->values);
}
