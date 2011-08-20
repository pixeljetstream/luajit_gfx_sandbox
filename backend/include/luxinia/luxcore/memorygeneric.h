// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_MEMORYGEN_H__
#define __LUXCORE_MEMORYGEN_H__


#include "memorybase.h"

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// MemoryGeneric, wrapper of an 
// generic allocator, mostly for profiling/ debugging/ stat tracking
// zalloc = set to 0

typedef struct lxMemoryGenericInfo_s{
  ptrdiff_t allocs;
  ptrdiff_t mem;
}lxMemoryGenericInfo_t;

typedef struct lxMemoryGenericDescr_s{
  void* sys;
  void* (__cdecl *_malloc)(void* sys, size_t sz);
  void* (__cdecl *_calloc)(void* sys, size_t sz, size_t num);
  void* (__cdecl *_realloc)(void* sys, void* ptr, size_t sz);
  void  (__cdecl *_free)(void* sys, void* ptr);
}lxMemoryGenericDescr_t;

typedef struct lxMemoryGeneric_s* lxMemoryGenericPTR;

LUX_API lxMemoryGenericDescr_t lxMemoryGenericDescr_default();
LUX_API lxMemoryGenericPTR lxMemoryGeneric_new(lxMemoryGenericDescr_t descr);
LUX_API void lxMemoryGeneric_delete(lxMemoryGenericPTR gen);

LUX_API lxMemoryGenericInfo_t lxMemoryGeneric_getInfo(lxMemoryGenericPTR gen);
// returns FALSE on error
LUX_API booln lxMemoryGeneric_deinitStats(lxMemoryGenericPTR gen);
LUX_API void  lxMemoryGeneric_dumpStats(lxMemoryGenericPTR gen, const char* fname);
LUX_API lxMemoryAllocatorPTR lxMemoryGeneric_allocator(lxMemoryGenericPTR gen);

//////////////////////////////////////////////////////////////////////////

LUX_INLINE lxMemoryAllocatorPTR lxMemoryGeneric_allocator(lxMemoryGenericPTR gen)
{
  return (lxMemoryAllocatorPTR)gen;
}

#ifdef __cplusplus
}
#endif

#endif

