// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_VECTOR4_H__
#define __LUXMATH_VECTOR4_H__

#include <luxinia/luxmath/basetypes.h>
#include <luxinia/luxmath/fastmath.h>

#ifdef __cplusplus
extern "C"{
#endif

LUX_API void lxVector4Set( lxVector4 pV, float x, float y, float z, float w );
LUX_API void lxVector4Copy( lxVector4 pOut, const lxVector4 pV1);
LUX_API void lxVector4Clear( lxVector4 pOut);
LUX_API float lxVector4LengthFast( const lxVector4 pV );
LUX_API float lxVector4Length( const lxVector4 pV );
LUX_API float lxVector4SqLength( const lxVector4 pV );
LUX_API float lxVector4Dot( const lxVector4 pV1, const lxVector4 pV2 );
LUX_API float lxVector4Normalize( lxVector4 pOut, const lxVector4 pV1 );
LUX_API float lxVector4Normalized( lxVector4 pOut );
LUX_API float lxVector4NormalizeFast( lxVector4 pOut, const lxVector4 pV1 );
LUX_API float lxVector4NormalizedFast( lxVector4 pOut );

LUX_API void lxVector4Add( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 );
LUX_API void lxVector4Sub( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 );
LUX_API void lxVector4Mul( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 );
LUX_API void lxVector4Div( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 );
LUX_API void lxVector4Min( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 );
LUX_API void lxVector4Max( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 );
LUX_API void lxVector4Scale( lxVector4 pOut, const lxVector4 pV, float s);
LUX_API void lxVector4ScaledAdd( lxVector4 pOut, const lxVector4 pV, float s, const lxVector4 pV2);
LUX_API void lxVector4MulAdd( lxVector4 pOut, const lxVector4 pV, const lxVector4 pV2, const lxVector4 pV3);
LUX_API void lxVector4Lerp( lxVector4 pOut, const float t, const lxVector4 pV1, const lxVector4 pV2);
LUX_API void lxVector4CatmullRom(lxVector4 vout, const float t, const lxVector4 v0, const lxVector4 v1, const lxVector4 v2, const lxVector4 v3);
LUX_API void lxVector4Negate( lxVector4 pOut, const lxVector4 pV1);
LUX_API void lxVector4Negated( lxVector4 pOut);
LUX_API void lxVector4Saturated( lxVector4 pOut);
LUX_API float lxVector4DistanceFast( const lxVector4 a, const lxVector4 b);
LUX_API float lxVector4Distance( const lxVector4 a, const lxVector4 b);
LUX_API float lxVector4SqDistance(const lxVector4 a, const lxVector4 b);

LUX_API void lxVector4Transform( lxVector4 output, const lxVector4 v1,  lxMatrix44CPTR mat ) ;
LUX_API void lxVector4Transform1( lxVector4 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector4TransformT( lxVector4 output,  const lxVector4 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector4TransformT1( lxVector4 v1, lxMatrix44CPTR mat ) ;


LUX_API void lxVector4float_FROM_ubyte( lxVector4 vec4, const uchar ub4[4]);
LUX_API void lxVector4ubyte_FROM_float( uchar ub4[4], const lxVector4 vec4);
LUX_API void lxVector4float_FROM_short( lxVector4 vec4, const short shrt4[4]);
LUX_API void lxVector4short_FROM_float( short shrt4[4], const lxVector4 vec4);
LUX_API void lxVector4float_FROM_ushort( lxVector4 vec4, const ushort shrt4[4]);
LUX_API void lxVector4ushort_FROM_float( ushort shrt4[4], const lxVector4 vec4);
LUX_API void lxVector4ubyte_FROM_ushort( uchar vec4[4], const ushort shrt4[4]);
LUX_API void lxVector4ushort_FROM_ubyte( ushort shrt4[4], const uchar vec4[4]);

//////////////////////////////////////////////////////////////////////////

LUX_INLINE void lxVector4Set( lxVector4 pV, float x, float y, float z, float w )
{
  pV[0] = x;
  pV[1] = y;
  pV[2] = z;
  pV[3] = w;
}
LUX_INLINE void lxVector4Copy( lxVector4 pOut, const lxVector4 pV1)
{
  pOut[0] = pV1[0];
  pOut[1] = pV1[1];
  pOut[2] = pV1[2];
  pOut[3] = pV1[3];
}

LUX_INLINE void lxVector4Clear( lxVector4 pOut)
{
  pOut[0] = 0.0f;
  pOut[1] = 0.0f;
  pOut[2] = 0.0f;
  pOut[3] = 0.0f;
}

LUX_INLINE float lxVector4Length( const lxVector4 pV )
{
#ifdef __cplusplus
  return sqrtf(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2] + pV[3] * pV[3]);
#else
  return (float) sqrt(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2] + pV[3] * pV[3]);
#endif
}

LUX_INLINE float lxVector4LengthFast( const lxVector4 pV )
{
  return lxFastSqrt(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2] + pV[3] * pV[3]);
}

