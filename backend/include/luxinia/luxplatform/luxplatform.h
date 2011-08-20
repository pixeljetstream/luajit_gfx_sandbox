/* 
** Luxinia 2 SDK - http://www.luxinia.de
**
** Copyright (C) 2004-2011 Christoph Kubisch and Eike Decker
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/

#ifndef __LUXPLATFORM_H__
#define __LUXPLATFORM_H__

// handles
//  platform: windows,mac..
//  arch: cpu architecture
//  compiler: specific handling
//  datatypes: size checks and default typedefs for uint,ushort and alike
//  profiling: defines that only exist in DEVBUILD

#include <stdio.h>
#include <stddef.h>

//////////////////////////////////////////////////////////////////////////
// General Defines
//
//  LUX_DEVBUILD
//    is always set on _DEBUG, but can be part of a release build as well
//    certain features might only be performed in _DEBUG
//    Main purpose is to have additional checking and info in release
//    builds.
//
//  LUX_SIMD
//    if SIMD functionality can be assumed as minimum
//    depending on compiler & architecture LUX_SIMD_SSE is set
//    to allow xmmintrin.h functionality
//
//  LUX_RENDERBACKEND
//    one of the following is legal
//    _OPENGL1  based on OpenGL 1.3 plus some extensions
//    _OPENGL3  (not yet implemented)
//
//  LUX_COMPILERINTRINSICS
//    allows LUX_ASSUME,LUX_RESTRICT,LUX_FASTCALL

#include "luxconfig.h"

#if defined(_DEBUG) && !defined(LUX_DEVBUILD)
  #define LUX_DEVBUILD
#endif


//////////////////////////////////////////////////////////////////////////
// Platform

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
  #define LUX_PLATFORM_WINDOWS
  #define LUX_PLATFORM_NAME   "win"

#elif defined(__APPLE__) && (defined (MACOSX) || defined(MACOS_X))
  #define LUX_PLATFORM_MAC
  #define LUX_PLATFORM_NAME   "mac"

#elif defined(__linux__)
  #define LUX_PLATFORM_LINUX
  #define LUX_PLATFORM_NAME   "nix"

#elif 
  #error "Platform not supported"
#endif

//////////////////////////////////////////////////////////////////////////
// Architecture

#if defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(__AMD64__)
  #define LUX_ARCH_X64
  #define LUX_ENDIAN_LITTLE

#elif defined(__i386__) || defined(_M_IX86) || defined(__X86__)
  #define LUX_ARCH_X86
  #define LUX_ENDIAN_LITTLE

#elif defined(__ppc__)
  #define LUX_ARCH_PPC
  #define LUX_ENDIAN_BIG

#elif 
  #error "Architecture not supported"
#endif

#if !defined(LUX_ENDIAN_BIG) && !defined(LUX_ENDIAN_LITTLE)
  #error "Endianess not defined"
#endif

//////////////////////////////////////////////////////////////////////////
// Compiler


#if defined(__GNUC__) && __GNUC__ >= 3 && __GNUC_MINOR__ >= 4
  #define LUX_COMPILER_GCC
  #define LUX_NOOP(...)
  #define LUX_INLINE          inline
  #define LUX_STACKALLOC      alloca
  #define LUX_STACKALLOC16    ((void *)((((size_t)alloca( (x)+15 )) + 15) & ~15))
  #define LUX_ALIGN_V(x,a)    x __attribute__((aligned (a)))
  #define LUX_ALIGN_BEGIN(a)  
  #define LUX_ALIGN_END(a)    __attribute__((aligned (a)))
  #define LUX_FASTCALL        __attribute__((fastcall))
  #define LUX_RESTRICT        __restrict__
  #define LUX_ASSUME          LUX_NOOP

#elif defined(__MSC__) || defined(_MSC_VER)
  #pragma warning(disable:4142) // redefinition of same type
  #if (_MSC_VER >= 1400)      // VC8+
    #pragma warning(disable : 4996)    // Either disable all deprecation warnings,
    // Or just turn off warnings about the newly deprecated CRT functions.
    //#ifndef _CRT_SECURE_NO_DEPRECATE
    //#define _CRT_SECURE_NO_DEPRECATE
    //#endif
    //#ifndef _CRT_NONSTDC_NO_DEPRECATE
    //#define _CRT_NONSTDC_NO_DEPRECATE
  #endif   // VC8+

  #define LUX_COMPILER_MSC
  #define LUX_NOOP            __noop
  #define LUX_INLINE          __forceinline
  #define LUX_STACKALLOC      _alloca
  #define LUX_STACKALLOC16    ((void *)((((size_t)_alloca( (x)+15 )) + 15) & ~15))
  #define LUX_ALIGN_V(x,a)    __declspec(align(a)) x
  #define LUX_ALIGN_BEGIN(a)  __declspec(align(a))
  #define LUX_ALIGN_END(a)  
  #define LUX_FASTCALL        __fastcall
  #define LUX_RESTRICT       __restrict
  #define LUX_ASSUME         __assume

#else
  #error "compiler unkown"
  
#endif

// additional features
#ifdef LUX_SIMD
#if defined(LUX_COMPILER_MSC) && (defined(LUX_ARCH_X86) || defined(LUX_ARCH_X64))
  // we have support for xmmintrin
  #define LUX_SIMD_SSE
  #include <xmmintrin.h>
  #define LUX_ALIGNSIMD_V(x)    LUX_ALIGN_V( x, 16 )
  #define LUX_ALIGNSIMD_BEGIN   LUX_ALIGN_BEGIN( 16 )
  #define LUX_ALIGNSIMD_END     LUX_ALIGN_END( 16 )
#endif
#else
  #define LUX_ALIGNSIMD_V(x)  x
  #define LUX_ALIGNSIMD_BEGIN
  #define LUX_ALIGNSIMD_END
#endif

#ifndef LUX_COMPILERINTRINSICS
  #undef  LUX_ASSUME
  #define LUX_ASSUME    LUX_NOOP
  #undef  LUX_RESTRICT
  #define LUX_RESTRICT
  #undef  LUX_FASTCALL
  #define LUX_FASTCALL
#endif

// sanity check
#if defined(LUX_PLATFORM_WINDOWS) && !defined(LUX_COMPILER_GCC) && !defined(LUX_COMPILER_MSC)
#error "unspported compiler for this platform"
#endif

#if defined(LUX_PLATFORM_MAC) && !defined(LUX_COMPILER_GCC)
#error "unspported compiler for this platform"
#endif

#if defined(LUX_PLATFORM_LINUX) && !defined(LUX_COMPILER_GCC)
#error "unspported compiler for this platform"
#endif

//////////////////////////////////////////////////////////////////////////
// Rendering Backend

#if !defined(LUX_RENDERBACKEND_OPENGL1) && !defined(LUX_RENDERBACKEND_OPENGL3)
#error "unsupported renderbackend specified"
#endif


//////////////////////////////////////////////////////////////////////////
// Profiling

#ifdef LUX_DEVBUILD
  #define LUX_PROFILING
  #define LUX_PROFILING_OP
  #define LUX_PROFILING_OP_MAX(var,chk) (var = (chk > var) ? chk : var)
#else
  #undef LUX_PROFILING
  #define LUX_PROFILING_OP    LUX_NOOP
  #define LUX_PROFILING_OP_MAX  LUX_NOOP
#endif

//////////////////////////////////////////////////////////////////////////
// Import/Export

#if defined(LUX_PLATFORM_WINDOWS) && defined(LUX_API_EXPORTS)

// We are building a Win32 DLL
#define LUX_API      __declspec(dllexport)
#define LUX_APIENTRY __cdecl
#define LUX_APICALL  __cdecl

#elif defined(LUX_PLATFORM_WINDOWS) && defined(LUX_API_IMPORTS)

// We are calling a Win32 DLL
#define LUX_API      __declspec(dllimport)
#define LUX_APIENTRY __cdecl
#define LUX_APICALL  __cdecl

#else

#define LUX_APIENTRY
#define LUX_API
#define LUX_APICALL

#endif

//////////////////////////////////////////////////////////////////////////
// Types

#include "luxtypes.h"


#endif
