// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifdef __cplusplus
extern "C"{
#endif

#include <luxinia/luxcore/contscalararray.h>
#include <luxinia/luxplatform/debug.h>
#include <luxinia/luxmath/vector2.h>
#include <luxinia/luxmath/matrix44.h>
#include <luxinia/luxmath/simdmath.h>
#include <memory.h>

#ifdef __cplusplus
}
#endif

#if defined(LUX_SIMD_SSE)
#define SCALAR_USE_XMM
#endif

//////////////////////////////////////////////////////////////////////////

size_t lx_gScalarTypeSizes[LUX_SCALARS] = {
  sizeof(float),
  sizeof(int8),
  sizeof(uint8),
  sizeof(int16),
  sizeof(uint16),
  sizeof(int32),
  sizeof(uint32),
  sizeof(float16),
  sizeof(float64),
  0,
};

static int l_ScalarMax[LUX_SCALARS] = {
  1,
  127,
  255,
  LUX_SHORT_SIGNEDMAX,
  LUX_SHORT_UNSIGNEDMAX,
  LUX_SHORT_SIGNEDMAX,
  LUX_SHORT_UNSIGNEDMAX,
  1,
  1,
  0,
};

static int l_ScalarMin[LUX_SCALARS] = {
  0,
  -127,
  0,
  -LUX_SHORT_SIGNEDMAX,
  0,
  -LUX_SHORT_SIGNEDMAX,
  0,

  0,
  0,
  0,
};

static booln l_ScalarSign[LUX_SCALARS] = {
  LUX_TRUE,
  
  LUX_TRUE,
  LUX_FALSE,
  LUX_TRUE,
  LUX_FALSE,
  LUX_TRUE,
  LUX_FALSE,

  LUX_TRUE,
  LUX_TRUE,
  LUX_FALSE,
};

typedef struct ScalarNormalize_s{
  float cmin;
  float cmax;
  float mul;
  float div;
}ScalarNormalize_t;


//////////////////////////////////////////////////////////////////////////
// ScalarArray access

LUX_API booln lxScalarArray3D_setDataOffset(lxScalarArray3D_t *sarr3d, uint start[3], void* data)
{
  byte* pData = (byte*)data;
  if (start[0] >= sarr3d->size[0] || start[1] >= sarr3d->size[1] || start[2] >= sarr3d->size[2]){
    return LUX_TRUE;
  }
  uint xysize = sarr3d->size[0]*sarr3d->size[1];
  uint offset = (start[0] + (start[1]*sarr3d->size[0]) + (start[2]*xysize));

  pData += lx_gScalarTypeSizes[sarr3d->sarr.type] * offset * sarr3d->sarr.stride;
  sarr3d->sarr.data.tvoid = pData;
  sarr3d->sarr.count = (xysize*sarr3d->size[2]) - offset;
  sarr3d->offset[0] = start[0];
  sarr3d->offset[1] = start[1];
  sarr3d->offset[2] = start[2];

  return LUX_FALSE;
}


//////////////////////////////////////////////////////////////////////////
// Conversion
typedef void (LUX_FASTCALL TScalarType_convert_fn)(void* pout, const void *pin, uint vectordim);

template <class Tout,class Tin>
void LUX_FASTCALL TScalarType_convert(void* pout, const void *pin, uint vectordim)
{
  const Tin* from = (const Tin*)pin;
  Tout *to = (Tout*)pout;

  switch(vectordim){
  case 4:
    to[0] = (Tout)from[0];
    to[1] = (Tout)from[1];
    to[2] = (Tout)from[2];
    to[3] = (Tout)from[3];
    break;
  case 3:
    to[0] = (Tout)from[0];
    to[1] = (Tout)from[1];
    to[2] = (Tout)from[2];
    break;
  case 2:
    to[0] = (Tout)from[0];
    to[1] = (Tout)from[1];
    break;
  case 1:
    to[0] = (Tout)from[0];
    break;
  default:
    break;
  }
}


void LUX_INLINE Float_normalizeClamped(float* to, const float *from, uint vectordim, const ScalarNormalize_t &nrm)
{
  switch(vectordim){
  case 4:
    to[0] = LUX_CLAMP(from[0],nrm.cmin,nrm.cmax)*nrm.mul;
    to[1] = LUX_CLAMP(from[1],nrm.cmin,nrm.cmax)*nrm.mul;
    to[2] = LUX_CLAMP(from[2],nrm.cmin,nrm.cmax)*nrm.mul;
    to[3] = LUX_CLAMP(from[3],nrm.cmin,nrm.cmax)*nrm.mul;
    break;
  case 3:
    to[0] = LUX_CLAMP(from[0],nrm.cmin,nrm.cmax)*nrm.mul;
    to[1] = LUX_CLAMP(from[1],nrm.cmin,nrm.cmax)*nrm.mul;
    to[2] = LUX_CLAMP(from[2],nrm.cmin,nrm.cmax)*nrm.mul;
    break;
  case 2:
    to[0] = LUX_CLAMP(from[0],nrm.cmin,nrm.cmax)*nrm.mul;
    to[1] = LUX_CLAMP(from[1],nrm.cmin,nrm.cmax)*nrm.mul;
    break;
  case 1:
    to[0] = LUX_CLAMP(from[0],nrm.cmin,nrm.cmax)*nrm.mul;
    break;
  default:
    break;
  }
}

void LUX_INLINE Float_normalize(float* to, const float *from, uint vectordim, const ScalarNormalize_t &nrm)
{
  switch(vectordim){
  case 4:
    to[0] = from[0]*nrm.div;
    to[1] = from[1]*nrm.div;
    to[2] = from[2]*nrm.div;
    to[3] = from[3]*nrm.div;
    break;
  case 3:
    to[0] = from[0]*nrm.div;
    to[1] = from[1]*nrm.div;
    to[2] = from[2]*nrm.div;
    break;
  case 2:
    to[0] = from[0]*nrm.div;
    to[1] = from[1]*nrm.div;
    break;
  case 1:
    to[0] = from[0]*nrm.div;
    break;
  default:
    break;
  }
}


static TScalarType_convert_fn* l_TtoFloat[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarType_convert<float,float>,
  TScalarType_convert<float,int8>,
  TScalarType_convert<float,uint8>,
  TScalarType_convert<float,int16>,
  TScalarType_convert<float,uint16>,
  TScalarType_convert<float,int32>,
  TScalarType_convert<float,uint32>,
};
static TScalarType_convert_fn* l_TfromFloat[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarType_convert<float,float>,
  TScalarType_convert<int8,float>,
  TScalarType_convert<uint8,float>,
  TScalarType_convert<int16,float>,
  TScalarType_convert<uint16,float>,
  TScalarType_convert<int32,float>,
  TScalarType_convert<uint32,float>,
};

void lxScalarType_toFloat(float* pout, lxScalarType_t intype, const void *pin, uint vectordim)
{
  LUX_DEBUGASSERT(intype < LUX_SCALAROPS_MAX_SUPPORTED);
  l_TtoFloat[intype](pout,pin,vectordim);
}
void lxScalarType_fromFloat(void* pout, lxScalarType_t outtype, const float *pin, uint vectordim)
{
  LUX_DEBUGASSERT(outtype < LUX_SCALAROPS_MAX_SUPPORTED);
  l_TfromFloat[outtype](pout,pin,vectordim);
}
static const ScalarNormalize_t l_FloatNormalize[LUX_SCALAROPS_MAX_SUPPORTED] = {
  {-1.0f,1.0f,1.0f,1.0f },
  {-1.0f,1.0f,127.0f,1.0f/127.0f },
  {0.0f,1.0f,255.0f,1.0f/255.0f },
  {-1.0f,1.0f,LUX_MUL_SHORT,LUX_DIV_SHORT },
  {0.0f,1.0f,LUX_MUL_USHORT,LUX_DIV_USHORT },
  {-1.0f,1.0f,LUX_MUL_SHORT,LUX_DIV_SHORT },
  {0.0f,1.0f,LUX_MUL_USHORT,LUX_DIV_USHORT },
};

void lxScalarType_toFloatNormalized(float* pout, lxScalarType_t intype, void *pin, uint vectordim)
{
  lxVector4 temp;
  LUX_DEBUGASSERT(intype < LUX_SCALAROPS_MAX_SUPPORTED);
  l_TtoFloat[intype](temp,pin,vectordim);
  Float_normalize(pout,temp,vectordim,l_FloatNormalize[intype]);
}

void lxScalarType_fromFloatNormalized(void* pout, lxScalarType_t outtype, const float *pin, uint vectordim)
{
  lxVector4 temp;
  LUX_DEBUGASSERT(outtype < LUX_SCALAROPS_MAX_SUPPORTED);
  if (outtype != LUX_SCALAR_UINT8){
    Float_normalizeClamped(temp,pin,vectordim,l_FloatNormalize[outtype]);
    l_TfromFloat[outtype](pout,temp,vectordim);
  }
  else{
    uint8 *ub = (uint8*)pout;
    switch(vectordim)
    {
    case 4:
      LUX_FP_NORM_TO_BYTE((ub)[0],(pin)[0]);
      LUX_FP_NORM_TO_BYTE((ub)[1],(pin)[1]);
      LUX_FP_NORM_TO_BYTE((ub)[2],(pin)[2]);
      LUX_FP_NORM_TO_BYTE((ub)[3],(pin)[3]);
      return;
    case 3:
      LUX_FP_NORM_TO_BYTE((ub)[0],(pin)[0]);
      LUX_FP_NORM_TO_BYTE((ub)[1],(pin)[1]);
      LUX_FP_NORM_TO_BYTE((ub)[2],(pin)[2]);
      return;
    case 2:
      LUX_FP_NORM_TO_BYTE((ub)[0],(pin)[0]);
      LUX_FP_NORM_TO_BYTE((ub)[1],(pin)[1]);
      return;
    case 1:
      LUX_FP_NORM_TO_BYTE((ub)[0],(pin)[0]);
      return;
    }
  }
}

void lxScalarType_normalizedFloat(float* pout, lxScalarType_t intype, float *pin, uint vectordim)
{
  LUX_DEBUGASSERT(intype < LUX_SCALAROPS_MAX_SUPPORTED);
  Float_normalize(pout,pin,vectordim,l_FloatNormalize[intype]);
}

void lxScalarType_from32(lxScalarVector_t *pout, lxScalarType_t type, void *pin, uint vectordim)
{
  LUX_DEBUGASSERT(type < LUX_SCALAROPS_MAX_SUPPORTED);
  switch(type){
  case LUX_SCALAR_FLOAT32:
    {
      float *pfloat = (float*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tfloat[i] =  pfloat[i];
      }
    }
    break;
  case LUX_SCALAR_INT32:
    {
      int32 *pint = (int32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint32[i] =  pint[i];
      }
    }
    break;
  case LUX_SCALAR_UINT32:
    {
      int32 *pint = (int32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tuint32[i] =  LUX_MAX(0,pint[i]);
      }
    }
    break;
  case LUX_SCALAR_INT8:
    {
      int32 *pint = (int32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint8[i] =  LUX_CLAMP(pint[i],-127,127);
      }
    }
    break;
  case LUX_SCALAR_UINT8:
    {
      int32 *pint = (int32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tuint8[i] =  LUX_CLAMP(pint[i],0,255);
      }
    }
    break;
  case LUX_SCALAR_INT16:
    {
      int32 *pint = (int32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint16[i] =  LUX_CLAMP(pint[i],-LUX_SHORT_SIGNEDMAX,LUX_SHORT_SIGNEDMAX);
      }
    }
    break;
  case LUX_SCALAR_UINT16:
    {
      int32 *pint = (int32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tuint16[i] =  LUX_CLAMP(pint[i],0,LUX_SHORT_UNSIGNEDMAX);
      }
    }
    break;
  }
}

