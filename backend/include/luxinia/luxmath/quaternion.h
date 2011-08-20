// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_QUATERNION_H__
#define __LUXMATH_QUATERNION_H__

#include <luxinia/luxmath/basetypes.h>
#include <luxinia/luxmath/fastmath.h>
#include <luxinia/luxmath/vector3.h>
#include <luxinia/luxmath/matrix44.h>

#ifdef __cplusplus
extern "C"{
#endif


LUX_API void lxQuatSet( lxQuat pV, float x, float y, float z, float w);
LUX_API void lxQuatCopy( lxQuat pOut, const lxQuat pV1);
LUX_API void lxQuatClear( lxQuat pOut);
LUX_API void lxQuatIdentity( lxQuat pOut);
LUX_API void lxQuatAdd(lxQuat q, const lxQuat q1, const lxQuat q2);
LUX_API void lxQuatAdd1(lxQuat q1,const lxQuat q);
LUX_API void lxQuatSub(lxQuat q, const lxQuat q1, const lxQuat q2);
LUX_API void lxQuatSub1(lxQuat q,const lxQuat q1);
LUX_API void lxQuatScale(lxQuat qout, const lxQuat q,const float value);

LUX_API void lxQuatMul(lxQuat q,const lxQuat q2,const lxQuat q1);
LUX_API void lxQuatMul1(lxQuat q2,const lxQuat q1);
LUX_API void lxQuatMul2(const lxQuat q2,lxQuat q1);

LUX_API void lxQuatToMatrix(const lxQuat q,lxMatrix44PTR a_mat);
LUX_API void lxQuatToMatrixIdentity(const lxQuat q,lxMatrix44PTR a_mat);
LUX_API void lxQuatFromMatrix(lxQuat q1,lxMatrix44CPTR in_mat);
LUX_API void lxQuatToRotationAxis(const lxQuat q,float *degrees, lxVector3 a);
LUX_API void lxQuatFromRotationAxisFast(lxQuat q,const float angle, const lxVector3 a);
LUX_API void lxQuatFromEulerFast(lxQuat q, float x, float y, float z);
LUX_API void lxQuatFromEulerXYZFast(lxQuat q, const lxVector3 vec);

LUX_API void lxQuatTransformVector(const lxQuat q, const lxVector3 v, lxVector3 out);

LUX_API float lxQuatLength(const lxQuat q);
LUX_API float lxQuatLengthSqr(const lxQuat q);
LUX_API float lxQuatDot(const lxQuat q,const lxQuat q2);

LUX_API void lxQuatNormalized(lxQuat q);
LUX_API void lxQuatInverted(lxQuat q);
LUX_API void lxQuatConjugate(lxQuat q1, const lxQuat q2);
LUX_API void lxQuatNaturalLog(lxQuat q1, const lxQuat q);
LUX_API void lxQuatExponent(lxQuat q1,const lxQuat q);

LUX_API void lxQuatSlerp(lxQuat q,const float t, const lxQuat q1, const lxQuat q2);
LUX_API void lxQuatSlerpLongest(lxQuat q,const float t, const lxQuat q1, const lxQuat q2);
LUX_API void lxQuatSlerpQuadratic(lxQuat q,const float t, const lxQuat q1, const lxQuat q2, const lxQuat q3, const lxQuat q4);
LUX_API void lxQuatSlerpQuadTangents(lxQuat qout,float t, const lxQuat q1, const lxQuat q2, const lxQuat q3, const lxQuat q4);
LUX_API void lxQuatGetBarycentric(lxQuat q,float f, float g, const lxQuat q1, const lxQuat q2, const lxQuat q3);

LUX_API void lxQuatSwizzle(lxQuat out, uint axis[3], lxVector3 dirs);

//////////////////////////////////////////////////////////////////////////

typedef struct Quat_s {
  float x;
  float y;
  float z;
  float w;
} Quat_t;

//--------------------------
// 3D Vector
//--------------------------

LUX_INLINE void lxQuatSet( lxQuat pV, float x, float y, float z, float w)
{
  pV[0] = x;
  pV[1] = y;
  pV[2] = z;
  pV[3] = w;
}

LUX_INLINE void lxQuatCopy( lxQuat pOut, const lxQuat pV1)
{
  pOut[0] = pV1[0];
  pOut[1] = pV1[1];
  pOut[2] = pV1[2];
  pOut[3] = pV1[3];
}

LUX_INLINE void lxQuatClear( lxQuat pOut)
{
  pOut[0] = 0.0f;
  pOut[1] = 0.0f;
  pOut[2] = 0.0f;
  pOut[3] = 0.0f;
}

LUX_INLINE void lxQuatIdentity( lxQuat pOut)
{
  pOut[0] = 0.0f;
  pOut[1] = 0.0f;
  pOut[2] = 0.0f;
  pOut[3] = 1.0f;
}

LUX_INLINE void lxQuatAdd1(lxQuat q1,const lxQuat q)
{
  q1[0]+= q[0];
  q1[1]+= q[1];
  q1[2]+= q[2];
  q1[3]+= q[3];
}

LUX_INLINE void lxQuatAdd(lxQuat q, const lxQuat q1, const lxQuat q2)
{
  q[0] = q1[0]+ q2[0];
  q[1] = q1[1]+ q2[1];
  q[2] = q1[2]+ q2[2];
  q[3] = q1[3]+ q2[3];
}

LUX_INLINE void lxQuatSub1(lxQuat q,const lxQuat q1)
{
  q[0]-= q1[0];
  q[1]-= q1[1];
  q[2]-= q1[2];
  q[3]-= q1[3];
}

LUX_INLINE void lxQuatSub(lxQuat q, const lxQuat q1, const lxQuat q2)
{
  q[0] = q1[0]- q2[0];
  q[1] = q1[1]- q2[1];
  q[2] = q1[2]- q2[2];
  q[3] = q1[3]- q2[3];
}

LUX_INLINE void lxQuatScale(lxQuat qout, const lxQuat q,const float value)
{
  // scale a Quat
  qout[0] = q[0]* value;
  qout[0] = q[1]* value;
  qout[0] = q[2]* value;
  qout[0] = q[3]* value;
}

LUX_INLINE void lxQuatMul1(lxQuat q2,const lxQuat q1)
{
  float A, B, C, D, E, F, G, H;
  A = (q1[3]+ q1[0])* (q2[3]+ q2[0]);
  B = (q1[2]- q1[1])* (q2[1]- q2[2]);
  C = (q1[3]- q1[0])* (q2[1]+ q2[2]);
  D = (q1[1]+ q1[2])* (q2[3]- q2[0]);
  E = (q1[0]+ q1[2])* (q2[0]+ q2[1]);
  F = (q1[0]- q1[2])* (q2[0]- q2[1]);
  G = (q1[3]+ q1[1])* (q2[3]- q2[2]);
  H = (q1[3]- q1[1])* (q2[3]+ q2[2]);
  q2[0] = A- ( E+ F+ G+ H)* 0.5f;
  q2[1] = C+ ( E- F+ G- H)* 0.5f;
  q2[2] = D+ ( E- F- G+ H)* 0.5f;
  q2[3] = B+ (-E- F+ G+ H)* 0.5f;
}

LUX_INLINE void lxQuatMul2(const lxQuat q2, lxQuat q1)
{
  float A, B, C, D, E, F, G, H;
  A = (q1[3]+ q1[0])* (q2[3]+ q2[0]);
  B = (q1[2]- q1[1])* (q2[1]- q2[2]);
  C = (q1[3]- q1[0])* (q2[1]+ q2[2]);
  D = (q1[1]+ q1[2])* (q2[3]- q2[0]);
  E = (q1[0]+ q1[2])* (q2[0]+ q2[1]);
  F = (q1[0]- q1[2])* (q2[0]- q2[1]);
  G = (q1[3]+ q1[1])* (q2[3]- q2[2]);
  H = (q1[3]- q1[1])* (q2[3]+ q2[2]);
  q1[0] = A- ( E+ F+ G+ H)* 0.5f;
  q1[1] = C+ ( E- F+ G- H)* 0.5f;
  q1[2] = D+ ( E- F- G+ H)* 0.5f;
  q1[3] = B+ (-E- F+ G+ H)* 0.5f;
}

LUX_INLINE void lxQuatToMatrix(const lxQuat q,lxMatrix44PTR a_mat)
{
  // Quat to mat conversion, calculate coefficients
  float x2 = q[0]+ q[0];
  float y2 = q[1]+ q[1];
  float z2 = q[2]+ q[2];
  float xx = q[0]* x2;
  float xy = q[0]* y2;
  float xz = q[0]* z2;
  float yy = q[1]* y2;
  float yz = q[1]* z2;
  float zz = q[2]* z2;
  float wx = q[3]* x2;
  float wy = q[3]* y2;
  float wz = q[3]* z2;

  // fill mat
  a_mat[0] = 1.0f- (yy+ zz);
  a_mat[4] = xy- wz;
  a_mat[8] = xz+ wy;

  a_mat[1] = xy+ wz;
  a_mat[5] = 1.0f- (xx+ zz);
  a_mat[9] = yz- wx;

  a_mat[2] = xz- wy;
  a_mat[6] = yz+ wx;
  a_mat[10]= 1.0f- (xx+ yy);
}

LUX_INLINE void lxQuatToMatrixIdentity(const lxQuat q,lxMatrix44PTR a_mat)
{
  lxQuatToMatrix(q,a_mat);

  // fill mat
  a_mat[3] = 0.0f;
  a_mat[7] = 0.0f;
  a_mat[11] = 0.0f;

  a_mat[12] = 0.0f;
  a_mat[13] = 0.0f;
  a_mat[14] = 0.0f;
  a_mat[15] = 1.0f;

}

LUX_INLINE void lxQuatMul(lxQuat qout, const lxQuat q2, const lxQuat q1)
{
  // mat like Quat multiply, same as Mul but
  // instead of (q1*q2) we do (q2*q1) to keep the same
  // order of operations as the mat Multiply()
  /*
  // Reference:
  //  "Rotating Objects Using Quats"
  //  by Nick Bobic @ Gamasutra
  */
  // same as above, but outputs to a user defined Quat
  float A, B, C, D, E, F, G, H;
  A = (q2[3]+ q2[0])* (q1[3]+ q1[0]);
  B = (q2[2]- q2[1])* (q1[1]- q1[2]);
  C = (q2[3]- q2[0])* (q1[1]+ q1[2]);
  D = (q2[1]+ q2[2])* (q1[3]- q1[0]);
  E = (q2[0]+ q2[2])* (q1[0]+ q1[1]);
  F = (q2[0]- q2[2])* (q1[0]- q1[1]);
  G = (q2[3]+ q2[1])* (q1[3]- q1[2]);
  H = (q2[3]- q2[1])* (q1[3]+ q1[2]);
  qout[0] = A- ( E+ F+ G+ H)* 0.5f;
  qout[1] = C+ ( E- F+ G- H)* 0.5f;
  qout[2] = D+ ( E- F- G+ H)* 0.5f;
  qout[3] = B+ (-E- F+ G+ H)* 0.5f;
}

LUX_INLINE void lxQuatTransformVector(const lxQuat q, const lxVector3 v, lxVector3 out)
{

  /*
  sandwich transform
  QvQ~ is
  vresult = v + 2 * cross(( v * Q.w - cross( v, Q.xyz ) ), Q.xyz );
  or Q~vQ
  vresult = v + 2 * cross( Q.xyz, (cross( v, Q.xyz ) - v * Q.w ) );
  */

  /*
  quaternion q( v.x * w + v.z * y - v.y * z,
  v.y * w + v.x * z - v.z * x,
  v.z * w + v.y * x - v.x * y,
  v.x * x + v.y * y + v.z * z);

  return vector3( w * q.x + x * q.w + y * q.z - z * q.y,
  w * q.y + y * q.w + z * q.x - x * q.z,
  w * q.z + z * q.w + x * q.y - y * q.x);
  */

  /*
  newx = a  (xx - yy - zz + ww) + 2 (w (yc - zb) + x (yb + zc))
  newy = b (-xx + yy - zz + ww) + 2 (w (za - xc) + y (xa + zc))
  newz = c (-xx - yy + zz + ww) + 2 (w (xb - ya) + z (xa + yb))
  */


  lxMatrix44 mat;
  lxQuatToMatrixIdentity(q,mat);
  lxVector3TransformRot(out,v,mat);
}

LUX_INLINE float lxQuatLength(const lxQuat q)
{
  return ((float)sqrt(q[0]*q[0]+ q[1]*q[1]+ q[2]*q[2]+ q[3]*q[3]));
}

LUX_INLINE float lxQuatLengthSqr(const lxQuat q)
{
  return (q[0]*q[0]+ q[1]*q[1]+ q[2]*q[2]+ q[3]*q[3]);
}

LUX_INLINE float lxQuatDot(const lxQuat q,const lxQuat q2)
{
  return (q[0]*q2[0]+ q[1]*q2[1]+ q[2]*q2[2]+ q[3]*q2[3]);
}

LUX_INLINE void lxQuatNormalized(lxQuat q)
{
  // normalizing a Quat is like normalizing
  // a generic 4-dimensional vector
  float len;

  len = lxQuatLength(q); // grab length

  // be carefull with floating point range
  if (len >= LUX_FLOAT_EPSILON)
  {
    // use the reciprocal to save us some cycles,
    // 1 div and 4 muls is faster than 4 divides
    len = 1.0f / len;
    q[0]*= len;
    q[1]*= len;
    q[2]*= len;
    q[3]*= len;
  }
}

LUX_INLINE void lxQuatInverted(lxQuat q)
{
  // formula [inv(q)] = [conjugate(q) / lengthsq(Q)]
  // save us some cycles with the reciprocal
  float tmp;

  tmp = 1.0f / lxQuatLengthSqr(q);
  q[0] = -q[0]* tmp;
  q[1] = -q[1]* tmp;
  q[2] = -q[2]* tmp;
  q[3] =  q[3]* tmp;
}

LUX_INLINE void lxQuatConjugate(lxQuat q1, const lxQuat q2)
{
  // formula [conjugate(q)] = [-x, -y, -z, w]
  q1[0] = -q2[0];
  q1[1] = -q2[1];
  q1[2] = -q2[2];
  q1[3] = q2[3];
}

LUX_INLINE void lxQuatNaturalLog(lxQuat q1, const lxQuat q)
{
  // formula [ln(q)] = [(0, theta* v)]
  float len;

  len = (float)sqrt(q[0]*q[0]+ q[1]*q[1]+ q[2]*q[2]+ q[3]*q[3]); // grab length

  // avoid div by 0
  if (q[3] != 0.0f)
  {
    len = (float)atan(len / q[3]);
  }
  else
  {
    len = (LUX_MUL_PI* 0.5f);
  }
  q1[0] = q[0]* len;
  q1[1] = q[1]* len;
  q1[2] = q[2]* len;
  q1[3] = 0.0f;
}

LUX_INLINE void lxQuatExponent(lxQuat q1,const lxQuat q)
{
  // formula [exp(q)] = [(cos(theta), sin(theta)* v)]
  float tmp, len;

  len = (float)sqrt(q[0]*q[0]+ q[1]*q[1]+ q[2]*q[2]);

  // avoid div by 0
  if (len >= LUX_FLOAT_EPSILON)
  {
    tmp = (float)sin(len) / len;
  }
  else
  {
    tmp = 1.0f;
  }
  q1[0] = q[0]* tmp;
  q1[1] = q[0]* tmp;
  q1[2] = q[0]* tmp;
  q1[3] = (float)cos(tmp);
}

LUX_INLINE void lxQuatFromMatrix(lxQuat q1,lxMatrix44CPTR in_mat)
{
  // mat to Quat conversion
  lxMatrix44 a_mat;
  float tr, s;
  int i, j, k;
  int nxt[3] = {1, 2, 0};

  lxMatrix44CopyRotTransposed(a_mat,in_mat);

  tr = a_mat[0]+ a_mat[5]+ a_mat[10];

  // check the diagonal
  if (LUX_FP_GREATER_ZERO(tr))
  {
    tr   += 1.0f;
    s  = (1.0f/lxFastSqrt(tr)) * 0.5f;

    q1[3] = s * tr;
    q1[0] = (a_mat[9]- a_mat[6])* s;
    q1[1] = (a_mat[2]- a_mat[8])* s;
    q1[2] = (a_mat[4]- a_mat[1])* s;
  }
  else
  {
    // diagonal is negative
    i = 0;
    if (a_mat[5] > a_mat[0]) i = 1;
    if (a_mat[10] > a_mat[i*5]) i = 2;
    j = nxt[i];
    k = nxt[j];
    tr = (a_mat[i*5]- (a_mat[j*5]+ a_mat[k*5]))+ 1.0f;

    s = (1.0f/lxFastSqrt(tr)) * 0.5f;

    q1[i] = s * tr;
    q1[3] = (a_mat[j+k*4]- a_mat[k+j*4])* s;
    q1[j] = (a_mat[i+j*4]+ a_mat[j+i*4])* s;
    q1[k] = (a_mat[i+k*4]+ a_mat[k+i*4])* s;
  }
}

LUX_INLINE void lxQuatSwizzle(lxQuat out, uint axis[3], lxVector3 dirs)
{
  lxVector3 vector;
  lxVector3Copy(vector,out);
  out[0] = dirs[0]*vector[axis[0]];
  out[1] = dirs[1]*vector[axis[1]];
  out[2] = dirs[2]*vector[axis[2]];
  out[3] = dirs[3]*out[3];
}

#ifdef __cplusplus
}
#endif

#endif
