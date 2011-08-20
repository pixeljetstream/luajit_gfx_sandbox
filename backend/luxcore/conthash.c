// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/conthash.h>
#include <luxinia/luxcore/memorypool.h>

#include "cont_defs.h"

#define CONT_HASH_FUNC(cv,k)  (k & (cv->mask))
#define CONT_HASH_SIZE(cv)    (cv->numBins)

LUX_INLINE size_t lxContHash_sizeof(uint numBins)
{
  return (sizeof(lxContHash_t)+(sizeof(lxContHashEntry_t*)*(numBins-1)));
}

void lxContHash_init(lxContHash_t *cv, lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize)
{
  uint extra = (valueSize > sizeof(void*) ? valueSize-(sizeof(void*)) : 0);
  uint itemsize = sizeof(lxContHashEntry_t)+ extra;

  LUX_DEBUGASSERT((numBins&(numBins-1))==0);
  
  itemsize += extra ? (4-(extra%4)) : 0;

  memset(cv,0,lxContHash_sizeof(numBins));
  cv->mask = (numBins-1);
  cv->numBins   = numBins;
  cv->valueSize = valueSize;
  
  lxMemoryPool_init(&cv->mempool,allocator,itemsize,CONT_HASH_PAGEBYTES/itemsize,4,LUX_TRUE);
}

void lxContHash_deinit(lxContHash_t *cv)
{
  lxMemoryPool_deinit(&cv->mempool);
}

LUX_API lxContHashPTR lxContHash_new(lxMemoryAllocatorPTR allocator,uint numBins,uint valueSize)
{
  lxContHashPTR cv = (lxContHashPTR) lxMemoryAllocator_malloc(allocator,lxContHash_sizeof(numBins));
  lxContHash_init(cv,allocator,numBins,valueSize);
  return cv;
}

LUX_API void  lxContHash_delete(lxContHashPTR cv)
{
  lxContHash_deinit(cv);
  lxMemoryAllocator_free(cv->mempool.allocator,cv,lxContHash_sizeof(cv->numBins));
}

LUX_API booln lxContHash_set(lxContHashPTR cv, uint32 key, const void *val)
{
  uint idx = CONT_HASH_FUNC(cv,key);
  uint valueSize = cv->valueSize;

  lxContHashEntry_t* entry = cv->table[idx];
  lxContHashEntry_t* oentry = entry;

  while (entry){
    if (entry->key == key){
      if (!valueSize){
        entry->data = (void*)val;
      }
      else{
        memcpy(entry->bytes,val,valueSize);
      }
      return LUX_TRUE;
    }
    entry = entry->next;
  }
  
  entry = (lxContHashEntry_t*)lxMemoryPool_allocItem(&cv->mempool);
  
  if (!valueSize){
    entry->data = (void*)val;
  }
  else{
    memcpy(entry->bytes,val,valueSize);
  }
  entry->key = key;
  entry->next = oentry;
  cv->table[idx] = entry;
  
  return LUX_FALSE;
}
LUX_API booln lxContHash_remove(lxContHashPTR cv, uint32 key)
{
  uint idx = CONT_HASH_FUNC(cv,key);
  lxContHashEntry_t* entry = cv->table[idx];
  lxContHashEntry_t** lastentry = &cv->table[idx];

  while (entry){
    if (entry->key == key){
      *lastentry = entry->next;
      lxMemoryPool_freeItem(&cv->mempool,entry);
      return LUX_TRUE;
    }
    lastentry = &entry->next;
    entry = entry->next;
  }
  
  return LUX_FALSE;
}
LUX_API booln lxContHash_get(lxContHashCPTR cv, uint32 key, void** outval)
{
  uint idx = CONT_HASH_FUNC(cv,key);
  booln valueSize = cv->valueSize;
  lxContHashEntry_t* entry = cv->table[idx];
    
  while (entry){
    if (entry->key == key){
      *outval = valueSize ? &entry->data : entry->data;
      return LUX_TRUE;
    }
    entry = entry->next;
  }
  
  return LUX_FALSE;
}

