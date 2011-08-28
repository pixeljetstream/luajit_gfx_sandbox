// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_REFSYS_H__
#define __LUXCORE_REFSYS_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <luxinia/luxcore/memorybase.h>

#ifdef __cplusplus
extern "C"{
#endif

  // A system to manage reference counted objects.
  // Prevent cyclic refs with addWeak and releaseWeak
  // The object is kept alive as long as users exist or was invalidated by user.
  // The reference is kept alive as long as weak users exist
  // (object itself being one weak user)

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxObjRefSys_s* lxObjRefSysPTR;
  typedef struct lxObjRef_s* lxObjRefPTR;
  typedef void (lxObjRefDelete_fn)(lxObjRefPTR r);
  typedef booln (lxObjRefCheckDelete_fn)(lxObjRefPTR r);

  enum lxObjRefType_e{
    LUX_OBJREF_TYPE_DELETED = -1,
    LUX_OBJREF_TYPE_FREEALLOC = 0,
    LUX_OBJREF_TYPE_USERSTART,
  };
  typedef int32 lxObjRefType_t;

  typedef struct lxObjId_s{
    void*             ptr;
    lxObjRefType_t    type;
  }lxObjId_t;

  typedef struct lxObjTypeInfo_s{
    lxObjRefDelete_fn*  fnDelete;
    const char*         name;
  }lxObjTypeInfo_t;


  //////////////////////////////////////////////////////////////////////////
  // ObjRefSys

    // allocs mem & runs init
  LUX_API lxObjRefSysPTR  lxObjRefSys_new(lxMemoryAllocatorPTR allocator);

    // runs deinit &  deallocs mem
  LUX_API void      lxObjRefSys_delete(lxObjRefSysPTR sys);

    // standalone init/deinit
  LUX_API void      lxObjRefSys_init(lxObjRefSysPTR sys, lxMemoryAllocatorPTR allocator);

    // optionally calls func prior all delete operations
  LUX_API void      lxObjRefSys_deinit(lxObjRefSysPTR sys, lxObjRefCheckDelete_fn* func);

  LUX_API int32     lxObjRefSys_refCount(lxObjRefSysPTR sys);

    // within sections delete by release is prevented
    // all deletes are postponed to leaving section
  LUX_API void lxObjRefSys_pushNoDelete(lxObjRefSysPTR sys);
  LUX_API void lxObjRefSys_popNoDelete(lxObjRefSysPTR sys);

    // register a new type
  LUX_API lxObjTypeInfo_t lxObjTypeInfo_new(lxObjRefDelete_fn* fnDelete, const char*  name);
  LUX_API void lxObjRefSys_register(lxObjRefSysPTR sys, lxObjRefType_t type, lxObjTypeInfo_t info);

    // usecounter and weakcounter inited at 1
  LUX_API lxObjRefPTR lxObjRefSys_newRef(lxObjRefSysPTR sys, lxObjRefType_t type, void *ptr);

    // sets internal state to deleted, clears ptr
    // leaves counters unchanged, calldestructor ignores NoDelete status!
    // returns true if there were non-weak links left, which means
    // it's quite unsafe to do!
  LUX_API booln   lxObjRefSys_invalidateRef(lxObjRefSysPTR sys,lxObjRefPTR cref, booln calldestructor);
  

  //////////////////////////////////////////////////////////////////////////
  // ObjRef

    // returns ptr only if reference is still valid (wasn't deleted)
  LUX_API lxObjId_t*  lxObjRef_getId(lxObjRefPTR cref);

    // returns ptr (is NULL when already deleted, ie weak refs must check this)
  LUX_API void*   lxObjRef_getPtr(lxObjRefPTR cref);

    // if you allow creation of ObjRefs with NULL ptrs, than you must
    // use this instead of getPtr
  LUX_API booln   lxObjRef_getSafe(lxObjRefPTR cref, void **ptr);
    
    // increments weak counter
  LUX_API void    lxObjRef_addWeak(lxObjRefPTR cref);

    // returns true on success (ref is valid)
  LUX_API booln   lxObjRef_addUser(lxObjRefPTR cref);

  LUX_API void    lxObjRef_releaseWeak(lxObjRefPTR cref);

    // returns true if ref remains valid
  LUX_API booln   lxObjRef_releaseUser(lxObjRefPTR cref);

    // set usecounter to 0 (only allowed when usecounter is 1)
    // useful if you want to return "newobjects", 
    // use with caution, will not call destructor nor change weakcounter.
  LUX_API booln   lxObjRef_makeVolatile(lxObjRefPTR cref);


  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // Inline & Details

  typedef struct lxObjRef_s{
    lxObjId_t       id;
    int32           usecounter;
    int32           weakcounter;
    lxObjRefSysPTR  sys;
  }lxObjRef_t;


  // internals
  LUX_API void    lxObjRefSys_deleteRef(lxObjRefSysPTR sys, lxObjRefPTR cref);
  LUX_API void    lxObjRefSys_deleteAlloc(lxObjRefSysPTR sys, lxObjRefPTR cref);
  
  LUX_INLINE lxObjTypeInfo_t lxObjTypeInfo_new(lxObjRefDelete_fn* fnDelete, const char* name)
  {
    lxObjTypeInfo_t info;
    info.fnDelete = fnDelete;
    info.name = name;
    return info;
  }

  LUX_INLINE void lxObjRef_releaseWeak(lxObjRefPTR cref)
  {
    if (--cref->weakcounter == 0 ){
      lxObjRefSys_deleteAlloc(cref->sys,cref);
    }
  }

  LUX_INLINE booln  lxObjRef_releaseUser(lxObjRefPTR cref)
  {
    booln valid = (cref->id.type >= LUX_OBJREF_TYPE_USERSTART);
    if( --cref->usecounter == 0 )
    {
      lxObjRefSys_deleteRef(cref->sys,cref);
      valid = LUX_FALSE;
    }

    return valid;
  }

  LUX_INLINE booln  lxObjRef_makeVolatile(lxObjRefPTR cref)
  {
    if (cref->id.type < LUX_OBJREF_TYPE_USERSTART || cref->usecounter != 1) return LUX_FALSE;
    cref->usecounter = 0;
    return LUX_TRUE;
  }
  LUX_INLINE booln  lxObjRef_getSafe(lxObjRefPTR cref, void **ptr)
  {
    booln valid = (cref && cref->id.type >= LUX_OBJREF_TYPE_USERSTART);
    *ptr = valid ? cref->id.ptr : NULL;
    return valid;
  }

  LUX_INLINE void*  lxObjRef_getPtr(lxObjRefPTR cref)
  {
    return cref->id.ptr;
  }
  LUX_INLINE lxObjId_t* lxObjRef_getId(lxObjRefPTR cref)
  {
    return cref->id.type >= LUX_OBJREF_TYPE_USERSTART ? (lxObjId_t*)cref : NULL;
  }

  LUX_INLINE void lxObjRef_addWeak(lxObjRefPTR cref)
  {
    cref->weakcounter++;
  }


  LUX_INLINE booln  lxObjRef_addUser(lxObjRefPTR cref)
  {
    if (cref->id.type < LUX_OBJREF_TYPE_USERSTART) return LUX_FALSE;

    cref->usecounter++;
    return LUX_TRUE;
  }


#ifdef __cplusplus
}
#endif


#endif
