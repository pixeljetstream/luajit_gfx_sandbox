// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt



#include <luxmath/frustum.h>
#include <luxmath/geometry.h>
#include <luxmath/vector3.h>
#include <luxmath/matrix44.h>

LUX_API void lxFrustum_update(lxFrustumPTR pFrustum,const lxMatrix44PTR viewproj)
{
  const float *clip = viewproj;
  float magnitude;
  int   side;


  // Now we actually want to get the sides of the pFrustum->planes.  To do this we take
  // the clipping planes we received above and extract the sides from them.


  pFrustum->fplanes[FRUSTUM_RIGHT].pvec[0] = clip[3] - clip[0];
  pFrustum->fplanes[FRUSTUM_RIGHT].pvec[1] = clip[7] - clip[4];
  pFrustum->fplanes[FRUSTUM_RIGHT].pvec[2] = clip[11] - clip[8];
  pFrustum->fplanes[FRUSTUM_RIGHT].pvec[3] = clip[15] - clip[12];

  pFrustum->fplanes[FRUSTUM_LEFT].pvec[0] = clip[3] + clip[0];
  pFrustum->fplanes[FRUSTUM_LEFT].pvec[1] = clip[7] + clip[4];
  pFrustum->fplanes[FRUSTUM_LEFT].pvec[2] = clip[11] + clip[8];
  pFrustum->fplanes[FRUSTUM_LEFT].pvec[3] = clip[15] + clip[12];

  pFrustum->fplanes[FRUSTUM_BOTTOM].pvec[0] = clip[3] + clip[1];
  pFrustum->fplanes[FRUSTUM_BOTTOM].pvec[1] = clip[7] + clip[5];
  pFrustum->fplanes[FRUSTUM_BOTTOM].pvec[2] = clip[11] + clip[9];
  pFrustum->fplanes[FRUSTUM_BOTTOM].pvec[3] = clip[15] + clip[13];
  
  pFrustum->fplanes[FRUSTUM_TOP].pvec[0] = clip[3] - clip[1];
  pFrustum->fplanes[FRUSTUM_TOP].pvec[1] = clip[7] - clip[5];
  pFrustum->fplanes[FRUSTUM_TOP].pvec[2] = clip[11] - clip[9];
  pFrustum->fplanes[FRUSTUM_TOP].pvec[3] = clip[15] - clip[13];

  pFrustum->fplanes[FRUSTUM_FAR].pvec[0] = clip[3] - clip[2];
  pFrustum->fplanes[FRUSTUM_FAR].pvec[1] = clip[7] - clip[6];
  pFrustum->fplanes[FRUSTUM_FAR].pvec[2] = clip[11] - clip[10];
  pFrustum->fplanes[FRUSTUM_FAR].pvec[3] = clip[15] - clip[14];

  pFrustum->fplanes[FRUSTUM_NEAR].pvec[0] = clip[3] + clip[2];
  pFrustum->fplanes[FRUSTUM_NEAR].pvec[1] = clip[7] + clip[6];
  pFrustum->fplanes[FRUSTUM_NEAR].pvec[2] = clip[11] + clip[10];
  pFrustum->fplanes[FRUSTUM_NEAR].pvec[3] = clip[15] + clip[14];

  for (side = 0; side < FRUSTUM_PLANES; side++){
    lxFrustumPlanePTR fplane = &pFrustum->fplanes[side];
    float *vec = pFrustum->fplanes[side].pvec;
    int i;

    magnitude = 1.0f/lxVector3LengthA(vec);

    // Then we divide the plane's values by it's magnitude.
    // This makes it easier to work with.
    vec[0] *= magnitude;
    vec[1] *= magnitude;
    vec[2] *= magnitude;
    vec[3] *= magnitude;

    for (i = 0; i < 3; i++){
      if (vec[i] >= 0.0f){
        fplane->p[i] = 3+i; // max
        fplane->n[i] = 0+i; // min
      }
      else{
        fplane->p[i] = 0+i; // min
        fplane->n[i] = 3+i; // max
      }
    }
  }
}

LUX_API void lxFrustum_updateSigns(lxFrustumPTR frustum)
{
  int side;
  for (side = 0; side < FRUSTUM_PLANES; side++){
    lxFrustumPlanePTR fplane = &frustum->fplanes[side];
    float *vec = frustum->fplanes[side].pvec;
    int i;

    for (i = 0; i < 3; i++){
      if (vec[i] >= 0.0f){
        fplane->p[i] = 3+i; // max
        fplane->n[i] = 0+i; // min
      }
      else{
        fplane->p[i] = 0+i; // min
        fplane->n[i] = 3+i; // max
      }
    }
  }
}

