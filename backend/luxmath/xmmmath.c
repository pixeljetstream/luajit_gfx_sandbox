// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/xmmmath.h>

#ifdef LUX_SIMD_SSE


static void SSE_compile_test()
{
  lxVector4SSE veca = lxVector4SSE_setAll(2.0f);
  lxVector4SSE vecb = lxVector4SSE_setAll(2.0f);
  lxVector4SSE vecc = lxVector4SSE_setAll(2.0f);
  lxVector4SSE vecd = lxVector4SSE_setAll(2.0f);
  veca = lxVector4SSE_vcatmullRom(lxVector4SSE_setAll(1.0),&veca,&vecb,&vecc,&vecd);
}



#endif

