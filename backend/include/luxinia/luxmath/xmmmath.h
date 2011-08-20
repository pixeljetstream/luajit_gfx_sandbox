// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

// Some SIMD code taken from "Nebula2SDK", (C) 2002 RadonLabs GmbH

#ifndef __LUXMATH_XMM_H__
#define __LUXMATH_XMM_H__

#include <luxinia/luxmath/basetypes.h>
#include <luxinia/luxmath/matrix44.h>
#include <luxinia/luxmath/fastmath.h>
#include <xmmintrin.h>

#ifdef __cplusplus
extern "C"{
#endif

// Type_func instead of TypeFunc because Type is not native, (struct/aligned mem...)
// some functions were speed-test against normal fpu on a intel quadcore, vc2005

typedef __m128 lxVector4SSE;
typedef __m128 lxMatrix44SSE[4];
typedef __m128 lxMatrix34SSE[3];
typedef __m128* LUX_RESTRICT lxVector3SSEp;
typedef __m128* LUX_RESTRICT lxMatrix44SSEp;
typedef __m128* LUX_RESTRICT lxMatrix34SSEp;


//////////////////////////////////////////////////////////////////////////
// Vector3SSE
// reads all 16-byte ( so make sure memory acces is allowed)
// but protects .w on write, 

LUX_API void lxVector3SSEp_transform(lxVector3SSEp out,  lxVector3SSEp src, lxMatrix44SSEp matrix);   // 1.7x
LUX_API void lxVector3SSEp_transformRot(lxVector3SSEp out,  lxVector3SSEp src, lxMatrix44SSEp matrix);  // 1.7x
LUX_API void lxVector3SSEp_lerp(lxVector3SSEp out,  float s, lxVector3SSEp a, lxVector3SSEp b);
LUX_API void lxVector3SSEp_catmullRom(lxVector3SSEp out,  float s, lxVector3SSEp prev, lxVector3SSEp a,lxVector3SSEp b, lxVector3SSEp next);

//////////////////////////////////////////////////////////////////////////
// Vector4SSE
LUX_API float    lxVector4SSE_get(lxVector4SSE vec, int n);
LUX_API lxVector4SSE lxVector4SSE_zero();
LUX_API lxVector4SSE lxVector4SSE_set(float x, float y, float z, float w);
LUX_API lxVector4SSE lxVector4SSE_setAll(float f);
LUX_API lxVector4SSE lxVector4SSE_reciprocal(lxVector4SSE v0);
LUX_API lxVector4SSE lxVector4SSE_scale(lxVector4SSE src,  float s);
LUX_API lxVector4SSE lxVector4SSE_add( lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE lxVector4SSE_sub( lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE lxVector4SSE_mul( lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE lxVector4SSE_div( lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE lxVector4SSE_min( lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE lxVector4SSE_max( lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE lxVector4SSE_dot( lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE lxVector4SSE_negated(lxVector4SSE src);
LUX_API lxVector4SSE lxVector4SSE_transform(  lxVector4SSE src, lxMatrix44SSEp matrix);
LUX_API lxVector4SSE lxVector4SSE_transformRot(  lxVector4SSE src, lxMatrix44SSEp matrix);
LUX_API lxVector4SSE lxVector4SSE_lerp( float s, lxVector4SSE a, lxVector4SSE b);
LUX_API lxVector4SSE lxVector4SSE_vlerp(lxVector4SSE s, lxVector4SSE a, lxVector4SSE b);
LUX_API lxVector4SSE lxVector4SSE_catmullRom(float s, lxVector4SSE* LUX_RESTRICT prev, lxVector4SSE* LUX_RESTRICT a,lxVector4SSE* LUX_RESTRICT b, lxVector4SSE* LUX_RESTRICT next);
LUX_API lxVector4SSE lxVector4SSE_vcatmullRom(lxVector4SSE t, lxVector4SSE* LUX_RESTRICT v0, lxVector4SSE* LUX_RESTRICT v1,lxVector4SSE* LUX_RESTRICT v2, lxVector4SSE* LUX_RESTRICT v3);

// ignores .w
// but may write to all
LUX_API lxVector4SSE  lxVector4SSE_transformV3(lxVector4SSE src, lxMatrix44SSEp matrix);  // 1.8x
LUX_API lxVector4SSE  lxVector4SSE_crossV3(lxVector4SSE v0,  lxVector4SSE v1);
LUX_API lxVector4SSE  lxVector4SSE_normalizeV3(lxVector4SSE vec3);
LUX_API float         lxVector4SSE_lengthV3( lxVector4SSE vec3);
LUX_API float         lxVector4SSE_sqLengthV3( lxVector4SSE vec3);
LUX_API lxVector4SSE lxVector4SSE_vlengthV3( lxVector4SSE vec3);
LUX_API lxVector4SSE lxVector4SSE_vsqLengthV3( lxVector4SSE vec3);
LUX_API lxVector4SSE lxVector4SSE_perpendicularV3(lxVector4SSE vec3);

//////////////////////////////////////////////////////////////////////////
// Matrix44SSE
LUX_API void lxMatrix44SSE_clear(lxMatrix44SSEp matrix);
LUX_API void lxMatrix44SSE_identity(lxMatrix44SSEp matrix);             // 2.0x
LUX_API void lxMatrix44SSE_copy(lxMatrix44SSEp matrix,  lxMatrix44SSEp from);     // 1.9x
LUX_API void lxMatrix44SSE_transpose1(lxMatrix44SSEp matrix);           // 2.5x
LUX_API void lxMatrix44SSE_transpose(lxMatrix44SSEp matrix,lxMatrix44SSEp from);
LUX_API lxVector4SSE lxMatrix44SSE_rotateVector(lxMatrix44SSEp matrix, lxVector4SSE src);
LUX_API void lxMatrix44SSE_affineInvert(lxMatrix44SSEp outmat,  lxMatrix44SSEp mat);  // 1.6x

LUX_API void lxMatrix44SSE_multiply(lxMatrix44SSEp output,  lxMatrix44SSEp mat0,  lxMatrix44SSEp mat1); // 2.5x vs simple, 3.9x vs full
LUX_API void lxMatrix44SSE_multiply1(lxMatrix44SSEp outmat0,  lxMatrix44SSEp mat1);
LUX_API void lxMatrix44SSE_multiply2(lxMatrix44SSEp mat0, lxMatrix44SSEp outmat1);
LUX_API void lxMatrix44SSE_invert(lxMatrix44SSEp outmat, lxMatrix44SSEp mat0);

//////////////////////////////////////////////////////////////////////////
// Matrix34SSE
LUX_API void lxMatrix34SSE_identity(lxMatrix34SSEp matrix);
LUX_API void lxMatrix34SSE_copy(lxMatrix34SSEp dst, lxMatrix34SSEp src);
LUX_API void lxMatrix34SSE_tmultiply44(lxMatrix34SSEp output, lxMatrix44SSEp mat0,  lxMatrix44SSEp mat1);

//////////////////////////////////////////////////////////////////////////
// Some SIMD code taken from "Nebula2SDK"
//  (C) 2002 RadonLabs GmbH

//////////////////////////////////////////////////////////////////////////
// Vector4SSE

LUX_INLINE lxVector4SSE lxVector4SSE_zero()
{
  return _mm_setzero_ps();
}

LUX_INLINE lxVector4SSE lxVector4SSE_set(float x, float y, float z, float w)
{
  return _mm_set_ps(w,z,y,x);
}

LUX_INLINE lxVector4SSE lxVector4SSE_setAll(float f)
{
  return _mm_set_ps1(f);
}

LUX_INLINE float lxVector4SSE_get(lxVector4SSE vec, int n)
{
  LUX_ASSUME(n >= 0 && n <= 3);
  return vec.m128_f32[n];
}

LUX_INLINE lxVector4SSE lxVector4SSE_reciprocal(lxVector4SSE v0)
{
  const __m128 one = {1.0,1.0,1.0,1.0};
  return _mm_div_ps(one,v0);
}

LUX_INLINE lxVector4SSE lxVector4SSE_scale(lxVector4SSE v0, float s)
{
  __m128 packed = _mm_set1_ps(s);
  return _mm_mul_ps(v0, packed);
}

LUX_INLINE lxVector4SSE lxVector4SSE_add( lxVector4SSE v0,  lxVector4SSE v1)
{
  return  _mm_add_ps(v0, v1);
}

LUX_INLINE lxVector4SSE lxVector4SSE_sub( lxVector4SSE v0,  lxVector4SSE v1)
{
  return _mm_sub_ps(v0, v1);
}

LUX_INLINE lxVector4SSE lxVector4SSE_mul( lxVector4SSE v0,  lxVector4SSE v1)
{
  return  _mm_mul_ps(v0, v1);
}

LUX_INLINE lxVector4SSE lxVector4SSE_div( lxVector4SSE v0,  lxVector4SSE v1)
{
  return  _mm_div_ps(v0, v1);
}

LUX_INLINE lxVector4SSE lxVector4SSE_min( lxVector4SSE v0, lxVector4SSE v1)
{
  return _mm_min_ps(v0, v1);
}

LUX_INLINE lxVector4SSE lxVector4SSE_max( lxVector4SSE v0, lxVector4SSE v1)
{
  return _mm_max_ps(v0, v1);
}


//////////////////////////////////////////////////////////////////////////////
// Fast SSE2 4-dimensional dot product by Gustavo M. Oliveira.
// Feel free to use for commercial or non-commercial applications,
// as long as this header is mantained.
//
// For more information on how to write and use high-performance
// SIMD Libraties, visit:
// http://www.guitarv.com/ComputerScience.aspx?page=articles
//
// The author assumes NO RESPONSIBILITY and/or LIABILITY  for any problems
// caused by the use of this software. Use it at your own risk.
//////////////////////////////////////////////////////////////////////////////
LUX_INLINE lxVector4SSE lxVector4SSE_dot(lxVector4SSE va, lxVector4SSE vb)
{
  __m128 t0 = _mm_mul_ps(va, vb);
  __m128 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1,0,3,2));
  __m128 t2 = _mm_add_ps(t0, t1);
  __m128 t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2,3,0,1));
  __m128 dot = _mm_add_ps(t3, t2);
  return (dot);
}

LUX_INLINE lxVector4SSE lxVector4SSE_negated(lxVector4SSE out)
{
  __m128 zero = _mm_setzero_ps();
  return _mm_sub_ps(zero, out);
}

LUX_INLINE lxVector4SSE lxVector4SSE_transformRot(lxVector4SSE src, lxMatrix44SSEp matrix)
{
  return _mm_add_ps(
    _mm_add_ps(
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(0,0,0,0)), matrix[0]), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(1,1,1,1)), matrix[1])), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(2,2,2,2)), matrix[2]));
}

