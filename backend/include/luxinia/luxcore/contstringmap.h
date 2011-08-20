// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_STRINGMAP_H__
#define __LUXCORE_STRINGMAP_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <luxinia/luxcore/memorybase.h>

#ifdef __cplusplus
extern "C"{
#endif

  //////////////////////////////////////////////////////////////////////////
  // CharStrDict
  //  uses a hash to manage a reference counted map of strings
  //  MemoryList used for allocations, ideally strings are <= 256
  
  typedef struct lxStrDict_s* lxStrDictPTR;
  typedef const struct lxStrDict_s* lxStrDictCPTR;
  typedef const char* lxStrDictName;
  typedef uint32      lxStrDictKey;

  LUX_API lxStrDictPTR  lxStrDict_new(lxMemoryAllocatorPTR allocator, uint numBins);
  LUX_API void          lxStrDict_delete(lxStrDictPTR dc);

  // returns hashkey
  LUX_API lxStrDictKey  lxStrDict_add(lxStrDictPTR dc, const char *str);
  LUX_API lxStrDictName lxStrDict_getFromKey(lxStrDictCPTR dc, lxStrDictKey key);
  LUX_API lxStrDictKey  lxStrDict_getKey(lxStrDictCPTR dc, const char *str);

  // returns current reference count
  LUX_API int     lxStrDict_rem(lxStrDictPTR dc, lxStrDictName str);
  LUX_API int     lxStrDict_remKey(lxStrDictPTR dc, lxStrDictKey key);


  ////////////////////////////////////////////////////////////////////////////////
  // CharStrMap
  //  associative char*->void* array
  //  data is put in a hash using the same keys the dictionary creates
  //  can use a shared dictionary, or creates its own


  typedef struct lxStrMap_s* lxStrMapPTR;

  typedef void* (lxStrMap_Iterator_fn)(void* fnData,const char *,void *browseData);

  LUX_API lxStrMapPTR  lxStrMap_new(lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize, lxStrDictPTR dc);
  LUX_API void  lxStrMap_delete(lxStrMapPTR   self, void (*fnFree)(void*));

  // returns TRUE when key was overwritten
  LUX_API booln lxStrMap_set(lxStrMapPTR  self, const char *key, void *value);
  LUX_API void* lxStrMap_get(lxStrMapPTR  self, const char *key);
  LUX_API void* lxStrMap_getNth(lxStrMapPTR self, uint n);
  LUX_API uint  lxStrMap_getCount(lxStrMapPTR self);

  // returns 1 on success
  LUX_API booln lxStrMap_remove(lxStrMapPTR self, const char *key);
  LUX_API booln lxStrMap_isSet(lxStrMapPTR  self, const char *key);


  // iterator returns fnData for next call
  LUX_API void  lxStrMap_iterate(lxStrMapPTR  self, lxStrMap_Iterator_fn *itfunc, void *fnData);

#ifdef __cplusplus
}
#endif

#endif

