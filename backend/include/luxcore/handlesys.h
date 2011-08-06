// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_HANDLESYS_H__
#define __LUXCORE_HANDLESYS_H__

#include <luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif


  typedef uint32 lxHandleID;
  typedef struct lxHandleSys_s* HandleSysPTR;

  LUX_API void  lxHandleSys_init(HandleSysPTR sys);

  // type must be greater 0
  LUX_API lxHandleID lxHandleSys_add(HandleSysPTR sys, uint32 type, void *data);
  LUX_API booln lxHandleSys_rem(HandleSysPTR sys, lxHandleID id);
  LUX_API booln lxHandleSys_replace(HandleSysPTR sys, lxHandleID id, void *data);

  LUX_API booln lxHandleSys_getSafe(HandleSysPTR sys, lxHandleID id, void **outval);
  LUX_API void* lxHandleSys_getPtr(HandleSysPTR sys, lxHandleID id);

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // Inline & Details

#define LUX_HANDLE_TYPE    (6)
#define LUX_HANDLE_IDX     (12)
#define LUX_HANDLE_COUNTER (32 - LUX_HANDLE_IDX - LUX_HANDLE_TYPE)
#define LUX_HANDLESYS_MAX  (1<<LUX_HANDLE_IDX)

  typedef struct Handle_s{
    uint type : LUX_HANDLE_TYPE;
    uint counter : LUX_HANDLE_COUNTER;
    uint idx : LUX_HANDLE_IDX;
  }Handle_t;

  typedef struct lxHandleEntry_s{
    Handle_t      handle;
    union{
      void*       data;
      uint32      nextUnused;
    };
  }lxHandleEntry_t;

  typedef struct lxHandleSys_s{
    uint32        numUsed;
    uint32        firstUnused;
    lxHandleEntry_t entries[LUX_HANDLESYS_MAX];
  }lxHandleSys_t;

  int LUX_INLINE lxHandleSys_checkIdx(HandleSysPTR sys, lxHandleID id)
  {
    const Handle_t* hdl = (const Handle_t*)&id;
    if (id && *(lxHandleID*)&sys->entries[hdl->idx].handle == id)
      return hdl->idx;
    else
      return -1;
  }

  LUX_INLINE booln  lxHandleSys_getSafe(HandleSysPTR sys, lxHandleID id, void **outval)
  {
    int idx = lxHandleSys_checkIdx(sys,id);
    if (idx > 0){
      *outval = sys->entries[idx].data;
      return LUX_TRUE;
    }
    else{
      return LUX_FALSE;
    }
  }

  LUX_INLINE void*  lxHandleSys_getPtr(HandleSysPTR sys, lxHandleID id)
  {
    int idx = lxHandleSys_checkIdx(sys,id);
    if (idx > 0){
      return sys->entries[idx].data;
    }
    else{
      return NULL;
    }
  }


#ifdef __cplusplus
}
#endif


#endif
