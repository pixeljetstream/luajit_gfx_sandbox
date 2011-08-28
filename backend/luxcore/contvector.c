// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/contvector.h>
#include <string.h>

#define LUX_CONTVECTOR_SMALL  (8)

LUX_API void  lxContVector_reserve(lxContVectorPTR cv,uint n)
{
  const uint 
    e = cv->elemsize,
    s = lxContVector_size(cv),
    c = lxContVector_capacity(cv);

  if (c >= n) return;
  if (c == 0)
  {
    if (cv->alignsize){
      cv->beg = (byte*)lxMemoryAllocator_mallocAligned(cv->allocator, e * n, cv->alignsize);
    }
    else{
      cv->beg = (byte*)lxMemoryAllocator_malloc(cv->allocator, e * n);
    }
  }
  else
  {
    if (cv->alignsize){
      cv->beg = (byte*)lxMemoryAllocator_reallocAligned(cv->allocator, cv->beg, n * e,  cv->alignsize, c * e);
    }
    else{
      cv->beg = (byte*)lxMemoryAllocator_realloc(cv->allocator, cv->beg, e * n, c * e);
    }
  }
  cv->end = cv->beg + s*e;
  cv->eos = cv->beg + n*e;
  LUX_DEBUGASSERT(lxContVector_capacity(cv) >= n);
  LUX_DEBUGASSERT(lxContVector_size(cv) == s);
}

LUX_API void lxContVector_prepGrowth(lxContVectorPTR cv, uint delta)
{
  const uint s = lxContVector_size(cv);
  const uint smallThreshold = LUX_CONTVECTOR_SMALL;
  if (s < smallThreshold)
  {
    lxContVector_reserve(cv,LUX_MAX(smallThreshold, delta));
  }
  else
  {
    const uint multiply = 3;
    const uint divide = 2;
    const uint suggestedSize = (s * multiply) / divide;
    lxContVector_reserve(cv,LUX_MAX(s + delta, suggestedSize));
  }
}

LUX_API lxContVector_t lxContVector_copy(lxContVectorPTR cv)
{
  lxContVector_t  ncv = lxContVector_new(cv->elemsize,cv->allocator);
  lxContVector_reserve(&ncv,lxContVector_size(cv));
  memcpy(ncv.beg, cv->beg, lxContVector_memused(cv));

  return ncv;
}

LUX_API void  lxContVector_clear(lxContVectorPTR cv){
  if(!lxContVector_isEmpty(cv)){
    if (cv->alignsize){
      lxMemoryAllocator_freeAligned(cv->allocator,cv->beg,lxContVector_size(cv)*cv->elemsize);
    }
    else{
      lxMemoryAllocator_free(cv->allocator,cv->beg,lxContVector_size(cv)*cv->elemsize);
    }
    cv->end = cv->beg;
    cv->eos = cv->beg;
  }
}

LUX_API LUX_INLINE void lxContVector_insertMany(lxContVectorPTR cv, uint idx, const void *data, uint cnt)
{
  const uint 
    e = cv->elemsize,
    s = lxContVector_size(cv),
    c = lxContVector_capacity(cv);

  const uint
    bytes = e*cnt;

  if (s+cnt > c) 
  {
    lxContVector_prepGrowth(cv,cnt);
  }

  {
    byte* idxpos = (byte*)lxContVector_at(cv,idx);

    if (idx < s){
      memmove(idxpos+bytes,idxpos,(s-idx)*e);
    }
    memcpy(idxpos,data,bytes);
    cv->end += (e*cnt);
  }
}

LUX_API void  lxContVector_insert(lxContVectorPTR cv, uint idx, const void *data)
{
  lxContVector_insertMany(cv,idx,data,1);
}

LUX_API void  lxContVector_insertRepeat(lxContVectorPTR cv, uint idx, void *data, uint cnt)
{
  const uint 
    e = cv->elemsize,
    s = lxContVector_size(cv),
    c = lxContVector_capacity(cv);

  const uint
    bytes = e*cnt;

  if (s+cnt > c) 
  {
    lxContVector_prepGrowth(cv,cnt);
  }

  {
    byte* idxpos = (byte*)lxContVector_at(cv,idx);
    byte* endpos = idxpos+bytes;
    
    if (idx < s){
      memmove(idxpos+bytes,idxpos,(s-idx)*e);
    }
    for (; idxpos < endpos; idxpos += e){
      memcpy(idxpos,data,e);
    }
    
    cv->end = idxpos;
  }
}

LUX_API LUX_INLINE void lxContVector_removeMany(lxContVectorPTR cv, uint idx, uint cnt)
{
  const uint 
    e = cv->elemsize,
    s = lxContVector_size(cv),
    c = lxContVector_capacity(cv);

  const uint
    ncnt = LUX_MIN(cnt,(s-idx));

  const uint
    bytes = e*ncnt;

  byte* idxpos = (byte*)lxContVector_at(cv,idx);
  byte* frompos = idxpos+bytes;

  if (frompos < cv->end){
    memmove(idxpos,frompos,bytes);
  }

  cv->end -= bytes;
}

LUX_API void  lxContVector_remove(lxContVectorPTR cv, uint idx)
{
  lxContVector_removeMany(cv,idx,1);
}

LUX_API void  lxContVector_shrink(lxContVectorPTR cv)
{
  const uint 
    e = cv->elemsize,
    s = lxContVector_size(cv),
    c = lxContVector_capacity(cv);

  if (s == 0) return;

  if (cv->alignsize){
    cv->beg = (byte*)lxMemoryAllocator_reallocAligned(cv->allocator, cv->beg, s*e, cv->alignsize, c*e);
  }
  else{
    cv->beg = (byte*)lxMemoryAllocator_realloc(cv->allocator, cv->beg,s * e, c*e);
  }

  cv->end = cv->beg + s*e;
  cv->eos = cv->end;
  LUX_DEBUGASSERT(lxContVector_capacity(cv) >= s);
  LUX_DEBUGASSERT(lxContVector_size(cv) == s);
}

LUX_API void  lxContVector_resize(lxContVectorPTR cv,uint cnt, const void *fill)
{
  const uint 
    e = cv->elemsize,
    s = lxContVector_size(cv),
    c = lxContVector_capacity(cv);

  if (c == cnt) return;

  lxContVector_reserve(cv,cnt);
  if (cnt > s){
    byte* curend = cv->end;
    byte* end = cv->eos;
    const byte* LUX_RESTRICT from = (const byte*)fill;

    for (; curend < end; curend += e){
      memcpy(curend,from,e);
    }
    cv->end = end;
  }
}
