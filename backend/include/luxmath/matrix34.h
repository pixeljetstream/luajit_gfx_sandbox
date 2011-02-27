// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt



#ifndef __LUXMATH_MATRIX34_H__
#define __LUXMATH_MATRIX34_H__

#include <luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif

  //  3 Rows (xyz), 4 Columns (X,Y,Z,T)

LUX_API const lxMatrix34PTR lxMatrix34GetIdentity();
LUX_API void lxMatrix34Copy(lxMatrix34PTR dst, const lxMatrix34PTR src);
LUX_API void lxMatrix34Identity(lxMatrix34PTR mat);
LUX_API void lxMatrix34TMultiply44( lxMatrix34PTR dst, const lxMatrix44PTR mat1,  const lxMatrix44PTR mat2 );

//////////////////////////////////////////////////////////////////////////

#ifdef LUX_COMPILER_MSC
#pragma warning( push )
#pragma warning( disable : 4028 )
#endif

LUX_API extern const float lx_gMatrix34_ident[12];

LUX_INLINE void lxMatrix34Copy(lxMatrix34PTR dst, const lxMatrix34PTR src)
{
  (dst)[0]  = (src)[0];  (dst)[1]  = (src)[1];  
  (dst)[2]  = (src)[2];  (dst)[3]  = (src)[3];  
  (dst)[4]  = (src)[4];  (dst)[5]  = (src)[5];  
  (dst)[6]  = (src)[6];  (dst)[7]  = (src)[7];  
  (dst)[8]  = (src)[8];  (dst)[9]  = (src)[9];  
  (dst)[10] = (src)[10]; (dst)[11] = (src)[11]; 
}

LUX_INLINE const lxMatrix34PTR lxMatrix34GetIdentity()
{
  return (const lxMatrix34PTR)lx_gMatrix34_ident;
}

LUX_INLINE void lxMatrix34Identity(lxMatrix34PTR m_mat)
{
  lxMatrix34Copy(m_mat,lxMatrix34GetIdentity());
}

LUX_INLINE void lxMatrix34TMultiply44( lxMatrix34PTR newmat, const lxMatrix44PTR m_mat,  const lxMatrix44PTR mat )
{
  const float *a_mat = m_mat;
  const float *b_mat = mat;

  newmat[0] = a_mat[0]*b_mat[0] + a_mat[4]*b_mat[1] + a_mat[8]*b_mat[2];
  newmat[4] = a_mat[1]*b_mat[0] + a_mat[5]*b_mat[1] + a_mat[9]*b_mat[2];
  newmat[8] = a_mat[2]*b_mat[0] + a_mat[6]*b_mat[1] + a_mat[10]*b_mat[2];

  newmat[1] = a_mat[0]*b_mat[4] + a_mat[4]*b_mat[5] + a_mat[8]*b_mat[6];
  newmat[5] = a_mat[1]*b_mat[4] + a_mat[5]*b_mat[5] + a_mat[9]*b_mat[6];
  newmat[9] = a_mat[2]*b_mat[4] + a_mat[6]*b_mat[5] + a_mat[10]*b_mat[6];

  newmat[2] = a_mat[0]*b_mat[8] + a_mat[4]*b_mat[9] + a_mat[8]*b_mat[10];
  newmat[6] = a_mat[1]*b_mat[8] + a_mat[5]*b_mat[9] + a_mat[9]*b_mat[10];
  newmat[10] = a_mat[2]*b_mat[8] + a_mat[6]*b_mat[9] + a_mat[10]*b_mat[10];

  newmat[3] = a_mat[0]*b_mat[12] + a_mat[4]*b_mat[13] + a_mat[8]*b_mat[14] + a_mat[12];
  newmat[7] = a_mat[1]*b_mat[12] + a_mat[5]*b_mat[13] + a_mat[9]*b_mat[14] + a_mat[13];
  newmat[11] = a_mat[2]*b_mat[12] + a_mat[6]*b_mat[13] + a_mat[10]*b_mat[14] + a_mat[14];
}

#ifdef LUX_COMPILER_MSC
#pragma warning( pop )
#endif

#ifdef __cplusplus
}
#endif

#endif