void lxScalarType_to32(lxScalarVector_t *pout, lxScalarType_t type, void *pin, uint vectordim)
{
  LUX_DEBUGASSERT(type < LUX_SCALAROPS_MAX_SUPPORTED);
  switch(type){
  case LUX_SCALAR_FLOAT32:
    {
      float *pfloat = (float*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tfloat[i] = pfloat[i];
      }
    }
    break;
  case LUX_SCALAR_INT32:
    {
      int32 *pint = (int32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint32[i] = pint[i];
      }
    }
    break;
  case LUX_SCALAR_UINT32:
    {
      uint32 *pint = (uint32*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint32[i] = LUX_MIN(pint[i],INT_MAX);
      }
    }
    break;
  case LUX_SCALAR_INT8:
    {
      int8 *pint = (int8*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint32[i] = pint[i];
      }
    }
    break;
  case LUX_SCALAR_UINT8:
    {
      uint8 *pint = (uint8*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint32[i] = pint[i];
      }
    }
    break;
  case LUX_SCALAR_INT16:
    {
      int16 *pint = (int16*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint32[i] = pint[i];
      }
    }
  case LUX_SCALAR_UINT16:
    {
      uint16 *pint = (uint16*)pin;
      for (uint i = 0; i < vectordim; i++){
        pout->tint32[i] = pint[i];
      }
    }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
// ScalarLoop for setting up iterations

#define SCALAR_UNROLL_HIGH  4
#define SCALAR_UNROLL_LOW 3
//#define SCALAR_UNROLL_FLEXIBLE


//////////////////////////////////////////////////////////////////////////
// Single Dimension
//
// vectordim might be overridden for loop unrolling
// checkUnrolled updates .cnt
// .cnt is "single scalars left"

class ScalarLoop{
public:
  uint    vectordim;
  uint    stride;
  uint    cnt;
  uint    cntvec;

  ScalarLoop(const lxScalarArray_t &sarray);
  void checkUnrolled(void *&pOut);
};

class ScalarLoop1{
public:
  size_t    vectordim;
  size_t    stride;
  size_t    stride0;
  size_t    cnt;
  size_t    cntvec;

  ScalarLoop1(const lxScalarArray_t &sarray,const lxScalarArray_t &sarray0);
  void checkUnrolled(void *&pOut,const void *&pArg0);
};

class ScalarLoop2{
public:
  uint    vectordim;
  uint    stride;
  uint    stride0;
  uint    stride1;
  uint    cnt;
  uint    cntvec;

  ScalarLoop2(const lxScalarArray_t &sarray,const lxScalarArray_t &sarray0,
        const lxScalarArray_t &sarray1);
  void checkUnrolled(void *&pOut,const void *&pArg0,const void *&pArg1);
};

class ScalarLoop3{
public:
  uint    vectordim;
  uint    stride;
  uint    stride0;
  uint    stride1;
  uint    stride2;
  uint    cnt;
  uint    cntvec;

  ScalarLoop3(const lxScalarArray_t &sarray,const lxScalarArray_t &sarray0,
        const lxScalarArray_t &sarray1,const lxScalarArray_t &sarray2);
  void checkUnrolled(void *&pOut,const void *&pArg0,const void *&pArg1,const void *&pArg2);
};

LUX_INLINE ScalarLoop::ScalarLoop(const lxScalarArray_t &sarray)
{
  vectordim = sarray.vectordim;
  stride =  sarray.stride;
  cnt = sarray.count * vectordim;
  
  
  // no gaps, can maximize vectordim
  if (stride == vectordim){
    vectordim = SCALAR_UNROLL_HIGH;
    stride = vectordim;
  }

  cntvec = cnt / vectordim;
}


LUX_INLINE void ScalarLoop::checkUnrolled(void *&pOut)
{
  cnt -= vectordim * cntvec;

#if SCALAR_UNROLL_FLEXIBLE
  vectordim = SCALAR_UNROLL_LOW;
  while (cnt / vectordim < 2){
    vectordim--;
  }
  {
#else
  vectordim = cnt;
  LUX_DEBUGASSERT(vectordim <= 4);
  if (vectordim){
#endif
    cntvec = cnt / vectordim;
    stride = vectordim;
  }
}

LUX_INLINE ScalarLoop1::ScalarLoop1(const lxScalarArray_t &sarray, 
  const lxScalarArray_t &sarray0)
{

  vectordim = sarray.vectordim;
  stride  = sarray.stride;
  stride0 = sarray0.stride;
  cnt = LUX_MIN(sarray.count,sarray0.count) * vectordim;
  

  // no gaps, can maximize vectordim
  if (stride == vectordim 
    && (stride0 == vectordim || (sarray0.vectordim == 1 && stride0 == 0)))
  {
    vectordim = SCALAR_UNROLL_HIGH;
    stride = vectordim;
    stride0 = vectordim * (stride0!=0);
  }

  cntvec = cnt / vectordim;
}


LUX_INLINE void ScalarLoop1::checkUnrolled(void *&pOut,const void *&pArg0)
{
  cnt -= vectordim * cntvec;

#if SCALAR_UNROLL_FLEXIBLE
  vectordim = SCALAR_UNROLL_LOW;
  while (cnt / vectordim < 2){
    vectordim--;
  }
  {
#else
  vectordim = cnt;
  LUX_DEBUGASSERT(vectordim <= 4);
  if (vectordim){
#endif
    stride = vectordim;
    stride0 = vectordim * (stride0!=0);
    cntvec = cnt / vectordim;
  }
}

LUX_INLINE ScalarLoop2::ScalarLoop2(const lxScalarArray_t &sarray, 
  const lxScalarArray_t &sarray0, const lxScalarArray_t &sarray1)
{

  vectordim = sarray.vectordim;
  stride =  sarray.stride;
  stride0 = sarray0.stride;
  stride1 = sarray1.stride;
  cnt = LUX_MIN(LUX_MIN(sarray.count,sarray0.count),sarray1.count) * vectordim;


  // no gaps, can maximize vectordim
  if (stride == vectordim 
    && stride0 == vectordim && (stride1 == vectordim || (sarray1.vectordim == 1 && stride1 == 0)))
  {
    vectordim = SCALAR_UNROLL_HIGH;
    stride = vectordim;
    stride0 = vectordim;
    stride1 = vectordim * (stride1!=0);
  }

  cntvec = cnt / vectordim;

}


LUX_INLINE void ScalarLoop2::checkUnrolled(void *&pOut,const void *&pArg0,const void *&pArg1)
{
  cnt -= vectordim * cntvec;

#if SCALAR_UNROLL_FLEXIBLE
  vectordim = SCALAR_UNROLL_LOW;
  while (cnt / vectordim < 2){
    vectordim--;
  }
  {
#else
  LUX_DEBUGASSERT(vectordim <= 4);
  if (vectordim){
#endif
    stride = vectordim;
    stride0 = vectordim;
    stride1 = vectordim * (stride1!=0);
    cntvec = cnt / vectordim;
  }
}

LUX_INLINE ScalarLoop3::ScalarLoop3(const lxScalarArray_t &sarray, 
  const lxScalarArray_t &sarray0, const lxScalarArray_t &sarray1, const lxScalarArray_t &sarray2)
{

  vectordim = sarray.vectordim;
  stride =  sarray.stride;
  stride0 = sarray0.stride;
  stride1 = sarray1.stride;
  stride2 = sarray2.stride;
  cnt = LUX_MIN(LUX_MIN(LUX_MIN(sarray.count,sarray0.count),sarray1.count),sarray2.count);


  // no gaps, can maximize vectordim
  if (stride == vectordim 
    && stride0 == vectordim && stride1 == vectordim 
    && (stride2 == vectordim || (sarray2.vectordim == 1 && stride2 == 0)))
  {
    vectordim = SCALAR_UNROLL_HIGH;
    stride = vectordim;
    stride0 = vectordim;
    stride1 = vectordim;
    stride2 = vectordim * (stride2!=0);
  }

  cntvec = cnt / vectordim;
}


LUX_INLINE void ScalarLoop3::checkUnrolled(void* &pOut,const void* &pArg0,const void* &pArg1,const void* &pArg2)
{
  cnt -= vectordim * cntvec;

#if SCALAR_UNROLL_FLEXIBLE
  vectordim = SCALAR_UNROLL_LOW;
  while (cnt / vectordim < 2){
    vectordim--;
  }
  {
#else
  vectordim = cnt;
  LUX_DEBUGASSERT(vectordim <= 4);
  if (vectordim){
#endif
    stride = vectordim;
    stride0 = vectordim;
    stride1 = vectordim;
    stride2 = vectordim * (stride2!=0);
    cntvec = cnt / vectordim;
  }
}
//////////////////////////////////////////////////////////////////////////
// Multi Dimensional
//
// doesnt change vectordim, but uses unrollCheck to advance pointers
// and keep loop running.
// cnt is "rows left"

class Scalar3DLoop{
public:
  uint    stride;
  uint    advance[2];

  uint    regionrow;

  uint    vectordim;
  uint    cnt;
  uint    cntvec;
  
  booln   valid;

  Scalar3DLoop(const lxScalarArray3D_t &sarray, uint region[3]);

  void checkUnrolled(void *& pOut);
};

class Scalar3DLoop1{
public:
  uint    stride;
  uint    stride0;
  uint    advance[2];
  uint    advance0[2];

  uint    regionrow;

  uint    vectordim;
  uint    cnt;
  uint    cntvec;

  booln   valid;

  Scalar3DLoop1(const lxScalarArray3D_t &sarray, uint region[3],
    const lxScalarArray3D_t &sarray0);

  void checkUnrolled(void *& pOut,const void *&pArg0);
};

class Scalar3DLoop2{
public:
  uint    stride;
  uint    stride0;
  uint    stride1;
  uint    advance[2];
  uint    advance0[2];
  uint    advance1[2];

  uint    regionrow;

  uint    vectordim;
  uint    cnt;
  uint    cntvec;

  booln   valid;

  Scalar3DLoop2(const lxScalarArray3D_t &sarray, uint region[3],
    const lxScalarArray3D_t &sarray0,const lxScalarArray3D_t &sarray1);

  void checkUnrolled(void *& pOut,const void *&pArg0,const void *&pArg1);
};

class Scalar3DLoop3{
public:
  uint    stride;
  uint    stride0;
  uint    stride1;
  uint    stride2;
  uint    advance[2];
  uint    advance0[2];
  uint    advance1[2];
  uint    advance2[2];

  uint    regionrow;

  uint    vectordim;
  uint    cnt;
  uint    cntvec;

  booln   valid;

  Scalar3DLoop3(const lxScalarArray3D_t &sarray, uint region[3],
    const lxScalarArray3D_t &sarray0,const lxScalarArray3D_t &sarray1,const lxScalarArray3D_t &sarray2);

  void checkUnrolled(void *& pOut,const void* &pArg0,const void* &pArg1,const void* &pArg2);
};

#define SCALAR_INIT_LOOP3D(ostride,oadvance,osa3d) \
    ostride = osa3d.sarr.stride;  \
    oadvance[0] = (osa3d.size[0]-region[0])*sizemul*osa3d.sarr.stride;  \
    oadvance[1] = (osa3d.size[1]-region[1])*osa3d.size[0]*sizemul*osa3d.sarr.stride;

#define SCALAR_VALID_LOOP3D(sarray) \
  (!sarray.sarr.stride ||( \
    sarray.size[0] >= region[0]+sarray.offset[0] && \
    sarray.size[1] >= region[1]+sarray.offset[1] && \
    sarray.size[2] >= region[2]+sarray.offset[2]))

LUX_INLINE Scalar3DLoop::Scalar3DLoop(const lxScalarArray3D_t &sarray, uint region[3])
{
  size_t sizemul = lx_gScalarTypeSizes[sarray.sarr.type];
  vectordim = sarray.sarr.vectordim;
  
  SCALAR_INIT_LOOP3D(stride,advance,sarray);
  
  regionrow = region[1];
  cnt = region[1] * region[2];
  cntvec = region[0];

  valid = SCALAR_VALID_LOOP3D(sarray);
}


LUX_INLINE void Scalar3DLoop::checkUnrolled(void *&pOut)
{
  byte *& pOutB = (byte *&)pOut;
  cnt--;

  uint row = (cnt % regionrow) == 0;
  pOutB += advance[0] + (row * advance[1]);
}

LUX_INLINE Scalar3DLoop1::Scalar3DLoop1(const lxScalarArray3D_t &sarray, uint region[3],
                    const lxScalarArray3D_t &sarray0)
{
  size_t sizemul = lx_gScalarTypeSizes[sarray.sarr.type];
  vectordim = sarray.sarr.vectordim;

  SCALAR_INIT_LOOP3D(stride,advance,sarray);
  SCALAR_INIT_LOOP3D(stride0,advance0,sarray0);

  regionrow = region[1];
  cnt = region[1] * region[2];
  cntvec = region[0];

  valid = SCALAR_VALID_LOOP3D(sarray)  
      && SCALAR_VALID_LOOP3D(sarray0);
}


LUX_INLINE void Scalar3DLoop1::checkUnrolled(void *&pOut,const void *&pArg0)
{
  byte *& pOutB = (byte *&)pOut;
  byte *& pArg0B = (byte *&)pArg0;

  cnt--;

  uint row = (cnt % regionrow) == 0;
  pOutB  += advance[0]  + (row * advance[1]);
  pArg0B += advance0[0] + (row * advance0[1]);
}

LUX_INLINE Scalar3DLoop2::Scalar3DLoop2(const lxScalarArray3D_t &sarray, uint region[3],
                    const lxScalarArray3D_t &sarray0,const lxScalarArray3D_t &sarray1)
{
  size_t sizemul = lx_gScalarTypeSizes[sarray.sarr.type];
  vectordim = sarray.sarr.vectordim;

  SCALAR_INIT_LOOP3D(stride,advance,sarray);
  SCALAR_INIT_LOOP3D(stride0,advance0,sarray0);
  SCALAR_INIT_LOOP3D(stride1,advance1,sarray1);

  regionrow = region[1];
  cnt = region[1] * region[2];
  cntvec = region[0];

  valid = SCALAR_VALID_LOOP3D(sarray)  
    && SCALAR_VALID_LOOP3D(sarray0)
    && SCALAR_VALID_LOOP3D(sarray1);
}


LUX_INLINE void Scalar3DLoop2::checkUnrolled(void *&pOut,const void *&pArg0,const void *&pArg1)
{
  byte *& pOutB = (byte *&)pOut;
  byte *& pArg0B = (byte *&)pArg0;
  byte *& pArg1B = (byte *&)pArg1;

  cnt--;

  uint row = (cnt % regionrow) == 0;
  pOutB  += advance[0]  + (row * advance[1]);
  pArg0B += advance0[0] + (row * advance0[1]);
  pArg1B += advance1[0] + (row * advance1[1]);
}

LUX_INLINE Scalar3DLoop3::Scalar3DLoop3(const lxScalarArray3D_t &sarray, uint region[3],
                    const lxScalarArray3D_t &sarray0,const lxScalarArray3D_t &sarray1,const lxScalarArray3D_t &sarray2)
{
  size_t sizemul = lx_gScalarTypeSizes[sarray.sarr.type];
  vectordim = sarray.sarr.vectordim;

  SCALAR_INIT_LOOP3D(stride,advance,sarray);
  SCALAR_INIT_LOOP3D(stride0,advance0,sarray0);
  SCALAR_INIT_LOOP3D(stride1,advance1,sarray1);
  SCALAR_INIT_LOOP3D(stride2,advance2,sarray2);

  regionrow = region[1];
  cnt = region[1] * region[2];
  cntvec = region[0];

  uint tsize = cnt * cntvec;
  valid = SCALAR_VALID_LOOP3D(sarray)  
    && SCALAR_VALID_LOOP3D(sarray0)
    && SCALAR_VALID_LOOP3D(sarray1)
    && SCALAR_VALID_LOOP3D(sarray2);
}


LUX_INLINE void Scalar3DLoop3::checkUnrolled(void *&pOut,const void *&pArg0,const void *&pArg1,const void *&pArg2)
{
  byte *& pOutB = (byte *&)pOut;
  byte *& pArg0B = (byte *&)pArg0;
  byte *& pArg1B = (byte *&)pArg1;
  byte *& pArg2B = (byte *&)pArg2;
  
  cnt--;

  uint row = (cnt % regionrow) == 0;
  pOutB  += advance[0]  + (row * advance[1]);
  pArg0B += advance0[0] + (row * advance0[1]);
  pArg1B += advance1[0] + (row * advance1[1]);
  pArg2B += advance2[0] + (row * advance2[1]);
}


#undef SCALAR_INIT_LOOP3D
#undef SCALAR_VALID_LOOP3D

//////////////////////////////////////////////////////////////////////////
// Operations (common types)

#define SCALAR_VECTOR_LOOP(ptrincr,funcprep,func) \
  switch(loop.vectordim)    \
  {   \
  case 4:   \
    {   \
      for (size_t i = 0; i < loop.cntvec; i ++, ptrincr){   \
        funcprep;   \
        func( 0 );    \
        func( 1 );    \
        func( 2 );    \
        func( 3 );    \
      }   \
    }   \
    break;    \
  case 3:   \
    {   \
      for (size_t i = 0; i < loop.cntvec; i ++, ptrincr){   \
        funcprep;   \
        func( 0 );    \
        func( 1 );    \
        func( 2 );    \
      }   \
    }   \
    break;    \
  case 2:   \
    {   \
      for (size_t i = 0; i < loop.cntvec; i ++, ptrincr){   \
        funcprep;   \
        func( 0 );    \
        func( 1 );    \
      }   \
    }   \
    break;    \
  case 1:   \
    {   \
      for (size_t i = 0; i < loop.cntvec; i ++, ptrincr){   \
        funcprep;   \
        func( 0 );    \
      }   \
    }   \
    break;    \
  default:    \
    LUX_ASSERT("never get here" == NULL);   \
    break;    \
  }   


// OP 0
template <class T, class TScalarLoop>
booln LUX_FASTCALL TScalarArrayOp_in0(lxScalarArrayOp_t op, TScalarLoop &loop, lxScalarArray_t &sOut)
{
  T *pOut = (T*)sOut.data.tvoid;

  switch(op){
  case LUX_SCALAR_OP0_CLEAR:
    do{
      if (loop.stride == loop.vectordim){
        size_t rowsize = loop.cntvec * loop.vectordim;
        memset(pOut,0,sizeof(T) * rowsize);
        pOut += rowsize;
      }
      else{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
        pOut[idx] = 0;

      SCALAR_VECTOR_LOOP((pOut += loop.stride),SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
      }
      loop.checkUnrolled((void*&)pOut);
    } while( loop.cnt );
    break;
  default:
    return LUX_TRUE;
  }
  return LUX_FALSE;
}


// OP1

template <class T, class TScalarLoop1>
booln LUX_FASTCALL TScalarArrayOp_in1(lxScalarArrayOp_t op, TScalarLoop1 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0)
{
  T *pOut = (T*)sOut.data.tvoid;
  const T *pArg0  = (const T*)sArg0.data.tvoid;

#define SCALAR_CURRENT_PTRS \
  (pOut += loop.stride, pArg0 += loop.stride0)

  if (sArg0.vectordim == 1){
    switch(op){
    case LUX_SCALAR_OP1_COPY:
      do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[0];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0);
      } while( loop.cnt );
      return LUX_FALSE;
    default:
      return LUX_TRUE;
    }
  }
  else{
    switch(op){
    case LUX_SCALAR_OP1_COPY:
      do{
        if (loop.stride == loop.vectordim && loop.stride0 == loop.vectordim){
          size_t rowsize = loop.cntvec * loop.vectordim;
          memcpy(pOut,pArg0,sizeof(T) * rowsize);
          pOut  += rowsize;
          pArg0 += rowsize;
        }
        else{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
          pOut[idx] = pArg0[idx];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
        }
        loop.checkUnrolled((void*&)pOut,(const void*&)pArg0);
      } while( loop.cnt );
      return LUX_FALSE;
    default:
      return LUX_TRUE;
    }
  }
  return LUX_FALSE;
#undef SCALAR_CURRENT_PTRS
}

// OP2

#ifdef SCALAR_USE_XMM

#define XMMLOOP_START while(run){

#define XMMLOOP_END   loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);\
            run = loop.cnt && (loop.vectordim == 4);}

#define XMMLOOP_FOR   for (size_t i = 0; i < loop.cntvec; i++, pOut+= loop.stride, pArg0 += loop.stride0, pArg1 += loop.stride1)

template<class TScalarLoop2>
booln LUX_FASTCALL XMMScalarArrayOp_in2_v44(lxScalarArrayOp_t op, TScalarLoop2 &loop, void *&pOutV,const void *&pArg0V,const void *&pArg1V)
{

  float *&pOut = (float*&)pOutV;
  const float *&pArg0  = (const float*&)pArg0V;
  const float *&pArg1  = (const float*&)pArg1V;
  booln run = LUX_TRUE;

  __m128 min = _mm_set_ps(0.0f,0.0f,0.0f,0.0f);
  __m128 max = _mm_set_ps(1.0f,1.0f,1.0f,1.0f);

  // Warning, if pointers are not 16-byte aligned we are DOOMED!!!
  switch(op){
  case LUX_SCALAR_OP2_ADD:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_add_ps(*(__m128*)pArg0,*(__m128*)pArg1);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_SUB:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_sub_ps(*(__m128*)pArg0,*(__m128*)pArg1);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MUL:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      //__m128 xmm0 = _mm_load_ps(pArg0);
      //__m128 xmm1 = _mm_load_ps(pArg1);
      //_mm_store_ps(pOut,_mm_mul_ps(_mm_load_ps(pArg0),_mm_load_ps(pArg1))); 
      *((__m128*)pOut) = _mm_mul_ps(*(__m128*)pArg0,*(__m128*)pArg1);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_DIV:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_div_ps(*(__m128*)pArg0,*(__m128*)pArg1);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MIN:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_min_ps(*(__m128*)pArg0,*(__m128*)pArg1);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MAX:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_max_ps(*(__m128*)pArg0,*(__m128*)pArg1);
    }
    XMMLOOP_END
    break;

  case LUX_SCALAR_OP2_ADD_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_add_ps(*(__m128*)pArg0,*(__m128*)pArg1),max),min);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_SUB_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_sub_ps(*(__m128*)pArg0,*(__m128*)pArg1),max),min);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MUL_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_mul_ps(*(__m128*)pArg0,*(__m128*)pArg1),max),min);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_DIV_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_div_ps(*(__m128*)pArg0,*(__m128*)pArg1),max),min);
    }
    XMMLOOP_END
    break;
  default:
    return LUX_TRUE;
  }
  return LUX_FALSE;
}
template<class TScalarLoop2>
booln LUX_FASTCALL XMMScalarArrayOp_in2_v41(lxScalarArrayOp_t op, TScalarLoop2 &loop, void *&pOutV,const void *&pArg0V,const void *&pArg1V)
{

  float *&pOut = (float*&)pOutV;
  const float *&pArg0  = (const float*&)pArg0V;
  const float *&pArg1  = (const float*&)pArg1V;
  booln run = LUX_TRUE;

  __m128 min = _mm_set_ps(0.0f,0.0f,0.0f,0.0f);
  __m128 max = _mm_set_ps(1.0f,1.0f,1.0f,1.0f);

  // Warning, if pointers are not 16-byte aligned we are DOOMED!!!
  

  switch(op){
  case LUX_SCALAR_OP2_ADD:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_add_ps(*(__m128*)pArg0,xmm0);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_SUB:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_sub_ps(*(__m128*)pArg0,xmm0);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MUL:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      //__m128 xmm0 = _mm_load_ps(pArg0);
      //__m128 xmm1 = _mm_load_ps(pArg1);
      //_mm_store_ps(pOut,_mm_mul_ps(_mm_load_ps(pArg0),_mm_load_ps(pArg1))); 
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_mul_ps(*(__m128*)pArg0,xmm0);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_DIV:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_div_ps(*(__m128*)pArg0,xmm0);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MIN:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_min_ps(*(__m128*)pArg0,xmm0);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MAX:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_max_ps(*(__m128*)pArg0,xmm0);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_ADD_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_add_ps(*(__m128*)pArg0,xmm0),max),min);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_SUB_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_sub_ps(*(__m128*)pArg0,xmm0),max),min);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_MUL_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      //__m128 xmm0 = _mm_load_ps(pArg0);
      //__m128 xmm1 = _mm_load_ps(pArg1);
      //_mm_store_ps(pOut,_mm_mul_ps(_mm_load_ps(pArg0),_mm_load_ps(pArg1))); 
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_mul_ps(*(__m128*)pArg0,xmm0),max),min);
    }
    XMMLOOP_END
    break;
  case LUX_SCALAR_OP2_DIV_SAT:
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg1);
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_div_ps(*(__m128*)pArg0,xmm0),max),min);
    }
    XMMLOOP_END
    break;
  default:
    return LUX_TRUE;
  }


  return LUX_FALSE;
}