LUX_INLINE float lxVector4SqLength( const lxVector4 pV )
{
  return pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2] + pV[3] * pV[3];
}

LUX_INLINE float lxVector4Dot( const lxVector4 pV1, const lxVector4 pV2 )
{

  return pV1[0] * pV2[0] + pV1[1] * pV2[1] + pV1[2] * pV2[2] + pV1[3] * pV2[3];
}

LUX_INLINE float lxVector4Normalize( lxVector4 pOut, const lxVector4 pV1 )
{
  float length = lxVector4Length(pV1);
  float lengthdiv = (length < 0.0001f) ? 1.0f : 1.0f/length;

  pOut[0] = pV1[0] * lengthdiv;
  pOut[1] = pV1[1] * lengthdiv;
  pOut[2] = pV1[2] * lengthdiv;
  pOut[3] = pV1[3] * lengthdiv;

  return length;
}

LUX_INLINE float lxVector4NormalizeFast( lxVector4 pOut, const lxVector4 pV1 )
{
  float lengthdiv = lxFastSqrtRcp(lxVector4SqLength(pV1));

  pOut[0] = pV1[0] * lengthdiv;
  pOut[1] = pV1[1] * lengthdiv;
  pOut[2] = pV1[2] * lengthdiv;
  pOut[3] = pV1[3] * lengthdiv;

  return 1.0f/lengthdiv;
}

LUX_INLINE float lxVector4Normalized( lxVector4 pOut)
{
  float length = lxVector4Length(pOut);
  float lengthdiv = (length < 0.0001f) ? 1.0f : 1.0f/length;

  pOut[0] = pOut[0] * lengthdiv;
  pOut[1] = pOut[1] * lengthdiv;
  pOut[2] = pOut[2] * lengthdiv;
  pOut[3] = pOut[3] * lengthdiv;

  return length;
}

LUX_INLINE float lxVector4NormalizedFast( lxVector4 pOut )
{
  float lengthdiv = lxFastSqrtRcp(lxVector4SqLength(pOut));

  pOut[0] = pOut[0] * lengthdiv;
  pOut[1] = pOut[1] * lengthdiv;
  pOut[2] = pOut[2] * lengthdiv;
  pOut[3] = pOut[3] * lengthdiv;

  return 1.0f/lengthdiv;
}

LUX_INLINE void lxVector4Add( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 )
{
  pOut[0] = pV1[0] + pV2[0];
  pOut[1] = pV1[1] + pV2[1];
  pOut[2] = pV1[2] + pV2[2];
  pOut[3] = pV1[3] + pV2[3];
}

LUX_INLINE void lxVector4Sub( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 )
{
  pOut[0] = pV1[0] - pV2[0];
  pOut[1] = pV1[1] - pV2[1];
  pOut[2] = pV1[2] - pV2[2];
  pOut[3] = pV1[3] - pV2[3];
}

LUX_INLINE void lxVector4Mul( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 )
{
  pOut[0] = pV1[0] * pV2[0];
  pOut[1] = pV1[1] * pV2[1];
  pOut[2] = pV1[2] * pV2[2];
  pOut[3] = pV1[3] * pV2[3];
}

LUX_INLINE void lxVector4Div( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 )
{
  pOut[0] = pV1[0] / pV2[0];
  pOut[1] = pV1[1] / pV2[1];
  pOut[2] = pV1[2] / pV2[2];
  pOut[3] = pV1[3] / pV2[3];
}

