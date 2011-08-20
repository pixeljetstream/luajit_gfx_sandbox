// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_CONTDEFS_H__
#define __LUXCORE_CONTDEFS_H__

#include <luxinia/luxplatform/luxplatform.h>
#include "memory_defs.h"

#ifdef __cplusplus
extern "C"{
#endif


//////////////////////////////////////////////////////////////////////////
#define CONT_HASH_PAGEBYTES 512

typedef struct lxContHashEntry_s{
  uint32          key;
  struct lxContHashEntry_s* next;
  union{
    void*       data;
    byte        bytes[sizeof(void*)];
  };
  
}lxContHashEntry_t;

typedef struct lxContHash_s{
  uint32              mask;
  uint                numBins;
  uint                valueSize;
  lxMemoryPool_t      mempool;
  lxContHashEntry_t*  table[1];
}lxContHash_t;

void lxContHash_init(lxContHash_t *cv, lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize);
void lxContHash_deinit(lxContHash_t *cv);
size_t lxContHash_sizeof(uint numBins);


//////////////////////////////////////////////////////////////////////////

typedef struct lxContPtrHashEntry_s{
  void*         key;
  struct lxContPtrHashEntry_s*  next;
  union{
    void*       data;
    byte        bytes[sizeof(void*)];
  };

}lxContPtrHashEntry_t;

typedef struct lxContPtrHash_s{
  uint32          mask;
  uint            numBins;
  uint            valueSize;
  lxMemoryPool_t  mempool;
  lxContPtrHashEntry_t* table[1];
}lxContPtrHash_t;

void lxContPtrHash_init(lxContPtrHash_t *cv, lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize);
void lxContPtrHash_deinit(lxContPtrHash_t *cv);
size_t lxContPtrHash_sizeof(uint numBins);

#ifdef __cplusplus
}
#endif

#endif
