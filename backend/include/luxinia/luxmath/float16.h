// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

// Based on Nvidia SDK

#ifndef __LUXMATH_FLOAT16_H__
#define __LUXMATH_FLOAT16_H__

#include <luxinia/luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif

  LUX_API float16 lxFloat32To16( float fval );
  LUX_API float lxFloat16To32( float16 ival );

//////////////////////////////////////////////////////////////////////////

LUX_INLINE float16 lxFloat32To16( float fval )
{
  unsigned long ival= * (unsigned long*)( &fval );
  if( !ival ){
    return  0;
  }
  else{
    int e=((ival & 0x7f800000)>>23) - 127 + 15;
    if( e < 0 ){
      return  0;
    }else if( e > 31 ){
      e= 31;
    }
    {
      unsigned long   s=  ival & 0x80000000;
      unsigned long   f=  ival & 0x007fffff;
      return  (float16) (((s>>16)&0x8000) | ((e<<10) & 0x7c00) | ((f>>13) & 0x03ff));
    }
  }
}

LUX_INLINE float lxFloat16To32( float16 ival )
{
  if( !ival ){
    return  0.0f;
  }
  else
  {
    unsigned long s=  ival & 0x8000;
    signed long e=((ival & 0x7c00) >>10) - 15 + 127;
    unsigned long f=  ival & 0x03ff;
    unsigned long fval= (s<<16) | ((e <<23)&0x7f800000) | (f<<13);
    return  * (float*)( &fval );
  }
}

#ifdef __cplusplus
}
#endif

#endif

