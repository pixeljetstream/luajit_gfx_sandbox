// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_MEMORYSTACK_H__
#define __LUXCORE_MEMORYSTACK_H__

#include "memorybase.h"

#ifdef __cplusplus
extern "C"{
#endif

  // will make all mempool allocs aligned
#define LUX_MEMORY_STACK_PAD    4

  //////////////////////////////////////////////////////////////////////////
  // MemoryStack
  //
  // Hunks big chunks that are filled linearly
  // Not able to free parts of it, just the complete chunk
  // NOT THREADSAFE!!

  typedef struct lxMemoryStack_s* lxMemoryStackPTR;


  LUX_API lxMemoryStackPTR  lxMemoryStack_new(lxMemoryAllocatorPTR allocator, const char* name, size_t totalbytes);
  LUX_API void      lxMemoryStack_delete(lxMemoryStackPTR mem);

  // performs memset 0
  LUX_API void* lxMemoryStack_zalloc(lxMemoryStackPTR mem,size_t size);   
  // allocs aligned + memset 0
  LUX_API void* lxMemoryStack_zallocAligned(lxMemoryStackPTR  mem,size_t size,size_t align);
  // performs no memset 0
  LUX_API void* lxMemoryStack_alloc(lxMemoryStackPTR  mem, size_t size);
  // returns current inuse pointer
  LUX_API void* lxMemoryStack_current(lxMemoryStackPTR  mem);

  // inits to size, or resizes if current is smaller
  // returns TRUE if resize/new alloc happened
  // also sets resized flag to true
  LUX_API booln lxMemoryStack_initMin(lxMemoryStackPTR  mem, size_t totalbytes);  

  // returns resized flag and clears it
  LUX_API int   lxMemoryStack_popResized(lxMemoryStackPTR mem);
  //    sets used megs back to 0
  LUX_API void  lxMemoryStack_clear(lxMemoryStackPTR  mem);
  // frees pool 
  LUX_API void  lxMemoryStack_deinit(lxMemoryStackPTR mem);
  // returns true if ptr is inside pool, otherwise assert
  LUX_API booln  lxMemoryStack_checkPtr(lxMemoryStackPTR  mem, void *ptr);
  // from this adress on new allocs will be done
  LUX_API void  lxMemoryStack_setBegin(lxMemoryStackPTR mem, void *ptr);
  LUX_API size_t  lxMemoryStack_bytesLeft(lxMemoryStackPTR  mem);
  LUX_API size_t  lxMemoryStack_bytesUsed(lxMemoryStackPTR  mem);
  LUX_API size_t  lxMemoryStack_bytesTotal(lxMemoryStackPTR mem);


#ifdef __cplusplus
};  
#endif

#endif
