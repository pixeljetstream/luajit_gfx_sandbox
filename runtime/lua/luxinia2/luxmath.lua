local ffi = require "ffi"
require "luxinia2.luxplatform"

ffi.cdef [[
// lxm | Lux Math
typedef float lxVector2 [ 2 ] ;
typedef float lxVector3 [ 3 ] ;
typedef float lxVector4 [ 4 ] ;
typedef float lxPlane [ 4 ] ;
typedef float lxMatrix44 [ 16 ] ;
typedef float lxMatrix34 [ 12 ] ;
typedef float lxQuat [ 4 ] ;
typedef float * lxVector2PTR ;
typedef float * lxVector3PTR ;
typedef float * lxVector4PTR ;
typedef float * lxPlanePTR ;
typedef float * lxMatrix44PTR ;
typedef float * lxMatrix34PTR ;
typedef float * lxQuatPTR ;
typedef const float * lxVector2CPTR ;
typedef const float * lxVector3CPTR ;
typedef const float * lxVector4CPTR ;
typedef const float * lxPlaneCPTR ;
typedef const float * lxMatrix44CPTR ;
typedef const float * lxMatrix34CPTR ;
typedef const float * lxQuatCPTR ;
typedef __declspec ( align ( 16 ) ) lxMatrix44 lxMatrix44SIMD ;
typedef __declspec ( align ( 16 ) ) lxVector4 lxVector4SIMD ;
typedef __declspec ( align ( 16 ) ) lxVector4 lxVector3SIMD ;
typedef struct lxFrustumPlane_s * lxFrustumPlanePTR ;
typedef const struct lxFrustumPlane_s * lxFrustumPlaneCPTR ;
typedef struct lxFrustum_s * lxFrustumPTR ;
typedef const struct lxFrustum_s * lxFrustumCPTR ;
typedef struct lxBoundingBox_s * lxBoundingBoxPTR ;
typedef const struct lxBoundingBox_s * lxBoundingBoxCPTR ;
typedef struct lxBoundingBoxCenter_s * lxBoundingBoxCenterPTR ;
typedef const struct lxBoundingBoxCenter_s * lxBoundingBoxCenterCPTR ;
typedef struct lxBoundingSphere_s * lxBoundingSpherePTR ;
typedef const struct lxBoundingSphere_s * lxBoundingSphereCPTR ;
typedef struct lxBoundingCone_s * lxBoundingConePTR ;
typedef const struct lxBoundingCone_s * lxBoundingConeCPTR ;
typedef struct lxBoundingCapsule_s * lxBoundingCapsulePTR ;
typedef const struct lxBoundingCapsule_s * lxBoundingCapsuleCPTR ;
typedef enum lxFrustumPlaneType_e
{
    LUX_FRUSTUM_TOP , LUX_FRUSTUM_BOTTOM , LUX_FRUSTUM_NEAR , LUX_FRUSTUM_FAR , LUX_FRUSTUM_LEFT , LUX_FRUSTUM_RIGHT , LUX_FRUSTUM_PLANES , }
lxFrustumPlaneType_t ;
typedef enum lxFrustumCornerType_e
{
    LUX_FRUSTUM_C_NTR , LUX_FRUSTUM_C_NTL , LUX_FRUSTUM_C_NBL , LUX_FRUSTUM_C_NBR , LUX_FRUSTUM_C_FTR , LUX_FRUSTUM_C_FTL , LUX_FRUSTUM_C_FBL , LUX_FRUSTUM_C_FBR , LUX_FRUSTUM_CORNERS , }
lxFrustumCornerType_t ;
typedef enum lxCullType_e
{
    LUX_CULL_INTERSECT = - 1 , LUX_CULL_OUTSIDE = 0 , LUX_CULL_INSIDE = 1 , }
lxCullType_t ;
typedef struct lxFrustumPlane_s
{
    lxPlane pvec ;
    union
    {
        int n [ 3 ] ;
        struct
        {
            int nx ;
            int ny ;
            int nz ;
            int _npad ;
        }
        ;
    }
    ;
    union
    {
        int p [ 3 ] ;
        struct
        {
            int px ;
            int py ;
            int pz ;
            int _ppad ;
        }
        ;
    }
    ;
}
lxFrustumPlane_t ;
typedef struct lxFrustum_s
{
    lxFrustumPlane_t fplanes [ LUX_FRUSTUM_PLANES ] ;
}
lxFrustum_t ;
typedef struct lxBoundingBox_s
{
    lxVector4 min ;
    lxVector4 max ;
}
lxBoundingBox_t ;
typedef struct lxBoundingBoxCenter_s
{
    lxVector4 center ;
    lxVector4 size ;
}
lxBoundingBoxCenter_t ;
typedef struct lxBoundingSphere_s
{
    lxVector3 center ;
    float radius ;
}
lxBoundingSphere_t ;
typedef struct lxBoundingCone_s
{
    lxVector4 top ;
    lxVector4 axis ;
    float sinDiv ;
    float sinSqr ;
    float cosSqr ;
    float _pad ;
}
lxBoundingCone_t ;
typedef struct lxBoundingCapsule_s
{
    lxVector4 origin ;
    lxVector4 toEnd ;
    float radius ;
    float radiusSqr ;
    float _pad [ 2 ] ;
}
lxBoundingCapsule_t ;
booln lxMinMax_intersectsV ( const float selfminmax [ 6 ] , const float otherminmax [ 6 ] ) ;
void lxBoundingBox_init ( lxBoundingBoxPTR bbox ) ;
void lxBoundingBox_toCenter ( lxBoundingBoxCPTR bbox , lxVector3 center , lxVector3 size ) ;
lxBoundingBoxPTR lxBoundingBox_copy ( lxBoundingBoxPTR dst , lxBoundingBoxCPTR src ) ;
lxBoundingSpherePTR lxBoundingSphere_copy ( lxBoundingSpherePTR dst , lxBoundingSphereCPTR src ) ;
lxBoundingBoxPTR lxBoundingBox_merge ( lxBoundingBoxPTR out , lxBoundingBoxCPTR a , lxBoundingBoxCPTR b ) ;
booln lxBoundingBox_mergeChange ( lxBoundingBoxPTR out , lxBoundingBoxCPTR a , lxBoundingBoxCPTR b ) ;
booln lxBoundingSphere_mergeChange ( lxBoundingSpherePTR out , lxBoundingSphereCPTR a , lxBoundingSphereCPTR b ) ;
void lxBoundingBox_toCenterBox ( lxBoundingBoxCPTR box , lxBoundingBoxCenterPTR ctr ) ;
void lxBoundingBox_fromCenterBox ( lxBoundingBoxPTR box , lxBoundingBoxCenterCPTR ctr ) ;
lxBoundingSpherePTR lxBoundingBox_toSphere ( lxBoundingBoxCPTR bbox , lxBoundingSpherePTR sphere ) ;
void lxBoundingBox_toSphereV ( const lxVector3 min , const lxVector3 max , lxVector3 center , float * radius ) ;
lxBoundingCapsulePTR lxBoundingBox_toCapsule ( lxBoundingBoxCPTR bbox , lxBoundingCapsulePTR capsule ) ;
lxBoundingBoxPTR lxBoundingBox_transform ( lxBoundingBoxPTR out , lxBoundingBoxCPTR in , lxMatrix44CPTR trans ) ;
void lxBoundingBox_transformBoxCorners ( lxBoundingBoxCPTR in , lxMatrix44CPTR trans , lxVector3 box [ 8 ] ) ;
void lxBoundingBox_transformV ( lxVector3 outmins , lxVector3 outmaxs , const lxVector3 mins , const lxVector3 maxs , lxMatrix44CPTR trans ) ;
void lxBoundingBox_fromCorners ( lxBoundingBoxPTR bbox , const lxVector3 vecs [ 8 ] ) ;
void lxBoundingCorners_fromCamera ( lxVector3 vecs [ 8 ] , lxMatrix44CPTR mat , const float fov , const float frontplane , const float backplane , const float aspect ) ;
booln lxBoundingBox_intersect ( lxBoundingBoxCPTR a , lxBoundingBoxCPTR b ) ;
booln lxBoundingBox_checkPoint ( lxBoundingBoxCPTR out , const lxVector3 point ) ;
booln lxBoundingCone_checkSphere ( lxBoundingConeCPTR cone , lxBoundingSphereCPTR sphere ) ;
void lxBoundingSphereCone_fromCamera ( lxBoundingSpherePTR sphere , lxBoundingConePTR cone , float frontplane , float backplane , const lxVector3 pos , const lxVector3 dir , float fov ) ;
void lxBoundingCone_fromFrustumCorners ( lxBoundingConePTR cone , const lxVector3 box [ LUX_FRUSTUM_CORNERS ] ) ;
void lxBoundingSphere_fromFrustumCorners ( lxBoundingSpherePTR sphere , const lxVector3 box [ LUX_FRUSTUM_CORNERS ] ) ;
void lxFrustum_update ( lxFrustumPTR frustum , lxMatrix44CPTR viewproj ) ;
booln lxFrustum_checkPoint ( lxFrustumCPTR frustum , lxVector3CPTR vec ) ;
booln lxFrustum_checkSphere ( lxFrustumCPTR frustum , lxBoundingSphereCPTR ) ;
booln lxFrustum_checkSphereCoherent ( lxFrustumCPTR pFrustum , lxBoundingSphereCPTR sphere , int * plane ) ;
booln lxFrustum_checkSphereFull ( lxFrustumCPTR frustum , lxBoundingSphereCPTR ) ;
booln lxFrustum_checkBoundingBox ( lxFrustumCPTR frustum , lxBoundingBoxCPTR bbox ) ;
booln lxFrustum_checkBoundingBoxCoherent ( lxFrustumCPTR pFrustum , lxBoundingBoxCPTR bbox , int * plane ) ;
lxCullType_t lxFrustum_cullBoundingBoxMaskedCoherent ( lxFrustumCPTR pFrustum , lxBoundingBoxCPTR bbox , int in_mask , int * out_mask , int * inoutstart_id ) ;
lxCullType_t lxFrustum_cullPoints ( lxFrustumCPTR frustum , const lxVector4 * vecarray , const int numVec ) ;
lxCullType_t lxFrustum_cullBoundingBox ( lxFrustumCPTR frustum , lxBoundingBoxCPTR bbox ) ;
void lxFrustum_getCorners ( lxFrustumCPTR frustum , lxVector3 box [ LUX_FRUSTUM_CORNERS ] ) ;
void lxFrustum_fromCorners ( lxFrustumPTR frustum , const lxVector3 box [ LUX_FRUSTUM_CORNERS ] ) ;
void lxFrustum_updateSigns ( lxFrustumPTR frustum ) ;
lxMatrix44CPTR lxMatrix44GetIdentity ( ) ;
void lxMatrix44Identity ( lxMatrix44PTR dst ) ;
void lxMatrix44Copy ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44CopyRot ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44CopyRotTransposed ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44SetTranslation ( lxMatrix44PTR dst , lxVector3CPTR translation ) ;
void lxMatrix44SetInvTranslation ( lxMatrix44PTR dst , lxVector3CPTR translation ) ;
void lxMatrix44SetScale ( lxMatrix44PTR dst , lxVector3CPTR scale ) ;
void lxMatrix44PreScale ( lxMatrix44PTR dst , lxMatrix44CPTR mat , lxVector3CPTR scale ) ;
void lxMatrix44SetRotRows ( lxMatrix44PTR dst , float a , float b , float c , float d , float e , float f , float g , float h , float i ) ;
float * lxMatrix44GetTranslation ( lxMatrix44CPTR mat , lxVector3PTR vec ) ;
void lxMatrix44Clear ( lxMatrix44PTR mat ) ;
void lxMatrix44ClearRot ( lxMatrix44PTR mat ) ;
void lxMatrix44ClearTranslation ( lxMatrix44PTR mat ) ;
void lxMatrix44VectorTransform ( lxMatrix44CPTR mat , lxVector3PTR v1 ) ;
void lxMatrix44VectorInvTransform ( lxMatrix44CPTR mat , lxVector3PTR pVect ) ;
void lxMatrix44VectorRotate ( lxMatrix44CPTR mat , lxVector3PTR v1 ) ;
void lxMatrix44VectorInvRotate ( lxMatrix44CPTR mat , lxVector3PTR pVect ) ;
void lxMatrix44VectorTranslate ( lxMatrix44CPTR mat , lxVector3PTR pVect ) ;
void lxMatrix44VectorInvTranslate ( lxMatrix44CPTR mat , lxVector3PTR pVect ) ;
void lxMatrix44Multiply ( lxMatrix44PTR dst , lxMatrix44CPTR mat1 , lxMatrix44CPTR mat2 ) ;
void lxMatrix44Multiply1 ( lxMatrix44PTR mat1 , lxMatrix44CPTR mat2 ) ;
void lxMatrix44Multiply2 ( lxMatrix44CPTR mat1 , lxMatrix44PTR mat2 ) ;
void lxMatrix44MultiplyFull ( lxMatrix44PTR clip , lxMatrix44CPTR proj , lxMatrix44CPTR modl ) ;
void lxMatrix44MultiplyRot ( lxMatrix44PTR dst , lxMatrix44CPTR mat1 , lxMatrix44CPTR mat2 ) ;
void lxMatrix44MultiplyRot1 ( lxMatrix44PTR mat1 , lxMatrix44CPTR mat2 ) ;
void lxMatrix44MultiplyRot2 ( lxMatrix44CPTR mat1 , lxMatrix44PTR mat2 ) ;
void lxMatrix44Orthonormalize ( lxMatrix44PTR dst , lxMatrix44PTR src ) ;
void lxMatrix44Transpose ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44Transpose1 ( lxMatrix44PTR mat ) ;
void lxMatrix44TransposeRot ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44TransposeRot1 ( lxMatrix44PTR mat ) ;
void lxMatrix44TransposeRotIdentity ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44Invert ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44Invert1 ( lxMatrix44PTR mat ) ;
void lxMatrix44AffineInvert ( lxMatrix44PTR dst , lxMatrix44CPTR src ) ;
void lxMatrix44Orient ( lxMatrix44PTR mat , lxVector3CPTR forward , lxVector3CPTR up , int axis ) ;
void lxMatrix44LookAt ( lxMatrix44PTR mat , lxVector3CPTR from , lxVector3CPTR to , lxVector3CPTR up ) ;
void lxMatrix44FromEulerZYX ( lxMatrix44PTR mat , lxVector3CPTR angles ) ;
void lxMatrix44FromEulerZYXFast ( lxMatrix44PTR mat , lxVector3CPTR angles ) ;
void lxMatrix44FromEulerZYXdeg ( lxMatrix44PTR mat , lxVector3CPTR angles ) ;
void lxMatrix44FromEulerXYZ ( lxMatrix44PTR mat , lxVector3PTR angles ) ;
void lxMatrix44FromEulerXYZFast ( lxMatrix44PTR mat , lxVector3PTR angles ) ;
void lxMatrix44FromAngleAxis ( lxMatrix44PTR mat , lxVector3CPTR axis , float cos , float sin , float oneminuscos ) ;
void lxMatrix44FromAngleAxisFast ( lxMatrix44PTR mat , float anglerad , lxVector3CPTR axis ) ;
void lxMatrix44RotateAngle ( lxMatrix44PTR mat , lxVector3PTR from , lxVector3PTR to ) ;
void lxMatrix44RotateAroundVector ( lxMatrix44PTR mat , lxVector3PTR axis , float cos , float sin , float oneminuscos ) ;
void lxMatrix44RotateAroundVectorFast ( lxMatrix44PTR mat , lxVector3PTR axis , float angleRad ) ;
void lxMatrix44RotateAroundPointFast ( lxMatrix44PTR mat , lxVector3CPTR center , lxVector3CPTR angles ) ;
void lxMatrix44ToEulerXYZ ( lxMatrix44CPTR mat , lxVector3PTR angles ) ;
void lxMatrix44ToEulerZYX ( lxMatrix44CPTR mat , lxVector3PTR angles ) ;
void lxMatrix44Perspective ( lxMatrix44PTR mat , const float fov , const float front , const float back , const float aspect ) ;
void lxMatrix44PerspectiveInf ( lxMatrix44PTR mat , const float fov , const float front , const float aspect ) ;
void lxMatrix44Ortho ( lxMatrix44PTR mat , const float height , const float front , const float back , const float aspect ) ;
void lxMatrix44OrthoDirect ( lxMatrix44PTR mat , const float left , const float right , const float bottom , const float top , const float front , const float back ) ;
void lxMatrix44PlaneProjection ( lxMatrix44PTR mat , lxVector3CPTR planenormal ) ;
void lxMatrix44ModifyProjectionClipplane ( lxMatrix44PTR projmatrix , lxMatrix44CPTR mview , lxMatrix44CPTR mviewinv , lxVector4CPTR clipPlane ) ;
void lxMatrix44Reflection ( lxMatrix44PTR mat , lxVector4PTR plane ) ;
float lxMatrix44CompareRot ( lxMatrix44PTR mat1 , lxMatrix44PTR mat2 ) ;
void lxMatrix44Swizzle1 ( lxMatrix44PTR mat , uint axis [ 3 ] , lxVector3PTR dirs ) ;
#pragma warning ( push )
#pragma warning ( disable : 4028 )
extern const float lx_gMatrix44_ident [ 16 ] ;
#pragma warning ( pop )
float16 lxFloat32To16 ( float fval ) ;
float lxFloat16To32 ( float16 ival ) ;
]]

return ffi.load("luxbackend")