LUX_INLINE lxVector4SSE lxVector4SSE_transform(lxVector4SSE src, lxMatrix44SSEp matrix)
{
  return _mm_add_ps(
    _mm_add_ps(
    _mm_add_ps(
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(0,0,0,0)), matrix[0]), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(1,1,1,1)), matrix[1])), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(2,2,2,2)), matrix[2])),
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(3,3,3,3)), matrix[3]));
}

LUX_INLINE lxVector4SSE lxVector4SSE_vlerp(lxVector4SSE s, lxVector4SSE a, lxVector4SSE b)
{
  // a + (b-a)*s

  return _mm_add_ps(a,_mm_mul_ps(_mm_sub_ps(b,a),s));
}

LUX_INLINE lxVector4SSE lxVector4SSE_lerp(float s, lxVector4SSE a, lxVector4SSE b)
{
  // a + (b-a)*s

  return lxVector4SSE_vlerp(_mm_set_ps1(s),a,b);
}

LUX_INLINE lxVector4SSE lxVector4SSE_vcatmullRom(lxVector4SSE t, lxVector4SSE* LUX_RESTRICT v0, lxVector4SSE* LUX_RESTRICT v1,lxVector4SSE* LUX_RESTRICT v2, lxVector4SSE* LUX_RESTRICT v3)
{
  __m128 t2 = _mm_mul_ps(t,t);
  __m128 t3 = _mm_mul_ps(t2,t);

  __m128 two = _mm_set_ps1(2.0f);

  __m128 coeff3 = _mm_sub_ps(t3,t2);
  __m128 coeff0 = _mm_sub_ps(_mm_sub_ps(_mm_mul_ps(two,t2),t3),t);
  __m128 t32 = _mm_mul_ps(_mm_set_ps1(3.0f),t3);
  __m128 coeff1 = _mm_add_ps(_mm_sub_ps(t32,_mm_mul_ps(_mm_set_ps1(5.0f),t2)),two);
  __m128 coeff2 = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(_mm_set_ps1(4.0f),t2),t32),t);
  

  return _mm_mul_ps(
    _mm_add_ps(
      _mm_mul_ps(*v0,(coeff0)),
      _mm_add_ps(
        _mm_mul_ps(*v1,(coeff1)),
        _mm_add_ps(
          _mm_mul_ps(*v2,(coeff2)),
          _mm_mul_ps(*v3,(coeff3))))),
    _mm_set_ps1(0.5f));
}
LUX_INLINE lxVector4SSE lxVector4SSE_catmullRom(float t, lxVector4SSE* LUX_RESTRICT v0, lxVector4SSE* LUX_RESTRICT v1,lxVector4SSE* LUX_RESTRICT v2, lxVector4SSE* LUX_RESTRICT v3)
{
  //  0.5 *(      (2 * P1) + (P2 - P0) * t + (2*P0 - 5*P1 + 4*P2 - P3) * t2 + (3*P1-3*P2 + P3 -P0) * t3)
#if 0
  float t2=t*t;
  float t3=t2*t;

  float coeff0 = -t3 + 2.0f * t2 - t;
  float coeff1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
  float coeff2 = -3.0f * t3 + 4.0f * t2 + t;
  float coeff3 = t3 - t2;

  return _mm_mul_ps(
    _mm_add_ps(
      _mm_mul_ps(v0,_mm_set_ps1(coeff0)),
      _mm_add_ps(
        _mm_mul_ps(v1,_mm_set_ps1(coeff1)),
        _mm_add_ps(
          _mm_mul_ps(v2,_mm_set_ps1(coeff2)),
          _mm_mul_ps(v3,_mm_set_ps1(coeff3))))),
    _mm_set_ps1(0.5f));
#else
  return lxVector4SSE_vcatmullRom(_mm_set_ps1(t),v0,v1,v2,v3);
#endif

}


