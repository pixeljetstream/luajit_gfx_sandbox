// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/memorypool.h>
#include <luxinia/luxplatform/debug.h>

//////////////////////////////////////////////////////////////////////////
// MemoryPool
/**
* Allows to allocate a chunk of data which is parted
* into clusters. You can malloc only one cluster per time. Usefull for data
* structures with constant size like listnodes. There is no guarantee that
* two malloced variables will be adjacent in the memory.
*/

#include "memory_defs.h"

LUX_API lxMemoryPoolPTR lxMemoryPool_new (lxMemoryAllocatorPTR allocator, uint valueSize, uint pageValues, uint alignSize, booln multiPages)
{
  lxMemoryPoolPTR memg = (lxMemoryPoolPTR) lxMemoryAllocator_malloc(allocator,sizeof(lxMemoryPool_t));
  lxMemoryPool_init(memg,allocator,valueSize,pageValues,alignSize,multiPages);

  return memg;
}

void lxMemoryPool_addPage (lxMemoryPoolPTR memg)
{
  uint valueSize = memg->valueSize;
  uint pageValues = memg->pageValues;
  uint alignSize = memg->alignSize;
  uint pageSize = pageValues*valueSize;
  uint usedValues = (pageSize-sizeof(lxMemoryPage_t))/valueSize;

  byte* cur;
  byte* data;
  lxMemoryNode_t* node;
  lxMemoryPage_t* page;
  uint32 i;

  LUX_DEBUGASSERT(usedValues);

  if (alignSize){
    data = (byte*)lxMemoryAllocator_mallocAligned(memg->allocator,pageSize, alignSize );
  }
  else{
    data = (byte*)lxMemoryAllocator_malloc(memg->allocator,pageSize);
  }
  // add page
  page = (lxMemoryPage_t*)data;
  page->next = memg->pagelist;
  memg->pagelist = page;

  // add freeitems
  data += valueSize*(pageValues-usedValues);
  cur = data;
  for (i = 0; i < usedValues-1; ++i, cur+=valueSize){
    node = (lxMemoryNode_t*)cur;
    node->next = (lxMemoryNode_t*)(cur+valueSize);
  }
  // last item
  node = (lxMemoryNode_t*)cur;
  node->next = memg->freelist;
  memg->freelist = (lxMemoryNode_t*)data;
  
}

void lxMemoryPool_init (lxMemoryPoolPTR memg, lxMemoryAllocatorPTR allocator, uint valueSize, uint pageValues, uint alignSize, booln multiPages)
{
  memg->valueSize = valueSize;
  memg->pageValues = pageValues;
  memg->alignSize = alignSize;
  memg->multiPages = multiPages;
  memg->used = 0;
  memg->pagelist = NULL;
  memg->freelist = NULL;
  memg->allocator = allocator;

  LUX_ASSERT(alignSize == 0 || (valueSize % alignSize) == 0);

  lxMemoryPool_addPage(memg);
}


LUX_API void* lxMemoryPool_allocItem (lxMemoryPoolPTR  mem) 
{
  lxMemoryNode_t* node = mem->freelist;
  if (!node && mem->multiPages){
    lxMemoryPool_addPage(mem);
    node = mem->freelist;
  }
  LUX_DEBUGASSERT(node);
  
  mem->used++;
  mem->freelist = node->next;
  
  return (void*)node;
}

LUX_API void lxMemoryPool_freeItem (lxMemoryPoolPTR  mem, void *ptrv)
{
  lxMemoryNode_t* node = (lxMemoryNode_t*)ptrv;
  node->next = mem->freelist;
  mem->freelist = node;

  mem->used--;
}

LUX_API uint lxMemoryPool_shrink(lxMemoryPoolPTR mem)
{
  lxMemoryPage_t* page  = mem->pagelist;
  lxMemoryNode_t* node  = mem->freelist;
  lxMemoryPage_t**  lastp = &mem->pagelist;
  booln aligned = mem->alignSize;
  uint pageSize = mem->pageValues * mem->valueSize;
  uint maxValues = (pageSize-sizeof(lxMemoryPage_t))/mem->valueSize;
  uint shrinked = 0;

  // init counters
  while(page){
    lxMemoryPage_t* next = page->next;
    page->size = 0;

    page = next;
  }

  // find out how many freeitems per page
  while (node){
    byte* nb = (byte*)node;

    page = mem->pagelist;
    while(page){
      lxMemoryPage_t* next = page->next;
      byte* pb = (byte*)page;

      page->size += (nb > pb && nb < pb+pageSize);

      page = next;
    }
    node = node->next;
  }

  while(page){
    lxMemoryPage_t* next = page->next;
    // remove from list
    if (page->size == maxValues){
      *lastp = next;
      if (aligned){
        lxMemoryAllocator_freeAligned(mem->allocator,page,pageSize);
      }
      else{
        lxMemoryAllocator_free(mem->allocator,page,pageSize);
      }

      shrinked  += pageSize;
    }
    else{
      lastp = &page->next;
    }
    page = next;
  }

  return shrinked;
}

void lxMemoryPool_deinit (lxMemoryPoolPTR mem) {
  lxMemoryPage_t* page  = mem->pagelist;
  uint pagesize = mem->pageValues*mem->valueSize;
  booln aligned = mem->alignSize;
  
  while(page){
    lxMemoryPage_t* next = page->next;
    if (aligned){
      lxMemoryAllocator_freeAligned(mem->allocator,page,pagesize);
    }
    else{
      lxMemoryAllocator_free(mem->allocator,page,pagesize);
    }
    
    page = next;
  }
  
  mem->pagelist = NULL;
  mem->freelist = NULL;
  mem->used = 0;
}

LUX_API LUX_INLINE uint lxMemoryPool_memUsed(lxMemoryPoolPTR mem)
{
  return mem->used*mem->valueSize;
}
LUX_API LUX_INLINE uint lxMemoryPool_memAllocated(lxMemoryPoolPTR mem)
{
  byte* page  = (byte*)mem->pagelist;
  uint pagesize = mem->pageValues*mem->valueSize;
  uint ret = 0;

  while(page){
    lxMemoryNode_t* next = (lxMemoryNode_t*)(page+pagesize);
    ret += pagesize;
    page = (byte*)next->next;
  }

  return ret;
}

LUX_API float lxMemoryPool_memRatio(lxMemoryPoolPTR mem)
{
  return (float)lxMemoryPool_memUsed(mem)/(float)lxMemoryPool_memAllocated(mem);
}

LUX_API uint lxMemoryPool_varSize(lxMemoryPoolCPTR mem)
{
  return mem->valueSize;
}

LUX_API uint lxMemoryPool_alignSize(lxMemoryPoolCPTR mem)
{
  return mem->alignSize;
}

LUX_API void lxMemoryPool_delete (lxMemoryPoolPTR mem) {
  lxMemoryPool_deinit(mem);
  lxMemoryAllocator_free(mem->allocator,mem,sizeof(lxMemoryPool_t));
}
