// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/memorygeneric.h>
#include <luxinia/luxplatform/debug.h>
#include <luxinia/luxcore/contmacrolinkedlist.h>

#include <malloc.h>
#include <memory.h>

//////////////////////////////////////////////////////////////////////////
// MemoryGeneric

typedef struct MemInfo_s
{
  size_t    size;
  int       line;
  const char* src;

  struct MemInfo_s LUX_LISTNODEVARS;
} MemInfo_t;

typedef struct MemStats_s{
  MemInfo_t *List;
}MemStats_t;


typedef struct lxMemoryGeneric_s{
  lxMemoryAllocator_t     allocator;
  lxMemoryGenericDescr_t  descr;
  lxMemoryGenericInfo_t   info;
  lxMemoryTracker_t       tracker;
  MemStats_t              stats;
}lxMemoryGeneric_t;


LUX_API lxMemoryGenericInfo_t lxMemoryGeneric_getInfo(lxMemoryGenericPTR gen)
{
  return gen->info;
}


//////////////////////////////////////////////////////////////////////////

static void lxMemoryGeneric_free(lxMemoryGenericPTR gen, void *ptr, size_t size) {
  gen->info.allocs--;
  gen->info.mem -= size;

  gen->descr._free(gen->descr.sys,ptr);
}

static void *lxMemoryGeneric_calloc(lxMemoryGenericPTR gen, size_t num, size_t size)
{
  gen->info.allocs++;
  gen->info.mem += size;
  {
    void *ptr = gen->descr._calloc(gen->descr.sys,size,num);

    return ptr;
  }
}

static void *lxMemoryGeneric_malloc(lxMemoryGenericPTR gen, size_t size)
{
  gen->info.allocs++;
  gen->info.mem += size;
  {
    void *ptr = gen->descr._malloc(gen->descr.sys,size);

    return ptr;
  }
}

static void *lxMemoryGeneric_realloc(lxMemoryGenericPTR gen, void* ptr, size_t size, size_t oldsize)
{
  gen->info.mem += size-oldsize;

  ptr = gen->descr._realloc(gen->descr.sys,ptr,size);
  return ptr;
}

static void lxMemoryGeneric_freeAligned(lxMemoryGenericPTR gen, void *ptr, size_t size) {
  gen->info.allocs--;

  ptr = lxMemAlignedToOrig(ptr);
  gen->descr._free(gen->descr.sys,ptr);
}

static void *lxMemoryGeneric_zallocAligned(lxMemoryGenericPTR gen, size_t size, size_t alignsize)
{
  void *ptr = gen->descr._malloc(gen->descr.sys,size + alignsize + sizeof(size_t));

  ptr = lxMemAlignedFromOrig(ptr,alignsize);
  memset(ptr,0,size);

  gen->info.allocs++;
  gen->info.mem += size;

  return ptr;
}

static void *lxMemoryGeneric_mallocAligned(lxMemoryGenericPTR gen, size_t size, size_t alignsize)
{
  void *ptr = gen->descr._malloc(gen->descr.sys,size + alignsize + sizeof(size_t));

  ptr = lxMemAlignedFromOrig(ptr,alignsize);

  gen->info.allocs++;
  gen->info.mem += size;

  return ptr;
}

static void *lxMemoryGeneric_callocAligned(lxMemoryGenericPTR gen, size_t num, size_t size, size_t alignsize)
{
  void *ptr = gen->descr._calloc(gen->descr.sys,1,(size*num) + alignsize + sizeof(size_t));

  ptr = lxMemAlignedFromOrig(ptr,alignsize);

  gen->info.allocs++;
  gen->info.mem += size;

  return ptr;
}

static void *lxMemoryGeneric_reallocAligned(lxMemoryGenericPTR gen, void* ptr, size_t newsize, size_t oldsize, size_t alignsize)
{
  byte* oldptr = (byte*)lxMemAlignedToOrig(ptr);
  size_t shiftold = (((byte*)ptr)-oldptr)-sizeof(size_t);

  ptr = gen->descr._realloc(gen->descr.sys,ptr,newsize + alignsize + sizeof(size_t));
  lxMemAlignedRealloc(ptr,alignsize,newsize,sizeof(size_t),shiftold);

  gen->info.mem += newsize-oldsize;

  return lxMemAlignedFromOrig(ptr,alignsize);
}