#undef  XMMLOOP_END
#define XMMLOOP_END   loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);\
            run = loop.cnt && (loop.vectordim == 4);}

#undef  XMMLOOP_FOR   
#define XMMLOOP_FOR   for (size_t i = 0; i < loop.cntvec; i++, pOut+= loop.stride, pArg0 += loop.stride0, pArg1 += loop.stride1, pArg2 += loop.stride1)

template<class TScalarLoop3>
booln LUX_FASTCALL XMMScalarArrayOp_in3_v44(lxScalarArrayOp_t op, TScalarLoop3 &loop, void *&pOutV,const void *&pArg0V,const void *&pArg1V,const void *&pArg2V)
{

  float *&pOut = (float*&)pOutV;
  const float *&pArg0  = (const float*&)pArg0V;
  const float *&pArg1  = (const float*&)pArg1V;
  const float *&pArg2  = (const float*&)pArg2V;
  booln run = LUX_TRUE;

  __m128 min = _mm_set_ps(0.0f,0.0f,0.0f,0.0f);
  __m128 max = _mm_set_ps(1.0f,1.0f,1.0f,1.0f);

  // Warning, if pointers are not 16-byte aligned we are DOOMED!!!
  switch(op){
  case LUX_SCALAR_OP3_LERP:
    // ((out) = (a) + (((b)-(a))*(t)))
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_add_ps(
                  *(__m128*)pArg0,
                  _mm_mul_ps(
                    _mm_sub_ps(*(__m128*)pArg1,*(__m128*)pArg0),
                    *(__m128*)pArg2));
    }
    XMMLOOP_END
    return LUX_FALSE;
  case LUX_SCALAR_OP3_LERPINV:
    // ((out) = (a) + (((b)-(a))*(1-t)))
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_add_ps(
                *(__m128*)pArg0, 
                _mm_mul_ps(
                  _mm_sub_ps(*(__m128*)pArg1,*(__m128*)pArg0),
                  _mm_sub_ps(max,*(__m128*)pArg2)));
    }
    XMMLOOP_END
    return LUX_FALSE;
  case LUX_SCALAR_OP3_MADD:
    // ((out) = (a + b*c)
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_add_ps(*(__m128*)pArg0,_mm_mul_ps(*(__m128*)pArg1,*(__m128*)pArg2));
    }
    XMMLOOP_END
    return LUX_FALSE;
  case LUX_SCALAR_OP3_MADD_SAT:
    // ((out) = (a + b*c)
    XMMLOOP_START
    XMMLOOP_FOR
    {
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_add_ps(*(__m128*)pArg0,_mm_mul_ps(*(__m128*)pArg1,*(__m128*)pArg2)),max),min);
    }
    XMMLOOP_END
    return LUX_FALSE;
  default:
    return LUX_TRUE;
  }

  return LUX_FALSE;
}

template<class TScalarLoop3>
booln LUX_FASTCALL XMMScalarArrayOp_in3_v41(lxScalarArrayOp_t op, TScalarLoop3 &loop, void *&pOutV,const void *&pArg0V,const void *&pArg1V,const void *&pArg2V)
{

  float *&pOut = (float*&)pOutV;
  const float *&pArg0  = (const float*&)pArg0V;
  const float *&pArg1  = (const float*&)pArg1V;
  const float *&pArg2  = (const float*&)pArg2V;
  booln run = LUX_TRUE;

  __m128 min = _mm_set_ps(0.0f,0.0f,0.0f,0.0f);
  __m128 max = _mm_set_ps(1.0f,1.0f,1.0f,1.0f);

  // Warning, if pointers are not 16-byte aligned we are DOOMED!!!
  switch(op){
  case LUX_SCALAR_OP3_LERP:
    // ((out) = (a) + (((b)-(a))*(t)))
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg2);
      *((__m128*)pOut) = _mm_add_ps(
                *(__m128*)pArg0,
                _mm_mul_ps(
                  _mm_sub_ps(*(__m128*)pArg1,*(__m128*)pArg0),
                  xmm0));
    }
    XMMLOOP_END
    return LUX_FALSE;
  case LUX_SCALAR_OP3_LERPINV:
    // ((out) = (a) + (((b)-(a))*(1-t)))
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg2);
      *((__m128*)pOut) = _mm_add_ps(
                  *(__m128*)pArg0, 
                  _mm_mul_ps(
                    _mm_sub_ps(*(__m128*)pArg1,*(__m128*)pArg0),
                    _mm_sub_ps(max,xmm0)));
    }
    XMMLOOP_END
    return LUX_FALSE;
  case LUX_SCALAR_OP3_MADD:
    // ((out) = (a + b*c)
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg2);
      *((__m128*)pOut) = _mm_add_ps(*(__m128*)pArg0,_mm_mul_ps(*(__m128*)pArg1,xmm0));
    }
    XMMLOOP_END
    return LUX_FALSE;
  case LUX_SCALAR_OP3_MADD_SAT:
    // ((out) = (a + b*c)
    XMMLOOP_START
    XMMLOOP_FOR
    {
      __m128 xmm0 = _mm_load_ps1(pArg2);
      *((__m128*)pOut) = _mm_max_ps(_mm_min_ps(_mm_add_ps(*(__m128*)pArg0,_mm_mul_ps(*(__m128*)pArg1,xmm0)),max),min);
    }
    XMMLOOP_END
    return LUX_FALSE;
  default:
    return LUX_TRUE;
  }

  return LUX_FALSE;
}

#undef XMMLOOP_START
#undef XMMLOOP_END
#undef XMMLOOP_FOR

#endif



template <class T, class TScalarLoop2, class Ttemp, int ISFLOAT>
booln LUX_FASTCALL TScalarArrayOp_in2(lxScalarArrayOp_t op, TScalarLoop2 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0, const lxScalarArray_t &sArg1)
{
  T *pOut = (T*)sOut.data.tvoid;
  const T *pArg0  = (const T*)sArg0.data.tvoid;
  const T *pArg1  = (const T*)sArg1.data.tvoid;

  booln single = sArg1.vectordim == 1;
  if (sArg0.vectordim != sArg1.vectordim){
    if (!single)
      return LUX_FALSE;
  }

#ifdef SCALAR_USE_XMM
  if (ISFLOAT){
    // check if we can perform the first pass
    // using SSE 
    if (loop.vectordim == 4 && LUX_IS_ALIGNED(pOut,16) && LUX_IS_ALIGNED(pArg0,16) && LUX_IS_ALIGNED(pArg1,16) 
      && loop.stride % 4 == 0 && loop.stride0 % 4 == 0 && loop.stride1 % 4 == 0)
    {
      booln err = (single) ? 
        XMMScalarArrayOp_in2_v41<TScalarLoop2>(op,loop,(void*&)pOut,(const void*&)pArg0,(const void*&)pArg1) : 
        XMMScalarArrayOp_in2_v44<TScalarLoop2>(op,loop,(void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);

      if (!loop.cnt)
        return err;
    }
  }
#endif

  Ttemp min = (Ttemp)l_ScalarMin[sOut.type];
  Ttemp max = (Ttemp)l_ScalarMax[sOut.type];

#define SCALAR_CURRENT_PTRS \
  (pOut += loop.stride, pArg0 += loop.stride0, pArg1 += loop.stride1)

  if (single){

    switch(op){
    case LUX_SCALAR_OP2_ADD:
      do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[idx] +  pArg1[0];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_SUB:
      do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[idx] -  pArg1[0];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MUL:
      do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[idx] *  pArg1[0];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_DIV:
      do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[idx] /  pArg1[0];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MIN:
      do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = LUX_MIN(pArg0[idx],pArg1[0]);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MAX:
      do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = LUX_MAX(pArg0[idx],pArg1[0])

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;


    case LUX_SCALAR_OP2_ADD_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] +  pArg1[0];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_SUB_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] -  pArg1[0];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MUL_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] *  pArg1[0];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_DIV_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] /  pArg1[0];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;


    }

  }
  else{
    switch(op){
    case LUX_SCALAR_OP2_ADD:
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = pArg0[idx] +  pArg1[idx];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_SUB:
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = pArg0[idx] -  pArg1[idx];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MUL:
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = pArg0[idx] *  pArg1[idx];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_DIV:
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = pArg0[idx] /  pArg1[idx];

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MIN:
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = LUX_MIN(pArg0[idx],pArg1[idx]);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MAX:
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = LUX_MAX(pArg0[idx],pArg1[idx])

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;


    case LUX_SCALAR_OP2_ADD_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] +  pArg1[idx];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_SUB_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] -  pArg1[idx];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_MUL_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] *  pArg1[idx];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    case LUX_SCALAR_OP2_DIV_SAT:
      do{
#define   SCALAR_CURRENT_INIT \
  Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] /  pArg1[idx];\
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1);
      } while( loop.cnt );
      return LUX_FALSE;

    }
  }
  return LUX_FALSE;
#undef SCALAR_CURRENT_PTRS
}


// OP3

template <class T, class TScalarLoop3, class Ttemp>
booln LUX_FASTCALL TScalarArrayOp_in3(lxScalarArrayOp_t op, TScalarLoop3 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0, const lxScalarArray_t &sArg1,  const lxScalarArray_t &sArg2)
{

  T *pOut = (T*)sOut.data.tvoid;
  const T *pArg0  = (const T*)sArg0.data.tvoid;
  const T *pArg1  = (const T*)sArg1.data.tvoid;
  const T *pArg2  = (const T*)sArg2.data.tvoid;

  Ttemp min = (Ttemp)l_ScalarMin[sOut.type];
  Ttemp max = (Ttemp)l_ScalarMax[sOut.type];

  booln single = sArg2.vectordim == 1;
  if (sArg0.vectordim != sArg2.vectordim){
    if (!single)
      return LUX_FALSE;
  }

#define SCALAR_CURRENT_PTRS \
  (pOut += loop.stride, pArg0 += loop.stride0, pArg1 += loop.stride1, pArg2+= loop.stride2)

  if (single){
    switch(op){
    case LUX_SCALAR_OP3_LERP:
      {
      // ((out) = (a) + (((b)-(a))*(t)))
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
          pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * pArg2[0]) / max);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
      } while( loop.cnt );
      return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_LERPINV:
      {
      // ((out) = (a) + (((b)-(a))*(t)))
      do{
  #define   SCALAR_CURRENT_INIT   \
          T lrp = max-pArg2[0];
  #define   SCALAR_CURRENT_OP( idx ) \
          pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * lrp) / max);

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
          loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
      } while( loop.cnt );
      return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_MADD:
      {
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
          pOut[idx] = pArg0[idx] +  (pArg1[idx]  * pArg2[0]);

            SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
      } while( loop.cnt );
      return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_MADD_SAT:
      {
      do{
#define   SCALAR_CURRENT_INIT
        Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] +  (pArg1[idx]  * pArg2[0]); \
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
      return LUX_FALSE;
      }
    default:
      return LUX_TRUE;
    }
  }
  else{
    switch(op){
    case LUX_SCALAR_OP3_LERP:
      {
        // ((out) = (a) + (((b)-(a))*(t)))
        do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * pArg2[idx]) / max);

            SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_LERPINV:
      {
        // ((out) = (a) + (((b)-(a))*(t)))
        do{
  #define   SCALAR_CURRENT_INIT   \
    T lrp;
  #define   SCALAR_CURRENT_OP( idx ) \
    lrp = max-pArg2[idx]; \
    pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * lrp) / max);

            SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_MADD:
      {
        do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[idx] +  (pArg1[idx]  * pArg2[idx]);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_MADD_SAT:
      {
        do{
#define   SCALAR_CURRENT_INIT
          Ttemp temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] +  (pArg1[idx]  * pArg2[idx]); \
  pOut[idx] = (T)LUX_CLAMP(temp,min,max);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    default:
      return LUX_TRUE;
    }
  }
  return LUX_FALSE;

#undef SCALAR_CURRENT_PTRS
}

