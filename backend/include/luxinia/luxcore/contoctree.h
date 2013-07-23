// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_OCTREE_H__
#define __LUXCORE_OCTREE_H__

//////////////////////////////////////////////////////////////////////////
// OcTree
//
// NOT THREADSAFE!! except traverse & collide with per-thread stacks

#include <luxinia/luxplatform/luxplatform.h>
#include <luxinia/luxcore/memorybase.h>

#ifdef __cplusplus
extern "C"{
#endif

  enum {
    LUX_OCTREE_MAX_DEPTH = 31,
    LUX_OCTREE_MAX_STACKITEMS = ((LUX_OCTREE_MAX_DEPTH+1)*8),
    LUX_OCTREE_NUMCHILDS = 8,
  };

typedef struct lxOcBounds_s
{
  float   min[4];     // keep order
  float   max[4];
}lxOcBounds_t;

typedef struct lxOcCenterBox_s
{
  float   center[3];
  float   size[3];
  lxOcBounds_t  bounds;
}lxOcCenterBox_t;


typedef struct lxOcContainerBox_s {
  lxOcCenterBox_t box;
  int       sector;
  void      *data;
} lxOcContainerBox_t;

typedef struct lxOcNode_s {
  lxOcBounds_t      bounds; // the bbox of the node's children
  lxOcBounds_t      volume; // the bbox of the containers in the list
  uint        listCount;
  lxOcContainerBox_t  **list;
  void        **dataList; 

  uint        childListCount;
  struct lxOcNode_s   *childs[LUX_OCTREE_NUMCHILDS];
} lxOcNode_t;

typedef struct lxOcTravStackItem_s {
  struct lxOcNode_s   *node;
  int         depth;
} lxOcTravStackItem_t;

typedef struct lxOcTravStack_s {
  lxOcTravStackItem_t items[LUX_OCTREE_MAX_STACKITEMS];
}lxOcTravStack_t;

typedef struct lxOcTree_s*  lxOcTreePTR;

  // called after OcNode is created
  // can be used to write to appended customdata per node
  // custom data will start after sizeof(OcNode_t)
typedef void (lxOcNodeBuild_fn) (lxOcNode_t* node,void *upvalue);

  // container - containerbox of 'other' tree
  // node - the Ocnode it intersects with
  // depth - current depth in the tree
  // return 0 to stop branching
typedef int (lxOcContactTest_fn) (lxOcContainerBox_t* occontainer, lxOcNode_t* node, int depth,void *upvalue);


  // node - the Ocnode it intersects with
  // depth - how far we are in the tree
  // return 0 to stop branching
typedef int (lxOcTraverse_fn) (lxOcNode_t* node, int depth, void *upvalue);

  // count - number of list items in the current ocnode
  // list - listnodes in the current ocnode
typedef void (lxOcListNodeTraverse_fn) (uint listcount, lxOcContainerBox_t** list, void **dataList, void *upvalue);

  // for drawing the octree
typedef void (lxOcDrawBox_fn) (const float minb[3], const float maxb[3], const float color[4]);

//////////////////////////////////////////////////////////////////////////
// OcNode

  // calls listcollector once on the content of self and all children 
LUX_API void lxOcNode_getDeepLists(lxOcNode_t *self, lxOcListNodeTraverse_fn *listcollector, void *upvalue);


////////////////////////////////////////////////////////////////////////////////
// OcTree


  // constructor
LUX_API lxOcTreePTR lxOcTree_new (lxMemoryAllocatorPTR allocator, uint startmemorysize);
  // destructor
LUX_API void lxOcTree_delete (lxOcTreePTR self);

  // after adding ALL items to the tree, this function must be called
  // the allocated memory per OcNode can be expanded to store custom data
  // ocNodeBuild can be passed to be called after OcNode was allocated
  //  returns TRUE if it ran out of memory, the OcTree is still functional
  //  but not completely distributed.
  //  Every build increases internal membuffer for next build
LUX_API booln lxOcTree_build(lxOcTreePTR  self, int maxDepth, uint nodeExtraMem, lxOcNodeBuild_fn* nodefunc, void *upvalue);

  // allows extra data to be copied at end of "add"
  // Can only be changed after "new" or "reset"
LUX_API void lxOcTree_containerExtraMem(lxOcTreePTR self, uint containerExtraMem);

  // adds an object to the tree, w,h,d are the diameters, so for a sphere of radius r, 
  // w = 2*r, ...
  // You can copy extra data (if OcTree has containerExtraMem)
  // to the pointer returned. This pointer will only be valid
  // until the next add!
LUX_API void* lxOcTree_add (lxOcTreePTR self, void *data, float x, float y, float z, 
  float w, float h, float d);

  // clears all objects from the tree - should be called after build() before add()
LUX_API void lxOcTree_reset(lxOcTreePTR self);

  // draws the octree if maxdepth is -1 we will use self's maxdepth
LUX_API void lxOcTree_draw (lxOcTreePTR self, int fromdepth, int maxdepth, lxOcDrawBox_fn *drawfunc);

  // traverses the octree's node which intersects the given volume. 
  // each node has a list of containers with your data. You must traverse the list
  // in order to check your containers. You can stop traversal of the current
  // branch (but not the traversal).
  // if threadstack is NULL, default is used (ie not threadsafe)
LUX_API void lxOcTree_traverse(lxOcTreePTR  self, lxOcTravStack_t *threadstack, lxOcTraverse_fn *traversefn,
  float cx, float cy, float cz, float w, float h, float d, void *upvalue);
  
  // similiar to traverse, if the traversing function returns 0, the branching is
  // stopped. Quickaccepts or rejects can be made this way
  // if threadstack is NULL, default is used (ie not threadsafe)
LUX_API void lxOcTree_collide (lxOcTreePTR  self, lxOcTravStack_t *threadstack, lxOcTreePTR other, lxOcContactTest_fn *contactTester, void *upvalue);


  // similar to above but for the whole tree
LUX_API void lxOcTree_getLists(lxOcTreePTR  self,lxOcListNodeTraverse_fn *listcollector, void *upvalue);


#ifdef __cplusplus
};  
#endif

#endif

