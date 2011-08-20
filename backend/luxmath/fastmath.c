// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/fastmath.h>
#include <luxinia/luxmath/float16.h>

lxFastMathCache_t*  lx_gFastMath;

//////////////////////////////////////////////////////////////////////////
// build up tables
typedef union FastSqrtUnion
{
  float f;
  unsigned int i;
} FastSqrtUnion;

LUX_API void lxFastMath_initset(lxFastMathCache_t* cache)
{
  lx_gFastMath = cache;
}

LUX_API void lxFastMath_init(lxFastMathCache_t* cache)
{

  unsigned int i;
  FastSqrtUnion s;

  lx_gFastMath = cache;

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

  for (i = 0; i < LUX_MATH_MAX_CIRCLE_ANGLE; i++){
    cache->mathcossintable[i] = (float)sin((double)i * LUX_MUL_PI / MATH_HALF_MAX_CIRCLE_ANGLE);
  }
}