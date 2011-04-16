// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt



#ifndef __LUXMATH_TYPES_H__
#define __LUXMATH_TYPES_H__

//////////////////////////////////////////////////////////////////////////

#include <luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif

#include <math.h>
#include <float.h>
#include <string.h>

  //////////////////////////////////////////////////////////////////////////

  typedef float lxVector2[2];
  typedef float lxVector3[3];
  typedef float lxVector4[4];
  typedef float lxPlane[4];
  typedef float lxMatrix44[16];
  typedef float lxMatrix34[12];
  typedef float lxQuat[4];

  #define lxVector2PTR    float* LUX_RESTRICT
  #define lxVector3PTR    float* LUX_RESTRICT
  #define lxVector4PTR    float* LUX_RESTRICT
  #define lxPlanePTR      float* LUX_RESTRICT
  #define lxMatrix44PTR   float* LUX_RESTRICT
  #define lxMatrix34PTR   float* LUX_RESTRICT
  #define lxQuatPTR       float* LUX_RESTRICT

#if 0
  typedef float*  LUX_RESTRICT lxVector2PTR;
  typedef float*  LUX_RESTRICT lxVector3PTR;
  typedef float*  LUX_RESTRICT lxVector4PTR;
  typedef float*  LUX_RESTRICT lxPlanePTR;
  typedef float*  LUX_RESTRICT lxMatrix44PTR;
  typedef float*  LUX_RESTRICT lxMatrix34PTR;
  typedef float*  LUX_RESTRICT lxQuatPTR;

  typedef const float*  LUX_RESTRICT lxVector2PTRconst;
  typedef const float*  LUX_RESTRICT lxVector3PTRconst;
  typedef const float*  LUX_RESTRICT lxVector4PTRconst;
  typedef const float*  LUX_RESTRICT lxPlanePTRconst;
  typedef const float*  LUX_RESTRICT lxMatrix44PTRconst;
  typedef const float*  LUX_RESTRICT lxMatrix34PTRconst;
  typedef const float*  LUX_RESTRICT lxQuatPTRconst;
#endif

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxFrustumPlaneType_e{
    LUX_FRUSTUM_TOP,
    LUX_FRUSTUM_BOTTOM,
    LUX_FRUSTUM_NEAR,
    LUX_FRUSTUM_FAR,
    LUX_FRUSTUM_LEFT,
    LUX_FRUSTUM_RIGHT,

    LUX_FRUSTUM_PLANES,
  }lxFrustumPlaneType_t;

  typedef enum lxFrustumCornerType_e{
    LUX_FRUSTUM_C_NTR,
    LUX_FRUSTUM_C_NTL,
    LUX_FRUSTUM_C_NBL,
    LUX_FRUSTUM_C_NBR,

    LUX_FRUSTUM_C_FTR,
    LUX_FRUSTUM_C_FTL,
    LUX_FRUSTUM_C_FBL,
    LUX_FRUSTUM_C_FBR,

    LUX_FRUSTUM_CORNERS,
  }lxFrustumCornerType_t;

  typedef enum lxCullType_e{
    LUX_CULL_INTERSECT = -1,
    LUX_CULL_OUTSIDE = 0,
    LUX_CULL_INSIDE = 1,
  }lxCullType_t;

  typedef struct lxFrustumPlane_s{
    lxPlane     pvec;
    union{
      int     n[3];
      struct{
        int   nx;
        int   ny;
        int   nz;
      };
    };
    union{
      int     p[3];
      struct{
        int   px;
        int   py;
        int   pz;
      };
    };
  }lxFrustumPlane_t;
  typedef lxFrustumPlane_t* lxFrustumPlanePTR;

  typedef struct lxFrustum_s
  {
    lxFrustumPlane_t  fplanes[LUX_FRUSTUM_PLANES];
  }lxFrustum_t;
  typedef lxFrustum_t* lxFrustumPTR;

  typedef struct lxBoundingBox_s
  {
    lxVector3 min;
    lxVector3 max;
    lxVector3 center;
    lxVector3 length;
  }lxBoundingBox_t;
  typedef lxBoundingBox_t* lxBoundingBoxPTR;

  typedef struct lxBoundingSphere_s
  {
    lxVector3 center;
    float radius;
    float radiusSqr;
  }lxBoundingSphere_t;
  typedef lxBoundingSphere_t* lxBoundingSpherePTR;

  typedef struct lxBoundingCone_s
  {
    lxVector3 top;
    lxVector3 axis;
    float sinDiv;
    float sinSqr;
    float cosSqr;
  }lxBoundingCone_t;
  typedef lxBoundingCone_t* lxBoundingConePTR;

  typedef struct lxBoundingCapsule_s
  {
    lxVector3 origin;
    lxVector3 toEnd;    // end-origin, non normalized
    float radius;
    float radiusSqr;
  }lxBoundingCapsule_t;
  typedef lxBoundingCapsule_t* lxBoundingCapsulePTR;

  //////////////////////////////////////////////////////////////////////////


#define LUX_CLAMP(a,min,max) (((a) < (min)) ? (min) : ( ((a) > (max)) ? (max) : (a) ))
#define LUX_LERP(t,a,b)      ((a) + (((b)-(a))*(t)))
#define LUX_BARYCENTRIC(u,v,w,p0,p1,p2)  ((u)*(p0) + (v)*(p1) + (w)*(p2))

  // makes a void data value from the float
  LUX_INLINE void* lxfloat2void(float f) {
    return (void*)*(size_t*)((void*)&f);
  }
  // makes a float from a data value
  LUX_INLINE float lxvoid2float(void *f) {
    return *(float*)&f;
  }

#define LUX_DIV_255     (0.003921568627451f)
#define LUX_FLOAT_EPSILON (1.192092896e-07F)

#ifndef LUX_MUL_PI
#define LUX_MUL_PI    (3.1415926535897932384626433832795f)
#endif
#ifndef LUX_MUL_HALF_PI
#define LUX_MUL_HALF_PI (1.5707963267948966192313216916395f)
#endif
#ifndef LUX_DIV_PI
#define LUX_DIV_PI    (0.31830988618379067153776752674503f)
#endif

#define LUX_MUL_TWOPI (6.283185307179586476925286766559f)
#define LUX_DEG2RAD( a ) (( (a) * LUX_MUL_PI   ) * 0.005555555555555555555555555555555f)
#define LUX_RAD2DEG( a ) (( (a) * 180.0f ) * LUX_DIV_PI )
#define LUX_SQRT_2 (1.4142135623730950488016887242097f)
#define LUX_SQRT_3 (1.7320508075688772935274463415059f)

#define LUX_SHORT_SIGNEDMAX   (32767)
#define LU_SHORT_UNSIGNEDMAX (0xffff)

#define LUX_MUL_SHORT   (32767.0f)
#define LUX_DIV_SHORT (3.0518509475997192297128208258309e-5f)

#define LUX_MUL_USHORT  (65535.0f)
#define LUX_DIV_USHORT  (1.5259021896696421759365224689097e-5f)


#ifdef __cplusplus
}
#endif

#endif