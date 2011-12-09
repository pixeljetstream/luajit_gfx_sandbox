// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_MATRIX44_H__
#define __LUXMATH_MATRIX44_H__

#include <luxinia/luxmath/basetypes.h>
#include <luxinia/luxmath/fastmath.h>
#include <luxinia/luxmath/vector3.h>
#include <luxinia/luxmath/vector4.h>

#ifdef __cplusplus
extern "C"{
#endif

LUX_API lxMatrix44CPTR lxMatrix44GetIdentity();
LUX_API void lxMatrix44Identity(lxMatrix44PTR dst);
LUX_API void lxMatrix44Copy(lxMatrix44PTR dst, lxMatrix44CPTR src);
LUX_API void lxMatrix44CopyRot(lxMatrix44PTR dst, lxMatrix44CPTR src);
LUX_API void lxMatrix44CopyRotTransposed(lxMatrix44PTR dst, lxMatrix44CPTR src);

LUX_API void lxMatrix44SetTranslation( lxMatrix44PTR dst, lxVector3CPTR translation );
LUX_API void lxMatrix44SetInvTranslation( lxMatrix44PTR dst, lxVector3CPTR translation );
LUX_API void lxMatrix44SetScale( lxMatrix44PTR dst, lxVector3CPTR scale);
LUX_API void lxMatrix44PreScale( lxMatrix44PTR dst, lxMatrix44CPTR mat, lxVector3CPTR scale);
LUX_API void lxMatrix44SetRotRows(lxMatrix44PTR dst, 
            float a, float b, float c, 
            float d, float e, float f, 
            float g, float h, float i);

LUX_API float*  lxMatrix44GetTranslation(lxMatrix44CPTR mat, lxVector3PTR vec);

LUX_API void lxMatrix44Clear(lxMatrix44PTR mat);
LUX_API void lxMatrix44ClearRot(lxMatrix44PTR mat);
LUX_API void lxMatrix44ClearTranslation(lxMatrix44PTR mat);

LUX_API void lxMatrix44VectorTransform( lxMatrix44CPTR mat, lxVector3PTR v1 );
LUX_API void lxMatrix44VectorInvTransform( lxMatrix44CPTR mat, lxVector3PTR pVect );
LUX_API void lxMatrix44VectorRotate( lxMatrix44CPTR mat, lxVector3PTR v1 );
LUX_API void lxMatrix44VectorInvRotate( lxMatrix44CPTR mat, lxVector3PTR pVect );
LUX_API void lxMatrix44VectorTranslate( lxMatrix44CPTR mat, lxVector3PTR pVect );
LUX_API void lxMatrix44VectorInvTranslate( lxMatrix44CPTR mat, lxVector3PTR pVect );

LUX_API void lxMatrix44Multiply( lxMatrix44PTR dst, lxMatrix44CPTR mat1,  lxMatrix44CPTR mat2 );
LUX_API void lxMatrix44Multiply1( lxMatrix44PTR mat1,  lxMatrix44CPTR mat2 );
LUX_API void lxMatrix44Multiply2( lxMatrix44CPTR mat1,  lxMatrix44PTR mat2 );
LUX_API void lxMatrix44MultiplyFull( lxMatrix44PTR clip, lxMatrix44CPTR proj , lxMatrix44CPTR modl);

LUX_API void lxMatrix44MultiplyRot(lxMatrix44PTR dst, lxMatrix44CPTR mat1, lxMatrix44CPTR mat2 );
LUX_API void lxMatrix44MultiplyRot1( lxMatrix44PTR mat1, lxMatrix44CPTR mat2 );
LUX_API void lxMatrix44MultiplyRot2( lxMatrix44CPTR mat1,  lxMatrix44PTR mat2 );

LUX_API void lxMatrix44Orthonormalize(lxMatrix44PTR dst,lxMatrix44PTR src);

LUX_API void lxMatrix44Transpose(lxMatrix44PTR dst, lxMatrix44CPTR src);
LUX_API void lxMatrix44Transpose1(lxMatrix44PTR mat);
LUX_API void lxMatrix44TransposeRot(lxMatrix44PTR dst, lxMatrix44CPTR src);
LUX_API void lxMatrix44TransposeRot1(lxMatrix44PTR mat);
LUX_API void lxMatrix44TransposeRotIdentity(lxMatrix44PTR dst, lxMatrix44CPTR src);

LUX_API void lxMatrix44Invert(lxMatrix44PTR dst, lxMatrix44CPTR src);
LUX_API void lxMatrix44Invert1(lxMatrix44PTR mat);
LUX_API void lxMatrix44AffineInvert(lxMatrix44PTR dst, lxMatrix44CPTR src);

// rotation
LUX_API void lxMatrix44Orient(lxMatrix44PTR mat, lxVector3CPTR dirnormalized, lxVector3CPTR up, int diraxis);
LUX_API void lxMatrix44LookAt(lxMatrix44PTR mat, lxVector3CPTR from, lxVector3CPTR to, lxVector3CPTR up);

LUX_API void lxMatrix44FromEulerZYX( lxMatrix44PTR mat, lxVector3CPTR angles );
LUX_API void lxMatrix44FromEulerZYXFast( lxMatrix44PTR mat, lxVector3CPTR angles );
LUX_API void lxMatrix44FromEulerZYXdeg( lxMatrix44PTR mat, lxVector3CPTR angles );
LUX_API void lxMatrix44FromEulerXYZ(lxMatrix44PTR mat, lxVector3PTR angles);
LUX_API void lxMatrix44FromEulerXYZFast(lxMatrix44PTR mat, lxVector3PTR angles);
LUX_API void lxMatrix44FromAngleAxis(lxMatrix44PTR mat, lxVector3CPTR axis, 
                 float cos, float sin, float oneminuscos);
LUX_API void lxMatrix44FromAngleAxisFast(lxMatrix44PTR mat, float anglerad, lxVector3CPTR axis);
LUX_API void lxMatrix44RotateAngle(lxMatrix44PTR mat, lxVector3PTR from, lxVector3PTR to);
LUX_API void lxMatrix44RotateAroundVector(lxMatrix44PTR mat, lxVector3PTR axis, float cos, float sin, float oneminuscos);
LUX_API void lxMatrix44RotateAroundVectorFast(lxMatrix44PTR mat, lxVector3PTR axis, float angleRad);
LUX_API void lxMatrix44RotateAroundPointFast( lxMatrix44PTR mat,  lxVector3CPTR center, lxVector3CPTR angles);

LUX_API void lxMatrix44ToEulerXYZ( lxMatrix44CPTR mat, lxVector3PTR angles);
LUX_API void lxMatrix44ToEulerZYX( lxMatrix44CPTR mat, lxVector3PTR angles);

// projection
LUX_API void lxMatrix44Perspective(lxMatrix44PTR mat, const float fov, 
          const float front, const float back, const float aspect);
LUX_API void lxMatrix44PerspectiveInf(lxMatrix44PTR mat, const float fov, const float front, const float aspect);
LUX_API void lxMatrix44Ortho(lxMatrix44PTR mat, const float height, 
          const float front, const float back, const float aspect);
LUX_API void lxMatrix44OrthoDirect(lxMatrix44PTR mat, const float left, const float right, 
          const float bottom, const float top, 
          const float front, const float back);
LUX_API void lxMatrix44PlaneProjection(lxMatrix44PTR mat, lxVector3CPTR planenormal);
LUX_API void lxMatrix44ModifyProjectionClipplane(lxMatrix44PTR projmatrix,lxMatrix44CPTR mview, 
          lxMatrix44CPTR mviewinv, lxVector4CPTR clipPlane);
LUX_API void lxMatrix44Reflection(lxMatrix44PTR mat,lxVector4PTR plane);

// misc
LUX_API float lxMatrix44CompareRot(lxMatrix44PTR mat1, lxMatrix44PTR mat2);
LUX_API void  lxMatrix44Swizzle1(lxMatrix44PTR mat, uint axis[3], lxVector3PTR dirs);



//////////////////////////////////////////////////////////////////////////

#ifdef LUX_COMPILER_MSC
#pragma warning( push )
#pragma warning( disable : 4028 )
#endif

LUX_API extern const float lx_gMatrix44_ident[16];

LUX_INLINE void lxMatrix44Copy(lxMatrix44PTR dst, lxMatrix44CPTR src)
{
  memcpy(dst,src,sizeof(lxMatrix44));
  /*
  (dst)[0]  = (src)[0];  (dst)[1]  = (src)[1];  
  (dst)[2]  = (src)[2];  (dst)[3]  = (src)[3];  
  (dst)[4]  = (src)[4];  (dst)[5]  = (src)[5];  
  (dst)[6]  = (src)[6];  (dst)[7]  = (src)[7];  
  (dst)[8]  = (src)[8];  (dst)[9]  = (src)[9];  
  (dst)[10] = (src)[10]; (dst)[11] = (src)[11]; 
  (dst)[12] = (src)[12]; (dst)[13] = (src)[13]; 
  (dst)[14] = (src)[14]; (dst)[15] = (src)[15];
  */
}
LUX_INLINE void lxMatrix44CopyRot(lxMatrix44PTR dst, lxMatrix44CPTR src) 
{
  (dst)[0]  = (src)[0];  (dst)[1]  = (src)[1];  
  (dst)[2]  = (src)[2];   
  (dst)[4]  = (src)[4];  (dst)[5]  = (src)[5];  
  (dst)[6]  = (src)[6];    
  (dst)[8]  = (src)[8];  (dst)[9]  = (src)[9];  
  (dst)[10] = (src)[10];
}

LUX_INLINE void lxMatrix44CopyRotTransposed(lxMatrix44PTR dst, lxMatrix44CPTR src) 
{ 
  (dst)[0]  = (src)[0];  (dst)[4]  = (src)[1];  
  (dst)[8]  = (src)[2];   
  (dst)[1]  = (src)[4];  (dst)[5]  = (src)[5];  
  (dst)[9]  = (src)[6];    
  (dst)[2]  = (src)[8];  (dst)[6]  = (src)[9];  
  (dst)[10] = (src)[10];
}

LUX_INLINE void lxMatrix44SetRotRows(lxMatrix44PTR dst, 
                  float a, float b, float c, 
                  float d, float e, float f, 
                  float g, float h, float i)
{
  (dst)[0] =  (a);(dst)[1]  = (d); (dst)[2]  = (g);
  (dst)[4]  = (b);(dst)[5]  = (e); (dst)[6]  = (h);
  (dst)[8]  = (c);(dst)[9]  = (f); (dst)[10] = (i);
}

LUX_INLINE void lxMatrix44Clear(lxMatrix44PTR b) 
{
  memset(b,0,sizeof(lxMatrix44));
  /*
  (b)[0]  = (b)[1]  = (b)[2]  = (b)[3]  = 
  (b)[4]  = (b)[5]  = (b)[6]  = (b)[7]  = 
  (b)[8]  = (b)[9]  = (b)[10] = (b)[11] = 
  (b)[12] = (b)[13] = (b)[14] = (b)[15] = 0.0f;
  */
}

LUX_INLINE void lxMatrix44ClearRot(lxMatrix44PTR b)  
{
  (b)[1] = (b)[2]  =   
  (b)[4] = (b)[6]  =   
  (b)[8] = (b)[9]  = 0.0f; 
  (b)[0] = (b)[5] = (b)[10] = 1.0f;
}

LUX_INLINE void lxMatrix44ClearTranslation(lxMatrix44PTR matrix) 
{
  matrix[12] = matrix[13] = matrix[14] = 0.0f;
}

LUX_INLINE void lxMatrix44FromAngleAxis(lxMatrix44PTR mat, lxVector3CPTR axis, float c, float s, float oneminusc)
{
  float tmp1,tmp2;

  (mat)[0] = c + (axis)[0]*(axis)[0]*oneminusc;
  (mat)[5] = c + (axis)[1]*(axis)[1]*oneminusc;
  (mat)[10] = c + (axis)[2]*(axis)[2]*oneminusc;
  
  tmp1 = (axis)[0]*(axis)[1]*oneminusc;
  tmp2 = (axis)[2]*s;
  (mat)[1] = tmp1 + tmp2;
  (mat)[4] = tmp1 - tmp2;
  
  tmp1 = (axis)[0]*(axis)[2]*oneminusc;
  tmp2 = (axis)[1]*s;
  (mat)[2] = tmp1 - tmp2;
  (mat)[8] = tmp1 + tmp2;
  
  tmp1 = (axis)[1]*(axis)[2]*oneminusc;
  tmp2 = (axis)[0]*s;
  (mat)[6] = tmp1 + tmp2;
  (mat)[9] = tmp1 - tmp2;
}

LUX_INLINE lxMatrix44CPTR lxMatrix44GetIdentity()
{
  return (lxMatrix44CPTR)lx_gMatrix44_ident;
}

LUX_INLINE void lxMatrix44Identity(lxMatrix44PTR m_mat)
{
  lxMatrix44Copy(m_mat,lxMatrix44GetIdentity());
}

LUX_INLINE void lxMatrix44FromAngleAxisFast(lxMatrix44PTR matrix, float anglerad, lxVector3CPTR axis)
{
  float c,s,oneminusc;
  c = lxFastCos(anglerad);
  s = lxFastSin(anglerad);
  oneminusc = 1.0f - c;
  lxMatrix44FromAngleAxis(matrix,axis,c,s,oneminusc);
}

LUX_INLINE void lxMatrix44PlaneProjection(lxMatrix44PTR matrix, lxVector3CPTR planenormal)
{
  matrix[3] = matrix[7] = matrix[11] = 0.0f;
  matrix[15] = 1.0f;
  lxMatrix44SetRotRows(matrix,
    (planenormal[1]*planenormal[1])+(planenormal[2]*planenormal[2]),    -planenormal[0]*planenormal[1],   -planenormal[1]*planenormal[2],
    -planenormal[0]*planenormal[1],   (planenormal[0]*planenormal[0])+(planenormal[2]*planenormal[2]),    -planenormal[1]*planenormal[1],
    -planenormal[0]*planenormal[2],   -planenormal[1]*planenormal[2],   (planenormal[0]*planenormal[0])+(planenormal[1]*planenormal[1]));
}

LUX_INLINE void lxMatrix44VectorRotate( lxMatrix44CPTR mat, lxVector3PTR v1 )
{
  float vector[3];

  vector[0] = v1[0]*mat[0]+v1[1]*mat[4]+v1[2]*mat[8];
  vector[1] = v1[0]*mat[1]+v1[1]*mat[5]+v1[2]*mat[9];
  vector[2] = v1[0]*mat[2]+v1[1]*mat[6]+v1[2]*mat[10];

  lxVector3Copy(v1,vector);
}

LUX_INLINE void lxMatrix44VectorInvRotate( lxMatrix44CPTR m_mat, lxVector3PTR pVect )
{
  float vec[3];

  vec[0] = pVect[0]*m_mat[0]+pVect[1]*m_mat[1]+pVect[2]*m_mat[2];
  vec[1] = pVect[0]*m_mat[4]+pVect[1]*m_mat[5]+pVect[2]*m_mat[6];
  vec[2] = pVect[0]*m_mat[8]+pVect[1]*m_mat[9]+pVect[2]*m_mat[10];

  lxVector3Copy(pVect, vec);
}

LUX_INLINE void lxMatrix44VectorTranslate( lxMatrix44CPTR m_mat, lxVector3PTR pVect )
{
  pVect[0] = pVect[0]+m_mat[12];
  pVect[1] = pVect[1]+m_mat[13];
  pVect[2] = pVect[2]+m_mat[14];
}

LUX_INLINE void lxMatrix44VectorInvTranslate( lxMatrix44CPTR m_mat, lxVector3PTR pVect )
{
  pVect[0] = pVect[0]-m_mat[12];
  pVect[1] = pVect[1]-m_mat[13];
  pVect[2] = pVect[2]-m_mat[14];
}

LUX_INLINE void lxMatrix44VectorTransform( lxMatrix44CPTR mat, lxVector3PTR v1 )
{
  lxVector3Transform1(v1,mat);
}

LUX_INLINE void lxMatrix44VectorInvTransform( lxMatrix44CPTR mat, lxVector3PTR v1 )
{
  lxVector3InvTransform1(v1,mat);
}

LUX_INLINE float* lxMatrix44GetTranslation(lxMatrix44CPTR mat, lxVector3PTR vec){
  vec[0] = mat[12];
  vec[1] = mat[13];
  vec[2] = mat[14];
  return vec;
}

LUX_INLINE void lxMatrix44MultiplyRot(lxMatrix44PTR newmat, lxMatrix44CPTR m_mat, lxMatrix44CPTR mat )
{
  const float *a_mat = m_mat,*b_mat = mat;

  newmat[0] = a_mat[0]*b_mat[0] + a_mat[4]*b_mat[1] + a_mat[8]*b_mat[2];
  newmat[1] = a_mat[1]*b_mat[0] + a_mat[5]*b_mat[1] + a_mat[9]*b_mat[2];
  newmat[2] = a_mat[2]*b_mat[0] + a_mat[6]*b_mat[1] + a_mat[10]*b_mat[2];

  newmat[4] = a_mat[0]*b_mat[4] + a_mat[4]*b_mat[5] + a_mat[8]*b_mat[6];
  newmat[5] = a_mat[1]*b_mat[4] + a_mat[5]*b_mat[5] + a_mat[9]*b_mat[6];
  newmat[6] = a_mat[2]*b_mat[4] + a_mat[6]*b_mat[5] + a_mat[10]*b_mat[6];

  newmat[8] = a_mat[0]*b_mat[8] + a_mat[4]*b_mat[9] + a_mat[8]*b_mat[10];
  newmat[9] = a_mat[1]*b_mat[8] + a_mat[5]*b_mat[9] + a_mat[9]*b_mat[10];
  newmat[10] = a_mat[2]*b_mat[8] + a_mat[6]*b_mat[9] + a_mat[10]*b_mat[10];

}

LUX_INLINE void lxMatrix44Multiply( lxMatrix44PTR newmat, lxMatrix44CPTR m_mat,  lxMatrix44CPTR mat )
{
  const float *a_mat = m_mat,*b_mat = mat;

  newmat[0] = a_mat[0]*b_mat[0] + a_mat[4]*b_mat[1] + a_mat[8]*b_mat[2];
  newmat[1] = a_mat[1]*b_mat[0] + a_mat[5]*b_mat[1] + a_mat[9]*b_mat[2];
  newmat[2] = a_mat[2]*b_mat[0] + a_mat[6]*b_mat[1] + a_mat[10]*b_mat[2];
  newmat[3] = 0;

  newmat[4] = a_mat[0]*b_mat[4] + a_mat[4]*b_mat[5] + a_mat[8]*b_mat[6];
  newmat[5] = a_mat[1]*b_mat[4] + a_mat[5]*b_mat[5] + a_mat[9]*b_mat[6];
  newmat[6] = a_mat[2]*b_mat[4] + a_mat[6]*b_mat[5] + a_mat[10]*b_mat[6];
  newmat[7] = 0;

  newmat[8] = a_mat[0]*b_mat[8] + a_mat[4]*b_mat[9] + a_mat[8]*b_mat[10];
  newmat[9] = a_mat[1]*b_mat[8] + a_mat[5]*b_mat[9] + a_mat[9]*b_mat[10];
  newmat[10] = a_mat[2]*b_mat[8] + a_mat[6]*b_mat[9] + a_mat[10]*b_mat[10];
  newmat[11] = 0;

  newmat[12] = a_mat[0]*b_mat[12] + a_mat[4]*b_mat[13] + a_mat[8]*b_mat[14] + a_mat[12];
  newmat[13] = a_mat[1]*b_mat[12] + a_mat[5]*b_mat[13] + a_mat[9]*b_mat[14] + a_mat[13];
  newmat[14] = a_mat[2]*b_mat[12] + a_mat[6]*b_mat[13] + a_mat[10]*b_mat[14] + a_mat[14];
  newmat[15] = 1;
}

LUX_INLINE void lxMatrix44MultiplyFull( lxMatrix44PTR clip, lxMatrix44CPTR proj , lxMatrix44CPTR modl)
{

  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

}

LUX_INLINE void lxMatrix44MultiplyRot1( lxMatrix44PTR m_mat, lxMatrix44CPTR mat )
{
  lxMatrix44 newmat;
  const float *a_mat,*b_mat;
  a_mat = m_mat; b_mat = mat;

  newmat[0] = a_mat[0]*b_mat[0] + a_mat[4]*b_mat[1] + a_mat[8]*b_mat[2];
  newmat[1] = a_mat[1]*b_mat[0] + a_mat[5]*b_mat[1] + a_mat[9]*b_mat[2];
  newmat[2] = a_mat[2]*b_mat[0] + a_mat[6]*b_mat[1] + a_mat[10]*b_mat[2];

  newmat[4] = a_mat[0]*b_mat[4] + a_mat[4]*b_mat[5] + a_mat[8]*b_mat[6];
  newmat[5] = a_mat[1]*b_mat[4] + a_mat[5]*b_mat[5] + a_mat[9]*b_mat[6];
  newmat[6] = a_mat[2]*b_mat[4] + a_mat[6]*b_mat[5] + a_mat[10]*b_mat[6];

  newmat[8] = a_mat[0]*b_mat[8] + a_mat[4]*b_mat[9] + a_mat[8]*b_mat[10];
  newmat[9] = a_mat[1]*b_mat[8] + a_mat[5]*b_mat[9] + a_mat[9]*b_mat[10];
  newmat[10] = a_mat[2]*b_mat[8] + a_mat[6]*b_mat[9] + a_mat[10]*b_mat[10];

  lxMatrix44CopyRot(m_mat,newmat);
}


LUX_INLINE void lxMatrix44Multiply1( lxMatrix44PTR m_mat,  lxMatrix44CPTR mat )
{
  lxMatrix44 newmat;
  const float *a_mat,*b_mat;
  a_mat = m_mat; b_mat = mat;

  newmat[0] = a_mat[0]*b_mat[0] + a_mat[4]*b_mat[1] + a_mat[8]*b_mat[2];
  newmat[1] = a_mat[1]*b_mat[0] + a_mat[5]*b_mat[1] + a_mat[9]*b_mat[2];
  newmat[2] = a_mat[2]*b_mat[0] + a_mat[6]*b_mat[1] + a_mat[10]*b_mat[2];
  newmat[3] = 0;

  newmat[4] = a_mat[0]*b_mat[4] + a_mat[4]*b_mat[5] + a_mat[8]*b_mat[6];
  newmat[5] = a_mat[1]*b_mat[4] + a_mat[5]*b_mat[5] + a_mat[9]*b_mat[6];
  newmat[6] = a_mat[2]*b_mat[4] + a_mat[6]*b_mat[5] + a_mat[10]*b_mat[6];
  newmat[7] = 0;

  newmat[8] = a_mat[0]*b_mat[8] + a_mat[4]*b_mat[9] + a_mat[8]*b_mat[10];
  newmat[9] = a_mat[1]*b_mat[8] + a_mat[5]*b_mat[9] + a_mat[9]*b_mat[10];
  newmat[10] = a_mat[2]*b_mat[8] + a_mat[6]*b_mat[9] + a_mat[10]*b_mat[10];
  newmat[11] = 0;

  newmat[12] = a_mat[0]*b_mat[12] + a_mat[4]*b_mat[13] + a_mat[8]*b_mat[14] + a_mat[12];
  newmat[13] = a_mat[1]*b_mat[12] + a_mat[5]*b_mat[13] + a_mat[9]*b_mat[14] + a_mat[13];
  newmat[14] = a_mat[2]*b_mat[12] + a_mat[6]*b_mat[13] + a_mat[10]*b_mat[14] + a_mat[14];
  newmat[15] = 1;

  lxMatrix44Copy(m_mat,newmat);
}

LUX_INLINE void lxMatrix44MultiplyRot2( lxMatrix44CPTR m_mat, lxMatrix44PTR mat )
{
  lxMatrix44 newmat;
  const float *a_mat,*b_mat;
  a_mat = m_mat; b_mat = mat;

  newmat[0] = a_mat[0]*b_mat[0] + a_mat[4]*b_mat[1] + a_mat[8]*b_mat[2];
  newmat[1] = a_mat[1]*b_mat[0] + a_mat[5]*b_mat[1] + a_mat[9]*b_mat[2];
  newmat[2] = a_mat[2]*b_mat[0] + a_mat[6]*b_mat[1] + a_mat[10]*b_mat[2];

  newmat[4] = a_mat[0]*b_mat[4] + a_mat[4]*b_mat[5] + a_mat[8]*b_mat[6];
  newmat[5] = a_mat[1]*b_mat[4] + a_mat[5]*b_mat[5] + a_mat[9]*b_mat[6];
  newmat[6] = a_mat[2]*b_mat[4] + a_mat[6]*b_mat[5] + a_mat[10]*b_mat[6];

  newmat[8] = a_mat[0]*b_mat[8] + a_mat[4]*b_mat[9] + a_mat[8]*b_mat[10];
  newmat[9] = a_mat[1]*b_mat[8] + a_mat[5]*b_mat[9] + a_mat[9]*b_mat[10];
  newmat[10] = a_mat[2]*b_mat[8] + a_mat[6]*b_mat[9] + a_mat[10]*b_mat[10];

  lxMatrix44CopyRot(mat,newmat);
}

LUX_INLINE void lxMatrix44Multiply2( lxMatrix44CPTR m_mat,  lxMatrix44PTR mat )
{
  lxMatrix44 newmat;
  const float *a_mat,*b_mat;
  a_mat = m_mat; b_mat = mat;

  newmat[0] = a_mat[0]*b_mat[0] + a_mat[4]*b_mat[1] + a_mat[8]*b_mat[2];
  newmat[1] = a_mat[1]*b_mat[0] + a_mat[5]*b_mat[1] + a_mat[9]*b_mat[2];
  newmat[2] = a_mat[2]*b_mat[0] + a_mat[6]*b_mat[1] + a_mat[10]*b_mat[2];
  newmat[3] = 0;

  newmat[4] = a_mat[0]*b_mat[4] + a_mat[4]*b_mat[5] + a_mat[8]*b_mat[6];
  newmat[5] = a_mat[1]*b_mat[4] + a_mat[5]*b_mat[5] + a_mat[9]*b_mat[6];
  newmat[6] = a_mat[2]*b_mat[4] + a_mat[6]*b_mat[5] + a_mat[10]*b_mat[6];
  newmat[7] = 0;

  newmat[8] = a_mat[0]*b_mat[8] + a_mat[4]*b_mat[9] + a_mat[8]*b_mat[10];
  newmat[9] = a_mat[1]*b_mat[8] + a_mat[5]*b_mat[9] + a_mat[9]*b_mat[10];
  newmat[10] = a_mat[2]*b_mat[8] + a_mat[6]*b_mat[9] + a_mat[10]*b_mat[10];
  newmat[11] = 0;

  newmat[12] = a_mat[0]*b_mat[12] + a_mat[4]*b_mat[13] + a_mat[8]*b_mat[14] + a_mat[12];
  newmat[13] = a_mat[1]*b_mat[12] + a_mat[5]*b_mat[13] + a_mat[9]*b_mat[14] + a_mat[13];
  newmat[14] = a_mat[2]*b_mat[12] + a_mat[6]*b_mat[13] + a_mat[10]*b_mat[14] + a_mat[14];
  newmat[15] = 1;

  lxMatrix44Copy(mat,newmat);
}

LUX_INLINE void lxMatrix44SetTranslation( lxMatrix44PTR m_mat, lxVector3CPTR translation )
{
  m_mat[12] = translation[0];
  m_mat[13] = translation[1];
  m_mat[14] = translation[2];
}

LUX_INLINE void lxMatrix44SetInvTranslation( lxMatrix44PTR m_mat, lxVector3CPTR translation )
{
  m_mat[12] = -translation[0];
  m_mat[13] = -translation[1];
  m_mat[14] = -translation[2];
}

LUX_INLINE void lxMatrix44SetScale( lxMatrix44PTR mat, lxVector3CPTR scale)
{
  mat[0] =scale[0];
  mat[5] =scale[1];
  mat[10] =scale[2];
}

LUX_INLINE void lxMatrix44PreScale( lxMatrix44PTR dst, lxMatrix44CPTR mat, lxVector3CPTR scale)
{
  lxVector4Scale(&dst[0], &mat[0], scale[0]);
  lxVector4Scale(&dst[4], &mat[4], scale[1]);
  lxVector4Scale(&dst[8], &mat[8], scale[2]);
  lxVector4Copy(&dst[12], &mat[12]);
}

LUX_INLINE void lxMatrix44Transpose1(lxMatrix44PTR mat)
{
  lxMatrix44 oldmat;
  lxMatrix44Copy(oldmat,mat);
  mat[1]=oldmat[4];
  mat[2]=oldmat[8];
  mat[3]=oldmat[12];
  mat[4]=oldmat[1];
  mat[8]=oldmat[2];
  mat[12]=oldmat[3];

  mat[6]=oldmat[9];
  mat[7]=oldmat[13];
  mat[9]=oldmat[6];
  mat[13]=oldmat[7];

  mat[11]=oldmat[14];
  mat[14]=oldmat[11];
}
LUX_INLINE void lxMatrix44TransposeRot1(lxMatrix44PTR mat)
{
  float tmp;
  tmp = mat[1]; mat[1] = mat[4]; mat[4] = tmp;
  tmp = mat[2]; mat[2] = mat[8]; mat[8] = tmp;
  tmp = mat[6]; mat[6] = mat[9]; mat[9] = tmp;
}

LUX_INLINE void lxMatrix44TransposeRot(lxMatrix44PTR mat, lxMatrix44CPTR oldmat)
{
  mat[0]=oldmat[0];
  mat[4]=oldmat[1];
  mat[8]=oldmat[2];

  mat[1]=oldmat[4];
  mat[5]=oldmat[5];
  mat[9]=oldmat[6];

  mat[2]=oldmat[8];
  mat[6]=oldmat[9];
  mat[10]=oldmat[10];
}

LUX_INLINE void lxMatrix44TransposeRotIdentity(lxMatrix44PTR mat, lxMatrix44CPTR oldmat)
{
  mat[0]=oldmat[0];
  mat[4]=oldmat[1];
  mat[8]=oldmat[2];
  mat[12]=0.0f;

  mat[1]=oldmat[4];
  mat[5]=oldmat[5];
  mat[9]=oldmat[6];
  mat[13]=0.0f;

  mat[2]=oldmat[8];
  mat[6]=oldmat[9];
  mat[10]=oldmat[10];
  mat[14]=0.0f;

  mat[3]=0.0f;
  mat[7]=0.0f;
  mat[11]=0.0f;
  mat[15]=1.0f;
}

LUX_INLINE void lxMatrix44Transpose(lxMatrix44PTR mat, lxMatrix44CPTR oldmat){
  mat[0]=oldmat[0];
  mat[4]=oldmat[1];
  mat[8]=oldmat[2];
  mat[12]=oldmat[3];

  mat[1]=oldmat[4];
  mat[5]=oldmat[5];
  mat[9]=oldmat[6];
  mat[13]=oldmat[7];

  mat[2]=oldmat[8];
  mat[6]=oldmat[9];
  mat[10]=oldmat[10];
  mat[14]=oldmat[11];

  mat[3]=oldmat[12];
  mat[7]=oldmat[13];
  mat[11]=oldmat[14];
  mat[15]=oldmat[15];
}


LUX_INLINE void lxMatrix44Orthonormalize(lxMatrix44PTR dst,lxMatrix44PTR src)
{
  lxVector3 u1;
  lxVector3 u2;
  lxVector3 u3;
  float d;

  lxVector3Normalize(u1,&src[0]);
  d = lxVector3Dot(&src[4],u1);
  lxVector3ScaledAdd(u3,&src[4],-d,u1);
  lxVector3Normalize(u2,u3);

  lxVector3Copy(&dst[4],u2);
  dst[7] = 0.0f;

  d = lxVector3Dot(&src[8],u1);
  lxVector3ScaledAdd(u3,&src[4],-d,u1);

  lxVector3Copy(&dst[0],u1);
  dst[3] = 0.0f;

  d = lxVector3Dot(&src[8],u2);
  lxVector3ScaledAdd(u1,u3,-d,u1);

  lxVector3Normalize(&dst[8],u1);
  dst[11] = 0.0f;
  lxVector4Copy(&dst[12],&src[12]);
}


LUX_INLINE void lxMatrix44Invert1(lxMatrix44PTR m)
{
  lxMatrix44 invmat;
  lxMatrix44Invert(invmat,m);
  lxMatrix44Copy(m,invmat);
}

LUX_INLINE void lxMatrix44AffineInvert(lxMatrix44PTR outmat, lxMatrix44CPTR mat){
  //Matrix44 outmat;
  outmat[0] = mat[0];
  outmat[1] = mat[4];
  outmat[2] = mat[8];
  outmat[3] = 0.0f;
  outmat[4] = mat[1];
  outmat[5] = mat[5];
  outmat[6] = mat[9];
  outmat[7] = 0.0f;
  outmat[8] = mat[2];
  outmat[9] = mat[6];
  outmat[10] = mat[10];
  outmat[11] = 0.0f;
  outmat[12] = -(mat[0]*mat[12]+mat[1]*mat[13]+mat[2]*mat[14]);
  outmat[13] = -(mat[4]*mat[12]+mat[5]*mat[13]+mat[6]*mat[14]);
  outmat[14] = -(mat[8]*mat[12]+mat[9]*mat[13]+mat[10]*mat[14]);
  outmat[15] = 1.0f;
  //Matrix44Copy(mat,outmat);
}


LUX_INLINE void lxMatrix44RotateAroundVector(lxMatrix44PTR mat, lxVector3PTR axis, float c, float s, float t)
{
  float u = axis[0];
  float v = axis[1];
  float w = axis[2];

  mat[0] =      c + u*u*t;
  mat[1] =  w * s + v*u*t;
  mat[2] = -v * s + w*u*t;
  mat[4] = -w * s + u*v*t;
  mat[5] =      c + v*v*t;
  mat[6] =  u * s + w*v*t;
  mat[8] =  v * s + u*w*t;
  mat[9] = -u * s + v*w*t;
  mat[10] =      c + w*w*t;

  /*
  mat[0] = c + axis[0]*axis[0]*t;
  mat[5] = c + axis[1]*axis[1]*t;
  mat[10] = c + axis[2]*axis[2]*t;

  tmp1 = axis[0]*axis[1]*t;
  tmp2 = axis[2]*s;
  mat[1] = tmp1 + tmp2;
  mat[4] = tmp1 - tmp2;

  tmp1 = axis[0]*axis[2]*t;
  tmp2 = axis[1]*s;
  mat[2] = tmp1 - tmp2;
  mat[8] = tmp1 + tmp2;

  tmp1 = axis[1]*axis[2]*t;
  tmp2 = axis[0]*s;
  mat[6] = tmp1 + tmp2;
  mat[9] = tmp1 - tmp2;
  */
}

LUX_INLINE void lxMatrix44RotateAroundVectorFast(lxMatrix44PTR mat, lxVector3PTR axis, float angleRad){
  float c = lxFastCos(angleRad);
  float s = lxFastSin(angleRad);
  float t = 1.0f - c;
  lxMatrix44RotateAroundVector(mat, axis, c, s, t);
}


// ugly fixes to get things into OpenGL coords
LUX_INLINE void Matrix443DSMAXtoGL(lxMatrix44PTR mat){
  lxMatrix44 a_mat;
  lxMatrix44Copy(a_mat,mat);

  // swap y,z
  // invert x

  mat[0] = -a_mat[0];
  mat[4] = -a_mat[4];
  mat[8] = -a_mat[8];
  mat[12]= -a_mat[12];
  mat[1] = a_mat[2];
  mat[5] = a_mat[6];
  mat[9] = a_mat[10];
  mat[13] = a_mat[14];
  mat[2]  = a_mat[1];
  mat[6]  = a_mat[5];
  mat[10] = a_mat[9];
  mat[14] = a_mat[13];
  mat[15] = 1;
  mat[3] = mat[7] = mat[11] = 0;
}

#ifdef LUX_COMPILER_MSC
#pragma warning( pop )
#endif

#ifdef __cplusplus
}
#endif

#endif

