local ffi = require "ffi"
require "luxinia2.luxplatform"

ffi.cdef [[
// lxm | Lux Math
  typedef float lxVector2[2];
  typedef float lxVector3[3];
  typedef float lxVector4[4];
  typedef float lxPlane[4];
  typedef float lxMatrix44[16];
  typedef float lxMatrix34[12];
  typedef float lxQuat[4];
  
    typedef enum lxFrustumPlaneType_e{
    LUX_FRUSTUM_TOP,
    LUX_FRUSTUM_BOTTOM,
    LUX_FRUSTUM_NEAR,
    LUX_FRUSTUM_FAR,
    LUX_FRUSTUM_LEFT,
    LUX_FRUSTUM_RIGHT,

    LUX_FRUSTUM_PLANES,
  }lxFrustumPlaneType_t;

  typedef enum lxFrustumCornerType_e{
    LUX_FRUSTUM_C_NTR,
    LUX_FRUSTUM_C_NTL,
    LUX_FRUSTUM_C_NBL,
    LUX_FRUSTUM_C_NBR,

    LUX_FRUSTUM_C_FTR,
    LUX_FRUSTUM_C_FTL,
    LUX_FRUSTUM_C_FBL,
    LUX_FRUSTUM_C_FBR,

    LUX_FRUSTUM_CORNERS,
  }lxFrustumCornerType_t;

  typedef enum lxCullType_e{
    LUX_CULL_INTERSECT = -1,
    LUX_CULL_OUTSIDE = 0,
    LUX_CULL_INSIDE = 1,
  }lxCullType_t;

  typedef struct lxFrustumPlane_s{
    lxPlane     pvec;
    union{
      int     n[3];
      struct{
        int   nx;
        int   ny;
        int   nz;
      };
    };
    union{
      int     p[3];
      struct{
        int   px;
        int   py;
        int   pz;
      };
    };
  }lxFrustumPlane_t;

  typedef struct lxFrustum_s
  {
    lxFrustumPlane_t  fplanes[LUX_FRUSTUM_PLANES];
  }lxFrustum_t;

  typedef struct lxBoundingBox_s
  {
    lxVector3 min;
    lxVector3 max;
    lxVector3 center;
    lxVector3 length;
  }lxBoundingBox_t;

  typedef struct lxBoundingSphere_s
  {
    lxVector3 center;
    float radius;
    float radiusSqr;
  }lxBoundingSphere_t;

  typedef struct lxBoundingCone_s
  {
    lxVector3 top;
    lxVector3 axis;
    float sinDiv;
    float sinSqr;
    float cosSqr;
  }lxBoundingCone_t;

  typedef struct lxBoundingCapsule_s
  {
    lxVector3 origin;
    lxVector3 toEnd;    // end-origin, non normalized
    float radius;
    float radiusSqr;
  }lxBoundingCapsule_t;

 const float* lxMatrix34GetIdentity();
 void lxMatrix34Copy(lxMatrix34 dst, const lxMatrix34 src);
 void lxMatrix34Identity(lxMatrix34 mat);
 void lxMatrix34TMultiply44( lxMatrix34 dst, const lxMatrix44 mat1,  const lxMatrix44 mat2 );

 const float* lxMatrix44GetIdentity();
 void lxMatrix44Identity(lxMatrix44 dst);
 void lxMatrix44Copy(lxMatrix44 dst, const lxMatrix44 src);
 void lxMatrix44CopyRot(lxMatrix44 dst, const lxMatrix44 src);
 void lxMatrix44CopyRotTransposed(lxMatrix44 dst, const lxMatrix44 src);

 void lxMatrix44SetTranslation( lxMatrix44 dst, const lxVector3 translation );
 void lxMatrix44SetInvTranslation( lxMatrix44 dst, const lxVector3 translation );
 void lxMatrix44SetScale( lxMatrix44 dst, const lxVector3 scale);
 void lxMatrix44PreScale( lxMatrix44 dst, const lxMatrix44 mat, const lxVector3 scale);
 void lxMatrix44SetRotRows(lxMatrix44 dst, 
            float a, float b, float c, 
            float d, float e, float f, 
            float g, float h, float i);

 float*  lxMatrix44GetTranslation(const lxMatrix44 mat, lxVector3 vec);

 void lxMatrix44Clear(lxMatrix44 mat);
 void lxMatrix44ClearRot(lxMatrix44 mat);
 void lxMatrix44ClearTranslation(lxMatrix44 mat);

 void lxMatrix44VectorTransform( const lxMatrix44 mat, lxVector3 v1 );
 void lxMatrix44VectorInvTransform( const lxMatrix44 mat, lxVector3 pVect );
 void lxMatrix44VectorRotate( const lxMatrix44 mat, lxVector3 v1 );
 void lxMatrix44VectorInvRotate( const lxMatrix44 mat, lxVector3 pVect );
 void lxMatrix44VectorTranslate( const lxMatrix44 mat, lxVector3 pVect );
 void lxMatrix44VectorInvTranslate( const lxMatrix44 mat, lxVector3 pVect );

 void lxMatrix44Multiply( lxMatrix44 dst, const lxMatrix44 mat1,  const lxMatrix44 mat2 );
 void lxMatrix44Multiply1( lxMatrix44 mat1,  const lxMatrix44 mat2 );
 void lxMatrix44Multiply2( const lxMatrix44 mat1,  lxMatrix44 mat2 );
 void lxMatrix44MultiplyFull( lxMatrix44 clip, const lxMatrix44 proj , const lxMatrix44 modl);

 void lxMatrix44MultiplyRot(lxMatrix44 dst, const lxMatrix44 mat1, const lxMatrix44 mat2 );
 void lxMatrix44MultiplyRot1( lxMatrix44 mat1, const lxMatrix44 mat2 );
 void lxMatrix44MultiplyRot2( const lxMatrix44 mat1,  lxMatrix44 mat2 );

 void lxMatrix44OrthonormalizeA(lxMatrix44 dst,lxMatrix44 src);

 void lxMatrix44Transpose(lxMatrix44 dst, const lxMatrix44 src);
 void lxMatrix44Transpose1(lxMatrix44 mat);
 void lxMatrix44TransposeRot(lxMatrix44 dst, const lxMatrix44 src);
 void lxMatrix44TransposeRot1(lxMatrix44 mat);
 void lxMatrix44TransposeRotIdentity(lxMatrix44 dst, const lxMatrix44 src);

 void lxMatrix44Invert(lxMatrix44 dst, const lxMatrix44 src);
 void lxMatrix44Invert1(lxMatrix44 mat);
 void lxMatrix44AffineInvert(lxMatrix44 dst, const lxMatrix44 src);

// rotation
 void lxMatrix44Orient(lxMatrix44 mat, lxVector3 forward,lxVector3 up, int axis);
 void lxMatrix44LookAt(lxMatrix44 mat, lxVector3 from, lxVector3 to, lxVector3 upn);

 void lxMatrix44FromEulerZYX( lxMatrix44 mat, const lxVector3 angles );
 void lxMatrix44FromEulerZYXdeg( lxMatrix44 mat, const lxVector3 angles );
 void lxMatrix44FromEulerXYZ(lxMatrix44 mat, lxVector3 angles);
 void lxMatrix44FromAngleAxisFast(lxMatrix44 mat, const lxVector3 axis, 
                 float cos, float sin, float oneminuscos);
 void lxMatrix44FromAngleAxis(lxMatrix44 mat, float anglerad, const lxVector3 axis);
 void lxMatrix44RotateAngle(lxMatrix44 mat, lxVector3 from, lxVector3 to);
 void lxMatrix44RotateAroundVector(lxMatrix44 mat, lxVector3 axis, float angleRad);
 void lxMatrix44RotateAroundPoint( lxMatrix44 mat,  const lxVector3 center, const lxVector3 angles);

 void lxMatrix44ToEulerXYZ( const lxMatrix44 mat, lxVector3 angles);
 void lxMatrix44ToEulerZYX( const lxMatrix44 mat, lxVector3 angles);

// projection
 void lxMatrix44Perspective(lxMatrix44 mat, const float fov, 
          const float front, const float back, const float aspect);
 void lxMatrix44PerspectiveInf(lxMatrix44 mat, const float fov, const float front, const float aspect);
 void lxMatrix44Ortho(lxMatrix44 mat, const float height, 
          const float front, const float back, const float aspect);
 void lxMatrix44OrthoDirect(lxMatrix44 mat, const float left, const float right, 
          const float bottom, const float top, 
          const float front, const float back);
 void lxMatrix44PlaneProjection(lxMatrix44 mat, const lxVector3 planenormal);
 void lxMatrix44ModifyProjectionClipplane(lxMatrix44 projmatrix,const lxMatrix44 mview, 
          const lxMatrix44 mviewinv, const lxVector4 clipPlane);
 void lxMatrix44Reflection(lxMatrix44 mat,lxVector4 plane);
 
 ///////////////////////////////////////////////////
 /// __LUXMATH_QUATERNION_H__
 
 void lxQuatSet( lxQuat pV, float x, float y, float z, float w);
 void lxQuatCopy( lxQuat pOut, const lxQuat pV1);
 void lxQuatClear( lxQuat pOut);
 void lxQuatIdentity( lxQuat pOut);
 void lxQuatAdd(lxQuat q, const lxQuat q1, const lxQuat q2);
 void lxQuatAdd1(lxQuat q1,const lxQuat q);
 void lxQuatSub(lxQuat q, const lxQuat q1, const lxQuat q2);
 void lxQuatSub1(lxQuat q,const lxQuat q1);
 void lxQuatScale(lxQuat qout, const lxQuat q,const float value);

 void lxQuatMul(lxQuat q,const lxQuat q2,const lxQuat q1);
 void lxQuatMul1(lxQuat q2,const lxQuat q1);
 void lxQuatMul2(const lxQuat q2,lxQuat q1);

 void lxQuatToMatrix(const lxQuat q,lxMatrix44 a_mat);
 void lxQuatToMatrixIdentity(const lxQuat q,lxMatrix44 a_mat);
 void lxQuatFromMatrix(lxQuat q1,const lxMatrix44 in_mat);
 void lxQuatToRotationAxis(const lxQuat q,float *degrees, lxVector3 a);
 void lxQuatFromRotationAxis(lxQuat q,const float angle, const lxVector3 a);
 void lxQuatFromEuler(lxQuat q, float x, float y, float z);
 void lxQuatFromEulerXYZ(lxQuat q, const lxVector3 vec);

 void lxQuatTransformVector(const lxQuat q, const lxVector3 v, lxVector3 out);

 float lxQuatLength(const lxQuat q);
 float lxQuatLengthSqr(const lxQuat q);
 float lxQuatDot(const lxQuat q,const lxQuat q2);

 void lxQuatNormalized(lxQuat q);
 void lxQuatInverted(lxQuat q);
 void lxQuatConjugate(lxQuat q1, const lxQuat q2);
 void lxQuatNaturalLog(lxQuat q1, const lxQuat q);
 void lxQuatExponent(lxQuat q1,const lxQuat q);

 void lxQuatSlerp(lxQuat q,const float t, const lxQuat q1, const lxQuat q2);
 void lxQuatSlerpLongest(lxQuat q,const float t, const lxQuat q1, const lxQuat q2);
 void lxQuatSlerpQuadratic(lxQuat q,const float t, const lxQuat q1, const lxQuat q2, const lxQuat q3, const lxQuat q4);
 void lxQuatSlerpQuadTangents(lxQuat qout,float t, const lxQuat q1, const lxQuat q2, const lxQuat q3, const lxQuat q4);
 void lxQuatGetBarycentric(lxQuat q,float f, float g, const lxQuat q1, const lxQuat q2, const lxQuat q3);

 void lxQuatSwizzle(lxQuat out, uint axis[3], lxVector3 dirs);
 
///////////////////////////////////////////////////
/// __LUXMATH_LUX_FRUSTUM_H__
 void lxFrustum_update(lxFrustum_t* frustum, const lxMatrix44 viewproj);

// returns FALSE if inside else TRUE
// the plane it failed on will be saved to it as well
 booln  lxFrustum_checkPointCoherent(const lxFrustum_t* frustum, const lxVector3 vec, int *clipplane);

// returns FALSE if inside/intersect
 booln  lxFrustum_checkSphereCoherent(const lxFrustum_t* frustum, const lxVector3 center, const float radius,int *clipplane);

// returns FALSE if sphere is fully inside
 booln  lxFrustum_checkSphereFull(const lxFrustum_t* frustum, const lxVector3 center, const float radius);

// returns FALSE if inside/intersect
 booln  lxFrustum_checkAABBvCoherent(const lxFrustum_t* frustum, const float minmaxs[6], int *clipplane);

// masking & temporal coherency
// cullF can report outside even if extends of box are outside all planes
 lxCullType_t  lxFrustum_cullAABBvMaskedCoherent(const lxFrustum_t* pFrustum, const float minmax[6], int in_mask, int *out_mask, int *inoutstart_id);

 lxCullType_t  lxFrustum_cullPoints(const lxFrustum_t* frustum, const lxVector4 *vecarray, const int numVec);
 lxCullType_t  lxFrustum_cullAABBv(const lxFrustum_t* frustum, const float minmaxs[6]);

// generates corner points for a frustum
 void lxFrustum_getCorners(const lxFrustum_t* frustum, lxVector3 box[LUX_FRUSTUM_CORNERS]);
 void lxFrustum_fromCorners(lxFrustum_t* frustum, const lxVector3 box[LUX_FRUSTUM_CORNERS]);

 void lxFrustum_updateSigns(lxFrustum_t* frustum);

///////////////////////////////////////////////////
/// __LUXMATH_BOUNDING_H__
 booln lxMinMax_intersectsV(float selfminmax[6], float otherminmax[6]);


///////////////////////////////////////////////////////////////////////////////
// BOUNDING VOLUMES

 void lxBoundingBox_init(lxBoundingBox_t* box);
 void lxBoundingBox_complete(lxBoundingBox_t* box, booln fromMinMax);

// copy a bounding volume to another
 lxBoundingBox_t* lxBoundingBox_copy(lxBoundingBox_t* out, lxBoundingBox_t* in);
 lxBoundingSphere_t* lxBoundingSphere_copy(lxBoundingSphere_t* out, const lxBoundingSphere_t* in);


// use min/max
// ------------
// merge two bounding volumes
 lxBoundingBox_t*  lxBoundingBox_merge(lxBoundingBox_t* out, lxBoundingBox_t* a, lxBoundingBox_t* b);
// returns TRUE if result differs from a
 booln lxBoundingBox_mergeChange(lxBoundingBox_t* out, lxBoundingBox_t* a, lxBoundingBox_t* b);
 booln lxBoundingSphere_mergeChange(lxBoundingSphere_t* out, const lxBoundingSphere_t* a, const lxBoundingSphere_t* b);

// converts a bounding box to a bounding sphere
 lxBoundingSphere_t* lxBoundingBox_toSphere(lxBoundingBox_t* bbox, lxBoundingSphere_t* sphere);
 void lxBoundingBox_toSphereV(const lxVector3 min,const lxVector3 max, lxVector3 center, float *radius);

// convert bounding box to capsule
// a capsule along major axis of boundingbox is generated with radius
// of the greater half dimension of the other 2 axis
 lxBoundingCapsule_t* lxBoundingBox_toCapsule(lxBoundingBox_t* bbox,lxBoundingCapsule_t* capsule);

// transforms a bounding box to an axis aligned bounding box
 lxBoundingBox_t* lxBoundingBox_transform( lxBoundingBox_t* out, lxBoundingBox_t* in, lxMatrix44 trans);
 void lxBoundingBox_transformBoxCorners(const lxBoundingBox_t* in, const lxMatrix44 trans, lxVector3 box[8]);
 void lxBoundingBox_transformV(lxVector3 outmins, lxVector3 outmaxs,lxVector3 mins, lxVector3 maxs, lxMatrix44 trans);

 void lxBoundingBox_fromCorners(lxBoundingBox_t* bbox,const lxVector3 vecs[8]);
 void lxBoundingVectors_fromCamera(lxVector3 vecs[8],const lxMatrix44 mat,const float fov, const float frontplane, const float backplane, const float aspect);

 booln lxBoundingBox_intersect(const lxBoundingBox_t* a,const lxBoundingBox_t* b);
 booln lxBoundingBox_checkPoint(const lxBoundingBox_t* out,const lxVector3 point);
 booln lxBoundingCone_checkSphereV(lxBoundingCone_t* cone,lxVector3 center, float radius, float radiusSqr);

// approximates frustum via Sphere & Cone, dir must be normalized
 void lxBoundingSphereCone_fromCamera(lxBoundingSphere_t* sphere, lxBoundingCone_t* cone,float frontplane, float backplane, lxVector3 pos, lxVector3 dir, float fov);
 void lxBoundingCone_fromFrustumCorners(lxBoundingCone_t* cone, lxVector3 box[LUX_FRUSTUM_CORNERS]);
 void lxBoundingSphere_fromFrustumCorners(lxBoundingSphere_t* sphere,lxVector3 box[LUX_FRUSTUM_CORNERS]);

//////////////////////////////////////////////////
/// __LUXMATH_GEOMETRY_H__
 void lxPlaneSet(lxPlane plane, const lxVector3 a, const lxVector3 b, const lxVector3 c);

 void lxPlaneIntersect(lxVector3 out, const lxPlane planea,const lxPlane planeb,const lxPlane planec);

 void lxPlaneVecCheckLine(lxVector3 intersectpoint, const lxVector3 planenormal, 
          const lxVector3 planepoint, const lxVector3 raystart, const lxVector3 rayend);

 float lxTriangleArea( const lxVector3 v0, const lxVector3 v1, const lxVector3 v2);

]]

return ffi.load("luxbackend")