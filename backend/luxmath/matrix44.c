// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/matrix44.h>
#include <luxinia/luxmath/vector3.h>
#include <luxinia/luxmath/vector4.h>


const LUX_ALIGNSIMD_V(float lx_gMatrix44_ident[16]) =
{
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f,
};

LUX_API void lxMatrix44FromEulerZYXFast( lxMatrix44PTR m_mat, lxVector3CPTR angles )
{
  float sxsy, cxsy;

  float cx = lxFastCos( angles[0] );
  float sx = lxFastSin( angles[0] );
  float cy = lxFastCos( angles[1] );
  float sy = lxFastSin( angles[1] );
  float cz = lxFastCos( angles[2] );
  float sz = lxFastSin( angles[2] );

  m_mat[0] = ( cy*cz );
  m_mat[1] = ( cy*sz );
  m_mat[2] = ( -sy );

  sxsy = sx*sy;
  cxsy = cx*sy;

  m_mat[4] = ( sxsy*cz-cx*sz );
  m_mat[5] = ( sxsy*sz+cx*cz );
  m_mat[6] = ( sx*cy );

  m_mat[8] = ( cxsy*cz+sx*sz );
  m_mat[9] = ( cxsy*sz-sx*cz );
  m_mat[10] = ( cx*cy );
}

LUX_API void lxMatrix44FromEulerXYZFast(lxMatrix44PTR m_mat, lxVector3PTR angles)
{
  float sxsy, cxsy;

  float cx = lxFastCos( angles[0] );
  float sx = lxFastSin( angles[0] );
  float cy = lxFastCos( angles[1] );
  float sy = lxFastSin( angles[1] );
  float cz = lxFastCos( angles[2] );
  float sz = lxFastSin( angles[2] );

  // rot =  cy*cz          -cy*sz           sy
  //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
  //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

  m_mat[0] = ( cy*cz );
  m_mat[4] = ( -cy*sz );
  m_mat[8] = ( sy );

  sxsy = sx*sy;
  cxsy = cx*sy;

  m_mat[1] = ( sxsy*cz+cx*sz );
  m_mat[5] = ( cx*cz-sxsy*sz );
  m_mat[9] = ( -sx*cy );

  m_mat[2] = ( -cxsy*cz+sx*sz );
  m_mat[6] = ( sx*cz+cxsy*sz );
  m_mat[10] = ( cx*cy );
}

LUX_API void lxMatrix44FromEulerZYX( lxMatrix44PTR m_mat, lxVector3CPTR angles )
{
  float sxsy, cxsy;

  float cx = cosf( angles[0] );
  float sx = sinf( angles[0] );
  float cy = cosf( angles[1] );
  float sy = sinf( angles[1] );
  float cz = cosf( angles[2] );
  float sz = sinf( angles[2] );

  m_mat[0] = ( cy*cz );
  m_mat[1] = ( cy*sz );
  m_mat[2] = ( -sy );

  sxsy = sx*sy;
  cxsy = cx*sy;

  m_mat[4] = ( sxsy*cz-cx*sz );
  m_mat[5] = ( sxsy*sz+cx*cz );
  m_mat[6] = ( sx*cy );

  m_mat[8] = ( cxsy*cz+sx*sz );
  m_mat[9] = ( cxsy*sz-sx*cz );
  m_mat[10] = ( cx*cy );
}

LUX_API void lxMatrix44FromEulerXYZ(lxMatrix44PTR m_mat, lxVector3PTR angles)
{
  float sxsy, cxsy;

  float cx = cosf( angles[0] );
  float sx = sinf( angles[0] );
  float cy = cosf( angles[1] );
  float sy = sinf( angles[1] );
  float cz = cosf( angles[2] );
  float sz = sinf( angles[2] );

  // rot =  cy*cz          -cy*sz           sy
  //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
  //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

  m_mat[0] = ( cy*cz );
  m_mat[4] = ( -cy*sz );
  m_mat[8] = ( sy );

  sxsy = sx*sy;
  cxsy = cx*sy;

  m_mat[1] = ( sxsy*cz+cx*sz );
  m_mat[5] = ( cx*cz-sxsy*sz );
  m_mat[9] = ( -sx*cy );

  m_mat[2] = ( -cxsy*cz+sx*sz );
  m_mat[6] = ( sx*cz+cxsy*sz );
  m_mat[10] = ( cx*cy );
}

