// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_SCALARMISC_H__
#define __LUXCORE_SCALARMISC_H__

#include <luxinia/luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef union lxScalarPtr_u{
  void*     tvoid;
  float*      tfloat;
  int8*     tint8;
  uint8*      tuint8;
  int16*      tint16;
  uint16*     tuint16;
  int32*      tint32;
  uint32*     tuint32;
  float16*    tfloat16;
  double*     tfloat64;
}lxScalarPtr_t;

typedef union lxScalarVector_u{
  float tfloat[4];
  uint8 tuint8[4];
  int8  tint8[4];
  uint16  tuint16[4];
  int16 tint16[4];
  uint32  tuint32[4];
  int32 tint32[4];
}lxScalarVector_t;

size_t lxScalarType_getSize(lxScalarType_t type);

//////////////////////////////////////////////////////////////////////////
// Conversion
LUX_API void lxScalarType_toFloat(float* pout, lxScalarType_t intype, const void *pin, uint vectordim);
LUX_API void lxScalarType_fromFloat(void* pout, lxScalarType_t outtype, const float *pin, uint vectordim);

// types are interpreted as 0,1 or -1,1
LUX_API void lxScalarType_toFloatNormalized(float* pout, lxScalarType_t intype, void *pin, uint vectordim);
LUX_API void lxScalarType_fromFloatNormalized(void* pout, lxScalarType_t outtype, const float *pin, uint vectordim);
// input already is float but in original dimensions
LUX_API void lxScalarType_normalizedFloat(float* pout, lxScalarType_t intype, float *pin, uint vectordim);

// from/to 32bit natives ie float32 and int32, uint32 will be clamped to INT_MAX
LUX_API void lxScalarType_from32(lxScalarVector_t *pout, lxScalarType_t type, void *pin, uint vectordim);
LUX_API void lxScalarType_to32(lxScalarVector_t *pout, lxScalarType_t type, void *pin, uint vectordim);


//////////////////////////////////////////////////////////////////////////

LUX_API extern size_t lx_gScalarTypeSizes[LUX_SCALARS];

LUX_INLINE size_t lxScalarType_getSize(lxScalarType_t type)
{
  LUX_ASSUME(type >= 0 && type < LUX_SCALARS);
  return lx_gScalarTypeSizes[type];
}

#ifdef __cplusplus
}
#endif

#endif