LUX_INLINE void lxVector4Min( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 )
{
  pOut[0] = pV1[0] < pV2[0] ? pV1[0] : pV2[0];
  pOut[1] = pV1[1] < pV2[1] ? pV1[1] : pV2[1];
  pOut[2] = pV1[2] < pV2[2] ? pV1[2] : pV2[2];
  pOut[3] = pV1[3] < pV2[3] ? pV1[3] : pV2[3];
}

LUX_INLINE void lxVector4Max( lxVector4 pOut, const lxVector4 pV1, const lxVector4 pV2 )
{
  pOut[0] = pV1[0] > pV2[0] ? pV1[0] : pV2[0];
  pOut[1] = pV1[1] > pV2[1] ? pV1[1] : pV2[1];
  pOut[2] = pV1[2] > pV2[2] ? pV1[2] : pV2[2];
  pOut[3] = pV1[3] > pV2[3] ? pV1[3] : pV2[3];
}

LUX_INLINE void lxVector4Scale( lxVector4 pOut, const lxVector4 pV, float s)
{
  pOut[0] = pV[0] * s;
  pOut[1] = pV[1] * s;
  pOut[2] = pV[2] * s;
  pOut[3] = pV[3] * s;
}

LUX_INLINE void lxVector4ScaledAdd( lxVector4 pOut, const lxVector4 pV, float s, const lxVector4 pV2)
{
  pOut[0] = pV[0] + (s * pV2[0]);
  pOut[1] = pV[1] + (s * pV2[1]);
  pOut[2] = pV[2] + (s * pV2[2]);
  pOut[3] = pV[3] + (s * pV2[3]);
}

LUX_INLINE void lxVector4MulAdd( lxVector4 pOut, const lxVector4 pV, const lxVector4 pV2, const lxVector4 pV3)
{
  pOut[0] = pV[0] + (pV2[0] * pV3[0]);
  pOut[1] = pV[1] + (pV2[1] * pV3[1]);
  pOut[2] = pV[2] + (pV2[2] * pV3[2]);
  pOut[3] = pV[3] + (pV2[3] * pV3[3]);
}

LUX_INLINE void lxVector4Lerp( lxVector4 pOut, const float s, const lxVector4 pV1, const lxVector4 pV2)
{
  pOut[0] = pV1[0] + s * (pV2[0] - pV1[0]);
  pOut[1] = pV1[1] + s * (pV2[1] - pV1[1]);
  pOut[2] = pV1[2] + s * (pV2[2] - pV1[2]);
  pOut[3] = pV1[3] + s * (pV2[3] - pV1[3]);
}


LUX_INLINE void lxVector4Negate( lxVector4 pOut, const lxVector4 pV1)
{
  pOut[0] = -pV1[0];
  pOut[1] = -pV1[1];
  pOut[2] = -pV1[2];
  pOut[3] = -pV1[3];
}

LUX_INLINE void lxVector4Negated( lxVector4 pOut)
{
  pOut[0] = -pOut[0];
  pOut[1] = -pOut[1];
  pOut[2] = -pOut[2];
  pOut[3] = -pOut[3];
}

LUX_INLINE void lxVector4Saturated( lxVector4 pOut)
{
  pOut[0] = (LUX_FP_GREATER_ONE(pOut[0])) ? 1.0f : (LUX_FP_LESS_ZERO(pOut[0])) ? 0.0f : pOut[0];
  pOut[1] = (LUX_FP_GREATER_ONE(pOut[1])) ? 1.0f : (LUX_FP_LESS_ZERO(pOut[1])) ? 0.0f : pOut[1];
  pOut[2] = (LUX_FP_GREATER_ONE(pOut[2])) ? 1.0f : (LUX_FP_LESS_ZERO(pOut[2])) ? 0.0f : pOut[2];
  pOut[3] = (LUX_FP_GREATER_ONE(pOut[3])) ? 1.0f : (LUX_FP_LESS_ZERO(pOut[3])) ? 0.0f : pOut[3];
}

