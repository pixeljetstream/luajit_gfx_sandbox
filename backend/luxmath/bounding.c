// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/bounding.h>
#include <luxinia/luxmath/vector3.h>

// Bounding Volumes
// ----------------


LUX_API booln lxBoundingBox_mergeChange(lxBoundingBoxPTR out, lxBoundingBoxCPTR a, lxBoundingBoxCPTR b)
{
  int change = LUX_FALSE;
  float temp;
  temp = LUX_MIN(a->min[0], b->min[0]);
  change |= (a->min[0] != temp);
  out->min[0] = temp;
  temp = LUX_MIN(a->min[1], b->min[1]);
  change |= (a->min[1] != temp);
  out->min[1] = temp;
  temp = LUX_MIN(a->min[2], b->min[2]);
  change |= (a->min[2] != temp);
  out->min[2] = temp;

  temp = LUX_MAX(a->max[0], b->max[0]);
  change |= (a->max[0] != temp);
  out->max[0] = temp;
  temp = LUX_MAX(a->max[1], b->max[1]);
  change |= (a->max[1] != temp);
  out->max[1] = temp;
  temp = LUX_MAX(a->max[2], b->max[2]);
  change |= (a->max[2] != temp);
  out->max[2] = temp;

  return change;
}

LUX_API void  lxBoundingBox_toCenterBox(lxBoundingBoxCPTR box, lxBoundingBoxCenterPTR ctr)
{
  lxVector3Sub(ctr->size,box->max, box->min);
  lxVector3ScaledAdd(ctr->center,box->min,0.5f,ctr->size);
}

LUX_API void  lxBoundingBox_fromCenterBox(lxBoundingBoxPTR box, lxBoundingBoxCenterCPTR ctr)
{
  lxVector3ScaledAdd(box->min,ctr->center,-0.5f,ctr->size);
  lxVector3ScaledAdd(box->max,ctr->center, 0.5f,ctr->size);
}

LUX_API booln lxBoundingSphere_mergeChange(lxBoundingSpherePTR out, lxBoundingSphereCPTR a, lxBoundingSphereCPTR b)
{
  lxVector3 vec;
  float rad;

  // if one sphere has a radius of 0 return the other sphere
  if(a->radius == 0){
    lxBoundingSphere_copy(out, b);
    return LUX_TRUE;
  }
  if(b->radius == 0){
    lxBoundingSphere_copy(out, a);
    return LUX_FALSE;
  }

  // get vector from a->center to b->center
  lxVector3Sub(vec,b->center, a->center);

  // get distance of the two centers and normalize the vector
  rad = lxVector3Normalized(vec);

  // if one sphere is in the other return the larger sphere
  if(rad + LUX_MIN(a->radius, b->radius) <= LUX_MAX(a->radius, b->radius))
  {
    if(a->radius > b->radius){
      lxBoundingSphere_copy(out, a);
      return LUX_FALSE;
    }
    else{
      lxBoundingSphere_copy(out, b);
      return LUX_TRUE;
    }
  }

  // calculate new radius
  rad += a->radius + b->radius;
  rad *= .5;

  // calculate new center
  lxVector3Scale(vec,vec, rad-a->radius);
  lxVector3Add(vec,vec, a->center);

  // copy the new sphere
  lxVector3Copy(out->center, vec);
  out->radius = rad;

  return LUX_TRUE;
}

LUX_API void lxBoundingBox_toSphereV(const lxVector3 min,const lxVector3 max, lxVector3 center, float *radius)
{
  // center = (max-min)/2 + min
  lxVector3Sub(center,max, min);
  center[0] *= 0.5f;
  center[1] *= 0.5f;
  center[2] *= 0.5f;

  // radius = len((max-min)/2)
  *radius = lxVector3LengthFast(center);

  lxVector3Add( center,center, min);
}

LUX_API lxBoundingSpherePTR lxBoundingBox_toSphere(lxBoundingBoxCPTR bbox, lxBoundingSpherePTR sphere)
{
  lxBoundingBox_toSphereV(bbox->min, bbox->max, sphere->center, &sphere->radius);
  return sphere;
}



