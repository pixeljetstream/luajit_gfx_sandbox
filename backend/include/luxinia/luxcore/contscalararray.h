// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __SCALARARRAY_H__
#define __SCALARARRAY_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <luxinia/luxcore/scalarmisc.h>

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// Scalar Array
//
// Useful for vector/scalar operations in masses.
// Provides a C Interface to templated array for different types.
// Main purpose is to use it in scripting languages.


typedef struct lxScalarArray_s{
  lxScalarType_t    type;

    // must be [1,4]
  uint        vectordim;  
    // number of vectors
  uint        count;

    // in steps to next vector (0 / >= vectordim)
    // compact: == vectordim
    // single value: == 0
  uint        stride;   

  lxScalarPtr_t     data;
}lxScalarArray_t;

typedef struct ScalarArray3D_s{
  lxScalarArray_t   sarr;

    // width,height,depth
  union{
    uint        size[3];
    struct{
      uint      width;
      uint      height;
      uint      depth;
    }sz;
  };
  
  uint        offset[3];
}lxScalarArray3D_t;

#define LUX_SCALAROPS_MAX_SUPPORTED   (LUX_SCALAR_UINT32+1)

typedef enum lxScalarArrayOp_e{
// Basic Operations
// operations are performed per vector
// vectordim either match, or last arg may have vectordim 1


  // operate on array directly
  // outarray = op(outarray)
  LUX_SCALAR_OP0_CLEAR,

  // from one to other array
  // outarray = op(arg0array)
  LUX_SCALAR_OP1_COPY,

  // outarray = op(arg0array, arg1array)
  LUX_SCALAR_OP2_ADD,
  LUX_SCALAR_OP2_SUB,
  LUX_SCALAR_OP2_MUL,
  LUX_SCALAR_OP2_DIV,
  LUX_SCALAR_OP2_MIN,
  LUX_SCALAR_OP2_MAX,
  // saturated, performs clamp to natural range post-op
  LUX_SCALAR_OP2_ADD_SAT,
  LUX_SCALAR_OP2_SUB_SAT,
  LUX_SCALAR_OP2_MUL_SAT,
  LUX_SCALAR_OP2_DIV_SAT,

  // outarray = op(arg0array, arg1array, arg2array )
  // arg2 has weights, integers are interpreted as 0-1 float
  LUX_SCALAR_OP3_LERP,
  // 1-fracc is used
  LUX_SCALAR_OP3_LERPINV,
  // arg0 + arg1*arg2
  LUX_SCALAR_OP3_MADD,
  // saturated, performs clamp to natural range post-op
  LUX_SCALAR_OP3_MADD_SAT,

  LUX_SCALAR_OPS,
}lxScalarArrayOp_t;


//////////////////////////////////////////////////////////////////////////
// ScalarArray access/update
LUX_API void* lxScalarArray_getPtr(lxScalarArray_t *sarr, uint idx);
LUX_API void* lxScalarArray_getPtr3D(lxScalarArray_t *sarr, const uint size[3], const uint coords[3]);
LUX_API void* lxScalarArray3D_getPtr(lxScalarArray3D_t *sarr, const uint coords[3]);

// updates count of elements and data ptr, for a "offset region"
// operation, returns TRUE on error
LUX_API booln lxScalarArray3D_setDataOffset(lxScalarArray3D_t *sarr, uint start[3], void* data);
LUX_API void  lxScalarArray3D_setData(lxScalarArray3D_t *sarr, void* data);

/////////////////////////////////////////////////////////////////////////
// ScalarArray Operations
//
// all operations must be of same type
// no error checking on type is performed!
// return TRUE on other errors

LUX_API lxScalarArray_t* lxScalarArray_init(lxScalarArray_t *ret, 
    lxScalarType_t intype, void *pin, uint vectordim, uint stride, uint count);

  // if you want a single "virtual" array (stride set to 0)