LUX_API booln lxContHash_isEmpty(lxContHashCPTR cv)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  uint idx;
  for (idx = 0; idx < tabsize; idx++){
    if (cv->table[idx]) return LUX_FALSE;
  }
  return LUX_TRUE;
}


LUX_API void  lxContHash_iterate(lxContHashPTR cv, lxContHash_Iterator_fn *itfunc, void *fnData)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  booln valueSize = cv->valueSize;
  uint idx;
  for (idx = 0; idx < tabsize; idx++){
    lxContHashEntry_t* entry = cv->table[idx];
    while (entry){
      lxContHashEntry_t* next = entry->next;
      fnData = itfunc(fnData,entry->key,valueSize ? (void*)entry->bytes : entry->data);
      entry = next;
    }
  }
}

LUX_API uint32  lxContHash_getFirstKey(lxContHashCPTR cv)
{
  uint idx;
  uint tabsize = CONT_HASH_SIZE(cv);

  for (idx = 0; idx < tabsize; idx++){
    lxContHashEntry_t* entry = cv->table[idx];
    if (entry){
      return entry->key;
    }
  }
  return 0;
}
LUX_API uint32  lxContHash_getNextKey(lxContHashCPTR cv, uint32 key)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  uint idx = CONT_HASH_FUNC(cv,key);
  lxContHashEntry_t* entry = cv->table[idx];

  while (entry){
    if (entry->key != key){
      return entry->key;
    }
    entry = entry->next;
  }

  for (idx = 0; idx < tabsize; idx++){
    entry = cv->table[idx];
    while (entry){
      if (entry->key != key){
        return entry->key;
      }
      entry = entry->next;
    }
  }
  return 0;
}

LUX_API uint  lxContHash_getCount(lxContHashCPTR cv)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  uint idx;
  uint cnt = 0;

  for (idx = 0; idx < tabsize; idx++){
    lxContHashEntry_t* entry = cv->table[idx];
    while (entry){
      cnt++;
      entry = entry->next;
    }
  }
  return cnt;
}
LUX_API void* lxContHash_getNth(lxContHashCPTR cv, uint n)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  uint idx;
  uint cnt = 0;
  booln valueSize = cv->valueSize;

  for (idx = 0; idx < tabsize; idx++){
    lxContHashEntry_t* entry = cv->table[idx];
    while (entry){
      if (cnt == n){
        return valueSize ? (void*)entry->bytes : entry->data;
      }
      cnt++;
      entry = entry->next;
    }
  }
  return NULL;
}

LUX_API uint lxContHash_shrink(lxContHashPTR cv)
{
  return lxMemoryPool_shrink(&cv->mempool);
}

LUX_API float lxContHash_memRatio(lxContHashPTR cv)
{
  return lxMemoryPool_memRatio(&cv->mempool);
}

//////////////////////////////////////////////////////////////////////////
// TODO should use template instead for both

#undef  CONT_HASH_FUNC  
#define CONT_HASH_FUNC(cv,k)  (((uint)(size_t)k) % (cv->mask))

LUX_INLINE size_t lxContPtrHash_sizeof(uint numBins)
{
  return (sizeof(lxContPtrHash_t)+(sizeof(lxContPtrHashEntry_t*)*(numBins-1)));
}

void lxContPtrHash_init(lxContPtrHash_t *cv, lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize)
{
  uint extra = (valueSize > sizeof(void*) ? valueSize-(sizeof(void*)) : 0);
  uint itemsize = sizeof(lxContPtrHashEntry_t)+ extra;

  LUX_DEBUGASSERT((numBins&(numBins-1))==0);

  itemsize += extra ? (4-(extra%4)) : 0;

  memset(cv,0,lxContPtrHash_sizeof(numBins));
  cv->numBins   = numBins;
  cv->valueSize = valueSize;
  cv->mask = ((numBins-1)|1);

  lxMemoryPool_init(&cv->mempool,allocator,itemsize,CONT_HASH_PAGEBYTES/itemsize,4,LUX_TRUE);
}

void lxContPtrHash_deinit(lxContPtrHash_t *cv)
{
  lxMemoryPool_deinit(&cv->mempool);
}

