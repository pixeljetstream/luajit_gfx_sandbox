// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/contstringmap.h>
#include <luxinia/luxcore/contmacrolinkedlist.h>

#include <string.h>

#include <luxinia/luxcore/conthash.h>
#include <luxinia/luxcore/memorylist.h>
#include <luxinia/luxcore/strmisc.h>

#include "cont_defs.h"

//////////////////////////////////////////////////////////////////////////
// CharStrDict
//  uses a hash to manage a reference counted map of strings
//  MemoryList used for allocations

typedef struct lxStrDictEntry_s{
  char*     str;
  int       refcnt;
}lxStrDictEntry_t;

typedef struct lxStrDict_s{
  lxMemoryList_t  memstr;
  lxContHash_t    hashtable;
}lxStrDict_t;

LUX_INLINE size_t lxStrDict_sizeof(uint bins)
{
  return (sizeof(lxStrDict_t)+lxContHash_sizeof(bins)-sizeof(lxContHash_t));
}

LUX_API lxStrDictPTR  lxStrDict_new(lxMemoryAllocatorPTR allocator, uint numBins)
{
  uint sizes[9] = {4,8,16,24,32,48,64,128,256};
  lxStrDict_t* self = (lxStrDict_t*)lxMemoryAllocator_malloc(allocator,lxStrDict_sizeof(numBins));

  lxContHash_init(&self->hashtable,allocator,numBins,sizeof(lxStrDictEntry_t));
  lxMemoryList_init(&self->memstr,allocator,sizeof(sizes)/sizeof(sizes[0]),sizes,512,0);

  return self;
}

LUX_API void  lxStrDict_delete(lxStrDictPTR self)
{
  lxContHash_deinit(&self->hashtable);
  lxMemoryList_deinit(&self->memstr);
  lxMemoryAllocator_free(self->memstr.allocator,self,lxStrDict_sizeof(self->hashtable.numBins));
}

typedef struct lxStrHashLen_s{
  uint32    key;
  uint      len;
}lxStrHashLen_t;

LUX_INLINE lxStrHashLen_t lxStrHashLen(const char *str)
{
  lxStrHashLen_t  ret = {0,0};
  const uchar* p = (const uchar*)str;
  uint32 c;

  while (c = *p++){
    ret.key = c + (ret.key << 6) + (ret.key << 16) - ret.key;
    ret.len++;
  }

  return ret;
}

LUX_API lxStrDictKey    lxStrDict_add(lxStrDictPTR self, const char *str)
{
  lxStrDictEntry_t* entry;
  lxStrHashLen_t  hashlen = lxStrHashLen(str);

  if (lxContHash_get(&self->hashtable,hashlen.key,(void**)&entry)){
    LUX_DEBUGASSERT(strcmp(entry->str,str)==0);
    entry->refcnt++;
  }
  else{
    lxStrDictEntry_t  nentry;
    nentry.refcnt = 1;
    nentry.str = (char*)lxMemoryList_allocItem(&self->memstr,hashlen.len+1);
    memcpy(nentry.str,str,hashlen.len);
    nentry.str[hashlen.len] = 0;
    lxContHash_set(&self->hashtable,hashlen.key,&nentry);
  }

  return hashlen.key;
}

LUX_API int   lxStrDict_rem(lxStrDictPTR self, lxStrDictName str)
{
  lxStrHashLen_t  hashlen = lxStrHashLen(str);
  lxStrDictEntry_t* entry;

  if (lxContHash_get(&self->hashtable,hashlen.key,(void**)&entry)){
    int cnt = --entry->refcnt;
    LUX_DEBUGASSERT(strcmp(entry->str,str)==0);

    if (cnt == 0){
      lxMemoryList_freeItem(&self->memstr,entry->str,hashlen.len+1);
      lxContHash_remove(&self->hashtable,hashlen.key);
    }
    return cnt;
  }
  else{
    return -1;
  }
}

LUX_API int lxStrDict_remKey(lxStrDictPTR self, lxStrDictKey key)
{
  lxStrDictEntry_t* entry;
  if (lxContHash_get(&self->hashtable,key,(void**)&entry)){
    int cnt = --entry->refcnt;
    
    if (cnt == 0){
      lxMemoryList_freeItem(&self->memstr,entry->str,strlen(entry->str)+1);
      lxContHash_remove(&self->hashtable,key);
    }
    return cnt;
  }
  else{
    return -1;
  }
}

LUX_API lxStrDictName lxStrDict_getFromKey(lxStrDictCPTR self, lxStrDictKey key)
{
  lxStrDictEntry_t* entry;
  if (lxContHash_get(&self->hashtable,key,(void**)&entry)){
    return entry->str;
  }
  return NULL;
}