LUX_API void lxMatrix44FromEulerZYXdeg( lxMatrix44PTR m_mat, lxVector3CPTR angles )
{
  float vec[3];
  vec[0] = ( float )( LUX_DEG2RAD(angles[0]) );
  vec[1] = ( float )( LUX_DEG2RAD(angles[1]) );
  vec[2] = ( float )( LUX_DEG2RAD(angles[2]) );
  lxMatrix44FromEulerZYX(m_mat, vec );
}

LUX_API void lxMatrix44ToEulerZYX( lxMatrix44CPTR mat, lxVector3PTR angles){
  if ( mat[2] < 0.998f)
  {
    if ( mat[2] > -0.998f ) 
    {
      angles[2] = (float)atan2((double)mat[1],(double)mat[0]);
      angles[1] = (float)asin(-(double)mat[2]);
      angles[0] = (float)atan2((double)mat[6],(double)mat[10]);
    }
    else
    {
      angles[2] = -(float)atan2(mat[4],mat[8]);
      angles[1] = LUX_MUL_HALF_PI;
      angles[0] = 0.0f;
    }
  }
  else
  {
    angles[2] = (float)atan2(-mat[4],-mat[8]);
    angles[1] = -LUX_MUL_HALF_PI;
    angles[0] = 0.0f;
  }
}

LUX_API void lxMatrix44ToEulerXYZ( lxMatrix44CPTR a_mat, lxVector3PTR angles)
{
#if 0
  double angle_y = D =  asin( mat[8]); 
  double C           =  cos( angle_y );
  angles[1] = angle_y;
  if ( fabs( C ) > 0.005 )             /* Gimball lock? */
  {
    angles[0]  = atan2( -mat[9]  / C, mat[10] / C );
    angles[2]  = atan2( -mat[4] / C, mat[0] / C );
  }
  else                                 /* Gimball lock has occurred */
  {
    angles[0]  = 0; 
    angles[2]  = atan2( mat[1], mat[5] );
  }
#else
  if ( a_mat[8] < 0.998f )
  {
    if ( a_mat[8] > -0.998f ) 
    {
      angles[0] = (float)atan2(-(double)a_mat[9],(double)a_mat[10]);
      angles[1] = (float)asin((double)a_mat[8]);
      angles[2] = (float)atan2(-(double)a_mat[4],(double)a_mat[0]);
    }
    else
    {
      angles[0] = -(float)atan2(a_mat[1],a_mat[5]);
      angles[1] = -LUX_MUL_HALF_PI;
      angles[2] = 0.0f;
    }
  }
  else
  {
    angles[0] = (float)atan2(a_mat[1],a_mat[5]);
    angles[1] = LUX_MUL_HALF_PI;
    angles[2] = 0.0f;
  }
#endif
}



LUX_API void lxMatrix44Perspective(lxMatrix44PTR mat, const float fov, const float front, const float back, const float aspect){
  float sine, cotangent, deltaZ;
  float radians;

  radians = fov / 2.0f * LUX_MUL_PI / 180.0f;

  deltaZ = back - front;
  sine = sinf(radians);
  if ((deltaZ == 0.0f) || (sine == 0.0f) || (aspect == 0)) {
    return;
  }
  cotangent = cosf(radians) / sine;

  lxMatrix44Identity(mat);
  mat[0] = cotangent / aspect;
  mat[5] = cotangent;
  mat[10] = -(back + front) / deltaZ;
  mat[11] = -1.0f;
  mat[14] = -2.0f * front * back / deltaZ;
  mat[15] = 0.0f;
}
LUX_API void lxMatrix44PerspectiveInf(lxMatrix44PTR mat, const float fov, const float front, const float aspect){
  float sine, cotangent;
  float radians;

  radians = fov / 2.0f * LUX_MUL_PI / 180.0f;

  sine = sinf(radians);
  if ((sine == 0.0f) || (aspect == 0.0f)) {
    return;
  }
  cotangent = cosf(radians) / sine;

  lxMatrix44Identity(mat);
  mat[0] = cotangent / aspect;
  mat[5] = cotangent;
  mat[10] = -1.0f;
  mat[11] = -1.0f;
  mat[14] = -2.0f * front;
  mat[15] = 0.0f;
}

