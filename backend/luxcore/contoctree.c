// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/contoctree.h>
#include <luxinia/luxcore/contmacrolinkedlist.h>
#include <luxinia/luxplatform/debug.h>
#include <luxinia/luxmath/vector3.h>
#include <luxinia/luxmath/vector4.h>



//////////////////////////////////////////////////////////////////////////
// DEFS

#define OCTREE_NODELIST_MIN     4
#define LUX_OCTREE_NUMCHILDS      8

typedef struct lxOcNodeBands_s{
  lxOcContainerBox_t  **final;
  lxOcContainerBox_t  **temp;
}lxOcNodeBands_t;

typedef struct lxOcTree_s {
  lxMemoryAllocatorPTR  allocator;
  lxOcBounds_t      volume;
  lxOcNode_t      *root;
  uint        maxdepth;
  uint        memsize;
  uint        memuse;
  uint        containerpos;
  byte        *memblock;
  void**        dataList;

  // special iterators/data 
  lxOcContactTest_fn  *contactTester;
  lxOcNodeBuild_fn    *nodeBuild;
  uint        nodeExtraAlloc;
  uint        containerExtraAlloc;
  void*       upvalue;
} OcTree_t;

static lxOcTravStack_t l_traverseStack;

static lxOcNode_t* OcTree_getNode(OcTree_t* self, lxOcContainerBox_t** liststart, size_t dataoffset);

////////////////////////////////////////////////////////////////////////////////
// OcBounds

static LUX_INLINE void OcBounds_init (lxOcBounds_t *self, float x, float y, float z, float w, float h, float d)
{

  self->min[0] = x-(w*0.5f);
  self->min[1] = y-(h*0.5f);
  self->min[2] = z-(d*0.5f);
  self->max[0] = x+(w*0.5f);
  self->max[1] = y+(h*0.5f);
  self->max[2] = z+(d*0.5f);
}

static LUX_INLINE void OcBounds_invalidate (lxOcBounds_t *self)
{

  self->min[0] = self->min[1] = self->min[2] = FLT_MAX;
  self->max[0] = self->max[1] = self->max[2] = -FLT_MAX;

}

static LUX_INLINE booln OcBounds_intersects(lxOcBounds_t* self, lxOcBounds_t* other){
  return (  (self)->min[0]<=(other)->max[0] && 
    (self)->min[1]<=(other)->max[1] &&
    (self)->min[2]<=(other)->max[2] &&
    (self)->max[0]>=(other)->min[0] &&
    (self)->max[1]>=(other)->min[1] &&
    (self)->max[2]>=(other)->min[2] );
}

static LUX_INLINE void OcBounds_copy(lxOcBounds_t* LUX_RESTRICT dst, lxOcBounds_t* LUX_RESTRICT src){
  memcpy(dst,src,sizeof(lxOcBounds_t));
}

static LUX_INLINE void OcBounds_merge(lxOcBounds_t* LUX_RESTRICT bounds, lxOcBounds_t* LUX_RESTRICT inbounds)
{
  bounds->min[0] = LUX_MIN(bounds->min[0],inbounds->min[0]);
  bounds->min[1] = LUX_MIN(bounds->min[1],inbounds->min[1]);
  bounds->min[2] = LUX_MIN(bounds->min[2],inbounds->min[2]);

  bounds->max[0] = LUX_MAX(bounds->max[0],inbounds->max[0]);
  bounds->max[1] = LUX_MAX(bounds->max[1],inbounds->max[1]);
  bounds->max[2] = LUX_MAX(bounds->max[2],inbounds->max[2]);
}

//////////////////////////////////////////////////////////////////////////
// OcCenterBox

static LUX_INLINE void OcCenterBox_init (lxOcCenterBox_t *self, float x, float y, float z, float w, float h, float d)
{

  self->center[0] = x;
  self->center[1] = y;
  self->center[2] = z;
  self->size[0] = w;
  self->size[1] = h;
  self->size[2] = d;

  OcBounds_init(&self->bounds,x,y,z,w,h,d);
}