LUX_INLINE void lxVector4CatmullRom(lxVector4 vout, const float t, const lxVector4 v0, const lxVector4 v1, const lxVector4 v2, const lxVector4 v3)
{
  // catmull rom spline interpolation
  // between v1 and v2
  //q(t) = 0.5 *(     (2 * P1) + (-P0 + P2) * t + (2*P0 - 5*P1 + 4*P2 - P3) * t2 + (-P0 + 3*P1- 3*P2 + P3) * t3)

  float t2=t*t;
  float t3=t2*t;

  float coeff0 = -t3 + 2.0f * t2 - t;
  float coeff1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
  float coeff2 = -3.0f * t3 + 4.0f * t2 + t;
  float coeff3 = t3 - t2;

  vout[0] = (coeff0 * v0[0] + coeff1 * v1[0] + coeff2 * v2[0] + coeff3 * v3[0])*0.5f;
  vout[1] = (coeff0 * v0[1] + coeff1 * v1[1] + coeff2 * v2[1] + coeff3 * v3[1])*0.5f;
  vout[2] = (coeff0 * v0[2] + coeff1 * v1[2] + coeff2 * v2[2] + coeff3 * v3[2])*0.5f;
  vout[3] = (coeff0 * v0[3] + coeff1 * v1[3] + coeff2 * v2[3] + coeff3 * v3[3])*0.5f;
}


LUX_INLINE float lxVector4DistanceFast( const lxVector4 a, const lxVector4 b)
{
  lxVector4 temp;

  lxVector4Sub(temp,a,b);
  return lxVector4LengthFast(temp);
}

LUX_INLINE float lxVector4Distance( const lxVector4 a, const lxVector4 b)
{
  lxVector4 temp;

  lxVector4Sub(temp,a,b);
  return lxVector4Length(temp);
}

LUX_INLINE float lxVector4SqDistance(const lxVector4 a, const lxVector4 b) 
{
  lxVector4 temp;

  lxVector4Sub(temp,a,b);
  return temp[0]*temp[0]+ temp[1]*temp[1]+ temp[2]*temp[2];
}

LUX_INLINE void lxVector4Transform( lxVector4 output, const lxVector4 v1,  lxMatrix44CPTR mat ) 
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(v1)[2]*(mat)[8]+(v1)[3]*(mat)[12];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[9]+(v1)[3]*(mat)[13];
  (output)[2] = (v1)[0]*(mat)[2]+(v1)[1]*(mat)[6]+(v1)[2]*(mat)[10]+(v1)[3]*(mat)[14];
  (output)[3] = (v1)[0]*(mat)[3]+(v1)[1]*(mat)[7]+(v1)[2]*(mat)[11]+(v1)[3]*(mat)[15];
}

