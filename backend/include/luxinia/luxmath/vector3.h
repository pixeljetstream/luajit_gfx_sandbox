// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_VECTOR3_H__
#define __LUXMATH_VECTOR3_H__

#include <luxinia/luxmath/basetypes.h>
#include <luxinia/luxmath/fastmath.h>
#include <luxinia/luxmath/misc.h>

#ifdef __cplusplus
extern "C"{
#endif

LUX_API void lxVector3Set( lxVector3 pV, float x, float y, float z );
LUX_API void lxVector3Copy( lxVector3 pOut, const lxVector3 pV1);
LUX_API void lxVector3Clear( lxVector3 pOut);

LUX_API float lxVector3LengthFast( const lxVector3 pV );
LUX_API float lxVector3Length( const lxVector3 pV );
LUX_API float lxVector3SqLength( const lxVector3 pV );
LUX_API float lxVector3Dot( const lxVector3 pV1, const lxVector3 pV2 );

LUX_API void lxVector3Add( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3Sub( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3Mul( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3Div( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3Min( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3Max( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3Rad2Deg(lxVector3 dst, lxVector3 src);
LUX_API void lxVector3Deg2Rad(lxVector3 dst, lxVector3 src);
LUX_API void lxVector3Random(lxVector3 out);

LUX_API void lxVector3Cross( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3Scale( lxVector3 pOut, const lxVector3 pV, float s);
LUX_API void lxVector3ScaledAdd( lxVector3 pOut, const lxVector3 pV, float s, const lxVector3 pV2);
LUX_API void lxVector3MulAdd( lxVector3 pOut, const lxVector3 pV, const lxVector3 pV2, const lxVector3 pV3);
LUX_API void lxVector3Lerp( lxVector3 pOut, const float t, const lxVector3 pV1, const lxVector3 pV2 );
LUX_API void lxVector3CatmullRom(lxVector3 vout, const float t, const lxVector3 v0, const lxVector3 v1, const lxVector3 v2, const lxVector3 v3);
LUX_API float lxVector3DistanceFast( const lxVector3 a, const lxVector3 b);
LUX_API float lxVector3Distance( const lxVector3 a, const lxVector3 b);
LUX_API float lxVector3SqDistance(const lxVector3 a, const lxVector3 b);

LUX_API float lxVector3GetRotateZcossin( const lxVector3 in, float *cosf, float *sinf);
LUX_API float lxVector3GetRotateZcossinNormalized( const lxVector3 in, float *cosf, float *sinf);
LUX_API void  lxVector3RotateZcossin( lxVector3 out, const lxVector3 in, float cosf, float sinf);

// fast but not precise, dont use in accumulated / matrix ops
LUX_API float lxVector3NormalizedFast( lxVector3 pOut );
LUX_API float lxVector3NormalizeFast( lxVector3 pOut, const lxVector3 pV1 );

// more accurate version
LUX_API float lxVector3Normalized( lxVector3 pOut );
LUX_API float lxVector3Normalize( lxVector3 pOut, const lxVector3 pV1 );

LUX_API void lxVector3Negate( lxVector3 pOut, const lxVector3 pV1);
LUX_API void lxVector3Negated( lxVector3 pOut);
LUX_API void lxVector3Saturated( lxVector3 pOut);

LUX_API booln lxVector3InsideAABB( const lxVector3 point,  const lxVector3 mins,  const lxVector3 maxs);
LUX_API booln lxVector3IsZero(const lxVector3 vec3);

LUX_API void lxVector3Transform( lxVector3 output, const lxVector3 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector3Transform1( lxVector3 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector3InvTransform( lxVector3 output, const lxVector3 vector,  lxMatrix44CPTR mat );
LUX_API void lxVector3InvTransform1( lxVector3 vector, lxMatrix44CPTR mat );
LUX_API void lxVector3TransformT( lxVector3 output,  const lxVector3 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector3TransformT1( lxVector3 v1, lxMatrix44CPTR mat );
LUX_API void lxVector3TransformRot( lxVector3 output,  const lxVector3 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector3TransformRot1( lxVector3 v1, lxMatrix44CPTR mat ) ;

LUX_API float lxVector3LineDistanceSq(const lxVector3 point,const lxVector3 linestart,const lxVector3 linedir);
LUX_API float lxVector3LineDistanceSqFracc(const lxVector3 point,
                 const lxVector3 linestart, const lxVector3 linedir, float *ofracc);
LUX_API void lxVector3SpreadFast(lxVector3 out, lxVector3 in, const float radin, const float radout);
LUX_API void lxVector3PerpendicularFast(lxVector3 out, lxVector3 in);
LUX_API void lxVector3Perpendicular(lxVector3 out, lxVector3 in);
LUX_API void lxVector3MinMax(lxVector3 min, lxVector3 max, lxVector3 pos);

// normalized conversions: float is [-1/0,1] and others are [-TYPEMAX/0,TYPEMAX]
LUX_API void lxVector3float_FROM_ubyte( lxVector3 vec3, const uchar ub3[3]);
LUX_API void lxVector3ubyte_FROM_float( uchar ub3[3], const lxVector3 vec3);
LUX_API void lxVector3float_FROM_short( lxVector3 vec3, const short shrt3[3]);
LUX_API void lxVector3short_FROM_float( short shrt3[3], const lxVector3 vec3);
LUX_API void lxVector3float_FROM_ushort( lxVector3 vec3, const ushort shrt3[3]);
LUX_API void lxVector3ushort_FROM_float( ushort shrt3[3], const lxVector3 vec3);
LUX_API void lxVector3ubyte_FROM_ushort( uchar vec3[3], const ushort shrt3[3]);
LUX_API void lxVector3ushort_FROM_ubyte( ushort shrt3[3], const uchar vec3[3]);
LUX_API void lxVector3pkubyte_FROM_short(uchar pkub3[3], const short shrt3[3]);
LUX_API void lxVector3short_FROM_pkubyte(short shrt3[3], const uchar pkub3[3]);
LUX_API void lxVector3pkubyte_FROM_float(uchar pkub3[3], const lxVector3 vec3);
LUX_API void lxVector3float_FROM_pkubyte(lxVector3 vec3, const uchar pkub3[3]);

LUX_API void lxVector3Swizzle(lxVector3 out, uint axis[3], lxVector3 dirs);

//////////////////////////////////////////////////////////////////////////


LUX_INLINE void lxVector3Set( lxVector3 pV, float x, float y, float z )
{
  pV[0] = x;
  pV[1] = y;
  pV[2] = z;
}
LUX_INLINE void lxVector3Copy( lxVector3 pOut, const lxVector3 pV1)
{
  pOut[0] = pV1[0];
  pOut[1] = pV1[1];
  pOut[2] = pV1[2];
}

LUX_INLINE void lxVector3Clear( lxVector3 pOut)
{
  pOut[0] = 0.0f;
  pOut[1] = 0.0f;
  pOut[2] = 0.0f;
}

LUX_INLINE float lxVector3Length( const lxVector3 pV )
{

#ifdef __cplusplus
  return sqrtf(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2]);
#else
  return (float) sqrt(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2]);
#endif
}

LUX_INLINE float lxVector3LengthFast( const lxVector3 pV )
{

#ifdef __cplusplus
  return lxFastSqrt(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2]);
#else
  return (float) lxFastSqrt(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2]);
#endif
}

LUX_INLINE float lxVector3SqLength( const lxVector3 pV )
{
  return pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2];
}

LUX_INLINE float lxVector3Dot( const lxVector3 pV1, const lxVector3 pV2 )
{
  return pV1[0] * pV2[0] + pV1[1] * pV2[1] + pV1[2] * pV2[2];
}

LUX_INLINE void lxVector3Cross( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[1] * pV2[2] - pV1[2] * pV2[1];
  pOut[1] = pV1[2] * pV2[0] - pV1[0] * pV2[2];
  pOut[2] = pV1[0] * pV2[1] - pV1[1] * pV2[0];
}

LUX_INLINE float lxVector3Normalize( lxVector3 pOut, const lxVector3 pV1 )
{
  float length = lxVector3Length(pV1);
  float lengthdiv = (length < 0.0001f) ? 1.0f : 1.0f/length;

  pOut[0] = pV1[0] * lengthdiv;
  pOut[1] = pV1[1] * lengthdiv;
  pOut[2] = pV1[2] * lengthdiv;

  return length;
}

LUX_INLINE float lxVector3Normalized( lxVector3 pOut )
{
  float length = lxVector3Length(pOut);
  float lengthdiv = (length < 0.0001f) ? 1.0f : 1.0f/length;

  pOut[0] = pOut[0] * lengthdiv;
  pOut[1] = pOut[1] * lengthdiv;
  pOut[2] = pOut[2] * lengthdiv;

  return length;
}

LUX_INLINE float lxVector3NormalizeFast( lxVector3 pOut, const lxVector3 pV1 )
{
  //float length = 1.0f / fastsqrt(Vector3SqLength(pV1));
  float length = lxFastSqrtRcp(lxVector3SqLength(pV1));
  pOut[0] = pV1[0] * length;
  pOut[1] = pV1[1] * length;
  pOut[2] = pV1[2] * length;

  return 1.0f/length;
}

LUX_INLINE float lxVector3NormalizedFast( lxVector3 pOut )
{
  //float length = 1.0f / fastsqrt(Vector3SqLength(pOut));
  float length = lxFastSqrtRcp(lxVector3SqLength(pOut));
  pOut[0] = pOut[0] * length;
  pOut[1] = pOut[1] * length;
  pOut[2] = pOut[2] * length;

  return 1.0f/length;
}


LUX_INLINE void lxVector3Add( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[0] + pV2[0];
  pOut[1] = pV1[1] + pV2[1];
  pOut[2] = pV1[2] + pV2[2];
}

LUX_INLINE void lxVector3Sub( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[0] - pV2[0];
  pOut[1] = pV1[1] - pV2[1];
  pOut[2] = pV1[2] - pV2[2];
}

LUX_INLINE void lxVector3Mul( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[0] * pV2[0];
  pOut[1] = pV1[1] * pV2[1];
  pOut[2] = pV1[2] * pV2[2];
}

LUX_INLINE void lxVector3Div( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[0] / pV2[0];
  pOut[1] = pV1[1] / pV2[1];
  pOut[2] = pV1[2] / pV2[2];
}

LUX_INLINE void lxVector3Min( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[0] < pV2[0] ? pV1[0] : pV2[0];
  pOut[1] = pV1[1] < pV2[1] ? pV1[1] : pV2[1];
  pOut[2] = pV1[2] < pV2[2] ? pV1[2] : pV2[2];
}

LUX_INLINE void lxVector3Max( lxVector3 pOut, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[0] > pV2[0] ? pV1[0] : pV2[0];
  pOut[1] = pV1[1] > pV2[1] ? pV1[1] : pV2[1];
  pOut[2] = pV1[2] > pV2[2] ? pV1[2] : pV2[2];
}

LUX_INLINE void lxVector3Scale( lxVector3 pOut, const lxVector3 pV, float s)
{
  pOut[0] = pV[0] * s;
  pOut[1] = pV[1] * s;
  pOut[2] = pV[2] * s;
}

LUX_INLINE void lxVector3ScaledAdd( lxVector3 pOut, const lxVector3 pV, float s, const lxVector3 pV2)
{
  pOut[0] = pV[0] + (s * pV2[0]);
  pOut[1] = pV[1] + (s * pV2[1]);
  pOut[2] = pV[2] + (s * pV2[2]);
}

LUX_INLINE void lxVector3MulAdd( lxVector3 pOut, const lxVector3 pV, const lxVector3 pV2, const lxVector3 pV3)
{
  pOut[0] = pV[0] + (pV2[0] * pV3[0]);
  pOut[1] = pV[1] + (pV2[1] * pV3[1]);
  pOut[2] = pV[2] + (pV2[2] * pV3[2]);
}

LUX_INLINE void lxVector3Lerp( lxVector3 pOut, const float s, const lxVector3 pV1, const lxVector3 pV2 )
{
  pOut[0] = pV1[0] + s * (pV2[0] - pV1[0]);
  pOut[1] = pV1[1] + s * (pV2[1] - pV1[1]);
  pOut[2] = pV1[2] + s * (pV2[2] - pV1[2]);
}


LUX_INLINE void lxVector3Negate( lxVector3 pOut, const lxVector3 pV1)
{
  pOut[0] = -pV1[0];
  pOut[1] = -pV1[1];
  pOut[2] = -pV1[2];
}

LUX_INLINE void lxVector3Negated( lxVector3 pOut)
{
  pOut[0] = -pOut[0];
  pOut[1] = -pOut[1];
  pOut[2] = -pOut[2];
}

LUX_INLINE void lxVector3Saturated( lxVector3 pOut)
{
  pOut[0] = (LUX_FP_GREATER_ONE(pOut[0])) ? 1.0f : (LUX_FP_LESS_ZERO(pOut[0])) ? 0.0f : pOut[0];
  pOut[1] = (LUX_FP_GREATER_ONE(pOut[1])) ? 1.0f : (LUX_FP_LESS_ZERO(pOut[1])) ? 0.0f : pOut[1];
  pOut[2] = (LUX_FP_GREATER_ONE(pOut[2])) ? 1.0f : (LUX_FP_LESS_ZERO(pOut[2])) ? 0.0f : pOut[2];
}

LUX_INLINE void lxVector3Random(lxVector3 out){
  booln zero;
  int i;

  zero = LUX_TRUE;
  while (zero){
    for (i = 0; i < 3; i++){
      out[i] = lxFrand() - 0.5f;
      if (out[i] > LUX_FLOAT_EPSILON || out[i] < LUX_FLOAT_EPSILON)
        zero = LUX_FALSE;
    }
  }
  lxVector3NormalizedFast(out);
}

// returns 0
LUX_INLINE float lxVector3GetRotateZcossinNormalized( const lxVector3 in, float *cosf, float *sinf)
{
  float len;

  len = 0.0f;
  *cosf = in[0];
  *sinf = in[1];

  return len;
}

// returns length in X/Y
LUX_INLINE float lxVector3GetRotateZcossin( const lxVector3 in, float *cosf, float *sinf)
{
  lxVector3 temp;
  float len;
  float out;

  lxVector3Copy(temp,in);
  len = lxFastSqrt(temp[0]*temp[0]+temp[1]*temp[1]);
  out = 1/len;
  *cosf = temp[0]*out;
  *sinf = temp[1]*out;

  return len;
}

LUX_INLINE void lxVector3RotateZcossin( lxVector3 out, const lxVector3 in, float cosf, float sinf)
{
  out[0] = cosf*in[0] - sinf*in[1];
  out[1] = sinf*in[0] + cosf*in[1];
  out[2] = in[2];
}

LUX_INLINE booln lxVector3InsideAABB( const lxVector3 point,  const lxVector3 mins,  const lxVector3 maxs)
{
  return (  !(point[0] < mins[0] || 
        point[0] > maxs[0] || 
        point[1] < mins[1] || 
        point[1] > maxs[1] || 
        point[2] < mins[2] || 
        point[2] > maxs[2]) );
}

LUX_INLINE booln lxVector3IsZero(const lxVector3 vec)
{
  return (!(vec)[0] && !(vec)[1] && !(vec)[2]);
}

LUX_INLINE void lxVector3CatmullRom(lxVector3 vout, const float t, const lxVector3 v0, const lxVector3 v1, const lxVector3 v2, const lxVector3 v3)
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
}


LUX_INLINE float lxVector3DistanceFast( const lxVector3 a, const lxVector3 b)
{
  lxVector3 temp;

  lxVector3Sub(temp,a,b);
  return lxVector3LengthFast(temp);
}

LUX_INLINE float lxVector3Distance( const lxVector3 a, const lxVector3 b)
{
  lxVector3 temp;

  lxVector3Sub(temp,a,b);
  return lxVector3Length(temp);
}

LUX_INLINE float lxVector3SqDistance(const lxVector3 a, const lxVector3 b) 
{
  lxVector3 temp;

  lxVector3Sub(temp,a,b);
  return temp[0]*temp[0]+ temp[1]*temp[1]+ temp[2]*temp[2];
}
  

LUX_INLINE void lxVector3Transform( lxVector3 output, const lxVector3 v1,  lxMatrix44CPTR mat ) 
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(v1)[2]*(mat)[8]+(mat)[12];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[9]+(mat)[13];
  (output)[2] = (v1)[0]*(mat)[2]+(v1)[1]*(mat)[6]+(v1)[2]*(mat)[10]+(mat)[14];
}

LUX_INLINE void lxVector3Transform1( lxVector3 v1,  lxMatrix44CPTR mat ) 
{
  lxVector3 vector;

  vector[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(v1)[2]*(mat)[8]+(mat)[12];
  vector[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[9]+(mat)[13];
  vector[2] = (v1)[0]*(mat)[2]+(v1)[1]*(mat)[6]+(v1)[2]*(mat)[10]+(mat)[14];
  lxVector3Copy(v1,vector);
}

LUX_INLINE void lxVector3InvTransform( lxVector3 output, const lxVector3 vector,  lxMatrix44CPTR mat ) 
{
  (output)[0] = (vector)[0]*(mat)[0]+(vector)[1]*(mat)[1]+(vector)[2]*(mat)[2]-(mat[0]*mat[12]+mat[1]*mat[13]+mat[2]*mat[14]);
  (output)[1] = (vector)[0]*(mat)[4]+(vector)[1]*(mat)[5]+(vector)[2]*(mat)[6]-(mat[4]*mat[12]+mat[5]*mat[13]+mat[6]*mat[14]);
  (output)[2] = (vector)[0]*(mat)[8]+(vector)[1]*(mat)[9]+(vector)[2]*(mat)[10]-(mat[8]*mat[12]+mat[9]*mat[13]+mat[10]*mat[14]);
}

LUX_INLINE void lxVector3InvTransform1( lxVector3 v1,  lxMatrix44CPTR mat ) 
{
  lxVector3 vector;

  vector[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[1]+(v1)[2]*(mat)[2]-(mat[0]*mat[12]+mat[1]*mat[13]+mat[2]*mat[14]);
  vector[1] = (v1)[0]*(mat)[4]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[6]-(mat[4]*mat[12]+mat[5]*mat[13]+mat[6]*mat[14]);
  vector[2] = (v1)[0]*(mat)[8]+(v1)[1]*(mat)[9]+(v1)[2]*(mat)[10]-(mat[8]*mat[12]+mat[9]*mat[13]+mat[10]*mat[14]);
  lxVector3Copy(v1,vector);
}

LUX_INLINE void lxVector3TransformT( lxVector3 output,  const lxVector3 v1,  lxMatrix44CPTR mat )
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[1]+(v1)[2]*(mat)[2]+(mat)[3];
  (output)[1] = (v1)[0]*(mat)[4]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[6]+(mat)[7];
  (output)[2] = (v1)[0]*(mat)[8]+(v1)[1]*(mat)[9]+(v1)[2]*(mat)[10]+(mat)[11];
}

LUX_INLINE void lxVector3TransformT1( lxVector3 v1, lxMatrix44CPTR mat ) 
{
  lxVector3 vector;

  vector[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[1]+(v1)[2]*(mat)[2]+(mat)[3];
  vector[1] = (v1)[0]*(mat)[4]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[6]+(mat)[7];
  vector[2] = (v1)[0]*(mat)[8]+(v1)[1]*(mat)[9]+(v1)[2]*(mat)[10]+(mat)[11];
  lxVector3Copy(v1,vector);
}

LUX_INLINE void lxVector3TransformRot( lxVector3 output,  const lxVector3 v1,  lxMatrix44CPTR mat )
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(v1)[2]*(mat)[8];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[9];
  (output)[2] = (v1)[0]*(mat)[2]+(v1)[1]*(mat)[6]+(v1)[2]*(mat)[10];
}

LUX_INLINE void lxVector3TransformRot1( lxVector3 v1, lxMatrix44CPTR mat ) 
{
  lxVector3 vector;

  vector[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(v1)[2]*(mat)[8];
  vector[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(v1)[2]*(mat)[9];
  vector[2] = (v1)[0]*(mat)[2]+(v1)[1]*(mat)[6]+(v1)[2]*(mat)[10];
  lxVector3Copy(v1,vector);
}


LUX_INLINE void lxVector3Rad2Deg(lxVector3 dst, lxVector3 src)
{
  (dst)[0]=LUX_RAD2DEG((src)[0]);
  (dst)[1]=LUX_RAD2DEG((src)[1]);
  (dst)[2]=LUX_RAD2DEG((src)[2]);
}

LUX_INLINE void lxVector3Deg2Rad(lxVector3 dst, lxVector3 src)
{
  (dst)[0]=LUX_DEG2RAD((src)[0]);
  (dst)[1]=LUX_DEG2RAD((src)[1]);
  (dst)[2]=LUX_DEG2RAD((src)[2]);
}

LUX_INLINE void lxVector3float_FROM_ubyte( lxVector3 vec3, const uchar ub3[3]) 
{
  (vec3)[0] = (float)(ub3)[0]*LUX_DIV_255;
  (vec3)[1] = (float)(ub3)[1]*LUX_DIV_255;
  (vec3)[2] = (float)(ub3)[2]*LUX_DIV_255;
}

LUX_INLINE void lxVector3ubyte_FROM_float( uchar ub3[3], const lxVector3 vec3) 
{
  LUX_FP_NORM_TO_BYTE((ub3)[0],(vec3)[0]);
  LUX_FP_NORM_TO_BYTE((ub3)[1],(vec3)[1]);
  LUX_FP_NORM_TO_BYTE((ub3)[2],(vec3)[2]);
}

LUX_INLINE void lxVector3float_FROM_short( lxVector3 vec3, const short shrt3[3]) 
{
  (vec3)[0]= LUX_DIV_SHORT*(float)(shrt3)[0];
  (vec3)[1]= LUX_DIV_SHORT*(float)(shrt3)[1];
  (vec3)[2]= LUX_DIV_SHORT*(float)(shrt3)[2];
}

LUX_INLINE void lxVector3short_FROM_float( short shrt3[3], const lxVector3 vec3) 
{
  (shrt3)[0]= (short)(LUX_MUL_SHORT*(vec3)[0]);
  (shrt3)[1]= (short)(LUX_MUL_SHORT*(vec3)[1]);
  (shrt3)[2]= (short)(LUX_MUL_SHORT*(vec3)[2]);
}

LUX_INLINE void lxVector3float_FROM_ushort( lxVector3 vec3, const ushort shrt3[3]) 
{
  (vec3)[0]= LUX_DIV_USHORT*(float)(shrt3)[0];
  (vec3)[1]= LUX_DIV_USHORT*(float)(shrt3)[1];
  (vec3)[2]= LUX_DIV_USHORT*(float)(shrt3)[2];
}

LUX_INLINE void lxVector3ushort_FROM_float( ushort shrt3[3], const lxVector3 vec3) 
{
  (shrt3)[0]= (short)(LUX_MUL_USHORT*(vec3)[0]);
  (shrt3)[1]= (short)(LUX_MUL_USHORT*(vec3)[1]);
  (shrt3)[2]= (short)(LUX_MUL_USHORT*(vec3)[2]);
}

LUX_INLINE void lxVector3ubyte_FROM_ushort( uchar vec3[3], const ushort shrt3[3]) 
{
  (vec3)[0]= (shrt3)[0]>>8;
  (vec3)[1]= (shrt3)[1]>>8;
  (vec3)[2]= (shrt3)[2]>>8;
}

LUX_INLINE void lxVector3ushort_FROM_ubyte( ushort shrt3[3], const uchar vec3[3]) 
{
  (shrt3)[0]= (vec3)[0]<<8;
  (shrt3)[1]= (vec3)[1]<<8;
  (shrt3)[2]= (vec3)[2]<<8;
}

LUX_INLINE void lxVector3pkubyte_FROM_short(uchar pkub3[3], const short shrt3[3]){
  pkub3[0] = ((((int)shrt3[0])/2)+LUX_SHORT_SIGNEDMAX)>>8;
  pkub3[1] = ((((int)shrt3[1])/2)+LUX_SHORT_SIGNEDMAX)>>8;
  pkub3[2] = ((((int)shrt3[2])/2)+LUX_SHORT_SIGNEDMAX)>>8;
}

LUX_INLINE void lxVector3short_FROM_pkubyte(short shrt3[3], const uchar pkub3[3]){
  shrt3[0] = (((short)pkub3[0])-128)*256;
  shrt3[1] = (((short)pkub3[1])-128)*256;
  shrt3[2] = (((short)pkub3[2])-128)*256;
}

LUX_INLINE void lxVector3pkubyte_FROM_float(uchar pkub3[3], const lxVector3 vec3){
  lxVector3 temp;
  lxVector3 offset = {0.5f,0.5f,0.5f};
  lxVector3ScaledAdd(temp,offset,0.5f,vec3);
  lxVector3ubyte_FROM_float(pkub3,temp);
}

LUX_INLINE void lxVector3float_FROM_pkubyte(lxVector3 vec3, const uchar pkub3[3]){
  lxVector3 temp;
  lxVector3 offset = {-1.0f,-1.0f,-1.0f};
  lxVector3float_FROM_ubyte(temp,pkub3);
  lxVector3ScaledAdd(vec3,offset,2.0f,temp);
}

LUX_INLINE void lxVector3Swizzle(lxVector3 out, uint axis[3], lxVector3 dirs)
{
  lxVector3 vector;
  lxVector3Copy(vector,out);
  out[0] = dirs[0]*vector[axis[0]];
  out[1] = dirs[1]*vector[axis[1]];
  out[2] = dirs[2]*vector[axis[2]];
}

#ifdef __cplusplus
}
#endif

#endif
