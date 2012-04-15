// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_TYPES_HPP__
#define __LUXMATH_TYPES_HPP__

//////////////////////////////////////////////////////////////////////////

#include <luxinia/luxmath/basetypes.h>

#ifdef __cplusplus
class lxCVector3;
class lxCVector4;


class lxCVector2 {
public:
  float x;
  float y;

  LUX_INLINE lxCVector2() {};
  lxCVector2(const lxCVector3& vec);
  lxCVector2(const lxCVector4& vec);
  template <typename S>
  LUX_INLINE lxCVector2(S all) : x(float(all)),y(float(all)) {};
  template <typename S0,typename S1>
  LUX_INLINE lxCVector2(S0 x, S1 y) : x(float(x)),y(float(y)) {};

  LUX_INLINE operator float* () {
    return &x;
  }
  LUX_INLINE operator const float* () const {
    return &x;
  }
  template <typename S0,typename S1>
  LUX_INLINE void Set(S0 nx, S1 ny){
    x = float(nx);
    y = float(ny);
  }
  LUX_INLINE float& operator[](int element) {
    LUX_ASSUME(element >= 0 && element < 4);
    return (&x)[element];
  }

  LUX_INLINE const float& operator[](int element) const {
    LUX_ASSUME(element >= 0 && element < 4);
    return (&x)[element];
  }
  LUX_INLINE lxCVector2& operator= (const lxCVector2& param)
  {
    x=param.x;
    y=param.y;
    return *this;
  }
};

class lxCVector3 {
public:
  float x;
  float y;
  float z;

  LUX_INLINE lxCVector3() {};
  template <typename S>
  LUX_INLINE lxCVector3(const lxCVector2 &vec, S z) : x(vec.x),y(vec.y),z(float(z)) {};
  LUX_INLINE lxCVector3(const lxCVector2 &vec) : x(vec.x),y(vec.y),z(0.0f) {};
  lxCVector3(const lxCVector4 &vec);
  template <typename S>
  LUX_INLINE lxCVector3(S all) : x(float(all)),y(float(all)),z(float(all)) {};
  template <typename S0,typename S1, typename S2>
  LUX_INLINE lxCVector3(S0 x, S1 y, S2 z) : x(float(x)),y(float(y)),z(float(z)) {};

  LUX_INLINE operator float* () {
    return &x;
  }
  LUX_INLINE operator const float* () const {
    return &x;
  }
  LUX_INLINE operator lxCVector2 () {
    return lxCVector2(x,y);
  }
  template <typename S0,typename S1, typename S2>
  LUX_INLINE void Set(S0 nx, S1 ny, S2 nz){
    x = float(nx);
    y = float(ny);
    z = float(nz);
  }

  LUX_INLINE float& operator[](int element) {
    LUX_ASSUME(element >= 0 && element < 3);
    return (&x)[element];
  }

  LUX_INLINE const float& operator[](int element) const {
    LUX_ASSUME(element >= 0 && element < 3);
    return (&x)[element];
  }
  LUX_INLINE lxCVector3& operator= (const lxCVector3& param)
  {
    x=param.x;
    y=param.y;
    z=param.z;
    return *this;
  }
};


class lxCVector4 {
public:
  float x;
  float y;
  float z;
  float w;

  LUX_INLINE lxCVector4() {};
  LUX_INLINE lxCVector4(const lxCVector2 &vec) : x(vec.x),y(vec.y),z(0.0f),w(1.0f) {};
  LUX_INLINE lxCVector4(const lxCVector3 &vec) : x(vec.x),y(vec.y),z(vec.z),w(1.0f) {};

  template <typename S0,typename S1>
  LUX_INLINE lxCVector4(const lxCVector2 &vec, S0 z, S1 w) : x(vec.x),y(vec.y),z(float(z)),w(float(w)) {};
  template <typename S0>
  LUX_INLINE lxCVector4(const lxCVector2 &vec, S0 z) : x(vec.x),y(vec.y),z(float(z)),w(1.0f) {};
  template <typename S>
  LUX_INLINE lxCVector4(const lxCVector3 &vec, S w) : x(vec.x),y(vec.y),z(vec.z),w(float(w)) {};
  template <typename S>
  LUX_INLINE lxCVector4(S all) : x(float(all)),y(float(all)),z(float(all)),w(float(all)) {};
  template <typename S0,typename S1, typename S2>
  LUX_INLINE lxCVector4(S0 x, S1 y, S2 z) : x(float(x)),y(float(y)),z(float(z)),w(1.0f) {};
  template <typename S0,typename S1, typename S2, typename S3>
  LUX_INLINE lxCVector4(S0 x, S1 y, S2 z, S3 w) : x(float(x)),y(float(y)),z(float(z)),w(float(w)) {};