LUX_API void lxMatrix44OrthoDirect(lxMatrix44PTR mat, const float left, const float right, const float bottom, const float top, const float front, const float back){
  lxMatrix44Identity(mat);
  mat[0] = 2.0f/(right-left);
  mat[5] = 2.0f/(top-bottom);
  mat[10]= -2.0f/(back-front);
  mat[12]= -(right + left)/(right-left);
  mat[13]= -(top + bottom)/(top-bottom);
  mat[14]= -(back + front)/(back - front);
}

void lxMatrix44Ortho(lxMatrix44PTR mat, const float height, const float front, const float back, const float aspect){
  float width;
  float right;
  float left;
  float top;
  float bottom;

  width = height * aspect;
  right  = width * 0.5f;
  left = - right;
  top = height * 0.5f;
  bottom = - top;

  lxMatrix44OrthoDirect(mat,left,right,bottom,top,front,back);
}

#define sgn(flt)  ((flt < 0.0f) ? -1.0f : ((flt > 0.0f) ? 1.0f : 0.0f ))
LUX_API void lxMatrix44ModifyProjectionClipplane(lxMatrix44PTR projmatrix,lxMatrix44CPTR mview, lxMatrix44CPTR mviewinv, lxVector4CPTR clipPlane)
{
  lxVector4   q;
  lxVector4   eyeplane;
  lxMatrix44  mviewinvT;
  float val;
  /*
  You need to multiply the plane's normal (in your case [0 -1 0 1]) by the inverse view matrix and then normalize it (only the xyz part leave the w out of it since you need it set to 1.0f for later).
  Then you need to transform a point on the water plan again by the inverse view matrix.
  Once you have those two under your belly, the new water plane will be something like (normal.xyz, -dot4(transformedPointOnAPlane, normal)).
  Use that plane to modify your projection matrix =-)
  */

  // transform clipplane to eyespace
  lxMatrix44Transpose(mviewinvT,mviewinv);
  // transform normal
  lxVector3Transform(eyeplane,clipPlane,mviewinvT);
  // transform point on normal
  lxVector3Scale(q,clipPlane,clipPlane[3]);
  lxVector3Transform1(q,mview);
  eyeplane[3] = -lxVector3Dot(q,eyeplane);

  // Calculate the clip-space corner point opposite the clipping plane
  // as (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) and
  // transform it into camera space by multiplying it
  // by the inverse of the projection matrix


  q[0] = (sgn(eyeplane[0]) + projmatrix[8]) / projmatrix[0];
  q[1] = (sgn(eyeplane[1]) + projmatrix[9]) / projmatrix[5];
  q[2] = -1.0F;
  q[3] = (1.0F + projmatrix[10]) / projmatrix[14];

  // Calculate the scaled plane vector
  val = (2.0F / lxVector4Dot(eyeplane, q));
  lxVector4Scale(q,eyeplane,val);

  // Replace the third row of the projection matrix
  projmatrix[2] = q[0];
  projmatrix[6] = q[1];
  projmatrix[10] = q[2] + 1.0F;
  projmatrix[14] = q[3];

}
#undef sgn

enum {
  SIDE,
  UP,
  FORWARD,
  DIRECTIONS,
};
LUX_API void lxMatrix44Orient(lxMatrix44PTR mat, lxVector3CPTR dirnormalized, lxVector3CPTR upn, int diraxis)
{
  // side, up, forward
  lxVector3 vectors[DIRECTIONS];
  int x, y, z;

  lxVector3Copy(vectors[FORWARD],dirnormalized);

  lxVector3Cross(vectors[SIDE],vectors[FORWARD],upn);
  lxVector3Cross(vectors[UP],vectors[SIDE],vectors[FORWARD]);

  lxVector3Normalized(vectors[SIDE]);
  lxVector3Normalized(vectors[UP]);

  switch(diraxis) {
  case 0:
    x = FORWARD;
    y = UP;
    z = SIDE;
    break;
  case 1:
    x = SIDE;
    y = FORWARD;
    z = UP;
    break;
  case 2:
    lxVector3Negated(vectors[SIDE]);
    x = SIDE;
    y = UP;
    z = FORWARD;
    break;
  default:
    return;
  }

  mat[0] = vectors[x][0];
  mat[1] = vectors[x][1];
  mat[2] = vectors[x][2];

  mat[4] = vectors[y][0];
  mat[5] = vectors[y][1];
  mat[6] = vectors[y][2];

  mat[8]  = vectors[z][0];
  mat[9]  = vectors[z][1];
  mat[10] = vectors[z][2];

}

