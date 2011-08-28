// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h
//
// modelled after yasli::vector (C) Andrei Alexandrescu
// http://www.moderncppdesign.com/


#ifndef __LUXCORE_CONTVECTOR_H__
#define __LUXCORE_CONTVECTOR_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <luxinia/luxplatform/debug.h>
#include <luxinia/luxcore/memorybase.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

  //////////////////////////////////////////////////////////////////////////
  // ContVector
  //  a simple vector implementation (continous memory)
  //  grows allocation as necessary

  typedef struct lxContVector_s{
    ushort    elemsize;
    ushort    alignsize;
    
    byte*   beg;
    byte*   end;
    byte*   eos;
    lxMemoryAllocatorPTR allocator;
  }lxContVector_t;
  typedef struct lxContVector_s* lxContVectorPTR;
  typedef const struct lxContVector_s* lxContVectorCPTR;

  LUX_API void lxContVector_init(lxContVectorPTR cv,lxMemoryAllocatorPTR allocator, uint elemsize);

  // alignsize must be power of 2 and should be >= 4
  // elemsize must be multiple of alignsize
  LUX_API void lxContVector_initAligned(lxContVectorPTR cv,lxMemoryAllocatorPTR allocator, uint elemsize, uint alignsize);
  LUX_API lxContVector_t lxContVector_new(uint elemsize, lxMemoryAllocatorPTR allocator);
  LUX_API lxContVector_t lxContVector_copy(lxContVectorPTR cv);

  LUX_API void* lxContVector_front(lxContVectorPTR cv);
  LUX_API void* lxContVector_back(lxContVectorPTR cv);
  LUX_API void* lxContVector_at(lxContVectorPTR cv, uint idx);
  LUX_API int   lxContVector_find(lxContVectorCPTR cv, const void *val);

  LUX_API uint  lxContVector_elemsize(lxContVectorCPTR cv);
  LUX_API uint  lxContVector_capacity(lxContVectorCPTR cv);
  LUX_API uint  lxContVector_size(lxContVectorCPTR cv);
  LUX_API uint  lxContVector_memused(lxContVectorCPTR cv);
  LUX_API uint  lxContVector_memreserved(lxContVectorCPTR cv);
  LUX_API booln lxContVector_isEmpty(lxContVectorCPTR cv);

  LUX_API void  lxContVector_pushBack(lxContVectorPTR cv, const void *data);
  LUX_API void  lxContVector_popBack(lxContVectorPTR cv);
  LUX_API void  lxContVector_clear(lxContVectorPTR cv);
  LUX_API void  lxContVector_makeEmpty(lxContVectorPTR cv);

  LUX_API void  lxContVector_insert(lxContVectorPTR cv, uint idx, const void *data);
  LUX_API void  lxContVector_insertRepeat(lxContVectorPTR cv, uint idx, const void *data, uint cnt);
  LUX_API void  lxContVector_insertMany(lxContVectorPTR cv, uint idx, const void *data, uint cnt);
  LUX_API void  lxContVector_remove(lxContVectorPTR cv, uint idx);
  LUX_API void  lxContVector_removeMany(lxContVectorPTR cv, uint idx, uint cnt);
  LUX_API void  lxContVector_removeUnsorted(lxContVectorPTR cv, uint idx);
  
  LUX_API void  lxContVector_shrink(lxContVectorPTR cv);
  LUX_API void  lxContVector_reserve(lxContVectorPTR cv,uint cnt);
  LUX_API void  lxContVector_resize(lxContVectorPTR cv,uint cnt, const void *fill);
  LUX_API void  lxContVector_prepGrowth(lxContVectorPTR cv, uint delta);


