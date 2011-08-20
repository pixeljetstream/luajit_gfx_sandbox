// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/memorylist.h>
#include <luxinia/luxplatform/debug.h>

#include "memory_defs.h"

static LUX_INLINE size_t alignValue(size_t val, size_t alignin)
{
  size_t align =  alignin ? alignin : 1;
  size_t offset = val % align;
  offset = offset ? align-offset : 0;
  return val + offset;
}


static void lxMemoryList_addPage (lxMemoryListPTR self, uint size)
{
  uint alignSize = self->alignSize;
  lxMemoryPage_t* oldchunk = (lxMemoryPage_t*)self->pagelist;
  lxMemoryPage_t* newchunk = (lxMemoryPage_t*)lxMemoryAllocator_malloc(self->allocator,size+sizeof(lxMemoryPage_t)+self->alignSize);
  LUX_DEBUGASSERT(newchunk);

  newchunk->next = oldchunk;
  newchunk->size = size;
  self->pagelist = newchunk;

  if (oldchunk){
    // to prevent wasting old memory lets throw it in the first 
    // largest fitting size (could use better heuristic here)

    uint chunksize = self->chunksize;
    uint chunkpos = self->chunkpos;
    int cnt = self->cnt;
    uint left = chunksize-chunkpos;
    int i;

    if (left){
      for (i = cnt-1 ; i >= 0; i--){
        lxMemorySizeSlot_t  *cursl = &self->slots[i];
        while (left >= cursl->size){
          lxMemoryNode_t* block = (lxMemoryNode_t*)&self->chunk[chunkpos];
          block->next = cursl->freelist;
          cursl->freelist = block;

          chunkpos += cursl->size;
          left -= cursl->size;
        }
      }
    }
  }

  self->chunk = (byte*)alignValue((size_t)(newchunk+1),self->alignSize);
  self->chunksize = size;
  self->chunkpos = 0;

  self->memAllocatedPages += size;
}

void lxMemoryList_init (lxMemoryListPTR self, lxMemoryAllocatorPTR allocator, uint numSizes, const uint* sizes, uint startsize, uint alignSize)
{
  uint i,n;
  uint cnt = numSizes;
  memset(self,0,sizeof(lxMemoryList_t));
  self->alignSize = alignSize;
  self->allocator = allocator;

  LUX_ASSERT(self->cnt <= MEMORY_LIST_MAXSLOTS);

  // correct sizs for alignment
  for (i = 0; i < numSizes; i++){
    self->slots[i].size = alignValue(sizes[i],alignSize);
  }

  // remove double entries
  for (i = 1, n = 1; i < numSizes; i++,n++){
    while (self->slots[i-1].size == self->slots[i].size && i < numSizes){
      cnt--;
      i++;
    }
    if (i < numSizes){
      self->slots[n].size = self->slots[i].size;
    }
  }

  self->cnt = cnt;
  lxMemoryList_addPage(self,startsize);
}

void lxMemoryList_deinit (lxMemoryListPTR self)
{
  lxMemoryPage_t* page = self->pagelist;
  uint alignSize = self->alignSize;

  while (page){
    lxMemoryPage_t* cur = page;
    page = cur->next;
    lxMemoryAllocator_free(self->allocator,cur,cur->size+sizeof(lxMemoryPage_t)+alignSize);
  }
  self->memAllocatedPages = 0;
}

LUX_API void lxMemoryList_freeItem (lxMemoryListPTR self, void* old, uint oldsize)
{
  int i;
  lxMemoryNode_t *block = (lxMemoryNode_t*)old;
  uint alignSize = self->alignSize;

  if (old==NULL || oldsize==0) return;

  self->numItems--;

  for (i = 0; i < self->cnt; i++) {
    lxMemorySizeSlot_t  *cursl = &self->slots[i];
    if (oldsize <= cursl->size) {
      block->next = cursl->freelist;
      cursl->freelist = block;
      cursl->cnt--;
      return;
    }
  }

  // outside of managed sized
  if (!alignSize){
    lxMemoryAllocator_free(self->allocator,old,oldsize);
  }
  else{
    lxMemoryAllocator_freeAligned(self->allocator,old,oldsize);
  }
  
  self->memAllocatedOuter -= oldsize;
}