LUX_API lxScalarArray_t*  lxScalarArray_initSingle(lxScalarArray_t *ret, lxScalarType_t intype, void *pin, uint vectordim);
LUX_API lxScalarArray_t   lxScalarArray_newSingle(lxScalarType_t intype, void *pin, uint vectordim);

LUX_API booln lxScalarArray_Op0(lxScalarArray_t *ret, lxScalarArrayOp_t op);
LUX_API booln lxScalarArray_Op1(lxScalarArray_t *ret, lxScalarArrayOp_t op, 
      const lxScalarArray_t *arg0);
LUX_API booln lxScalarArray_Op2(lxScalarArray_t *ret, lxScalarArrayOp_t op, 
      const lxScalarArray_t *arg0,  const lxScalarArray_t *arg1);
LUX_API booln lxScalarArray_Op3(lxScalarArray_t *ret, lxScalarArrayOp_t op, 
            const lxScalarArray_t *arg0,  const lxScalarArray_t *arg1,  const lxScalarArray_t *arg2);

LUX_API booln lxScalarArray3D_Op0(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op);
LUX_API booln lxScalarArray3D_Op1(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op,
            const lxScalarArray3D_t *arg0);
LUX_API booln lxScalarArray3D_Op2(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op,
            const lxScalarArray3D_t *arg0,  const lxScalarArray3D_t *arg1);
LUX_API booln lxScalarArray3D_Op3(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op,
            const lxScalarArray3D_t *arg0,  const lxScalarArray3D_t *arg1,  const lxScalarArray3D_t *arg2);

//booln ScalarArray3D_convolute(ScalarArray3D_t *ret, const ScalarArray3D_t *arg0, const ScalarArray3D_t *weights, booln wrap);

//////////////////////////////////////////////////////////////////////////
// ScalarArray Conversion

// straight casting, no checks on range
LUX_API booln lxScalarArray_convert(lxScalarArray_t *sarrayOut, const lxScalarArray_t *sarrayIn);

// clamps input values, and scales them to output range
// first element of vector is min, second is max
// interpolation is performed in
// float for most types,
// double for 32bit integers
LUX_API booln lxScalarArray_convertRanged(
    lxScalarArray_t *sarrayOut, const lxScalarVector_t* outminmax, 
    const lxScalarArray_t *sarrayIn,  const lxScalarVector_t* inminmax);

// scales inputs/outputs to saturation range of other type
// float uses -1,1 for signed and 0,1 for unsigned
LUX_API booln lxScalarArray_convertNormalized(
    lxScalarArray_t *sarrayOut, const lxScalarArray_t *sarrayIn);

//////////////////////////////////////////////////////////////////////////
// ScalarArray Interpolated (in float precision)
//
// Spline interpolation done with CatmullRom

// assumes constant timespaces between values
//booln ScalarArray_splinePointEven(float *outvals, const ScalarArray_t *sarray, float fracc, booln closed);
//booln ScalarArray_linearPointEven(float *outvals, const ScalarArray_t *sarray, float fracc, booln closed);

// times must be float1 array
//booln ScalarArray_splinePointTimed(float *outvals, const ScalarArray_t *sarray, 
//      float time, booln closed, const ScalarArray_t *floattimes);
//booln ScalarArray_linearPointTimed(float *outvals, const ScalarArray_t *sarray, 
//      float time, booln closed, const ScalarArray_t *floattimes);

  // returns cubeside(0:+X,1:-X,...) and 2D coords
LUX_API uint lxSampleCubeTo2DCoord(float coordsout[2], const float coords[3]);

LUX_API booln lxScalarArray_sampleLinear(float* LUX_RESTRICT outvals, const lxScalarArray_t *sarray, 
      const uint size[3], const float coords[3], booln notclamped[3]);

LUX_API booln lxScalarArray3D_sampleLinear(float* LUX_RESTRICT outvals, const lxScalarArray3D_t *sarray, 
                const float coords[3], booln notclamped[3]);