//////////////////////////////////////////////////////////////////////////

  

  LUX_INLINE void lxContVector_init(lxContVectorPTR cv, lxMemoryAllocatorPTR allocator, uint elemsize){
    cv->elemsize = elemsize;
    cv->alignsize = 0;
    cv->end = cv->beg;
    cv->eos = cv->beg;
    cv->allocator = allocator;
  }
  LUX_INLINE void lxContVector_initAligned(lxContVectorPTR cv, lxMemoryAllocatorPTR allocator, uint elemsize, uint alignsize){
    cv->elemsize = elemsize;
    cv->alignsize = alignsize;
    cv->end = cv->beg;
    cv->eos = cv->beg;
    cv->allocator = allocator;
  }
  LUX_INLINE lxContVector_t lxContVector_new(uint elemsize, lxMemoryAllocatorPTR allocator){
    lxContVector_t  cv;
    cv.elemsize = elemsize;
#ifdef _DEBUG
    cv.beg = NULL;
    cv.end = NULL;
    cv.eos = NULL;
#else
    cv.end = cv.beg;
    cv.eos = cv.beg;
#endif
    cv.allocator = allocator;
    return cv;
  }

  LUX_INLINE uint lxContVector_elemsize(lxContVectorCPTR cv){
    return cv->elemsize;
  }
  LUX_INLINE uint lxContVector_memused(lxContVectorCPTR cv){
    return (uint)(cv->end-cv->beg);
  }
  LUX_INLINE uint lxContVector_memreserved(lxContVectorCPTR cv){
    return (uint)(cv->eos-cv->beg);
  }
  LUX_INLINE uint lxContVector_capacityS(lxContVectorCPTR cv, uint elemsize){
    return (uint)((cv->eos-cv->beg)/elemsize);
  }
  LUX_INLINE uint lxContVector_capacity(lxContVectorCPTR cv){
    return lxContVector_capacityS(cv,(uint)cv->elemsize);
  }
  LUX_INLINE uint lxContVector_sizeS(lxContVectorCPTR cv, uint elemsize){
    return (uint)((cv->end-cv->beg)/elemsize);
  }
  LUX_INLINE uint lxContVector_size(lxContVectorCPTR cv){
    return lxContVector_sizeS(cv,cv->elemsize);
  }
  LUX_INLINE booln  lxContVector_isEmpty(lxContVectorCPTR cv){
    return cv->beg == cv->end;
  }
  LUX_INLINE void lxContVector_makeEmpty(lxContVectorPTR cv){
    cv->end = cv->beg;
  }
  LUX_INLINE void*  lxContVector_front(lxContVectorPTR cv){
    LUX_DEBUGASSERT(!lxContVector_isEmpty(cv));
    return (void*)cv->beg;
  }
  LUX_INLINE void*  lxContVector_backS(lxContVectorPTR cv, uint elemsize){
    LUX_DEBUGASSERT(!lxContVector_isEmpty(cv));
    return (void*)(cv->end-(uint)elemsize);
  }
  LUX_INLINE void*  lxContVector_back(lxContVectorPTR cv){
    return lxContVector_backS(cv,cv->elemsize);
  }
  LUX_INLINE void*  lxContVector_atS(lxContVectorPTR cv, uint elemsize, uint idx){
    LUX_DEBUGASSERT(!lxContVector_isEmpty(cv) && idx < lxContVector_sizeS(cv,elemsize));
    return (void*)(cv->beg+(elemsize*idx));
  }
  LUX_INLINE void*  lxContVector_at(lxContVectorPTR cv, uint idx){
    return lxContVector_atS(cv,cv->elemsize,idx);
  }
  LUX_INLINE int  lxContVector_findS(lxContVectorCPTR cv, uint elemsize, const void *val){
    byte* beg = cv->beg;
    while (beg < cv->end){
      if (!memcmp(val,beg,elemsize))
        return (int)((beg-cv->beg)/elemsize);
      beg += elemsize;
    }

    return -1;
  }
  LUX_INLINE int  lxContVector_find(lxContVectorCPTR cv, const void *val){
    return lxContVector_findS(cv,cv->elemsize,val);
  }
  
  LUX_INLINE void lxContVector_pushBackS(lxContVectorPTR cv, uint elemsize, const void *data)
  {
    if (cv->end == cv->eos) 
    {
      lxContVector_prepGrowth(cv,1);
    }

    memcpy(cv->end,data,elemsize);
    cv->end += elemsize;
  }
  LUX_INLINE void lxContVector_pushBack(lxContVectorPTR cv, const void *data){
    lxContVector_pushBackS(cv,cv->elemsize,data);
  }

  LUX_INLINE void lxContVector_popBackS(lxContVectorPTR cv, uint elemsize)
  {
    LUX_DEBUGASSERT(!lxContVector_isEmpty(cv));
    cv->end-=elemsize;
  }
  LUX_INLINE void lxContVector_popBack(lxContVectorPTR cv){
    lxContVector_popBackS(cv,cv->elemsize);
  }

  LUX_INLINE void lxContVector_removeUnsortedS(lxContVectorPTR cv, uint elemsize, uint idx)
  {
    byte* idxpos = (byte*)lxContVector_atS(cv,elemsize,idx);
    byte* endpos = cv->end - elemsize;
    if (idxpos != endpos){
      memcpy(idxpos,cv->end-elemsize,elemsize);
    }

    cv->end -= elemsize;
  }
  LUX_INLINE void lxContVector_removeUnsorted(lxContVectorPTR cv, uint idx)
  {
    lxContVector_removeUnsortedS(cv,cv->elemsize,idx);
  }

#ifdef __cplusplus
}
#endif


#endif