LUX_API void lxMatrix44LookAt(lxMatrix44PTR mat, lxVector3CPTR from, lxVector3CPTR to, lxVector3CPTR upn)
{
  lxMatrix44 m2;
  lxVector3 forward, side, up;

  lxVector3Sub(forward,to,from);
  lxVector3Normalized(forward);

  lxVector3Cross(side,upn,forward);
  lxVector3Normalized(side);
  lxVector3Cross(up,forward,side);
  lxVector3Normalized(up);

  mat[0] = side[0];
  mat[4] = side[1];
  mat[8] = side[2];
  mat[12] = 0;

  mat[1] = up[0];
  mat[5] = up[1];
  mat[9] = up[2];
  mat[13] = 0;

  mat[2] = -forward[0];
  mat[6] = -forward[1];
  mat[10] = -forward[2];
  mat[14] = 0;

  mat[3] = 0;
  mat[7] = 0;
  mat[11] = 0;
  mat[15] = 1;


  lxMatrix44Identity(m2);
  lxMatrix44SetInvTranslation(m2,from);
  lxMatrix44Multiply1(mat,m2);
}

LUX_API void lxMatrix44RotateAngle(lxMatrix44PTR mat, lxVector3PTR from, lxVector3PTR to){
  /*
  Vector3 vs;
  Vector3 vt;
  Vector3 v;
  float ca;

  Vector3NormalizedA(from);
  Vector3NormalizedA(to);

  Vector3Cross(vs,from, to);
  // axis multiplied by sin

  Vector3Copy(v,vs);
  Vector3NormalizedA(v);
  // axis of rotation
  ca = Vector3Dot(from, to) ; // cos angle

  Vector3Copy(vt,v);
  Vector3Scale(v,v,(1.0f -ca));

  Matrix44Identity(m);
  mat[0] = vt[0] * v[0] + ca;
  mat[5] = vt[1] * v[1] + ca;
  mat[10] = vt[2] * v[2] + ca;

  vt[0] *= v[1];
  vt[2] *= v[0];
  vt[1] *= v[2];

  mat[1] = vt[0] - vs[2];
  mat[2] = vt[2] + vs[1];
  mat[4] = vt[0] + vs[2];
  mat[6] = vt[1] - vs[0];
  mat[8] = vt[2] - vs[1];
  mat[9] = vt[1] + vs[0];
  */
  // http://lists.apple.com/archives/mac-opengl/2001/Jan/msg00059.html
  // Author: Tomas Moller, 1999
#define M(row,col) mat[row*4+col]

  float v[3];
  float e,h;

  lxVector3Normalized(from);
  lxVector3Normalized(to);

  lxVector3Cross(v,from,to);
  e=lxVector3Dot(from,to);

  if(e>1.0-LUX_FLOAT_EPSILON) // "from" almost or equal to "to"-vector?
  {
    // return identity
    M(0, 0)=1.0; M(0, 1)=0.0; M(0, 2)=0.0;
    M(1, 0)=0.0; M(1, 1)=1.0; M(1, 2)=0.0;
    M(2, 0)=0.0; M(2, 1)=0.0; M(2, 2)=1.0;
  }
  else if(e<-1.0+LUX_FLOAT_EPSILON) // "from" almost or equal to negated "to"?
  {
    float up[3],left[3];
    float fxx,fyy,fzz,fxy,fxz,fyz;
    float uxx,uyy,uzz,uxy,uxz,uyz;
    float lxx,lyy,lzz,lxy,lxz,lyz;
    // left=CROSS(from, (1,0,0))
    left[0]=0.0; left[1]=from[2]; left[2]=-from[1];
    if(lxVector3Dot(left,left)<LUX_FLOAT_EPSILON) // was left=CROSS(from,(1,0,0)) a good choice?
    {
      // here we now that left = CROSS(from, (1,0,0)) will be a good choice
      left[0]=-from[2]; left[1]=0.0; left[2]=from[0];
    }
    // normalize "left"
    lxVector3Normalized(left);
    lxVector3Cross(up,left,from);
    // now we have a coordinate system, i.e., a basis;
    // M=(from, up, left), and we want to rotate to:
    // N=(-from, up, -left). This is done with the mat:
    // N*M^T where M^T is the transpose of M
    fxx=-from[0]*from[0]; fyy=-from[1]*from[1]; fzz=-from[2]*from[2];
    fxy=-from[0]*from[1]; fxz=-from[0]*from[2]; fyz=-from[1]*from[2];

    uxx=up[0]*up[0]; uyy=up[1]*up[1]; uzz=up[2]*up[2];
    uxy=up[0]*up[1]; uxz=up[0]*up[2]; uyz=up[1]*up[2];

    lxx=-left[0]*left[0]; lyy=-left[1]*left[1]; lzz=-left[2]*left[2];
    lxy=-left[0]*left[1]; lxz=-left[0]*left[2]; lyz=-left[1]*left[2];
    // symmetric mat
    M(0, 0)=fxx+uxx+lxx; M(0, 1)=fxy+uxy+lxy; M(0, 2)=fxz+uxz+lxz;
    M(1, 0)=M(0, 1); M(1, 1)=fyy+uyy+lyy; M(1, 2)=fyz+uyz+lyz;
    M(2, 0)=M(0, 2); M(2, 1)=M(1, 2); M(2, 2)=fzz+uzz+lzz;
  }
  else // the most common case, unless "from"="to", or "from"=-"to"
  {
#if 0
    // unoptimized version - a good compiler will optimize this.
    h=(1.0-e)/Vector3Dot(v,v);
    M(0, 0)=e+h*v[0]*v[0]; M(0, 1)=h*v[0]*v[1]-v[2]; M(0,
      2)=h*v[0]*v[2]+v[1];
    M(1, 0)=h*v[0]*v[1]+v[2]; M(1, 1)=e+h*v[1]*v[1]; M(1,
      2)h*v[1]*v[2]-v[0];
    M(2, 0)=h*v[0]*v[2]-v[1]; M(2, 1)=h*v[1]*v[2]+v[0]; M(2,
      2)=e+h*v[2]*v[2];
#else
    // ...otherwise use this hand optimized version (9 mults less)
    float hvx,hvz,hvxy,hvxz,hvyz;
    //h=(1.0-e)/Vector3Dot(v,v);
    h = 1.0f/(1+e);
    hvx=h*v[0];
    hvz=h*v[2];
    hvxy=hvx*v[1];
    hvxz=hvx*v[2];
    hvyz=hvz*v[1];
    M(0, 0)=e+hvx*v[0]; M(0, 1)=hvxy-v[2]; M(0, 2)=hvxz+v[1];
    M(1, 0)=hvxy+v[2]; M(1, 1)=e+h*v[1]*v[1]; M(1, 2)=hvyz-v[0];
    M(2, 0)=hvxz-v[1]; M(2, 1)=hvyz+v[0]; M(2, 2)=e+hvz*v[2];
#endif
  }
#undef M

}