LUX_API lxBoundingCapsulePTR lxBoundingBox_toCapsule(lxBoundingBoxCPTR bbox, lxBoundingCapsulePTR capsule)
{
  lxVector3 center;
  lxVector3 length;
  int   minaxis = 0;
  float t;
  float mindim;
  lxBoundingBox_toCenter(bbox, center, length);
  mindim = length[0];

  if (length[1] < mindim){
    mindim = length[1];
    minaxis = 1;
  }
  if (length[2] < mindim){
    mindim = length[2];
    minaxis = 2;
  }

  // pick the greater dimension of the other 2 axis as radius
  mindim = length[(minaxis+1)%3];
  mindim*=0.5f;
  t = length[(minaxis+2)%3];
  t*=0.5f;
  capsule->radius = (float)sqrt(mindim*mindim + t*t);
  lxVector3Clear(capsule->toEnd);
  // length of line segment is "length - 2 * radius"
  capsule->toEnd[minaxis] = length[minaxis];

  //capsule->radius*=0.5f;
  capsule->radiusSqr = capsule->radius*capsule->radius;

  // create start point
  lxVector3Copy(capsule->origin,center);
  capsule->origin[minaxis] = capsule->origin[minaxis]- length[minaxis]*0.5f;

  return capsule;
}



LUX_API void lxBoundingBox_transformBoxCorners(lxBoundingBoxCPTR in, lxMatrix44CPTR trans, lxVector3 box[8])
{
  int i;
  // create box corners
  lxVector3Copy(box[0],in->min);
  lxVector3Copy(box[1],in->min);
  box[1][0] = in->max[0];
  lxVector3Copy(box[2],box[0]);
  lxVector3Copy(box[3],box[1]);
  box[2][1] = box[3][1] = in->max[1];
  lxVector3Copy(box[4],in->max);
  lxVector3Copy(box[5],in->max);
  box[5][0] = in->min[0];
  lxVector3Copy(box[6],box[4]);
  lxVector3Copy(box[7],box[5]);
  box[6][1] = box[7][1] = in->min[1];

  for (i = 0; i < 8 ; i++)
    lxVector3Transform1(box[i],trans);
}

LUX_API void lxBoundingBox_transformV(lxVector3 outmins, lxVector3 outmaxs,const lxVector3 mins, const lxVector3 maxs, lxMatrix44CPTR trans)
{
  int i;
  lxVector3 test;
  lxVector3 box[8];
  // create box corners
  lxVector3Copy(box[0],mins);
  lxVector3Copy(box[1],mins);
  box[1][0] = maxs[0];
  lxVector3Copy(box[2],box[0]);
  lxVector3Copy(box[3],box[1]);
  box[2][1] = box[3][1] = maxs[1];
  lxVector3Copy(box[4],maxs);
  lxVector3Copy(box[5],maxs);
  box[5][0] = mins[0];
  lxVector3Copy(box[6],box[4]);
  lxVector3Copy(box[7],box[5]);
  box[6][1] = box[7][1] = mins[1];
  // transform box corners
  // and find new mins,maxs

  lxVector3Transform(test,box[0],trans);
  lxVector3Set(outmins,test[0],test[1],test[2]);
  lxVector3Set(outmaxs,test[0],test[1],test[2]);
  for (i = 1; i < 8 ; i++){
    lxVector3Transform(test,box[i],trans);

    outmins[0] = LUX_MIN(outmins[0],test[0]);
    outmins[1] = LUX_MIN(outmins[1],test[1]);
    outmins[2] = LUX_MIN(outmins[2],test[2]);

    outmaxs[0] = LUX_MAX(outmaxs[0], test[0]);
    outmaxs[1] = LUX_MAX(outmaxs[1], test[1]);
    outmaxs[2] = LUX_MAX(outmaxs[2], test[2]);
  }
}

LUX_API lxBoundingBoxPTR lxBoundingBox_transform(lxBoundingBoxPTR out, lxBoundingBoxCPTR in, lxMatrix44CPTR trans)
{
  lxBoundingBox_transformV(out->min,out->max,in->min, in->max, trans);
  return out;
}

LUX_API void lxBoundingBox_fromCorners(lxBoundingBoxPTR bbox,const lxVector3 vecs[8])
{
  const float *test;
  int i;

  test = vecs[0];
  lxVector3Set(bbox->min,test[0],test[1],test[2]);
  lxVector3Set(bbox->max,test[0],test[1],test[2]);

  for (i = 1; i < 8 ; i++){
    test = vecs[i];
    bbox->min[0] = LUX_MIN(bbox->min[0],test[0]);
    bbox->min[1] = LUX_MIN(bbox->min[1],test[1]);
    bbox->min[2] = LUX_MIN(bbox->min[2],test[2]);

    bbox->max[0] = LUX_MAX(bbox->max[0], test[0]);
    bbox->max[1] = LUX_MAX(bbox->max[1], test[1]);
    bbox->max[2] = LUX_MAX(bbox->max[2], test[2]);
  }
}

