// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/memorystack.h>
#include <luxinia/luxplatform/debug.h>

#include <string.h>

#include "memory_defs.h"

//////////////////////////////////////////////////////////////////////////
// MemoryStack

LUX_API lxMemoryStackPTR lxMemoryStack_new(lxMemoryAllocatorPTR allocator, const char* name, size_t totalbytes)
{
  lxMemoryStackPTR mem;

  //if (l_poolMutex == -1) l_poolMutex = critical_create();

  mem = (lxMemoryStackPTR )lxMemoryAllocator_malloc(allocator,sizeof(lxMemoryStack_t)+strlen(name)+1);
  memset(mem,0,sizeof(lxMemoryStack_t));
  mem->allocator = allocator;

  mem->name = (const char*)(mem+1);
  strcpy((char*)mem->name,name);

  lxMemoryStack_initMin(mem,totalbytes);

  return mem;
}
LUX_API void* lxMemoryStack_zalloc(lxMemoryStackPTR mem, size_t size)
{
  byte *pMem;
  // always pad size to 4-byte alignment
#ifdef LUX_MEMORY_STACK_PAD
  size_t padsize = size%LUX_MEMORY_STACK_PAD;
  size += padsize ? LUX_MEMORY_STACK_PAD-padsize : 0;
#endif

  if (size+mem->inuse > mem->total){
    LUX_PRINTF("ERROR: MemPoolMalloc failed - %s - size: %d inuse: %d total: %d\n",mem->name,size,mem->inuse,mem->total);
    LUX_ASSERT(0);
    return NULL;
  }
  else{
    pMem = &mem->data[mem->inuse];
    mem->inuse += size;
    memset(pMem,0,size);
    return pMem;
  }
}
LUX_API void* lxMemoryStack_zallocAligned(lxMemoryStackPTR mem, size_t size, size_t align)
{
  byte *pMem;

  size_t padsize;
  size_t offset;

  // shift mem->inuse
  offset = ((size_t)(&mem->data[mem->inuse]))%align;
  offset = offset ? align-offset : 0;
  mem->inuse += offset;

  // add pad to 4
#ifdef LUX_MEMORY_STACK_PAD
  padsize = size%LUX_MEMORY_STACK_PAD;
  size += padsize ? LUX_MEMORY_STACK_PAD-padsize : 0;
#endif

  if (size+mem->inuse > mem->total){
    LUX_PRINTF("ERROR: MemPoolMalloc failed - %s - size: %d inuse: %d total: %d\n",mem->name,size,mem->inuse,mem->total);
    LUX_ASSERT(0);
    return NULL;
  }
  else{
    pMem = &mem->data[mem->inuse];
    mem->inuse += size;
    memset(pMem,0,size);
    return pMem;
  }
}
LUX_API void* lxMemoryStack_alloc(lxMemoryStackPTR mem, size_t size)
{
  byte *pMem;

  if (size+mem->inuse > mem->total){
    LUX_PRINTF("ERROR: MemPoolMalloc failed - %s - size: %d inuse: %d total: %d\n",mem->name,size,mem->inuse,mem->total);
    LUX_ASSERT(0);
    return NULL;
  }
  else{
    pMem = &mem->data[mem->inuse];
    mem->inuse += size;
    return pMem;
  }
}
LUX_API void* lxMemoryStack_current(lxMemoryStackPTR mem)
{
  void *ptr;

  ptr = &mem->data[mem->inuse];

  return ptr;
}
// returns true if ptr is inside pool
LUX_API booln  lxMemoryStack_checkPtr(lxMemoryStackPTR mem, void *ptr)
{
  if ((byte*)ptr >= mem->data && (byte*)ptr < mem->data+mem->total)
    return LUX_TRUE;
  LUX_PRINTF("ERROR: MemPoolPtr failed - %s - ptr: %p inuse: %d total: %d\n",mem->name,ptr,mem->inuse,mem->total);
  LUX_ASSERT(0);
  return LUX_FALSE;
}

// from this point on new allocs will be done
// if outside pool false is returned
LUX_API void lxMemoryStack_setBegin(lxMemoryStackPTR mem, void *ptr){

  if (!lxMemoryStack_checkPtr(mem,ptr)){
    LUX_PRINTF("ERROR: MemPoolSetBegin failed - %s - ptr: %d inuse: %d total: %d\n",mem->name,ptr,mem->inuse,mem->total);
    LUX_ASSERT(0);
    return;
  }

  mem->inuse = (int)((byte*)ptr - mem->data);
}
LUX_API booln lxMemoryStack_initMin(lxMemoryStackPTR mem, size_t totalbytes)
{
  byte *olddata = mem->dataorig;

  if (mem->dataorig){
    if (totalbytes > mem->total){
      mem->dataorig = (byte*)lxMemoryAllocator_realloc(mem->allocator,mem->data, totalbytes + LUX_MEMORY_STACK_PAD,mem->total + LUX_MEMORY_STACK_PAD);
      mem->data = (byte*)lxPointerAlign(mem->dataorig,LUX_MEMORY_STACK_PAD);
      mem->total = totalbytes;
    }
  }
  else{
    mem->dataorig = (byte*)lxMemoryAllocator_malloc(mem->allocator,totalbytes + LUX_MEMORY_STACK_PAD);
    mem->data = (byte*)lxPointerAlign(mem->dataorig,LUX_MEMORY_STACK_PAD);
    mem->total = totalbytes;
  }

  mem->inuse = 0;
  mem->resized = (olddata != mem->dataorig);

  return mem->resized;
}

LUX_API int  lxMemoryStack_popResized(lxMemoryStackPTR mem)
{
  int resized = mem->resized;
  mem->resized = LUX_FALSE;

  return resized;
}

LUX_API void lxMemoryStack_clear(lxMemoryStackPTR mem)
{
  mem->inuse = 0;
}

void lxMemoryStack_deinit(lxMemoryStackPTR mem)
{
  if (mem->dataorig != NULL)
    lxMemoryAllocator_free(mem->allocator,mem->dataorig,mem->total + LUX_MEMORY_STACK_PAD);

  mem->data = NULL;
  mem->inuse = 0;
  mem->total = 0;
}

LUX_API void  lxMemoryStack_delete(lxMemoryStackPTR mem)
{
  lxMemoryStack_deinit(mem);
  lxMemoryAllocator_free(mem->allocator,mem,sizeof(lxMemoryStack_t)+strlen(mem->name)+1);
}

LUX_API size_t  lxMemoryStack_bytesLeft(lxMemoryStackPTR mem)
{
  return (mem->total-mem->inuse);
}
LUX_API size_t lxMemoryStack_bytesUsed(lxMemoryStackPTR mem)
{
  return (mem->inuse);
}
LUX_API size_t lxMemoryStack_bytesTotal(lxMemoryStackPTR mem)
{
  return (mem->total);
}