// ignore .w
LUX_INLINE lxVector4SSE  lxVector4SSE_transformV3( lxVector4SSE src, lxMatrix44SSEp matrix)
{
  return _mm_add_ps(
    _mm_add_ps(
    _mm_add_ps(
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(0,0,0,0)), matrix[0]), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(1,1,1,1)), matrix[1])), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(2,2,2,2)), matrix[2])),
    matrix[3]);
}
LUX_INLINE lxVector4SSE lxVector4SSE_crossV3(lxVector4SSE v0,  lxVector4SSE v1)
{
  __m128 a = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3, 0, 2, 1));
  __m128 b = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2));
  __m128 c = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3, 1, 0, 2));
  __m128 d = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));

  __m128 e = _mm_mul_ps(a, b);
  __m128 f = _mm_mul_ps(c, d);

  return _mm_sub_ps(e, f);
}

LUX_INLINE lxVector4SSE lxVector4SSE_normalizeV3(lxVector4SSE vec3)
{
  __m128 a = _mm_mul_ps(vec3, vec3);

  // horizontal add
  __m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));

  // get reciprocal of square root of squared length
  __m128 f = _mm_rsqrt_ss(b);
  __m128 oneDivLen = _mm_shuffle_ps(f, f, _MM_SHUFFLE(0,0,0,0));

  return _mm_mul_ps(vec3, oneDivLen);
}