#if 0
booln Frustum_checkPointCoherent(const FrustumPTR pFrustum,const Vector3 vec, int *clipplane)
{
  int i,skip;

  skip = (*clipplane);
  if(pFrustum->fplanes[skip].pvec[0] * vec[0] + pFrustum->fplanes[skip].pvec[1] * vec[1] + pFrustum->fplanes[skip].pvec[2] * vec[2] + pFrustum->fplanes[skip].pvec[3] <= 0)
  {
    return TRUE;
  }
  for(i = 0; i < FRUSTUM_PLANES; i++ )
  {
    if(i != skip && pFrustum->fplanes[i].pvec[0] * vec[0] + pFrustum->fplanes[i].pvec[1] * vec[1] + pFrustum->fplanes[i].pvec[2] * vec[2] + pFrustum->fplanes[i].pvec[3] <= 0)
    {
      *clipplane = i;
      return TRUE;
    }
  }

  return FALSE;
}
booln Frustum_checkSphereCoherent(const FrustumPTR pFrustum,const Vector3 vec,const float radius, int *clipplane)
{
  int i,skip;

  skip = *clipplane;
  if(pFrustum->fplanes[skip].pvec[0] * vec[0] + pFrustum->fplanes[skip].pvec[1] * vec[1] + pFrustum->fplanes[skip].pvec[2] * vec[2] + pFrustum->fplanes[skip].pvec[3] <= -radius)
  {
    return TRUE;
  }

  for( i = 0; i <FRUSTUM_PLANES; i++ )
  {
    if(i != skip &&  pFrustum->fplanes[i].pvec[0] * vec[0] + pFrustum->fplanes[i].pvec[1] * vec[1] + pFrustum->fplanes[i].pvec[2] * vec[2] + pFrustum->fplanes[i].pvec[3] <= -radius )
    {
      *clipplane = i;
      return TRUE;
    }
  }
  return FALSE;
}


booln   Frustum_checkAABBvCoherent(const FrustumPTR pFrustum,const float minmax[6], int *clipplane)
{
  int i,skip;
  skip = *clipplane;
  {
    const FrustumPlanePTR sp = &pFrustum->fplanes[skip];
    if ((sp->pvec[0] * minmax[sp->px]) + 
      (sp->pvec[1] * minmax[sp->py]) + 
      (sp->pvec[2] * minmax[sp->pz]) + sp->pvec[3] < 0)
    {
      return TRUE;
    }
  }


  for (i = 0; i < FRUSTUM_PLANES; i++){
    if (i != skip){

      const FrustumPlanePTR sp = &pFrustum->fplanes[i];
      if ((sp->pvec[0] * minmax[sp->px]) + 
        (sp->pvec[1] * minmax[sp->py]) + 
        (sp->pvec[2] * minmax[sp->pz]) + sp->pvec[3] < 0)
      {
        *clipplane = i;
        return TRUE;
      }
    }
  }

  return FALSE;
}

#endif

LUX_API booln  lxFrustum_checkSphereFull(const lxFrustumPTR pFrustum, const lxVector3 center, const float radius)
{
  int i;

  for( i = 0; i < FRUSTUM_PLANES; i++ )
  {
    if(pFrustum->fplanes[i].pvec[0] * center[0] + pFrustum->fplanes[i].pvec[1] * center[1] + pFrustum->fplanes[i].pvec[2] * center[2] + pFrustum->fplanes[i].pvec[3] <= radius )
    {
      return LUX_TRUE;
    }
  }
  return LUX_FALSE;
}

// returns 0 if all points are outside of one of the planes
// 1 if fully inside, -1 if intersects
lxCullType_t   lxFrustum_cullPoints(const lxFrustumPTR pFrustum, const lxVector4 *vecarray, const int numVec)
{
  const float *vec;
  int i,n;
  int inside;
  int allinside;
  int planesinside = 0;

  for(i = 0; i < FRUSTUM_PLANES; i++ )
  {
    allinside = LUX_TRUE;
    inside = numVec;
    // we already check the firstplane
    // Calculate the plane equation and check if the point is behind a side of the pFrustum->planes
    vec = vecarray[0];
    for (n = 0; n < numVec; n++,vec+=4){
      if(pFrustum->fplanes[i].pvec[0] * vec[0] + pFrustum->fplanes[i].pvec[1] * vec[1] + pFrustum->fplanes[i].pvec[2] * vec[2] + pFrustum->fplanes[i].pvec[3] * vec[3] < 0)
      {
        // The point was outside a side
        inside--;
        allinside = LUX_FALSE;
      }
    }
    // all were outside
    if (!inside)
      return CULL_OUTSIDE;

    if (allinside)
      planesinside++;

  }

  return planesinside == 6 ? CULL_INSIDE : CULL_INTERSECT;
}


