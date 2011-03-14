// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxcore/memorygeneric.h>
#include <luxplatform/debug.h>
#include <luxcore/contmacrolinkedlist.h>


//////////////////////////////////////////////////////////////////////////
// MemoryGeneric

typedef struct MemInfo_s
{
  size_t    size;
  int     line;
  const char* src;

  struct MemInfo_s LUX_LISTNODEVARS;
} MemInfo_t;

typedef struct MemStats_s{
  MemInfo_t *List;
}MemStats_t;


typedef struct lxMemoryGeneric_s{
  lxMemoryAllocator_t   allocator;
  lxMemoryGenericInfo_t info;
  #if defined(MEMORY_STATS)
  MemStats_t    stats;
  #endif
}lxMemoryGeneric_t;


lxMemoryGenericInfo_t lxMemoryGeneric_getInfo(lxMemoryGenericPTR gen)
{
  return gen->info;
}


#define MEMORY_GLOBAL_CALLOC(a,b)     calloc(a,b)
#define MEMORY_GLOBAL_MALLOC(a)       malloc(a)
#define MEMORY_GLOBAL_REALLOC(a,b)    realloc(a,b)
#define MEMORY_GLOBAL_FREE(a)         free(a)

//////////////////////////////////////////////////////////////////////////

LUX_API void lxMemoryGeneric_free(lxMemoryGenericPTR gen, void *ptr, size_t size) {
  gen->info.allocs--;
  gen->info.mem -= size;

  MEMORY_GLOBAL_FREE(ptr);
}

LUX_API void *lxMemoryGeneric_zalloc(lxMemoryGenericPTR gen, size_t size)
{
  gen->info.allocs++;
  gen->info.mem += size;
  {
    void *ptr = MEMORY_GLOBAL_CALLOC(1,size);

    return ptr;
  }
}

LUX_API void *lxMemoryGeneric_calloc(lxMemoryGenericPTR gen, size_t num, size_t size)
{
  gen->info.allocs++;
  gen->info.mem += size;
  {
    void *ptr = MEMORY_GLOBAL_CALLOC(num,size);

    return ptr;
  }
}

LUX_API void *lxMemoryGeneric_malloc(lxMemoryGenericPTR gen, size_t size)
{
  gen->info.allocs++;
  gen->info.mem += size;
  {
    void *ptr = MEMORY_GLOBAL_MALLOC(size);

    return ptr;
  }
}

LUX_API void *lxMemoryGeneric_realloc(lxMemoryGenericPTR gen, void* ptr, size_t size, size_t oldsize)
{
  gen->info.mem += size-oldsize;

  ptr = MEMORY_GLOBAL_REALLOC(ptr,size);
  return ptr;
}

LUX_API void lxMemoryGeneric_freeAligned(lxMemoryGenericPTR gen, void *ptr, size_t size) {
  gen->info.allocs--;

  ptr = lxMemAlignedToOrig(ptr);
  MEMORY_GLOBAL_FREE(ptr);
}

LUX_API void *lxMemoryGeneric_zallocAligned(lxMemoryGenericPTR gen, size_t size, size_t alignsize)
{
  void *ptr = MEMORY_GLOBAL_MALLOC(size + alignsize + sizeof(size_t));

  ptr = lxMemAlignedFromOrig(ptr,alignsize);
  memset(ptr,0,size);

  gen->info.allocs++;
  gen->info.mem += size;

  return ptr;
}

LUX_API void *lxMemoryGeneric_mallocAligned(lxMemoryGenericPTR gen, size_t size, size_t alignsize)
{
  void *ptr = MEMORY_GLOBAL_MALLOC(size + alignsize + sizeof(size_t));

  ptr = lxMemAlignedFromOrig(ptr,alignsize);

  gen->info.allocs++;
  gen->info.mem += size;

  return ptr;
}

LUX_API void *lxMemoryGeneric_callocAligned(lxMemoryGenericPTR gen, size_t num, size_t size, size_t alignsize)
{
  void *ptr = MEMORY_GLOBAL_CALLOC(1,(size*num) + alignsize + sizeof(size_t));

  ptr = lxMemAlignedFromOrig(ptr,alignsize);

  gen->info.allocs++;
  gen->info.mem += size;

  return ptr;
}

LUX_API void *lxMemoryGeneric_reallocAligned(lxMemoryGenericPTR gen, void* ptr, size_t newsize, size_t oldsize, size_t alignsize)
{
  byte* oldptr = (byte*)lxMemAlignedToOrig(ptr);
  size_t shiftold = (((byte*)ptr)-oldptr)-sizeof(size_t);

  ptr = MEMORY_GLOBAL_REALLOC(ptr,newsize + alignsize + sizeof(size_t));
  lxMemAlignedRealloc(ptr,alignsize,newsize,sizeof(size_t),shiftold);

  gen->info.mem += newsize-oldsize;

  return lxMemAlignedFromOrig(ptr,alignsize);
}