/*

rcos = cos(phi);
rsin = sin(phi);
matrix[0][0] =      rcos + u*u*(1-rcos);
matrix[1][0] =  w * rsin + v*u*(1-rcos);
matrix[2][0] = -v * rsin + w*u*(1-rcos);
matrix[0][1] = -w * rsin + u*v*(1-rcos);
matrix[1][1] =      rcos + v*v*(1-rcos);
matrix[2][1] =  u * rsin + w*v*(1-rcos);
matrix[0][2] =  v * rsin + u*w*(1-rcos);
matrix[1][2] = -u * rsin + v*w*(1-rcos);
matrix[2][2] =      rcos + w*w*(1-rcos);

*/

LUX_API void lxMatrix44RotateAroundPointFast( lxMatrix44PTR transforms,  lxVector3CPTR center, lxVector3CPTR angles)
{
  lxMatrix44 rot;

  lxMatrix44Identity(transforms);
  lxMatrix44Identity(rot);

  lxMatrix44FromEulerZYXdeg(rot,angles);
  lxMatrix44SetInvTranslation(transforms,center);
  lxMatrix44Multiply1(rot,transforms);

  lxMatrix44Identity(transforms);
  lxMatrix44SetTranslation(transforms,center);

  lxMatrix44Multiply1(transforms,rot);

}


