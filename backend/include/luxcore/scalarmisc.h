// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXCORE_SCALARMISC_H__
#define __LUXCORE_SCALARMISC_H__

#include <luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum lxScalarType_e{  
  // warning may not change order!!
  // interpolation is based on abs(max)
  SCALAR_FLOAT32,   // sat: [0,1]
  SCALAR_INT8,    // sat: [-127,127]
  SCALAR_UINT8,   // sat: [0,255]               
  SCALAR_INT16,   // sat: [-SHORT_SIGNEDMAX,SHORT_SIGNEDMAX]
  SCALAR_UINT16,    // sat: [0,SHORT_UNSIGNEDMAX]
  SCALAR_INT32,   // sat: same as INT16
  SCALAR_UINT32,    // sat: same as UINT16

  SCALAR_FLOAT16,   // not supported in conversion and array ops
  SCALAR_FLOAT64,
  SCALAR_ILLEGAL,
  SCALARS,
}lxScalarType_t;

#define SCALAROPS_MAX_SUPPORTED   (SCALAR_UINT32+1)

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

LUX_API extern size_t lx_gScalarTypeSizes[SCALARS];

LUX_INLINE size_t lxScalarType_getSize(lxScalarType_t type)
{
  LUX_ASSUME(type >= 0 && type < SCALARS);
  return lx_gScalarTypeSizes[type];
}

#ifdef __cplusplus
}
#endif

#endif