LUX_API void lxBoundingCorners_fromCamera(lxVector3 box[8],lxMatrix44CPTR mat,const float fov, const float frontplane, const float backplane, const float aspect)
{
  int i;
  float invaspect = 1.0f/aspect;
  if (fov < 0.0f){
    lxVector3Set(box[0],fov*aspect*0.5f,frontplane,fov*invaspect*0.5f);
    lxVector3Set(box[1],-box[0][0],box[0][1],box[0][2]);
    lxVector3Set(box[2],-box[0][0],box[0][1],-box[0][2]);
    lxVector3Set(box[3],box[0][0],box[0][1],-box[0][2]);

    lxVector3Set(box[4],box[0][0],backplane,box[0][2]);
    lxVector3Set(box[5],-box[4][0],box[4][1],box[4][2]);
    lxVector3Set(box[6],-box[4][0],box[4][1],-box[4][2]);
    lxVector3Set(box[7],box[4][0],box[4][1],-box[4][2]);
  }
  else{
    float tfov = (float)tan(LUX_DEG2RAD(fov*0.5f));
    float tifov = (float)tan(LUX_DEG2RAD(fov*invaspect*0.5f));

    lxVector3Set(box[0],tfov*frontplane,frontplane,tifov*frontplane);
    lxVector3Set(box[1],-box[0][0],box[0][1],box[0][2]);
    lxVector3Set(box[2],-box[0][0],box[0][1],-box[0][2]);
    lxVector3Set(box[3],box[0][0],box[0][1],-box[0][2]);

    lxVector3Set(box[4],tfov*backplane,backplane,tifov*backplane);
    lxVector3Set(box[5],-box[4][0],box[4][1],box[4][2]);
    lxVector3Set(box[6],-box[4][0],box[4][1],-box[4][2]);
    lxVector3Set(box[7],box[4][0],box[4][1],-box[4][2]);
  }


  for (i=0; i < 8; i++){
    lxVector3Transform1(box[i],mat);
  }
}



LUX_API booln lxBoundingCone_checkSphere(lxBoundingConeCPTR cone, lxBoundingSphereCPTR sphere)
{
  lxVector3 d;

  float radius = sphere->radius;
  float radiusSqr = radius * radius;
  float flt = radius * cone->sinDiv;
  float e;
  

  lxVector3ScaledAdd(d,cone->top,-flt,cone->axis);
  lxVector3Sub(d,sphere->center,d);

  flt = lxVector3Dot(d,d);
  e = lxVector3Dot(cone->axis,d);
  if ( e > 0 && e*e >= flt*cone->cosSqr )
  {
    lxVector3Sub(d,sphere->center,cone->top);
    flt = lxVector3Dot(d,d);
    e = -lxVector3Dot(cone->axis,d);
    if ( e > 0 && e*e >= flt*cone->sinSqr)
      return (flt <= radiusSqr);
    else
      return LUX_TRUE;
  }
  return LUX_FALSE;
}

