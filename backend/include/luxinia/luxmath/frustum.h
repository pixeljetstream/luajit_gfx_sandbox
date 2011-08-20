// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXMATH_LUX_FRUSTUM_H__
#define __LUXMATH_LUX_FRUSTUM_H__

#include <luxinia/luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// Frustum

LUX_API void lxFrustum_update(lxFrustumPTR frustum, lxMatrix44CPTR viewproj);

// returns FALSE if inside else TRUE
LUX_API booln  lxFrustum_checkPoint(lxFrustumCPTR frustum, lxVector3CPTR vec);

// returns FALSE if inside/intersect
LUX_API booln  lxFrustum_checkSphere(lxFrustumCPTR frustum, lxBoundingSphereCPTR );
LUX_API booln  lxFrustum_checkSphereCoherent(lxFrustumCPTR pFrustum, lxBoundingSphereCPTR sphere, int *plane);

// returns FALSE if sphere is fully inside
LUX_API booln  lxFrustum_checkSphereFull(lxFrustumCPTR frustum, lxBoundingSphereCPTR );

// returns FALSE if inside/intersect
LUX_API booln  lxFrustum_checkBoundingBox(lxFrustumCPTR frustum, lxBoundingBoxCPTR bbox);
LUX_API booln  lxFrustum_checkBoundingBoxCoherent(lxFrustumCPTR pFrustum, lxBoundingBoxCPTR bbox, int *plane);

// masking & temporal coherency
// cullF can report outside even if extends of box are outside all planes
LUX_API lxCullType_t  lxFrustum_cullBoundingBoxMaskedCoherent(lxFrustumCPTR pFrustum, lxBoundingBoxCPTR bbox, int in_mask, int *out_mask, int *inoutstart_id);

LUX_API lxCullType_t  lxFrustum_cullPoints(lxFrustumCPTR frustum, const lxVector4 *vecarray, const int numVec);
LUX_API lxCullType_t  lxFrustum_cullBoundingBox(lxFrustumCPTR frustum, lxBoundingBoxCPTR bbox );

// generates corner points for a frustum
LUX_API void lxFrustum_getCorners(lxFrustumCPTR frustum, lxVector3 box[LUX_FRUSTUM_CORNERS]);
LUX_API void lxFrustum_fromCorners(lxFrustumPTR frustum, const lxVector3 box[LUX_FRUSTUM_CORNERS]);

LUX_API void lxFrustum_updateSigns(lxFrustumPTR frustum);

//////////////////////////////////////////////////////////////////////////
//

LUX_INLINE booln lxFrustum_checkPoint(lxFrustumCPTR pFrustum, lxVector3CPTR vec)
{
  int i;

  for(i = 0; i < LUX_FRUSTUM_PLANES; i++ )
  {
    if( pFrustum->fplanes[i].pvec[0] * vec[0] + pFrustum->fplanes[i].pvec[1] * vec[1] + pFrustum->fplanes[i].pvec[2] * vec[2] + pFrustum->fplanes[i].pvec[3] <= 0)
    {
      return LUX_TRUE;
    }
  }

  return LUX_FALSE;
}

LUX_INLINE booln lxFrustum_checkSphere(lxFrustumCPTR pFrustum, lxBoundingSphereCPTR sphere)
{
  int i;
  for( i = 0; i < LUX_FRUSTUM_PLANES; i++ )
  {
    if(pFrustum->fplanes[i].pvec[0] * sphere->center[0] + pFrustum->fplanes[i].pvec[1] * sphere->center[1] + pFrustum->fplanes[i].pvec[2] * sphere->center[2] + pFrustum->fplanes[i].pvec[3] <= -sphere->radius )
    {
      return LUX_TRUE;
    }
  }
  return LUX_FALSE;
}


LUX_INLINE booln  lxFrustum_checkBoundingBox(lxFrustumCPTR pFrustum, lxBoundingBoxCPTR bbox)
{
  int i;
  for (i = 0; i < LUX_FRUSTUM_PLANES; i++){
    lxFrustumPlaneCPTR sp = &pFrustum->fplanes[i];
    if ((sp->pvec[0] * bbox->min[sp->px]) + 
      (sp->pvec[1] * bbox->min[sp->py]) + 
      (sp->pvec[2] * bbox->min[sp->pz]) + sp->pvec[3] < 0)
    {
      return LUX_TRUE;
    }
  }

  return LUX_FALSE;
}

LUX_INLINE booln lxFrustum_checkSphereCoherent(lxFrustumCPTR pFrustum, lxBoundingSphereCPTR sphere, int *plane)
{
  int i;
  int offset = *plane;
  for( i = 0; i < LUX_FRUSTUM_PLANES; i++ ){
    int n = (i + offset) % LUX_FRUSTUM_PLANES;
    lxFrustumPlaneCPTR sp = &pFrustum->fplanes[n];
    if(sp->pvec[0] * sphere->center[0] + sp->pvec[1] * sphere->center[1] + sp->pvec[2] * sphere->center[2] + sp->pvec[3] <= -sphere->radius )
    {
      *plane = n;
      return LUX_TRUE;
    }
  }
  return LUX_FALSE;
}


LUX_INLINE booln  lxFrustum_checkBoundingBoxCoherent(lxFrustumCPTR pFrustum, lxBoundingBoxCPTR bbox, int *plane)
{
  int i;
  int offset = *plane;
  for (i = 0; i < LUX_FRUSTUM_PLANES; i++){
    int n = (i + offset) % LUX_FRUSTUM_PLANES;
    lxFrustumPlaneCPTR sp = &pFrustum->fplanes[n];
    if ((sp->pvec[0] * bbox->min[sp->px]) + 
      (sp->pvec[1] * bbox->min[sp->py]) + 
      (sp->pvec[2] * bbox->min[sp->pz]) + sp->pvec[3] < 0)
    {
      *plane = n;
      return LUX_TRUE;
    }
  }

  return LUX_FALSE;
}
#ifdef __cplusplus
}
#endif

#endif

