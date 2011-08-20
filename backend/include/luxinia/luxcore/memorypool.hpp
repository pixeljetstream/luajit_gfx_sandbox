// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_MEMORYPOOL_HPP__
#define __LUXCORE_MEMORYPOOL_HPP__

#include <luxinia/luxcore/memorypool.h>

class lxCMemoryPool
{
public:
  lxMemoryPoolPTR m_ptr;

  LUX_INLINE operator lxMemoryPoolPTR(){
    return m_ptr;
  }

  template <class T>
  LUX_INLINE T* alloc(){
    LUX_ASSERT((uint)sizeof(T) == lxMemoryPool_varSize(m_ptr));
    return (T*)lxMemoryPool_allocItem(m_ptr);
  }

  template <class T>
  LUX_INLINE void free(T* item){
    LUX_ASSERT((uint)sizeof(T) == lxMemoryPool_varSize(m_ptr));
    lxMemoryPool_freeItem(m_ptr, item);
  }

  LUX_INLINE void shrink(){
    lxMemoryPool_shrink(m_ptr);
  }
};

#endif