LUX_INLINE void lxVector4Transform1( lxVector4 v1,  lxMatrix44CPTR mat ) 
{
  lxVector4 vector;

  vector[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(v1)[2]*(mat)[8]+(v1)[3]*(mat)[12];
  vector[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[9]+(v1)[3]*(mat)[13];
  vector[2] = (v1)[0]*(mat)[2]+(v1)[1]*(mat)[6]+(v1)[2]*(mat)[10]+(v1)[3]*(mat)[14];
  vector[3] = (v1)[0]*(mat)[3]+(v1)[1]*(mat)[7]+(v1)[2]*(mat)[11]+(v1)[3]*(mat)[15];
  lxVector4Copy(v1,vector);
}

LUX_INLINE void lxVector4TransformT( lxVector4 output,  const lxVector4 v1,  lxMatrix44CPTR mat )
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[1]+(v1)[2]*(mat)[2]+(v1)[3]*(mat)[3];
  (output)[1] = (v1)[0]*(mat)[4]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[6]+(v1)[3]*(mat)[7];
  (output)[2] = (v1)[0]*(mat)[8]+(v1)[1]*(mat)[9]+(v1)[2]*(mat)[10]+(v1)[3]*(mat)[11];
  (output)[3] = (v1)[0]*(mat)[12]+(v1)[1]*(mat)[13]+(v1)[2]*(mat)[14]+(v1)[3]*(mat)[15];
}

LUX_INLINE void lxVector4TransformT1( lxVector4 v1, lxMatrix44CPTR mat ) 
{
  lxVector4 vector;

  vector[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[1]+(v1)[2]*(mat)[2]+(v1)[3]*(mat)[3];
  vector[1] = (v1)[0]*(mat)[4]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[6]+(v1)[3]*(mat)[7];
  vector[2] = (v1)[0]*(mat)[8]+(v1)[1]*(mat)[9]+(v1)[2]*(mat)[10]+(v1)[3]*(mat)[11];
  vector[3] = (v1)[0]*(mat)[12]+(v1)[1]*(mat)[13]+(v1)[2]*(mat)[14]+(v1)[3]*(mat)[15];
  lxVector4Copy(v1,vector);
}

LUX_INLINE void lxVector4float_FROM_ubyte( lxVector4 vec4, const uchar ub4[4]) 
{
  (vec4)[0] = (float)(ub4)[0]*LUX_DIV_255;
  (vec4)[1] = (float)(ub4)[1]*LUX_DIV_255;
  (vec4)[2] = (float)(ub4)[2]*LUX_DIV_255;
  (vec4)[3] = (float)(ub4)[3]*LUX_DIV_255;
}

LUX_INLINE void lxVector4ubyte_FROM_float( uchar ub4[4], const lxVector4 vec4) 
{
  LUX_FP_NORM_TO_BYTE((ub4)[0],(vec4)[0]);
  LUX_FP_NORM_TO_BYTE((ub4)[1],(vec4)[1]);
  LUX_FP_NORM_TO_BYTE((ub4)[2],(vec4)[2]);
  LUX_FP_NORM_TO_BYTE((ub4)[3],(vec4)[3]);
}

LUX_INLINE void lxVector4float_FROM_short( lxVector4 vec4, const short shrt4[4]) 
{
  (vec4)[0]= LUX_DIV_SHORT*(float)(shrt4)[0];
  (vec4)[1]= LUX_DIV_SHORT*(float)(shrt4)[1];
  (vec4)[2]= LUX_DIV_SHORT*(float)(shrt4)[2];
  (vec4)[3]= LUX_DIV_SHORT*(float)(shrt4)[3];
}

LUX_INLINE void lxVector4short_FROM_float( short shrt4[4], const lxVector4 vec4) 
{
  (shrt4)[0]= (short)(LUX_MUL_SHORT*(vec4)[0]);
  (shrt4)[1]= (short)(LUX_MUL_SHORT*(vec4)[1]);
  (shrt4)[2]= (short)(LUX_MUL_SHORT*(vec4)[2]);
  (shrt4)[3]= (short)(LUX_MUL_SHORT*(vec4)[3]);
}

LUX_INLINE void lxVector4float_FROM_ushort( lxVector4 vec4, const ushort shrt4[4]) 
{
  (vec4)[0]= LUX_DIV_USHORT*(float)(shrt4)[0];
  (vec4)[1]= LUX_DIV_USHORT*(float)(shrt4)[1];
  (vec4)[2]= LUX_DIV_USHORT*(float)(shrt4)[2];
  (vec4)[3]= LUX_DIV_USHORT*(float)(shrt4)[3];
}

LUX_INLINE void lxVector4ushort_FROM_float( ushort shrt4[4], const lxVector4 vec4) 
{
  (shrt4)[0]= (short)(LUX_MUL_USHORT*(vec4)[0]);
  (shrt4)[1]= (short)(LUX_MUL_USHORT*(vec4)[1]);
  (shrt4)[2]= (short)(LUX_MUL_USHORT*(vec4)[2]);
  (shrt4)[3]= (short)(LUX_MUL_USHORT*(vec4)[3]);
}

LUX_INLINE void lxVector4ubyte_FROM_ushort( uchar vec4[4], const ushort shrt4[4]) 
{
  (vec4)[0]= (shrt4)[0]>>8;
  (vec4)[1]= (shrt4)[1]>>8;
  (vec4)[2]= (shrt4)[2]>>8;
  (vec4)[3]= (shrt4)[3]>>8;
}

LUX_INLINE void lxVector4ushort_FROM_ubyte( ushort shrt4[4], const uchar vec4[4]) 
{
  (shrt4)[0]= (vec4)[0]<<8;
  (shrt4)[1]= (vec4)[1]<<8;
  (shrt4)[2]= (vec4)[2]<<8;
  (shrt4)[3]= (vec4)[3]<<8;
}

#ifdef __cplusplus
}
#endif

#endif
