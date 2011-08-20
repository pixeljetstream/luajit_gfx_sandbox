// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h
//
// Forwards to code by Pierre Terdiman


#ifndef __LUXCORE_SORTRADIX_H__
#define __LUXCORE_SORTRADIX_H__

#include <luxinia/luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif

  // returns pointer to indices1 or indices2
  // which must have "size" length
  // indices1 must be preinitialized with used indices
  // NOT THREADSAFE!!
LUX_API uint32* lxSortRadixArrayInt(const uint32 *data, uint size, booln sign, uint32 *indices1, uint32 *indices2);
LUX_API uint32* lxSortRadixArrayFloat(const float *data, uint size, uint32 *indices1, uint32 *indices2);

#ifdef __cplusplus
}
#endif

#endif