LUX_API lxStrDictKey    lxStrDict_getKey(lxStrDictCPTR self, const char *str)
{
  lxStrHashLen_t  hashlen = lxStrHashLen(str);
  return hashlen.key;
}

//////////////////////////////////////////////////////////////////////////
// CharStrMap

typedef struct lxStrMap_s {
  lxStrDictPTR  dict;
  booln     owndict;
  lxContHash_t    hashtable;
} lxStrMap_t;

LUX_INLINE size_t lxStrMap_sizeof(uint numBins)
{
  return (sizeof(lxStrMap_t)+lxContHash_sizeof(numBins)-sizeof(lxContHash_t));
}

LUX_API lxStrMapPTR  lxStrMap_new(lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize, lxStrDictPTR  dict)
{
  lxStrMapPTR self = lxMemoryAllocator_malloc(allocator,lxStrMap_sizeof(numBins));
  lxContHash_init(&self->hashtable,allocator,numBins,valueSize);
  self->owndict = dict == NULL;
  self->dict = dict ? dict : lxStrDict_new(allocator,numBins);

  return self;
}



// returns 1 if something was added, or 0 when key was overwritten
LUX_API booln lxStrMap_set(lxStrMapPTR  self, const char *keystr, void *value)
{
  uint32 key = lxStrDict_add(self->dict,keystr);
  return lxContHash_set(&self->hashtable,key,value);
}

LUX_API void* lxStrMap_get(lxStrMapPTR  self, const char *keystr)
{
  uint32 key = lxStrDict_getKey(self->dict,keystr);
  void* ret;
  if(lxContHash_get(&self->hashtable,key,&ret)){
    return ret;
  }
  else{
    return NULL;
  }
}

LUX_API void* lxStrMap_getNth(lxStrMapPTR self, uint n)
{
  return lxContHash_getNth(&self->hashtable,n);
}

LUX_API uint  lxStrMap_getCount(lxStrMapPTR self)
{
  return lxContHash_getCount(&self->hashtable);
}

// returns 1 on success
LUX_API booln lxStrMap_remove(lxStrMapPTR self, const char *keystr)
{
  uint32 key = lxStrDict_getKey(self->dict,keystr);

  if (lxContHash_remove(&self->hashtable,key)){
    lxStrDict_remKey(self->dict,key);
    return LUX_TRUE;
  }
  return LUX_FALSE;
}
LUX_API booln lxStrMap_isSet(lxStrMapPTR  self, const char *keystr)
{
  uint32 key = lxStrDict_getKey(self->dict,keystr);
  void* ret;
  return lxContHash_get(&self->hashtable,key,&ret);
}

typedef struct CharStrMapFreeIt_s{
  void (*fnFree)(void*);
  lxStrDictPTR      dict;
}CharStrMapFreeIt_t;

static void* CharStrMap_deleteIterator(void *up, uint32 key, void* data)
{
  CharStrMapFreeIt_t* it = (CharStrMapFreeIt_t*)up;
  
  if (it->fnFree) it->fnFree(data);
  
  lxStrDict_remKey(it->dict,key);

  return up;
}
LUX_API void  lxStrMap_delete(lxStrMapPTR   self, void (*fnFree)(void*))
{
  CharStrMapFreeIt_t it = {fnFree,self->dict};

  lxContHash_iterate(&self->hashtable,CharStrMap_deleteIterator,&it);
  lxContHash_deinit(&self->hashtable);

  if (self->owndict){
    lxStrDict_delete(self->dict);
  }

  lxMemoryAllocator_free(self->hashtable.mempool.allocator, self, lxStrMap_sizeof(self->hashtable.numBins));
}

typedef struct CharStrMapIt_s{
  lxStrMap_Iterator_fn *itfunc;
  lxStrMapPTR     map;
  void*         up;
}CharStrMapIt_t;

static void* CharStrMap_Iterator(void* fnData, uint32 key, void *browseData)
{
  CharStrMapIt_t* it = (CharStrMapIt_t*)fnData;
  it->up = it->itfunc(it->up,lxStrDict_getFromKey(it->map->dict,key),browseData);
  return it;
}

LUX_API void  lxStrMap_iterate(lxStrMapPTR  self, lxStrMap_Iterator_fn *itfunc, void *fnData)
{
  CharStrMapIt_t it = {itfunc,self,fnData};
  lxContHash_iterate(&self->hashtable,CharStrMap_Iterator,&it);
}