LUX_API void lxBoundingSphereCone_fromCamera(lxBoundingSpherePTR sphere, lxBoundingConePTR cone,float frontplane, float backplane, const lxVector3 pos, const lxVector3 dir, float fov){
  lxVector3 p,q,diff;
  float fFov = LUX_DEG2RAD(fov);
  float fViewLen = backplane - frontplane;
  // use some trig to find the height of the frustum at the far plane
  float fHeight = fViewLen * (float)tan(fFov * 0.5f);
  // with an aspect ratio of 1, the width will be the same
  float fWidth = fHeight;
  float fDepth  = fHeight / (float)tan(fFov * 0.5f);

  // calculate the corner of the screen
  float fCorner = lxFastSqrt(fWidth * fWidth + fHeight * fHeight);

  // now calculate the new fov
  fFov = (float)atan(fCorner / fDepth);

  // halfway point between near/far planes starting at the origin and extending along the z axis
  lxVector3Set (p,0.0f, 0.0f, -(frontplane + fViewLen * 0.5f));

  // the calculate far corner of the frustum
  lxVector3Set (q,fWidth, fHeight, fViewLen);

  // the vector between P and Q
  lxVector3Sub(diff,p,q);

  // the radius becomes the length of this vector
  if (fov < 0.0f)
    sphere->radius = (-fov)*2.0f;
  else
    sphere->radius = lxVector3LengthFast(diff);

  // calculate the center of the sphere
  fViewLen = fViewLen * 0.5f + frontplane;
  lxVector3ScaledAdd(sphere->center,pos,fViewLen,dir);

  // apply to the cone
  lxVector3Copy(cone->axis,dir);
  lxVector3Copy(cone->top,pos);
  cone->cosSqr = cosf(fFov);
  cone->cosSqr *= cone->cosSqr;
  cone->sinSqr = sinf(fFov);
  cone->sinDiv = 1/cone->sinSqr;
  cone->sinSqr *= cone->sinSqr;
}

LUX_API void lxBoundingSphere_fromFrustumCorners(lxBoundingSpherePTR sphere, const lxVector3 box[LUX_FRUSTUM_CORNERS])
{
  lxVector3 ctr = {0.0f,0.0f,0.0f};
  float rad = 0.0f;
  int i;
  for (i = 0; i < LUX_FRUSTUM_CORNERS; i++){
    lxVector3Add(ctr,ctr,box[i]);
  }
  lxVector3Scale(ctr,ctr,1.0f/((float)LUX_FRUSTUM_CORNERS));

  for (i = 0; i < LUX_FRUSTUM_CORNERS; i++){
    lxVector3 temp;
    float cur;
    lxVector3Sub(temp,ctr,box[i]);
    cur = lxVector3Dot(temp,temp);
    rad = LUX_MAX(rad,cur);
  }

  lxVector3Copy(sphere->center,ctr);
  sphere->radius = sqrtf(rad);
}

LUX_API void lxBoundingCone_fromFrustumCorners(lxBoundingConePTR cone, const lxVector3 box[LUX_FRUSTUM_CORNERS])
{
  lxVector3 near;
  lxVector3 far;
  lxVector3 temp;
  lxVector3 ctr;
  float radn = 0.0f;
  float radf = 0.0f;
  float rad = 0.0f;
  int i;

  lxVector3Add(near,box[LUX_FRUSTUM_C_NTR],box[LUX_FRUSTUM_C_NTL]);
  lxVector3Add(near,near,box[LUX_FRUSTUM_C_NBL]);
  lxVector3Add(near,near,box[LUX_FRUSTUM_C_NBR]);
  lxVector3Scale(near,near,0.25f);

  lxVector3Add(far,box[LUX_FRUSTUM_C_FTR],box[LUX_FRUSTUM_C_FTL]);
  lxVector3Add(far,far,box[LUX_FRUSTUM_C_FBL]);
  lxVector3Add(far,far,box[LUX_FRUSTUM_C_FBR]);
  lxVector3Scale(far,far,0.25f);

  for (i = 0; i < 4; i++){
    float distsq;
    lxVector3Sub(temp,near,box[LUX_FRUSTUM_C_NTR+i]);
    distsq = lxVector3Dot(temp,temp);
    radn = LUX_MAX(radn,distsq);

    lxVector3Sub(temp,far,box[LUX_FRUSTUM_C_FTR+i]);
    distsq = lxVector3Dot(temp,temp);
    radf = LUX_MAX(radf,distsq);
  }

  lxVector3Sub(temp,near,far);
  radn = sqrtf(radn);
  radf = sqrtf(radf);
  rad = radn/radf;

  if (rad < 0.95f){
    lxVector3Scale(ctr,temp,1.0f/(1.0f-rad));
    rad = lxVector3Length(ctr);
    lxVector3Add(cone->top,ctr,far);
    lxVector3Scale(cone->axis,ctr,-1.0f/rad);
    rad = (float)atan(radf/rad)*2.0f;
    radf = cosf(rad);
    cone->cosSqr = radf*radf;
    radf = sinf(rad);
    cone->sinSqr = radf*radf;
    cone->sinDiv = 1.0f/radf;
  }
  else{
    cone->cosSqr = 1.0f;
    cone->sinDiv = 1.0f;
    cone->sinSqr = 0.0f;
  }
}


