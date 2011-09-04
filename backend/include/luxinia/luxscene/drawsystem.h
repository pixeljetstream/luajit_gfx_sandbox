// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXSCENE_DRAWSYS_H__
#define __LUXSCENE_DRAWSYS_H__


#include <luxinia/luxscene/shader.h>
#include <luxinia/luxscene/meshbase.h>
#include <luxinia/luxmath/basetypes.h>

#ifdef __cplusplus
extern "C"{
#endif

  typedef struct lxDrawBounding_s{
    lxBoundingSphere_t  bsphere;
    lxBoundingBox_t     bbox;
  }lxDrawBounding_t;

  typedef struct lxDrawSpatial_s{
    lxMatrix44PTR       localMatrix;
    lxMatrix44PTR       localMatrixInv;
    lxDrawBounding_t*   localBounding;
    lxMatrix44PTR       worldMatrix;
    lxMatrix44PTR       worldMatrixInv;
    lxDrawBounding_t*   worldBounding;
    struct lxDrawSpatial_s*  sibling;
    struct lxDrawSpatial_s*  children;
    // -- 128 byte barrier in 64-bit --
    struct lxDrawSpatial_s*  parent;
    uint32              numChildren;
    lxContVector_t      drawBoundings;
    lxContVector_t      drawItems;
  }lxDrawSpatial_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxDrawInfo_s{
    lxGLPrimitiveType_t   primitive;
    uint32                instanceCount;
    uint32                primCount;
    uint32                firstOffset;
    int32                 vertexBase;
    uint32                vertexBaseOffset;
  }lxDrawInfo_t;

  typedef struct lxDrawGeometry_s{
    uint32                geometryID;
    lxgVertexDeclPTR      vertexDecl;
    lxScalarType_t        indexType;
    lxgStreamHost_t       indexStream;
    lxgStreamHost_t       vertexStreams[LUXGFX_MAX_VERTEX_STREAMS];
  }lxDrawGeometry_t;

  typedef struct lxDrawItem_s{
    flags32                   userFlags;
    uint32                    sortKey;
    uint32                    geometryID;
    uint32                    materialID;
    lxDrawInfo_t              drawinfo;
    lxDrawGeometry_t*         geometry;
    lxShaderLevel_t           itemLevel;
    lxDrawSpatial_t*          spatial;
  }lxDrawItem_t;


  //////////////////////////////////////////////////////////////////////////

  LUX_API void  lxDrawSpatial_init(lxDrawSpatial_t* spatial, lxMemoryAllocatorPTR allocator);
  LUX_API void  lxDrawSpatial_deinit(lxDrawSpatial_t* spatial);

  LUX_API void  lxDrawSpatial_updateTree(lxDrawSpatial_t* root);

  LUX_API void  lxDrawSpatial_addBounding(lxDrawSpatial_t* spatial, const lxDrawBounding_t* bounding);
  LUX_API void  lxDrawSpatial_remBounding(lxDrawSpatial_t* spatial, const lxDrawBounding_t* bounding);
  LUX_API void  lxDrawSpatial_updateBounding(lxDrawSpatial_t* spatial);

  LUX_API uint32 lxDrawSpatial_getVisibleItems(lxDrawSpatial_t* root, lxFrustumCPTR frustum, lxDrawItem_t* itembuffer, size_t maxItems);

  //////////////////////////////////////////////////////////////////////////

  LUX_API void lxDrawItem_init(lxDrawItem_t* draw, lxDrawGeometry_t* geometry, uint32 materialID, lxDrawSpatial_t* spatial);
  LUX_API void lxDrawItem_setGeometry(lxDrawItem_t* draw);
  LUX_API void lxDrawItem_setMaterialID(lxDrawItem_t* draw);
  LUX_API void lxDrawItem_setSpatial(lxDrawItem_t* draw);
  LUX_API void lxDrawItem_update(lxDrawItem_t* draw);
  LUX_API void lxDrawItem_deinit(lxDrawItem_t* draw);




#ifdef __cplusplus
}
#endif


#endif