static LUX_INLINE booln OcCenterBox_intersects(lxOcCenterBox_t *self, lxOcCenterBox_t *other){
    return (OcBounds_intersects(&self->bounds,&other->bounds));
}

static LUX_INLINE booln OcNode_intersectsCenter(lxOcNode_t *self, lxOcCenterBox_t *other){
  return (OcBounds_intersects(&self->bounds,&other->bounds));
}



////////////////////////////////////////////////////////////////////////////////
// OcNode


static LUX_INLINE lxOcNode_t* OcNode_add(lxOcNode_t *self, lxOcContainerBox_t *container)
{
  if (container == NULL) return NULL;

  self->dataList[self->listCount] = container->data;
  self->list[self->listCount++] = container;

  OcBounds_merge(&self->volume,&container->box.bounds);
  return self;
}

static LUX_INLINE void OcNode_invalidateVolume(lxOcNode_t *self)
{
  // set volume to invalid values
  self->volume.min[0] = self->bounds.max[0];  
  self->volume.min[1] = self->bounds.max[1];
  self->volume.min[2] = self->bounds.max[2];
  self->volume.max[0] = self->bounds.min[0];
  self->volume.max[1] = self->bounds.min[1];
  self->volume.max[2] = self->bounds.min[2];

}

static void OcNode_prepSize(lxOcNode_t *self,lxVector3 ctr, lxVector3 dim)
{
  float cx,cy,cz,w,h,d,max;

  cx = (self->bounds.max[0]+self->bounds.min[0])*0.5f;
  cy = (self->bounds.max[1]+self->bounds.min[1])*0.5f;
  cz = (self->bounds.max[2]+self->bounds.min[2])*0.5f;
  w = self->bounds.max[0]-self->bounds.min[0];
  h = self->bounds.max[1]-self->bounds.min[1];
  d = self->bounds.max[2]-self->bounds.min[2];


#define MAX3(a,b,c) (((a)>(b))?((a)>(c))?(a):(c):((b)>(c))?(b):(c))
  max = MAX3(w,h,d);

  // the bounding box seems to be planar => make it quadtreelike by modifying the bounds
  if (max>2*w) {
    w*=2.01f;
    cx+=w*0.25f;
  }
  if (max>2*h) {
    h*=2.01f;
    cy+=h*0.25f;
  }
  if (max>2*d) {
    d*=2.01f;
    cz+=d*0.25f;
  }

  OcNode_invalidateVolume(self);

  lxVector3Set(ctr,cx,cy,cz);
  lxVector3Set(dim,w,h,d);
}

static lxOcNode_t* OcNode_init(lxOcNode_t *self, lxOcContainerBox_t** liststart, void** dataList)
{

  if (self==NULL) return NULL;
  OcBounds_invalidate(&self->volume);

  memset(self->childs,0,sizeof(lxOcNode_t*)*LUX_OCTREE_NUMCHILDS);

  self->listCount = 0;
  self->childListCount = 0;
  self->list = liststart;
  self->dataList = dataList;

  return self;
}