static size_t lxMemoryGeneric_offsetAligned(lxMemoryGenericPTR gen, void *ptr)
{
  byte* inptr = (byte*)ptr;
  byte* optr  = (byte*)lxMemAlignedToOrig(ptr);

  return (inptr-optr);
}


//////////////////////////////////////////////////////////////////////////


static void lxMemoryGeneric_freeStats(lxMemoryGenericPTR gen, void *ptr, size_t size,const char *source, int line)
{
  MemInfo_t *info;

  ptr = (char*)ptr-sizeof(MemInfo_t);
  info = (MemInfo_t*)ptr;
  lxListNode_remove(gen->stats.List,info);

  if (info->size!=size){
    LUX_PRINTF("free size err: %s (l:%i) (%i/%i)\n  allocated in %s (l:%i)\n",
      source,line,info->size,size, info->src,info->line);
  }

  gen->descr._free(gen->descr.sys,ptr);

  gen->info.allocs--;
  gen->info.mem -= size;
}

#define MEMORY_ALIGNED_MAGIC  0xAABBCCDD

static size_t  lxMemoryGeneric_offsetAlignedStats(lxMemoryGenericPTR gen, void *alptr)
{
  size_t *ptr1 = ((size_t *)alptr) - 3;
  size_t *ptr2 = ((size_t *)alptr) - 2;
  size_t *ptr3 = ((size_t *)alptr) - 1;

  if (*ptr1 != MEMORY_ALIGNED_MAGIC){
    LUX_ASSERT(0);
  }

  return *ptr3;
}

static void* lxMemoryGeneric_reallocStats(lxMemoryGenericPTR gen, void *ptr, size_t size,size_t oldsize,const char *source, int line)
{
  ptrdiff_t sz = size;
  MemInfo_t *info;
  char *mem;

  ptr = (char*)ptr-sizeof(MemInfo_t);
  info = (MemInfo_t*)ptr;
  lxListNode_remove(gen->stats.List,info);

  gen->info.mem += (ptrdiff_t)size - (ptrdiff_t)oldsize;
  gen->info.allocs--;


  if (info->size!=oldsize){
    LUX_PRINTF("realloc oldsize err: %s (l:%i) (%i/%i)\n  allocated in %s (l:%i)\n",
      source,line,info->size,size, info->src,info->line);
  }

  mem = (char*)gen->descr._realloc(gen->descr.sys,ptr,size+sizeof(MemInfo_t));

  info = (MemInfo_t*)mem;
  info->size = size;
  info->line = line;
  info->src = source;
  lxListNode_init(info);
  lxListNode_pushBack(gen->stats.List,info);

  return mem+sizeof(MemInfo_t);
}

static void *lxMemoryGeneric_callocStats(lxMemoryGenericPTR gen, size_t num, size_t size,const char *source,int line)
{
  char *mem;
  MemInfo_t *info;


  gen->info.mem += size*num;
  gen->info.allocs++;

  mem = (char*)gen->descr._calloc(gen->descr.sys,(size*num)+sizeof(MemInfo_t),1);

  info = (MemInfo_t*)mem;
  info->size = size*num;
  info->line = line;
  info->src = source;
  lxListNode_init(info);
  lxListNode_pushBack(gen->stats.List,info);


  return mem+sizeof(MemInfo_t);
}

static void *lxMemoryGeneric_mallocStats(lxMemoryGenericPTR gen, size_t size,const char *source,int line)
{
  char *mem;
  MemInfo_t *info;


  gen->info.mem += size;
  gen->info.allocs++;

  mem = (char*)gen->descr._malloc(gen->descr.sys,size+sizeof(MemInfo_t));

  info = (MemInfo_t*)mem;
  info->size = size;
  info->line = line;
  info->src = source;
  lxListNode_init(info);
  lxListNode_pushBack(gen->stats.List,info);


  return mem+sizeof(MemInfo_t);
}

