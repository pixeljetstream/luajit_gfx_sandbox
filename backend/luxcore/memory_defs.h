// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXCORE_MEMDEFS_H__
#define __LUXCORE_MEMDEFS_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <luxinia/luxcore/memorybase.h>
#include <luxinia/luxcore/memorylist.h>
#include <luxinia/luxcore/memorypool.h>
#include <luxinia/luxplatform/debug.h>


#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////

typedef struct lxMemoryNode_s{
  struct lxMemoryNode_s*  next;
}lxMemoryNode_t;

typedef struct lxMemoryPage_s {
  struct lxMemoryPage_s*  next;
  uint          size;
} lxMemoryPage_t;

//////////////////////////////////////////////////////////////////////////
// MemoryList
#define MEMORY_LIST_MAXSLOTS  32

typedef struct lxMemorySizeSlot_s {
  uint      size;
  lxMemoryNode_t* freelist;
  int       cnt;
} lxMemorySizeSlot_t;

typedef struct lxMemoryList_s {
  int   cnt;
  lxMemorySizeSlot_t  slots[MEMORY_LIST_MAXSLOTS];

  byte*     chunk;
  uint      chunkpos;
  uint      chunksize;
  uint      alignSize;
  
  lxMemoryPage_t  *pagelist;
  lxMemoryAllocatorPTR allocator;
  int       numItems;
  uint      memAllocatedPages;
  uint      memAllocatedOuter;
  uint      memWasted;
} lxMemoryList_t;

void lxMemoryList_initBits (lxMemoryListPTR mc, lxMemoryAllocatorPTR allocator, uint sizeminbit, uint sizemaxbit, uint startpagesize, uint alignSize);
void lxMemoryList_init (lxMemoryListPTR mc, lxMemoryAllocatorPTR allocator, uint numSizes, const uint *sizes, uint startpagesize, uint alignSize);
void lxMemoryList_deinit (lxMemoryListPTR mc);

//////////////////////////////////////////////////////////////////////////
// MemoryPool

typedef struct lxMemoryPool_s {
  lxMemoryAllocatorPTR allocator;
  uint  valueSize;
  uint  pageValues;
  uint  alignSize;
  uint  used;
  booln multiPages;

  lxMemoryNode_t* freelist;
  lxMemoryPage_t* pagelist;
} lxMemoryPool_t;

void lxMemoryPool_init (lxMemoryPoolPTR mem, lxMemoryAllocatorPTR allocator, uint valueSize, uint pageValues, uint alignSize, booln multiPages);
void lxMemoryPool_deinit (lxMemoryPoolPTR mem);


//////////////////////////////////////////////////////////////////////////
// MemoryStack

typedef struct lxMemoryStack_s{
  lxMemoryAllocatorPTR allocator;
  const char    *name;
  byte      *dataorig;  // the allocated memory
  byte      *data;    // the aligned start
  size_t      inuse;    // what is used in bytes
  size_t      total;    // the total in bytes
  booln     resized;
}lxMemoryStack_t;

#ifdef __cplusplus
}
#endif

#endif
