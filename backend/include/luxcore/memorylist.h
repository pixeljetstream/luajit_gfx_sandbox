// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_MEMORYLIST_H__
#define __LUXCORE_MEMORYLIST_H__

#include "memorybase.h"

#ifdef __cplusplus
extern "C"{
#endif
  typedef struct lxMemoryList_s* lxMemoryListPTR;

  //////////////////////////////////////////////////////////////////////////
  // MemoryList
  //
  // Allocator with fixed size slots for allocation re-use. Allocates
  // larger pages whose chunks are reused. Outside of managed sizes
  // get their own allocation and are not take from page.
  //
  // currently a maximum of 16 slots can be used
  // if alignSize is given, then all managed sizes must be a multiple of it 
  // (otherwise are rounded up to next alignSize multiple and redundant
  // sizes are removed)

    // useful 4,19,1<<20 == min alloc is 16 bytes, max alloc is 512kb, startup is 1 meg
  LUX_API lxMemoryListPTR lxMemoryList_newBits(lxMemoryAllocatorPTR allocator, uint sizeminbit, uint sizemaxbit, uint startpagesize, uint alignSize);

    // sizes must be passed in ascending order
  LUX_API lxMemoryListPTR lxMemoryList_new(lxMemoryAllocatorPTR allocator, uint numSizes, uint *sizes, uint startpagesize, uint alignSize);
  LUX_API void lxMemoryList_delete(lxMemoryListPTR);

  LUX_API void lxMemoryList_stats(lxMemoryListPTR, uint *allocTotal, uint *allocPaged, float* pageratio);

  LUX_API void lxMemoryList_freeItem (lxMemoryListPTR mc, void* ptr, uint oldsize);
  LUX_API void* lxMemoryList_allocItem (lxMemoryListPTR mc, uint size);
  LUX_API void* lxMemoryList_zallocItem (lxMemoryListPTR mc, uint size);
  LUX_API void* lxMemoryList_reallocItem (lxMemoryListPTR mc, void* ptr, uint oldsize, uint newsize);


  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE void* lxMemoryList_zallocItem (lxMemoryListPTR mc, uint size){
    void* ptr = lxMemoryList_allocItem(mc,size);
    memset(ptr,0,size);
    return ptr;
  }
#ifdef __cplusplus
}
#endif


#endif
