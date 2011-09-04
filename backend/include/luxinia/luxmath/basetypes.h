// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_TYPES_H__
#define __LUXMATH_TYPES_H__

//////////////////////////////////////////////////////////////////////////

#include <luxinia/luxplatform/luxplatform.h>

#include <math.h>
#include <float.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif


  //////////////////////////////////////////////////////////////////////////

  typedef float lxVector2[2];
  typedef float lxVector3[3];
  typedef float lxVector4[4];
  typedef float lxPlane[4];
  typedef float lxMatrix44[16];
  typedef float lxMatrix34[12];
  typedef float lxQuat[4];

  typedef float* LUX_RESTRICT lxVector2PTR;
  typedef float* LUX_RESTRICT lxVector3PTR;
  typedef float* LUX_RESTRICT lxVector4PTR;
  typedef float* LUX_RESTRICT lxPlanePTR;
  typedef float* LUX_RESTRICT lxMatrix44PTR;
  typedef float* LUX_RESTRICT lxMatrix34PTR;
  typedef float* LUX_RESTRICT lxQuatPTR;

  typedef const float* LUX_RESTRICT lxVector2CPTR;
  typedef const float* LUX_RESTRICT lxVector3CPTR;
  typedef const float* LUX_RESTRICT lxVector4CPTR;
  typedef const float* LUX_RESTRICT lxPlaneCPTR;
  typedef const float* LUX_RESTRICT lxMatrix44CPTR;
  typedef const float* LUX_RESTRICT lxMatrix34CPTR;
  typedef const float* LUX_RESTRICT lxQuatCPTR;

#ifdef LUX_COMPILER_MSC
  typedef LUX_ALIGNSIMD_BEGIN lxMatrix44 lxMatrix44SIMD;
  typedef LUX_ALIGNSIMD_BEGIN lxVector4 lxVector4SIMD;
  typedef LUX_ALIGNSIMD_BEGIN lxVector4 lxVector3SIMD;
#elif defined(LUX_COMPILER_GCC)
  typedef lxMatrix44 lxMatrix44SIMD LUX_ALIGNSIMD_END;
  typedef lxVector4 lxVector4SIMD LUX_ALIGNSIMD_END;
  typedef lxVector4 lxVector3SIMD LUX_ALIGNSIMD_END;
#endif

  typedef struct lxFrustumPlane_s* LUX_RESTRICT lxFrustumPlanePTR;
  typedef const struct lxFrustumPlane_s* LUX_RESTRICT lxFrustumPlaneCPTR;

  typedef struct lxFrustum_s* LUX_RESTRICT lxFrustumPTR;
  typedef const struct lxFrustum_s* LUX_RESTRICT lxFrustumCPTR;

  typedef struct lxBoundingBox_s* LUX_RESTRICT lxBoundingBoxPTR;
  typedef const struct lxBoundingBox_s* LUX_RESTRICT lxBoundingBoxCPTR;

  typedef struct lxBoundingBoxCenter_s* LUX_RESTRICT lxBoundingBoxCenterPTR;
  typedef const struct lxBoundingBoxCenter_s* LUX_RESTRICT lxBoundingBoxCenterCPTR;

  typedef struct lxBoundingSphere_s* LUX_RESTRICT lxBoundingSpherePTR;
  typedef const struct lxBoundingSphere_s* LUX_RESTRICT lxBoundingSphereCPTR;

  typedef struct lxBoundingCone_s* LUX_RESTRICT lxBoundingConePTR;
  typedef const struct lxBoundingCone_s* LUX_RESTRICT lxBoundingConeCPTR;

  typedef struct lxBoundingCapsule_s* LUX_RESTRICT lxBoundingCapsulePTR;
  typedef const struct lxBoundingCapsule_s* LUX_RESTRICT lxBoundingCapsuleCPTR;


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
        int   _npad;
      };
    };
    union{
      int     p[3];
      struct{
        int   px;
        int   py;
        int   pz;
        int   _ppad;
      };
    };
  }lxFrustumPlane_t;

  typedef struct lxFrustum_s
  {
    lxFrustumPlane_t  fplanes[LUX_FRUSTUM_PLANES];
  }lxFrustum_t;

  typedef struct lxBoundingBox_s
  {
    lxVector4 min;
    lxVector4 max;
  }lxBoundingBox_t;

  typedef struct lxBoundingBoxCenter_s
  {
    lxVector4 center;
    lxVector4 size;
  }lxBoundingBoxCenter_t;

  typedef struct lxBoundingSphere_s
  {
    lxVector3 center;
    float     radius;
  }lxBoundingSphere_t;

  typedef struct lxBoundingCone_s
  {
    lxVector4 top;
    lxVector4 axis;
    float sinDiv;
    float sinSqr;
    float cosSqr;
    float _pad;
  }lxBoundingCone_t;

  typedef struct lxBoundingCapsule_s
  {
    lxVector4 origin;
    lxVector4 toEnd;    // end-origin, non normalized
    float radius;
    float radiusSqr;
    float _pad[2];
  }lxBoundingCapsule_t;

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
#define LUX_SHORT_UNSIGNEDMAX (0xffff)

#define LUX_MUL_SHORT   (32767.0f)
#define LUX_DIV_SHORT (3.0518509475997192297128208258309e-5f)

#define LUX_MUL_USHORT  (65535.0f)
#define LUX_DIV_USHORT  (1.5259021896696421759365224689097e-5f)


#ifdef __cplusplus
}
#endif

#endif