static void* lxMemoryGeneric_mallocAlignedStats(lxMemoryGenericPTR gen, size_t size, size_t alignsize,const char *source,int line)
{
  char *ptr = (char*)lxMemoryGeneric_mallocStats(gen,size + (sizeof(size_t)*3) + alignsize,source,line);
  char *alptr = (char*)lxPointerAlign(ptr+sizeof(size_t)*3,alignsize);

  {
    size_t *ptr1 = ((size_t *)alptr) - 3;
    size_t *ptr2 = ((size_t *)alptr) - 2;
    size_t *ptr3 = ((size_t *)alptr) - 1;

    *ptr1 = MEMORY_ALIGNED_MAGIC;
    *ptr2 = (sizeof(size_t)*3) + alignsize;
    *ptr3 = alptr-ptr;
  }


  return alptr;
}

static void* lxMemoryGeneric_callocAlignedStats(lxMemoryGenericPTR gen, size_t num, size_t size, size_t alignsize,const char *source,int line)
{
  char *ptr = (char*)lxMemoryGeneric_callocStats(gen,size*num + (sizeof(size_t)*3) + alignsize,1,source,line);
  char *alptr = (char*)lxPointerAlign(ptr+sizeof(size_t)*3,alignsize);

  {
    size_t *ptr1 = ((size_t *)alptr) - 3;
    size_t *ptr2 = ((size_t *)alptr) - 2;
    size_t *ptr3 = ((size_t *)alptr) - 1;

    *ptr1 = MEMORY_ALIGNED_MAGIC;
    *ptr2 = (sizeof(size_t)*3) + alignsize;
    *ptr3 = alptr-ptr;
  }


  return alptr;
}

static void* lxMemoryGeneric_reallocAlignedStats(lxMemoryGenericPTR gen, void* ptr,size_t size,size_t alignsize,size_t oldsize,const char *source,int line)
{
  size_t *ptr1 = ((size_t *)ptr) - 3;
  size_t *ptr2 = ((size_t *)ptr) - 2;
  size_t *ptr3 = ((size_t *)ptr) - 1;
  size_t offset = *ptr3;
  char *newptr;
  char *alptr;

  if (*ptr1 != MEMORY_ALIGNED_MAGIC){
    LUX_PRINTF("realloc align err: %s (l:%i) (%s)\n  non-aligned alloc used\n",source,line,size);
    LUX_ASSERT(0);
  }
  else{
    ptr = (void*)(((char*)ptr) - offset);
  }

  newptr = (char*)lxMemoryGeneric_reallocStats(gen,ptr, size + (sizeof(size_t)*3) + alignsize, oldsize + *ptr2,source,line);
  lxMemAlignedRealloc(newptr,alignsize,size,(sizeof(size_t)*3),(offset)-(sizeof(size_t)*3));

  alptr = (char*)lxPointerAlign(newptr+sizeof(size_t)*3,alignsize);

  {
    ptr1 = ((size_t *)alptr) - 3;
    ptr2 = ((size_t *)alptr) - 2;
    ptr3 = ((size_t *)alptr) - 1;

    *ptr1 = MEMORY_ALIGNED_MAGIC;
    *ptr2 = (sizeof(size_t)*3) + alignsize;
    *ptr3 = alptr-newptr;
  }


  return alptr;
}

LUX_API void  lxMemoryGeneric_freeAlignedStats(lxMemoryGenericPTR gen, void *ptr, size_t size,const char *source,int line)
{
  size_t *ptr1 = ((size_t *)ptr) - 3;
  size_t *ptr2 = ((size_t *)ptr) - 2;
  size_t *ptr3 = ((size_t *)ptr) - 1;

  if (*ptr1 != MEMORY_ALIGNED_MAGIC){
    LUX_PRINTF("free align err: %s (l:%i) (%s)\n  non-aligned alloc used\n",source,line,size);
    LUX_ASSERT(0);
  }
  else{
    ptr = (void*)(((char*)ptr) - *ptr3);
  }

  lxMemoryGeneric_freeStats(gen,ptr,size + *ptr2,source,line);
}

#undef MEMORY_ALIGNED_MAGIC


//////////////////////////////////////////////////////////////////////////

LUX_API void lxMemoryGeneric_dumpStats(lxMemoryGenericPTR gen, const char* fname)
{
#if defined(LUX_MEMORY_STATS)
  MemInfo_t* browse;
  FILE* of;

  if (!gen->stats.List) return;

  of = fopen(fname,"wb");
  fprintf(of,"return {\n");
  lxListNode_forEach(gen->stats.List,browse)
    fprintf(of,"{file=[[%s]],\tline=%d,\tsize=%d,},\n",browse->src,browse->line,browse->size);
  lxListNode_forEachEnd(gen->stats.List,browse);
  fprintf(of,"}\n");
  fclose(of);
#endif
}