LUX_API size_t lxMemoryGeneric_offsetAligned(lxMemoryGenericPTR gen, void *ptr)
{
  byte* inptr = (byte*)ptr;
  byte* optr  = (byte*)lxMemAlignedToOrig(ptr);

  return (inptr-optr);
}


//////////////////////////////////////////////////////////////////////////

#if defined(MEMORY_STATS)

LUX_API void lxMemoryGeneric_freeStats(lxMemoryGenericPTR gen, void *ptr, size_t size,const char *source, int line)
{
  MemInfo_t *info;

  ptr = (char*)ptr-sizeof(MemInfo_t);
  info = (MemInfo_t*)ptr;
  lxListNode_remove(gen->stats.List,info);

  if (info->size!=size){
    LUX_PRINTF("free size err: %s (l:%i) (%i/%i)\n  allocated in %s (l:%i)\n",
      source,line,info->size,size, info->src,info->line);
  }

  MEMORY_GLOBAL_FREE(ptr);

  gen->info.allocs--;
  gen->info.mem -= size;
}

#define MEMORY_ALIGNED_MAGIC  0xAABBCCDD

LUX_API size_t  lxMemoryGeneric_offsetAlignedStats(lxMemoryGenericPTR gen, void *alptr)
{
  size_t *ptr1 = ((size_t *)alptr) - 3;
  size_t *ptr2 = ((size_t *)alptr) - 2;
  size_t *ptr3 = ((size_t *)alptr) - 1;

  if (*ptr1 != MEMORY_ALIGNED_MAGIC){
    LUX_ASSERT(0);
  }

  return *ptr3;
}

LUX_API void* lxMemoryGeneric_reallocStats(lxMemoryGenericPTR gen, void *ptr, size_t size,size_t oldsize,const char *source, int line)
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

  mem = (char*)MEMORY_GLOBAL_REALLOC(ptr,size+sizeof(MemInfo_t));

  info = (MemInfo_t*)mem;
  info->size = size;
  info->line = line;
  info->src = source;
  lxListNode_init(info);
  lxListNode_pushBack(gen->stats.List,info);

  return mem+sizeof(MemInfo_t);
}

LUX_API void *lxMemoryGeneric_zallocStats(lxMemoryGenericPTR gen, size_t size,const char *source,int line)
{
  char *mem;
  MemInfo_t *info;


  gen->info.mem += size;
  gen->info.allocs++;

  mem = (char*)MEMORY_GLOBAL_CALLOC(1,size+sizeof(MemInfo_t));

  info = (MemInfo_t*)mem;
  info->size = size;
  info->line = line;
  info->src = source;
  lxListNode_init(info);
  lxListNode_pushBack(gen->stats.List,info);


  return mem+sizeof(MemInfo_t);
}

void *lxMemoryGeneric_callocStats(lxMemoryGenericPTR gen, size_t num, size_t size,const char *source,int line)
{
  char *mem;
  MemInfo_t *info;


  gen->info.mem += size*num;
  gen->info.allocs++;

  mem = (char*)MEMORY_GLOBAL_CALLOC(num,size+sizeof(MemInfo_t));

  info = (MemInfo_t*)mem;
  info->size = size*num;
  info->line = line;
  info->src = source;
  lxListNode_init(info);
  lxListNode_pushBack(gen->stats.List,info);


  return mem+sizeof(MemInfo_t);
}

LUX_API void *lxMemoryGeneric_mallocStats(lxMemoryGenericPTR gen, size_t size,const char *source,int line)
{
  char *mem;
  MemInfo_t *info;


  gen->info.mem += size;
  gen->info.allocs++;

  mem = (char*)MEMORY_GLOBAL_MALLOC(size+sizeof(MemInfo_t));

  info = (MemInfo_t*)mem;
  info->size = size;
  info->line = line;
  info->src = source;
  lxListNode_init(info);
  lxListNode_pushBack(gen->stats.List,info);


  return mem+sizeof(MemInfo_t);
}