LUX_API void* lxMemoryList_allocItem (lxMemoryListPTR self, uint size)
{
  byte *mem;
  int i;
  int cnt = self->cnt;

  self->numItems++;

  // outside of managed sizes
  if (size > self->slots[cnt-1].size) {
    uint alignSize = self->alignSize;
    self->memAllocatedOuter += size;
    if (!alignSize){
      return lxMemoryAllocator_malloc(self->allocator,size);;
    }
    else{
      return lxMemoryAllocator_mallocAligned(self->allocator,size,alignSize);
    }
  }

  // find next best slot / reuse
  for (i = 0; i < cnt; i++) {
    lxMemorySizeSlot_t  *cursl = &self->slots[i];

    if (size <= cursl->size) {
      size  = cursl->size;
      cursl->cnt++;
      // reuse
      if (mem = (byte*)cursl->freelist){
        cursl->freelist = cursl->freelist->next;
        return mem;
      }
#if 1
      //else if (self->chunkpos + size >= self->chunksize){
      else{
        // steal from next
        int n = i+1;
        while (n < cnt){
          cursl = &self->slots[n];
          if (mem = (byte*)cursl->freelist){
            // split among lower
            uint spread = cursl->size - size;
            void* outmem = (void*)mem;
            lxMemoryNode_t* node = (lxMemoryNode_t*)mem;
            cursl->freelist = node->next;
            mem += size;

            i = n-1;
            for (;i >= 0; --i){
              cursl = &self->slots[i];
              size = cursl->size;
              while (spread >= size){
                spread -= size;
                node = (lxMemoryNode_t*)(mem);
                node->next = cursl->freelist;
                cursl->freelist = node;
                mem += size;
              }
            }
            self->memWasted += spread;
            return outmem;
          }
          n++;
        }
      }
#endif
      
      break;
    }
  }

  // pool is full
  if (self->chunkpos + size >= self->chunksize) {
    uint nsize = (self->chunksize);
    nsize = nsize < size*2 ? size*2 : nsize;

    lxMemoryList_addPage(self,nsize);
  }

  mem = &self->chunk[self->chunkpos];
  self->chunkpos += size;


  return mem;
}

LUX_API void* lxMemoryList_reallocItem (lxMemoryListPTR self, void* old, uint oldsize, uint newsize)
{
  void *ptr;
  int i;
  int cnt = self->cnt;

  // already within same slotted size
  for (i=1; i < cnt; i++) {
    lxMemorySizeSlot_t  *prevsl = &self->slots[i-1];
    lxMemorySizeSlot_t  *cursl = &self->slots[i];

    if (oldsize <= cursl->size && newsize <= cursl->size &&
      oldsize > prevsl->size && newsize > prevsl->size)
      return old;
  }

  // find best fit / reuse 
  for (i=0; i < cnt; i++) {
    lxMemorySizeSlot_t  *cursl = &self->slots[i];

    if (newsize <= cursl->size) {
      if (cursl->freelist == NULL) {
        ptr = (void*)lxMemoryList_allocItem(self,cursl->size);
        break;
      }
      cursl->cnt++;
      ptr = (void*)cursl->freelist;
      cursl->freelist = cursl->freelist->next;
      break;
    }
  }

  // out of managed sizes
  if (i == cnt) {
    ptr = (void*)lxMemoryList_allocItem(self,newsize);
  }

  if (old) {
    memcpy(ptr,old,LUX_MIN(oldsize,newsize));
    lxMemoryList_freeItem(self,old,oldsize);
  }
  return ptr;
}

LUX_API void lxMemoryList_stats(lxMemoryListPTR self, uint *allocTotal, uint *allocPaged, float *pageratio)
{

  int i;
  uint inpages = 0;
  for (i=0; i < self->cnt; i++){
    inpages += self->slots[i].size * self->slots[i].cnt;
  }
  *allocTotal = self->memAllocatedPages+self->memAllocatedOuter;
  *allocPaged = self->memAllocatedPages;
  *pageratio  = ((float)inpages)/((float)self->memAllocatedPages);
}

void lxMemoryList_initBits(lxMemoryListPTR self, lxMemoryAllocatorPTR allocator, uint sizeminbit, uint sizemaxbit, uint startsize, uint alignSize)
{
  uint sizes[MEMORY_LIST_MAXSLOTS];
  int i;
  int numSizes = sizemaxbit-sizeminbit+1;
  LUX_ASSERT(numSizes <= MEMORY_LIST_MAXSLOTS);

  for (i=0; i < numSizes; i++){
    sizes[i] =  1<<(i+sizeminbit);
  }

  lxMemoryList_init(self,allocator,numSizes,sizes,startsize,alignSize);
}

LUX_API lxMemoryListPTR lxMemoryList_newBits(lxMemoryAllocatorPTR allocator, uint sizeminbit, uint sizemaxbit, uint startsize, uint alignSize)
{
  lxMemoryListPTR self;
  if (sizemaxbit-sizeminbit+1 > MEMORY_LIST_MAXSLOTS) return NULL;
  
  self = (lxMemoryList_t*)lxMemoryAllocator_malloc(allocator,sizeof(lxMemoryList_t));
  lxMemoryList_initBits(self,allocator,sizeminbit,sizemaxbit,startsize,alignSize);
  return self;
}

LUX_API lxMemoryListPTR lxMemoryList_new(lxMemoryAllocatorPTR allocator, uint numSizes, uint* sizes, uint startsize, uint alignSize)
{
  lxMemoryListPTR self = (lxMemoryList_t*)lxMemoryAllocator_malloc(allocator,sizeof(lxMemoryList_t));
  lxMemoryList_init(self,allocator,numSizes,sizes,startsize,alignSize);
  return self;
}

LUX_API void lxMemoryList_delete(lxMemoryListPTR self)
{
  lxMemoryList_deinit(self);
  lxMemoryAllocator_free(self->allocator,self,sizeof(lxMemoryList_t));
}