template <class TScalarLoop3>
booln LUX_FASTCALL FScalarArrayOp_in3(lxScalarArrayOp_t op, TScalarLoop3 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0, const lxScalarArray_t &sArg1,  const lxScalarArray_t &sArg2)
{
  float *pOut = (float*)sOut.data.tvoid;
  const float *pArg0  = (const float*)sArg0.data.tvoid;
  const float *pArg1  = (const float*)sArg1.data.tvoid;
  const float *pArg2  = (const float*)sArg2.data.tvoid;

  booln single = sArg2.vectordim == 1;
  if (sArg0.vectordim != sArg2.vectordim){
    if (!single)
      return LUX_FALSE;
  }

  // TODO SSE CHECK
#ifdef SCALAR_USE_XMM
  // check if we can perform the first pass
  // using SSE
  if (loop.vectordim == 4 && LUX_IS_ALIGNED(pOut,16) && LUX_IS_ALIGNED(pArg0,16) && LUX_IS_ALIGNED(pArg1,16) && LUX_IS_ALIGNED(pArg2,16)
    && loop.stride % 4 == 0 && loop.stride0 % 4 == 0 && loop.stride1 % 4 == 0 && loop.stride2 % 4 == 0)
  {
    booln err = (single) ? 
      XMMScalarArrayOp_in3_v41<TScalarLoop3>(op,loop,(void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2) : 
      XMMScalarArrayOp_in3_v44<TScalarLoop3>(op,loop,(void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);

    if (!loop.cnt)
      return err;
  }
#endif

#define SCALAR_CURRENT_PTRS \
  (pOut += loop.stride, pArg0 += loop.stride0, pArg1 += loop.stride1, pArg2+= loop.stride2)

  if (single){
    switch(op){
    case LUX_SCALAR_OP3_LERP:
      {
      do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
            pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * pArg2[0]));

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
      } while( loop.cnt );
      }
      return LUX_FALSE;

    case LUX_SCALAR_OP3_LERPINV:
      {
        do{
  #define   SCALAR_CURRENT_INIT \
        float lrp = 1.0f-pArg2[0];
  #define   SCALAR_CURRENT_OP( idx ) \
          pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * lrp));

        SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
      }
      return LUX_FALSE;
    case LUX_SCALAR_OP3_MADD:
      {
        do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[idx] +  (pArg1[idx]  * pArg2[0]);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_MADD_SAT:
      {
        do{
#define   SCALAR_CURRENT_INIT
          float temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] +  (pArg1[idx]  * pArg2[0]); \
  pOut[idx] = LUX_CLAMP(temp,0.0f,1.0f);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    default:
      return LUX_TRUE;
    }
  }
  else {
    switch(op){
    case LUX_SCALAR_OP3_LERP:
      {
        do{
  #define   SCALAR_CURRENT_INIT
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * pArg2[idx]));

            SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
      }
      return LUX_FALSE;

    case LUX_SCALAR_OP3_LERPINV:
      {
        do{
  #define   SCALAR_CURRENT_INIT 
  #define   SCALAR_CURRENT_OP( idx ) \
    pOut[idx] = pArg0[idx] +  (((pArg1[idx] - pArg0[idx]) * (1.0f-pArg2[idx])));

            SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
  #undef    SCALAR_CURRENT_OP
  #undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
      }
      return LUX_FALSE;

    case LUX_SCALAR_OP3_MADD:
      {
        do{
#define   SCALAR_CURRENT_INIT
#define   SCALAR_CURRENT_OP( idx ) \
  pOut[idx] = pArg0[idx] +  (pArg1[idx]  * pArg2[idx]);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    case LUX_SCALAR_OP3_MADD_SAT:
      {
        do{
#define   SCALAR_CURRENT_INIT
          float temp;
#define   SCALAR_CURRENT_OP( idx ) \
  temp = pArg0[idx] +  (pArg1[idx]  * pArg2[idx]); \
  pOut[idx] = LUX_CLAMP(temp,0.0f,1.0f);

          SCALAR_VECTOR_LOOP(SCALAR_CURRENT_PTRS,SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef    SCALAR_CURRENT_OP
#undef    SCALAR_CURRENT_INIT
            loop.checkUnrolled((void*&)pOut,(const void*&)pArg0,(const void*&)pArg1,(const void*&)pArg2);
        } while( loop.cnt );
        return LUX_FALSE;
      }
    default:
      return LUX_TRUE;
    }
  }
  return LUX_FALSE;

#undef SCALAR_CURRENT_PTRS
}

//////////////////////////////////////////////////////////////////////////

typedef booln (LUX_FASTCALL TScalarArrayOp_in0_fn)(lxScalarArrayOp_t op, ScalarLoop &loop, lxScalarArray_t &sRet);

static TScalarArrayOp_in0_fn* l_TOp0[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArrayOp_in0<float,ScalarLoop>,
  TScalarArrayOp_in0<int8,ScalarLoop>,
  TScalarArrayOp_in0<uint8,ScalarLoop>,
  TScalarArrayOp_in0<int16,ScalarLoop>,
  TScalarArrayOp_in0<uint16,ScalarLoop>,
  TScalarArrayOp_in0<int32,ScalarLoop>,
  TScalarArrayOp_in0<uint32,ScalarLoop>,
};

LUX_API booln lxScalarArray_Op0(lxScalarArray_t *ret, lxScalarArrayOp_t op)
{
  ScalarLoop loop(*ret);

  return l_TOp0[ret->type](op,loop,*ret);
}

typedef booln (LUX_FASTCALL TScalarArrayOp_in1_fn)(lxScalarArrayOp_t op, ScalarLoop1 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sIn);

static TScalarArrayOp_in1_fn* l_TOp1[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArrayOp_in1<float,ScalarLoop1>,
  TScalarArrayOp_in1<int8,ScalarLoop1>,
  TScalarArrayOp_in1<uint8,ScalarLoop1>,
  TScalarArrayOp_in1<int16,ScalarLoop1>,
  TScalarArrayOp_in1<uint16,ScalarLoop1>,
  TScalarArrayOp_in1<int32,ScalarLoop1>,
  TScalarArrayOp_in1<uint32,ScalarLoop1>,
};


LUX_API booln lxScalarArray_Op1(lxScalarArray_t *ret, lxScalarArrayOp_t op, 
              const lxScalarArray_t *arg0)
{
  ScalarLoop1 loop(*ret,*arg0);
  LUX_ASSERT( ret->type == arg0->type);

  return l_TOp1[ret->type](op,loop,*ret,*arg0);
}


typedef booln (LUX_FASTCALL TScalarArrayOp_in2_fn)(lxScalarArrayOp_t op, ScalarLoop2 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0, const lxScalarArray_t &sArg1);

static TScalarArrayOp_in2_fn* l_TOp2[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArrayOp_in2<float,ScalarLoop2, float,1>,
  TScalarArrayOp_in2<int8,ScalarLoop2,  int32,0>,
  TScalarArrayOp_in2<uint8,ScalarLoop2, int32,0>,
  TScalarArrayOp_in2<int16,ScalarLoop2, int32,0>,
  TScalarArrayOp_in2<uint16,ScalarLoop2,int32,0>,
  TScalarArrayOp_in2<int32,ScalarLoop2, int32,0>,
  TScalarArrayOp_in2<uint32,ScalarLoop2,int32,0>,
};


LUX_API booln lxScalarArray_Op2(lxScalarArray_t *ret, lxScalarArrayOp_t op,
  const lxScalarArray_t *arg0,const lxScalarArray_t *arg1)
{
  ScalarLoop2 loop(*ret,*arg0,*arg1);
  LUX_ASSERT( ret->type == arg0->type &&
        ret->type == arg1->type);

  return l_TOp2[ret->type](op,loop,*ret,*arg0,*arg1);
}

typedef booln (LUX_FASTCALL TScalarArrayOp_in3_fn)(lxScalarArrayOp_t op, ScalarLoop3 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0, const lxScalarArray_t &sArg1, const lxScalarArray_t &sArg2);

static TScalarArrayOp_in3_fn* l_TOp3[LUX_SCALAROPS_MAX_SUPPORTED] = {
  FScalarArrayOp_in3<ScalarLoop3>,
  TScalarArrayOp_in3<int8,ScalarLoop3,  int32>,
  TScalarArrayOp_in3<uint8,ScalarLoop3, int32>,
  TScalarArrayOp_in3<int16,ScalarLoop3, int32>,
  TScalarArrayOp_in3<uint16,ScalarLoop3,  int32>,
  TScalarArrayOp_in3<int32,ScalarLoop3, int32>,
  TScalarArrayOp_in3<uint32,ScalarLoop3,  int32>,
};


LUX_API booln lxScalarArray_Op3(lxScalarArray_t *ret, lxScalarArrayOp_t op, 
  const lxScalarArray_t *arg0,  const lxScalarArray_t *arg1,  const lxScalarArray_t *arg2)
{
  ScalarLoop3 loop(*ret,*arg0,*arg1,*arg2);

  LUX_ASSERT( ret->type == arg0->type &&
        ret->type == arg1->type &&
        ret->type == arg2->type);
  
  return l_TOp3[ret->type](op,loop,*ret,*arg0,*arg1,*arg2);
}


//////////////////////////////////////////////////////////////////////////
// MultiDim Operations



typedef booln (LUX_FASTCALL TScalarArrayOp3D_in0_fn)(lxScalarArrayOp_t op, Scalar3DLoop &loop, lxScalarArray_t &sRet);

static TScalarArrayOp3D_in0_fn* l_T3DOp0[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArrayOp_in0<float,Scalar3DLoop>,
  TScalarArrayOp_in0<int8,Scalar3DLoop>,
  TScalarArrayOp_in0<uint8,Scalar3DLoop>,
  TScalarArrayOp_in0<int16,Scalar3DLoop>,
  TScalarArrayOp_in0<uint16,Scalar3DLoop>,
  TScalarArrayOp_in0<int32,Scalar3DLoop>,
  TScalarArrayOp_in0<uint32,Scalar3DLoop>,
};

LUX_API booln lxScalarArray3D_Op0(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op)
{
  Scalar3DLoop loop(*ret,region);
  if (!loop.valid) 
    return LUX_TRUE;

  return l_T3DOp0[ret->sarr.type](op,loop,ret->sarr);
}

typedef booln (LUX_FASTCALL TScalarArrayOp3D_in1_fn)(lxScalarArrayOp_t op, Scalar3DLoop1 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sIn);

static TScalarArrayOp3D_in1_fn* l_T3DOp1[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArrayOp_in1<float,Scalar3DLoop1>,
  TScalarArrayOp_in1<int8,Scalar3DLoop1>,
  TScalarArrayOp_in1<uint8,Scalar3DLoop1>,
  TScalarArrayOp_in1<int16,Scalar3DLoop1>,
  TScalarArrayOp_in1<uint16,Scalar3DLoop1>,
  TScalarArrayOp_in1<int32,Scalar3DLoop1>,
  TScalarArrayOp_in1<uint32,Scalar3DLoop1>,
};


LUX_API booln lxScalarArray3D_Op1(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op,
            const lxScalarArray3D_t *arg0)
{
  Scalar3DLoop1 loop(*ret,region,*arg0);
  if (!loop.valid) 
    return LUX_TRUE;
  LUX_ASSERT( ret->sarr.type == arg0->sarr.type);

  return l_T3DOp1[ret->sarr.type](op,loop,ret->sarr,arg0->sarr);
}


typedef booln (LUX_FASTCALL TScalarArrayOp3D_in2_fn)(lxScalarArrayOp_t op, Scalar3DLoop2 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0, const lxScalarArray_t &sArg1);

static TScalarArrayOp3D_in2_fn* l_T3DOp2[LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArrayOp_in2<float,Scalar3DLoop2, float,1>,
  TScalarArrayOp_in2<int8,Scalar3DLoop2,  int32,0>,
  TScalarArrayOp_in2<uint8,Scalar3DLoop2, int32,0>,
  TScalarArrayOp_in2<int16,Scalar3DLoop2, int32,0>,
  TScalarArrayOp_in2<uint16,Scalar3DLoop2,int32,0>,
  TScalarArrayOp_in2<int32,Scalar3DLoop2, int32,0>,
  TScalarArrayOp_in2<uint32,Scalar3DLoop2,int32,0>,
};


LUX_API booln lxScalarArray3D_Op2(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op,
            const lxScalarArray3D_t *arg0,  const lxScalarArray3D_t *arg1)
{
  Scalar3DLoop2 loop(*ret,region,*arg0,*arg1);
  if (!loop.valid) 
    return LUX_TRUE;

  LUX_ASSERT( ret->sarr.type == arg0->sarr.type &&
        ret->sarr.type == arg1->sarr.type);

  return l_T3DOp2[ret->sarr.type](op,loop,ret->sarr,arg0->sarr,arg1->sarr);
}

typedef booln (LUX_FASTCALL TScalarArrayOp3D_in3_fn)(lxScalarArrayOp_t op, Scalar3DLoop3 &loop, lxScalarArray_t &sOut, const lxScalarArray_t &sArg0, const lxScalarArray_t &sArg1, const lxScalarArray_t &sArg2);

static TScalarArrayOp3D_in3_fn* l_T3DOp3[LUX_SCALAROPS_MAX_SUPPORTED] = {
  FScalarArrayOp_in3<Scalar3DLoop3>,
  TScalarArrayOp_in3<int8,Scalar3DLoop3,  int32>,
  TScalarArrayOp_in3<uint8,Scalar3DLoop3, int32>,
  TScalarArrayOp_in3<int16,Scalar3DLoop3, int32>,
  TScalarArrayOp_in3<uint16,Scalar3DLoop3,int32>,
  TScalarArrayOp_in3<int32,Scalar3DLoop3, int32>,
  TScalarArrayOp_in3<uint32,Scalar3DLoop3,int32>,
};


LUX_API booln lxScalarArray3D_Op3(lxScalarArray3D_t *ret, uint region[3], lxScalarArrayOp_t op, 
            const lxScalarArray3D_t *arg0,  const lxScalarArray3D_t *arg1,  const lxScalarArray3D_t *arg2)
{
  Scalar3DLoop3 loop(*ret,region,*arg0,*arg1,*arg2);
  if (!loop.valid) 
    return LUX_TRUE;

  LUX_ASSERT( ret->sarr.type == arg0->sarr.type &&
        ret->sarr.type == arg1->sarr.type &&
        ret->sarr.type == arg2->sarr.type);

  return l_T3DOp3[ret->sarr.type](op,loop,ret->sarr,arg0->sarr,arg1->sarr,arg2->sarr);
}



//////////////////////////////////////////////////////////////////////////
// Conversion

typedef void (LUX_FASTCALL TScalarArray_convert_fn)(lxScalarArray_t &sOut, const lxScalarArray_t &sIn);

template <class Tout, class Tin>
void LUX_FASTCALL TScalarArray_convert(lxScalarArray_t &sOut, const lxScalarArray_t &sArg0)
{
  ScalarLoop1 loop(sOut,sArg0);

  Tout * LUX_RESTRICT pOut = (Tout*)sOut.data.tvoid;
  const Tin  * LUX_RESTRICT pArg0  = (const Tin*)sArg0.data.tvoid;
  

  do{
    switch(loop.vectordim)
    {
    case 4:
    {
      for (size_t i = 0; i < loop.cntvec; i ++, pOut += loop.stride0, pArg0 += loop.stride0){
        pOut[0] = (Tout)pArg0[0];
        pOut[1] = (Tout)pArg0[1];
        pOut[2] = (Tout)pArg0[2];
        pOut[3] = (Tout)pArg0[3];
      }
    }
      break;
    case 3:
    {
      for (size_t i = 0; i < loop.cntvec; i ++, pOut += loop.stride0, pArg0 += loop.stride0){
        pOut[0] = (Tout)pArg0[0];
        pOut[1] = (Tout)pArg0[1];
        pOut[2] = (Tout)pArg0[2];
      }
    }
      break;
    case 2:
    {
      for (size_t i = 0; i < loop.cntvec; i ++, pOut += loop.stride0, pArg0 += loop.stride0){
        pOut[0] = (Tout)pArg0[0];
        pOut[1] = (Tout)pArg0[1];
      }
    }
      break;
    case 1:
    {
      for (size_t i = 0; i < loop.cntvec; i ++, pOut += loop.stride0, pArg0 += loop.stride0){
        pOut[0] = (Tout)pArg0[0];
      }
    }
      break;
    default:
      LUX_ASSERT("never get here" == NULL);
      break;
    }
    loop.checkUnrolled((void*&)pOut,(const void*&)pArg0);
  } while( loop.cnt );

}


typedef void (LUX_FASTCALL TScalarArray_convertRanged_fn)(
         lxScalarArray_t &sOut, const void *outminmax,
         const lxScalarArray_t &sIn, const void *inminmax);

template <class Tout, class Tin, class Tinter>
void LUX_FASTCALL TScalarArray_convertRanged(lxScalarArray_t &sOut,const void *outminmax, 
                const lxScalarArray_t &sArg0, const void *inminmax)
{
  ScalarLoop1 loop(sOut,sArg0);

  Tout *  LUX_RESTRICT pOut = (Tout*)sOut.data.tvoid;
  const Tin * LUX_RESTRICT pArg0  = (const Tin*)sArg0.data.tvoid;

  const Tout *poutminmax = reinterpret_cast<const Tout*>(outminmax);
  const Tin  *pinminmax  = reinterpret_cast<const Tin*>(inminmax);

  Tinter inmin = (Tinter)(pinminmax[0]);
  Tinter inmax = (Tinter)(pinminmax[1]);
  Tinter inwidth = inmax-inmin;

  Tinter outmin = (Tinter)(poutminmax[0]);
  Tinter outmax = (Tinter)(poutminmax[1]);
  Tinter outwidth = outmax-outmin;


  do{
#define SCALAR_CURRENT_INIT \
      Tinter src;
#define SCALAR_CURRENT_OP( idx )  \
      src = (Tinter)pArg0[ idx ]; \
      src = LUX_CLAMP(src, inmin, inmax);  \
      src = (((src - inmin) * outwidth) / inwidth) + outmin;  \
      pOut[idx ] = (Tout)src; 
 
    SCALAR_VECTOR_LOOP((pOut += loop.stride, pArg0 += loop.stride0),SCALAR_CURRENT_INIT, SCALAR_CURRENT_OP)
#undef  SCALAR_CURRENT_OP
#undef  SCALAR_CURRENT_INIT

    loop.checkUnrolled((void*&)pOut,(const void*&)pArg0);

  } while( loop.cnt );
}

/*
SCALAR_FLOAT32,
SCALAR_INT8,
SCALAR_UINT8,
SCALAR_INT16,
SCALAR_UINT16,
SCALAR_INT32,
SCALAR_UINT32,
*/

static TScalarArray_convert_fn* l_TConv[LUX_SCALAROPS_MAX_SUPPORTED*LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArray_convert<float,float>,
  TScalarArray_convert<float,int8>,
  TScalarArray_convert<float,uint8>,
  TScalarArray_convert<float,int16>,
  TScalarArray_convert<float,uint16>,
  TScalarArray_convert<float,int32>,
  TScalarArray_convert<float,uint32>,

  TScalarArray_convert<int8,float>,
  TScalarArray_convert<int8,int8>,
  TScalarArray_convert<int8,uint8>,
  TScalarArray_convert<int8,int16>,
  TScalarArray_convert<int8,uint16>,
  TScalarArray_convert<int8,int32>,
  TScalarArray_convert<int8,uint32>,

  TScalarArray_convert<uint8,float>,
  TScalarArray_convert<uint8,int8>,
  TScalarArray_convert<uint8,uint8>,
  TScalarArray_convert<uint8,int16>,
  TScalarArray_convert<uint8,uint16>,
  TScalarArray_convert<uint8,int32>,
  TScalarArray_convert<uint8,uint32>,

  TScalarArray_convert<int16,float>,
  TScalarArray_convert<int16,int8>,
  TScalarArray_convert<int16,uint8>,
  TScalarArray_convert<int16,int16>,
  TScalarArray_convert<int16,uint16>,
  TScalarArray_convert<int16,int32>,
  TScalarArray_convert<int16,uint32>,

  TScalarArray_convert<uint16,float>,
  TScalarArray_convert<uint16,int8>,
  TScalarArray_convert<uint16,uint8>,
  TScalarArray_convert<uint16,int16>,
  TScalarArray_convert<uint16,uint16>,
  TScalarArray_convert<uint16,int32>,
  TScalarArray_convert<uint16,uint32>,

  TScalarArray_convert<int32,float>,
  TScalarArray_convert<int32,int8>,
  TScalarArray_convert<int32,uint8>,
  TScalarArray_convert<int32,int16>,
  TScalarArray_convert<int32,uint16>,
  TScalarArray_convert<int32,int32>,
  TScalarArray_convert<int32,uint32>,

  TScalarArray_convert<uint32,float>,
  TScalarArray_convert<uint32,int8>,
  TScalarArray_convert<uint32,uint8>,
  TScalarArray_convert<uint32,int16>,
  TScalarArray_convert<uint32,uint16>,
  TScalarArray_convert<uint32,int32>,
  TScalarArray_convert<uint32,uint32>,
};

LUX_API booln lxScalarArray_convert(lxScalarArray_t *sarrayOut, const lxScalarArray_t *sarrayIn){
  if (sarrayIn->vectordim != sarrayOut->vectordim)
    return LUX_TRUE;

  LUX_ASSERT(sarrayIn->stride);
  LUX_ASSERT(sarrayOut->stride);
  LUX_ASSERT(sarrayIn->vectordim > 0 && sarrayIn->vectordim < 5);

  l_TConv[(sarrayOut->type*LUX_SCALAROPS_MAX_SUPPORTED) + sarrayIn->type](*sarrayOut,*sarrayIn);

  return LUX_FALSE;
}

static TScalarArray_convertRanged_fn* l_TConvRanged[LUX_SCALAROPS_MAX_SUPPORTED*LUX_SCALAROPS_MAX_SUPPORTED] = {
  TScalarArray_convertRanged<float,float,float>,
  TScalarArray_convertRanged<float,int8,float>,
  TScalarArray_convertRanged<float,uint8,float>,
  TScalarArray_convertRanged<float,int16,float>,
  TScalarArray_convertRanged<float,uint16,float>,
  TScalarArray_convertRanged<float,int32,double>,
  TScalarArray_convertRanged<float,uint32,double>,

  TScalarArray_convertRanged<int8,float,float>,
  TScalarArray_convertRanged<int8,int8,float>,
  TScalarArray_convertRanged<int8,uint8,float>,
  TScalarArray_convertRanged<int8,int16,float>,
  TScalarArray_convertRanged<int8,uint16,float>,
  TScalarArray_convertRanged<int8,int32,double>,
  TScalarArray_convertRanged<int8,uint32,double>,

  TScalarArray_convertRanged<uint8,float,float>,
  TScalarArray_convertRanged<uint8,int8,float>,
  TScalarArray_convertRanged<uint8,uint8,float>,
  TScalarArray_convertRanged<uint8,int16,float>,
  TScalarArray_convertRanged<uint8,uint16,float>,
  TScalarArray_convertRanged<uint8,int32,double>,
  TScalarArray_convertRanged<uint8,uint32,double>,

  TScalarArray_convertRanged<int16,float,float>,
  TScalarArray_convertRanged<int16,int8,float>,
  TScalarArray_convertRanged<int16,uint8,float>,
  TScalarArray_convertRanged<int16,int16,float>,
  TScalarArray_convertRanged<int16,uint16,float>,
  TScalarArray_convertRanged<int16,int32,double>,
  TScalarArray_convertRanged<int16,uint32,double>,

  TScalarArray_convertRanged<uint16,float,float>,
  TScalarArray_convertRanged<uint16,int8,float>,
  TScalarArray_convertRanged<uint16,uint8,float>,
  TScalarArray_convertRanged<uint16,int16,float>,
  TScalarArray_convertRanged<uint16,uint16,float>,
  TScalarArray_convertRanged<uint16,int32,double>,
  TScalarArray_convertRanged<uint16,uint32,double>,

  TScalarArray_convertRanged<int32,float,float>,
  TScalarArray_convertRanged<int32,int8,float>,
  TScalarArray_convertRanged<int32,uint8,float>,
  TScalarArray_convertRanged<int32,int16,float>,
  TScalarArray_convertRanged<int32,uint16,float>,
  TScalarArray_convertRanged<int32,int32,double>,
  TScalarArray_convertRanged<int32,uint32,double>,

  TScalarArray_convertRanged<uint32,float,float>,
  TScalarArray_convertRanged<uint32,int8,float>,
  TScalarArray_convertRanged<uint32,uint8,float>,
  TScalarArray_convertRanged<uint32,int16,float>,
  TScalarArray_convertRanged<uint32,uint16,float>,
  TScalarArray_convertRanged<uint32,int32,double>,
  TScalarArray_convertRanged<uint32,uint32,double>,
};

LUX_API booln lxScalarArray_convertRanged(
    lxScalarArray_t *sarrayOut, const lxScalarVector_t *outminmax, 
    const lxScalarArray_t *sarrayIn,  const lxScalarVector_t *inminmax)
{
  if (sarrayIn->vectordim != sarrayOut->vectordim)
    return LUX_TRUE;

  LUX_ASSERT(sarrayIn->stride);
  LUX_ASSERT(sarrayOut->stride);
  LUX_ASSERT(sarrayIn->vectordim > 0 && sarrayIn->vectordim < 5);

  l_TConvRanged[(sarrayOut->type*LUX_SCALAROPS_MAX_SUPPORTED) + sarrayIn->type](*sarrayOut,(void*)outminmax,*sarrayIn,(void*)inminmax);

  return LUX_FALSE;
}
static float32  l_float32minmaxu[2] = {0.0f,1.0f};
static float32  l_float32minmaxs[2] = {-1.0f,1.0f};
static int8   l_tint8minmax[2] = {-127,127};
static uint8  l_tuint8minmax[2] = {0,255};
static int16  l_tint16minmax[2] = {LUX_SHORT_SIGNEDMAX,LUX_SHORT_SIGNEDMAX};
static uint16 l_tuint16minmax[2] = {0,LUX_SHORT_UNSIGNEDMAX};
static int32  l_tint32minmax[2] = {INT_MIN,INT_MAX};
static uint32 l_tuint32minmax[2] = {0,UINT_MAX};

static void*  l_tminmax[LUX_SCALAROPS_MAX_SUPPORTED*2] = {
  &l_float32minmaxu[0],
  &l_tint8minmax[0],
  &l_tuint8minmax[0],
  &l_tint16minmax[0],
  &l_tuint16minmax[0],
  &l_tint32minmax[0],
  &l_tuint32minmax[0],

  &l_float32minmaxs[0],
  &l_tint8minmax[0],
  &l_tuint8minmax[0],
  &l_tint16minmax[0],
  &l_tuint16minmax[0],
  &l_tint32minmax[0],
  &l_tuint32minmax[0],
};

LUX_API booln lxScalarArray_convertNormalized(
    lxScalarArray_t *sarrayOut, const lxScalarArray_t *sarrayIn)
{
  if (sarrayIn->vectordim != sarrayOut->vectordim)
    return LUX_TRUE;

  LUX_ASSERT(sarrayIn->stride);
  LUX_ASSERT(sarrayOut->stride);
  LUX_ASSERT(sarrayIn->vectordim > 0 && sarrayIn->vectordim < 5);

  void* inminmax  = l_tminmax[sarrayIn->type + (l_ScalarSign[sarrayOut->type]*LUX_SCALAROPS_MAX_SUPPORTED)];
  void* outminmax = l_tminmax[sarrayOut->type + (l_ScalarSign[sarrayIn->type]*LUX_SCALAROPS_MAX_SUPPORTED)];

  l_TConvRanged[(sarrayOut->type*LUX_SCALAROPS_MAX_SUPPORTED) + sarrayIn->type](*sarrayOut,(void*)outminmax,*sarrayIn,(void*)inminmax);

  return LUX_FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Float Interpolated
/*
* Some source regarding sampling from MESA 3D
* Copyright (C) 1999-2007  Brian Paul   All Rights Reserved
* MESA 3D
*/


LUX_API uint lxSampleCubeTo2DCoord(float coordsout[2], const float coords[3])
{
  uint combine;
  lxVector3 coordsf;

  coordsf[0] = fabs((float)coords[0]);
  coordsf[1] = fabs((float)coords[1]);
  coordsf[2] = fabs((float)coords[2]);


  if (coordsf[0] > coordsf[1])
  combine = (coordsf[0] > coordsf[2]) ? 0 : 4;
  else
  combine = (coordsf[1] > coordsf[2]) ? 2 : 4;

  combine += coords[combine] < 0;
  /*
  major axis
  direction     target             sc     tc    ma
  ----------    -------------------------------    ---    ---   ---
  +rx     TEXTURE_CUBE_MAP_POSITIVE_X_ARB    -rz    -ry   rx
  -rx     TEXTURE_CUBE_MAP_NEGATIVE_X_ARB    +rz    -ry   rx
  +ry     TEXTURE_CUBE_MAP_POSITIVE_Y_ARB    +rx    +rz   ry
  -ry     TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB    +rx    -rz   ry
  +rz     TEXTURE_CUBE_MAP_POSITIVE_Z_ARB    +rx    -ry   rz
  -rz     TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB    -rx    -ry   rz

  Using the sc, tc, and ma determined by the major axis direction as
  specified in the table above, an updated (s,t) is calculated as
  follows

  s   = ( sc/|ma| + 1 ) / 2
  t   = ( tc/|ma| + 1 ) / 2
  */
  switch(combine)
  {
    // X
  case 0:
    coordsout[0] = ((-coords[2]/coordsf[0])+1.0f)*0.5f;
    coordsout[1] = ((-coords[1]/coordsf[0])+1.0f)*0.5f;
    break;
  case 1:
    coordsout[0] = ((coords[2]/coordsf[0])+1.0f)*0.5f;
    coordsout[1] = ((-coords[1]/coordsf[0])+1.0f)*0.5f;
    break;
    // Y
  case 2:
    coordsout[0] = ((coords[0]/coordsf[1])+1.0f)*0.5f;
    coordsout[1] = ((coords[2]/coordsf[1])+1.0f)*0.5f;
    break;
  case 3:
    coordsout[0] = ((coords[0]/coordsf[1])+1.0f)*0.5f;
    coordsout[1] = ((-coords[2]/coordsf[1])+1.0f)*0.5f;
    break;
    // Z
  case 4:
    coordsout[0] = ((coords[0]/coordsf[2])+1.0f)*0.5f;
    coordsout[1] = ((-coords[1]/coordsf[2])+1.0f)*0.5f;
    break;
  case 5:
    coordsout[0] = ((-coords[0]/coordsf[2])+1.0f)*0.5f;
    coordsout[1] = ((-coords[1]/coordsf[2])+1.0f)*0.5f;
    break;
  }

  return combine;
}

LUX_INLINE int repeat_remainder(int a, int b)
{
  if (a >= 0)
    return a % b;
  else
    return (a + 1) % b + b - 1;
}


/**
* Used to compute texel locations for linear sampling.
* Input:
*    wrapMode = GL_REPEAT, GL_CLAMP_TO_EDGE
*    S = texcoord in [0,1]
*    SIZE = width (or height or depth) of texture
* Output:
*    U = texcoord in [0, width]
*    I0, I1 = two nearest texel indexes
*/
#define COMPUTE_LINEAR_TEXEL_LOCATIONS(notclamped, S, U, SIZE, I0, I1)  \
  if(notclamped){           \
  U = S * SIZE - 0.5F;            \
  I0 = repeat_remainder(lxFastIntFloor(U), SIZE);     \
  I1 = repeat_remainder(I0 + 1, SIZE);        \
  }\
  else{     \
  if (S <= 0.0F)              \
  U = 0.0F;             \
    else if (S >= 1.0F)           \
    U = (float) SIZE;           \
    else                \
    U = S * SIZE;             \
    U -= 0.5F;              \
    I0 = lxFastIntFloor(U);             \
    I1 = I0 + 1;              \
    if (I0 < 0)             \
    I0 = 0;             \
    if (I1 >= (int) SIZE)           \
    I1 = SIZE - 1;              \
}

#ifdef SCALAR_USE_XMM
booln LUX_FASTCALL XMMScalarArray_sampleLinear(float *outvals, const lxScalarArray_t &sarray, 
                       const uint size[3], const float coords[3], booln notclamped[3])
{
  int coordsi[6];
  lxVector3 coordslerp;
  lxVector3 coordsf;

  int texdim = 1 + (size[1]>1) + (size[2] > 1);
  int runs = 1 + (size[2] > 1);
  


  for (int i = 0; i < 3; i++){
    int ci = i*2;
    int ci2 = ci+1;
    COMPUTE_LINEAR_TEXEL_LOCATIONS(notclamped[i],coords[i],coordsf[i],size[i],coordsi[ci],coordsi[ci2]);
    coordslerp[i] = lxFastFrac(coordsf[i]);
  }

  coordsi[2] *= size[0];
  coordsi[3] *= size[0];

  uint sidesize = size[0]*size[1];
  coordsi[4] *= sidesize;
  coordsi[5] *= sidesize;

  {
    uint stride = sarray.stride/4;

    __m128 * LUX_RESTRICT imageData = (__m128 *)sarray.data.tvoid;

    __m128 colorstore;
    __m128 colorstore2;

    while(runs){
      __m128 ptrs[4];
      __m128 temp;
      __m128 lerpx = lxVector4SSE_setAll(coordslerp[0]);

      // sample first
      ptrs[0] = imageData[(coordsi[0]+(coordsi[2])+(coordsi[4]))*stride];
      ptrs[1] = imageData[(coordsi[1]+(coordsi[2])+(coordsi[4]))*stride];
      ptrs[2] = imageData[(coordsi[0]+(coordsi[3])+(coordsi[4]))*stride];
      ptrs[3] = imageData[(coordsi[1]+(coordsi[3])+(coordsi[4]))*stride];

      colorstore = lxVector4SSE_vlerp(lerpx,ptrs[0],ptrs[1]);
      temp = lxVector4SSE_vlerp(lerpx,ptrs[2],ptrs[3]);
      colorstore = lxVector4SSE_lerp(coordslerp[1],colorstore,temp);

      runs--;
      if (runs == 1){
        colorstore2 = _mm_load_ps(colorstore.m128_f32);
        coordsi[4] = coordsi[5];
      }
    }

    if (size[2] > 1){
      colorstore = lxVector4SSE_lerp(coordslerp[2],colorstore2,colorstore);
    }
    lxVector4Copy(outvals,colorstore.m128_f32);
  }

  return LUX_FALSE;
}
#endif


// sampleLinear
typedef booln (LUX_FASTCALL TScalarArray_sampleLinear_fn)(float *outvals, const lxScalarArray_t &sarray, 
                               const uint size[3], const float coords[3], booln clamped[3]);

template <class T, int VECSIZE>
booln LUX_FASTCALL TScalarArray_sampleLinear(float * LUX_RESTRICT outvals, const lxScalarArray_t &sarray, 
                        const uint size[3], const float coords[3], booln notclamped[3])
{
  int coordsi[6];
  lxVector3 coordslerp;
  lxVector3 coordsf;
  
  int texdim = 1 + (size[1]>1) + (size[2] > 1);
  int runs = 1 + (size[2] > 1);

  
  for (int i = 0; i < 3; i++){
    int ci = i*2;
    int ci2 = ci+1;
    COMPUTE_LINEAR_TEXEL_LOCATIONS(notclamped[i],coords[i],coordsf[i],size[i],coordsi[ci],coordsi[ci2]);
    coordslerp[i] = lxFastFrac(coordsf[i]);
  }

  coordsi[2] *= size[0];
  coordsi[3] *= size[0];

  uint sidesize = size[0]*size[1];
  coordsi[4] *= sidesize;
  coordsi[5] *= sidesize;
  
  {
    uint stride = sarray.stride;
    uint sidesize = size[0]*size[1];

    float colors[4][VECSIZE];
    float colorstore[VECSIZE];
    
    const T * LUX_RESTRICT imageData = (const T *)sarray.data.tvoid;
    float * LUX_RESTRICT colorout = outvals;

    while(runs){
      const T *ptrs[4];
      // sample first
      ptrs[0] = &imageData[(coordsi[0]+(coordsi[2])+(coordsi[4]))*stride];
      ptrs[1] = &imageData[(coordsi[1]+(coordsi[2])+(coordsi[4]))*stride];
      ptrs[2] = &imageData[(coordsi[0]+(coordsi[3])+(coordsi[4]))*stride];
      ptrs[3] = &imageData[(coordsi[1]+(coordsi[3])+(coordsi[4]))*stride];

      for (int s = 0; s < 4; s++){
        const T* ptr = ptrs[s];
        float* color = colors[s];
        for (int v = 0; v < VECSIZE; v++){
          color[v] = (float)ptr[v];
        }
      }

      for (int v = 0; v < VECSIZE; v++){
        colorout[v]  = LUX_LERP(coordslerp[0],colors[0][v],colors[1][v]);
      }
      for (int v = 0; v < VECSIZE; v++){
        colors[0][v] = LUX_LERP(coordslerp[0],colors[2][v],colors[3][v]);
      }
      for (int v = 0; v < VECSIZE; v++){
        colorout[v]  = LUX_LERP(coordslerp[1],colorout[v],colors[0][v]);
      }

      runs--;
      colorout = colorstore;
      coordsi[4] = coordsi[5];
    }

    if (size[2] > 1){
      for (int v = 0; v < VECSIZE; v++){
        outvals[v] = LUX_LERP(coordslerp[2],outvals[v],colorstore[v]);
      }
    }
  }

  return LUX_FALSE;
}

//////////////////////////////////////////////////////////////////////////

static TScalarArray_sampleLinear_fn* l_TSampleL[LUX_SCALAROPS_MAX_SUPPORTED*4] = {
  TScalarArray_sampleLinear<float,1>,
  TScalarArray_sampleLinear<int8,1>,
  TScalarArray_sampleLinear<uint8,1>,
  TScalarArray_sampleLinear<int16,1>,
  TScalarArray_sampleLinear<uint16,1>,
  TScalarArray_sampleLinear<int32,1>,
  TScalarArray_sampleLinear<uint32,1>,

  TScalarArray_sampleLinear<float,2>,
  TScalarArray_sampleLinear<int8,2>,
  TScalarArray_sampleLinear<uint8,2>,
  TScalarArray_sampleLinear<int16,2>,
  TScalarArray_sampleLinear<uint16,2>,
  TScalarArray_sampleLinear<int32,2>,
  TScalarArray_sampleLinear<uint32,2>,

  TScalarArray_sampleLinear<float,3>,
  TScalarArray_sampleLinear<int8,3>,
  TScalarArray_sampleLinear<uint8,3>,
  TScalarArray_sampleLinear<int16,3>,
  TScalarArray_sampleLinear<uint16,3>,
  TScalarArray_sampleLinear<int32,3>,
  TScalarArray_sampleLinear<uint32,3>,

  TScalarArray_sampleLinear<float,4>,
  TScalarArray_sampleLinear<int8,4>,
  TScalarArray_sampleLinear<uint8,4>,
  TScalarArray_sampleLinear<int16,4>,
  TScalarArray_sampleLinear<uint16,4>,
  TScalarArray_sampleLinear<int32,4>,
  TScalarArray_sampleLinear<uint32,4>,
};


LUX_API LUX_INLINE booln lxScalarArray_sampleLinear(float* LUX_RESTRICT outvals, const lxScalarArray_t *sarray, 
                 const uint size[3], const float coords[3], booln clamped[3])
{
  LUX_ASSERT(sarray->vectordim >= 1 && sarray->vectordim <= 4);
  if (size[0]*size[1]*size[2] > sarray->count)
    return LUX_TRUE;

#ifdef SCALAR_USE_XMM
  if (sarray->type == LUX_SCALAR_FLOAT32 && sarray->vectordim == 4 && 
    LUX_IS_ALIGNED(sarray->data.tvoid,16) && sarray->stride % 4 == 0 )
  {
    return XMMScalarArray_sampleLinear(outvals,*sarray,size,coords,clamped);
  }
#endif

  return l_TSampleL[sarray->type + ((sarray->vectordim-1)*LUX_SCALAROPS_MAX_SUPPORTED)](outvals,*sarray,size,coords,clamped);
}

LUX_API booln lxScalarArray3D_sampleLinear(float* LUX_RESTRICT outvals, const lxScalarArray3D_t *sarray, 
                 const float coords[3], booln notclamped[3])
{
  return lxScalarArray_sampleLinear(outvals,&sarray->sarr,sarray->size,coords,notclamped);
}

//////////////////////////////////////////////////////////////////////////
// splineCurve


#ifdef SCALAR_USE_XMM
booln LUX_FASTCALL XMMScalarArray_curveSpline(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, booln closed)
{
  uint outcount = sOut.count;
  uint incount  = sIn.count;
  uint outstride = sOut.stride/4;
  uint instride = sIn.stride/4;
  

  if (sOut.vectordim != sIn.vectordim || outcount < incount || incount < 2)
    return LUX_TRUE;

  float step = 1.0f/(float)((closed && incount==2) ? 1.0f : incount - 1 + (closed==LUX_TRUE));
  float fracc = 1.0f/(float)(outcount-(closed==LUX_FALSE));

  __m128* pOut = (__m128*) sOut.data.tvoid;
  __m128* pInStart = (__m128*)sIn.data.tvoid;
  __m128* pInEnd = (__m128*)pInStart+((incount-1)*instride);
  __m128* pIn = (__m128*)pInStart;
  
  __m128 *prev;
  __m128 *a;
  __m128 *b;
  __m128 *next;

  __m128 tempstart;
  __m128 tempend;

  // convert first set
  if (closed){
    prev = (pInEnd);
    a = (pIn);
    b = (pIn+instride);
    next = (pIn+instride*(2%incount));
  }else{
    tempstart = _mm_sub_ps( _mm_mul_ps(_mm_set_ps1(2.0f),*(pIn)), *(pIn+instride));
    prev = &tempstart;
    a = (pIn);
    b = (pIn+instride);
    if (incount == 2){
      tempend = _mm_sub_ps( _mm_mul_ps(_mm_set_ps1(2.0f),*(pIn+instride)), *(pIn));
      next = &tempend;
    }
    else{
      next = (pIn+(instride*2));
    }
  }
  pIn = pInStart + (instride*2);


  float f = 0;
  float stepdiv = 1.0f/step;

  for (uint i = 0; i < sOut.count; i++, pOut+=outstride, f+=fracc){
    if (f > step){
      f -= step;
      pIn += instride;
      // convert "next"
      prev = a;
      a = b;
      b = next;
      if (pIn > pInEnd){
        if (closed){
          pIn = pInStart;
          next = pIn;
        }
        else{
          tempend = _mm_sub_ps( _mm_mul_ps(_mm_set_ps1(2.0f),*(pInEnd)), *(pInEnd-instride));
          next = &tempend;
        }
      }
      else{
        next = pIn;
      }
    }
    float t = f*stepdiv;
    *pOut = lxVector4SSE_catmullRom(t,prev,a,b,next);
  }

  return LUX_FALSE;
}
booln LUX_FASTCALL XMMScalarArray_curveLinear(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, booln closed)
{
  uint outcount = sOut.count;
  uint incount  = sIn.count;
  uint outstride = sOut.stride/4;
  uint instride = sIn.stride/4;

  if (sOut.vectordim != sIn.vectordim || outcount < incount || incount < 2)
    return LUX_TRUE;

  float step = 1.0f/(float)((closed && incount==2) ? 1.0f : incount - 1 + (closed==LUX_TRUE));
  float fracc = 1.0f/(float)(outcount-(closed==LUX_FALSE));

  __m128* pOut = (__m128*) sOut.data.tvoid;
  __m128* pInStart = (__m128*)sIn.data.tvoid;
  __m128* pInEnd = (__m128*)pInStart+((incount-1)*instride);
  __m128* pIn = (__m128*)pInStart;

  __m128 *a = (pIn);
  __m128 *b = (pIn+instride);

  pIn = pInStart+instride;

  float f = 0;
  float stepdiv = 1.0f/step;

  for (uint i = 0; i < outcount; i++, pOut+=outstride, f+=fracc){
    if (f > step){
      f -= step;
      pIn += instride;
      // convert "b"
      __m128 *temp = b;
      b = a;
      a = temp;
      if (pIn > pInEnd){
        pIn = pInStart;
      }
      b = (pIn);
    }
    float t = f*stepdiv;
    *pOut = lxVector4SSE_lerp(t,*a,*b);
  }

  return LUX_FALSE;
}
#endif


typedef booln (LUX_FASTCALL TScalarArray_curveSpline_fn)(lxScalarArray_t &sarray, const lxScalarArray_t &sarray0, booln closed);

template <class T, int VECSIZE,int COMPACT>
booln LUX_FASTCALL TScalarArray_curveSpline(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, booln closed)
{
  uint outcount = sOut.count;
  uint incount  = sIn.count;
  uint outstride = COMPACT ? VECSIZE : sOut.stride;
  uint instride = COMPACT ? VECSIZE : sIn.stride;

  if (sOut.vectordim != sIn.vectordim || outcount < incount || incount < 2)
    return LUX_TRUE;

  float step = 1.0f/(float)((closed && incount==2) ? 1.0f : incount - 1 + (closed==LUX_TRUE));
  float fracc = 1.0f/(float)(outcount-(closed==LUX_FALSE));

  T* LUX_RESTRICT pOut = (T*) sOut.data.tvoid;
  const T * LUX_RESTRICT pInStart = (const T*)sIn.data.tvoid;
  const T * LUX_RESTRICT pInEnd = pInStart+((incount-1)*instride);
  const T * LUX_RESTRICT pIn = pInStart;

  float vec[4][VECSIZE];

  // convert first set
  if (closed){
    for (int v = 0; v < VECSIZE; v++){
      vec[0][v] = (float)((pInEnd)[v]);
      vec[1][v] = (float)((pIn)[v]);
      vec[2][v] = (float)((pIn+instride)[v]);
      vec[3][v] = (float)((pIn+instride*(2%incount))[v]);
    }

  }else{
    for (int v = 0; v < VECSIZE; v++){
      vec[0][v] = (float)((pIn)[v]*T(2) - (pIn+instride)[v]);
      vec[1][v] = (float)((pIn)[v]);
      vec[2][v] = (float)((pIn+instride)[v]);
    }
    if (incount == 2){
      for (int v = 0; v < VECSIZE; v++){
        vec[3][v] = (float)((pIn+instride)[v]*T(2) - (pIn)[v]);
      }
    }
    else{
      for (int v = 0; v < VECSIZE; v++){
        vec[3][v] = (float)((pIn+(instride*2))[v]);
      }
    }
  }
  pIn = pInStart + (instride*2);
  
  
  float f = 0;

  float *prev = vec[0];
  float *a = vec[1];
  float *b = vec[2];
  float *next = vec[3];
  float stepdiv = 1.0f/step;

  for (uint i = 0; i < outcount; i++, pOut+=outstride, f+=fracc){
    if (f > step){
      f -= step;
      pIn += instride;
      // convert "next"
      float *temp = prev;
      prev = a;
      a = b;
      b = next;
      next = temp;
      if (pIn > pInEnd){
        if (closed){
          pIn = pInStart;
          for (int v = 0; v < VECSIZE; v++){
            next[v] = (float)((pIn)[v]);
          }
        }
        else{
          for (int v = 0; v < VECSIZE; v++){
            next[v] = (float)((pInEnd)[v]*T(2) - (pInEnd-instride)[v]);
          }
        }
      }
      else{
        for (int v = 0; v < VECSIZE; v++){
          next[v] = (float)((pIn)[v]);
        }
      }
    }
    float t = f*stepdiv;
    float t2=t*t;
    float t3=t2*t;

    float coeff0 = -t3 + 2.0f * t2 - t;
    float coeff1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
    float coeff2 = -3.0f * t3 + 4.0f * t2 + t;
    float coeff3 = t3 - t2;

    for (int v = 0; v < VECSIZE; v++){
      pOut[v] = (T)((coeff0 * prev[v] + coeff1 * a[v] + coeff2 * b[v] + coeff3 * next[3])* 0.5f);
    }
  }

  return LUX_FALSE;
}


template <int VECSIZE,int COMPACT>
booln LUX_FASTCALL TFScalarArray_curveSpline(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, booln closed)
{
  uint outcount = sOut.count;
  uint incount  = sIn.count;
  uint outstride = COMPACT ? VECSIZE : sOut.stride;
  uint instride = COMPACT ? VECSIZE : sIn.stride;

  if (sOut.vectordim != sIn.vectordim || outcount < incount || incount < 2)
    return LUX_TRUE;

  float step = 1.0f/(float)((closed && incount==2) ? 1.0f : incount - 1 + (closed==LUX_TRUE));
  float fracc = 1.0f/(float)(outcount-(closed==LUX_FALSE));

  float * LUX_RESTRICT pOut = (float*) sOut.data.tvoid;
  float * LUX_RESTRICT pInStart = (float*)sIn.data.tvoid;
  float * LUX_RESTRICT pInEnd = pInStart+((incount-1)*instride);
  float * LUX_RESTRICT pIn = pInStart;

  float *prev;
  float *a;
  float *b;
  float *next;
  
  float tempstart[VECSIZE];
  float tempend[VECSIZE];

  // convert first set
  if (closed){
    prev = pInEnd;
    a = pIn;
    b = pIn+instride;
    next = pIn+(instride*(2%incount));
  }else{
    a = pIn;
    b = pIn+instride;

    for (int v = 0; v < VECSIZE; v++){
      tempstart[v] = (a[v]*2.0f - b[v]);
    }
    prev = tempstart;

    if (incount == 2){
      for (int v = 0; v < VECSIZE; v++){
        tempend[v] = (b[v]*2.0f - a[v]);
      }
      next = tempend;
    }
    else{
      next = pIn+(instride*2);
      for (int v = 0; v < VECSIZE; v++){
        tempend[v] = ((pInEnd)[v]*2.0f - (pInEnd-instride)[v]);
      }
    }
  }
  pIn = pInStart + (instride*2);

  float f = 0.0f;
  float stepdiv = 1.0f/step;

  if (closed)
    for (uint i = 0; i < outcount; i++, pOut+=outstride, f+=fracc){
      if (f > step){
        f -= step;
        pIn += instride;
        // convert "next"
        prev = a;
        a = b;
        b = next;
        if (pIn > pInEnd){
          if (closed){
            pIn = pInStart;
            next = pIn;
          }
          else{
            next = tempend;
          }
        }
        else{
          next = pIn;
        }
      }
      float t = f*stepdiv;
      float t2=t*t;
      float t3=t2*t;

      float coeff0 = -t3 + 2.0f * t2 - t;
      float coeff1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
      float coeff2 = -3.0f * t3 + 4.0f * t2 + t;
      float coeff3 = t3 - t2;

      for (int v = 0; v < VECSIZE; v++){
        pOut[v] = ((coeff0 * prev[v] + coeff1 * a[v] + coeff2 * b[v] + coeff3 * next[3])* 0.5f);
      }
    }
  else{
    for (uint i = 0; i < outcount; i++, pOut+=outstride, f+=fracc){
      if (f > step && a < pInEnd){
        f -= step;
        pIn += instride;
        // convert "next"
        prev = a;
        a += instride;
        if (pIn > pInEnd){
          next = tempend;
        }
        else{
          next = pIn;
        }
      }
      float t = f*stepdiv;
      float t2=t*t;
      float t3=t2*t;
      b = a+instride;

      float coeff0 = -t3 + 2.0f * t2 - t;
      float coeff1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
      float coeff2 = -3.0f * t3 + 4.0f * t2 + t;
      float coeff3 = t3 - t2;

      for (int v = 0; v < VECSIZE; v++){
        pOut[v] = ((coeff0 * prev[v] + coeff1 * a[v] + coeff2 * b[v] + coeff3 * next[3])* 0.5f);
      }
    }
  }

  return LUX_FALSE;
}
// linearCurve
typedef booln (LUX_FASTCALL TScalarArray_curveLinear_fn)(lxScalarArray_t &sarray, const lxScalarArray_t &sarray0, booln closed);

template <class T, int VECSIZE, int COMPACT>
booln LUX_FASTCALL TScalarArray_curveLinear(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, booln closed)
{
  uint outcount = sOut.count;
  uint incount  = sIn.count;
  uint outstride = COMPACT ? VECSIZE : sOut.stride;
  uint instride  = COMPACT ? VECSIZE : sIn.stride;

  if (sOut.vectordim != sIn.vectordim || outcount < incount || incount < 2)
    return LUX_TRUE;

  float step = 1.0f/(float)((closed && incount==2) ? 1.0f : incount - 1 + (closed==LUX_TRUE));
  float fracc = 1.0f/(float)(outcount-(closed==LUX_FALSE));

  T* LUX_RESTRICT pOut = (T*) sOut.data.tvoid;
  const T * LUX_RESTRICT pInStart = (const T*)sIn.data.tvoid;
  const T * LUX_RESTRICT pInEnd = pInStart+((incount-1)*instride);
  const T * LUX_RESTRICT pIn = pInStart;

  float vec[2][VECSIZE];


  // convert first set
  for (int v = 0; v < VECSIZE; v++){
    vec[0][v] = (float)((pIn)[v]);
    vec[1][v] = (float)((pIn+instride)[v]);
  }
  pIn = pInStart + instride;


  float f = 0;
  float *a = vec[0];
  float *b = vec[1];
  float stepdiv = 1.0f/step;

  for (uint i = 0; i < outcount; i++, pOut+=outstride, f+=fracc){
    if (f > step){
      f -= step;
      pIn += instride;
      // convert "b"
      float *temp = b;
      b = a;
      a = temp;
      if (pIn > pInEnd){
        pIn = pInStart;
      }
      for (int v = 0; v < VECSIZE; v++){
        b[v] = (float)((pIn)[v]);
      }
    }
    float t = f*stepdiv;
    for (int v = 0; v < VECSIZE; v++){
      pOut[v] = (T)((a[v]) + (((b[v])-(a[v]))*(t)));
    }
  }

  return LUX_FALSE;
}

template <int VECSIZE, int COMPACT>
booln LUX_FASTCALL TFScalarArray_curveLinear(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, booln closed)
{
  uint outcount = sOut.count;
  uint incount  = sIn.count;
  uint outstride = COMPACT ? VECSIZE : sOut.stride;
  uint instride  = COMPACT ? VECSIZE : sIn.stride;

  if (sOut.vectordim != sIn.vectordim || outcount < incount || incount < 2)
    return LUX_TRUE;

  float step = 1.0f/(float)((closed && incount==2) ? 1.0f : incount - 1 + (closed==LUX_TRUE));
  float fracc = 1.0f/(float)(outcount-(closed==LUX_FALSE));

  float * LUX_RESTRICT pOut = (float*) sOut.data.tvoid;
  float * LUX_RESTRICT pInStart = (float*)sIn.data.tvoid;
  float * LUX_RESTRICT pInEnd = pInStart+((incount-1)*instride);
  float * LUX_RESTRICT pIn = pInStart;
  pIn = pInStart + instride;


  float f = 0;
  float *a = (pIn);
  float *b = (pIn+instride);
  float stepdiv = 1.0f/step;

  for (uint i = 0; i < outcount; i++, pOut+=outstride, f+=fracc){
    if (f > step ){
      f -= step;
      pIn += instride;
      // convert "b"
      a = b;
      if (pIn > pInEnd){
        pIn = pInStart;
      }
      b = pIn;
    }
    float t = f*stepdiv;
    for (int v = 0; v < VECSIZE; v++){
      pOut[v] = ((a[v]) + (((b[v])-(a[v]))*(t)));
    }
  }

  return LUX_FALSE;
}


static TScalarArray_curveSpline_fn* l_CurveSpline[LUX_SCALAROPS_MAX_SUPPORTED*4*2] = {
  TFScalarArray_curveSpline<1,LUX_FALSE>,
  TScalarArray_curveSpline<int8,1,LUX_FALSE>,
  TScalarArray_curveSpline<uint8,1,LUX_FALSE>,
  TScalarArray_curveSpline<int16,1,LUX_FALSE>,
  TScalarArray_curveSpline<uint16,1,LUX_FALSE>,
  TScalarArray_curveSpline<int32,1,LUX_FALSE>,
  TScalarArray_curveSpline<uint32,1,LUX_FALSE>,

  TFScalarArray_curveSpline<2,LUX_FALSE>,
  TScalarArray_curveSpline<int8,2,LUX_FALSE>,
  TScalarArray_curveSpline<uint8,2,LUX_FALSE>,
  TScalarArray_curveSpline<int16,2,LUX_FALSE>,
  TScalarArray_curveSpline<uint16,2,LUX_FALSE>,
  TScalarArray_curveSpline<int32,2,LUX_FALSE>,
  TScalarArray_curveSpline<uint32,2,LUX_FALSE>,

  TFScalarArray_curveSpline<3,LUX_FALSE>,
  TScalarArray_curveSpline<int8,3,LUX_FALSE>,
  TScalarArray_curveSpline<uint8,3,LUX_FALSE>,
  TScalarArray_curveSpline<int16,3,LUX_FALSE>,
  TScalarArray_curveSpline<uint16,3,LUX_FALSE>,
  TScalarArray_curveSpline<int32,3,LUX_FALSE>,
  TScalarArray_curveSpline<uint32,3,LUX_FALSE>,

  TFScalarArray_curveSpline<4,LUX_FALSE>,
  TScalarArray_curveSpline<int8,4,LUX_FALSE>,
  TScalarArray_curveSpline<uint8,4,LUX_FALSE>,
  TScalarArray_curveSpline<int16,4,LUX_FALSE>,
  TScalarArray_curveSpline<uint16,4,LUX_FALSE>,
  TScalarArray_curveSpline<int32,4,LUX_FALSE>,
  TScalarArray_curveSpline<uint32,4,LUX_FALSE>,

  TFScalarArray_curveSpline<1,LUX_TRUE>,
  TScalarArray_curveSpline<int8,1,LUX_TRUE>,
  TScalarArray_curveSpline<uint8,1,LUX_TRUE>,
  TScalarArray_curveSpline<int16,1,LUX_TRUE>,
  TScalarArray_curveSpline<uint16,1,LUX_TRUE>,
  TScalarArray_curveSpline<int32,1,LUX_TRUE>,
  TScalarArray_curveSpline<uint32,1,LUX_TRUE>,

  TFScalarArray_curveSpline<2,LUX_TRUE>,
  TScalarArray_curveSpline<int8,2,LUX_TRUE>,
  TScalarArray_curveSpline<uint8,2,LUX_TRUE>,
  TScalarArray_curveSpline<int16,2,LUX_TRUE>,
  TScalarArray_curveSpline<uint16,2,LUX_TRUE>,
  TScalarArray_curveSpline<int32,2,LUX_TRUE>,
  TScalarArray_curveSpline<uint32,2,LUX_TRUE>,

  TFScalarArray_curveSpline<3,LUX_TRUE>,
  TScalarArray_curveSpline<int8,3,LUX_TRUE>,
  TScalarArray_curveSpline<uint8,3,LUX_TRUE>,
  TScalarArray_curveSpline<int16,3,LUX_TRUE>,
  TScalarArray_curveSpline<uint16,3,LUX_TRUE>,
  TScalarArray_curveSpline<int32,3,LUX_TRUE>,
  TScalarArray_curveSpline<uint32,3,LUX_TRUE>,

  TFScalarArray_curveSpline<4,LUX_TRUE>,
  TScalarArray_curveSpline<int8,4,LUX_TRUE>,
  TScalarArray_curveSpline<uint8,4,LUX_TRUE>,
  TScalarArray_curveSpline<int16,4,LUX_TRUE>,
  TScalarArray_curveSpline<uint16,4,LUX_TRUE>,
  TScalarArray_curveSpline<int32,4,LUX_TRUE>,
  TScalarArray_curveSpline<uint32,4,LUX_TRUE>,
};

static TScalarArray_curveLinear_fn* l_CurveLinear[LUX_SCALAROPS_MAX_SUPPORTED*4*2] = {
  TFScalarArray_curveLinear<1,LUX_FALSE>,
  TScalarArray_curveLinear<int8,1,LUX_FALSE>,
  TScalarArray_curveLinear<uint8,1,LUX_FALSE>,
  TScalarArray_curveLinear<int16,1,LUX_FALSE>,
  TScalarArray_curveLinear<uint16,1,LUX_FALSE>,
  TScalarArray_curveLinear<int32,1,LUX_FALSE>,
  TScalarArray_curveLinear<uint32,1,LUX_FALSE>,

  TFScalarArray_curveLinear<2,LUX_FALSE>,
  TScalarArray_curveLinear<int8,2,LUX_FALSE>,
  TScalarArray_curveLinear<uint8,2,LUX_FALSE>,
  TScalarArray_curveLinear<int16,2,LUX_FALSE>,
  TScalarArray_curveLinear<uint16,2,LUX_FALSE>,
  TScalarArray_curveLinear<int32,2,LUX_FALSE>,
  TScalarArray_curveLinear<uint32,2,LUX_FALSE>,

  TFScalarArray_curveLinear<3,LUX_FALSE>,
  TScalarArray_curveLinear<int8,3,LUX_FALSE>,
  TScalarArray_curveLinear<uint8,3,LUX_FALSE>,
  TScalarArray_curveLinear<int16,3,LUX_FALSE>,
  TScalarArray_curveLinear<uint16,3,LUX_FALSE>,
  TScalarArray_curveLinear<int32,3,LUX_FALSE>,
  TScalarArray_curveLinear<uint32,3,LUX_FALSE>,

  TFScalarArray_curveLinear<4,LUX_FALSE>,
  TScalarArray_curveLinear<int8,4,LUX_FALSE>,
  TScalarArray_curveLinear<uint8,4,LUX_FALSE>,
  TScalarArray_curveLinear<int16,4,LUX_FALSE>,
  TScalarArray_curveLinear<uint16,4,LUX_FALSE>,
  TScalarArray_curveLinear<int32,4,LUX_FALSE>,
  TScalarArray_curveLinear<uint32,4,LUX_FALSE>,

  TFScalarArray_curveLinear<1,LUX_TRUE>,
  TScalarArray_curveLinear<int8,1,LUX_TRUE>,
  TScalarArray_curveLinear<uint8,1,LUX_TRUE>,
  TScalarArray_curveLinear<int16,1,LUX_TRUE>,
  TScalarArray_curveLinear<uint16,1,LUX_TRUE>,
  TScalarArray_curveLinear<int32,1,LUX_TRUE>,
  TScalarArray_curveLinear<uint32,1,LUX_TRUE>,

  TFScalarArray_curveLinear<2,LUX_TRUE>,
  TScalarArray_curveLinear<int8,2,LUX_TRUE>,
  TScalarArray_curveLinear<uint8,2,LUX_TRUE>,
  TScalarArray_curveLinear<int16,2,LUX_TRUE>,
  TScalarArray_curveLinear<uint16,2,LUX_TRUE>,
  TScalarArray_curveLinear<int32,2,LUX_TRUE>,
  TScalarArray_curveLinear<uint32,2,LUX_TRUE>,

  TFScalarArray_curveLinear<3,LUX_TRUE>,
  TScalarArray_curveLinear<int8,3,LUX_TRUE>,
  TScalarArray_curveLinear<uint8,3,LUX_TRUE>,
  TScalarArray_curveLinear<int16,3,LUX_TRUE>,
  TScalarArray_curveLinear<uint16,3,LUX_TRUE>,
  TScalarArray_curveLinear<int32,3,LUX_TRUE>,
  TScalarArray_curveLinear<uint32,3,LUX_TRUE>,

  TFScalarArray_curveLinear<4,LUX_TRUE>,
  TScalarArray_curveLinear<int8,4,LUX_TRUE>,
  TScalarArray_curveLinear<uint8,4,LUX_TRUE>,
  TScalarArray_curveLinear<int16,4,LUX_TRUE>,
  TScalarArray_curveLinear<uint16,4,LUX_TRUE>,
  TScalarArray_curveLinear<int32,4,LUX_TRUE>,
  TScalarArray_curveLinear<uint32,4,LUX_TRUE>,
};

// output array will contain interpolated
// points between argarray's points
LUX_API booln lxScalarArray_curveLinear(lxScalarArray_t *sarray, const lxScalarArray_t *sarray0, booln closed)
{
  LUX_ASSERT(sarray->vectordim >= 1 && sarray->vectordim <= 4);
  if (sarray->type != sarray0->type)
    return LUX_TRUE;

#ifdef SCALAR_USE_XMM
  if (sarray->type == LUX_SCALAR_FLOAT32 && sarray->vectordim == 4 && 
        LUX_IS_ALIGNED(sarray->data.tvoid,16) && LUX_IS_ALIGNED(sarray0->data.tvoid,16) &&
        sarray->stride % 4 == 0 && sarray0->stride % 4 == 0)
  {
    return XMMScalarArray_curveLinear(*sarray,*sarray0,closed);
  }
#endif
  booln compact = sarray->vectordim==sarray->stride && sarray0->vectordim==sarray0->stride;

  return l_CurveLinear[sarray->type + ((sarray->vectordim-1)*LUX_SCALAROPS_MAX_SUPPORTED) + ((compact)*LUX_SCALAROPS_MAX_SUPPORTED*4)](*sarray,*sarray0,closed);
}
LUX_API booln lxScalarArray_curveSpline(lxScalarArray_t *sarray, const lxScalarArray_t *sarray0, booln closed)
{
  LUX_ASSERT(sarray->vectordim >= 1 && sarray->vectordim <= 4);
  if (sarray->type != sarray0->type)
    return LUX_TRUE;

#ifdef SCALAR_USE_XMM
  if (sarray->type == LUX_SCALAR_FLOAT32 && sarray->vectordim == 4 && 
    LUX_IS_ALIGNED(sarray->data.tvoid,16) && LUX_IS_ALIGNED(sarray0->data.tvoid,16) &&
    sarray->stride % 4 == 0 && sarray0->stride % 4 == 0)
  {
    return XMMScalarArray_curveSpline(*sarray,*sarray0,closed);
  }
#endif
  booln compact = sarray->vectordim==sarray->stride && sarray0->vectordim==sarray0->stride;

  return l_CurveSpline[sarray->type + ((sarray->vectordim-1)*LUX_SCALAROPS_MAX_SUPPORTED) + ((compact)*LUX_SCALAROPS_MAX_SUPPORTED*4)](*sarray,*sarray0,closed);
}

//////////////////////////////////////////////////////////////////////////
// ScalarArray Float Only

struct FVector1 {
  float data[1];

  LUX_INLINE void  Set(float val){
    data[0] = val;
  }

  LUX_INLINE FVector1& operator *= ( float val){
    data[0] *= val;

    return *this;
  }

  LUX_INLINE float Distance(const FVector1& other) const {
    return fabsf(data[0]-other.data[0]);
  }
};


struct FVector2 {
  float data[2];

  LUX_INLINE void  Set(float val){
    data[0] = val;
    data[1] = val;
  }

  LUX_INLINE FVector2& operator *= ( float val){
    data[0] *= val;
    data[1] *= val;

    return *this;
  }

  LUX_INLINE float Distance(const FVector2& other) const {
    float a = data[0]-other.data[0];
    float b = data[1]-other.data[1];

    return sqrtf(a*a + b*b);
  }
};

struct FVector3 {
  float data[3];

  LUX_INLINE void  Set(float val){
    data[0] = val;
    data[1] = val;
    data[2] = val;
  }

  LUX_INLINE FVector3& operator *= ( float val){
    data[0] *= val;
    data[1] *= val;
    data[2] *= val;

    return *this;
  }

  LUX_INLINE float Distance(const FVector3& other) const {
    float a = data[0]-other.data[0];
    float b = data[1]-other.data[1];
    float c = data[2]-other.data[2];

    return sqrtf(a*a + b*b + c*c);
  }
};

struct FVector4 {
  float data[4];

  LUX_INLINE void  Set(float val){
    data[0] = val;
    data[1] = val;
    data[2] = val;
    data[3] = val;
  }

  LUX_INLINE FVector4& operator *= ( float val){
    data[0] *= val;
    data[1] *= val;
    data[2] *= val;
    data[3] *= val;

    return *this;
  }

  LUX_INLINE float Distance(const FVector4& other) const {
    float a = data[0]-other.data[0];
    float b = data[1]-other.data[1];
    float c = data[2]-other.data[2];
    float d = data[3]-other.data[3];

    return sqrtf(a*a + b*b + c*c + d*d);
  }
};
#ifdef SCALAR_USE_XMM
struct FVector4SSE{
  __m128 data;

  LUX_INLINE void  Set(float val){
    data = _mm_set_ps1(val);
  }

  LUX_INLINE FVector4SSE& operator *= ( float val){
    data = _mm_mul_ps(data,_mm_set_ps1(val));

    return *this;
  }

  LUX_INLINE float Distance(const FVector4SSE& other) const {
    __m128 diff = _mm_sub_ps(data,other.data);
    diff = _mm_mul_ps(diff,diff);
    return sqrtf(diff.m128_f32[0] + diff.m128_f32[1] + diff.m128_f32[2]);
  }
};
#endif

struct FVectorTransform{
  static void LUX_INLINE proc(FVector2 &avec, FVector2 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector2Transform(avec.data,bvec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector3 &avec, FVector3 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector3Transform(avec.data,bvec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector4 &avec, FVector4 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector3Transform(avec.data,bvec.data,matrix44);
    avec.data[3] = 1.0f;
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec, FVector4SSE &bvec, lxMatrix44CPTR matrix44)
  {
    avec.data = lxVector4SSE_transformV3(bvec.data,(__m128*)matrix44);
  }
#endif


  static void LUX_INLINE proc(FVector2 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector2Transform1(avec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector3 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector3Transform1(avec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector4 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector3Transform1(avec.data,matrix44);
    avec.data[3] = 1.0f;
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec, lxMatrix44CPTR matrix44)
  {
    avec.data = lxVector4SSE_transformV3(avec.data,(__m128*)matrix44);
  }
#endif
};
struct FVectorTransformRot {
  static void LUX_INLINE proc(FVector2 &avec, FVector2 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector2TransformRot(avec.data,bvec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector3 &avec, FVector3 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector3TransformRot(avec.data,bvec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector4 &avec, FVector4 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector3TransformRot(avec.data,bvec.data,matrix44);
    avec.data[3] = 1.0f;
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec, FVector4SSE &bvec, lxMatrix44CPTR matrix44)
  {
    avec.data = lxVector4SSE_transformRot(bvec.data,(__m128*)matrix44);
  }
#endif

  static void LUX_INLINE proc(FVector2 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector2TransformRot1(avec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector3 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector3TransformRot1(avec.data,matrix44);
  }
  static void LUX_INLINE proc(FVector4 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector3TransformRot1(avec.data,matrix44);
    avec.data[3] = 1.0f;
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec, lxMatrix44CPTR matrix44)
  {
    avec.data = lxVector4SSE_transformRot(avec.data,(__m128*)matrix44);
  }
#endif
};

struct FVectorTransformFull {
  static void LUX_INLINE proc(FVector2 &avec, const FVector2 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector4 vec = {bvec.data[0],bvec.data[1],0.0f,1.0f};
    lxVector4Transform1(vec,matrix44);
    lxVector2Scale(avec.data,vec,1.0f/vec[3]);
  }
  static void LUX_INLINE proc(FVector3 &avec, const FVector3 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector4 vec = {bvec.data[0],bvec.data[1],bvec.data[2],1.0f};
    lxVector4Transform1(vec,matrix44);
    lxVector3Scale(avec.data,vec,1.0f/vec[3]);
  }
  static void LUX_INLINE proc(FVector4 &avec, const FVector4 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector4Transform(avec.data,bvec.data,matrix44);
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec,  FVector4SSE &bvec, lxMatrix44CPTR matrix44)
  {
    avec.data = lxVector4SSE_transform(bvec.data,(__m128*)matrix44);
  }
#endif

  static void LUX_INLINE proc(FVector2 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector4 vec = {avec.data[0],avec.data[1],0.0f,1.0f};
    lxVector4Transform1(vec,matrix44);
    lxVector2Scale(avec.data,vec,1.0f/vec[3]);
  }
  static void LUX_INLINE proc(FVector3 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector4 vec = {avec.data[0],avec.data[1],avec.data[2],1.0f};
    lxVector4Transform1(vec,matrix44);
    lxVector3Scale(avec.data,vec,1.0f/vec[3]);
  }
  static void LUX_INLINE proc(FVector4 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector4Transform1(avec.data,matrix44);
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec, lxMatrix44CPTR matrix44)
  {
    avec.data = lxVector4SSE_transform(avec.data,(__m128*)matrix44);
  }
#endif
};

struct FVectorNormalize {
  static void LUX_INLINE proc(FVector2 &avec, const FVector2 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector2NormalizeFast(avec.data,bvec.data);
  }
  static void LUX_INLINE proc(FVector3 &avec, const FVector3 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector3NormalizeFast(avec.data,bvec.data);
  }
  static void LUX_INLINE proc(FVector4 &avec, const FVector4 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector4NormalizeFast(avec.data,bvec.data);
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec,  FVector4SSE &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector4NormalizeFast(avec.data.m128_f32,bvec.data.m128_f32);
  }
#endif

  static void LUX_INLINE proc(FVector2 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector2NormalizedFast(avec.data);
  }
  static void LUX_INLINE proc(FVector3 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector3NormalizedFast(avec.data);
  }
  static void LUX_INLINE proc(FVector4 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector4NormalizedFast(avec.data);
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec, lxMatrix44CPTR matrix44)
  {
    lxVector4NormalizedFast(avec.data.m128_f32);
  }
#endif
};

struct FVectorNormalizeAcc {
  static void LUX_INLINE proc(FVector2 &avec, const FVector2 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector2Normalize(avec.data,bvec.data);
  }
  static void LUX_INLINE proc(FVector3 &avec, const FVector3 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector3Normalize(avec.data,bvec.data);
  }
  static void LUX_INLINE proc(FVector4 &avec, const FVector4 &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector4Normalize(avec.data,bvec.data);
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec,  FVector4SSE &bvec, lxMatrix44CPTR matrix44)
  {
    lxVector4Normalize(avec.data.m128_f32,bvec.data.m128_f32);
  }
#endif

  static void LUX_INLINE proc(FVector2 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector2Normalized(avec.data);
  }
  static void LUX_INLINE proc(FVector3 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector3Normalized(avec.data);
  }
  static void LUX_INLINE proc(FVector4 &avec, lxMatrix44CPTR matrix44)
  {
    lxVector4Normalized(avec.data);
  }
#ifdef SCALAR_USE_XMM
  static void LUX_INLINE proc(FVector4SSE &avec, lxMatrix44CPTR matrix44)
  {
    lxVector4Normalized(avec.data.m128_f32);
  }
#endif
};


template <class T,class TProc, int VECSIZE>
void LUX_FASTCALL TFScalarArray_transform1(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, const float* LUX_RESTRICT arg)
{
  T* LUX_RESTRICT pOut = (T*)sOut.data.tfloat;
  T* LUX_RESTRICT pIn  = (T*)sIn.data.tfloat;
  size_t ostride = sOut.stride / VECSIZE;
  size_t istride = sIn.stride / VECSIZE;
  size_t count = LUX_MIN(sOut.count,sIn.count);

  if (ostride == 1 && istride == 1){
    size_t unrollcnt = (count/8);
    for (size_t i = 0; i < unrollcnt; i++, pOut +=8, pIn +=8){
      TProc::proc(pOut[0],pIn[0],arg);
      TProc::proc(pOut[1],pIn[1],arg);
      TProc::proc(pOut[2],pIn[2],arg);
      TProc::proc(pOut[3],pIn[3],arg);
      TProc::proc(pOut[4],pIn[4],arg);
      TProc::proc(pOut[5],pIn[5],arg);
      TProc::proc(pOut[6],pIn[6],arg);
      TProc::proc(pOut[7],pIn[7],arg);

    }
    for (size_t i = unrollcnt*8 ; i < count; i++, pOut ++, pIn ++){
      TProc::proc(pOut[0],pIn[0],arg);
    }
  }
  else{
    for (size_t i = 0; i < count; i++, pOut += ostride, pIn += istride){
      TProc::proc(pOut[0],pIn[0],arg);
    }
  }
}

template <class T,class TProc, int VECSIZE>
void LUX_FASTCALL TFScalarArray_transform0(lxScalarArray_t &sOut, const float* LUX_RESTRICT arg)
{
  T* pOut = (T*)sOut.data.tfloat;
  size_t ostride = sOut.stride / VECSIZE;
  size_t count = sOut.count;

  if (ostride == 1){
    size_t unrollcnt = (count/8);
    for (size_t i = 0; i < unrollcnt; i++, pOut +=8){
      TProc::proc(pOut[0],arg);
      TProc::proc(pOut[1],arg);
      TProc::proc(pOut[2],arg);
      TProc::proc(pOut[3],arg);
      TProc::proc(pOut[4],arg);
      TProc::proc(pOut[5],arg);
      TProc::proc(pOut[6],arg);
      TProc::proc(pOut[7],arg);

    }
    for (size_t i = unrollcnt*8 ; i < count; i++, pOut ++){
      TProc::proc(pOut[0],arg);
    }
  }
  else{
    for (size_t i = 0; i < count; i++, pOut += ostride){
      TProc::proc(pOut[0],arg);
    }
  }
}

typedef void (LUX_FASTCALL TFScalarArray_transform0_fn)(lxScalarArray_t &sOut, const float* LUX_RESTRICT arg);
typedef void (LUX_FASTCALL TFScalarArray_transform1_fn)(lxScalarArray_t &sOut, const lxScalarArray_t &sIn, const float* LUX_RESTRICT arg);


#ifdef SCALAR_USE_XMM
static TFScalarArray_transform0_fn *l_FTransform0SSE[LUX_FSCALAR_OP1S*3] =
{
  TFScalarArray_transform0<FVector4SSE,FVectorTransform,4>,
  TFScalarArray_transform0<FVector4SSE,FVectorTransformRot,4>,
  TFScalarArray_transform0<FVector4SSE,FVectorTransformFull,4>,
  TFScalarArray_transform0<FVector4SSE,FVectorNormalize,4>,
  TFScalarArray_transform0<FVector4SSE,FVectorNormalizeAcc,4>,
};

static TFScalarArray_transform1_fn *l_FTransform1SSE[LUX_FSCALAR_OP1S*3] =
{
  TFScalarArray_transform1<FVector4SSE,FVectorTransform,4>,
  TFScalarArray_transform1<FVector4SSE,FVectorTransformRot,4>,
  TFScalarArray_transform1<FVector4SSE,FVectorTransformFull,4>,
  TFScalarArray_transform1<FVector4SSE,FVectorNormalize,4>,
  TFScalarArray_transform1<FVector4SSE,FVectorNormalizeAcc,4>,
};

#endif

static TFScalarArray_transform0_fn *l_FTransform0[LUX_FSCALAR_OP1S*3] =
{
  TFScalarArray_transform0<FVector2,FVectorTransform,2>,
  TFScalarArray_transform0<FVector3,FVectorTransform,3>,
  TFScalarArray_transform0<FVector4,FVectorTransform,4>,

  TFScalarArray_transform0<FVector2,FVectorTransformRot,2>,
  TFScalarArray_transform0<FVector3,FVectorTransformRot,3>,
  TFScalarArray_transform0<FVector4,FVectorTransformRot,4>,

  TFScalarArray_transform0<FVector2,FVectorTransformFull,2>,
  TFScalarArray_transform0<FVector3,FVectorTransformFull,3>,
  TFScalarArray_transform0<FVector4,FVectorTransformFull,4>,

  TFScalarArray_transform0<FVector2,FVectorNormalize,2>,
  TFScalarArray_transform0<FVector3,FVectorNormalize,3>,
  TFScalarArray_transform0<FVector4,FVectorNormalize,4>,

  TFScalarArray_transform0<FVector2,FVectorNormalizeAcc,2>,
  TFScalarArray_transform0<FVector3,FVectorNormalizeAcc,3>,
  TFScalarArray_transform0<FVector4,FVectorNormalizeAcc,4>,
};




static TFScalarArray_transform1_fn *l_FTransform1[LUX_FSCALAR_OP1S*3] =
{
  TFScalarArray_transform1<FVector2,FVectorTransform,2>,
  TFScalarArray_transform1<FVector3,FVectorTransform,3>,
  TFScalarArray_transform1<FVector4,FVectorTransform,4>,

  TFScalarArray_transform1<FVector2,FVectorTransformRot,2>,
  TFScalarArray_transform1<FVector3,FVectorTransformRot,3>,
  TFScalarArray_transform1<FVector4,FVectorTransformRot,4>,

  TFScalarArray_transform1<FVector2,FVectorTransformFull,2>,
  TFScalarArray_transform1<FVector3,FVectorTransformFull,3>,
  TFScalarArray_transform1<FVector4,FVectorTransformFull,4>,

  TFScalarArray_transform1<FVector2,FVectorNormalize,2>,
  TFScalarArray_transform1<FVector3,FVectorNormalize,3>,
  TFScalarArray_transform1<FVector4,FVectorNormalize,4>,

  TFScalarArray_transform1<FVector2,FVectorNormalizeAcc,2>,
  TFScalarArray_transform1<FVector3,FVectorNormalizeAcc,3>,
  TFScalarArray_transform1<FVector4,FVectorNormalizeAcc,4>,
};

// works on vector2,3,4
LUX_API booln lxFScalarArray_op1(lxScalarArray_t *sarray, lxFScalarArrayOp_t op, const lxScalarArray_t *sarray0, const float * LUX_RESTRICT arg)
{
  if (sarray->type != LUX_SCALAR_FLOAT32 || sarray->type != sarray0->type || sarray->vectordim != sarray0->vectordim || sarray->vectordim < 2)
    return LUX_TRUE;

  LUX_ASSERT(sarray->vectordim < 5);

  LUX_ASSUME(sarray->vectordim < 5);
  LUX_ASSUME(sarray0->vectordim < 5);

#ifdef SCALAR_USE_XMM
  if (sarray->vectordim == 4 && sarray->stride % 4 == 0 && sarray0->stride % 4 == 0 &&
    LUX_IS_ALIGNED(sarray->data.tvoid,16) && LUX_IS_ALIGNED(sarray0->data.tvoid,16) &&
    LUX_IS_ALIGNED(arg,16))
  {
    if (sarray->data.tvoid == sarray0->data.tvoid){
      l_FTransform0SSE[op](*sarray,arg);
    }else{
      l_FTransform1SSE[op](*sarray,*sarray0,arg);
    }
  }
#endif

  if (sarray->data.tvoid == sarray0->data.tvoid){
    l_FTransform0[(sarray->vectordim - 1) + (op * 3)](*sarray,arg);
  }else{
    l_FTransform1[(sarray->vectordim - 1) + (op * 3)](*sarray,*sarray0,arg);
  }
  
  return LUX_FALSE;
}



typedef float (LUX_FASTCALL TFScalarArray_relLength_fn)(lxScalarArray_t &sOut, lxScalarArray_t &sIn);

template <class Tout, class Tin, int OUTVECSIZE, int INVECSIZE>
float LUX_FASTCALL TFScalarArray_relLength(lxScalarArray_t &sOut, lxScalarArray_t &sIn)
{
  Tout* LUX_RESTRICT pOut = (Tout*)sOut.data.tfloat;
  Tin*  LUX_RESTRICT pIn  = (Tin*)sIn.data.tfloat;

  size_t ostride = sOut.stride / OUTVECSIZE;
  size_t istride = sIn.stride / INVECSIZE;
  size_t count = LUX_MIN(sOut.count,sIn.count);


  float length = 0.0f;

  if (ostride == 1 && istride == 1){
    size_t unrollcnt = ((count-1)/8);

    pOut[0].Set(0.0f);
    pOut++;
    pIn++;
    for (size_t i = 0; i < unrollcnt; i++, pOut += 8, pIn += 8){
      length += pIn[0].Distance(pIn[-1]);
      pOut[0].Set(length);

      length += pIn[1].Distance(pIn[0]);
      pOut[1].Set(length);

      length += pIn[2].Distance(pIn[1]);
      pOut[2].Set(length);

      length += pIn[3].Distance(pIn[2]);
      pOut[3].Set(length);

      length += pIn[4].Distance(pIn[3]);
      pOut[4].Set(length);

      length += pIn[5].Distance(pIn[4]);
      pOut[5].Set(length);

      length += pIn[6].Distance(pIn[5]);
      pOut[6].Set(length);

      length += pIn[7].Distance(pIn[6]);
      pOut[7].Set(length);
    }
    for (size_t i = unrollcnt*8 +1; i < count; i++, pOut ++, pIn++){
      length += pIn[0].Distance(pIn[-1]);
      pOut[0].Set(length);
    }


    pOut = (Tout*)sOut.data.tfloat;
    float invlength = 1.0f/length;

    for (size_t i = 0; i < unrollcnt; i++, pOut += 8){
      pOut[0] *= invlength;
      pOut[1] *= invlength;
      pOut[2] *= invlength;
      pOut[3] *= invlength;
      pOut[4] *= invlength;
      pOut[5] *= invlength;
      pOut[6] *= invlength;
      pOut[7] *= invlength;
    }
    for (size_t i = unrollcnt*8 +1; i < count; i++, pOut ++){
      pOut[0] *= invlength;
    }

  }
  else{
    pOut[0].Set(0.0f);
    pOut += ostride;
    pIn += istride;
    for (size_t i = 1; i < count; i++, pOut += ostride, pIn += istride){
      Tin* LUX_RESTRICT pInLast = pIn - istride;
      length += pIn[0].Distance(pInLast[0]);
      pOut[0].Set(length);
    }

    pOut = (Tout*)sOut.data.tfloat;
    float invlength = 1.0f/length;
    for (size_t i = 1; i < count; i++, pOut += ostride){
      pOut[0] *= invlength;
    }
  }

  return length;
}

#ifdef SCALAR_USE_XMM
static TFScalarArray_relLength_fn *l_FRelLengthSSE[4] = {
  TFScalarArray_relLength<FVector1,FVector4SSE,1,4>,
  TFScalarArray_relLength<FVector1,FVector4SSE,1,4>,
  TFScalarArray_relLength<FVector1,FVector4SSE,1,4>,
  TFScalarArray_relLength<FVector1,FVector4SSE,1,4>,
};
#endif

static TFScalarArray_relLength_fn *l_FRelLength[4] = {
  TFScalarArray_relLength<FVector1,FVector1,1,1>,
  TFScalarArray_relLength<FVector1,FVector2,1,2>,
  TFScalarArray_relLength<FVector1,FVector3,1,3>,
  TFScalarArray_relLength<FVector1,FVector4,1,4>,
};

LUX_API booln lxFScalarArray_relLength(lxScalarArray_t *sarray, lxScalarArray_t *sarray0, float *outlength)
{
  if (sarray->type != LUX_SCALAR_FLOAT32 || sarray->type != sarray0->type || 
    sarray0->count < 2 || sarray->count < 2 || sarray->vectordim != 1)
  {
    return LUX_TRUE;
  }

  LUX_ASSERT(sarray0->vectordim < 5);

  LUX_ASSUME(sarray0->vectordim < 5);
#ifdef SCALAR_USE_XMM
  if (sarray0->vectordim == 4 && sarray0->stride % 4 == 0 &&
    LUX_IS_ALIGNED(sarray0->data.tvoid,16) )
  {
    float len = l_FRelLengthSSE[sarray0->vectordim](*sarray,*sarray0);
    if (outlength) *outlength = len;
    return LUX_FALSE;
  }
#endif

  float len = l_FRelLength[sarray0->vectordim](*sarray,*sarray0);
  if (outlength) *outlength = len;

  return LUX_FALSE;
}