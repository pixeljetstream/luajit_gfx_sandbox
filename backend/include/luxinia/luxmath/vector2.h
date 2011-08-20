// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_VECTOR2_H__
#define __LUXMATH_VECTOR2_H__

#include <luxinia/luxmath/basetypes.h>
#include <luxinia/luxmath/fastmath.h>

#ifdef __cplusplus
extern "C"{
#endif


LUX_API void lxVector2Set( lxVector2 pV, float x, float y);
LUX_API void lxVector2Copy( lxVector2 pOut, const lxVector2 pV1);
LUX_API void lxVector2Clear( lxVector2 pOut);
LUX_API void lxVector2Scale( lxVector2 pOut, const lxVector2 pV1, float val);

LUX_API void lxVector2Transform( lxVector2 output, const lxVector2 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector2Transform1( lxVector2 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector2TransformRot( lxVector2 output, const lxVector2 v1,  lxMatrix44CPTR mat );
LUX_API void lxVector2TransformRot1( lxVector2 output, lxMatrix44CPTR mat );
LUX_API void lxVector2InvTransform( lxVector2 output, const lxVector2 v1,  lxMatrix44CPTR mat ) ;
LUX_API void lxVector2InvTransform1( lxVector2 v1,  lxMatrix44CPTR mat );

LUX_API float lxVector2Length( const lxVector2 pV );
LUX_API float lxVector2LengthFast( const lxVector2 pV );
LUX_API float lxVector2SqLength( const lxVector2 pV );
LUX_API float lxVector2Dot( const lxVector2 pV1, const lxVector2 pV2 );
LUX_API float lxVector2Normalize( lxVector2 pOut, const lxVector2 pV1 );
LUX_API float lxVector2NormalizeFast( lxVector2 pOut, const lxVector2 pV1 );
LUX_API float lxVector2Normalized( lxVector2 pOut );
LUX_API float lxVector2NormalizedFast( lxVector2 pOut );
//////////////////////////////////////////////////////////////////////////


LUX_INLINE void lxVector2Set( lxVector2 pV, float x, float y)
{
  pV[0] = x;
  pV[1] = y;
}
LUX_INLINE void lxVector2Copy( lxVector2 pOut, const lxVector2 pV1)
{
  pOut[0] = pV1[0];
  pOut[1] = pV1[1];
}

LUX_INLINE void lxVector2Clear( lxVector2 pOut)
{
  pOut[0] = 0.0f;
  pOut[1] = 0.0f;
}

LUX_INLINE void lxVector2Scale( lxVector2 pOut, const lxVector2 pV1, float val)
{
  pOut[0] = pV1[0] * val;
  pOut[1] = pV1[1] * val;
}

LUX_INLINE float lxVector2Length( const lxVector2 pV )
{
#ifdef __cplusplus
  return sqrtf(pV[0] * pV[0] + pV[1] * pV[1]);
#else
  return (float) sqrt(pV[0] * pV[0] + pV[1]);
#endif
}

LUX_INLINE float lxVector2LengthFast( const lxVector2 pV )
{
  return lxFastSqrt(pV[0] * pV[0] + pV[1] * pV[1]);
}

LUX_INLINE float lxVector2SqLength( const lxVector2 pV )
{
  return pV[0] * pV[0] + pV[1] * pV[1] + pV[2];
}

LUX_INLINE float lxVector2Dot( const lxVector2 pV1, const lxVector2 pV2 )
{

  return pV1[0] * pV2[0] + pV1[1] * pV2[1];
}

LUX_INLINE float lxVector2Normalize( lxVector2 pOut, const lxVector2 pV1 )
{
  float length = lxVector2Length(pV1);
  float lengthdiv = (length < 0.0001f) ? 1.0f : 1.0f/length;

  pOut[0] = pV1[0] * lengthdiv;
  pOut[1] = pV1[1] * lengthdiv;

  return length;
}

LUX_INLINE float lxVector2NormalizeFast( lxVector2 pOut, const lxVector2 pV1 )
{
  float lengthdiv = lxFastSqrtRcp(lxVector2SqLength(pV1));
  pOut[0] = pV1[0] * lengthdiv;
  pOut[1] = pV1[1] * lengthdiv;

  return 1.0f/lengthdiv;
}

LUX_INLINE float lxVector2Normalized( lxVector2 pOut )
{
  float length = lxVector2Length(pOut);
  float lengthdiv = (length < 0.0001f) ? 1.0f : 1.0f/length;

  pOut[0] = pOut[0] * lengthdiv;
  pOut[1] = pOut[1] * lengthdiv;

  return length;
}

LUX_INLINE float lxVector2NormalizedFast( lxVector2 pOut )
{
  float lengthdiv = lxFastSqrtRcp(lxVector2SqLength(pOut));
  pOut[0] = pOut[0] * lengthdiv;
  pOut[1] = pOut[1] * lengthdiv;

  return 1.0f/lengthdiv;
}

LUX_INLINE void lxVector2Transform( lxVector2 output, const lxVector2 v1,  lxMatrix44CPTR mat ) 
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(mat)[12];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(mat)[13];
}

LUX_INLINE void lxVector2Transform1( lxVector2 v1,  lxMatrix44CPTR mat ) 
{
  lxVector2 output;

  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(mat)[12];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(mat)[13];
  lxVector2Copy(v1,output);
}

LUX_INLINE void lxVector2TransformRot( lxVector2 output, const lxVector2 v1,  lxMatrix44CPTR mat ) 
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5];
}

LUX_INLINE void lxVector2TransformRot1( lxVector2 v1,  lxMatrix44CPTR mat ) 
{
  lxVector2 output;

  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5];
  lxVector2Copy(v1,output);
}

LUX_INLINE void lxVector2InvTransform( lxVector2 output, const lxVector2 v1,  lxMatrix44CPTR mat ) 
{
  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[1]-(mat[0]*mat[12]+mat[1]*mat[13]+mat[2]*mat[14]);
  (output)[1] = (v1)[0]*(mat)[4]+(v1)[1]*(mat)[5]-(mat[4]*mat[12]+mat[5]*mat[13]+mat[6]*mat[14]);
}

LUX_INLINE void lxVector2InvTransform1( lxVector2 v1,  lxMatrix44CPTR mat ) 
{
  lxVector2 output;

  (output)[0] = (v1)[0]*(mat)[0]+(v1)[1]*(mat)[4]+(mat)[12];
  (output)[1] = (v1)[0]*(mat)[1]+(v1)[1]*(mat)[5]+(mat)[13];
  lxVector2Copy(v1,output);
}

#ifdef __cplusplus
}
#endif

#endif

