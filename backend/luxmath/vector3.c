// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/vector3.h>
#include <luxinia/luxmath/misc.h>

LUX_API float lxVector3LineDistanceSq(const lxVector3 point,const lxVector3 linestart,const lxVector3 linedir)
{
  lxVector3 topoint;
  float fracc;
  float sqlen;

  lxVector3Sub(topoint,point,linestart);

  fracc = lxVector3Dot(linedir,topoint);

  if (fracc > 0){
    sqlen = lxVector3SqLength(linedir);
    if (fracc >= sqlen)
      lxVector3Sub(topoint,topoint,linedir);
    else{
      fracc /= sqlen;
      lxVector3ScaledAdd(topoint,topoint,-fracc,linedir);
    }
  }

  return lxVector3SqLength(topoint);
}
LUX_API float lxVector3LineDistanceSqFracc(const lxVector3 point,const lxVector3 linestart, const lxVector3 linedir, float *ofracc)
{
  lxVector3 topoint;
  float fracc;
  float sqlen;

  lxVector3Sub(topoint,point,linestart);

  fracc = lxVector3Dot(linedir,topoint);

  if (fracc > 0){
    sqlen = lxVector3SqLength(linedir);
    if (fracc >= sqlen)
      lxVector3Sub(topoint,topoint,linedir);
    else{
      fracc /= sqlen;
      lxVector3ScaledAdd(topoint,topoint,-fracc,linedir);
    }
  }
  *ofracc = LUX_MIN(1,LUX_MAX(0,fracc));

  return lxVector3SqLength(topoint);
}

LUX_API void lxVector3SpreadFast(lxVector3 out, lxVector3 in, const float radin, const float radout){
  lxVector3 forward;
  lxVector3 right;
  lxVector3 up;
  float length;
  float spread;
  float angle;
  uchar flip;

  flip = LUX_FALSE;
  lxVector3Copy(forward,in);
  length = lxVector3NormalizedFast(forward);
  lxVector3PerpendicularFast(right,forward);
  lxVector3Cross(up,forward,right);

  spread = radin + lxFrand()*(radout-radin);
  if (spread > LUX_MUL_HALF_PI)
    length = -length;
  spread = lxFastSin(spread)/lxFastCos(spread); // (float)tan (spread);
  angle = lxFrand()*LUX_MUL_TWOPI;

  lxVector3Copy(out,forward);
  lxVector3ScaledAdd(out,out,spread*lxFastSin(angle),right);
  lxVector3ScaledAdd(out,out,spread*lxFastCos(angle),up);

  lxVector3NormalizedFast(out);
  lxVector3Scale(out,out,length);
}

LUX_API void lxVector3PerpendicularFast(lxVector3 out, lxVector3 in)
{ // creates a random normalized vector which is perpendicular to in
  lxVector3 nin;
  lxVector3 x = {1.0f,0.0f,0.0f};
  lxVector3 y = {0.0f,1.0f,0.0f};
  float rdot;
  // ~25° tolerance
  lxVector3NormalizeFast(nin,in);
  rdot = lxVector3Dot(x,nin);
  if (LUX_FP_ABS_BITS(rdot) < 0x3F666666){
    lxVector3Cross(out,x,nin);
  }
  else{
    /*
    rdot = Vector3Dot(z,nin);
    if (LUX_FP_ABS_BITS(rdot) < 0x3F666666){
      Vector3Cross(out,z,nin);
    }
    else{
      Vector3Cross(out,y,nin);
    }
    */
    lxVector3Cross(out,y,nin);
  }
  lxVector3NormalizedFast(out);
}

LUX_API void lxVector3Perpendicular(lxVector3 out, lxVector3 in)
{ // creates a random normalized vector which is perpendicular to in
  lxVector3 nin;
  lxVector3 x = {1.0f,0.0f,0.0f};
  lxVector3 y = {0.0f,1.0f,0.0f};
  lxVector3 z = {0.0f,0.0f,1.0f};
  float rdot;
  // ~25° tolerance
  lxVector3Normalize(nin,in);
  rdot = lxVector3Dot(x,nin);
  if (LUX_FP_ABS_BITS(rdot) < 0x3F666666){
    lxVector3Cross(out,x,nin);
  }
  else{
    rdot = lxVector3Dot(z,nin);
    if (LUX_FP_ABS_BITS(rdot) < 0x3F666666){
      lxVector3Cross(out,z,nin);
    }
    else{
      lxVector3Cross(out,y,nin);
    }
  }
  lxVector3Normalized(out);
}

LUX_API void lxVector3MinMax(lxVector3 min, lxVector3 max, lxVector3 pos)
{
  min[0] = LUX_MIN(min[0],pos[0]);
  min[1] = LUX_MIN(min[1],pos[1]);
  min[2] = LUX_MIN(min[2],pos[2]);

  max[0] = LUX_MAX(max[0],pos[0]);
  max[1] = LUX_MAX(max[1],pos[1]);
  max[2] = LUX_MAX(max[2],pos[2]);
}