LUX_INLINE lxVector4SSE lxVector4SSE_vlengthV3(lxVector4SSE vec3)
{
  __m128 a = _mm_mul_ps(vec3, vec3);

  // horizontal add
  __m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));
  return _mm_sqrt_ss(b);
}

LUX_INLINE float lxVector4SSE_lengthV3(lxVector4SSE vec3){
  return lxVector4SSE_get(lxVector4SSE_vlengthV3(vec3),0);
}

LUX_INLINE lxVector4SSE lxVector4SSE_vsqLengthV3( lxVector4SSE  vec3)
{
  __m128 a = _mm_mul_ps(vec3, vec3);
  __m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));
  return b;
}

LUX_INLINE float lxVector4SSE_sqLengthV3(lxVector4SSE vec3){
  return lxVector4SSE_get(lxVector4SSE_vsqLengthV3(vec3),0);
}

LUX_INLINE lxVector4SSE lxVector4SSE_perpendicularV3(lxVector4SSE vec3)
{ // creates a random normalized vector which is perpendicular to in
  __m128 nin = lxVector4SSE_normalizeV3(vec3);
  __m128 x = {1.0f,0.0f,0.0f,0.0f};
  __m128 y = {0.0f,1.0f,0.0f,0.0f};
  
  __m128 a = _mm_mul_ps(nin, x);
  // horizontal add
  __m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));

  float rdot = b.m128_f32[0];
  // ~25° tolerance
  if (LUX_FP_ABS_BITS(rdot) < 0x3F666666){
    vec3 = lxVector4SSE_crossV3(x,nin);
  }
  else{
    vec3 = lxVector4SSE_crossV3(y,nin);
  }
  return lxVector4SSE_normalizeV3(vec3);
}