LUX_API void lxMatrix44Reflection(lxMatrix44PTR matrix,lxVector4PTR plane)
{
  /*
  -Px2 + Pz* Pz + Py* Py    - 2 * Px * Py       - 2 * Px * Pz
  - 2 * Py * Px         -Py2 + Px*Px + Pz*Pz  - 2 * Py * Pz
  - 2 * Pz * Px         -2 * Pz * Py      -Pz2 + Py*Py + Px*Px
  */
  lxMatrix44 temp;
  float x2 = plane[0]*plane[0];
  float y2 = plane[1]*plane[1];
  float z2 = plane[2]*plane[2];
  float xy = -2*plane[0]*plane[1];
  float xz = -2*plane[0]*plane[2];
  float yz = -2*plane[1]*plane[2];

  lxVector3 trans;


  lxMatrix44SetRotRows(matrix,
    -x2+z2+y2,    xy,     xz,
    xy,       -y2+x2+z2,  yz,
    xz,       yz,     -z2+y2+x2);

  matrix[12] = 0;
  matrix[13] = 0;
  matrix[14] = 0;
  matrix[3] = 0;
  matrix[7] = 0;
  matrix[11] = 0;
  matrix[15] = 1;


  lxVector3Scale(trans,plane,plane[3]);
  lxMatrix44Identity(temp);
  lxMatrix44SetInvTranslation(temp,trans);
  lxMatrix44Multiply1(matrix,temp);
  lxMatrix44SetTranslation(temp,trans);
  lxMatrix44Multiply2(temp,matrix);

}

LUX_API void lxMatrix44Swizzle1(lxMatrix44PTR mat, uint axis[3], lxVector3PTR dirs)
{
  lxMatrix44 a_mat;
  lxMatrix44Copy(a_mat,mat);

  mat[0] = dirs[0]*a_mat[0+axis[0]];
  mat[4] = dirs[0]*a_mat[4+axis[0]];
  mat[8] = dirs[0]*a_mat[8+axis[0]];
  mat[12]= dirs[0]*a_mat[12+axis[0]];

  mat[1] = dirs[1]*a_mat[0+axis[1]];
  mat[5] = dirs[1]*a_mat[4+axis[1]];
  mat[9] = dirs[1]*a_mat[8+axis[1]];
  mat[13]= dirs[1]*a_mat[12+axis[1]];

  mat[2] = dirs[2]*a_mat[0+axis[2]];
  mat[6] = dirs[2]*a_mat[4+axis[2]];
  mat[10]= dirs[2]*a_mat[8+axis[2]];
  mat[14]= dirs[2]*a_mat[12+axis[2]];

  mat[15] = 1.0f;
  mat[3] = mat[7] = mat[11] = 0.0f;
}

LUX_API void lxMatrix44Invert(lxMatrix44PTR inverse, lxMatrix44CPTR src)
{
  int i, j, k, swap;
  float t;
  float temp[4][4];

  for (i=0; i<4; i++) {
    for (j=0; j<4; j++) {
      temp[i][j] = src[i*4+j];
    }
  }
  lxMatrix44Identity(inverse);

  for (i = 0; i < 4; i++) {
    /*
    ** Look for largest element in column
    */
    swap = i;
    for (j = i + 1; j < 4; j++) {
      if (fabs(temp[j][i]) > fabs(temp[i][i])) {
        swap = j;
      }
    }

    if (swap != i) {
      /*
      ** Swap rows.
      */
      for (k = 0; k < 4; k++) {
        t = temp[i][k];
        temp[i][k] = temp[swap][k];
        temp[swap][k] = t;

        t = inverse[i*4+k];
        inverse[i*4+k] = inverse[swap*4+k];
        inverse[swap*4+k] = t;
      }
    }

    if (temp[i][i] == 0) {
      /*
      ** No non-zero pivot.  The matrix is singular, which shouldn't
      ** happen.  This means the user gave us a bad matrix.
      */
      return;
    }

    t = temp[i][i];
    for (k = 0; k < 4; k++) {
      temp[i][k] /= t;
      inverse[i*4+k] /= t;
    }
    for (j = 0; j < 4; j++) {
      if (j != i) {
        t = temp[j][i];
        for (k = 0; k < 4; k++) {
          temp[j][k] -= temp[i][k]*t;
          inverse[j*4+k] -= inverse[i*4+k]*t;
        }
      }
    }
  }
}

