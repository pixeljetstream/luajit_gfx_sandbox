// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/geometry.h>
#include <luxinia/luxmath/vector3.h>
#include <luxinia/luxmath/vector4.h>
#include <luxinia/luxmath/matrix44.h>

LUX_API void lxPlaneSet
(lxPlanePTR plane, const lxVector3 a, const lxVector3 b, const lxVector3 c)
{
  lxVector3 toB;
  lxVector3 toC;

  lxVector3Sub(toB,b,a);
  lxVector3Sub(toC,c,a);
  lxVector3Cross(plane,toB,toC);
  lxVector3Normalized(plane);
  plane[3] = -lxVector3Dot(plane,a);
}

LUX_API void lxPlaneIntersect
(lxVector3 out, lxPlaneCPTR planea,lxPlaneCPTR planeb,lxPlaneCPTR planec)
{
  // http://local.wasp.uwa.edu.au/~pbourke/geometry/3planes/
  lxVector3 da;
  lxVector3 db;
  lxVector3 dc;

  lxVector3Cross(da,planec,planeb);
  lxVector3Cross(db,planea,planec);
  lxVector3Cross(dc,planeb,planea);

  lxVector3Scale(dc,dc,-planec[3]);
  lxVector3ScaledAdd(dc,dc,-planeb[3],db);
  lxVector3ScaledAdd(dc,dc,-planea[3],da);

  lxVector3Scale(out,dc,1.0f/(lxVector3Dot(planea,da)));
}

LUX_API void lxPlaneVecCheckLine
(lxVector3 intersectpoint, const lxVector3 planenormal, const lxVector3 planepoint, const lxVector3 raystart, const lxVector3 rayend)
{
  lxVector3 dir;
  float ratio;
  float distp = lxVector3Dot(planenormal,planepoint);
  float diststart = lxVector3Dot(planenormal,raystart);
  float distend = lxVector3Dot(planenormal,rayend);

  ratio = (distp-diststart)/(distend-diststart);
  ratio = LUX_MIN(ratio,1.0f);
  ratio = LUX_MAX(ratio,0.0f);
  lxVector3Sub(dir,rayend,raystart);
  lxVector3ScaledAdd(intersectpoint,raystart,ratio,dir);
}

LUX_API float lxTriangleArea
( const lxVector3 v0, const lxVector3 v1, const lxVector3 v2)
{
  lxVector3 a,b,cross;
  lxVector3Sub(a,v1,v0);
  lxVector3Sub(b,v2,v0);
  lxVector3Cross(cross,a,b);

  return lxVector3LengthFast(cross)*0.5f;
}
