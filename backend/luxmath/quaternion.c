// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/quaternion.h>
#include <luxinia/luxmath/vector3.h>
#include <luxinia/luxmath/matrix44.h>

//=================================================================
// Quat -- operations
LUX_API void lxQuatSlerp(lxQuat q,const float t, const lxQuat q1, const lxQuat q2)
{
  // perform spherical linear interpolation between q1 and q2
  /*
  // Reference:
  //  "Rotating Objects Using Quats"
  //  by Nick Bobic @ Gamasutra
  //
  // Formula:
  //                    p*sin((1-t)* theta) + q*sin(t* theta)
  //  slerp(t, p, q) = ---------------------------------------
  //                                sin(theta)
  */
  float omega, cosom, sinom, scale0, scale1;

  // calculate cosine
  cosom = (q1[0]*q2[0])+ (q1[1]*q2[1])+ (q1[2]*q2[2])+ (q1[3]*q2[3]);

  // adjust signs (if necessary)
  if (LUX_FP_LESS_ZERO(cosom))
  {
    cosom = -cosom;
    lxQuatSet(q,-q2[0], -q2[1], -q2[2], -q2[3]);
  }
  else
  {
    lxQuatCopy(q,q2);
  }
  // calculate coefficients
  if ((1.0f- cosom) > LUX_FLOAT_EPSILON)
  {
    // standard case (slerp)
    omega = (float)acos(cosom);
    sinom = (float)sin(omega);
    scale0 = (float)sin((1.0f- t)* omega)/ sinom;
    scale1 = (float)sin(t* omega)/ sinom;
  }
  else
  {
    // "from" and "to" Quats are very close
    //  ... so we can do a linear interpolation
    scale0 = 1.0f- t;
    scale1 = t;
  }
  // calculate final values
  q[0] = (scale0* q1[0])+ (scale1* q[0]);
  q[1] = (scale0* q1[1])+ (scale1* q[1]);
  q[2] = (scale0* q1[2])+ (scale1* q[2]);
  q[3] = (scale0* q1[3])+ (scale1* q[3]);
}

LUX_API void lxQuatSlerpLongest(lxQuat q,const float t, const lxQuat q1, const lxQuat q2)
{
  float omega, sinom, scale0, scale1;
  float cosom = lxQuatDot(q1,q2);

  if (LUX_FP_LESS_ZERO(cosom))
  {
    lxQuatCopy(q,q2);
  }
  else
  {
    cosom = -cosom;
    lxQuatSet(q,-q2[0], -q2[1], -q2[2], -q2[3]);
  }

  // calculate coefficients
  if ((1.0f- cosom) > LUX_FLOAT_EPSILON)
  {
    // standard case (slerp)
    omega = (float)acos(cosom);
    sinom = (float)sin(omega);
    scale0 = (float)sin((1.0f- t)* omega)/ sinom;
    scale1 = (float)sin(t* omega)/ sinom;
  }
  else
  {
    // "from" and "to" Quats are very close
    //  ... so we can do a linear interpolation
    scale0 = 1.0f- t;
    scale1 = t;
  }
  // calculate final values
  q[0] = (scale0* q1[0])+ (scale1* q[0]);
  q[1] = (scale0* q1[1])+ (scale1* q[1]);
  q[2] = (scale0* q1[2])+ (scale1* q[2]);
  q[3] = (scale0* q1[3])+ (scale1* q[3]);
}

LUX_API void lxQuatSlerpQuadratic(lxQuat q,const float t, const lxQuat q1, const lxQuat q2, const lxQuat q3, const lxQuat q4)
{
  /*
  // Reference:
  //  Microsoft DirectX 8.1 SDK Documentation
  //
  // Formula:
  //  slerpQuad(t,a,b,c,d) = slerp(2t(1-t), slerp(t, a, d), slerp(t, b, c))
  */
  lxQuat tmp1, tmp2;
  lxQuatSlerp(tmp1,t, q1, q4);
  lxQuatSlerp(tmp2,t, q2, q3);
  lxQuatSlerp(q,2.0f * t * (1.0f - t), tmp1, tmp2);
  lxQuatNormalized(q);
}

