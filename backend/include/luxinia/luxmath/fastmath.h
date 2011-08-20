// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

// Based on Nvidia SDK
// http://www.nvidia.com/object/fast_math_routines.html


#ifndef __LUXMATH_FASTMATH_H__
#define __LUXMATH_FASTMATH_H__

#include <luxinia/luxmath/basetypes.h>


#ifdef __cplusplus
extern "C"{
#endif

#define LUX_MATH_MAX_CIRCLE_ANGLE 8192

  // tablesize (32 KB = LUX_MATH_MAX_CIRCLE_ANGLE*4)
//#define LUX_MATH_SINCOS_TABLE

  // tablesize (256 KB)
//#define LUX_MATH_SQRT_TABLE

  // not very accurate
//#define LUX_MATH_RSQRT_HACK

typedef struct lxFastMathCache_s{
  unsigned int  fast_sqrt_table[0x10000];
  float     mathcossintable[LUX_MATH_MAX_CIRCLE_ANGLE];
}lxFastMathCache_t;


// initializes lookup tables
LUX_API void lxFastMath_init(lxFastMathCache_t *cache);
// assumes lookup tables already inited
LUX_API void lxFastMath_initset(lxFastMathCache_t *cache);

LUX_API void lxFloatToInt(int *int_pointer,const float f);
LUX_API float lxModuloPositive(float in, float mod);

// may use lookup tables depending on settings above
// otherwise uses SSE versions or falls back to regular

  
LUX_API float lxFastSqrt(float n);
LUX_API float lxFastCos(const float n);
LUX_API float lxFastSin(const float n);

  // less accurate when hack is applied
LUX_API float lxFastSqrtRcp(float n);


#ifdef LUX_SIMD_SSE
  LUX_API __m128 lxFastExp_ps(__m128 x);
  LUX_API __m128 lxFastLog_ps(__m128 x);
  LUX_API __m128 lxFastCos_ps(__m128 x);
  LUX_API __m128 lxFastSin_ps(__m128 x);
  LUX_API void lxFastSinCos_ps(__m128 x,__m128 *s, __m128 *c);
#endif

LUX_API int   lxFastIntRound(float f);
LUX_API float lxFastFrac(float f);
LUX_API int   lxFastIntFloor(float f);

// macros
//#define LUX_FP_INV(r,p)
//#define LUX_FP_EXP(e,p)
//#define LUX_FP_NORM_TO_BYTE(i,p)

//#define LUX_FP_INTCAST(f)
//#define LUX_FP_UINTCAST(f)

//#define LUX_FP_GREATER_ZERO(f)
//#define LUX_FP_LESS_ZERO(f)
//#define LUX_FP_GEQUAL_ZERO(f)
//#define LUX_FP_LEQUAL_ZERO(f)

//#define LUX_FP_LESS_ONE(f)
//#define LUX_FP_GREATER_ONE(f)
//#define LUX_FP_LEQUAL_ONE(f)
//#define LUX_FP_GEQUAL_ONE(f)

//////////////////////////////////////////////////////////////////////////

#define LUX_FP_INTCAST(f) (*((int *)(&f)))
#define LUX_FP_UINTCAST(f)  (*((unsigned int *)(&f)))

#define LUX_FP_GEQUAL_ZERO(f) ((LUX_FP_UINTCAST(f) <= 0x80000000U))
#define LUX_FP_LESS_ZERO(f)   ((LUX_FP_UINTCAST(f) > 0x80000000U))
#define LUX_FP_GREATER_ZERO(f)  ((LUX_FP_INTCAST(f) > 0))
#define LUX_FP_LEQUAL_ZERO(f) ((LUX_FP_INTCAST(f) <= 0))


#define LUX_FP_LESS_ONE(f)    ((LUX_FP_INTCAST(f) < 0x3F800000))
#define LUX_FP_GREATER_ONE(f) ((LUX_FP_INTCAST(f) > 0x3F800000))
#define LUX_FP_LEQUAL_ONE(f)  ((LUX_FP_INTCAST(f) <= 0x3F800000))
#define LUX_FP_GEQUAL_ONE(f)  ((LUX_FP_INTCAST(f) >= 0x3F800000))

#define LUX_FP_BITS(fp) (*(uint32 *)&(fp))
#define LUX_FP_ABS_BITS(fp) (LUX_FP_BITS(fp)&0x7FFFFFFF)
#define LUX_FP_SIGN_BIT(fp) (LUX_FP_BITS(fp)&0x80000000)
#define LUX_FP_ONE_BITS 0x3F800000

#if !defined(_DEBUG) && defined(LUX_COMPILER_MSC) && defined(LUX_ARCH_X86)
LUX_INLINE void lxFloatToInt(int *int_pointer,const float f)
{
  __asm  fld  f
  __asm  mov  edx,int_pointer
  __asm  FRNDINT
  __asm  fistp dword ptr [edx];

}
#else
LUX_INLINE void lxFloatToInt(int *int_pointer,const float flt)
{
  *(int_pointer)=(int)((flt) + 0.5f);
}
#endif

LUX_INLINE float  lxModuloPositive(float in, float mod){
  return ((LUX_FP_LESS_ZERO(in)) ? ((mod) - (float)fmod(-(in),(mod)))  : (float)fmod((in),(mod)));
}

// 1/sqrt
LUX_INLINE float lxFastSqrtRcp (float x){
#ifdef LUX_MATH_RSQRT_HACK
  float xhalf = 0.5f*x;
  int i = *(int*)&x;
  i = 0x5f3759df - (i>>1);
  x = *(float*)&i;
  x = x*(1.5f - xhalf*x*x);
  return x;
#elif defined(LUX_SIMD_SSE)
  __m128 res = _mm_rsqrt_ss(_mm_set_ss(x));
  return res.m128_f32[0];
#else
  return 1.0f/sqrtf(x);
#endif
}

//////////////////////////////////////////////////////////////////////////
// misc

#define LUX_FP_INV(r,p)                                                          \
{                                                                            \
  int _i = 2 * LUX_FP_ONE_BITS - *(int *)&(p);                                 \
  r = *(float *)&_i;                                                       \
  r = r * (2.0f - (p) * r);                                                \
}


#define LUX_FP_EXP(e,p)                                                          \
{                                                                            \
  int _i;                                                                  \
  e = -1.44269504f * (float)0x00800000 * (p);                              \
  _i = (int)e + 0x3F800000;                                                \
  e = *(float *)&_i;                                                       \
}


#if defined(LUX_COMPILER_MSC)
#define LUX_FP_NORM_TO_BYTE(i,p)            \
{                         \
  float _temp = (p) + 1.0f;           \
  (i) = (*(int *)&_temp >= 0x40000000)?0xFF:    \
  (*(int *)&_temp <=0x3F800000)?0:        \
  ((*(int *)&_temp) >> 15)&0xFF;          \
}
#else // if buggy compiler (like gcc...)
#define LUX_FP_NORM_TO_BYTE(i,p)                    \
{                                 \
  (i) = ((p)>=1.0f)?0xFF:((p)<=0)?0:(unsigned char)((p)*255.0f);  \
}
#endif

//////////////////////////////////////////////////////////////////////////
// rounding and fractional

#if !defined(_DEBUG) && defined(LUX_COMPILER_GCC) && defined(LUX_ARCH_X86)
LUX_INLINE int lxFastIntRound(float f)
{
  int r;
  __asm__ ("fistpl %0" : "=m" (r) : "t" (f) : "st");
  return r;
}
#elif !defined(_DEBUG) && defined(LUX_COMPILER_MSC) && defined(LUX_ARCH_X86)
LUX_INLINE int lxFastIntRound(float f)
{
  int r;
  _asm {
    fld f
      fistp r
  }
  return r;
}
#else
LUX_INLINE int lxFastIntRound(float f)
{
  return ((int) (((f) >= 0.0F) ? ((f) + 0.5F) : ((f) - 0.5F)));
}
#endif

LUX_INLINE int lxFastIntFloor(float f)
{
  int i = lxFastIntRound(f);
  return (i > f) ? i - 1 : i;
}
LUX_INLINE float lxFastFrac(float f)
{
  return ((f) - lxFastIntFloor(f));
}

//////////////////////////////////////////////////////////////////////////
extern lxFastMathCache_t* lx_gFastMath;

LUX_INLINE float lxFastSqrt(float n)
{

  if (LUX_FP_BITS(n) == 0)
    return 0.0;                 // check for square root of 0

#if defined(LUX_SIMD_SSE)
  {
    __m128 res = _mm_sqrt_ss(_mm_set_ss(n));
    return res.m128_f32[0];
  }
#elif defined(LUX_MATH_SQRT_TABLE)

  LUX_FP_BITS(n) = lx_gFastMath->fast_sqrt_table[(LUX_FP_BITS(n) >> 8) & 0xFFFF] | ((((LUX_FP_BITS(n) - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);

  return n;
#else
  return sqrtf(n);
#endif
  
}

//////////////////////////////////////////////////////////////////////////
// sin cos
#define MATH_HALF_MAX_CIRCLE_ANGLE    (LUX_MATH_MAX_CIRCLE_ANGLE/2)
#define MATH_QUARTER_MAX_CIRCLE_ANGLE (LUX_MATH_MAX_CIRCLE_ANGLE/4)
#define MATH_MASK_MAX_CIRCLE_ANGLE    (LUX_MATH_MAX_CIRCLE_ANGLE - 1)


// from flipcode
LUX_INLINE float lxFastCos(const float n)
{
#if defined(LUX_SIMD_SSE)
  __m128 res = lxFastCos_ps(_mm_set_ps1(n));
  return res.m128_f32[0];
#elif defined(LUX_MATH_SINCOS_TABLE)
  float f;
  int i;

  f = n * MATH_HALF_MAX_CIRCLE_ANGLE * LUX_DIV_PI;
  lxFloatToInt(&i, f);
  if (i < 0)
  {
    return lx_gFastMath->mathcossintable[((-i) + MATH_QUARTER_MAX_CIRCLE_ANGLE)&MATH_MASK_MAX_CIRCLE_ANGLE];
  }
  else
  {
    return lx_gFastMath->mathcossintable[(i + MATH_QUARTER_MAX_CIRCLE_ANGLE)&MATH_MASK_MAX_CIRCLE_ANGLE];
  }
#else
  return cosf(n);
#endif
}

LUX_INLINE float lxFastSin(const float n)
{
#if defined(LUX_SIMD_SSE)
  __m128 res = lxFastSin_ps(_mm_set_ps1(n));
  return res.m128_f32[0];
#elif defined(LUX_MATH_SINCOS_TABLE)
  float f;
  int i;

  f = n * MATH_HALF_MAX_CIRCLE_ANGLE * LUX_DIV_PI;
  lxFloatToInt(&i, f);
  if (i < 0)
  {
    return lx_gFastMath->mathcossintable[(-((-i)&MATH_MASK_MAX_CIRCLE_ANGLE)) + LUX_MATH_MAX_CIRCLE_ANGLE];
  }
  else
  {
    return lx_gFastMath->mathcossintable[i&MATH_MASK_MAX_CIRCLE_ANGLE];
  }
#else
  return sinf(n);
#endif
}



#ifdef __cplusplus
}
#endif

#endif
