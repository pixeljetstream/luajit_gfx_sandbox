// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_CONTMAP_H__
#define __LUXCORE_CONTMAP_H__

#include <luxinia/luxcore/contvector.h>

#ifdef __cplusplus
extern "C"{
#endif

  //////////////////////////////////////////////////////////////////////////
  // ContMap
  //  a simple map, that uses memcmp to for comparing keys
  //  Based on vector, therefore removal can be quite costly

  typedef struct lxContMap_s* lxContMapPTR;
  typedef const struct lxContMap_s* lxContMapCPTR;

  typedef struct lxContMap_s{
    lxContVector_t keys;
    lxContVector_t values;
  }lxContMap_t;

  LUX_API void  lxContMap_init(lxContMapPTR cv, lxMemoryAllocatorPTR allocator, size_t keysize, size_t valsize);
  
  LUX_API booln lxContMap_set(lxContMapPTR cv, void *key, void *val);
  LUX_API booln lxContMap_get(lxContMapCPTR cv, void *key, void **outval);
  LUX_API booln lxContMap_remove(lxContMapPTR cv, void *key);

    // frees memory
  LUX_API void  lxContMap_clear(lxContMapPTR cv);
  
#ifdef __cplusplus
}
#endif


#endif