// output array will contain interpolated
// points between argarray's points
LUX_API booln lxScalarArray_curveLinear(lxScalarArray_t *sarray, const lxScalarArray_t *sarray0, booln closed);
LUX_API booln lxScalarArray_curveSpline(lxScalarArray_t *sarray, const lxScalarArray_t *sarray0, booln closed);

//////////////////////////////////////////////////////////////////////////
// ScalarArray Float Only

typedef enum lxFScalarArrayOp_e{
  // arg is Matrix44
  LUX_FSCALAR_OP1_TRANSFORM,
  LUX_FSCALAR_OP1_TRANSFORMROT,
  LUX_FSCALAR_OP1_TRANSFORMFULL,
  // arg is NULL
  LUX_FSCALAR_OP1_NORMALIZE,
  LUX_FSCALAR_OP1_NORMALIZEACC,
  LUX_FSCALAR_OP1S,
}lxFScalarArrayOp_t;

  // works on vector2,3,4
  // sarray/sarray0 & their datapointers can be identical, proper code will be called
  // vector4 uses vector3 length
LUX_API booln lxFScalarArray_op1(lxScalarArray_t *sarray, lxFScalarArrayOp_t op, const lxScalarArray_t *sarray0, const float * LUX_RESTRICT arg);

  // sarraylen = vector1
  // sarraypath = vector1,2,3,4
  // interprets sarraypath as continous path 
  // writes relative length to all vectorcomponents of sarray
  // vector4 uses vector3 length
LUX_API booln lxFScalarArray_relLength(lxScalarArray_t *sarraylen, lxScalarArray_t *sarraypath, float *outlength);

//////////////////////////////////////////////////////////////////////////
// INLINE

LUX_INLINE lxScalarArray_t* lxScalarArray_init(lxScalarArray_t *ret, 
    lxScalarType_t intype, void *pin, uint vectordim, uint stride, uint count)
{
  ret->type = intype;
  ret->vectordim = vectordim;
  ret->count = count;
  ret->stride = stride;
  ret->data.tvoid = pin;

  return ret;
}

LUX_INLINE lxScalarArray_t* lxScalarArray_initSingle(lxScalarArray_t *ret, 
            lxScalarType_t intype, void *pin, uint vectordim)
{
  ret->type = intype;
  ret->vectordim = vectordim;
  ret->count = 1;
  ret->stride = 0;
  ret->data.tvoid = pin;

  return ret;
}
LUX_INLINE lxScalarArray_t  lxScalarArray_newSingle(lxScalarType_t intype, void *pin, uint vectordim)
{
  lxScalarArray_t ret;
  lxScalarArray_initSingle(&ret,intype,pin,vectordim);
  return ret;
}

LUX_INLINE void lxScalarArray3D_setData(lxScalarArray3D_t *sarr3d, void* data)
{
  sarr3d->sarr.data.tvoid = data;
  sarr3d->sarr.count = sarr3d->size[0]*sarr3d->size[1]*sarr3d->size[2];
  sarr3d->offset[0] = sarr3d->offset[1] = sarr3d->offset[2] = 0;
}


LUX_INLINE void*  lxScalarArray_getPtr(lxScalarArray_t *sarr, uint idx)
{
  return idx < sarr->count ? &sarr->data.tuint8[idx * lxScalarType_getSize(sarr->type) * sarr->vectordim] : NULL;
}
LUX_INLINE void*  lxScalarArray_getPtr3D(lxScalarArray_t *sarr, const uint size[3], const uint coords[3])
{
  uint idx = coords[0] + (coords[1] + coords[2]*size[1])*size[0];
  return idx < sarr->count ? &sarr->data.tuint8[idx * lxScalarType_getSize(sarr->type) * sarr->vectordim] : NULL;
}
LUX_INLINE void*  lxScalarArray3D_getPtr(lxScalarArray3D_t *sarr, const uint coords[3])
{
  return lxScalarArray_getPtr3D(&sarr->sarr,sarr->size,coords);
}

#ifdef __cplusplus
};  
#endif

#endif

