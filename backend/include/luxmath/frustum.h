// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt



#ifndef __LUXMATH_FRUSTUM_H__
#define __LUXMATH_FRUSTUM_H__

#include <luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// Frustum

LUX_API void lxFrustum_update(lxFrustumPTR frustum, const lxMatrix44PTR viewproj);

// returns FALSE if inside else TRUE
// the plane it failed on will be saved to it as well
LUX_API booln  lxFrustum_checkPointCoherent(const lxFrustumPTR frustum, const lxVector3 vec, int *clipplane);

// returns FALSE if inside/intersect
LUX_API booln  lxFrustum_checkSphereCoherent(const lxFrustumPTR frustum, const lxVector3 center, const float radius,int *clipplane);

// returns FALSE if sphere is fully inside
LUX_API booln  lxFrustum_checkSphereFull(const lxFrustumPTR frustum, const lxVector3 center, const float radius);

// returns FALSE if inside/intersect
LUX_API booln  lxFrustum_checkAABBvCoherent(const lxFrustumPTR frustum, const float minmaxs[6], int *clipplane);

// masking & temporal coherency
// cullF can report outside even if extends of box are outside all planes
LUX_API lxCullType_t  lxFrustum_cullAABBvMaskedCoherent(const lxFrustumPTR pFrustum, const float minmax[6], int in_mask, int *out_mask, int *inoutstart_id);

LUX_API lxCullType_t  lxFrustum_cullPoints(const lxFrustumPTR frustum, const lxVector4 *vecarray, const int numVec);
LUX_API lxCullType_t  lxFrustum_cullAABBv(const lxFrustumPTR frustum, const float minmaxs[6]);

// generates corner points for a frustum
LUX_API void lxFrustum_getCorners(const lxFrustumPTR frustum, lxVector3 box[FRUSTUM_CORNERS]);
LUX_API void lxFrustum_fromCorners(lxFrustumPTR frustum, const lxVector3 box[FRUSTUM_CORNERS]);

LUX_API void lxFrustum_updateSigns(lxFrustumPTR frustum);

//////////////////////////////////////////////////////////////////////////
//

LUX_INLINE booln lxFrustum_checkPointCoherent(const lxFrustumPTR pFrustum,const lxVector3 vec, int *clipplane)
{
  int i,skip;

  skip = (*clipplane);
  if(pFrustum->fplanes[skip].pvec[0] * vec[0] + pFrustum->fplanes[skip].pvec[1] * vec[1] + pFrustum->fplanes[skip].pvec[2] * vec[2] + pFrustum->fplanes[skip].pvec[3] <= 0)
  {
    return LUX_TRUE;
  }
  for(i = 0; i < FRUSTUM_PLANES; i++ )
  {
    if(i != skip && pFrustum->fplanes[i].pvec[0] * vec[0] + pFrustum->fplanes[i].pvec[1] * vec[1] + pFrustum->fplanes[i].pvec[2] * vec[2] + pFrustum->fplanes[i].pvec[3] <= 0)
    {
      *clipplane = i;
      return LUX_TRUE;
    }
  }

  return LUX_FALSE;
}

LUX_INLINE booln lxFrustum_checkSphereCoherent(const lxFrustumPTR pFrustum,const lxVector3 vec,const float radius, int *clipplane)
{
  int i,skip;

  skip = *clipplane;
  if(pFrustum->fplanes[skip].pvec[0] * vec[0] + pFrustum->fplanes[skip].pvec[1] * vec[1] + pFrustum->fplanes[skip].pvec[2] * vec[2] + pFrustum->fplanes[skip].pvec[3] <= -radius)
  {
    return LUX_TRUE;
  }

  for( i = 0; i < FRUSTUM_PLANES; i++ )
  {
    if(i != skip &&  pFrustum->fplanes[i].pvec[0] * vec[0] + pFrustum->fplanes[i].pvec[1] * vec[1] + pFrustum->fplanes[i].pvec[2] * vec[2] + pFrustum->fplanes[i].pvec[3] <= -radius )
    {
      *clipplane = i;
      return LUX_TRUE;
    }
  }
  return LUX_FALSE;
}


LUX_INLINE booln  lxFrustum_checkAABBvCoherent(const lxFrustumPTR pFrustum,const float minmax[6], int *clipplane)
{
  int i,skip;
  skip = *clipplane;
  {
    const lxFrustumPlanePTR sp = &pFrustum->fplanes[skip];
    if ((sp->pvec[0] * minmax[sp->px]) + 
      (sp->pvec[1] * minmax[sp->py]) + 
      (sp->pvec[2] * minmax[sp->pz]) + sp->pvec[3] < 0)
    {
      return LUX_TRUE;
    }
  }


  for (i = 0; i < FRUSTUM_PLANES; i++){
    if (i != skip){

      const lxFrustumPlanePTR sp = &pFrustum->fplanes[i];
      if ((sp->pvec[0] * minmax[sp->px]) + 
        (sp->pvec[1] * minmax[sp->py]) + 
        (sp->pvec[2] * minmax[sp->pz]) + sp->pvec[3] < 0)
      {
        *clipplane = i;
        return LUX_TRUE;
      }
    }
  }

  return LUX_FALSE;
}

#ifdef __cplusplus
}
#endif

#endif