void lxQuatSlerpQuadTangents(lxQuat qout,float t, const lxQuat q1, const lxQuat q2, const lxQuat q3, const lxQuat q4) {
  // Contrary to the QuatSlerpQuadric, q1 and q4 are here the outer tangents
  // which are used for quadratic interpolation between q2 and q3
  lxQuat v[2]; // temporary quats

  t = t*0.5f+0.25f;
  if (t<0.5f) {
    lxQuatSlerp(v[0],t+0.5f,q1,q2);
    lxQuatSlerp(v[1],t,q2,q3);
    lxQuatSlerp(qout,t*2.0f, v[0], v[1]);
  } else {
    lxQuatSlerp(v[0],t,q2,q3);
    lxQuatSlerp(v[1],t-0.5f,q3,q4);
    lxQuatSlerp(qout,t*2.0f-1.0f, v[0], v[1]);
  }
  lxQuatNormalized(qout);
}

LUX_API void lxQuatGetBarycentric(lxQuat q,float f, float g, const lxQuat q1, const lxQuat q2, const lxQuat q3)
{
  /*
  // Reference:
  //  Microsoft DirectX 8.1 SDK Documentation
  //
  // Formula:
  //  barycentric(f,g,a,b,c) = slerp(slerp(a, b, f+g), slerp(a, c, f+g), g/(f+g))
  */
  lxQuat tmp1, tmp2;
  lxQuatSlerp(tmp1,f+ g, q1, q2);
  lxQuatSlerp(tmp2,f+ g, q1, q3);
  if (f+ g != 0.0f)
  {
    lxQuatSlerp(q,g/ (f+ g), tmp1, tmp2);
  }
  else
  {
    lxQuatSlerp(q,g, tmp1, tmp2);
  }
  lxQuatNormalized(q);
}


LUX_API void lxQuatFromRotationAxisFast(lxQuat q,const float angle, const lxVector3 a)
{
  /*
  // Reference:
  //  "Rotating Objects Using Quats"
  //  by Nick Bobic @ Gamasutra
  */
  float theta;
  float len;

  theta = angle * 0.5f;
  len = (float)lxFastSqrt(a[0]*a[0]+ a[1]*a[1]+ a[2]*a[2]); // grab length
  if (len > LUX_FLOAT_EPSILON)
  {
    len = (float)lxFastSin(theta) / len;
  }
  else
  {
    len = 1.0f;
  }
  q[0] = a[0]* len;
  q[1] = a[1]* len;
  q[2] = a[2]* len;
  q[3] = lxFastCos(theta);
}

LUX_API void lxQuatToRotationAxis(const lxQuat q,float *degrees, lxVector3 a)
{
  /*
  // Reference:
  //  "Rotating Objects Using Quats"
  //  by Nick Bobic @ Gamasutra
  */
  float len;

  len = (float)sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]); // grab length
  if (len <= LUX_FLOAT_EPSILON) len = 1.0f;
  len = 1.0f / len;
  a[0] = q[0]* len;
  a[1] = q[1]* len;
  a[2] = q[2]* len;
  *degrees = LUX_RAD2DEG(2.0f* (float)acos(q[3]));
}

LUX_API void lxQuatFromEulerXYZFast(lxQuat q, const lxVector3 vec)
{
  float sx, sy, sz, cx,cy,cz,cycz,sysz;

  sx = lxFastSin(vec[0] / 2);
  sy = lxFastSin(vec[1] / 2);
  sz = lxFastSin(vec[2] / 2);
  cx = lxFastCos(vec[0] / 2);
  cy = lxFastCos(vec[1] / 2);
  cz = lxFastCos(vec[2] / 2);

  sysz = sy * sz;
  cycz = cy * cz;

  q[3] = (cx * cycz) + (sx * sysz);
  q[0] = (sx * cycz) - (cx * sysz);
  q[1] = (cx * sy * cz) + (sx * cy * sz);
  q[2] = (cx * cy * sz) - (sx * sy * cz);
  lxQuatNormalized(q);
}

LUX_API void lxQuatFromEuler(lxQuat q, float x, float y, float z)
{
  lxVector3 temp;
  lxVector3Set(temp,x,y,z);
  lxQuatFromEulerXYZFast(q,temp);
}