static booln OcNode_buildSplit(lxOcNode_t *self, OcTree_t *tree, lxOcNodeBands_t bands)
{
  uint  counters[LUX_OCTREE_NUMCHILDS+1] = {0,0,0,0, 0,0,0,0, 0};
  lxOcNode_t* childs[LUX_OCTREE_NUMCHILDS+1];

  uint  listCount = self->listCount;
  uint  i;
  lxVector3 ctr,dim;
  lxOcContainerBox_t**  finallist = self->list;
  lxOcContainerBox_t**  newlist   = bands.temp;
  booln outofmem = LUX_FALSE;
  size_t  offset = finallist - bands.final;

  OcNode_prepSize(self,ctr,dim);

  /*
    Uses two memory bands to generate the output container pointer lists
    original list = own list + child lists

      final band: |..... <start... original list ...end>  ...other |

    1. offsets need to be created for child and own into temp band
      temp band:  |<child lists...    ....own list>     ....unused |

    2. generate lists into temp
    3. copy content of temp to original start
      final band: |..... <child lists...    ...own list>  ...other |

    4. rebase old ptrs from temp with respect to final start

  */

  // first pass counts and classifies stuff
  for (i = 0; i < listCount; i++){
    lxOcContainerBox_t *container = finallist[i];
    int sector;

    if (container->box.size[0]*2.0f < dim[0] &&   // cb removed || 
      container->box.size[1]*2.0f < dim[1] &&
      container->box.size[2]*2.0f < dim[2])
    {
      int a = ctr[0] > container->box.center[0];
      int b = ctr[1] > container->box.center[1];
      int c = ctr[2] > container->box.center[2];
      sector = (a<<2)|(b<<1)|c;
    }
    else{
      sector = LUX_OCTREE_NUMCHILDS;
    }

    container->sector = sector;
    counters[sector]++;
  }


  // generate children and setup ptrs
  // all stuff is put into temp first

  for (i = 0; i < LUX_OCTREE_NUMCHILDS; i++){
    if (counters[i]){
      float a = (float)((i & (1<<2)) != 0);
      float b = (float)((i & (1<<1)) != 0);
      float c = (float)((i & (1<<0)) != 0);

      lxOcNode_t *newnode = OcTree_getNode(tree,newlist,offset);

      if (newnode){
        childs[i] = newnode;

        offset  += counters[i];
        newlist += counters[i];
      }
      else{
        outofmem = LUX_TRUE;

        // add self as dummy child, for easier insertion
        childs[i] = self;
      }
    }
  }
  childs[LUX_OCTREE_NUMCHILDS] = self;
  self->list = newlist;
  self->listCount = 0;
  self->dataList = tree->dataList+offset;

  // now copy into temp, and build tight node->volumes
  for (i = 0; i < listCount; i++){
    lxOcContainerBox_t *container = finallist[i];
    OcNode_add(childs[container->sector],container);
  }
  


  // cpy new layout back to final
  memcpy(finallist,bands.temp,sizeof(lxOcContainerBox_t*)*listCount);

  // rebase lists
  for (i = 0; i < LUX_OCTREE_NUMCHILDS; i++){
    lxOcNode_t* child = childs[i];

    if (counters[i] && child != self){
      self->childs[i] = child;
      child->list = finallist + (child->list - bands.temp);
      // correct the actual bounds to content bounds
      OcBounds_copy(&child->bounds,&child->volume);
    }
  }
  self->list = finallist + (self->list - bands.temp);
  self->childListCount = listCount - self->listCount;

  return outofmem;
}

static booln OcNode_build(lxOcNode_t *self, OcTree_t *tree, lxOcNodeBands_t bands, int restDepth)
{

  int pos,depth;
  booln outofmem = LUX_FALSE;

  pos = 1;
  l_traverseStack.items[0].node = self;
  l_traverseStack.items[0].depth = restDepth;

  while (pos>0) {
    depth = l_traverseStack.items[--pos].depth;
    self  = l_traverseStack.items[pos].node;

    if (self->listCount && self->listCount > OCTREE_NODELIST_MIN && !outofmem)
    {
      outofmem |= OcNode_buildSplit(self,tree,bands);
    }

    if (depth>0) {
      int i;
      for (i = 0;i < LUX_OCTREE_NUMCHILDS;i++)
      {
        if (self->childs[i]!=NULL) {
          l_traverseStack.items[pos].node    = self->childs[i];
          l_traverseStack.items[pos++].depth = depth - 1;
          LUX_DEBUGASSERT(LUX_OCTREE_MAX_STACKITEMS!=pos);// stack overflow of local stack
        }
      }
    }
  }

  return outofmem;
}