LUX_API booln lxMemoryGeneric_deinitStats(lxMemoryGenericPTR gen)
{
#if !defined(LUX_MEMORY_STATS)
  return LUX_TRUE;
#else
  MemInfo_t* browse;
  int i,sz;

  if (gen->stats.List == NULL) return LUX_TRUE;

  i = 0; sz = 0;
  LUX_PRINTF("--Un-freed blocks:\n");
  while (gen->stats.List) {
    lxListNode_popBack(gen->stats.List,browse);
    LUX_PRINTF("{ num = %5.i, ptr = %p, size = %5.i, src = \"%s(%i)\",},\n",++i,browse+1,browse->size,browse->src,browse->line);
    sz+=browse->size;
  }
  LUX_PRINTF("--Number of unfreed blocks: %i (%i bytes)\n",i,sz);

  return LUX_FALSE;
#endif
}


static void lxMemoryGeneric_init(lxMemoryGenericPTR gen,lxMemoryGenericDescr_t descr)
{
  memset(gen,0,sizeof(lxMemoryGeneric_t));
  gen->descr = descr;
  gen->allocator._malloc = (lxMalloc_fn)lxMemoryGeneric_malloc;
  gen->allocator._calloc = (lxCalloc_fn)lxMemoryGeneric_calloc;
  gen->allocator._realloc = (lxRealloc_fn)lxMemoryGeneric_realloc;
  gen->allocator._free = (lxFree_fn)lxMemoryGeneric_free;
  gen->allocator._mallocAligned = (lxMallocAligned_fn)lxMemoryGeneric_mallocAligned;
  gen->allocator._callocAligned = (lxCallocAligned_fn)lxMemoryGeneric_callocAligned;
  gen->allocator._reallocAligned = (lxReallocAligned_fn)lxMemoryGeneric_reallocAligned;
  gen->allocator._freeAligned = (lxFreeAligned_fn)lxMemoryGeneric_freeAligned;
  gen->allocator.tracker = &gen->tracker;
  gen->tracker._malloc = (lxMallocStats_fn)lxMemoryGeneric_mallocStats;
  gen->tracker._calloc = (lxCallocStats_fn)lxMemoryGeneric_callocStats;
  gen->tracker._realloc = (lxReallocStats_fn)lxMemoryGeneric_reallocStats;
  gen->tracker._free = (lxFreeStats_fn)lxMemoryGeneric_freeStats;
  gen->tracker._mallocAligned = (lxMallocAlignedStats_fn)lxMemoryGeneric_mallocAlignedStats;
  gen->tracker._callocAligned = (lxCallocAlignedStats_fn)lxMemoryGeneric_callocAlignedStats;
  gen->tracker._reallocAligned = (lxReallocAlignedStats_fn)lxMemoryGeneric_reallocAlignedStats;
  gen->tracker._freeAligned = (lxFreeAlignedStats_fn)lxMemoryGeneric_freeAlignedStats;
}

LUX_API lxMemoryGenericPTR lxMemoryGeneric_new(lxMemoryGenericDescr_t  descr)
{
  lxMemoryGenericPTR gen = (lxMemoryGeneric_t*)descr._malloc(descr.sys,sizeof(lxMemoryGeneric_t));
  lxMemoryGeneric_init(gen,descr);
  return gen;
}

LUX_API void lxMemoryGeneric_delete(lxMemoryGenericPTR gen)
{
  gen->descr._free(gen->descr.sys,gen);
}

static void* default_malloc(void* sys, size_t sz)
{
  return malloc(sz);
}

static void* default_calloc(void* sys, size_t sz, size_t num)
{
  return calloc(sz,num);
}

static void* default_realloc(void* sys, void* ptr, size_t sz)
{
  return realloc(ptr,sz);
}

static void default_free(void* sys, void* ptr)
{
  free(ptr);
}

LUX_API lxMemoryGenericDescr_t lxMemoryGenericDescr_default()
{
  static lxMemoryGenericDescr_t descr = {NULL,default_malloc, default_calloc, default_realloc, default_free};
  return descr;
}

