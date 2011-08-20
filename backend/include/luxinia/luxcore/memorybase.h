// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_MEMORYBASE_H__
#define __LUXCORE_MEMORYBASE_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////
// Switches
//
//  MEMORY_STATS


#ifdef _DEBUG
#define LUX_MEMORY_STATS
#endif

#ifdef __cplusplus
extern "C"{
#endif

#define LUX_MEMORY_MEGS(a)    (a*1048576)
#define LUX_MEMORY_KBS(a)   (a*1024)

  typedef struct lxMemoryAllocator_s* lxMemoryAllocatorPTR;

  typedef void* (__cdecl *lxMalloc_fn)(lxMemoryAllocatorPTR, size_t sz);
  typedef void* (__cdecl *lxMallocAligned_fn)(lxMemoryAllocatorPTR, size_t, size_t align);
  typedef void* (__cdecl *lxCalloc_fn)(lxMemoryAllocatorPTR, size_t sz, size_t num);
  typedef void* (__cdecl *lxCallocAligned_fn)(lxMemoryAllocatorPTR, size_t sz,size_t num, size_t align);
  typedef void* (__cdecl *lxRealloc_fn)(lxMemoryAllocatorPTR, void*, size_t sz, size_t oldsz);
  typedef void* (__cdecl *lxReallocAligned_fn)(lxMemoryAllocatorPTR, void*, size_t sz, size_t oldsz, size_t align);
  typedef void  (__cdecl *lxFree_fn)(lxMemoryAllocatorPTR, void*, size_t oldsz);
  typedef void  (__cdecl *lxFreeAligned_fn)(lxMemoryAllocatorPTR, void*, size_t oldsz);

  typedef void* (__cdecl *lxMallocStats_fn)(lxMemoryAllocatorPTR, size_t sz, const char *file, int line);
  typedef void* (__cdecl *lxMallocAlignedStats_fn)(lxMemoryAllocatorPTR, size_t, size_t align, const char *file, int line);
  typedef void* (__cdecl *lxCallocStats_fn)(lxMemoryAllocatorPTR, size_t sz,size_t num, const char *file, int line);
  typedef void* (__cdecl *lxCallocAlignedStats_fn)(lxMemoryAllocatorPTR, size_t sz,size_t num, size_t align, const char *file, int line);
  typedef void* (__cdecl *lxReallocStats_fn)(lxMemoryAllocatorPTR, void*, size_t sz, size_t oldsz, const char *file, int line);
  typedef void* (__cdecl *lxReallocAlignedStats_fn)(lxMemoryAllocatorPTR, void*, size_t sz, size_t oldsz, size_t align, const char *file, int line);
  typedef void  (__cdecl *lxFreeStats_fn)(lxMemoryAllocatorPTR, void*, size_t oldsz, const char *file, int line);
  typedef void  (__cdecl *lxFreeAlignedStats_fn)(lxMemoryAllocatorPTR, void*, size_t oldsz, const char *file, int line);

  typedef struct lxMemoryTracker_s{
    lxMallocStats_fn          _malloc;
    lxCallocStats_fn          _calloc;
    lxReallocStats_fn         _realloc;
    lxFreeStats_fn            _free;
    lxMallocAlignedStats_fn   _mallocAligned;
    lxCallocAlignedStats_fn   _callocAligned;
    lxReallocAlignedStats_fn  _reallocAligned;
    lxFreeAlignedStats_fn     _freeAligned;
  }lxMemoryTracker_t;
  
  typedef struct lxMemoryAllocator_s{
    lxMalloc_fn           _malloc;
    lxCalloc_fn           _calloc;
    lxRealloc_fn          _realloc;
    lxFree_fn             _free;
    lxMallocAligned_fn    _mallocAligned;
    lxCallocAligned_fn    _callocAligned;
    lxReallocAligned_fn   _reallocAligned;
    lxFreeAligned_fn      _freeAligned;
    lxMemoryTracker_t*    tracker;
  }lxMemoryAllocator_t;

#ifndef LUX_MEMORY_STATS
  void* lxMemoryAllocator_malloc(lxMemoryAllocatorPTR alloc, size_t sz);
  void* lxMemoryAllocator_mallocAligned(lxMemoryAllocatorPTR alloc, size_t sz, size_t align);
  void* lxMemoryAllocator_calloc(lxMemoryAllocatorPTR alloc, size_t sz, size_t num);
  void* lxMemoryAllocator_callocAligned(lxMemoryAllocatorPTR alloc, size_t sz, size_t num, size_t align);
  void* lxMemoryAllocator_realloc(lxMemoryAllocatorPTR alloc, void*ptr, size_t sz, size_t oldsz);
  void* lxMemoryAllocator_reallocAligned(lxMemoryAllocatorPTR alloc, void*ptr, size_t sz, size_t oldsz, size_t align);
  void lxMemoryAllocator_free(lxMemoryAllocatorPTR alloc, void* ptr, size_t oldsz);
  void lxMemoryAllocator_freeAligned(lxMemoryAllocatorPTR alloc, void* ptr, size_t oldsz);
#endif

//////////////////////////////////////////////////////////////////////////
// Aligned alloc & free

void  lxMemAlignedRealloc(void* reallocated, size_t alignsize, 
                size_t newsize, size_t info, size_t shiftold);

void* lxMemAlignedToOrig(void *ptr);
void* lxMemAlignedFromOrig(void *ptr, size_t align_size);


//////////////////////////////////////////////////////////////////////////
// Pointer

// advances a pointer for continous allocation, assumes all allocations
// fit into memory block
void* lxPointerAdvanceAlloc(void **ptr, size_t size);
void* lxPointerAdvanceAllocAligned(void **ptr, size_t size, size_t align);

// aligns must be power of 2
void* lxPointerAlign(void *ptr, size_t align);
size_t  lxSizeAlign(size_t val, size_t align);

//////////////////////////////////////////////////////////////////////////
#ifdef LUX_MEMORY_STATS
#define lxMemoryAllocator_malloc(a,s)                 (a->tracker->_malloc((a),(s),__FILE__,__LINE__))
#define lxMemoryAllocator_mallocAligned(a,s,al)       (a->tracker->_mallocAligned((a),(s),(al),__FILE__,__LINE__))
#define lxMemoryAllocator_calloc(a,e,s)               (a->tracker->_calloc((a),(e),(s),__FILE__,__LINE__))
#define lxMemoryAllocator_callocAligned(a,e,s,al)     (a->tracker->_callocAligned((a),(e),(s),(al),__FILE__,__LINE__))
#define lxMemoryAllocator_realloc(a,p,s,o)            (a->tracker->_realloc((a),(p),(s),(o),__FILE__,__LINE__))
#define lxMemoryAllocator_reallocAligned(a,p,s,o,al)  (a->tracker->_reallocAligned((a),(p),(s),(o),(al),__FILE__,__LINE__))
#define lxMemoryAllocator_free(a,p,o)                 (a->tracker->_free((a),(p),(o),__FILE__,__LINE__))
#define lxMemoryAllocator_freeAligned(a,p,o)          (a->tracker->_freeAligned((a),(p),(o),__FILE__,__LINE__))
#else
LUX_INLINE void* lxMemoryAllocator_malloc(lxMemoryAllocatorPTR alloc, size_t sz){
  return alloc->_malloc(alloc,sz);
}
LUX_INLINE void* lxMemoryAllocator_mallocAligned(lxMemoryAllocatorPTR alloc, size_t sz, size_t align){
  return alloc->_mallocAligned(alloc,sz,align);
}
LUX_INLINE void* lxMemoryAllocator_calloc(lxMemoryAllocatorPTR alloc, size_t sz, size_t num){
  return alloc->_calloc(alloc,sz,num);
}
LUX_INLINE void* lxMemoryAllocator_callocAligned(lxMemoryAllocatorPTR alloc, size_t sz, size_t num, size_t align){
  return alloc->_callocAligned(alloc,sz,num,align);
}
LUX_INLINE void* lxMemoryAllocator_realloc(lxMemoryAllocatorPTR alloc, void* ptr, size_t sz, size_t oldsz){
  return alloc->_realloc(alloc,ptr,sz,oldsz);
}
LUX_INLINE void* lxMemoryAllocator_reallocAligned(lxMemoryAllocatorPTR alloc, void*ptr, size_t sz, size_t oldsz, size_t align){
  return alloc->_reallocAligned(alloc,ptr,sz,oldsz,align);
}
LUX_INLINE void lxMemoryAllocator_free(lxMemoryAllocatorPTR alloc, void* ptr, size_t oldsz){
  alloc->_free(alloc,ptr,oldsz);
}
LUX_INLINE void lxMemoryAllocator_freeAligned(lxMemoryAllocatorPTR alloc, void* ptr, size_t oldsz){
  alloc->_freeAligned(alloc,ptr,oldsz);
}
#endif

LUX_INLINE void*  lxPointerAlign(void *ptr, size_t align){
  align = align-1;
  return (void*) ((((size_t)ptr) + align) & ~align);
}
LUX_INLINE size_t lxSizeAlign(size_t ptr, size_t align){
  align = align-1;
  return ((((size_t)ptr) + align) & ~align);
}

LUX_INLINE void*  lxPointerAdvanceAlloc(void **ptr, size_t size)
{
  uchar *inptr = *(uchar**)ptr;
  *ptr = inptr + size;

  return inptr;
}

LUX_INLINE void*  lxPointerAdvanceAllocAligned(void **ptr, size_t size, size_t align)
{
  uchar *inptr = *(uchar**)ptr;
  size_t offset;

  // shift mem->inuse
  offset = ((size_t)inptr)%align;
  offset = offset ? align-offset : 0;
  inptr += offset;

  *ptr = inptr + size;

  return inptr;
}

LUX_INLINE void*  lxMemAlignedToOrig(void *ptr){
  char *ptrb = (char*)ptr;
  size_t *ptr2=(size_t *)ptr - 1;
  ptrb -= *ptr2;

  return ptrb;
}

LUX_INLINE void*  lxMemAlignedFromOrig(void* optr, size_t align_size)
{
  size_t align_mask = align_size - 1;
  char *ptr = (char*)optr;
  char *ptr2 = ptr + sizeof(size_t);
  char *aligned_ptr = ptr2 + (align_size - ((size_t)ptr2 & align_mask));


  ptr2 = aligned_ptr - sizeof(size_t);
  *((size_t *)ptr2)=(size_t)(aligned_ptr - ptr);

  return (void*)aligned_ptr;
}

LUX_INLINE void lxMemAlignedRealloc(void* reallocated, size_t alignsize, 
                    size_t newsize, size_t info, size_t shiftold)
{
  byte* oldptr = (byte*)reallocated;
  byte* ptr = (byte*)lxPointerAlign(oldptr+info,alignsize);
  size_t shiftnew = (ptr-oldptr);
  ptrdiff_t diff = shiftold-shiftnew;

  if (diff == 0){
    return;
  }
  memmove(ptr,oldptr+(shiftold+info),newsize);
}

#ifdef __cplusplus
};  
#endif

#endif


