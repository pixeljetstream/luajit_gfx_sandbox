// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_MISC_H__
#define __LUXMATH_MISC_H__

#include <luxinia/luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////

LUX_API int   lxNextPowerOf2(const int size);
LUX_API int   lxNearestPowerOf2(const int size);

LUX_API float lxFrand();
LUX_API float lxZigzag(const float in);
LUX_API float lxVariance(const float in,const float var, const float rand);
LUX_API void  lxSwapFloat(float *a, float *b);

LUX_API void  lxPermutation(uint32 *array, int32 n);

LUX_API float lxGetAverageOfN (const float *array, int n); // calculates the avg of n elements in array
LUX_API float lxGetMinOfN (const float *array, int n);  // returns the min of n elements in array
LUX_API float lxGetMaxOfN (const float *array, int n); // returns the max of n elements in array

LUX_API float lxGetIntAverageOfN (const int *array, int n); // calculates the avg of n elements in array
LUX_API int   lxGetIntMinOfN (const int *array, int n); // returns the min of n elements in array
LUX_API int   lxGetIntMaxOfN (const int *array, int n); // returns the max of n elements in array


//////////////////////////////////////////////////////////////////////////

// perform linear interpolation between two vectors
#define LUX_ARRAY3LERPTYPE(type, out,  t,  a,  b)  (\
  out[0] = (type)((float)((a)[0])+ ((float)((b)[0])- (float)((a)[0]))* (float)t),\
  out[1] = (type)((float)((a)[1])+ ((float)((b)[1])- (float)((a)[1]))* (float)t),\
  out[2] = (type)((float)((a)[2])+ ((float)((b)[2])- (float)((a)[2]))* (float)t))

// perform linear interpolation between two vectors
#define LUX_ARRAY4LERPTYPE(type, out,  t,  a,  b)  (\
  out[0] = (type)((float)((a)[0])+ ((float)((b)[0])- (float)((a)[0]))* (float)t),\
  out[1] = (type)((float)((a)[1])+ ((float)((b)[1])- (float)((a)[1]))* (float)t),\
  out[2] = (type)((float)((a)[2])+ ((float)((b)[2])- (float)((a)[2]))* (float)t),\
  out[3] = (type)((float)((a)[3])+ ((float)((b)[3])- (float)((a)[3]))* (float)t))

#define LUX_ARRAY4CLAMP(color,min,max) \
  ((color)[0] = ((color)[0] > max) ? max : ((color)[0] < min) ? min : (color)[0], \
  (color)[1] = ((color)[1] > max) ? max : ((color)[1] < min) ? min : (color)[1],  \
  (color)[2] = ((color)[2] > max) ? max : ((color)[2] < min) ? min : (color)[2],  \
  (color)[3] = ((color)[3] > max) ? max : ((color)[3] < min) ? min : (color)[3])

//////////////////////////////////////////////////////////////////////////

LUX_INLINE int  lxNextPowerOf2(const int size){
  int power = 2;

  if (size <= 1)
    return 1;

  while (power < size){
    power<<=1;
  }
  return power;
}

LUX_INLINE int  lxNearestPowerOf2(const int size)
{
  int power = 2;

  if (size <= 1){
    return 1;
  }

  while (1)
  {
    // we found the proper one
    if (size == power)
      return power;
    if (size > power && size < (power * 2))
    {
      // find towards which it is closer
      if (size >= ((power+(power*2))/2))
        return power*2;
      else
        return power;
    }
    power *= 2;
  };
}

LUX_INLINE float lxFrand()
{
  return (float)rand()/(float)RAND_MAX;
}

// zigzag width = 2, first half is a positive triangle, second half a negative
LUX_INLINE float lxZigzag(const float in){
  float calc;
  calc = (float)fmod(in,2);
  if (calc < 0){
    calc = -calc;
    if (calc > 1)
      calc -= 1.0;
    else
      calc += 1.0;
  }
  if (calc <= 0.5)
    return calc*2;
  if (calc <= 1.5)
    return 1 - (calc-0.5f)*2;
  if (calc <= 2)
    return (calc - 1.5f)*2 - 1;

  return 1;
}

LUX_INLINE float lxVariance(const float in,const float var, const float rand){
  return LUX_LERP(rand,in-var,in+var);
}

LUX_INLINE void lxSwapFloat(float *a, float *b){
  float t=*b;
  *b=*a;
  *a=t;
}

#ifdef __cplusplus
}
#endif

#endif
