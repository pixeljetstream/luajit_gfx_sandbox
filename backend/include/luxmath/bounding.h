// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt



#ifndef __LUXMATH_BOUNDING_H__
#define __LUXMATH_BOUDNING_H__

#include <luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif


//////////////////////////////////////////////////////////////////////////
// Generic

LUX_API booln lxMinMax_intersectsV(float selfminmax[6], float otherminmax[6]);


///////////////////////////////////////////////////////////////////////////////
// BOUNDING VOLUMES

LUX_API void lxBoundingBox_init(lxBoundingBoxPTR box);
LUX_API void lxBoundingBox_complete(lxBoundingBoxPTR box, booln fromMinMax);

// copy a bounding volume to another
LUX_API lxBoundingBoxPTR lxBoundingBox_copy(lxBoundingBoxPTR out, lxBoundingBoxPTR in);
LUX_API lxBoundingSpherePTR lxBoundingSphere_copy(lxBoundingSpherePTR out, const lxBoundingSpherePTR in);


// use min/max
// ------------
// merge two bounding volumes
LUX_API lxBoundingBoxPTR  lxBoundingBox_merge(lxBoundingBoxPTR out, lxBoundingBoxPTR a, lxBoundingBoxPTR b);
// returns TRUE if result differs from a
LUX_API booln lxBoundingBox_mergeChange(lxBoundingBoxPTR out, lxBoundingBoxPTR a, lxBoundingBoxPTR b);
LUX_API booln lxBoundingSphere_mergeChange(lxBoundingSpherePTR out, const lxBoundingSpherePTR a, const lxBoundingSpherePTR b);

// converts a bounding box to a bounding sphere
LUX_API lxBoundingSpherePTR lxBoundingBox_toSphere(lxBoundingBoxPTR bbox, lxBoundingSpherePTR sphere);
LUX_API void lxBoundingBox_toSphereV(const lxVector3 min,const lxVector3 max, lxVector3 center, float *radius);

// convert bounding box to capsule
// a capsule along major axis of boundingbox is generated with radius
// of the greater half dimension of the other 2 axis
LUX_API lxBoundingCapsulePTR lxBoundingBox_toCapsule(lxBoundingBoxPTR bbox,lxBoundingCapsulePTR capsule);

// transforms a bounding box to an axis aligned bounding box
LUX_API lxBoundingBoxPTR lxBoundingBox_transform( lxBoundingBoxPTR out, lxBoundingBoxPTR in, lxMatrix44PTR trans);
LUX_API void lxBoundingBox_transformBoxCorners(const lxBoundingBoxPTR in, const lxMatrix44PTR trans, lxVector3 box[8]);
LUX_API void lxBoundingBox_transformV(lxVector3 outmins, lxVector3 outmaxs,lxVector3 mins, lxVector3 maxs, lxMatrix44PTR trans);

LUX_API void lxBoundingBox_fromCorners(lxBoundingBoxPTR bbox,const lxVector3 vecs[8]);
LUX_API void lxBoundingVectors_fromCamera(lxVector3 vecs[8],const lxMatrix44PTR mat,const float fov, const float frontplane, const float backplane, const float aspect);

LUX_API booln lxBoundingBox_intersect(const lxBoundingBoxPTR a,const lxBoundingBoxPTR b);
LUX_API booln lxBoundingBox_checkPoint(const lxBoundingBoxPTR out,const lxVector3 point);
LUX_API booln lxBoundingCone_checkSphereV(lxBoundingConePTR cone,lxVector3 center, float radius, float radiusSqr);

// approximates frustum via Sphere & Cone, dir must be normalized
LUX_API void lxBoundingSphereCone_fromCamera(lxBoundingSpherePTR sphere, lxBoundingConePTR cone,float frontplane, float backplane, lxVector3 pos, lxVector3 dir, float fov);
LUX_API void lxBoundingCone_fromFrustumCorners(lxBoundingConePTR cone, lxVector3 box[FRUSTUM_CORNERS]);
LUX_API void lxBoundingSphere_fromFrustumCorners(lxBoundingSpherePTR sphere,lxVector3 box[FRUSTUM_CORNERS]);

//////////////////////////////////////////////////////////////////////////
LUX_INLINE void lxBoundingBox_init(lxBoundingBoxPTR box)
{
  box->min[0] = 0.0f;
  box->min[1] = 0.0f;
  box->min[2] = 0.0f;

  box->max[0] = 0.0f;
  box->max[1] = 0.0f;
  box->max[2] = 0.0f;

  box->center[0] = 0.0f;
  box->center[1] = 0.0f;
  box->center[2] = 0.0f;

  box->length[0] = 0.0f;
  box->length[1] = 0.0f;
  box->length[2] = 0.0f;
}

LUX_INLINE booln lxMinMax_intersectsV(float selfminmax[6], float otherminmax[6]){
  return (  
    selfminmax[0] <= otherminmax[3] && 
    selfminmax[1] <= otherminmax[4] &&
    selfminmax[2] <= otherminmax[5] &&
    selfminmax[3] >= otherminmax[0] &&
    selfminmax[4] >= otherminmax[1] &&
    selfminmax[5] >= otherminmax[2] );
}

#ifdef __cplusplus
}
#endif

#endif

