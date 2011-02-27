// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxmath/fastmath.h>
#include <luxmath/float16.h>

lxFastMathCache_t*  g_fmcache;

//////////////////////////////////////////////////////////////////////////
// build up tables
typedef union FastSqrtUnion
{
  float f;
  unsigned int i;
} FastSqrtUnion;

LUX_API void lxFastMath_initset(lxFastMathCache_t* cache)
{
  g_fmcache = cache;
}

LUX_API void lxFastMath_init(lxFastMathCache_t* cache)
{

  unsigned int i;
  FastSqrtUnion s;

  g_fmcache = cache;

  for (i = 0; i <= 0x7FFF; i++)
  {

    // Build a float with the bit pattern i as mantissa
    //  and an exponent of 0, stored as 127

    s.i = (i << 8) | (0x7F << 23);
    s.f = (float)sqrt(s.f);

    // Take the square root then strip the first 7 bits of
    //  the mantissa into the table

    cache->fast_sqrt_table[i + 0x8000] = (s.i & 0x7FFFFF);

    // Repeat the process, this time with an exponent of 1,
    //  stored as 128

    s.i = (i << 8) | (0x80 << 23);
    s.f = (float)sqrt(s.f);

    cache->fast_sqrt_table[i] = (s.i & 0x7FFFFF);
  }

  for (i = 0; i < MATH_MAX_CIRCLE_ANGLE; i++){
    cache->mathcossintable[i] = (float)sin((double)i * MUL_PI / MATH_HALF_MAX_CIRCLE_ANGLE);
  }

  for (i = 0; i < 0x10000; i++){
    cache->float16conv[i] = lxFloat16To32((float16)i);
  }
}