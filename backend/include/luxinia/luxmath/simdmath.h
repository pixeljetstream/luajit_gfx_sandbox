// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_SIMD_H__
#define __LUXMATH_SIMD_H__

#include <luxinia/luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif


//////////////////////////////////////////////////////////////////////////
// Math SIMD
//
// A few wrapper macros, that either forward to regular
// math functions, or SIMD version.
// There can be only one SIMD version active.
// Alternatively directly use the backend you want when you want to make 
// larger use of SIMD.

#if defined(LUX_SIMD_SSE)
// Only use when you are sure that pointers passed are 16-byte aligned
#include <luxinia/luxmath/xmmmath.h>
#define lxMatrix44IdentitySIMD(o)     lxMatrix44SSE_identity((__m128*)(o))
#define lxMatrix44Transpose1SIMD(o)     lxMatrix44SSE_transpose1((__m128*)(o))
#define lxMatrix44CopySIMD(o,a)       lxMatrix44SSE_copy((__m128*)(o),(__m128*)(a))
#define lxMatrix44AffineInvertSIMD(o,a)   lxMatrix44SSE_affineInvert((__m128*)(o),(__m128*)(a))
#define lxMatrix44InvertSIMD(o,a)     lxMatrix44SSE_invert((__m128*)(o),(__m128*)(a))
#define lxMatrix44TransposeSIMD(o,a)    lxMatrix44SSE_transpose((__m128*)(o),(__m128*)(a))
#define lxMatrix44Multiply1SIMD(o,a)    lxMatrix44SSE_multiply1((__m128*)(o),(__m128*)(a))
#define lxMatrix44Multiply2SIMD(a,o)    lxMatrix44SSE_multiply2((__m128*)(a),(__m128*)(o))
#define lxMatrix44MultiplySIMD(o,a,b)   lxMatrix44SSE_multiply((__m128*)(o),(__m128*)(a),(__m128*)(b))
#define lxMatrix44MultiplyFullSIMD(o,a,b) lxMatrix44SSE_multiply((__m128*)(o),(__m128*)(a),(__m128*)(b))


#define lxMatrix34IdentitySIMD(o)     lxMatrix34SSE_identity((__m128*)(o))
#define lxMatrix34CopySIMD(o,a)       lxMatrix34SSE_copy((__m128*)(o),(__m128*)(a))
#define lxMatrix34TMultiply44SIMD(o,a,b)  lxMatrix34SSE_tmultiply44((__m128*)(o),(__m128*)(a),(__m128*)(b))

#define lxVector4LerpSIMD(o,t,a,b)      *(__m128*)(o) = lxVector4SSE_lerp(t,*(__m128*)(a),*(__m128*)(b))
#define lxVector4CatmullRomSIMD(o,t,a,b,c,d)  *(__m128*)(o) = lxVector4SSE_catmullRom(t,(__m128*)(a),(__m128*)(b),(__m128*)(c),(__m128*)(d))
#else
#include <luxinia/luxmath/matrix44.h>
#define lxMatrix44IdentitySIMD      lxMatrix44Identity
#define lxMatrix44Transpose1SIMD    lxMatrix44Transpose1
#define lxMatrix44CopySIMD        lxMatrix44Copy
#define lxMatrix44AffineInvertSIMD    lxMatrix44AffineInvert
#define lxMatrix44InvertSIMD      lxMatrix44Invert
#define lxMatrix44TransposeSIMD     lxMatrix44Transpose
#define lxMatrix44Multiply1SIMD     lxMatrix44Multiply1
#define lxMatrix44Multiply2SIMD     lxMatrix44Multiply2
#define lxMatrix44MultiplySIMD      lxMatrix44Multiply
#define lxMatrix44MultiplyFullSIMD    lxMatrix44MultiplyFull

#define lxMatrix34IdentitySIMD      lxMatrix34Identity
#define lxMatrix34CopySIMD        lxMatrix34Copy
#define lxMatrix34TMultiply44SIMD   lxMatrix34TMultiply44

#define lxVector4LerpSIMD(o,t,a,b)    lxVector4Lerp
#define lxVector4CatmullRomSIMD     lxVector4CatmullRom
#endif

#ifdef __cplusplus
}
#endif

#endif