  LUX_INLINE operator float* () {
    return &x;
  }
  LUX_INLINE operator const float* () const {
    return &x;
  }
  LUX_INLINE operator lxCVector3 () {
    return lxCVector3(x,y,z);
  }
  template <typename S0,typename S1, typename S2>
  LUX_INLINE void Set(S0 nx, S1 ny, S2 nz){
    x = float(x);
    y = float(y);
    z = float(z);
    w = 1.0f;
  }
  template <typename S0,typename S1, typename S2, typename S3>
  LUX_INLINE void Set(S0 nx, S1 ny, S2 nz, S3 nw){
    x = float(nx);
    y = float(ny);
    z = float(nz);
    w = float(nw);
  }
  LUX_INLINE float& operator[](int element) {
    LUX_ASSUME(element >= 0 && element < 4);
    return (&x)[element];
  }

  LUX_INLINE const float& operator[](int element) const {
    LUX_ASSUME(element >= 0 && element < 4);
    return (&x)[element];
  }
  LUX_INLINE lxCVector4& operator= (const lxCVector4& param)
  {
    x=param.x;
    y=param.y;
    z=param.z;
    w=param.w;
    return *this;
  }
};

typedef lxCVector4 lxCQuat;

class lxCMatrix44 {
public:
  lxCVector4  columns[4];

  LUX_INLINE operator float* () {
    return &columns[0].x;
  }
  LUX_INLINE operator const float* () const {
    return &columns[0].x;
  }
  LUX_INLINE float& operator[](int element) {
    LUX_ASSUME(element >= 0 && element < 16);
    return (&columns[0].x)[element];
  }
  LUX_INLINE const float& operator[](int element) const {
    LUX_ASSUME(element >= 0 && element < 16);
    return (&columns[0].x)[element];
  }

  lxCMatrix44() {}
  lxCMatrix44(int one) 
  {
    columns[0].Set(1.0f,0.0f,0.0f,0.0f);
    columns[1].Set(0.0f,1.0f,0.0f,0.0f);
    columns[2].Set(0.0f,0.0f,1.0f,0.0f);
    columns[3].Set(0.0f,0.0f,0.0f,1.0f);
  }

  LUX_INLINE lxCMatrix44& operator= (const lxCMatrix44& param)
  {
    memcpy(columns,param.columns,sizeof(columns));
    return *this;
  }

#ifdef LUX_SIMD_SSE
  lxCMatrix44(__m128 *ptr) {
    columns[0].x = ptr[0].m128_f32[0];
    columns[0].y = ptr[0].m128_f32[1];
    columns[0].z = ptr[0].m128_f32[2];
    columns[0].w = ptr[0].m128_f32[3];

    columns[1].x = ptr[1].m128_f32[0];
    columns[1].y = ptr[1].m128_f32[1];
    columns[1].z = ptr[1].m128_f32[2];
    columns[1].w = ptr[1].m128_f32[3];

    columns[2].x = ptr[2].m128_f32[0];
    columns[2].y = ptr[2].m128_f32[1];
    columns[2].z = ptr[2].m128_f32[2];
    columns[2].w = ptr[2].m128_f32[3];

    columns[3].x = ptr[3].m128_f32[0];
    columns[3].y = ptr[3].m128_f32[1];
    columns[3].z = ptr[3].m128_f32[2];
    columns[3].w = ptr[3].m128_f32[3];
  }
#endif
};


typedef LUX_ALIGNSIMD_BEGIN lxCMatrix44 lxCMatrix44SIMD LUX_ALIGNSIMD_END;
typedef LUX_ALIGNSIMD_BEGIN lxCVector3 lxCVector3SIMD LUX_ALIGNSIMD_END;
typedef LUX_ALIGNSIMD_BEGIN lxCVector4 lxCVector4SIMD LUX_ALIGNSIMD_END;

LUX_INLINE lxCVector3::lxCVector3(const lxCVector4 &vec) : x(vec.x),y(vec.y),z(vec.z) {};
LUX_INLINE lxCVector2::lxCVector2(const lxCVector3 &vec) : x(vec.x),y(vec.y) {};
LUX_INLINE lxCVector2::lxCVector2(const lxCVector4 &vec) : x(vec.x),y(vec.y) {};


#endif

#endif