LUX_API lxCullType_t  lxFrustum_cullAABBv(const lxFrustumPTR pFrustum, const float minmax[6])
{
//#define FRUSTUM_CULLAABB_DEBUG

#ifdef FRUSTUM_CULLAABB_DEBUG
  lxCullType_t res;
  lxVector4   box[8];
  const float *mins = minmax;
  const float *maxs = mins+3;
  // create box corners
  lxVector3Copy(box[0],mins);
  box[0][3] = 1.0f;
  lxVector3Copy(box[1],mins);
  box[1][3] = 1.0f;
  box[1][0] = maxs[0];
  lxVector3Copy(box[2],box[0]);
  box[2][3] = 1.0f;
  lxVector3Copy(box[3],box[1]);
  box[3][3] = 1.0f;
  box[2][1] = box[3][1] = maxs[1];
  lxVector3Copy(box[4],maxs);
  box[4][3] = 1.0f;
  lxVector3Copy(box[5],maxs);
  box[5][0] = mins[0];
  box[5][3] = 1.0f;
  lxVector3Copy(box[6],box[4]);
  box[6][3] = 1.0f;
  lxVector3Copy(box[7],box[5]);
  box[6][1] = box[7][1] = mins[1];
  box[7][3] = 1.0f;

  res = lxFrustum_cullPoints(pFrustum,box,8);
  return res;
#endif
  {
    lxCullType_t result = CULL_INSIDE;
    int i;
    for (i = 0; i < FRUSTUM_PLANES; i++){
      const lxFrustumPlanePTR sp = &pFrustum->fplanes[i];
      if ((sp->pvec[0] * minmax[sp->px]) + 
        (sp->pvec[1] * minmax[sp->py]) + 
        (sp->pvec[2] * minmax[sp->pz]) + sp->pvec[3] < 0)
      {
#ifdef FRUSTUM_CULLAABB_DEBUG
        LUX_ASSERT(CULL_OUTSIDE == res);
#endif
        return CULL_OUTSIDE;
      }

      if ((sp->pvec[0] * minmax[sp->nx]) + 
        (sp->pvec[1] * minmax[sp->ny]) + 
        (sp->pvec[2] * minmax[sp->nz]) + sp->pvec[3] < 0)
      {
        result = CULL_INTERSECT;
      }
    }
#ifdef FRUSTUM_CULLAABB_DEBUG
    LUX_ASSERT(result == res);
#endif
    return result;
  }
}

//////////////////////////////////////////////////////////////////////////
// http://www.cescg.org/CESCG-2002/DSykoraJJelinek/index.html

LUX_API lxCullType_t lxFrustum_cullAABBvMaskedCoherent(const lxFrustumPTR pFrustum, const float minmax[6], int in_mask, int *out_mask, int *inoutstart_id)
{
  int start_id = *inoutstart_id;
  int i, k = 1 << start_id;
  lxCullType_t result = CULL_INSIDE;
  const lxFrustumPlane_t* fplanes = pFrustum->fplanes;
  const lxFrustumPlane_t* sp = fplanes + start_id; 

  *out_mask=0;

  if (k & in_mask) {
    if ((sp->pvec[0] * minmax[sp->px]) + 
      (sp->pvec[1] * minmax[sp->py]) + 
      (sp->pvec[2] * minmax[sp->pz]) + sp->pvec[3] < 0)
      return CULL_OUTSIDE;

    if ((sp->pvec[0] * minmax[sp->nx]) + 
      (sp->pvec[1] * minmax[sp->ny]) + 
      (sp->pvec[2] * minmax[sp->nz]) + sp->pvec[3] < 0)
    { 
      *out_mask |= k; 
      result = CULL_INTERSECT; 
    }

  }

  for (i = 0, k = 1; k <= in_mask; i++, k += k){
    if ((i != start_id) && (k & in_mask)) {
      sp = fplanes + i;
      if ((sp->pvec[0] * minmax[sp->px]) + 
        (sp->pvec[1] * minmax[sp->py]) + 
        (sp->pvec[2] * minmax[sp->pz]) + sp->pvec[3] < 0)
      { 
        *inoutstart_id = i; 
        return CULL_OUTSIDE; 
      }

      if ((sp->pvec[0] * minmax[sp->nx]) + 
        (sp->pvec[1] * minmax[sp->ny]) + 
        (sp->pvec[2] * minmax[sp->nz]) + sp->pvec[3] < 0)
      { 
        *out_mask |= k; 
        result = CULL_INTERSECT; 
      }
    } 
  }

  return result;
}

