// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_MEMORYPOOL_H__
#define __LUXCORE_MEMORYPOOL_H__

#include "memorybase.h"

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// MemoryPool
/**
* Allows to allocate a page of data which is parted
* into equal sized items. You can alloc only one item per time. Usefull for data
* structures with constant size like listnodes. There is no guarantee that
* two allocated variables will be adjacent in the memory.
* itemsize must be >= sizeof(void*).
* Additional pages may be allocated depending on setting.
*/

typedef struct lxMemoryPool_s* lxMemoryPoolPTR;
typedef const struct lxMemoryPool_s* lxMemoryPoolCPTR;

  // actual pageValues is at least 1 less, due to pool management costs
LUX_API lxMemoryPoolPTR lxMemoryPool_new (lxMemoryAllocatorPTR allocator, uint varSize, uint pageValues, uint alignSize, booln allowMultiPages);
LUX_API void lxMemoryPool_delete (lxMemoryPoolPTR mem);

LUX_API void* lxMemoryPool_allocItem (lxMemoryPoolPTR mem);
LUX_API void lxMemoryPool_freeItem (lxMemoryPoolPTR mem, void *ptr);
LUX_API uint lxMemoryPool_shrink(lxMemoryPoolPTR mem);

LUX_API uint lxMemoryPool_memUsed(lxMemoryPoolPTR mem);
LUX_API uint lxMemoryPool_memAllocated(lxMemoryPoolPTR mem);
LUX_API float lxMemoryPool_memRatio(lxMemoryPoolPTR mem);

LUX_API uint lxMemoryPool_varSize(lxMemoryPoolCPTR mem);
LUX_API uint lxMemoryPool_alignSize(lxMemoryPoolCPTR mem);


#ifdef __cplusplus
}
#endif


#endif
