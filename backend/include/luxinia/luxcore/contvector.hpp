// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h
//
// modelled after yasli::vector (C) Andrei Alexandrescu
// http://www.moderncppdesign.com/


#ifndef __LUXCORE_CONTVECTOR_HPP__
#define __LUXCORE_CONTVECTOR_HPP__

#include <luxinia/luxcore/contvector.h>


template <class T, int ALIGN=0>
class lxCContVector
{
private:
  lxContVector_t  m_vector;

public:
  LUX_INLINE operator lxContVectorPTR () {return &m_vector;}
  LUX_INLINE operator lxContVectorCPTR () const {return &m_vector;}

  LUX_INLINE lxCContVector(){
    lxContVector_initAligned(&m_vector,sizeof(T),ALIGN);
  }

  LUX_INLINE ~lxCContVector(){
    if(!lxContVector_isEmpty(&m_vector))
      lxContVector_clear(&m_vector);
  }

  LUX_INLINE const T* front() const { return (const T*)lxContVector_front(&m_vector);}
  LUX_INLINE T* front(){ return (T*)lxContVector_front(&m_vector);}
  LUX_INLINE const T* back() const { return (const T*)lxContVector_backS(&m_vector,sizeof(T));}
  LUX_INLINE T* back(){ return (T*)lxContVector_backS(&m_vector,sizeof(T));}
  LUX_INLINE const T* at(uint idx) const { return (const T*)lxContVector_atS(&m_vector,sizeof(T),idx);}
  LUX_INLINE T* at(uint idx){ return (T*)lxContVector_atS(&m_vector,sizeof(T),idx);}

  LUX_INLINE const T& operator [](uint idx) const { 
    T* arr = (T*)m_vector.beg;
    LUX_DEBUGASSERT(arr);
    return arr[idx];
  }

  LUX_INLINE T& operator [](uint idx) { 
    T* arr = (T*)m_vector.beg;
    LUX_DEBUGASSERT(arr);
    return arr[idx];
  }

  LUX_INLINE int find(const T& val){
    T* beg = (T*)m_vector.beg;
    T* browse = beg;
    while (browse < (T*)m_vector.end){
      if (val == *browse){
        return (int)(browse-beg);
      }
      browse++;
    }

    return -1;
  }

  LUX_INLINE void pushBack(const T &val){
    if (m_vector.end == m_vector.eos) 
    {
      lxContVector_prepGrowth(this,1);
    }
    T* last = (T*)(m_vector.end-sizeof(T));
    last[0] = val;
  }
  LUX_INLINE booln isEmpty() const { return lxContVector_isEmpty(&m_vector);}
  LUX_INLINE uint capacity() const { return lxContVector_capacityS(&m_vector,sizeof(T));}
  LUX_INLINE uint size() const { return lxContVector_sizeS(&m_vector,sizeof(T));}
  LUX_INLINE uint memused() const { return lxContVector_memused(&m_vector);}
  LUX_INLINE uint memreserved() const { return lxContVector_memreserved(&m_vector);}

  LUX_INLINE void makeEmpty() {lxContVector_makeEmpty(&m_vector); }
  LUX_INLINE void clear() {lxContVector_clear(&m_vector); }

  LUX_INLINE void reserve(uint cnt) { lxContVector_reserve(&m_vector,cnt);}
  LUX_INLINE void resize(uint cnt, T* fill=NULL) {lxContVector_resize(&m_vector,cnt,fill);}

  LUX_INLINE void popBack(uint cnt){ lxContVector_popBackS(&m_vector,sizeof(T));}


};
  

#endif