//////////////////////////////////////////////////////////////////////////
// Vector3SSE

//#define SSE_STORE3_SIMD
LUX_INLINE void lxVector3SSEp_transform(lxVector3SSEp out,  lxVector3SSEp src, lxMatrix44SSEp matrix)
{
#ifndef SSE_STORE3_SIMD
  float old = out->m128_f32[3];
  *out = lxVector4SSE_transformV3(*src,matrix);
  out->m128_f32[3] = old;
#else
  __m128 temp;
  lxVector4SSE_transformV3(temp,src,matrix);

  temp = _mm_shuffle_ps(temp,temp,_MM_SHUFFLE(2,3,0,1));
  _mm_store_ss(((float*)out)+2,temp);
  _mm_storeh_pi((__m64*)out,temp);
#endif
}
#undef SSE_STORE3_SIMD

LUX_INLINE void lxVector3SSEp_transformRot(lxVector3SSEp out,  lxVector3SSEp src, lxMatrix44SSEp matrix)
{
  float old = out->m128_f32[3];
  *out = lxVector4SSE_transformRot(*src,matrix);
  out->m128_f32[3] = old;
}
LUX_INLINE void lxVector3SSEp_lerp(lxVector3SSEp out,  float s, lxVector3SSEp a, lxVector3SSEp b)
{
  float old = out->m128_f32[3];
  *out = lxVector4SSE_lerp(s,*a,*b);
  out->m128_f32[3] = old;
}
LUX_INLINE void lxVector3SSEp_catmullRom(lxVector3SSEp out,  float s, lxVector3SSEp v0, lxVector3SSEp v1,lxVector3SSEp v2, lxVector3SSEp v3)
{
  float old = out->m128_f32[3];
  *out = lxVector4SSE_catmullRom(s,v0,v1,v2,v3);
  out->m128_f32[3] = old;
}



//////////////////////////////////////////////////////////////////////////
// Matrix

LUX_INLINE void lxMatrix44SSE_clear(lxMatrix44SSEp matrix)
{
  memset(matrix,0,sizeof(lxMatrix44SSEp));
  /*
  matrix[0]=_mm_setzero_ps();
  matrix[1]=_mm_setzero_ps();
  matrix[2]=_mm_setzero_ps();
  matrix[3]=_mm_setzero_ps();
  */
}

LUX_INLINE void lxMatrix44SSE_identity(lxMatrix44SSEp matrix)
{
  memcpy(matrix,lx_gMatrix44_ident,sizeof(__m128)*4);
  /*
  matrix[0]=_mm_set_ps(0.0f,0.0f,0.0f,1.0f);
  matrix[1]=_mm_set_ps(0.0f,0.0f,1.0f,0.0f);
  matrix[2]=_mm_set_ps(0.0f,1.0f,0.0f,0.0f);
  matrix[3]=_mm_set_ps(1.0f,0.0f,0.0f,0.0f);

  matrix[0]=_mm_setzero_ps();
  matrix[1]=_mm_setzero_ps();
  matrix[2]=_mm_setzero_ps();
  matrix[3]=_mm_setzero_ps();

  matrix[0].m128_f32[0] = 
    matrix[1].m128_f32[1] = 
    matrix[2].m128_f32[2] = 
    matrix[3].m128_f32[3] = 1.0f;
  */
}

