// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxcore/memorybase.h>



//////////////////////////////////////////////////////////////////////////
// Aligned alloc/free
#if 0
void* lxMemAlignedAlloc(size_t size, size_t align_size) 
{

  char *ptr=(char *)MEMORY_GLOBAL_MALLOC(size + align_size + sizeof(size_t));
  if(ptr==NULL) return(NULL);

  return(lxMemAlignedFromOrig(ptr,align_size));
}

void  lxMemAlignedFree(void *ptr) 
{
  MEMORY_GLOBAL_FREE(lxMemAlignedToOrig(ptr));
}
#endif
