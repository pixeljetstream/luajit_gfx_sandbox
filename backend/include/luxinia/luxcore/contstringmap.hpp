// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_STRINGMAP_HPP__
#define __LUXCORE_STRINGMAP_HPP__

#include <luxinia/luxcore/contstringmap.h>


class lxCStrDict
{
public:

  lxStrDictPTR  m_ptr;

  LUX_INLINE operator lxStrDictPTR(){
    return m_ptr;
  }

  LUX_INLINE lxStrDictKey add( const char *str ){
    return lxStrDict_add(m_ptr, str);
  }
  LUX_INLINE lxStrDictName getFromKey( lxStrDictKey key ) const{
    return lxStrDict_getFromKey(m_ptr, key);
  }
  LUX_INLINE lxStrDictKey getKey( const char *str ) const{
    return lxStrDict_getKey(m_ptr, str);
  }
  LUX_INLINE int      rem( lxStrDictName str ){
    return lxStrDict_rem(m_ptr, str);
  }
  LUX_INLINE int      remKey( lxStrDictKey key){
    return lxStrDict_remKey(m_ptr, key);
  }
};

#endif

