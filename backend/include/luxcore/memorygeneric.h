// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

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
  void* (__cdecl *malloc_fn)(void* sys, size_t sz);
  void* (__cdecl *calloc_fn)(void* sys, size_t sz, size_t num);
  void* (__cdecl *realloc_fn)(void* sys, void* ptr, size_t sz);
  void  (__cdecl *free_fn)(void* sys, void* ptr);
}lxMemoryGenericDescr_t;

typedef struct lxMemoryGeneric_s* lxMemoryGenericPTR;

LUX_API lxMemoryGenericDescr_t lxMemoryGenericWrap_default();
LUX_API lxMemoryGenericPTR lxMemoryGeneric_new(lxMemoryGenericDescr_t descr);
LUX_API void lxMemoryGeneric_delete(lxMemoryGenericPTR gen);

LUX_API lxMemoryGenericInfo_t lxMemoryGeneric_getInfo(lxMemoryGenericPTR gen);
// returns FALSE on error
LUX_API booln lxMemoryGeneric_deinitStats(lxMemoryGenericPTR gen);
LUX_API void  lxMemoryGeneric_dumpStats(lxMemoryGenericPTR gen, const char* fname);
LUX_API lxMemoryAllocatorPTR lxMemoryGeneric_allocator(lxMemoryGenericPTR gen);

LUX_API void* lxMemoryGeneric_zalloc(lxMemoryGenericPTR gen, size_t size);
LUX_API void* lxMemoryGeneric_malloc(lxMemoryGenericPTR gen, size_t size);
LUX_API void* lxMemoryGeneric_calloc(lxMemoryGenericPTR gen, size_t num, size_t size);
LUX_API void  lxMemoryGeneric_free(lxMemoryGenericPTR gen, void *ptr, size_t size);
LUX_API void* lxMemoryGeneric_realloc(lxMemoryGenericPTR gen, void* ptr, size_t size, size_t oldsize);
LUX_API void* lxMemoryGeneric_zallocAligned(lxMemoryGenericPTR gen, size_t size, size_t alignsize);
LUX_API void* lxMemoryGeneric_mallocAligned(lxMemoryGenericPTR gen, size_t size, size_t alignsize);
LUX_API void* lxMemoryGeneric_callocAligned(lxMemoryGenericPTR gen, size_t num, size_t size, size_t alignsize);
LUX_API void* lxMemoryGeneric_reallocAligned(lxMemoryGenericPTR gen, void* ptr,size_t size,size_t oldsize,size_t alignsize);
LUX_API void  lxMemoryGeneric_freeAligned(lxMemoryGenericPTR gen, void *ptr, size_t oldsize);
LUX_API size_t  lxMemoryGeneric_offsetAligned(lxMemoryGenericPTR gen, void *ptr);

#if defined(MEMORY_STATS)
LUX_API void* lxMemoryGeneric_zallocStats(lxMemoryGenericPTR gen, size_t size,const char *source,int line);
LUX_API void* lxMemoryGeneric_mallocStats(lxMemoryGenericPTR gen, size_t size,const char *source,int line);
LUX_API void* lxMemoryGeneric_callocStats(lxMemoryGenericPTR gen, size_t num, size_t size,const char *source,int line);
LUX_API void* lxMemoryGeneric_reallocStats(lxMemoryGenericPTR gen, void *ptr, size_t size, size_t oldsize, const char *source, int line);
LUX_API void  lxMemoryGeneric_freeStats(lxMemoryGenericPTR gen, void *ptr, size_t size,const char *source,int line);
LUX_API void* lxMemoryGeneric_mallocAlignedStats(lxMemoryGenericPTR gen, size_t size, size_t alignsize,const char *source,int line);
LUX_API void* lxMemoryGeneric_zallocAlignedStats(lxMemoryGenericPTR gen, size_t size, size_t alignsize,const char *source,int line);
LUX_API void* lxMemoryGeneric_callocAlignedStats(lxMemoryGenericPTR gen, size_t num, size_t size, size_t alignsize,const char *source,int line);
LUX_API void* lxMemoryGeneric_reallocAlignedStats(lxMemoryGenericPTR gen, void* ptr,size_t size,size_t oldsize,size_t alignsize,const char *source,int line);
LUX_API void  lxMemoryGeneric_freeAlignedStats(lxMemoryGenericPTR gen, void *ptr, size_t size,const char *source,int line);
LUX_API size_t  lxMemoryGeneric_offsetAlignedStats(lxMemoryGenericPTR gen, void *ptr);
#endif



LUX_INLINE lxMemoryAllocatorPTR lxMemoryGeneric_allocator(lxMemoryGenericPTR gen)
{
  return (lxMemoryAllocatorPTR)gen;
}

#ifdef __cplusplus
}
#endif

#endif

