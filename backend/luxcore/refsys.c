// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/refsys.h>
#include <luxinia/luxcore/contvector.h>

  // for refsys (only in case you want to create it statically)
#include <luxinia/luxcore/contvector.h>
  typedef struct lxObjRefAllocSys_s{
    lxObjRef_t      *freerefs;
    struct lxObjRefPage_s *pagelist;
    size_t        lastinpage;
  }lxObjRefAllocSys_t;


  typedef struct lxObjRefSys_s{
    lxMemoryAllocatorPTR   allocator;
    int32       nodelete;
    int32       count;
    lxObjRefAllocSys_t  refalloc;
    lxContVector_t    typeinfovec;
    lxObjTypeInfo_t   *typeinfos;
    lxContVector_t    nodelvec;
  }lxObjRefSys_t;
//////////////////////////////////////////////////////////////////////////
#define OBJREFSYS_PAGESIZE        512
#define OBJREFSYS_ALLOC_PERPAGE ((OBJREFSYS_PAGESIZE-(sizeof(lxObjRef_t)))/(sizeof(lxObjRef_t)))

typedef struct lxObjRefPage_s{
  lxObjRef_t        next; // sacrifice one for pagelist and to keep alignment well
  lxObjRef_t        block[OBJREFSYS_ALLOC_PERPAGE];
}lxObjRefPage_t;

//////////////////////////////////////////////////////////////////////////

LUX_API lxObjRefSysPTR  lxObjRefSys_new(lxMemoryAllocatorPTR allocator)
{
  lxObjRefSysPTR refsys = (lxObjRefSysPTR)lxMemoryAllocator_malloc(allocator,sizeof(lxObjRefSys_t));
  lxObjRefSys_init(refsys,allocator);
  return refsys;
}
LUX_API void      lxObjRefSys_delete(lxObjRefSysPTR sys)
{
  lxObjRefSys_deinit(sys,NULL);
  lxMemoryAllocator_free(sys->allocator,sys,sizeof(lxObjRefSys_t));
}

LUX_API void lxObjRefSys_popNoDelete(lxObjRefSysPTR sys)
{
  if (--sys->nodelete == 0){
    while (lxContVector_size(&sys->nodelvec)){
      lxObjRef_t* cref = (lxObjRef_t*)lxContVector_back(&sys->nodelvec);

      // in the ugly case someone has created a new reference
      // within the NoDelete section
      if (cref->usecounter == 0){
        lxObjRefSys_deleteRef(sys,cref);
      }
      

      lxContVector_popBack(&sys->nodelvec);
    }
  }
  LUX_ASSERT(sys->nodelete >= 0);
}

LUX_API void lxObjRefSys_pushNoDelete(lxObjRefSysPTR sys)
{
  LUX_ASSERT(sys->nodelete >= 0);

  sys->nodelete++;
}

LUX_API int32 lxObjRefSys_refCount(lxObjRefSysPTR sys){
  return sys->count;
}

LUX_API void lxObjRefSys_init(lxObjRefSysPTR sys, lxMemoryAllocatorPTR allocator)
{
  memset(sys,0,sizeof(lxObjRefSys_t));
  sys->allocator = allocator;
  sys->refalloc.lastinpage = OBJREFSYS_ALLOC_PERPAGE;
  lxContVector_init(&sys->typeinfovec,allocator,sizeof(lxObjTypeInfo_t));
  lxContVector_init(&sys->nodelvec,allocator,sizeof(lxObjRefPTR));
}

LUX_API void lxObjRefSys_deinit(lxObjRefSysPTR sys, lxObjRefCheckDelete_fn *func)
{
  // cleanup all refs
  lxObjRefPage_t* pagelist = sys->refalloc.pagelist;

  while (sys->nodelete > 0){
    lxObjRefSys_popNoDelete(sys);
  }

  while (pagelist){
    lxObjRefPage_t* page = pagelist;
    uint32    i;

    pagelist = (lxObjRefPage_t*)page->next.id.ptr;

    for (i = 0; i < OBJREFSYS_ALLOC_PERPAGE; i++)
    {
      lxObjRef_t  *cref = &page->block[i];
      if (cref->id.type >= LUX_OBJREF_TYPE_USERSTART &&
        (!func || func(cref)) &&
        sys->typeinfos[cref->id.type].fnDelete)
      {
        sys->typeinfos[cref->id.type].fnDelete(cref);
      }
    }

    lxMemoryAllocator_free(sys->allocator,page,OBJREFSYS_PAGESIZE);
  }

  lxContVector_clear(&sys->typeinfovec);
  lxContVector_clear(&sys->nodelvec);
  {
    lxMemoryAllocatorPTR allocator = sys->allocator;
    memset(sys,0,sizeof(lxObjRefSys_t));
    sys->allocator = allocator;
  }
  
}