LUX_API void lxFrustum_getCorners(const lxFrustumPTR pFrustum,lxVector3 box[FRUSTUM_CORNERS]){

  lxPlaneIntersect(box[FRUSTUM_C_NTR],pFrustum->fplanes[FRUSTUM_NEAR].pvec,pFrustum->fplanes[FRUSTUM_TOP].pvec,pFrustum->fplanes[FRUSTUM_RIGHT].pvec);
  lxPlaneIntersect(box[FRUSTUM_C_NTL],pFrustum->fplanes[FRUSTUM_NEAR].pvec,pFrustum->fplanes[FRUSTUM_TOP].pvec,pFrustum->fplanes[FRUSTUM_LEFT].pvec);
  lxPlaneIntersect(box[FRUSTUM_C_NBL],pFrustum->fplanes[FRUSTUM_NEAR].pvec,pFrustum->fplanes[FRUSTUM_BOTTOM].pvec,pFrustum->fplanes[FRUSTUM_LEFT].pvec);
  lxPlaneIntersect(box[FRUSTUM_C_NBR],pFrustum->fplanes[FRUSTUM_NEAR].pvec,pFrustum->fplanes[FRUSTUM_BOTTOM].pvec,pFrustum->fplanes[FRUSTUM_RIGHT].pvec);
  lxPlaneIntersect(box[FRUSTUM_C_FTR],pFrustum->fplanes[FRUSTUM_FAR].pvec,pFrustum->fplanes[FRUSTUM_TOP].pvec,pFrustum->fplanes[FRUSTUM_RIGHT].pvec);
  lxPlaneIntersect(box[FRUSTUM_C_FTL],pFrustum->fplanes[FRUSTUM_FAR].pvec,pFrustum->fplanes[FRUSTUM_TOP].pvec,pFrustum->fplanes[FRUSTUM_LEFT].pvec);
  lxPlaneIntersect(box[FRUSTUM_C_FBL],pFrustum->fplanes[FRUSTUM_FAR].pvec,pFrustum->fplanes[FRUSTUM_BOTTOM].pvec,pFrustum->fplanes[FRUSTUM_LEFT].pvec);
  lxPlaneIntersect(box[FRUSTUM_C_FBR],pFrustum->fplanes[FRUSTUM_FAR].pvec,pFrustum->fplanes[FRUSTUM_BOTTOM].pvec,pFrustum->fplanes[FRUSTUM_RIGHT].pvec);
}

LUX_API void lxFrustum_fromCorners(lxFrustumPTR frustum, const lxVector3 box[FRUSTUM_CORNERS])
{
  lxPlaneSet(frustum->fplanes[FRUSTUM_NEAR].pvec,box[FRUSTUM_C_NBL],box[FRUSTUM_C_NTL],box[FRUSTUM_C_NTR]);
  lxPlaneSet(frustum->fplanes[FRUSTUM_FAR].pvec,box[FRUSTUM_C_FTR],box[FRUSTUM_C_FTL],box[FRUSTUM_C_FBL]);
  lxPlaneSet(frustum->fplanes[FRUSTUM_TOP].pvec,box[FRUSTUM_C_FTR],box[FRUSTUM_C_NTR],box[FRUSTUM_C_NTL]);
  lxPlaneSet(frustum->fplanes[FRUSTUM_BOTTOM].pvec,box[FRUSTUM_C_FBL],box[FRUSTUM_C_NBL],box[FRUSTUM_C_NBR]);
  lxPlaneSet(frustum->fplanes[FRUSTUM_RIGHT].pvec,box[FRUSTUM_C_FBR],box[FRUSTUM_C_NBR],box[FRUSTUM_C_NTR]);
  lxPlaneSet(frustum->fplanes[FRUSTUM_LEFT].pvec,box[FRUSTUM_C_FTL],box[FRUSTUM_C_NTL],box[FRUSTUM_C_NBL]);
}





