// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_CONTHASH_H__
#define __LUXCORE_CONTHASH_H__

#include <luxinia/luxcore/contvector.h>

#ifdef __cplusplus
extern "C"{
#endif

  //////////////////////////////////////////////////////////////////////////
  // ContHash
  //  A Hash Table (with fixed bins)
  //  By default stores the pointer-value, unless valueSize
  //  is passed on creation, then the entire data from
  //  the pointer passed is copied.

  typedef struct lxContHash_s* lxContHashPTR;
  typedef const struct lxContHash_s* lxContHashCPTR;

  // if valueSize > 0 then each hash entry allocates enough space
  // and copies the value on "add", otherwise just stores the
  // pointer itself.

    // numBins must be power of 2
  LUX_API lxContHashPTR lxContHash_new(lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize);
  LUX_API void  lxContHash_delete(lxContHashPTR cv);

  // return true on overwrite
  LUX_API booln lxContHash_set(lxContHashPTR cv, uint32 key, const void *val);
  // return true on success and stores to outval
  LUX_API booln lxContHash_get(lxContHashCPTR cv, uint32 key, void** outval);

  // returns true on success
  LUX_API booln lxContHash_remove(lxContHashPTR cv, uint32 key);

  LUX_API booln lxContHash_isEmpty(lxContHashCPTR cv);

  LUX_API uint  lxContHash_getCount(lxContHashCPTR cv);
  LUX_API void* lxContHash_getNth(lxContHashCPTR cv, uint n);

  LUX_API uint32  lxContHash_getFirstKey(lxContHashCPTR cv);
  LUX_API uint32  lxContHash_getNextKey(lxContHashCPTR cv, uint32 key);

  // iterator returns fnData for next call
  typedef void* (lxContHash_Iterator_fn)(void* fnData, uint32 key, void *val);
  // you may remove the current item during iteration
  LUX_API void  lxContHash_iterate(lxContHashPTR cv, lxContHash_Iterator_fn *itfunc, void *fnData);

  LUX_API uint  lxContHash_shrink(lxContHashPTR cv);
  LUX_API float lxContHash_memRatio(lxContHashPTR cv);



  //////////////////////////////////////////////////////////////////////////
  // ContPtrHash
  //  same as above but optimized for pointer hashing

  typedef struct lxContPtrHash_s* lxContPtrHashPTR;
  typedef const struct lxContPtrHash_s* lxContPtrHashCPTR;

  // numBins must be power of 2
  LUX_API lxContPtrHashPTR  lxContPtrHash_new(lxMemoryAllocatorPTR allocator, uint numBins, uint valueSize);
  LUX_API void  lxContPtrHash_delete(lxContPtrHashPTR cv);

  // return true on overwrite
  LUX_API booln lxContPtrHash_set(lxContPtrHashPTR cv, void* key, const void *val);
  // return true on success and stores to outval
  LUX_API booln lxContPtrHash_get(lxContPtrHashCPTR cv, void* key, void** outval);
  // returns true on success
  LUX_API booln lxContPtrHash_remove(lxContPtrHashPTR cv, void* key);

  LUX_API booln lxContPtrHash_isEmpty(lxContPtrHashCPTR cv);

  // iterator returns fnData for next call
  typedef void* (lxContPtrHash_Iterator_fn)(void* fnData, void* key, void *val);
  // you may remove the current item during iteration
  LUX_API void  lxContPtrHash_iterate(lxContPtrHashPTR cv, lxContPtrHash_Iterator_fn *itfunc, void *fnData);

  LUX_API uint lxContPtrHash_shrink(lxContPtrHashPTR cv);
  LUX_API void lxContPtrHash_clear(lxContPtrHashPTR cv);

#ifdef __cplusplus
}
#endif


#endif
