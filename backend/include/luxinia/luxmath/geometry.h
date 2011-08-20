// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_GEOMETRY_H__
#define __LUXMATH_GEOMETRY_H__

#include <luxinia/luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif

LUX_API void lxPlaneSet(lxPlanePTR plane, const lxVector3 a, const lxVector3 b, const lxVector3 c);

LUX_API void lxPlaneIntersect(lxVector3 out, lxPlaneCPTR planea,lxPlaneCPTR planeb,lxPlaneCPTR planec);

LUX_API void lxPlaneVecCheckLine(lxVector3 intersectpoint, const lxVector3 planenormal, 
          const lxVector3 planepoint, const lxVector3 raystart, const lxVector3 rayend);

LUX_API float lxTriangleArea( const lxVector3 v0, const lxVector3 v1, const lxVector3 v2);

#ifdef __cplusplus
}
#endif

#endif