LUX_API void lxObjRefSys_register(lxObjRefSysPTR sys, lxObjRefType_t type, lxObjTypeInfo_t infocpy)
{
  static const lxObjTypeInfo_t nullinfo = {NULL};
  size_t sz = lxContVector_size(&sys->typeinfovec);

  LUX_ASSERT(type != LUX_OBJREF_TYPE_FREEALLOC);

  if ((size_t)type+1 > sz){
    lxContVector_resize(&sys->typeinfovec,(type+1),&nullinfo);
    sys->typeinfos = (lxObjTypeInfo_t*) lxContVector_front(&sys->typeinfovec);
  }
  sys->typeinfos[type] = infocpy;
}

LUX_API void lxObjRefSys_deleteAlloc(lxObjRefSysPTR sys, lxObjRefPTR data)
{
  lxObjRefAllocSys_t* allocator = &sys->refalloc;
  LUX_ASSERT( data->sys == sys);
  
  if (data){
    data->id.ptr = allocator->freerefs;
    data->id.type = LUX_OBJREF_TYPE_FREEALLOC;
    allocator->freerefs = data;
    sys->count--;
  }
}

static lxObjRef_t* lxObjRefSys_newAlloc(lxObjRefSysPTR sys){
  lxObjRefAllocSys_t* allocator = &sys->refalloc;
  lxObjRef_t* block;

  sys->count++;
  if ( block = allocator->freerefs){
    allocator->freerefs = (lxObjRef_t*)block->id.ptr;
    return block;
  }
  else{
    if (allocator->lastinpage == OBJREFSYS_ALLOC_PERPAGE){
      lxObjRefPage_t *page = (lxObjRefPage_t*)lxMemoryAllocator_calloc(sys->allocator,OBJREFSYS_PAGESIZE,1);
      page->next.id.ptr = allocator->pagelist;      
      allocator->pagelist = page;
      allocator->lastinpage = 0;
    }
    return &allocator->pagelist->block[allocator->lastinpage++];
  }
}

//////////////////////////////////////////////////////////////////////////


LUX_API lxObjRefPTR lxObjRefSys_newRef(lxObjRefSysPTR sys, lxObjRefType_t type, void *ptr)
{
  lxObjRefPTR cref = (lxObjRefPTR) lxObjRefSys_newAlloc(sys);

  LUX_ASSERT(type >= LUX_OBJREF_TYPE_USERSTART);

  cref->id.ptr = ptr;
  cref->id.type = type;
  cref->usecounter = 1;
  cref->weakcounter = 1;
  cref->sys = sys;

  return cref;
}


LUX_API void  lxObjRefSys_deleteRef(lxObjRefSysPTR sys, lxObjRefPTR cref)
{
  LUX_ASSERT( cref->sys == sys);

  if (!sys->nodelete){
    if (lxObjRef_getId(cref)){
      lxObjRefDelete_fn*  fndel = sys->typeinfos[cref->id.type].fnDelete;

      if (fndel){
        fndel(cref);
      }
    }
    cref->id.type = LUX_OBJREF_TYPE_DELETED;
    cref->id.ptr = NULL;
    lxObjRef_releaseWeak(cref);
  }
  else{
    lxContVector_pushBack(&sys->nodelvec,cref);
  }
}

LUX_API booln lxObjRefSys_invalidateRef(lxObjRefSysPTR sys,lxObjRefPTR cref, booln calldestructor)
{

  if (lxObjRef_getId(cref) && calldestructor){
    lxObjRefDelete_fn*  fndel = sys->typeinfos[cref->id.type].fnDelete;

    if (fndel){
      fndel(cref);
    }
  }
  cref->id.type = LUX_OBJREF_TYPE_DELETED;
  cref->id.ptr = NULL;
  return (cref->usecounter > 0);
}
