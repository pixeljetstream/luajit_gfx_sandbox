// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_REFSYS_HPP__
#define __LUXCORE_REFSYS_HPP__

#include <luxinia/luxcore/refsys.h>


template <class T>
class lxCObjRef
{
public:
  lxObjRefPTR   m_refptr;

  lxCObjRef() : m_refptr(NULL) {}
  lxCObjRef(lxObjRefPTR ptr) : m_refptr(ptr) {}

  LUX_INLINE operator lxObjRefPTR&(){
    return m_refptr;
  }

  LUX_INLINE T* getPtr(){
    return lxObjRef_getPtr(m_refptr);
  }

  LUX_INLINE booln getSafe(T** out){
    return lxObjRef_getSafe(m_refptr, out);
  }

  LUX_INLINE operator bool(){
    return m_refptr != NULL;
  }

  LUX_INLINE operator T*(){
#ifdef _DEBUG
    T* ptr;
    booln res = getSafe(&ptr);
    LUX_ASSERT(res);
    return ptr;
#else
    return getPtr();
#endif
  }

  LUX_INLINE void addWeak(){
    return lxObjRef_addWeak(m_refptr);
  }

  LUX_INLINE booln addUser(){
    return lxObjRef_addWeak(m_refptr);
  }

  LUX_INLINE void releaseWeak(){
    return lxObjRef_releaseWeak(m_refptr);
  }

  LUX_INLINE booln releaseUser(){
    return lxObjRef_releaseUser(m_refptr);
  }
};


#endif
