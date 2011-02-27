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


  typedef LUX_ALIGNSIMD_S lxMatrix44 lxMatrix44SIMD;
  typedef LUX_ALIGNSIMD_S lxVector4 lxVector4SIMD;
  typedef LUX_ALIGNSIMD_S lxVector4 lxVector3SIMD;


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
#endif

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxFrustumPlaneType_e{
    FRUSTUM_TOP,
    FRUSTUM_BOTTOM,
    FRUSTUM_NEAR,
    FRUSTUM_FAR,
    FRUSTUM_LEFT,
    FRUSTUM_RIGHT,

    FRUSTUM_PLANES,
  }lxFrustumPlaneType_t;

  typedef enum lxFrustumCornerType_e{
    FRUSTUM_C_NTR,
    FRUSTUM_C_NTL,
    FRUSTUM_C_NBL,
    FRUSTUM_C_NBR,

    FRUSTUM_C_FTR,
    FRUSTUM_C_FTL,
    FRUSTUM_C_FBL,
    FRUSTUM_C_FBR,

    FRUSTUM_CORNERS,
  }lxFrustumCornerType_t;

  typedef enum lxCullType_e{
    CULL_INTERSECT = -1,
    CULL_OUTSIDE = 0,
    CULL_INSIDE = 1,
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
    lxFrustumPlane_t  fplanes[FRUSTUM_PLANES];
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
  LUX_INLINE void* float2void(float f) {
    return (void*)*(size_t*)((void*)&f);
  }
  // makes a float from a data value
  LUX_INLINE float void2float(void *f) {
    return *(float*)&f;
  }

#define DIV_255     (0.003921568627451f)
#define FLOAT_EPSILON (1.192092896e-07F)

#ifndef MUL_PI
#define MUL_PI    (3.1415926535897932384626433832795f)
#endif
#ifndef MUL_HALF_PI
#define MUL_HALF_PI (1.5707963267948966192313216916395f)
#endif
#ifndef DIV_PI
#define DIV_PI    (0.31830988618379067153776752674503f)
#endif

#define MUL_TWOPI (6.283185307179586476925286766559f)
#define DEG2RAD( a ) (( (a) * MUL_PI   ) * 0.005555555555555555555555555555555f)
#define RAD2DEG( a ) (( (a) * 180.0f ) * DIV_PI )
#define SQRT_2 (1.4142135623730950488016887242097f)
#define SQRT_3 (1.7320508075688772935274463415059f)

#define SHORT_SIGNEDMAX   (32767)
#define SHORT_UNSIGNEDMAX (0xffff)

#define M_SHORT   (32767.0f)
#define DIV_SHORT (3.0518509475997192297128208258309e-5f)

#define M_USHORT  (65535.0f)
#define DIV_USHORT  (1.5259021896696421759365224689097e-5f)


#ifdef __cplusplus
}
#endif

#endif