LUX_INLINE void lxMatrix44SSE_copy(lxMatrix44SSEp matrix, lxMatrix44SSEp from)
{
  
  matrix[0] = from[0];
  matrix[1] = from[1];
  matrix[2] = from[2];
  matrix[3] = from[3];
}

LUX_INLINE void lxMatrix44SSE_transpose1(lxMatrix44SSEp matrix)
{
  _MM_TRANSPOSE4_PS(matrix[0], matrix[1], matrix[2], matrix[3]);
}

LUX_INLINE void lxMatrix44SSE_transpose(lxMatrix44SSEp matrix,lxMatrix44SSEp from)
{
  __m128 tmp3, tmp2, tmp1, tmp0;                         
  tmp0   = _mm_shuffle_ps(from[0], from[1], 0x44); 
  tmp2   = _mm_shuffle_ps(from[0], from[1], 0xEE);
  tmp1   = _mm_shuffle_ps(from[2], from[3], 0x44);
  tmp3   = _mm_shuffle_ps(from[2], from[3], 0xEE); 

  matrix[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
  matrix[1] = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
  matrix[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
  matrix[3] = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
}

LUX_INLINE lxVector4SSE lxMatrix44SSE_rotateVector(lxMatrix44SSEp matrix, lxVector4SSE src)
{
  return  _mm_add_ps(
    _mm_add_ps(
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(0,0,0,0)), matrix[0]), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(1,1,1,1)), matrix[1])), 
    _mm_mul_ps(_mm_shuffle_ps(src, src, _MM_SHUFFLE(2,2,2,2)), matrix[2]));

}


LUX_INLINE void lxMatrix44SSE_multiply(lxMatrix44SSEp output, lxMatrix44SSEp mat1, lxMatrix44SSEp mat0)
{
  output[0] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  output[1] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  output[2] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  output[3] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
}

LUX_INLINE void lxMatrix44SSE_multiply1(  lxMatrix44SSEp mat1, lxMatrix44SSEp mat0)
{
  __m128 m1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  __m128 m2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  __m128 m3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  __m128 m4 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  mat1[0] = m1;
  mat1[1] = m2;
  mat1[2] = m3;
  mat1[3] = m4;
}
LUX_INLINE void lxMatrix44SSE_multiply2( lxMatrix44SSEp mat1, lxMatrix44SSEp mat0)
{

  mat0[0] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  mat0[1] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  mat0[2] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  mat0[3] = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(3,3,3,3)), mat1[3]));

}


LUX_INLINE void lxMatrix44SSE_affineInvert(lxMatrix44SSEp outmat, lxMatrix44SSEp mat)
{
  outmat[0] = mat[0];
  outmat[1] = mat[1];
  outmat[2] = mat[2];
  outmat[3] = mat[3];

  _MM_TRANSPOSE4_PS(outmat[0], outmat[1], outmat[2], outmat[3]);

  outmat[3] = _mm_add_ps(
    _mm_add_ps(
    _mm_mul_ps(
    _mm_shuffle_ps(mat[3], mat[3], _MM_SHUFFLE(0,0,0,0)), outmat[0]), 
    _mm_mul_ps(
    _mm_shuffle_ps(mat[3], mat[3], _MM_SHUFFLE(1,1,1,1)), outmat[1])), 
    _mm_mul_ps(
    _mm_shuffle_ps(mat[3], mat[3], _MM_SHUFFLE(2,2,2,2)), outmat[2]));

  outmat[3] = lxVector4SSE_negated(outmat[3]);

  outmat[0].m128_f32[3] = 0.0f;
  outmat[1].m128_f32[3] = 0.0f;
  outmat[2].m128_f32[3] = 0.0f;
  outmat[3].m128_f32[3] = 1.0f;

}