static void Matrix44Invert_(lxMatrix44PTR inv_mat, lxMatrix44PTR mat)
{
  float det;


  // calculate determinants of required 2x2 sub matrices
  float det01 =mat[9] *mat[14] -mat[13] *mat[10];   // these are required for the determinant
  float det02 =mat[5] *mat[14] -mat[13] *mat[6];
  float det03 =mat[5] *mat[10] -mat[9] *mat[6];
  float det04 =mat[1] *mat[14] -mat[13] *mat[2];
  float det05 =mat[1] *mat[10] -mat[9] *mat[2];
  float det06 =mat[1] *mat[6] -mat[5] *mat[2];

  det =  -mat[3] * (mat[4] * det01 -mat[8] * det02 +mat[12] * det03) +
    mat[7] * (mat[0] * det01 -mat[8] * det04 +mat[12] * det05) -
    mat[11] * (mat[0] * det02 -mat[4] * det04 +mat[12] * det06) +
    mat[15] * (mat[0] * det03 -mat[4] * det05 +mat[8] * det06);

  if (fabs(det) < LUX_FLOAT_EPSILON)
  {
    lxMatrix44Identity(inv_mat);
  }
  else
  {
    // calculate (transposed cofactor mat / determinant of given mat)
    float det07 =mat[10] *mat[15] -mat[14] *mat[11];    // these are additionally required determinants of 2x2 sub matrices for the cofactor mat
    float det08 =mat[14] *mat[7] -mat[6] *mat[15];
    float det09 =mat[6] *mat[11] -mat[10] *mat[7];
    float det10 =mat[8] *mat[15] -mat[12] *mat[11];
    float det11 =mat[12] *mat[7] -mat[4] *mat[15];
    float det12 =mat[4] *mat[11] -mat[8] *mat[7];
    float det13 =mat[8] *mat[13] -mat[12] *mat[9];
    float det14 =mat[12] *mat[5] -mat[4] *mat[13];
    float det15 =mat[4] *mat[9] -mat[8] *mat[5];
    float det16 =mat[2] *mat[15] -mat[14] *mat[3];
    float det17 =mat[10] *mat[3] -mat[2] *mat[11];
    float det18 =mat[0] *mat[15] -mat[12] *mat[3];
    float det19 =mat[8] *mat[3] -mat[0] *mat[11];
    float det20 =mat[0] *mat[13] -mat[12] *mat[1];
    float det21 =mat[8] *mat[1] -mat[0] *mat[9];
    float det22 =mat[2] *mat[7] -mat[6] *mat[3];
    float det23 =mat[0] *mat[7] -mat[4] *mat[3];
    float det24 =mat[0] *mat[5] -mat[4] *mat[1];

    det = 1 / det;

    inv_mat[0] = det * (mat[5] * det07 +mat[9] * det08 +mat[13] * det09);
    inv_mat[4] = det * (mat[6] * det10 +mat[10] * det11 +mat[14] * det12);
    inv_mat[8] = det * (mat[7] * det13 +mat[11] * det14 +mat[15] * det15);
    inv_mat[12] = det * (mat[4] * -det01 +mat[8] * det02 +mat[12] * -det03);

    inv_mat[1] = det * (mat[9] * det16 +mat[13] * det17 -mat[1] * det07);
    inv_mat[5] = det * (mat[10] * det18 +mat[14] * det19 -mat[2] * det10);
    inv_mat[9] = det * (mat[11] * det20 +mat[15] * det21 -mat[3] * det13);
    inv_mat[13] = det * (mat[8] * -det04 +mat[12] * det05 +mat[0] * det01);

    inv_mat[2] = det * (mat[13] * det22 -mat[1] * det08 -mat[5] * det16);
    inv_mat[6] = det * (mat[14] * det23 -mat[2] * det11 -mat[6] * det18);
    inv_mat[10] = det * (mat[15] * det24 -mat[3] * det14 -mat[7] * det20);
    inv_mat[14] = det * (mat[12] * -det06 +mat[0] * -det02 +mat[4] * det04);

    inv_mat[3] = det * (mat[1] * -det09 -mat[5] * det17 -mat[9] * det22);
    inv_mat[7] = det * (mat[2] * -det12 -mat[6] * det19 -mat[10] * det23);
    inv_mat[11] = det * (mat[3] * -det15 -mat[7] * det21 -mat[11] * det24);
    inv_mat[15] = det * (mat[0] * det03 +mat[4] * -det05 +mat[8] * det06);
  }

}

LUX_API float  lxMatrix44CompareRot(lxMatrix44PTR a, lxMatrix44PTR b)
{
  float sum;
  int c,r;

  sum = 0;
  for (c = 0; c < 3; c++)
    for (r = 0; r < 3; r++){
      sum += (float)fabs((a[r+(c*4)] - b[r+(c*4)]));
    }

    return sum;
}