LUX_API lxContPtrHashPTR  lxContPtrHash_new(lxMemoryAllocatorPTR allocator, uint numBins,uint valueSize)
{
  lxContPtrHashPTR cv = (lxContPtrHashPTR) lxMemoryAllocator_malloc(allocator,lxContPtrHash_sizeof(numBins));
  lxContPtrHash_init(cv,allocator,numBins,valueSize);
  return cv;
}

LUX_API void  lxContPtrHash_delete(lxContPtrHashPTR cv)
{
  lxContPtrHash_deinit(cv);
  lxMemoryAllocator_free(cv->mempool.allocator,cv,lxContPtrHash_sizeof(cv->numBins));
}

LUX_API booln lxContPtrHash_set(lxContPtrHashPTR cv, void* key, const void *val)
{
  uint idx = CONT_HASH_FUNC(cv,key);
  uint valueSize = cv->valueSize;

  lxContPtrHashEntry_t* entry = cv->table[idx];
  lxContPtrHashEntry_t* oentry = entry;

  while (entry){
    if (entry->key == key){
      if (!valueSize){
        entry->data = (void*)val;
      }
      else{
        memcpy(entry->bytes,val,valueSize);
      }
      return LUX_TRUE;
    }
    entry = entry->next;
  }

  entry = (lxContPtrHashEntry_t*)lxMemoryPool_allocItem(&cv->mempool);

  if (!valueSize){
    entry->data = (void*)val;
  }
  else{
    memcpy((void*)entry->bytes,val,valueSize);
  }
  entry->key = key;
  entry->next = oentry;
  cv->table[idx] = entry;

  return LUX_FALSE;
}
LUX_API booln lxContPtrHash_remove(lxContPtrHashPTR cv, void* key)
{
  uint idx = CONT_HASH_FUNC(cv,key);
  lxContPtrHashEntry_t* entry = cv->table[idx];
  lxContPtrHashEntry_t** lastentry = &cv->table[idx];

  while (entry){
    if (entry->key == key){
      *lastentry = entry->next;
      lxMemoryPool_freeItem(&cv->mempool,entry);
      return LUX_TRUE;
    }
    lastentry = &entry->next;
    entry = entry->next;
  }


  return LUX_FALSE;
}
LUX_API booln lxContPtrHash_get(lxContPtrHashCPTR cv, void* key, void** outval)
{
  uint idx = CONT_HASH_FUNC(cv,key);
  booln valueSize = cv->valueSize;
  lxContPtrHashEntry_t* entry = cv->table[idx];

  while (entry){
    if (entry->key == key){
      *outval = valueSize ? (void*)entry->bytes : entry->data;
      return LUX_TRUE;
    }
    entry = entry->next;
  }

  return LUX_FALSE;
}

LUX_API booln lxContPtrHash_isEmpty(lxContPtrHashCPTR cv)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  uint idx;
  for (idx = 0; idx < tabsize; idx++){
    if (cv->table[idx]) return LUX_FALSE;
  }
  return LUX_TRUE;
}

LUX_API void  lxContPtrHash_iterate(lxContPtrHashPTR cv, lxContPtrHash_Iterator_fn *itfunc, void *fnData)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  booln valueSize = cv->valueSize;
  uint idx;
  for (idx = 0; idx < tabsize; idx++){
    lxContPtrHashEntry_t* entry = cv->table[idx];
    while (entry){
      lxContPtrHashEntry_t* next = entry->next;
      fnData = itfunc(fnData,entry->key,valueSize ? (void*)entry->bytes : entry->data);
      entry = next;
    }
  }
}

LUX_API void lxContPtrHash_clear(lxContPtrHashPTR cv)
{
  uint tabsize = CONT_HASH_SIZE(cv);
  uint idx;
  for (idx = 0; idx < tabsize; idx++){
    lxContPtrHashEntry_t* entry = cv->table[idx];
    while (entry){
      lxContPtrHashEntry_t* next = entry->next;
      lxMemoryPool_freeItem(&cv->mempool,entry);
      entry = next;
    }
    cv->table[idx] = NULL;
  }
}

LUX_API uint lxContPtrHash_shrink(lxContPtrHashPTR cv)
{
  return lxMemoryPool_shrink(&cv->mempool);
}
