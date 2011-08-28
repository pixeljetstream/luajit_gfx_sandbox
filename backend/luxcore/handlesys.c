// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/handlesys.h>
#include <luxinia/luxplatform/debug.h>

LUX_API void lxHandleSys_init( lxHandleSysPTR sys )
{
  int i;
  sys->numUsed = 0;
  sys->firstUnused = 0;
  for ( i = 0; i < LUX_HANDLESYS_MAX; i++){
    lxHandleEntry_t* entry = &sys->entries[i];
    entry->nextUnused = i+1;
    entry->handle.type |= (~0);
    entry->handle.counter = 0;
    entry->handle.idx = i;
  }
}

LUX_API lxHandleID lxHandleSys_add( lxHandleSysPTR sys, uint32 type, void *data )
{
  lxHandleEntry_t* entry = &sys->entries[sys->firstUnused];
  LUX_ASSERT(type >= 1);
  LUX_ASSERT(sys->numUsed < LUX_HANDLESYS_MAX);
  entry->handle.counter++;
  entry->handle.type = type;
  sys->firstUnused = entry->nextUnused;
  entry->data = data;
  sys->numUsed++;

  return *((lxHandleID*)&entry->handle);
}

LUX_API booln lxHandleSys_rem( lxHandleSysPTR sys, lxHandleID id )
{
  int idx = lxHandleSys_checkIdx(sys,id);
  lxHandleEntry_t* entry = &sys->entries[idx];
  if (idx < 0) return LUX_FALSE;

  entry->handle.type |= (~0);
  entry->nextUnused = sys->firstUnused;
  sys->firstUnused = idx;

  return LUX_TRUE;
}

LUX_API booln lxHandleSys_replace( lxHandleSysPTR sys, lxHandleID id, void *data )
{
  int idx = lxHandleSys_checkIdx(sys,id);
  lxHandleEntry_t* entry = &sys->entries[idx];
  if (idx < 0) return LUX_FALSE;

  entry->data = data;
  return LUX_TRUE;
}