static void OcNode_traverse(lxOcNode_t *self, lxOcTravStack_t *threadstack, lxOcTraverse_fn *traversefn,
  int depth,lxOcCenterBox_t *box, void *upvalue)
{
  int i;
  int pos;

  pos = 1;
  threadstack->items[0].node = self;
  threadstack->items[0].depth = 0;

  while (pos>0) {
    depth = threadstack->items[--pos].depth;
    self  = threadstack->items[pos].node;
    if (OcBounds_intersects(&self->volume,&box->bounds) &&
      !traversefn(self,depth,upvalue)) continue;

    for (i=0;i<LUX_OCTREE_NUMCHILDS;i++)
      if (self->childs[i]!=NULL) {
        threadstack->items[pos].node = self->childs[i];
        threadstack->items[pos++].depth = depth + 1;
        LUX_DEBUGASSERT(LUX_OCTREE_MAX_STACKITEMS!=pos);// stack overflow of local stack
      }
  }
}

static void OcNode_addContact (lxOcNode_t *self, lxOcTravStack_t *threadstack, lxOcContainerBox_t *contbox,
  lxOcContactTest_fn *tester, int depth, void *upvalue)
{
  int i;
  int pos;

  pos = 1;
  threadstack->items[0].node = self;
  threadstack->items[0].depth = 0;

  while (pos>0) {
    depth = threadstack->items[--pos].depth;
    self = threadstack->items[pos].node;

    if (!tester(contbox,self,depth,upvalue)) continue;

    for (i = 0;i<LUX_OCTREE_NUMCHILDS;i++)
      if (self->childs[i]!=NULL)
        if (OcNode_intersectsCenter(self->childs[i],&contbox->box))
      {
        threadstack->items[pos].node = self->childs[i];
        threadstack->items[pos++].depth = depth + 1;
        LUX_DEBUGASSERT(LUX_OCTREE_MAX_STACKITEMS!=pos); // stack overflow of local stack
      }
  }
}

static void OcNode_draw_recursive(lxOcNode_t *self,int depth,lxOcDrawBox_fn *drawfunc, const int drawfrom, const int drawto)
{
  lxVector4 color;
  uint i;

  if (depth < drawfrom || depth > drawto);
  else{
    float range = (float)(drawto-drawfrom);
    float depthcolor = (1-((depth-drawfrom)/((range<1)? 1 : range))*0.8f);
    
    lxVector4Set(color,depthcolor,0.0f,0.0f,1.0f);
    drawfunc(self->volume.min,self->volume.max,color);
    if (self->list!=NULL) {
      lxVector4Set(color,0.0f,depthcolor,0.0f,1.0f);
      drawfunc(self->bounds.min,self->bounds.max,color);

      for (i = 0; i < self->listCount; i++){
        lxOcContainerBox_t* container = self->list[i];
        lxVector4Set(color,0.0f,0.0f,depthcolor,1.0f);
        drawfunc(container->box.bounds.min,container->box.bounds.max,color);
      }
    }
  }
  for (i = 0;i < LUX_OCTREE_NUMCHILDS;i++)
    if (self->childs[i]!=NULL)
      OcNode_draw_recursive(self->childs[i],depth+1,drawfunc,drawfrom,drawto);
}




////////////////////////////////////////////////////////////////////////////////
// Container functions
static lxOcContainerBox_t* OcContainerBox_init (lxOcContainerBox_t* self, void *data,
  float x, float y, float z, float w, float h, float d)
{
  OcCenterBox_init(&self->box,x,y,z,w,h,d);
  self->data = data;

  return self;
}

////////////////////////////////////////////////////////////////////////////////
// OcTree functions

LUX_API void lxOcTree_reset(OcTree_t *self)
{
  self->memuse = 0;
  self->containerpos = 0;
  self->root = NULL;
  self->containerExtraAlloc = 0;
  self->nodeExtraAlloc = 0;
}

static void* OcTree_malloc(OcTree_t* self,int size)
{
  byte* pos;

  if (self->memuse+size >= self->memsize) {
    int oldsize = self->memsize;

    if (self->containerpos!=0) return NULL;

    self->memsize=self->memsize*2+size;
    self->memblock = (byte*)lxMemoryAllocator_realloc(self->allocator,self->memblock,self->memsize,oldsize);
  }

  pos = &self->memblock[self->memuse];
  self->memuse+=size;

  return pos;
}

