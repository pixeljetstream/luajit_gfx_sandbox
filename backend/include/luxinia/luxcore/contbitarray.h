// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_BITARRAY_H__
#define __LUXCORE_BITARRAY_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <luxinia/luxplatform/debug.h>
#include <memory.h>

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// BitArray

typedef struct lxBitArray_s{
  uint32* bits;
  uint32  num32;
}lxBitArray_t;

// if out ouf range assert is thrown
LUX_API void  lxBitArray_set(lxBitArray_t *ba, uint bit, booln state);
LUX_API booln lxBitArray_get(const lxBitArray_t *ba, uint bit);
// sets all to 0
LUX_API void  lxBitArray_clear(lxBitArray_t *ba);
// sets all to 1
LUX_API void  lxBitArray_all(lxBitArray_t *ba);
// is any bit set
LUX_API booln lxBitArray_any(const lxBitArray_t *ba);
// index of first item with state, -1 returned 
LUX_API int32 lxBitArray_getFirst(const lxBitArray_t *ba, booln state);

//////////////////////////////////////////////////////////////////////////


LUX_INLINE void lxBitArray_set(lxBitArray_t *ba, uint bit, booln state)
{
  LUX_DEBUGASSERT(bit < ba->num32*32);
  if (state){
    ba->bits[bit/32] |= 1<<(bit%32);
  }
  else{
    ba->bits[bit/32] &= ~(1<<(bit%32));
  }
}

LUX_INLINE booln  lxBitArray_get(const lxBitArray_t *ba, uint bit)
{
  LUX_DEBUGASSERT(bit < ba->num32*32);
  return (ba->bits[bit/32] & 1<<(bit%32))!= 0;
}

LUX_INLINE void lxBitArray_clear(lxBitArray_t *ba)
{
  memset(ba->bits,0,sizeof(uint32)*ba->num32);
}
LUX_INLINE void lxBitArray_all(lxBitArray_t *ba)
{
  memset(ba->bits,0xFFFFFFFF,sizeof(uint32)*ba->num32);
}
LUX_INLINE booln  lxBitArray_any(const lxBitArray_t *ba)
{
  const uint32 *start = ba->bits;
  const uint32 *end = start + ba->num32;

  while (start < end){
    if (*start)
      return LUX_TRUE;
    start++;
  }
  return LUX_FALSE;
}


#ifdef __cplusplus
};  
#endif

#endif