LUX_API void* lxMemoryGeneric_zallocAlignedStats(lxMemoryGenericPTR gen, size_t size, size_t alignsize,const char *source,int line)
{
  char *ptr = (char*)lxMemoryGeneric_zallocStats(gen,size + (sizeof(size_t)*3) + alignsize,source,line);
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

LUX_API void* lxMemoryGeneric_mallocAlignedStats(lxMemoryGenericPTR gen, size_t size, size_t alignsize,const char *source,int line)
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

LUX_API void* lxMemoryGeneric_callocAlignedStats(lxMemoryGenericPTR gen, size_t num, size_t size, size_t alignsize,const char *source,int line)
{
  return lxMemoryGeneric_zallocAlignedStats(gen, num*size,alignsize,source,line);
}

LUX_API void* lxMemoryGeneric_reallocAlignedStats(lxMemoryGenericPTR gen, void* ptr,size_t size,size_t alignsize,size_t oldsize,const char *source,int line)
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

#endif // #if defined(MEMORY_STATS)

//////////////////////////////////////////////////////////////////////////

LUX_API void lxMemoryGeneric_dumpStats(lxMemoryGenericPTR gen, const char* fname)
{
#if defined(MEMORY_STATS)
  MemInfo_t* browse;
  FILE* of = fopen(fname,"wb");
  fprintf(of,"return {\n");
  lxListNode_forEach(gen->stats.List,browse)
    fprintf(of,"{file=[[%s]],\tline=%d,\tsize=%d,},\n",browse->src,browse->line,browse->size);
  lxListNode_forEachEnd(gen->stats.List,browse);
  fprintf(of,"}\n");
  fclose(of);
#endif
}

booln lxMemoryGeneric_deinitStats(lxMemoryGenericPTR gen)
{
#if !defined(MEMORY_STATS)
  return LUX_TRUE;
#else
  MemInfo_t* browse;
  int i,sz;


  if (gen->stats.List == NULL) return LUX_TRUE;
  i = 0; sz = 0;
  LUX_PRINTF("Unfreed blocks:\n");
  while (gen->stats.List) {
    lxListNode_popBack(gen->stats.List,browse);
    LUX_PRINTF("    %5.i: %p (%5.i): %s (l:%i)\n",++i,browse+1,browse->size,browse->src,browse->line);
    sz+=browse->size;
  }
  LUX_PRINTF("Number of unfreed blocks: %i (%i bytes)\n",i,sz);

  return LUX_FALSE;
#endif
}


static void lxMemoryGeneric_init(lxMemoryGenericPTR gen)
{
  memset(gen,0,sizeof(lxMemoryGeneric_t));
#ifdef MEMORY_STATS
  gen->allocator._malloc = (lxMallocStats_fn)lxMemoryGeneric_mallocStats;
  gen->allocator._calloc = (lxCallocStats_fn)lxMemoryGeneric_callocStats;
  gen->allocator._realloc = (lxReallocStats_fn)lxMemoryGeneric_reallocStats;
  gen->allocator._free = (lxFreeStats_fn)lxMemoryGeneric_freeStats;
  gen->allocator._mallocAligned = (lxMallocAlignedStats_fn)lxMemoryGeneric_mallocAlignedStats;
  gen->allocator._callocAligned = (lxCallocAlignedStats_fn)lxMemoryGeneric_callocAlignedStats;
  gen->allocator._reallocAligned = (lxReallocAlignedStats_fn)lxMemoryGeneric_reallocAlignedStats;
  gen->allocator._freeAligned = (lxFreeAlignedStats_fn)lxMemoryGeneric_freeStats;
#else
  gen->allocator._malloc = (lxMalloc_fn)lxMemoryGeneric_malloc;
  gen->allocator._calloc = (lxCalloc_fn)lxMemoryGeneric_calloc;
  gen->allocator._realloc = (lxRealloc_fn)lxMemoryGeneric_realloc;
  gen->allocator._free = (lxFree_fn)lxMemoryGeneric_free;
  gen->allocator._mallocAligned = (lxMallocAligned_fn)lxMemoryGeneric_mallocAligned;
  gen->allocator._callocAligned = (lxCallocAligned_fn)lxMemoryGeneric_callocAligned;
  gen->allocator._reallocAligned = (lxReallocAligned_fn)lxMemoryGeneric_reallocAligned;
  gen->allocator._freeAligned = (lxFreeAligned_fn)lxMemoryGeneric_free;
#endif
}

LUX_API lxMemoryGenericPTR lxMemoryGeneric_new()
{
  lxMemoryGenericPTR gen = MEMORY_GLOBAL_MALLOC(sizeof(lxMemoryGeneric_t));
  lxMemoryGeneric_init(gen);
  return gen;
}

LUX_API void lxMemoryGeneric_delete(lxMemoryGenericPTR gen)
{
  MEMORY_GLOBAL_FREE(gen);
}