static lxOcNode_t* OcTree_getNode(OcTree_t* self, lxOcContainerBox_t** liststart, size_t dataoffset)
{
  lxOcNode_t* node = OcNode_init((lxOcNode_t*)OcTree_malloc(self,sizeof(lxOcNode_t)+self->nodeExtraAlloc),liststart,self->dataList+dataoffset);
  if (node && self->nodeBuild) self->nodeBuild( node, self->upvalue);

  return node;
}

LUX_API void lxOcTree_traverse(OcTree_t *self, lxOcTravStack_t *threadstack, lxOcTraverse_fn *traversefn,
  float cx, float cy, float cz, float w, float h, float d, void *upvalue)
{
  lxOcCenterBox_t box;

  if (self->root == NULL) return;
  box.center[0] = cx;
  box.center[1] = cy;
  box.center[2] = cz;
  box.size[0] = w;
  box.size[1] = h;
  box.size[2] = d;
  box.bounds.min[0] = cx-w*0.5f;
  box.bounds.min[1] = cy-h*0.5f;
  box.bounds.min[2] = cz-d*0.5f;
  box.bounds.max[0] = cx+w*0.5f;
  box.bounds.max[1] = cy+h*0.5f;
  box.bounds.max[2] = cz+d*0.5f;

  threadstack = threadstack ? threadstack : &l_traverseStack;
  OcNode_traverse(self->root,threadstack,traversefn,0,&box,upvalue);
}

LUX_API booln lxOcTree_build(OcTree_t *self, int maxDepth, uint nodeExtraMem, lxOcNodeBuild_fn* nodefunc, void *upvalue)
{
  lxOcNodeBands_t bands;
  byte *box;
  byte *fin;
  uint i;
  uint  occontsize = sizeof(lxOcContainerBox_t) + self->containerExtraAlloc;
  uint  numContainers = self->memuse/occontsize;
  uint minmemsize = numContainers * (occontsize + sizeof(lxOcContainerBox_t*)*2 + sizeof(void*)) + 
            sizeof(lxOcNode_t) + nodeExtraMem;

  if (self->memuse == 0) 
    return LUX_FALSE;

  self->maxdepth = LUX_MIN(LUX_OCTREE_MAX_DEPTH,maxDepth);

  if (self->memsize/self->memuse < 4 || self->memsize < minmemsize) {
    int oldsize = self->memsize;
    self->memsize *= 4;
    self->memsize = LUX_MAX(minmemsize,self->memsize);
    self->memblock = (byte*)lxMemoryAllocator_realloc(self->allocator,self->memblock,self->memsize,oldsize);
  }

  self->nodeExtraAlloc = nodeExtraMem;
  self->nodeBuild = nodefunc;
  self->upvalue = upvalue;

  box = self->memblock;
  fin = self->memblock + self->memuse;
  self->containerpos = self->memuse;

  bands.final = (lxOcContainerBox_t**)OcTree_malloc(self,sizeof(lxOcContainerBox_t*)*numContainers);
  bands.temp  = (lxOcContainerBox_t**)OcTree_malloc(self,sizeof(lxOcContainerBox_t*)*numContainers);
  
  self->dataList = (void**)OcTree_malloc(self,sizeof(void*)*numContainers); 

  
  self->root = OcTree_getNode(self,bands.final,0);
  OcBounds_copy(&self->root->bounds,&self->volume);
  OcBounds_copy(&self->root->volume,&self->volume);
  self->root->listCount = numContainers;

  i = 0;
  while (box < fin) {
    self->dataList[i] = ((lxOcContainerBox_t*)box)->data;
    bands.final[i++] = (lxOcContainerBox_t*)box;
    box += occontsize;
  }

  if (maxDepth>1) {
    return OcNode_build(self->root,self,bands,maxDepth-1);
  }

  return LUX_FALSE;
}

