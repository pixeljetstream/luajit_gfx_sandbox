// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

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
//    one of the following must be specified
//    _OPENGL1   (assumes "compatibility" profile)
//    _OPENGL3   (not yet implemented)
//
//  LUX_COMPILERINTRINSICS
//    allows LUX_ASSUME,LUX_RESTRICT,LUX_FASTCALL
//
//  LUX_API_EXPORTS, LUX_API_IMPORTS
//    for dll & lib generation

#ifndef LUX_RENDERBACKEND_OPENGL1
#define LUX_RENDERBACKEND_OPENGL1
#endif

#ifndef LUX_SIMD
#define LUX_SIMD
#endif

#ifndef LUX_COMPILERINTRINSICS
#define LUX_COMPILERINTRINSICS
#endif

#ifndef LUX_API_IMPORTS
#define LUX_API_EXPORTS
#endif