LUX_INLINE void lxMatrix44SSE_invert(lxMatrix44SSEp outmat, lxMatrix44SSEp mat)
{
  float* src = (float*)mat;
  float* dst = (float*)outmat;

  __m128 minor0, minor1, minor2, minor3;
  __m128 row0, row1, row2, row3;
  __m128 det, tmp1;

#ifdef _DEBUG
  row1 = row3 = tmp1 = mat[0];
#endif

  tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
  row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));

  row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
  row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);

  tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
  row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));

  row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
  row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

  tmp1 = _mm_mul_ps(row2, row3);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

  minor0 = _mm_mul_ps(row1, tmp1);
  minor1 = _mm_mul_ps(row0, tmp1);

  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

  minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
  minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
  minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

  tmp1 = _mm_mul_ps(row1, row2);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

  minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
  minor3 = _mm_mul_ps(row0, tmp1);

  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
  minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
  minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

  tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
  row2 = _mm_shuffle_ps(row2, row2, 0x4E);

  minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
  minor2 = _mm_mul_ps(row0, tmp1);

  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
  minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
  minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

  tmp1 = _mm_mul_ps(row0, row1);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

  minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
  minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);

  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

  minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
  minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

  tmp1 = _mm_mul_ps(row0, row3);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

  minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
  minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);

  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

  minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
  minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

  tmp1 = _mm_mul_ps(row0, row2);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

  minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
  minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));

  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

  minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
  minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

  det = _mm_mul_ps(row0, minor0);
  det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
  det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
  tmp1 = _mm_rcp_ss(det);

  det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
  det = _mm_shuffle_ps(det, det, 0x00);

  minor0 = _mm_mul_ps(det, minor0);
  _mm_storel_pi((__m64*)(dst), minor0);
  _mm_storeh_pi((__m64*)(dst+2), minor0);

  minor1 = _mm_mul_ps(det, minor1);
  _mm_storel_pi((__m64*)(dst+4), minor1);
  _mm_storeh_pi((__m64*)(dst+6), minor1);

  minor2 = _mm_mul_ps(det, minor2);
  _mm_storel_pi((__m64*)(dst+ 8), minor2);
  _mm_storeh_pi((__m64*)(dst+10), minor2);

  minor3 = _mm_mul_ps(det, minor3);
  _mm_storel_pi((__m64*)(dst+12), minor3);
  _mm_storeh_pi((__m64*)(dst+14), minor3);
}

//////////////////////////////////////////////////////////////////////////
// Matrix34SSE
LUX_INLINE void lxMatrix34SSE_identity(lxMatrix34SSEp outmat)
{
  outmat[0] = _mm_set_ps(0.0f,0.0f,0.0f,1.0f);
  outmat[1] = _mm_set_ps(0.0f,0.0f,1.0f,0.0f);
  outmat[2] = _mm_set_ps(0.0f,1.0f,0.0f,0.0f);
}

LUX_INLINE void lxMatrix34SSE_copy(lxMatrix34SSEp outmat, lxMatrix34SSEp src)
{
  outmat[0] = src[0];
  outmat[1] = src[1];
  outmat[2] = src[2];
}

LUX_INLINE void lxMatrix34SSE_tmultiply44(lxMatrix34SSEp outmat, lxMatrix44SSEp mat1,  lxMatrix44SSEp mat0)
{
  __m128 row0 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[0], mat0[0], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  __m128 row1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[1], mat0[1], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  __m128 row2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[2], mat0[2], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
  __m128 row3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(0,0,0,0)), mat1[0]), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(1,1,1,1)), mat1[1])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(2,2,2,2)), mat1[2])), _mm_mul_ps(_mm_shuffle_ps(mat0[3], mat0[3], _MM_SHUFFLE(3,3,3,3)), mat1[3]));
            
  __m128 tmp3, tmp2, tmp1, tmp0;                          
  
  tmp0   = _mm_shuffle_ps((row0), (row1), 0x44);          
  tmp2   = _mm_shuffle_ps((row0), (row1), 0xEE);          
  tmp1   = _mm_shuffle_ps((row2), (row3), 0x44);          
  tmp3   = _mm_shuffle_ps((row2), (row3), 0xEE);          
  
  outmat[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);              
  outmat[1] = _mm_shuffle_ps(tmp0, tmp1, 0xDD);              
  outmat[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);              
}


//////////////////////////////////////////////////////////////////////////
//
/*
LUX_INLINE void QuatToMatrixSSE(const Quat q,Matrix44SSE a_mat)
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
*/


#ifdef __cplusplus
}
#endif

#endif