LUX_API void lxOcTree_collide (OcTree_t *self, lxOcTravStack_t *threadstack, OcTree_t *other, lxOcContactTest_fn *contactTester,void *upvalue)
{
  byte *box = other->memblock;
  byte *fin = other->memblock + other->containerpos;
  size_t occontsize = sizeof(lxOcContainerBox_t) + other->containerExtraAlloc;

  if (!contactTester || !self->root)
    return;

  threadstack = threadstack ? threadstack : &l_traverseStack;
  while (box < fin) {
    OcNode_addContact(self->root,threadstack,(lxOcContainerBox_t*)box,contactTester,0,upvalue);
    box += occontsize;
  }
}

static void OcNode_getLists_recursive(lxOcNode_t *self, lxOcListNodeTraverse_fn *listcollector, void *upvalue)
{
  int i;
  listcollector(self->listCount, self->list, self->dataList, upvalue);
  for (i=0;i<LUX_OCTREE_NUMCHILDS;i++){
    if (self->childs[i]!=NULL){
      OcNode_getLists_recursive(self->childs[i],listcollector,upvalue);
    }
  }
}


LUX_API void lxOcNode_getDeepLists(lxOcNode_t *self, lxOcListNodeTraverse_fn *listcollector, void *upvalue)
{
  void**        firstData = self->dataList-self->childListCount;
  lxOcContainerBox_t** first = self->list-self->childListCount;
  lxOcContainerBox_t** last = self->list+self->listCount;
  
  listcollector(last-first, first, firstData, upvalue);

}

LUX_API void lxOcTree_getLists(OcTree_t *self,lxOcListNodeTraverse_fn *listcollector, void *upvalue)
{
  if (self->root){
    lxOcNode_getDeepLists(self->root,listcollector,upvalue);
  }
}

LUX_API void lxOcTree_containerExtraMem(lxOcTreePTR self, uint containerExtraMem)
{
  LUX_ASSERT(self->memuse == 0);
  self->containerExtraAlloc = containerExtraMem;    
}

LUX_API void* lxOcTree_add (OcTree_t *self, void *data, float x, float y, float z,
  float w, float h, float d)
{
  lxOcContainerBox_t *container;

  // Octree_add not allowed after build
  LUX_ASSERT(self->containerpos == 0);

  container = OcContainerBox_init((lxOcContainerBox_t*)OcTree_malloc(self,
    sizeof(lxOcContainerBox_t) + self->containerExtraAlloc),data,x,y,z,w,h,d);

  if (self->memuse == 0)
  { // first element that was added => set boundingbox to this object
    OcBounds_copy(&self->volume,&container->box.bounds);
  } else {
    OcBounds_merge(&self->volume,&container->box.bounds);
  }

  return self->containerExtraAlloc ? container+1 : NULL;
}

LUX_API OcTree_t* lxOcTree_new (lxMemoryAllocatorPTR allocator, uint startmemorysize)
{
  OcTree_t *self = NULL;

  self = (OcTree_t*)lxMemoryAllocator_calloc(allocator,sizeof(OcTree_t),1);
  self->allocator = allocator;
  self->memsize = startmemorysize;
  self->memuse = 0;
  self->containerpos = 0;
  self->memblock = (byte*)lxMemoryAllocator_malloc(allocator,self->memsize);
  lxOcTree_reset(self);

  return self;
}

LUX_API void lxOcTree_delete (OcTree_t *self)
{
  lxMemoryAllocator_free(self->allocator,self->memblock,self->memsize);
  lxMemoryAllocator_free(self->allocator,self,sizeof(OcTree_t));
}

LUX_API void lxOcTree_draw (OcTree_t *self, int fromdepth, int maxdepth, lxOcDrawBox_fn *drawfunc)
{
  lxVector4 color = {0.0f,1.0f,0.0f,1.0f};
  if (maxdepth < 0)
    maxdepth = self->maxdepth;

  drawfunc(self->volume.min,self->volume.max,color);
  if (self->root!=NULL)
    OcNode_draw_recursive(self->root,1,drawfunc,fromdepth,maxdepth);
}