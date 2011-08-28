// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXPLATFORM_DEBUG_H__
#define __LUXPLATFORM_DEBUG_H__

#include <luxinia/luxplatform/luxplatform.h>

//////////////////////////////////////////////////////////////////////////
// Debug

#ifdef __cplusplus
extern "C"{
#endif

LUX_API void lxDebugAssertFailed( const char *file, int line, const char *expression );
LUX_API void lxDebugPrintf(const char* format, ...);


//////////////////////////////////////////////////////////////////////////
#ifdef LUX_DEVBUILD
  #define LUX_ASSERT( X )   if ( !(X) ) lxDebugAssertFailed( __FILE__, __LINE__, #X )
  #define LUX_PRINTF      lxDebugPrintf
#else
  #define LUX_ASSERT      LUX_NOOP
  #define LUX_PRINTF      LUX_NOOP
#endif

#ifdef _DEBUG
  #define LUX_DEBUGASSERT( X )  if ( !(X) ) lxDebugAssertFailed( __FILE__, __LINE__, #X )
  #define LUX_DEBUGPRINTF     lxDebugPrintf
#ifdef LUX_SIMD
  #define LUX_SIMDASSERT( X )   if ( !(X) ) lxDebugAssertFailed( __FILE__, __LINE__, #X )
#else
  #define LUX_SIMDASSERT      LUX_NOOP
#endif
#else
  #define LUX_DEBUGASSERT     LUX_NOOP
  #define LUX_DEBUGPRINTF     LUX_NOOP
  #define LUX_SIMDASSERT      LUX_NOOP
#endif

#ifdef __cplusplus
};  
#endif

#endif
