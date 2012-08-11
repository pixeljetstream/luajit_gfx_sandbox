#ifdef GLEW_MX
#pragma error "unsupported option"
#endif
/*
** The OpenGL Extension Wrangler Library
** Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>
** Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>
** Copyright (C) 2002, Lev Povalahev
** All rights reserved.
** 
** Redistribution and use in source and binary forms, with or without 
** modification, are permitted provided that the following conditions are met:
** 
** * Redistributions of source code must retain the above copyright notice, 
**   this list of conditions and the following disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice, 
**   this list of conditions and the following disclaimer in the documentation 
**   and/or other materials provided with the distribution.
** * The name of the author may be used to endorse or promote products 
**   derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
** THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * Mesa 3-D graphics library
 * Version:  7.0
 *
 * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
** Copyright (c) 2007 The Khronos Group Inc.
** 
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
** 
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
** 
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

#ifndef __glew_h__
#define __glew_h__
#define __GLEW_H__

#if defined(__gl_h_) || defined(__GL_H__) || defined(__X_GL_H)
#error gl.h included before glew.h
#endif
#if defined(__REGAL_H__)
#error Regal.h included before glew.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#error glext.h included before glew.h
#endif
#if defined(__gl_ATI_h_)
#error glATI.h included before glew.h
#endif

#define __gl_h_
#define __GL_H__
#define __REGAL_H__
#define __X_GL_H
#define __glext_h_
#define __GLEXT_H_
#define __gl_ATI_h_

#if defined(_WIN32)

/*
 * GLEW does not include <windows.h> to avoid name space pollution.
 * GL needs GLAPI and GLAPIENTRY, GLU needs APIENTRY, CALLBACK, and wchar_t
 * defined properly.
 */
/* <windef.h> */
#ifndef APIENTRY
#define GLEW_APIENTRY_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define APIENTRY __stdcall
#  elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#    define APIENTRY __stdcall
#  else
#    define APIENTRY
#  endif
#endif
#ifndef GLAPI
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define GLAPI extern
#  endif
#endif
/* <winnt.h> */
#ifndef CALLBACK
#define GLEW_CALLBACK_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define CALLBACK __attribute__ ((__stdcall__))
#  elif (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#  else
#    define CALLBACK
#  endif
#endif
/* <wingdi.h> and <winnt.h> */
#ifndef WINGDIAPI
#define GLEW_WINGDIAPI_DEFINED
#define WINGDIAPI __declspec(dllimport)
#endif
/* <ctype.h> */
#if (defined(_MSC_VER) || defined(__BORLANDC__)) && !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#  define _WCHAR_T_DEFINED
#endif
/* <stddef.h> */
#if !defined(_W64)
#  if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && defined(_MSC_VER) && _MSC_VER >= 1300
#    define _W64 __w64
#  else
#    define _W64
#  endif
#endif
#if !defined(_PTRDIFF_T_DEFINED) && !defined(_PTRDIFF_T_) && !defined(__MINGW64__)
#  ifdef _WIN64
typedef __int64 ptrdiff_t;
#  else
typedef _W64 int ptrdiff_t;
#  endif
#  define _PTRDIFF_T_DEFINED
#  define _PTRDIFF_T_
#endif

#ifndef GLAPI
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define GLAPI extern
#  else
#    define GLAPI WINGDIAPI
#  endif
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifndef GLEWAPIENTRY
#define GLEWAPIENTRY APIENTRY
#endif

/*
 * GLEW_STATIC is defined for static library.
 * GLEW_BUILD  is defined for building the DLL library.
 */

#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  ifdef GLEW_BUILD
#    define GLEWAPI extern __declspec(dllexport)
#  else
#    define GLEWAPI extern __declspec(dllimport)
#  endif
#endif

#else /* _UNIX */

/*
 * Needed for ptrdiff_t in turn needed by VBO.  This is defined by ISO
 * C.  On my system, this amounts to _3 lines_ of included code, all of
 * them pretty much harmless.  If you know of a way of detecting 32 vs
 * 64 _targets_ at compile time you are free to replace this with
 * something that's portable.  For now, _this_ is the portable solution.
 * (mem, 2004-01-04)
 */

#include <stddef.h>

/* SGI MIPSPro doesn't like stdint.h in C++ mode          */
/* ID: 3376260 Solaris 9 has inttypes.h, but not stdint.h */

#if (defined(__sgi) || defined(__sun)) && !defined(__GNUC__)
#include <inttypes.h>
#else
#include <stdint.h>
#endif

#define GLEW_APIENTRY_DEFINED
#define APIENTRY

/*
 * GLEW_STATIC is defined for static library.
 */

#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  if defined(__GNUC__) && __GNUC__>=4
#   define GLEWAPI extern __attribute__ ((visibility("default")))
#  elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#   define GLEWAPI extern __global
#  else
#   define GLEWAPI extern
#  endif
#endif

/* <glu.h> */
#ifndef GLAPI
#define GLAPI extern
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#ifndef GLEWAPIENTRY
#define GLEWAPIENTRY
#endif

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------- GL_VERSION_1_1 ---------------------------- */

#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1 1

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
#if defined(_MSC_VER) && _MSC_VER < 1400
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#else
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#include <inttypes.h>
#  endif
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;

typedef char GLchar;

#define GL_ZERO 0
#define GL_FALSE 0
#define GL_LOGIC_OP 0x0BF1
#define GL_NONE 0
#define GL_TEXTURE_COMPONENTS 0x1003
#define GL_NO_ERROR 0
#define GL_POINTS 0x0000
#define GL_CURRENT_BIT 0x00000001
#define GL_TRUE 1
#define GL_ONE 1
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_POINT_BIT 0x00000002
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_LINE_STRIP 0x0003
#define GL_LINE_BIT 0x00000004
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON 0x0009
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_LIGHTING_BIT 0x00000040
#define GL_FOG_BIT 0x00000080
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_ACCUM 0x0100
#define GL_LOAD 0x0101
#define GL_RETURN 0x0102
#define GL_MULT 0x0103
#define GL_ADD 0x0104
#define GL_NEVER 0x0200
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_AUX0 0x0409
#define GL_AUX1 0x040A
#define GL_AUX2 0x040B
#define GL_AUX3 0x040C
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505
#define GL_2D 0x0600
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_4D_COLOR_TEXTURE 0x0604
#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_POINT_TOKEN 0x0701
#define GL_LINE_TOKEN 0x0702
#define GL_POLYGON_TOKEN 0x0703
#define GL_BITMAP_TOKEN 0x0704
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_LINE_RESET_TOKEN 0x0707
#define GL_EXP 0x0800
#define GL_VIEWPORT_BIT 0x00000800
#define GL_EXP2 0x0801
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_COEFF 0x0A00
#define GL_ORDER 0x0A01
#define GL_DOMAIN 0x0A02
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LIST_MODE 0x0B30
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_LIST_BASE 0x0B32
#define GL_LIST_INDEX 0x0B33
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_EDGE_FLAG 0x0B43
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_LIGHTING 0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_SHADE_MODEL 0x0B54
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_COLOR_MATERIAL 0x0B57
#define GL_FOG 0x0B60
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_FOG_MODE 0x0B65
#define GL_FOG_COLOR 0x0B66
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_MATRIX_MODE 0x0BA0
#define GL_NORMALIZE 0x0BA1
#define GL_VIEWPORT 0x0BA2
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_DITHER 0x0BD0
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND 0x0BE2
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_AUX_BUFFERS 0x0C00
#define GL_DRAW_BUFFER 0x0C01
#define GL_READ_BUFFER 0x0C02
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_INDEX_MODE 0x0C30
#define GL_RGBA_MODE 0x0C31
#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO 0x0C33
#define GL_RENDER_MODE 0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_FOG_HINT 0x0C54
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_STENCIL 0x0D11
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_OFFSET 0x0D13
#define GL_RED_SCALE 0x0D14
#define GL_RED_BIAS 0x0D15
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17
#define GL_GREEN_SCALE 0x0D18
#define GL_GREEN_BIAS 0x0D19
#define GL_BLUE_SCALE 0x0D1A
#define GL_BLUE_BIAS 0x0D1B
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_BIAS 0x0D1F
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_INDEX_BITS 0x0D51
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_ALPHA_BITS 0x0D55
#define GL_DEPTH_BITS 0x0D56
#define GL_STENCIL_BITS 0x0D57
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_NAME_STACK_DEPTH 0x0D70
#define GL_AUTO_NORMAL 0x0D80
#define GL_MAP1_COLOR_4 0x0D90
#define GL_MAP1_INDEX 0x0D91
#define GL_MAP1_NORMAL 0x0D92
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#define GL_MAP1_VERTEX_3 0x0D97
#define GL_MAP1_VERTEX_4 0x0D98
#define GL_MAP2_COLOR_4 0x0DB0
#define GL_MAP2_INDEX 0x0DB1
#define GL_MAP2_NORMAL 0x0DB2
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#define GL_MAP2_VERTEX_3 0x0DB7
#define GL_MAP2_VERTEX_4 0x0DB8
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#define GL_SELECTION_BUFFER_SIZE 0x0DF4
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TRANSFORM_BIT 0x00001000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_BORDER 0x1005
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_AMBIENT 0x1200
#define GL_DIFFUSE 0x1201
#define GL_SPECULAR 0x1202
#define GL_POSITION 0x1203
#define GL_SPOT_DIRECTION 0x1204
#define GL_SPOT_EXPONENT 0x1205
#define GL_SPOT_CUTOFF 0x1206
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_2_BYTES 0x1407
#define GL_3_BYTES 0x1408
#define GL_4_BYTES 0x1409
#define GL_DOUBLE 0x140A
#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
#define GL_SET 0x150F
#define GL_EMISSION 0x1600
#define GL_SHININESS 0x1601
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_COLOR_INDEXES 0x1603
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_COLOR_INDEX 0x1900
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_BITMAP 0x1A00
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_RENDER 0x1C00
#define GL_FEEDBACK 0x1C01
#define GL_SELECT 0x1C02
#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_S 0x2000
#define GL_ENABLE_BIT 0x00002000
#define GL_T 0x2001
#define GL_R 0x2002
#define GL_Q 0x2003
#define GL_MODULATE 0x2100
#define GL_DECAL 0x2101
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_ENV 0x2300
#define GL_EYE_LINEAR 0x2400
#define GL_OBJECT_LINEAR 0x2401
#define GL_SPHERE_MAP 0x2402
#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_OBJECT_PLANE 0x2501
#define GL_EYE_PLANE 0x2502
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_R3_G3_B2 0x2A10
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_C3F_V3F 0x2A24
#define GL_N3F_V3F 0x2A25
#define GL_C4F_N3F_V3F 0x2A26
#define GL_T2F_V3F 0x2A27
#define GL_T4F_V4F 0x2A28
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_LIGHT0 0x4000
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007
#define GL_HINT_BIT 0x00008000
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_INTENSITY 0x8049
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB8 0x8051
#define GL_RGB10 0x8052
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_INDEX_ARRAY 0x8077
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7
#define GL_EVAL_BIT 0x00010000
#define GL_LIST_BIT 0x00020000
#define GL_TEXTURE_BIT 0x00040000
#define GL_SCISSOR_BIT 0x00080000
#define GL_ALL_ATTRIB_BITS 0x000fffff
#define GL_CLIENT_ALL_ATTRIB_BITS 0xffffffff

typedef char GLchar;
typedef double GLclampd;
typedef double GLdouble;
typedef float GLclampf;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef short GLshort;
typedef signed char GLbyte;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef unsigned int GLbitfield;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef unsigned long GLulong;
typedef unsigned short GLushort;
typedef void GLvoid;

typedef void (GLAPIENTRY * PFNGLACCUMPROC) (GLenum op, GLfloat value);
typedef void (GLAPIENTRY * PFNGLALPHAFUNCPROC) (GLenum func, GLclampf ref);
typedef GLboolean (GLAPIENTRY * PFNGLARETEXTURESRESIDENTPROC) (GLsizei n, const GLuint *textures, GLboolean *residences);
typedef void (GLAPIENTRY * PFNGLARRAYELEMENTPROC) (GLint i);
typedef void (GLAPIENTRY * PFNGLBEGINPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLBITMAPPROC) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCPROC) (GLenum sfactor, GLenum dfactor);
typedef void (GLAPIENTRY * PFNGLCALLLISTPROC) (GLuint list);
typedef void (GLAPIENTRY * PFNGLCALLLISTSPROC) (GLsizei n, GLenum type, const GLvoid *lists);
typedef void (GLAPIENTRY * PFNGLCLEARPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLCLEARACCUMPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * PFNGLCLEARCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (GLAPIENTRY * PFNGLCLEARDEPTHPROC) (GLclampd depth);
typedef void (GLAPIENTRY * PFNGLCLEARINDEXPROC) (GLfloat c);
typedef void (GLAPIENTRY * PFNGLCLEARSTENCILPROC) (GLint s);
typedef void (GLAPIENTRY * PFNGLCLIPPLANEPROC) (GLenum plane, const GLdouble *equation);
typedef void (GLAPIENTRY * PFNGLCOLOR3BPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3BVPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLCOLOR3DPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLCOLOR3FPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR3IPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLCOLOR3SPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLCOLOR3UBPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3UBVPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLCOLOR3UIPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3UIVPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLCOLOR3USPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3USVPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4BPROC) (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4BVPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4DPROC) (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4FPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4IPROC) (GLint red, GLint green, GLint blue, GLint alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4SPROC) (GLshort red, GLshort green, GLshort blue, GLshort alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBPROC) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4UIPROC) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4UIVPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4USPROC) (GLushort red, GLushort green, GLushort blue, GLushort alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4USVPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLCOLORMASKPROC) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (GLAPIENTRY * PFNGLCOLORMATERIALPROC) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * PFNGLCOLORPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLCOPYPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE1DPROC) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCULLFACEPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLDELETELISTSPROC) (GLuint list, GLsizei range);
typedef void (GLAPIENTRY * PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (GLAPIENTRY * PFNGLDEPTHFUNCPROC) (GLenum func);
typedef void (GLAPIENTRY * PFNGLDEPTHMASKPROC) (GLboolean flag);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEPROC) (GLclampd zNear, GLclampd zFar);
typedef void (GLAPIENTRY * PFNGLDISABLEPROC) (GLenum cap);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEPROC) (GLenum array);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (GLAPIENTRY * PFNGLDRAWPIXELSPROC) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPROC) (GLboolean flag);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPOINTERPROC) (GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGVPROC) (const GLboolean *flag);
typedef void (GLAPIENTRY * PFNGLENABLEPROC) (GLenum cap);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEPROC) (GLenum array);
typedef void (GLAPIENTRY * PFNGLENDPROC) (void);
typedef void (GLAPIENTRY * PFNGLENDLISTPROC) (void);
typedef void (GLAPIENTRY * PFNGLEVALCOORD1DPROC) (GLdouble u);
typedef void (GLAPIENTRY * PFNGLEVALCOORD1DVPROC) (const GLdouble *u);
typedef void (GLAPIENTRY * PFNGLEVALCOORD1FPROC) (GLfloat u);
typedef void (GLAPIENTRY * PFNGLEVALCOORD1FVPROC) (const GLfloat *u);
typedef void (GLAPIENTRY * PFNGLEVALCOORD2DPROC) (GLdouble u, GLdouble v);
typedef void (GLAPIENTRY * PFNGLEVALCOORD2DVPROC) (const GLdouble *u);
typedef void (GLAPIENTRY * PFNGLEVALCOORD2FPROC) (GLfloat u, GLfloat v);
typedef void (GLAPIENTRY * PFNGLEVALCOORD2FVPROC) (const GLfloat *u);
typedef void (GLAPIENTRY * PFNGLEVALMESH1PROC) (GLenum mode, GLint i1, GLint i2);
typedef void (GLAPIENTRY * PFNGLEVALMESH2PROC) (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
typedef void (GLAPIENTRY * PFNGLEVALPOINT1PROC) (GLint i);
typedef void (GLAPIENTRY * PFNGLEVALPOINT2PROC) (GLint i, GLint j);
typedef void (GLAPIENTRY * PFNGLFEEDBACKBUFFERPROC) (GLsizei size, GLenum type, GLfloat *buffer);
typedef void (GLAPIENTRY * PFNGLFINISHPROC) (void);
typedef void (GLAPIENTRY * PFNGLFLUSHPROC) (void);
typedef void (GLAPIENTRY * PFNGLFOGFPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFOGFVPROC) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLFOGIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFOGIVPROC) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLFRONTFACEPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRUSTUMPROC) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef GLuint (GLAPIENTRY * PFNGLGENLISTSPROC) (GLsizei range);
typedef void (GLAPIENTRY * PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANVPROC) (GLenum pname, GLboolean *params);
typedef void (GLAPIENTRY * PFNGLGETCLIPPLANEPROC) (GLenum plane, GLdouble *equation);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEVPROC) (GLenum pname, GLdouble *params);
typedef GLenum (GLAPIENTRY * PFNGLGETERRORPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETFLOATVPROC) (GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETINTEGERVPROC) (GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETLIGHTFVPROC) (GLenum light, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETLIGHTIVPROC) (GLenum light, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETMAPDVPROC) (GLenum target, GLenum query, GLdouble *v);
typedef void (GLAPIENTRY * PFNGLGETMAPFVPROC) (GLenum target, GLenum query, GLfloat *v);
typedef void (GLAPIENTRY * PFNGLGETMAPIVPROC) (GLenum target, GLenum query, GLint *v);
typedef void (GLAPIENTRY * PFNGLGETMATERIALFVPROC) (GLenum face, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETMATERIALIVPROC) (GLenum face, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETPIXELMAPFVPROC) (GLenum map, GLfloat *values);
typedef void (GLAPIENTRY * PFNGLGETPIXELMAPUIVPROC) (GLenum map, GLuint *values);
typedef void (GLAPIENTRY * PFNGLGETPIXELMAPUSVPROC) (GLenum map, GLushort *values);
typedef void (GLAPIENTRY * PFNGLGETPOINTERVPROC) (GLenum pname, GLvoid* *params);
typedef void (GLAPIENTRY * PFNGLGETPOLYGONSTIPPLEPROC) (GLubyte *mask);
typedef GLubyte* (GLAPIENTRY * PFNGLGETSTRINGPROC) (GLenum s);
typedef void (GLAPIENTRY * PFNGLGETTEXENVFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETTEXENVIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETTEXGENDVPROC) (GLenum coord, GLenum pname, GLdouble *params);
typedef void (GLAPIENTRY * PFNGLGETTEXGENFVPROC) (GLenum coord, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETTEXGENIVPROC) (GLenum coord, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETTEXIMAGEPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLGETTEXLEVELPARAMETERFVPROC) (GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETTEXLEVELPARAMETERIVPROC) (GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLHINTPROC) (GLenum target, GLenum mode);
typedef void (GLAPIENTRY * PFNGLINDEXMASKPROC) (GLuint mask);
typedef void (GLAPIENTRY * PFNGLINDEXPOINTERPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLINDEXDPROC) (GLdouble c);
typedef void (GLAPIENTRY * PFNGLINDEXDVPROC) (const GLdouble *c);
typedef void (GLAPIENTRY * PFNGLINDEXFPROC) (GLfloat c);
typedef void (GLAPIENTRY * PFNGLINDEXFVPROC) (const GLfloat *c);
typedef void (GLAPIENTRY * PFNGLINDEXIPROC) (GLint c);
typedef void (GLAPIENTRY * PFNGLINDEXIVPROC) (const GLint *c);
typedef void (GLAPIENTRY * PFNGLINDEXSPROC) (GLshort c);
typedef void (GLAPIENTRY * PFNGLINDEXSVPROC) (const GLshort *c);
typedef void (GLAPIENTRY * PFNGLINDEXUBPROC) (GLubyte c);
typedef void (GLAPIENTRY * PFNGLINDEXUBVPROC) (const GLubyte *c);
typedef void (GLAPIENTRY * PFNGLINITNAMESPROC) (void);
typedef void (GLAPIENTRY * PFNGLINTERLEAVEDARRAYSPROC) (GLenum format, GLsizei stride, const GLvoid *pointer);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDPROC) (GLenum cap);
typedef GLboolean (GLAPIENTRY * PFNGLISLISTPROC) (GLuint list);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREPROC) (GLuint texture);
typedef void (GLAPIENTRY * PFNGLLIGHTMODELFPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLLIGHTMODELFVPROC) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLLIGHTMODELIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLLIGHTMODELIVPROC) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLLIGHTFPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLLIGHTFVPROC) (GLenum light, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLLIGHTIPROC) (GLenum light, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLLIGHTIVPROC) (GLenum light, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLLINESTIPPLEPROC) (GLint factor, GLushort pattern);
typedef void (GLAPIENTRY * PFNGLLINEWIDTHPROC) (GLfloat width);
typedef void (GLAPIENTRY * PFNGLLISTBASEPROC) (GLuint base);
typedef void (GLAPIENTRY * PFNGLLOADIDENTITYPROC) (void);
typedef void (GLAPIENTRY * PFNGLLOADMATRIXDPROC) (const GLdouble *m);
typedef void (GLAPIENTRY * PFNGLLOADMATRIXFPROC) (const GLfloat *m);
typedef void (GLAPIENTRY * PFNGLLOADNAMEPROC) (GLuint name);
typedef void (GLAPIENTRY * PFNGLLOGICOPPROC) (GLenum opcode);
typedef void (GLAPIENTRY * PFNGLMAP1DPROC) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
typedef void (GLAPIENTRY * PFNGLMAP1FPROC) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
typedef void (GLAPIENTRY * PFNGLMAP2DPROC) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
typedef void (GLAPIENTRY * PFNGLMAP2FPROC) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
typedef void (GLAPIENTRY * PFNGLMAPGRID1DPROC) (GLint un, GLdouble u1, GLdouble u2);
typedef void (GLAPIENTRY * PFNGLMAPGRID1FPROC) (GLint un, GLfloat u1, GLfloat u2);
typedef void (GLAPIENTRY * PFNGLMAPGRID2DPROC) (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
typedef void (GLAPIENTRY * PFNGLMAPGRID2FPROC) (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLMATERIALFPROC) (GLenum face, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMATERIALFVPROC) (GLenum face, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLMATERIALIPROC) (GLenum face, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMATERIALIVPROC) (GLenum face, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLMATRIXMODEPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLMULTMATRIXDPROC) (const GLdouble *m);
typedef void (GLAPIENTRY * PFNGLMULTMATRIXFPROC) (const GLfloat *m);
typedef void (GLAPIENTRY * PFNGLNEWLISTPROC) (GLuint list, GLenum mode);
typedef void (GLAPIENTRY * PFNGLNORMAL3BPROC) (GLbyte nx, GLbyte ny, GLbyte nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3BVPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLNORMAL3DPROC) (GLdouble nx, GLdouble ny, GLdouble nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLNORMAL3FPROC) (GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLNORMAL3IPROC) (GLint nx, GLint ny, GLint nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLNORMAL3SPROC) (GLshort nx, GLshort ny, GLshort nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTERPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLORTHOPROC) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY * PFNGLPASSTHROUGHPROC) (GLfloat token);
typedef void (GLAPIENTRY * PFNGLPIXELMAPFVPROC) (GLenum map, GLsizei mapsize, const GLfloat *values);
typedef void (GLAPIENTRY * PFNGLPIXELMAPUIVPROC) (GLenum map, GLsizei mapsize, const GLuint *values);
typedef void (GLAPIENTRY * PFNGLPIXELMAPUSVPROC) (GLenum map, GLsizei mapsize, const GLushort *values);
typedef void (GLAPIENTRY * PFNGLPIXELSTOREFPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPIXELSTOREIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFERFPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFERIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPIXELZOOMPROC) (GLfloat xfactor, GLfloat yfactor);
typedef void (GLAPIENTRY * PFNGLPOINTSIZEPROC) (GLfloat size);
typedef void (GLAPIENTRY * PFNGLPOLYGONMODEPROC) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * PFNGLPOLYGONOFFSETPROC) (GLfloat factor, GLfloat units);
typedef void (GLAPIENTRY * PFNGLPOLYGONSTIPPLEPROC) (const GLubyte *mask);
typedef void (GLAPIENTRY * PFNGLPOPATTRIBPROC) (void);
typedef void (GLAPIENTRY * PFNGLPOPCLIENTATTRIBPROC) (void);
typedef void (GLAPIENTRY * PFNGLPOPMATRIXPROC) (void);
typedef void (GLAPIENTRY * PFNGLPOPNAMEPROC) (void);
typedef void (GLAPIENTRY * PFNGLPRIORITIZETEXTURESPROC) (GLsizei n, const GLuint *textures, const GLclampf *priorities);
typedef void (GLAPIENTRY * PFNGLPUSHATTRIBPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLPUSHCLIENTATTRIBPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLPUSHMATRIXPROC) (void);
typedef void (GLAPIENTRY * PFNGLPUSHNAMEPROC) (GLuint name);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2DPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2FPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2IPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2SPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLRASTERPOS2SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3DPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3FPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3IPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3SPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLRASTERPOS3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4DPROC) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4FPROC) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4IPROC) (GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4SPROC) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLRASTERPOS4SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLREADBUFFERPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLREADPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLRECTDPROC) (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
typedef void (GLAPIENTRY * PFNGLRECTDVPROC) (const GLdouble *v1, const GLdouble *v2);
typedef void (GLAPIENTRY * PFNGLRECTFPROC) (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
typedef void (GLAPIENTRY * PFNGLRECTFVPROC) (const GLfloat *v1, const GLfloat *v2);
typedef void (GLAPIENTRY * PFNGLRECTIPROC) (GLint x1, GLint y1, GLint x2, GLint y2);
typedef void (GLAPIENTRY * PFNGLRECTIVPROC) (const GLint *v1, const GLint *v2);
typedef void (GLAPIENTRY * PFNGLRECTSPROC) (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
typedef void (GLAPIENTRY * PFNGLRECTSVPROC) (const GLshort *v1, const GLshort *v2);
typedef GLint (GLAPIENTRY * PFNGLRENDERMODEPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLROTATEDPROC) (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLROTATEFPROC) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLSCALEDPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLSCALEFPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLSCISSORPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLSELECTBUFFERPROC) (GLsizei size, GLuint *buffer);
typedef void (GLAPIENTRY * PFNGLSHADEMODELPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLSTENCILFUNCPROC) (GLenum func, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILMASKPROC) (GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILOPPROC) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1DPROC) (GLdouble s);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1FPROC) (GLfloat s);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1IPROC) (GLint s);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1SPROC) (GLshort s);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2DPROC) (GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FPROC) (GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2IPROC) (GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2SPROC) (GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3DPROC) (GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3FPROC) (GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3IPROC) (GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3SPROC) (GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4DPROC) (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4IPROC) (GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4SPROC) (GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLTEXENVFPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXENVFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLTEXENVIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXENVIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLTEXGENDPROC) (GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * PFNGLTEXGENDVPROC) (GLenum coord, GLenum pname, const GLdouble *params);
typedef void (GLAPIENTRY * PFNGLTEXGENFPROC) (GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXGENFVPROC) (GLenum coord, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLTEXGENIPROC) (GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXGENIVPROC) (GLenum coord, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE1DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLTRANSLATEDPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLTRANSLATEFPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEX2DPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEX2DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLVERTEX2FPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEX2FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLVERTEX2IPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEX2IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEX2SPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEX2SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLVERTEX3DPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEX3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLVERTEX3FPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEX3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLVERTEX3IPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEX3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEX3SPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEX3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLVERTEX4DPROC) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEX4DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLVERTEX4FPROC) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEX4FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLVERTEX4IPROC) (GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEX4IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEX4SPROC) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEX4SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);


#define GLEW_VERSION_1_1 GLEW_GET_VAR(__GLEW_VERSION_1_1)

#endif /* GL_VERSION_1_1 */

/* ---------------------------------- GLU ---------------------------------- */

#ifndef GLEW_NO_GLU
/* this is where we can safely include GLU */
#  if defined(__APPLE__) && defined(__MACH__)
#    include <OpenGL/glu.h>
#  else
#    include <GL/glu.h>
#  endif
#endif

/* ----------------------------- GL_VERSION_1_2 ---------------------------- */

#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1

#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_RESCALE_NORMAL 0x803A
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SINGLE_COLOR 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E

typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);


#define GLEW_VERSION_1_2 GLEW_GET_VAR(__GLEW_VERSION_1_2)

#endif /* GL_VERSION_1_2 */

/* ---------------------------- GL_VERSION_1_2_1 --------------------------- */

#ifndef GL_VERSION_1_2_1
#define GL_VERSION_1_2_1 1

#define GLEW_VERSION_1_2_1 GLEW_GET_VAR(__GLEW_VERSION_1_2_1)

#endif /* GL_VERSION_1_2_1 */

/* ----------------------------- GL_VERSION_1_3 ---------------------------- */

#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1

#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_SUBTRACT 0x84E7
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_NORMAL_MAP 0x8511
#define GL_REFLECTION_MAP 0x8512
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE2_ALPHA 0x858A
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#define GL_MULTISAMPLE_BIT 0x20000000

typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint lod, GLvoid *img);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEPROC) (GLclampf value, GLboolean invert);


#define GLEW_VERSION_1_3 GLEW_GET_VAR(__GLEW_VERSION_1_3)

#endif /* GL_VERSION_1_3 */

/* ----------------------------- GL_VERSION_1_4 ---------------------------- */

#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1

#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_MIRRORED_REPEAT 0x8370
#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORDINATE 0x8451
#define GL_FRAGMENT_DEPTH 0x8452
#define GL_CURRENT_FOG_COORDINATE 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#define GL_FOG_COORDINATE_ARRAY 0x8457
#define GL_COLOR_SUM 0x8458
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_COMPARE_R_TO_TEXTURE 0x884E

typedef void (GLAPIENTRY * PFNGLBLENDCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTERPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVPROC) (const GLdouble *coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVPROC) (const GLfloat *coord);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSPROC) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVPROC) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIVPROC) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVPROC) (const GLdouble *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVPROC) (const GLfloat *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVPROC) (const GLint *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVPROC) (const GLshort *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVPROC) (const GLdouble *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVPROC) (const GLfloat *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVPROC) (const GLint *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVPROC) (const GLshort *p);


#define GLEW_VERSION_1_4 GLEW_GET_VAR(__GLEW_VERSION_1_4)

#endif /* GL_VERSION_1_4 */

/* ----------------------------- GL_VERSION_1_5 ---------------------------- */

#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1

#define GL_FOG_COORD_SRC GL_FOG_COORDINATE_SOURCE
#define GL_FOG_COORD GL_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY GL_FOG_COORDINATE_ARRAY
#define GL_SRC0_RGB GL_SOURCE0_RGB
#define GL_FOG_COORD_ARRAY_POINTER GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY_TYPE GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_SRC1_ALPHA GL_SOURCE1_ALPHA
#define GL_CURRENT_FOG_COORD GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY_STRIDE GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_SRC0_ALPHA GL_SOURCE0_ALPHA
#define GL_SRC1_RGB GL_SOURCE1_RGB
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_SRC2_ALPHA GL_SOURCE2_ALPHA
#define GL_SRC2_RGB GL_SOURCE2_RGB
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_SAMPLES_PASSED 0x8914

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

typedef void (GLAPIENTRY * PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDQUERYPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGENQUERIESPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVPROC) (GLenum target, GLenum pname, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERPROC) (GLuint buffer);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYPROC) (GLuint id);
typedef GLvoid* (GLAPIENTRY * PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERPROC) (GLenum target);


#define GLEW_VERSION_1_5 GLEW_GET_VAR(__GLEW_VERSION_1_5)

#endif /* GL_VERSION_1_5 */

/* ----------------------------- GL_VERSION_2_0 ---------------------------- */

#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0 1

#define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_POINT_SPRITE 0x8861
#define GL_COORD_REPLACE 0x8862
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_COORDS 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE 0x8B4F
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5

typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum, GLenum);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint (GLAPIENTRY * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSPROC) (GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEPROC) (GLuint obj, GLsizei maxLength, GLsizei* length, GLchar* source);
typedef void (GLAPIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVPROC) (GLuint program, GLint location, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVPROC) (GLuint program, GLint location, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint, GLenum, GLvoid**);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVPROC) (GLuint, GLenum, GLdouble*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVPROC) (GLuint, GLenum, GLfloat*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVPROC) (GLuint, GLenum, GLint*);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean (GLAPIENTRY * PFNGLISSHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
typedef void (GLAPIENTRY * PFNGLSTENCILFUNCSEPARATEPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILMASKSEPARATEPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);


#define GLEW_VERSION_2_0 GLEW_GET_VAR(__GLEW_VERSION_2_0)

#endif /* GL_VERSION_2_0 */

/* ----------------------------- GL_VERSION_2_1 ---------------------------- */

#ifndef GL_VERSION_2_1
#define GL_VERSION_2_1 1

#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB_ALPHA 0x8C42
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_SLUMINANCE_ALPHA 0x8C44
#define GL_SLUMINANCE8_ALPHA8 0x8C45
#define GL_SLUMINANCE 0x8C46
#define GL_SLUMINANCE8 0x8C47
#define GL_COMPRESSED_SRGB 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#define GL_COMPRESSED_SLUMINANCE 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA 0x8C4B

typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);


#define GLEW_VERSION_2_1 GLEW_GET_VAR(__GLEW_VERSION_2_1)

#endif /* GL_VERSION_2_1 */

/* ----------------------------- GL_VERSION_3_0 ---------------------------- */

#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1

#define GL_MAX_CLIP_DISTANCES GL_MAX_CLIP_PLANES
#define GL_CLIP_DISTANCE5 GL_CLIP_PLANE5
#define GL_CLIP_DISTANCE1 GL_CLIP_PLANE1
#define GL_CLIP_DISTANCE3 GL_CLIP_PLANE3
#define GL_COMPARE_REF_TO_TEXTURE GL_COMPARE_R_TO_TEXTURE_ARB
#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
#define GL_CLIP_DISTANCE4 GL_CLIP_PLANE4
#define GL_CLIP_DISTANCE2 GL_CLIP_PLANE2
#define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_FLOATS
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_DEPTH_BUFFER 0x8223
#define GL_STENCIL_BUFFER 0x8224
#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RG 0x8226
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_ALPHA_INTEGER 0x8D97
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16

typedef void (GLAPIENTRY * PFNGLBEGINCONDITIONALRENDERPROC) (GLuint, GLenum);
typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKPROC) (GLenum);
typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONPROC) (GLuint, GLuint, const GLchar*);
typedef void (GLAPIENTRY * PFNGLCLAMPCOLORPROC) (GLenum, GLenum);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFIPROC) (GLenum, GLint, GLfloat, GLint);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFVPROC) (GLenum, GLint, const GLfloat*);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERIVPROC) (GLenum, GLint, const GLint*);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERUIVPROC) (GLenum, GLint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLCOLORMASKIPROC) (GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
typedef void (GLAPIENTRY * PFNGLDISABLEIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLENABLEIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLENDCONDITIONALRENDERPROC) (void);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANI_VPROC) (GLenum, GLuint, GLboolean*);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONPROC) (GLuint, const GLchar*);
typedef const GLubyte* (GLAPIENTRY * PFNGLGETSTRINGIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVPROC) (GLenum, GLenum, GLint*);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVPROC) (GLenum, GLenum, GLuint*);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) (GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVPROC) (GLuint, GLint, GLuint*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVPROC) (GLuint, GLenum, GLint*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVPROC) (GLuint, GLenum, GLuint*);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVPROC) (GLenum, GLenum, const GLint*);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVPROC) (GLenum, GLenum, const GLuint*);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSPROC) (GLuint, GLsizei, const GLchar **, GLenum);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIPROC) (GLint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIPROC) (GLint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIPROC) (GLint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIPROC) (GLint, GLuint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IPROC) (GLuint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIPROC) (GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IPROC) (GLuint, GLint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIPROC) (GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IPROC) (GLuint, GLint, GLint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIPROC) (GLuint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVPROC) (GLuint, const GLbyte*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IPROC) (GLuint, GLint, GLint, GLint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVPROC) (GLuint, const GLshort*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVPROC) (GLuint, const GLubyte*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIPROC) (GLuint, GLuint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVPROC) (GLuint, const GLushort*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTERPROC) (GLuint, GLint, GLenum, GLsizei, const GLvoid*);


#define GLEW_VERSION_3_0 GLEW_GET_VAR(__GLEW_VERSION_3_0)

#endif /* GL_VERSION_3_0 */

/* ----------------------------- GL_VERSION_3_1 ---------------------------- */

#ifndef GL_VERSION_3_1
#define GL_VERSION_3_1 1

#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT 0x8C2E
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_RED_SNORM 0x8F90
#define GL_RG_SNORM 0x8F91
#define GL_RGB_SNORM 0x8F92
#define GL_RGBA_SNORM 0x8F93
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_PRIMITIVE_RESTART 0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum, GLint, GLsizei, GLsizei);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTINDEXPROC) (GLuint);
typedef void (GLAPIENTRY * PFNGLTEXBUFFERPROC) (GLenum, GLenum, GLuint);


#define GLEW_VERSION_3_1 GLEW_GET_VAR(__GLEW_VERSION_3_1)

#endif /* GL_VERSION_3_1 */

/* ----------------------------- GL_VERSION_3_2 ---------------------------- */

#ifndef GL_VERSION_3_2
#define GL_VERSION_3_2 1

#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_CONTEXT_PROFILE_MASK 0x9126

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREPROC) (GLenum, GLenum, GLuint, GLint);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERI64VPROC) (GLenum, GLenum, GLint64 *);
typedef void (GLAPIENTRY * PFNGLGETINTEGER64I_VPROC) (GLenum, GLuint, GLint64 *);


#define GLEW_VERSION_3_2 GLEW_GET_VAR(__GLEW_VERSION_3_2)

#endif /* GL_VERSION_3_2 */

/* ----------------------------- GL_VERSION_3_3 ---------------------------- */

#ifndef GL_VERSION_3_3
#define GL_VERSION_3_3 1

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
#define GL_RGB10_A2UI 0x906F

typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);


#define GLEW_VERSION_3_3 GLEW_GET_VAR(__GLEW_VERSION_3_3)

#endif /* GL_VERSION_3_3 */

/* ----------------------------- GL_VERSION_4_0 ---------------------------- */

#ifndef GL_VERSION_4_0
#define GL_VERSION_4_0 1

#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_SAMPLE_SHADING 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS 0x8F9F
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEIPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONIPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEIPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCIPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (GLAPIENTRY * PFNGLMINSAMPLESHADINGPROC) (GLclampf value);


#define GLEW_VERSION_4_0 GLEW_GET_VAR(__GLEW_VERSION_4_0)

#endif /* GL_VERSION_4_0 */

/* ----------------------------- GL_VERSION_4_1 ---------------------------- */

#ifndef GL_VERSION_4_1
#define GL_VERSION_4_1 1

#define GLEW_VERSION_4_1 GLEW_GET_VAR(__GLEW_VERSION_4_1)

#endif /* GL_VERSION_4_1 */

/* ----------------------------- GL_VERSION_4_2 ---------------------------- */

#ifndef GL_VERSION_4_2
#define GL_VERSION_4_2 1

#define GL_COMPRESSED_RGBA_BPTC_UNORM 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT 0x8E8F

#define GLEW_VERSION_4_2 GLEW_GET_VAR(__GLEW_VERSION_4_2)

#endif /* GL_VERSION_4_2 */

/* ----------------------------- GL_VERSION_4_3 ---------------------------- */

#ifndef GL_VERSION_4_3
#define GL_VERSION_4_3 1

#define GL_NUM_SHADING_LANGUAGE_VERSIONS 0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG 0x874E

#define GLEW_VERSION_4_3 GLEW_GET_VAR(__GLEW_VERSION_4_3)

#endif /* GL_VERSION_4_3 */

/* -------------------------- GL_3DFX_multisample -------------------------- */

#ifndef GL_3DFX_multisample
#define GL_3DFX_multisample 1

#define GL_MULTISAMPLE_3DFX 0x86B2
#define GL_SAMPLE_BUFFERS_3DFX 0x86B3
#define GL_SAMPLES_3DFX 0x86B4
#define GL_MULTISAMPLE_BIT_3DFX 0x20000000

#define GLEW_3DFX_multisample GLEW_GET_VAR(__GLEW_3DFX_multisample)

#endif /* GL_3DFX_multisample */

/* ---------------------------- GL_3DFX_tbuffer ---------------------------- */

#ifndef GL_3DFX_tbuffer
#define GL_3DFX_tbuffer 1

typedef void (GLAPIENTRY * PFNGLTBUFFERMASK3DFXPROC) (GLuint mask);


#define GLEW_3DFX_tbuffer GLEW_GET_VAR(__GLEW_3DFX_tbuffer)

#endif /* GL_3DFX_tbuffer */

/* -------------------- GL_3DFX_texture_compression_FXT1 ------------------- */

#ifndef GL_3DFX_texture_compression_FXT1
#define GL_3DFX_texture_compression_FXT1 1

#define GL_COMPRESSED_RGB_FXT1_3DFX 0x86B0
#define GL_COMPRESSED_RGBA_FXT1_3DFX 0x86B1

#define GLEW_3DFX_texture_compression_FXT1 GLEW_GET_VAR(__GLEW_3DFX_texture_compression_FXT1)

#endif /* GL_3DFX_texture_compression_FXT1 */

/* ----------------------- GL_AMD_blend_minmax_factor ---------------------- */

#ifndef GL_AMD_blend_minmax_factor
#define GL_AMD_blend_minmax_factor 1

#define GL_FACTOR_MIN_AMD 0x901C
#define GL_FACTOR_MAX_AMD 0x901D

#define GLEW_AMD_blend_minmax_factor GLEW_GET_VAR(__GLEW_AMD_blend_minmax_factor)

#endif /* GL_AMD_blend_minmax_factor */

/* ----------------------- GL_AMD_conservative_depth ----------------------- */

#ifndef GL_AMD_conservative_depth
#define GL_AMD_conservative_depth 1

#define GLEW_AMD_conservative_depth GLEW_GET_VAR(__GLEW_AMD_conservative_depth)

#endif /* GL_AMD_conservative_depth */

/* -------------------------- GL_AMD_debug_output -------------------------- */

#ifndef GL_AMD_debug_output
#define GL_AMD_debug_output 1

#define GL_MAX_DEBUG_MESSAGE_LENGTH_AMD 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_AMD 0x9144
#define GL_DEBUG_LOGGED_MESSAGES_AMD 0x9145
#define GL_DEBUG_SEVERITY_HIGH_AMD 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_AMD 0x9147
#define GL_DEBUG_SEVERITY_LOW_AMD 0x9148
#define GL_DEBUG_CATEGORY_API_ERROR_AMD 0x9149
#define GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD 0x914A
#define GL_DEBUG_CATEGORY_DEPRECATION_AMD 0x914B
#define GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD 0x914C
#define GL_DEBUG_CATEGORY_PERFORMANCE_AMD 0x914D
#define GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD 0x914E
#define GL_DEBUG_CATEGORY_APPLICATION_AMD 0x914F
#define GL_DEBUG_CATEGORY_OTHER_AMD 0x9150

typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKAMDPROC) (GLDEBUGPROCAMD callback, void* userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEENABLEAMDPROC) (GLenum category, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTAMDPROC) (GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGAMDPROC) (GLuint count, GLsizei bufsize, GLenum* categories, GLuint* severities, GLuint* ids, GLsizei* lengths, GLchar* message);


#define GLEW_AMD_debug_output GLEW_GET_VAR(__GLEW_AMD_debug_output)

#endif /* GL_AMD_debug_output */

/* ---------------------- GL_AMD_depth_clamp_separate ---------------------- */

#ifndef GL_AMD_depth_clamp_separate
#define GL_AMD_depth_clamp_separate 1

#define GL_DEPTH_CLAMP_NEAR_AMD 0x901E
#define GL_DEPTH_CLAMP_FAR_AMD 0x901F

#define GLEW_AMD_depth_clamp_separate GLEW_GET_VAR(__GLEW_AMD_depth_clamp_separate)

#endif /* GL_AMD_depth_clamp_separate */

/* ----------------------- GL_AMD_draw_buffers_blend ----------------------- */

#ifndef GL_AMD_draw_buffers_blend
#define GL_AMD_draw_buffers_blend 1

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONINDEXEDAMDPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCINDEXEDAMDPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);


#define GLEW_AMD_draw_buffers_blend GLEW_GET_VAR(__GLEW_AMD_draw_buffers_blend)

#endif /* GL_AMD_draw_buffers_blend */

/* ----------------------- GL_AMD_multi_draw_indirect ---------------------- */

#ifndef GL_AMD_multi_draw_indirect
#define GL_AMD_multi_draw_indirect 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTAMDPROC) (GLenum mode, const void* indirect, GLsizei primcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTAMDPROC) (GLenum mode, GLenum type, const void* indirect, GLsizei primcount, GLsizei stride);


#define GLEW_AMD_multi_draw_indirect GLEW_GET_VAR(__GLEW_AMD_multi_draw_indirect)

#endif /* GL_AMD_multi_draw_indirect */

/* ------------------------- GL_AMD_name_gen_delete ------------------------ */

#ifndef GL_AMD_name_gen_delete
#define GL_AMD_name_gen_delete 1

#define GL_DATA_BUFFER_AMD 0x9151
#define GL_PERFORMANCE_MONITOR_AMD 0x9152
#define GL_QUERY_OBJECT_AMD 0x9153
#define GL_VERTEX_ARRAY_OBJECT_AMD 0x9154
#define GL_SAMPLER_OBJECT_AMD 0x9155

typedef void (GLAPIENTRY * PFNGLDELETENAMESAMDPROC) (GLenum identifier, GLuint num, const GLuint* names);
typedef void (GLAPIENTRY * PFNGLGENNAMESAMDPROC) (GLenum identifier, GLuint num, GLuint* names);
typedef GLboolean (GLAPIENTRY * PFNGLISNAMEAMDPROC) (GLenum identifier, GLuint name);


#define GLEW_AMD_name_gen_delete GLEW_GET_VAR(__GLEW_AMD_name_gen_delete)

#endif /* GL_AMD_name_gen_delete */

/* ----------------------- GL_AMD_performance_monitor ---------------------- */

#ifndef GL_AMD_performance_monitor
#define GL_AMD_performance_monitor 1

#define GL_COUNTER_TYPE_AMD 0x8BC0
#define GL_COUNTER_RANGE_AMD 0x8BC1
#define GL_UNSIGNED_INT64_AMD 0x8BC2
#define GL_PERCENTAGE_AMD 0x8BC3
#define GL_PERFMON_RESULT_AVAILABLE_AMD 0x8BC4
#define GL_PERFMON_RESULT_SIZE_AMD 0x8BC5
#define GL_PERFMON_RESULT_AMD 0x8BC6

typedef void (GLAPIENTRY * PFNGLBEGINPERFMONITORAMDPROC) (GLuint monitor);
typedef void (GLAPIENTRY * PFNGLDELETEPERFMONITORSAMDPROC) (GLsizei n, GLuint* monitors);
typedef void (GLAPIENTRY * PFNGLENDPERFMONITORAMDPROC) (GLuint monitor);
typedef void (GLAPIENTRY * PFNGLGENPERFMONITORSAMDPROC) (GLsizei n, GLuint* monitors);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERDATAAMDPROC) (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint *bytesWritten);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERINFOAMDPROC) (GLuint group, GLuint counter, GLenum pname, void* data);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC) (GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, GLchar *counterString);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERSAMDPROC) (GLuint group, GLint* numCounters, GLint *maxActiveCounters, GLsizei countersSize, GLuint *counters);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORGROUPSTRINGAMDPROC) (GLuint group, GLsizei bufSize, GLsizei* length, GLchar *groupString);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORGROUPSAMDPROC) (GLint* numGroups, GLsizei groupsSize, GLuint *groups);
typedef void (GLAPIENTRY * PFNGLSELECTPERFMONITORCOUNTERSAMDPROC) (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* counterList);


#define GLEW_AMD_performance_monitor GLEW_GET_VAR(__GLEW_AMD_performance_monitor)

#endif /* GL_AMD_performance_monitor */

/* -------------------------- GL_AMD_pinned_memory ------------------------- */

#ifndef GL_AMD_pinned_memory
#define GL_AMD_pinned_memory 1

#define GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD 0x9160

#define GLEW_AMD_pinned_memory GLEW_GET_VAR(__GLEW_AMD_pinned_memory)

#endif /* GL_AMD_pinned_memory */

/* ----------------------- GL_AMD_query_buffer_object ---------------------- */

#ifndef GL_AMD_query_buffer_object
#define GL_AMD_query_buffer_object 1

#define GL_QUERY_BUFFER_AMD 0x9192
#define GL_QUERY_BUFFER_BINDING_AMD 0x9193
#define GL_QUERY_RESULT_NO_WAIT_AMD 0x9194

#define GLEW_AMD_query_buffer_object GLEW_GET_VAR(__GLEW_AMD_query_buffer_object)

#endif /* GL_AMD_query_buffer_object */

/* ------------------------ GL_AMD_sample_positions ------------------------ */

#ifndef GL_AMD_sample_positions
#define GL_AMD_sample_positions 1

#define GL_SUBSAMPLE_DISTANCE_AMD 0x883F

typedef void (GLAPIENTRY * PFNGLSETMULTISAMPLEFVAMDPROC) (GLenum pname, GLuint index, const GLfloat* val);


#define GLEW_AMD_sample_positions GLEW_GET_VAR(__GLEW_AMD_sample_positions)

#endif /* GL_AMD_sample_positions */

/* ------------------ GL_AMD_seamless_cubemap_per_texture ------------------ */

#ifndef GL_AMD_seamless_cubemap_per_texture
#define GL_AMD_seamless_cubemap_per_texture 1

#define GL_TEXTURE_CUBE_MAP_SEAMLESS_ARB 0x884F

#define GLEW_AMD_seamless_cubemap_per_texture GLEW_GET_VAR(__GLEW_AMD_seamless_cubemap_per_texture)

#endif /* GL_AMD_seamless_cubemap_per_texture */

/* ---------------------- GL_AMD_shader_stencil_export --------------------- */

#ifndef GL_AMD_shader_stencil_export
#define GL_AMD_shader_stencil_export 1

#define GLEW_AMD_shader_stencil_export GLEW_GET_VAR(__GLEW_AMD_shader_stencil_export)

#endif /* GL_AMD_shader_stencil_export */

/* ------------------- GL_AMD_stencil_operation_extended ------------------- */

#ifndef GL_AMD_stencil_operation_extended
#define GL_AMD_stencil_operation_extended 1

#define GL_SET_AMD 0x874A
#define GL_REPLACE_VALUE_AMD 0x874B
#define GL_STENCIL_OP_VALUE_AMD 0x874C
#define GL_STENCIL_BACK_OP_VALUE_AMD 0x874D

typedef void (GLAPIENTRY * PFNGLSTENCILOPVALUEAMDPROC) (GLenum face, GLuint value);


#define GLEW_AMD_stencil_operation_extended GLEW_GET_VAR(__GLEW_AMD_stencil_operation_extended)

#endif /* GL_AMD_stencil_operation_extended */

/* ------------------------ GL_AMD_texture_texture4 ------------------------ */

#ifndef GL_AMD_texture_texture4
#define GL_AMD_texture_texture4 1

#define GLEW_AMD_texture_texture4 GLEW_GET_VAR(__GLEW_AMD_texture_texture4)

#endif /* GL_AMD_texture_texture4 */

/* --------------- GL_AMD_transform_feedback3_lines_triangles -------------- */

#ifndef GL_AMD_transform_feedback3_lines_triangles
#define GL_AMD_transform_feedback3_lines_triangles 1

#define GLEW_AMD_transform_feedback3_lines_triangles GLEW_GET_VAR(__GLEW_AMD_transform_feedback3_lines_triangles)

#endif /* GL_AMD_transform_feedback3_lines_triangles */

/* ----------------------- GL_AMD_vertex_shader_layer ---------------------- */

#ifndef GL_AMD_vertex_shader_layer
#define GL_AMD_vertex_shader_layer 1

#define GLEW_AMD_vertex_shader_layer GLEW_GET_VAR(__GLEW_AMD_vertex_shader_layer)

#endif /* GL_AMD_vertex_shader_layer */

/* -------------------- GL_AMD_vertex_shader_tessellator ------------------- */

#ifndef GL_AMD_vertex_shader_tessellator
#define GL_AMD_vertex_shader_tessellator 1

#define GL_SAMPLER_BUFFER_AMD 0x9001
#define GL_INT_SAMPLER_BUFFER_AMD 0x9002
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_AMD 0x9003
#define GL_TESSELLATION_MODE_AMD 0x9004
#define GL_TESSELLATION_FACTOR_AMD 0x9005
#define GL_DISCRETE_AMD 0x9006
#define GL_CONTINUOUS_AMD 0x9007

typedef void (GLAPIENTRY * PFNGLTESSELLATIONFACTORAMDPROC) (GLfloat factor);
typedef void (GLAPIENTRY * PFNGLTESSELLATIONMODEAMDPROC) (GLenum mode);


#define GLEW_AMD_vertex_shader_tessellator GLEW_GET_VAR(__GLEW_AMD_vertex_shader_tessellator)

#endif /* GL_AMD_vertex_shader_tessellator */

/* ------------------ GL_AMD_vertex_shader_viewport_index ------------------ */

#ifndef GL_AMD_vertex_shader_viewport_index
#define GL_AMD_vertex_shader_viewport_index 1

#define GLEW_AMD_vertex_shader_viewport_index GLEW_GET_VAR(__GLEW_AMD_vertex_shader_viewport_index)

#endif /* GL_AMD_vertex_shader_viewport_index */

/* ----------------------- GL_APPLE_aux_depth_stencil ---------------------- */

#ifndef GL_APPLE_aux_depth_stencil
#define GL_APPLE_aux_depth_stencil 1

#define GL_AUX_DEPTH_STENCIL_APPLE 0x8A14

#define GLEW_APPLE_aux_depth_stencil GLEW_GET_VAR(__GLEW_APPLE_aux_depth_stencil)

#endif /* GL_APPLE_aux_depth_stencil */

/* ------------------------ GL_APPLE_client_storage ------------------------ */

#ifndef GL_APPLE_client_storage
#define GL_APPLE_client_storage 1

#define GL_UNPACK_CLIENT_STORAGE_APPLE 0x85B2

#define GLEW_APPLE_client_storage GLEW_GET_VAR(__GLEW_APPLE_client_storage)

#endif /* GL_APPLE_client_storage */

/* ------------------------- GL_APPLE_element_array ------------------------ */

#ifndef GL_APPLE_element_array
#define GL_APPLE_element_array 1

#define GL_ELEMENT_ARRAY_APPLE 0x8A0C
#define GL_ELEMENT_ARRAY_TYPE_APPLE 0x8A0D
#define GL_ELEMENT_ARRAY_POINTER_APPLE 0x8A0E

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTARRAYAPPLEPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC) (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLELEMENTPOINTERAPPLEPROC) (GLenum type, const void* pointer);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC) (GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC) (GLenum mode, GLuint start, GLuint end, const GLint* first, const GLsizei *count, GLsizei primcount);


#define GLEW_APPLE_element_array GLEW_GET_VAR(__GLEW_APPLE_element_array)

#endif /* GL_APPLE_element_array */

/* ----------------------------- GL_APPLE_fence ---------------------------- */

#ifndef GL_APPLE_fence
#define GL_APPLE_fence 1

#define GL_DRAW_PIXELS_APPLE 0x8A0A
#define GL_FENCE_APPLE 0x8A0B

typedef void (GLAPIENTRY * PFNGLDELETEFENCESAPPLEPROC) (GLsizei n, const GLuint* fences);
typedef void (GLAPIENTRY * PFNGLFINISHFENCEAPPLEPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLFINISHOBJECTAPPLEPROC) (GLenum object, GLint name);
typedef void (GLAPIENTRY * PFNGLGENFENCESAPPLEPROC) (GLsizei n, GLuint* fences);
typedef GLboolean (GLAPIENTRY * PFNGLISFENCEAPPLEPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLSETFENCEAPPLEPROC) (GLuint fence);
typedef GLboolean (GLAPIENTRY * PFNGLTESTFENCEAPPLEPROC) (GLuint fence);
typedef GLboolean (GLAPIENTRY * PFNGLTESTOBJECTAPPLEPROC) (GLenum object, GLuint name);


#define GLEW_APPLE_fence GLEW_GET_VAR(__GLEW_APPLE_fence)

#endif /* GL_APPLE_fence */

/* ------------------------- GL_APPLE_float_pixels ------------------------- */

#ifndef GL_APPLE_float_pixels
#define GL_APPLE_float_pixels 1

#define GL_HALF_APPLE 0x140B
#define GL_RGBA_FLOAT32_APPLE 0x8814
#define GL_RGB_FLOAT32_APPLE 0x8815
#define GL_ALPHA_FLOAT32_APPLE 0x8816
#define GL_INTENSITY_FLOAT32_APPLE 0x8817
#define GL_LUMINANCE_FLOAT32_APPLE 0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_APPLE 0x8819
#define GL_RGBA_FLOAT16_APPLE 0x881A
#define GL_RGB_FLOAT16_APPLE 0x881B
#define GL_ALPHA_FLOAT16_APPLE 0x881C
#define GL_INTENSITY_FLOAT16_APPLE 0x881D
#define GL_LUMINANCE_FLOAT16_APPLE 0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_APPLE 0x881F
#define GL_COLOR_FLOAT_APPLE 0x8A0F

#define GLEW_APPLE_float_pixels GLEW_GET_VAR(__GLEW_APPLE_float_pixels)

#endif /* GL_APPLE_float_pixels */

/* ---------------------- GL_APPLE_flush_buffer_range ---------------------- */

#ifndef GL_APPLE_flush_buffer_range
#define GL_APPLE_flush_buffer_range 1

#define GL_BUFFER_SERIALIZED_MODIFY_APPLE 0x8A12
#define GL_BUFFER_FLUSHING_UNMAP_APPLE 0x8A13

typedef void (GLAPIENTRY * PFNGLBUFFERPARAMETERIAPPLEPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC) (GLenum target, GLintptr offset, GLsizeiptr size);


#define GLEW_APPLE_flush_buffer_range GLEW_GET_VAR(__GLEW_APPLE_flush_buffer_range)

#endif /* GL_APPLE_flush_buffer_range */

/* ----------------------- GL_APPLE_object_purgeable ----------------------- */

#ifndef GL_APPLE_object_purgeable
#define GL_APPLE_object_purgeable 1

#define GL_BUFFER_OBJECT_APPLE 0x85B3
#define GL_RELEASED_APPLE 0x8A19
#define GL_VOLATILE_APPLE 0x8A1A
#define GL_RETAINED_APPLE 0x8A1B
#define GL_UNDEFINED_APPLE 0x8A1C
#define GL_PURGEABLE_APPLE 0x8A1D

typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERIVAPPLEPROC) (GLenum objectType, GLuint name, GLenum pname, GLint* params);
typedef GLenum (GLAPIENTRY * PFNGLOBJECTPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);
typedef GLenum (GLAPIENTRY * PFNGLOBJECTUNPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);


#define GLEW_APPLE_object_purgeable GLEW_GET_VAR(__GLEW_APPLE_object_purgeable)

#endif /* GL_APPLE_object_purgeable */

/* ------------------------- GL_APPLE_pixel_buffer ------------------------- */

#ifndef GL_APPLE_pixel_buffer
#define GL_APPLE_pixel_buffer 1

#define GL_MIN_PBUFFER_VIEWPORT_DIMS_APPLE 0x8A10

#define GLEW_APPLE_pixel_buffer GLEW_GET_VAR(__GLEW_APPLE_pixel_buffer)

#endif /* GL_APPLE_pixel_buffer */

/* ---------------------------- GL_APPLE_rgb_422 --------------------------- */

#ifndef GL_APPLE_rgb_422
#define GL_APPLE_rgb_422 1

#define GL_UNSIGNED_SHORT_8_8_APPLE 0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE 0x85BB
#define GL_RGB_422_APPLE 0x8A1F

#define GLEW_APPLE_rgb_422 GLEW_GET_VAR(__GLEW_APPLE_rgb_422)

#endif /* GL_APPLE_rgb_422 */

/* --------------------------- GL_APPLE_row_bytes -------------------------- */

#ifndef GL_APPLE_row_bytes
#define GL_APPLE_row_bytes 1

#define GL_PACK_ROW_BYTES_APPLE 0x8A15
#define GL_UNPACK_ROW_BYTES_APPLE 0x8A16

#define GLEW_APPLE_row_bytes GLEW_GET_VAR(__GLEW_APPLE_row_bytes)

#endif /* GL_APPLE_row_bytes */

/* ------------------------ GL_APPLE_specular_vector ----------------------- */

#ifndef GL_APPLE_specular_vector
#define GL_APPLE_specular_vector 1

#define GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE 0x85B0

#define GLEW_APPLE_specular_vector GLEW_GET_VAR(__GLEW_APPLE_specular_vector)

#endif /* GL_APPLE_specular_vector */

/* ------------------------- GL_APPLE_texture_range ------------------------ */

#ifndef GL_APPLE_texture_range
#define GL_APPLE_texture_range 1

#define GL_TEXTURE_RANGE_LENGTH_APPLE 0x85B7
#define GL_TEXTURE_RANGE_POINTER_APPLE 0x85B8
#define GL_TEXTURE_STORAGE_HINT_APPLE 0x85BC
#define GL_STORAGE_PRIVATE_APPLE 0x85BD
#define GL_STORAGE_CACHED_APPLE 0x85BE
#define GL_STORAGE_SHARED_APPLE 0x85BF

typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC) (GLenum target, GLenum pname, GLvoid **params);
typedef void (GLAPIENTRY * PFNGLTEXTURERANGEAPPLEPROC) (GLenum target, GLsizei length, GLvoid *pointer);


#define GLEW_APPLE_texture_range GLEW_GET_VAR(__GLEW_APPLE_texture_range)

#endif /* GL_APPLE_texture_range */

/* ------------------------ GL_APPLE_transform_hint ------------------------ */

#ifndef GL_APPLE_transform_hint
#define GL_APPLE_transform_hint 1

#define GL_TRANSFORM_HINT_APPLE 0x85B1

#define GLEW_APPLE_transform_hint GLEW_GET_VAR(__GLEW_APPLE_transform_hint)

#endif /* GL_APPLE_transform_hint */

/* ---------------------- GL_APPLE_vertex_array_object --------------------- */

#ifndef GL_APPLE_vertex_array_object
#define GL_APPLE_vertex_array_object 1

#define GL_VERTEX_ARRAY_BINDING_APPLE 0x85B5

typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYAPPLEPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXARRAYSAPPLEPROC) (GLsizei n, const GLuint* arrays);
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSAPPLEPROC) (GLsizei n, const GLuint* arrays);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXARRAYAPPLEPROC) (GLuint array);


#define GLEW_APPLE_vertex_array_object GLEW_GET_VAR(__GLEW_APPLE_vertex_array_object)

#endif /* GL_APPLE_vertex_array_object */

/* ---------------------- GL_APPLE_vertex_array_range ---------------------- */

#ifndef GL_APPLE_vertex_array_range
#define GL_APPLE_vertex_array_range 1

#define GL_VERTEX_ARRAY_RANGE_APPLE 0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE 0x851E
#define GL_VERTEX_ARRAY_STORAGE_HINT_APPLE 0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_APPLE 0x8521
#define GL_STORAGE_CLIENT_APPLE 0x85B4
#define GL_STORAGE_CACHED_APPLE 0x85BE
#define GL_STORAGE_SHARED_APPLE 0x85BF

typedef void (GLAPIENTRY * PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC) (GLsizei length, void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYPARAMETERIAPPLEPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYRANGEAPPLEPROC) (GLsizei length, void* pointer);


#define GLEW_APPLE_vertex_array_range GLEW_GET_VAR(__GLEW_APPLE_vertex_array_range)

#endif /* GL_APPLE_vertex_array_range */

/* ------------------- GL_APPLE_vertex_program_evaluators ------------------ */

#ifndef GL_APPLE_vertex_program_evaluators
#define GL_APPLE_vertex_program_evaluators 1

#define GL_VERTEX_ATTRIB_MAP1_APPLE 0x8A00
#define GL_VERTEX_ATTRIB_MAP2_APPLE 0x8A01
#define GL_VERTEX_ATTRIB_MAP1_SIZE_APPLE 0x8A02
#define GL_VERTEX_ATTRIB_MAP1_COEFF_APPLE 0x8A03
#define GL_VERTEX_ATTRIB_MAP1_ORDER_APPLE 0x8A04
#define GL_VERTEX_ATTRIB_MAP1_DOMAIN_APPLE 0x8A05
#define GL_VERTEX_ATTRIB_MAP2_SIZE_APPLE 0x8A06
#define GL_VERTEX_ATTRIB_MAP2_COEFF_APPLE 0x8A07
#define GL_VERTEX_ATTRIB_MAP2_ORDER_APPLE 0x8A08
#define GL_VERTEX_ATTRIB_MAP2_DOMAIN_APPLE 0x8A09

typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBAPPLEPROC) (GLuint index, GLenum pname);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBAPPLEPROC) (GLuint index, GLenum pname);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXATTRIBENABLEDAPPLEPROC) (GLuint index, GLenum pname);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB1DAPPLEPROC) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB1FAPPLEPROC) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB2DAPPLEPROC) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB2FAPPLEPROC) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points);


#define GLEW_APPLE_vertex_program_evaluators GLEW_GET_VAR(__GLEW_APPLE_vertex_program_evaluators)

#endif /* GL_APPLE_vertex_program_evaluators */

/* --------------------------- GL_APPLE_ycbcr_422 -------------------------- */

#ifndef GL_APPLE_ycbcr_422
#define GL_APPLE_ycbcr_422 1

#define GL_YCBCR_422_APPLE 0x85B9

#define GLEW_APPLE_ycbcr_422 GLEW_GET_VAR(__GLEW_APPLE_ycbcr_422)

#endif /* GL_APPLE_ycbcr_422 */

/* ------------------------ GL_ARB_ES2_compatibility ----------------------- */

#ifndef GL_ARB_ES2_compatibility
#define GL_ARB_ES2_compatibility 1

#define GL_FIXED 0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_RGB565 0x8D62
#define GL_LOW_FLOAT 0x8DF0
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_HIGH_FLOAT 0x8DF2
#define GL_LOW_INT 0x8DF3
#define GL_MEDIUM_INT 0x8DF4
#define GL_HIGH_INT 0x8DF5
#define GL_SHADER_BINARY_FORMATS 0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_SHADER_COMPILER 0x8DFA
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD

typedef void (GLAPIENTRY * PFNGLCLEARDEPTHFPROC) (GLclampf d);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEFPROC) (GLclampf n, GLclampf f);
typedef void (GLAPIENTRY * PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint *precision);
typedef void (GLAPIENTRY * PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void (GLAPIENTRY * PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint* shaders, GLenum binaryformat, const GLvoid*binary, GLsizei length);


#define GLEW_ARB_ES2_compatibility GLEW_GET_VAR(__GLEW_ARB_ES2_compatibility)

#endif /* GL_ARB_ES2_compatibility */

/* ------------------------ GL_ARB_ES3_compatibility ----------------------- */

#ifndef GL_ARB_ES3_compatibility
#define GL_ARB_ES3_compatibility 1

#define GL_PRIMITIVE_RESTART_FIXED_INDEX 0x8D69
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_MAX_ELEMENT_INDEX 0x8D6B
#define GL_COMPRESSED_R11_EAC 0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#define GL_COMPRESSED_RG11_EAC 0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#define GL_COMPRESSED_SRGB8_ETC2 0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279

#define GLEW_ARB_ES3_compatibility GLEW_GET_VAR(__GLEW_ARB_ES3_compatibility)

#endif /* GL_ARB_ES3_compatibility */

/* ------------------------ GL_ARB_arrays_of_arrays ------------------------ */

#ifndef GL_ARB_arrays_of_arrays
#define GL_ARB_arrays_of_arrays 1

#define GLEW_ARB_arrays_of_arrays GLEW_GET_VAR(__GLEW_ARB_arrays_of_arrays)

#endif /* GL_ARB_arrays_of_arrays */

/* -------------------------- GL_ARB_base_instance ------------------------- */

#ifndef GL_ARB_base_instance
#define GL_ARB_base_instance 1

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);


#define GLEW_ARB_base_instance GLEW_GET_VAR(__GLEW_ARB_base_instance)

#endif /* GL_ARB_base_instance */

/* ----------------------- GL_ARB_blend_func_extended ---------------------- */

#ifndef GL_ARB_blend_func_extended
#define GL_ARB_blend_func_extended 1

#define GL_SRC1_COLOR 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC

typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) (GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATAINDEXPROC) (GLuint program, const GLchar * name);


#define GLEW_ARB_blend_func_extended GLEW_GET_VAR(__GLEW_ARB_blend_func_extended)

#endif /* GL_ARB_blend_func_extended */

/* ---------------------------- GL_ARB_cl_event ---------------------------- */

#ifndef GL_ARB_cl_event
#define GL_ARB_cl_event 1

#define GL_SYNC_CL_EVENT_ARB 0x8240
#define GL_SYNC_CL_EVENT_COMPLETE_ARB 0x8241

typedef struct _cl_context *cl_context;
typedef struct _cl_event *cl_event;

typedef GLsync (GLAPIENTRY * PFNGLCREATESYNCFROMCLEVENTARBPROC) (cl_context context, cl_event event, GLbitfield flags);


#define GLEW_ARB_cl_event GLEW_GET_VAR(__GLEW_ARB_cl_event)

#endif /* GL_ARB_cl_event */

/* ----------------------- GL_ARB_clear_buffer_object ---------------------- */

#ifndef GL_ARB_clear_buffer_object
#define GL_ARB_clear_buffer_object 1

typedef void (GLAPIENTRY * PFNGLCLEARBUFFERDATAPROC) (GLenum target, GLenum internalformat, GLenum format, GLenum type, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERSUBDATAPROC) (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid* data);


#define GLEW_ARB_clear_buffer_object GLEW_GET_VAR(__GLEW_ARB_clear_buffer_object)

#endif /* GL_ARB_clear_buffer_object */

/* ----------------------- GL_ARB_color_buffer_float ----------------------- */

#ifndef GL_ARB_color_buffer_float
#define GL_ARB_color_buffer_float 1

#define GL_RGBA_FLOAT_MODE_ARB 0x8820
#define GL_CLAMP_VERTEX_COLOR_ARB 0x891A
#define GL_CLAMP_FRAGMENT_COLOR_ARB 0x891B
#define GL_CLAMP_READ_COLOR_ARB 0x891C
#define GL_FIXED_ONLY_ARB 0x891D

typedef void (GLAPIENTRY * PFNGLCLAMPCOLORARBPROC) (GLenum target, GLenum clamp);


#define GLEW_ARB_color_buffer_float GLEW_GET_VAR(__GLEW_ARB_color_buffer_float)

#endif /* GL_ARB_color_buffer_float */

/* -------------------------- GL_ARB_compatibility ------------------------- */

#ifndef GL_ARB_compatibility
#define GL_ARB_compatibility 1

#define GLEW_ARB_compatibility GLEW_GET_VAR(__GLEW_ARB_compatibility)

#endif /* GL_ARB_compatibility */

/* ---------------- GL_ARB_compressed_texture_pixel_storage ---------------- */

#ifndef GL_ARB_compressed_texture_pixel_storage
#define GL_ARB_compressed_texture_pixel_storage 1

#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH 0x9127
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT 0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH 0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE 0x912A
#define GL_PACK_COMPRESSED_BLOCK_WIDTH 0x912B
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT 0x912C
#define GL_PACK_COMPRESSED_BLOCK_DEPTH 0x912D
#define GL_PACK_COMPRESSED_BLOCK_SIZE 0x912E

#define GLEW_ARB_compressed_texture_pixel_storage GLEW_GET_VAR(__GLEW_ARB_compressed_texture_pixel_storage)

#endif /* GL_ARB_compressed_texture_pixel_storage */

/* ------------------------- GL_ARB_compute_shader ------------------------- */

#ifndef GL_ARB_compute_shader
#define GL_ARB_compute_shader 1

#define GL_COMPUTE_SHADER_BIT 0x00000020
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_COMPUTE_WORK_GROUP_SIZE 0x8267
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER 0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF

typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEINDIRECTPROC) (GLintptr indirect);


#define GLEW_ARB_compute_shader GLEW_GET_VAR(__GLEW_ARB_compute_shader)

#endif /* GL_ARB_compute_shader */

/* ----------------------- GL_ARB_conservative_depth ----------------------- */

#ifndef GL_ARB_conservative_depth
#define GL_ARB_conservative_depth 1

#define GLEW_ARB_conservative_depth GLEW_GET_VAR(__GLEW_ARB_conservative_depth)

#endif /* GL_ARB_conservative_depth */

/* --------------------------- GL_ARB_copy_buffer -------------------------- */

#ifndef GL_ARB_copy_buffer
#define GL_ARB_copy_buffer 1

#define GL_COPY_READ_BUFFER 0x8F36
#define GL_COPY_WRITE_BUFFER 0x8F37

typedef void (GLAPIENTRY * PFNGLCOPYBUFFERSUBDATAPROC) (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);


#define GLEW_ARB_copy_buffer GLEW_GET_VAR(__GLEW_ARB_copy_buffer)

#endif /* GL_ARB_copy_buffer */

/* --------------------------- GL_ARB_copy_image --------------------------- */

#ifndef GL_ARB_copy_image
#define GL_ARB_copy_image 1

typedef void (GLAPIENTRY * PFNGLCOPYIMAGESUBDATAPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);


#define GLEW_ARB_copy_image GLEW_GET_VAR(__GLEW_ARB_copy_image)

#endif /* GL_ARB_copy_image */

/* -------------------------- GL_ARB_debug_output -------------------------- */

#ifndef GL_ARB_debug_output
#define GL_ARB_debug_output 1

#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION_ARB 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB 0x8245
#define GL_DEBUG_SOURCE_API_ARB 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB 0x8249
#define GL_DEBUG_SOURCE_APPLICATION_ARB 0x824A
#define GL_DEBUG_SOURCE_OTHER_ARB 0x824B
#define GL_DEBUG_TYPE_ERROR_ARB 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB 0x824E
#define GL_DEBUG_TYPE_PORTABILITY_ARB 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE_ARB 0x8250
#define GL_DEBUG_TYPE_OTHER_ARB 0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB 0x9144
#define GL_DEBUG_LOGGED_MESSAGES_ARB 0x9145
#define GL_DEBUG_SEVERITY_HIGH_ARB 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB 0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB 0x9148

typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKARBPROC) (GLDEBUGPROCARB callback, void* userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECONTROLARBPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTARBPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGARBPROC) (GLuint count, GLsizei bufsize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);


#define GLEW_ARB_debug_output GLEW_GET_VAR(__GLEW_ARB_debug_output)

#endif /* GL_ARB_debug_output */

/* ----------------------- GL_ARB_depth_buffer_float ----------------------- */

#ifndef GL_ARB_depth_buffer_float
#define GL_ARB_depth_buffer_float 1

#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD

#define GLEW_ARB_depth_buffer_float GLEW_GET_VAR(__GLEW_ARB_depth_buffer_float)

#endif /* GL_ARB_depth_buffer_float */

/* --------------------------- GL_ARB_depth_clamp -------------------------- */

#ifndef GL_ARB_depth_clamp
#define GL_ARB_depth_clamp 1

#define GL_DEPTH_CLAMP 0x864F

#define GLEW_ARB_depth_clamp GLEW_GET_VAR(__GLEW_ARB_depth_clamp)

#endif /* GL_ARB_depth_clamp */

/* -------------------------- GL_ARB_depth_texture ------------------------- */

#ifndef GL_ARB_depth_texture
#define GL_ARB_depth_texture 1

#define GL_DEPTH_COMPONENT16_ARB 0x81A5
#define GL_DEPTH_COMPONENT24_ARB 0x81A6
#define GL_DEPTH_COMPONENT32_ARB 0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB 0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB 0x884B

#define GLEW_ARB_depth_texture GLEW_GET_VAR(__GLEW_ARB_depth_texture)

#endif /* GL_ARB_depth_texture */

/* -------------------------- GL_ARB_draw_buffers -------------------------- */

#ifndef GL_ARB_draw_buffers
#define GL_ARB_draw_buffers 1

#define GL_MAX_DRAW_BUFFERS_ARB 0x8824
#define GL_DRAW_BUFFER0_ARB 0x8825
#define GL_DRAW_BUFFER1_ARB 0x8826
#define GL_DRAW_BUFFER2_ARB 0x8827
#define GL_DRAW_BUFFER3_ARB 0x8828
#define GL_DRAW_BUFFER4_ARB 0x8829
#define GL_DRAW_BUFFER5_ARB 0x882A
#define GL_DRAW_BUFFER6_ARB 0x882B
#define GL_DRAW_BUFFER7_ARB 0x882C
#define GL_DRAW_BUFFER8_ARB 0x882D
#define GL_DRAW_BUFFER9_ARB 0x882E
#define GL_DRAW_BUFFER10_ARB 0x882F
#define GL_DRAW_BUFFER11_ARB 0x8830
#define GL_DRAW_BUFFER12_ARB 0x8831
#define GL_DRAW_BUFFER13_ARB 0x8832
#define GL_DRAW_BUFFER14_ARB 0x8833
#define GL_DRAW_BUFFER15_ARB 0x8834

typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSARBPROC) (GLsizei n, const GLenum* bufs);


#define GLEW_ARB_draw_buffers GLEW_GET_VAR(__GLEW_ARB_draw_buffers)

#endif /* GL_ARB_draw_buffers */

/* ----------------------- GL_ARB_draw_buffers_blend ----------------------- */

#ifndef GL_ARB_draw_buffers_blend
#define GL_ARB_draw_buffers_blend 1

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEIARBPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONIARBPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEIARBPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCIARBPROC) (GLuint buf, GLenum src, GLenum dst);


#define GLEW_ARB_draw_buffers_blend GLEW_GET_VAR(__GLEW_ARB_draw_buffers_blend)

#endif /* GL_ARB_draw_buffers_blend */

/* -------------------- GL_ARB_draw_elements_base_vertex ------------------- */

#ifndef GL_ARB_draw_elements_base_vertex
#define GL_ARB_draw_elements_base_vertex 1

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, void* indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei* count, GLenum type, GLvoid**indices, GLsizei primcount, GLint *basevertex);


#define GLEW_ARB_draw_elements_base_vertex GLEW_GET_VAR(__GLEW_ARB_draw_elements_base_vertex)

#endif /* GL_ARB_draw_elements_base_vertex */

/* -------------------------- GL_ARB_draw_indirect ------------------------- */

#ifndef GL_ARB_draw_indirect
#define GL_ARB_draw_indirect 1

#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING 0x8F43

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINDIRECTPROC) (GLenum mode, const void* indirect);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void* indirect);


#define GLEW_ARB_draw_indirect GLEW_GET_VAR(__GLEW_ARB_draw_indirect)

#endif /* GL_ARB_draw_indirect */

/* ------------------------- GL_ARB_draw_instanced ------------------------- */

#ifndef GL_ARB_draw_instanced
#define GL_ARB_draw_instanced 1

#define GLEW_ARB_draw_instanced GLEW_GET_VAR(__GLEW_ARB_draw_instanced)

#endif /* GL_ARB_draw_instanced */

/* -------------------- GL_ARB_explicit_attrib_location -------------------- */

#ifndef GL_ARB_explicit_attrib_location
#define GL_ARB_explicit_attrib_location 1

#define GLEW_ARB_explicit_attrib_location GLEW_GET_VAR(__GLEW_ARB_explicit_attrib_location)

#endif /* GL_ARB_explicit_attrib_location */

/* -------------------- GL_ARB_explicit_uniform_location ------------------- */

#ifndef GL_ARB_explicit_uniform_location
#define GL_ARB_explicit_uniform_location 1

#define GL_MAX_UNIFORM_LOCATIONS 0x826E

#define GLEW_ARB_explicit_uniform_location GLEW_GET_VAR(__GLEW_ARB_explicit_uniform_location)

#endif /* GL_ARB_explicit_uniform_location */

/* ------------------- GL_ARB_fragment_coord_conventions ------------------- */

#ifndef GL_ARB_fragment_coord_conventions
#define GL_ARB_fragment_coord_conventions 1

#define GLEW_ARB_fragment_coord_conventions GLEW_GET_VAR(__GLEW_ARB_fragment_coord_conventions)

#endif /* GL_ARB_fragment_coord_conventions */

/* --------------------- GL_ARB_fragment_layer_viewport -------------------- */

#ifndef GL_ARB_fragment_layer_viewport
#define GL_ARB_fragment_layer_viewport 1

#define GLEW_ARB_fragment_layer_viewport GLEW_GET_VAR(__GLEW_ARB_fragment_layer_viewport)

#endif /* GL_ARB_fragment_layer_viewport */

/* ------------------------ GL_ARB_fragment_program ------------------------ */

#ifndef GL_ARB_fragment_program
#define GL_ARB_fragment_program 1

#define GL_FRAGMENT_PROGRAM_ARB 0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB 0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB 0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB 0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#define GL_MAX_TEXTURE_COORDS_ARB 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB 0x8872

#define GLEW_ARB_fragment_program GLEW_GET_VAR(__GLEW_ARB_fragment_program)

#endif /* GL_ARB_fragment_program */

/* --------------------- GL_ARB_fragment_program_shadow -------------------- */

#ifndef GL_ARB_fragment_program_shadow
#define GL_ARB_fragment_program_shadow 1

#define GLEW_ARB_fragment_program_shadow GLEW_GET_VAR(__GLEW_ARB_fragment_program_shadow)

#endif /* GL_ARB_fragment_program_shadow */

/* ------------------------- GL_ARB_fragment_shader ------------------------ */

#ifndef GL_ARB_fragment_shader
#define GL_ARB_fragment_shader 1

#define GL_FRAGMENT_SHADER_ARB 0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB 0x8B49
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB 0x8B8B

#define GLEW_ARB_fragment_shader GLEW_GET_VAR(__GLEW_ARB_fragment_shader)

#endif /* GL_ARB_fragment_shader */

/* ------------------- GL_ARB_framebuffer_no_attachments ------------------- */

#ifndef GL_ARB_framebuffer_no_attachments
#define GL_ARB_framebuffer_no_attachments 1

#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS 0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC) (GLuint framebuffer, GLenum pname, GLint param);


#define GLEW_ARB_framebuffer_no_attachments GLEW_GET_VAR(__GLEW_ARB_framebuffer_no_attachments)

#endif /* GL_ARB_framebuffer_no_attachments */

/* ----------------------- GL_ARB_framebuffer_object ----------------------- */

#ifndef GL_ARB_framebuffer_object
#define GL_ARB_framebuffer_object 1

#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_INDEX 0x8222
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_SRGB 0x8C40
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57

typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target,GLenum attachment, GLuint texture,GLint level,GLint layer);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);


#define GLEW_ARB_framebuffer_object GLEW_GET_VAR(__GLEW_ARB_framebuffer_object)

#endif /* GL_ARB_framebuffer_object */

/* ------------------------ GL_ARB_framebuffer_sRGB ------------------------ */

#ifndef GL_ARB_framebuffer_sRGB
#define GL_ARB_framebuffer_sRGB 1

#define GL_FRAMEBUFFER_SRGB 0x8DB9

#define GLEW_ARB_framebuffer_sRGB GLEW_GET_VAR(__GLEW_ARB_framebuffer_sRGB)

#endif /* GL_ARB_framebuffer_sRGB */

/* ------------------------ GL_ARB_geometry_shader4 ------------------------ */

#ifndef GL_ARB_geometry_shader4
#define GL_ARB_geometry_shader4 1

#define GL_LINES_ADJACENCY_ARB 0xA
#define GL_LINE_STRIP_ADJACENCY_ARB 0xB
#define GL_TRIANGLES_ADJACENCY_ARB 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_ARB 0xD
#define GL_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_ARB 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB 0x8DA9
#define GL_GEOMETRY_SHADER_ARB 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT_ARB 0x8DDA
#define GL_GEOMETRY_INPUT_TYPE_ARB 0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE_ARB 0x8DDC
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_ARB 0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS_ARB 0x8DDE
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB 0x8DE1

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIARBPROC) (GLuint program, GLenum pname, GLint value);


#define GLEW_ARB_geometry_shader4 GLEW_GET_VAR(__GLEW_ARB_geometry_shader4)

#endif /* GL_ARB_geometry_shader4 */

/* ----------------------- GL_ARB_get_program_binary ----------------------- */

#ifndef GL_ARB_get_program_binary
#define GL_ARB_get_program_binary 1

#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH 0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS 0x87FE
#define GL_PROGRAM_BINARY_FORMATS 0x87FF

typedef void (GLAPIENTRY * PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum *binaryFormat, GLvoid*binary);
typedef void (GLAPIENTRY * PFNGLPROGRAMBINARYPROC) (GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);


#define GLEW_ARB_get_program_binary GLEW_GET_VAR(__GLEW_ARB_get_program_binary)

#endif /* GL_ARB_get_program_binary */

/* --------------------------- GL_ARB_gpu_shader5 -------------------------- */

#ifndef GL_ARB_gpu_shader5
#define GL_ARB_gpu_shader5 1

#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MAX_VERTEX_STREAMS 0x8E71

#define GLEW_ARB_gpu_shader5 GLEW_GET_VAR(__GLEW_ARB_gpu_shader5)

#endif /* GL_ARB_gpu_shader5 */

/* ------------------------- GL_ARB_gpu_shader_fp64 ------------------------ */

#ifndef GL_ARB_gpu_shader_fp64
#define GL_ARB_gpu_shader_fp64 1

#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_MAT2x3 0x8F49
#define GL_DOUBLE_MAT2x4 0x8F4A
#define GL_DOUBLE_MAT3x2 0x8F4B
#define GL_DOUBLE_MAT3x4 0x8F4C
#define GL_DOUBLE_MAT4x2 0x8F4D
#define GL_DOUBLE_MAT4x3 0x8F4E
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETUNIFORMDVPROC) (GLuint program, GLint location, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLUNIFORM1DPROC) (GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2DPROC) (GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);


#define GLEW_ARB_gpu_shader_fp64 GLEW_GET_VAR(__GLEW_ARB_gpu_shader_fp64)

#endif /* GL_ARB_gpu_shader_fp64 */

/* ------------------------ GL_ARB_half_float_pixel ------------------------ */

#ifndef GL_ARB_half_float_pixel
#define GL_ARB_half_float_pixel 1

#define GL_HALF_FLOAT_ARB 0x140B

#define GLEW_ARB_half_float_pixel GLEW_GET_VAR(__GLEW_ARB_half_float_pixel)

#endif /* GL_ARB_half_float_pixel */

/* ------------------------ GL_ARB_half_float_vertex ----------------------- */

#ifndef GL_ARB_half_float_vertex
#define GL_ARB_half_float_vertex 1

#define GL_HALF_FLOAT 0x140B

#define GLEW_ARB_half_float_vertex GLEW_GET_VAR(__GLEW_ARB_half_float_vertex)

#endif /* GL_ARB_half_float_vertex */

/* ----------------------------- GL_ARB_imaging ---------------------------- */

#ifndef GL_ARB_imaging
#define GL_ARB_imaging 1

#define GL_CONSTANT_COLOR 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_BLEND_COLOR 0x8005
#define GL_FUNC_ADD 0x8006
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_BLEND_EQUATION 0x8009
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_CONVOLUTION_1D 0x8010
#define GL_CONVOLUTION_2D 0x8011
#define GL_SEPARABLE_2D 0x8012
#define GL_CONVOLUTION_BORDER_MODE 0x8013
#define GL_CONVOLUTION_FILTER_SCALE 0x8014
#define GL_CONVOLUTION_FILTER_BIAS 0x8015
#define GL_REDUCE 0x8016
#define GL_CONVOLUTION_FORMAT 0x8017
#define GL_CONVOLUTION_WIDTH 0x8018
#define GL_CONVOLUTION_HEIGHT 0x8019
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#define GL_POST_CONVOLUTION_RED_SCALE 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS 0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS 0x8023
#define GL_HISTOGRAM 0x8024
#define GL_PROXY_HISTOGRAM 0x8025
#define GL_HISTOGRAM_WIDTH 0x8026
#define GL_HISTOGRAM_FORMAT 0x8027
#define GL_HISTOGRAM_RED_SIZE 0x8028
#define GL_HISTOGRAM_GREEN_SIZE 0x8029
#define GL_HISTOGRAM_BLUE_SIZE 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE 0x802C
#define GL_HISTOGRAM_SINK 0x802D
#define GL_MINMAX 0x802E
#define GL_MINMAX_FORMAT 0x802F
#define GL_MINMAX_SINK 0x8030
#define GL_TABLE_TOO_LARGE 0x8031
#define GL_COLOR_MATRIX 0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH 0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH 0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE 0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE 0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE 0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE 0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS 0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS 0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS 0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS 0x80BB
#define GL_COLOR_TABLE 0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE 0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE 0x80D2
#define GL_PROXY_COLOR_TABLE 0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_COLOR_TABLE_SCALE 0x80D6
#define GL_COLOR_TABLE_BIAS 0x80D7
#define GL_COLOR_TABLE_FORMAT 0x80D8
#define GL_COLOR_TABLE_WIDTH 0x80D9
#define GL_COLOR_TABLE_RED_SIZE 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE 0x80DF
#define GL_IGNORE_BORDER 0x8150
#define GL_CONSTANT_BORDER 0x8151
#define GL_WRAP_BORDER 0x8152
#define GL_REPLICATE_BORDER 0x8153
#define GL_CONVOLUTION_BORDER_COLOR 0x8154

typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIPROC) (GLenum target, GLenum pname, GLint params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPROC) (GLenum target, GLenum format, GLenum type, GLvoid *table);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONFILTERPROC) (GLenum target, GLenum format, GLenum type, GLvoid *image);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPROC) (GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETSEPARABLEFILTERPROC) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
typedef void (GLAPIENTRY * PFNGLHISTOGRAMPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLMINMAXPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLRESETHISTOGRAMPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLRESETMINMAXPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLSEPARABLEFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);


#define GLEW_ARB_imaging GLEW_GET_VAR(__GLEW_ARB_imaging)

#endif /* GL_ARB_imaging */

/* ------------------------ GL_ARB_instanced_arrays ------------------------ */

#ifndef GL_ARB_instanced_arrays
#define GL_ARB_instanced_arrays 1

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB 0x88FE

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDARBPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDARBPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISORARBPROC) (GLuint index, GLuint divisor);


#define GLEW_ARB_instanced_arrays GLEW_GET_VAR(__GLEW_ARB_instanced_arrays)

#endif /* GL_ARB_instanced_arrays */

/* ---------------------- GL_ARB_internalformat_query ---------------------- */

#ifndef GL_ARB_internalformat_query
#define GL_ARB_internalformat_query 1

#define GL_NUM_SAMPLE_COUNTS 0x9380

typedef void (GLAPIENTRY * PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);


#define GLEW_ARB_internalformat_query GLEW_GET_VAR(__GLEW_ARB_internalformat_query)

#endif /* GL_ARB_internalformat_query */

/* ---------------------- GL_ARB_internalformat_query2 --------------------- */

#ifndef GL_ARB_internalformat_query2
#define GL_ARB_internalformat_query2 1

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x806F
#define GL_SAMPLES 0x80A9
#define GL_INTERNALFORMAT_SUPPORTED 0x826F
#define GL_INTERNALFORMAT_PREFERRED 0x8270
#define GL_INTERNALFORMAT_RED_SIZE 0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE 0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE 0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE 0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE 0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE 0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE 0x8277
#define GL_INTERNALFORMAT_RED_TYPE 0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE 0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE 0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE 0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE 0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE 0x827D
#define GL_MAX_WIDTH 0x827E
#define GL_MAX_HEIGHT 0x827F
#define GL_MAX_DEPTH 0x8280
#define GL_MAX_LAYERS 0x8281
#define GL_MAX_COMBINED_DIMENSIONS 0x8282
#define GL_COLOR_COMPONENTS 0x8283
#define GL_DEPTH_COMPONENTS 0x8284
#define GL_STENCIL_COMPONENTS 0x8285
#define GL_COLOR_RENDERABLE 0x8286
#define GL_DEPTH_RENDERABLE 0x8287
#define GL_STENCIL_RENDERABLE 0x8288
#define GL_FRAMEBUFFER_RENDERABLE 0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED 0x828A
#define GL_FRAMEBUFFER_BLEND 0x828B
#define GL_READ_PIXELS 0x828C
#define GL_READ_PIXELS_FORMAT 0x828D
#define GL_READ_PIXELS_TYPE 0x828E
#define GL_TEXTURE_IMAGE_FORMAT 0x828F
#define GL_TEXTURE_IMAGE_TYPE 0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT 0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE 0x8292
#define GL_MIPMAP 0x8293
#define GL_MANUAL_GENERATE_MIPMAP 0x8294
#define GL_AUTO_GENERATE_MIPMAP 0x8295
#define GL_COLOR_ENCODING 0x8296
#define GL_SRGB_READ 0x8297
#define GL_SRGB_WRITE 0x8298
#define GL_SRGB_DECODE_ARB 0x8299
#define GL_FILTER 0x829A
#define GL_VERTEX_TEXTURE 0x829B
#define GL_TESS_CONTROL_TEXTURE 0x829C
#define GL_TESS_EVALUATION_TEXTURE 0x829D
#define GL_GEOMETRY_TEXTURE 0x829E
#define GL_FRAGMENT_TEXTURE 0x829F
#define GL_COMPUTE_TEXTURE 0x82A0
#define GL_TEXTURE_SHADOW 0x82A1
#define GL_TEXTURE_GATHER 0x82A2
#define GL_TEXTURE_GATHER_SHADOW 0x82A3
#define GL_SHADER_IMAGE_LOAD 0x82A4
#define GL_SHADER_IMAGE_STORE 0x82A5
#define GL_SHADER_IMAGE_ATOMIC 0x82A6
#define GL_IMAGE_TEXEL_SIZE 0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS 0x82A8
#define GL_IMAGE_PIXEL_FORMAT 0x82A9
#define GL_IMAGE_PIXEL_TYPE 0x82AA
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST 0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST 0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE 0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE 0x82AF
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH 0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE 0x82B3
#define GL_CLEAR_BUFFER 0x82B4
#define GL_TEXTURE_VIEW 0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS 0x82B6
#define GL_FULL_SUPPORT 0x82B7
#define GL_CAVEAT_SUPPORT 0x82B8
#define GL_IMAGE_CLASS_4_X_32 0x82B9
#define GL_IMAGE_CLASS_2_X_32 0x82BA
#define GL_IMAGE_CLASS_1_X_32 0x82BB
#define GL_IMAGE_CLASS_4_X_16 0x82BC
#define GL_IMAGE_CLASS_2_X_16 0x82BD
#define GL_IMAGE_CLASS_1_X_16 0x82BE
#define GL_IMAGE_CLASS_4_X_8 0x82BF
#define GL_IMAGE_CLASS_2_X_8 0x82C0
#define GL_IMAGE_CLASS_1_X_8 0x82C1
#define GL_IMAGE_CLASS_11_11_10 0x82C2
#define GL_IMAGE_CLASS_10_10_10_2 0x82C3
#define GL_VIEW_CLASS_128_BITS 0x82C4
#define GL_VIEW_CLASS_96_BITS 0x82C5
#define GL_VIEW_CLASS_64_BITS 0x82C6
#define GL_VIEW_CLASS_48_BITS 0x82C7
#define GL_VIEW_CLASS_32_BITS 0x82C8
#define GL_VIEW_CLASS_24_BITS 0x82C9
#define GL_VIEW_CLASS_16_BITS 0x82CA
#define GL_VIEW_CLASS_8_BITS 0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB 0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA 0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA 0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA 0x82CF
#define GL_VIEW_CLASS_RGTC1_RED 0x82D0
#define GL_VIEW_CLASS_RGTC2_RG 0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM 0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT 0x82D3
#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_RENDERBUFFER 0x8D41
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_NUM_SAMPLE_COUNTS 0x9380

typedef void (GLAPIENTRY * PFNGLGETINTERNALFORMATI64VPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64* params);


#define GLEW_ARB_internalformat_query2 GLEW_GET_VAR(__GLEW_ARB_internalformat_query2)

#endif /* GL_ARB_internalformat_query2 */

/* ----------------------- GL_ARB_invalidate_subdata ----------------------- */

#ifndef GL_ARB_invalidate_subdata
#define GL_ARB_invalidate_subdata 1

typedef void (GLAPIENTRY * PFNGLINVALIDATEBUFFERDATAPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLINVALIDATEBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLINVALIDATEFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum* attachments);
typedef void (GLAPIENTRY * PFNGLINVALIDATESUBFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLINVALIDATETEXIMAGEPROC) (GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLINVALIDATETEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);


#define GLEW_ARB_invalidate_subdata GLEW_GET_VAR(__GLEW_ARB_invalidate_subdata)

#endif /* GL_ARB_invalidate_subdata */

/* ---------------------- GL_ARB_map_buffer_alignment ---------------------- */

#ifndef GL_ARB_map_buffer_alignment
#define GL_ARB_map_buffer_alignment 1

#define GL_MIN_MAP_BUFFER_ALIGNMENT 0x90BC

#define GLEW_ARB_map_buffer_alignment GLEW_GET_VAR(__GLEW_ARB_map_buffer_alignment)

#endif /* GL_ARB_map_buffer_alignment */

/* ------------------------ GL_ARB_map_buffer_range ------------------------ */

#ifndef GL_ARB_map_buffer_range
#define GL_ARB_map_buffer_range 1

#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020

typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);


#define GLEW_ARB_map_buffer_range GLEW_GET_VAR(__GLEW_ARB_map_buffer_range)

#endif /* GL_ARB_map_buffer_range */

/* ------------------------- GL_ARB_matrix_palette ------------------------- */

#ifndef GL_ARB_matrix_palette
#define GL_ARB_matrix_palette 1

#define GL_MATRIX_PALETTE_ARB 0x8840
#define GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB 0x8841
#define GL_MAX_PALETTE_MATRICES_ARB 0x8842
#define GL_CURRENT_PALETTE_MATRIX_ARB 0x8843
#define GL_MATRIX_INDEX_ARRAY_ARB 0x8844
#define GL_CURRENT_MATRIX_INDEX_ARB 0x8845
#define GL_MATRIX_INDEX_ARRAY_SIZE_ARB 0x8846
#define GL_MATRIX_INDEX_ARRAY_TYPE_ARB 0x8847
#define GL_MATRIX_INDEX_ARRAY_STRIDE_ARB 0x8848
#define GL_MATRIX_INDEX_ARRAY_POINTER_ARB 0x8849

typedef void (GLAPIENTRY * PFNGLCURRENTPALETTEMATRIXARBPROC) (GLint index);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUBVARBPROC) (GLint size, GLubyte *indices);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUIVARBPROC) (GLint size, GLuint *indices);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUSVARBPROC) (GLint size, GLushort *indices);


#define GLEW_ARB_matrix_palette GLEW_GET_VAR(__GLEW_ARB_matrix_palette)

#endif /* GL_ARB_matrix_palette */

/* ----------------------- GL_ARB_multi_draw_indirect ---------------------- */

#ifndef GL_ARB_multi_draw_indirect
#define GL_ARB_multi_draw_indirect 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTPROC) (GLenum mode, const void* indirect, GLsizei primcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void* indirect, GLsizei primcount, GLsizei stride);


#define GLEW_ARB_multi_draw_indirect GLEW_GET_VAR(__GLEW_ARB_multi_draw_indirect)

#endif /* GL_ARB_multi_draw_indirect */

/* --------------------------- GL_ARB_multisample -------------------------- */

#ifndef GL_ARB_multisample
#define GL_ARB_multisample 1

#define GL_MULTISAMPLE_ARB 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB 0x809F
#define GL_SAMPLE_COVERAGE_ARB 0x80A0
#define GL_SAMPLE_BUFFERS_ARB 0x80A8
#define GL_SAMPLES_ARB 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB 0x80AB
#define GL_MULTISAMPLE_BIT_ARB 0x20000000

typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEARBPROC) (GLclampf value, GLboolean invert);


#define GLEW_ARB_multisample GLEW_GET_VAR(__GLEW_ARB_multisample)

#endif /* GL_ARB_multisample */

/* -------------------------- GL_ARB_multitexture -------------------------- */

#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1

#define GL_TEXTURE0_ARB 0x84C0
#define GL_TEXTURE1_ARB 0x84C1
#define GL_TEXTURE2_ARB 0x84C2
#define GL_TEXTURE3_ARB 0x84C3
#define GL_TEXTURE4_ARB 0x84C4
#define GL_TEXTURE5_ARB 0x84C5
#define GL_TEXTURE6_ARB 0x84C6
#define GL_TEXTURE7_ARB 0x84C7
#define GL_TEXTURE8_ARB 0x84C8
#define GL_TEXTURE9_ARB 0x84C9
#define GL_TEXTURE10_ARB 0x84CA
#define GL_TEXTURE11_ARB 0x84CB
#define GL_TEXTURE12_ARB 0x84CC
#define GL_TEXTURE13_ARB 0x84CD
#define GL_TEXTURE14_ARB 0x84CE
#define GL_TEXTURE15_ARB 0x84CF
#define GL_TEXTURE16_ARB 0x84D0
#define GL_TEXTURE17_ARB 0x84D1
#define GL_TEXTURE18_ARB 0x84D2
#define GL_TEXTURE19_ARB 0x84D3
#define GL_TEXTURE20_ARB 0x84D4
#define GL_TEXTURE21_ARB 0x84D5
#define GL_TEXTURE22_ARB 0x84D6
#define GL_TEXTURE23_ARB 0x84D7
#define GL_TEXTURE24_ARB 0x84D8
#define GL_TEXTURE25_ARB 0x84D9
#define GL_TEXTURE26_ARB 0x84DA
#define GL_TEXTURE27_ARB 0x84DB
#define GL_TEXTURE28_ARB 0x84DC
#define GL_TEXTURE29_ARB 0x84DD
#define GL_TEXTURE30_ARB 0x84DE
#define GL_TEXTURE31_ARB 0x84DF
#define GL_ACTIVE_TEXTURE_ARB 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB 0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB 0x84E2

typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FARBPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IARBPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SARBPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IARBPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SARBPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IARBPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVARBPROC) (GLenum target, const GLshort *v);


#define GLEW_ARB_multitexture GLEW_GET_VAR(__GLEW_ARB_multitexture)

#endif /* GL_ARB_multitexture */

/* ------------------------- GL_ARB_occlusion_query ------------------------ */

#ifndef GL_ARB_occlusion_query
#define GL_ARB_occlusion_query 1

#define GL_QUERY_COUNTER_BITS_ARB 0x8864
#define GL_CURRENT_QUERY_ARB 0x8865
#define GL_QUERY_RESULT_ARB 0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB 0x8867
#define GL_SAMPLES_PASSED_ARB 0x8914

typedef void (GLAPIENTRY * PFNGLBEGINQUERYARBPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESARBPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDQUERYARBPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENQUERIESARBPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVARBPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVARBPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYARBPROC) (GLuint id);


#define GLEW_ARB_occlusion_query GLEW_GET_VAR(__GLEW_ARB_occlusion_query)

#endif /* GL_ARB_occlusion_query */

/* ------------------------ GL_ARB_occlusion_query2 ------------------------ */

#ifndef GL_ARB_occlusion_query2
#define GL_ARB_occlusion_query2 1

#define GL_ANY_SAMPLES_PASSED 0x8C2F

#define GLEW_ARB_occlusion_query2 GLEW_GET_VAR(__GLEW_ARB_occlusion_query2)

#endif /* GL_ARB_occlusion_query2 */

/* ----------------------- GL_ARB_pixel_buffer_object ---------------------- */

#ifndef GL_ARB_pixel_buffer_object
#define GL_ARB_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_ARB 0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB 0x88EF

#define GLEW_ARB_pixel_buffer_object GLEW_GET_VAR(__GLEW_ARB_pixel_buffer_object)

#endif /* GL_ARB_pixel_buffer_object */

/* ------------------------ GL_ARB_point_parameters ------------------------ */

#ifndef GL_ARB_point_parameters
#define GL_ARB_point_parameters 1

#define GL_POINT_SIZE_MIN_ARB 0x8126
#define GL_POINT_SIZE_MAX_ARB 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB 0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFARBPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVARBPROC) (GLenum pname, const GLfloat* params);


#define GLEW_ARB_point_parameters GLEW_GET_VAR(__GLEW_ARB_point_parameters)

#endif /* GL_ARB_point_parameters */

/* -------------------------- GL_ARB_point_sprite -------------------------- */

#ifndef GL_ARB_point_sprite
#define GL_ARB_point_sprite 1

#define GL_POINT_SPRITE_ARB 0x8861
#define GL_COORD_REPLACE_ARB 0x8862

#define GLEW_ARB_point_sprite GLEW_GET_VAR(__GLEW_ARB_point_sprite)

#endif /* GL_ARB_point_sprite */

/* --------------------- GL_ARB_program_interface_query -------------------- */

#ifndef GL_ARB_program_interface_query
#define GL_ARB_program_interface_query 1

#define GL_UNIFORM 0x92E1
#define GL_UNIFORM_BLOCK 0x92E2
#define GL_PROGRAM_INPUT 0x92E3
#define GL_PROGRAM_OUTPUT 0x92E4
#define GL_BUFFER_VARIABLE 0x92E5
#define GL_SHADER_STORAGE_BLOCK 0x92E6
#define GL_IS_PER_PATCH 0x92E7
#define GL_VERTEX_SUBROUTINE 0x92E8
#define GL_TESS_CONTROL_SUBROUTINE 0x92E9
#define GL_TESS_EVALUATION_SUBROUTINE 0x92EA
#define GL_GEOMETRY_SUBROUTINE 0x92EB
#define GL_FRAGMENT_SUBROUTINE 0x92EC
#define GL_COMPUTE_SUBROUTINE 0x92ED
#define GL_VERTEX_SUBROUTINE_UNIFORM 0x92EE
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM 0x92EF
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM 0x92F0
#define GL_GEOMETRY_SUBROUTINE_UNIFORM 0x92F1
#define GL_FRAGMENT_SUBROUTINE_UNIFORM 0x92F2
#define GL_COMPUTE_SUBROUTINE_UNIFORM 0x92F3
#define GL_TRANSFORM_FEEDBACK_VARYING 0x92F4
#define GL_ACTIVE_RESOURCES 0x92F5
#define GL_MAX_NAME_LENGTH 0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES 0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES 0x92F8
#define GL_NAME_LENGTH 0x92F9
#define GL_TYPE 0x92FA
#define GL_ARRAY_SIZE 0x92FB
#define GL_OFFSET 0x92FC
#define GL_BLOCK_INDEX 0x92FD
#define GL_ARRAY_STRIDE 0x92FE
#define GL_MATRIX_STRIDE 0x92FF
#define GL_IS_ROW_MAJOR 0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX 0x9301
#define GL_BUFFER_BINDING 0x9302
#define GL_BUFFER_DATA_SIZE 0x9303
#define GL_NUM_ACTIVE_VARIABLES 0x9304
#define GL_ACTIVE_VARIABLES 0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER 0x9306
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_REFERENCED_BY_GEOMETRY_SHADER 0x9309
#define GL_REFERENCED_BY_FRAGMENT_SHADER 0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER 0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE 0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE 0x930D
#define GL_LOCATION 0x930E
#define GL_LOCATION_INDEX 0x930F

typedef void (GLAPIENTRY * PFNGLGETPROGRAMINTERFACEIVPROC) (GLuint program, GLenum programInterface, GLenum pname, GLint* params);
typedef GLuint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCEINDEXPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCELOCATIONPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMRESOURCENAMEPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMRESOURCEIVPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei *length, GLint *params);


#define GLEW_ARB_program_interface_query GLEW_GET_VAR(__GLEW_ARB_program_interface_query)

#endif /* GL_ARB_program_interface_query */

/* ------------------------ GL_ARB_provoking_vertex ------------------------ */

#ifndef GL_ARB_provoking_vertex
#define GL_ARB_provoking_vertex 1

#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F

typedef void (GLAPIENTRY * PFNGLPROVOKINGVERTEXPROC) (GLenum mode);


#define GLEW_ARB_provoking_vertex GLEW_GET_VAR(__GLEW_ARB_provoking_vertex)

#endif /* GL_ARB_provoking_vertex */

/* ------------------ GL_ARB_robust_buffer_access_behavior ----------------- */

#ifndef GL_ARB_robust_buffer_access_behavior
#define GL_ARB_robust_buffer_access_behavior 1

#define GLEW_ARB_robust_buffer_access_behavior GLEW_GET_VAR(__GLEW_ARB_robust_buffer_access_behavior)

#endif /* GL_ARB_robust_buffer_access_behavior */

/* --------------------------- GL_ARB_robustness --------------------------- */

#ifndef GL_ARB_robustness
#define GL_ARB_robustness 1

#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB 0x00000004
#define GL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define GL_GUILTY_CONTEXT_RESET_ARB 0x8253
#define GL_INNOCENT_CONTEXT_RESET_ARB 0x8254
#define GL_UNKNOWN_CONTEXT_RESET_ARB 0x8255
#define GL_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define GL_NO_RESET_NOTIFICATION_ARB 0x8261

typedef GLenum (GLAPIENTRY * PFNGLGETGRAPHICSRESETSTATUSARBPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETNCOLORTABLEARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* table);
typedef void (GLAPIENTRY * PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, GLsizei bufSize, void* img);
typedef void (GLAPIENTRY * PFNGLGETNCONVOLUTIONFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* image);
typedef void (GLAPIENTRY * PFNGLGETNHISTOGRAMARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void (GLAPIENTRY * PFNGLGETNMAPDVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLdouble* v);
typedef void (GLAPIENTRY * PFNGLGETNMAPFVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLfloat* v);
typedef void (GLAPIENTRY * PFNGLGETNMAPIVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLint* v);
typedef void (GLAPIENTRY * PFNGLGETNMINMAXARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPFVARBPROC) (GLenum map, GLsizei bufSize, GLfloat* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPUIVARBPROC) (GLenum map, GLsizei bufSize, GLuint* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPUSVARBPROC) (GLenum map, GLsizei bufSize, GLushort* values);
typedef void (GLAPIENTRY * PFNGLGETNPOLYGONSTIPPLEARBPROC) (GLsizei bufSize, GLubyte* pattern);
typedef void (GLAPIENTRY * PFNGLGETNSEPARABLEFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void* row, GLsizei columnBufSize, GLvoid*column, GLvoid*span);
typedef void (GLAPIENTRY * PFNGLGETNTEXIMAGEARBPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* img);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMDVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMFVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMUIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint* params);
typedef void (GLAPIENTRY * PFNGLREADNPIXELSARBPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);


#define GLEW_ARB_robustness GLEW_GET_VAR(__GLEW_ARB_robustness)

#endif /* GL_ARB_robustness */

/* ---------------- GL_ARB_robustness_application_isolation ---------------- */

#ifndef GL_ARB_robustness_application_isolation
#define GL_ARB_robustness_application_isolation 1

#define GLEW_ARB_robustness_application_isolation GLEW_GET_VAR(__GLEW_ARB_robustness_application_isolation)

#endif /* GL_ARB_robustness_application_isolation */

/* ---------------- GL_ARB_robustness_share_group_isolation ---------------- */

#ifndef GL_ARB_robustness_share_group_isolation
#define GL_ARB_robustness_share_group_isolation 1

#define GLEW_ARB_robustness_share_group_isolation GLEW_GET_VAR(__GLEW_ARB_robustness_share_group_isolation)

#endif /* GL_ARB_robustness_share_group_isolation */

/* ------------------------- GL_ARB_sample_shading ------------------------- */

#ifndef GL_ARB_sample_shading
#define GL_ARB_sample_shading 1

#define GL_SAMPLE_SHADING_ARB 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE_ARB 0x8C37

typedef void (GLAPIENTRY * PFNGLMINSAMPLESHADINGARBPROC) (GLclampf value);


#define GLEW_ARB_sample_shading GLEW_GET_VAR(__GLEW_ARB_sample_shading)

#endif /* GL_ARB_sample_shading */

/* ------------------------- GL_ARB_sampler_objects ------------------------ */

#ifndef GL_ARB_sampler_objects
#define GL_ARB_sampler_objects 1

#define GL_SAMPLER_BINDING 0x8919

typedef void (GLAPIENTRY * PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
typedef void (GLAPIENTRY * PFNGLDELETESAMPLERSPROC) (GLsizei count, const GLuint * samplers);
typedef void (GLAPIENTRY * PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint* samplers);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISSAMPLERPROC) (GLuint sampler);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, const GLint* params);


#define GLEW_ARB_sampler_objects GLEW_GET_VAR(__GLEW_ARB_sampler_objects)

#endif /* GL_ARB_sampler_objects */

/* ------------------------ GL_ARB_seamless_cube_map ----------------------- */

#ifndef GL_ARB_seamless_cube_map
#define GL_ARB_seamless_cube_map 1

#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F

#define GLEW_ARB_seamless_cube_map GLEW_GET_VAR(__GLEW_ARB_seamless_cube_map)

#endif /* GL_ARB_seamless_cube_map */

/* --------------------- GL_ARB_separate_shader_objects -------------------- */

#ifndef GL_ARB_separate_shader_objects
#define GL_ARB_separate_shader_objects 1

#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_PROGRAM_SEPARABLE 0x8258
#define GL_ACTIVE_PROGRAM 0x8259
#define GL_PROGRAM_PIPELINE_BINDING 0x825A
#define GL_ALL_SHADER_BITS 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
typedef void (GLAPIENTRY * PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const GLchar ** strings);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint* pipelines);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar *infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DPROC) (GLuint program, GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);


#define GLEW_ARB_separate_shader_objects GLEW_GET_VAR(__GLEW_ARB_separate_shader_objects)

#endif /* GL_ARB_separate_shader_objects */

/* --------------------- GL_ARB_shader_atomic_counters --------------------- */

#ifndef GL_ARB_shader_atomic_counters
#define GL_ARB_shader_atomic_counters 1

#define GL_ATOMIC_COUNTER_BUFFER 0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING 0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START 0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE 0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS 0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER 0x92DB
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC

typedef void (GLAPIENTRY * PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) (GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);


#define GLEW_ARB_shader_atomic_counters GLEW_GET_VAR(__GLEW_ARB_shader_atomic_counters)

#endif /* GL_ARB_shader_atomic_counters */

/* ----------------------- GL_ARB_shader_bit_encoding ---------------------- */

#ifndef GL_ARB_shader_bit_encoding
#define GL_ARB_shader_bit_encoding 1

#define GLEW_ARB_shader_bit_encoding GLEW_GET_VAR(__GLEW_ARB_shader_bit_encoding)

#endif /* GL_ARB_shader_bit_encoding */

/* --------------------- GL_ARB_shader_image_load_store -------------------- */

#ifndef GL_ARB_shader_image_load_store
#define GL_ARB_shader_image_load_store 1

#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT 0x00000002
#define GL_UNIFORM_BARRIER_BIT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT 0x00001000
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS 0x8F39
#define GL_IMAGE_BINDING_NAME 0x8F3A
#define GL_IMAGE_BINDING_LEVEL 0x8F3B
#define GL_IMAGE_BINDING_LAYERED 0x8F3C
#define GL_IMAGE_BINDING_LAYER 0x8F3D
#define GL_IMAGE_BINDING_ACCESS 0x8F3E
#define GL_IMAGE_1D 0x904C
#define GL_IMAGE_2D 0x904D
#define GL_IMAGE_3D 0x904E
#define GL_IMAGE_2D_RECT 0x904F
#define GL_IMAGE_CUBE 0x9050
#define GL_IMAGE_BUFFER 0x9051
#define GL_IMAGE_1D_ARRAY 0x9052
#define GL_IMAGE_2D_ARRAY 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY 0x9054
#define GL_IMAGE_2D_MULTISAMPLE 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY 0x9056
#define GL_INT_IMAGE_1D 0x9057
#define GL_INT_IMAGE_2D 0x9058
#define GL_INT_IMAGE_3D 0x9059
#define GL_INT_IMAGE_2D_RECT 0x905A
#define GL_INT_IMAGE_CUBE 0x905B
#define GL_INT_IMAGE_BUFFER 0x905C
#define GL_INT_IMAGE_1D_ARRAY 0x905D
#define GL_INT_IMAGE_2D_ARRAY 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY 0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D 0x9062
#define GL_UNSIGNED_INT_IMAGE_2D 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D 0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER 0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x906C
#define GL_MAX_IMAGE_SAMPLES 0x906D
#define GL_IMAGE_BINDING_FORMAT 0x906E
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS 0x90CA
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS 0x90CD
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS 0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS 0x90CF
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (GLAPIENTRY * PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);


#define GLEW_ARB_shader_image_load_store GLEW_GET_VAR(__GLEW_ARB_shader_image_load_store)

#endif /* GL_ARB_shader_image_load_store */

/* ------------------------ GL_ARB_shader_image_size ----------------------- */

#ifndef GL_ARB_shader_image_size
#define GL_ARB_shader_image_size 1

#define GLEW_ARB_shader_image_size GLEW_GET_VAR(__GLEW_ARB_shader_image_size)

#endif /* GL_ARB_shader_image_size */

/* ------------------------- GL_ARB_shader_objects ------------------------- */

#ifndef GL_ARB_shader_objects
#define GL_ARB_shader_objects 1

#define GL_PROGRAM_OBJECT_ARB 0x8B40
#define GL_SHADER_OBJECT_ARB 0x8B48
#define GL_OBJECT_TYPE_ARB 0x8B4E
#define GL_OBJECT_SUBTYPE_ARB 0x8B4F
#define GL_FLOAT_VEC2_ARB 0x8B50
#define GL_FLOAT_VEC3_ARB 0x8B51
#define GL_FLOAT_VEC4_ARB 0x8B52
#define GL_INT_VEC2_ARB 0x8B53
#define GL_INT_VEC3_ARB 0x8B54
#define GL_INT_VEC4_ARB 0x8B55
#define GL_BOOL_ARB 0x8B56
#define GL_BOOL_VEC2_ARB 0x8B57
#define GL_BOOL_VEC3_ARB 0x8B58
#define GL_BOOL_VEC4_ARB 0x8B59
#define GL_FLOAT_MAT2_ARB 0x8B5A
#define GL_FLOAT_MAT3_ARB 0x8B5B
#define GL_FLOAT_MAT4_ARB 0x8B5C
#define GL_SAMPLER_1D_ARB 0x8B5D
#define GL_SAMPLER_2D_ARB 0x8B5E
#define GL_SAMPLER_3D_ARB 0x8B5F
#define GL_SAMPLER_CUBE_ARB 0x8B60
#define GL_SAMPLER_1D_SHADOW_ARB 0x8B61
#define GL_SAMPLER_2D_SHADOW_ARB 0x8B62
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64
#define GL_OBJECT_DELETE_STATUS_ARB 0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB 0x8B81
#define GL_OBJECT_LINK_STATUS_ARB 0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB 0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB 0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB 0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB 0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88

typedef char GLcharARB;
typedef unsigned int GLhandleARB;

typedef void (GLAPIENTRY * PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERARBPROC) (GLhandleARB shaderObj);
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void (GLAPIENTRY * PFNGLDELETEOBJECTARBPROC) (GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDOBJECTSARBPROC) (GLhandleARB containerObj, GLsizei maxCount, GLsizei* count, GLhandleARB *obj);
typedef GLhandleARB (GLAPIENTRY * PFNGLGETHANDLEARBPROC) (GLenum pname);
typedef void (GLAPIENTRY * PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERFVARBPROC) (GLhandleARB obj, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERIVARBPROC) (GLhandleARB obj, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *source);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVARBPROC) (GLhandleARB programObj, GLint location, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVARBPROC) (GLhandleARB programObj, GLint location, GLint* params);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMARBPROC) (GLhandleARB programObj);


#define GLEW_ARB_shader_objects GLEW_GET_VAR(__GLEW_ARB_shader_objects)

#endif /* GL_ARB_shader_objects */

/* ------------------------ GL_ARB_shader_precision ------------------------ */

#ifndef GL_ARB_shader_precision
#define GL_ARB_shader_precision 1

#define GLEW_ARB_shader_precision GLEW_GET_VAR(__GLEW_ARB_shader_precision)

#endif /* GL_ARB_shader_precision */

/* ---------------------- GL_ARB_shader_stencil_export --------------------- */

#ifndef GL_ARB_shader_stencil_export
#define GL_ARB_shader_stencil_export 1

#define GLEW_ARB_shader_stencil_export GLEW_GET_VAR(__GLEW_ARB_shader_stencil_export)

#endif /* GL_ARB_shader_stencil_export */

/* ------------------ GL_ARB_shader_storage_buffer_object ------------------ */

#ifndef GL_ARB_shader_storage_buffer_object
#define GL_ARB_shader_storage_buffer_object 1

#define GL_SHADER_STORAGE_BARRIER_BIT 0x2000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF

typedef void (GLAPIENTRY * PFNGLSHADERSTORAGEBLOCKBINDINGPROC) (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);


#define GLEW_ARB_shader_storage_buffer_object GLEW_GET_VAR(__GLEW_ARB_shader_storage_buffer_object)

#endif /* GL_ARB_shader_storage_buffer_object */

/* ------------------------ GL_ARB_shader_subroutine ----------------------- */

#ifndef GL_ARB_shader_subroutine
#define GL_ARB_shader_subroutine 1

#define GL_ACTIVE_SUBROUTINES 0x8DE5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS 0x8DE6
#define GL_MAX_SUBROUTINES 0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS 0x8E47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH 0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH 0x8E49
#define GL_NUM_COMPATIBLE_SUBROUTINES 0x8E4A
#define GL_COMPATIBLE_SUBROUTINES 0x8E4B

typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINENAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTAGEIVPROC) (GLuint program, GLenum shadertype, GLenum pname, GLint* values);
typedef GLuint (GLAPIENTRY * PFNGLGETSUBROUTINEINDEXPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMSUBROUTINEUIVPROC) (GLenum shadertype, GLint location, GLuint* params);
typedef void (GLAPIENTRY * PFNGLUNIFORMSUBROUTINESUIVPROC) (GLenum shadertype, GLsizei count, const GLuint* indices);


#define GLEW_ARB_shader_subroutine GLEW_GET_VAR(__GLEW_ARB_shader_subroutine)

#endif /* GL_ARB_shader_subroutine */

/* ----------------------- GL_ARB_shader_texture_lod ----------------------- */

#ifndef GL_ARB_shader_texture_lod
#define GL_ARB_shader_texture_lod 1

#define GLEW_ARB_shader_texture_lod GLEW_GET_VAR(__GLEW_ARB_shader_texture_lod)

#endif /* GL_ARB_shader_texture_lod */

/* ---------------------- GL_ARB_shading_language_100 ---------------------- */

#ifndef GL_ARB_shading_language_100
#define GL_ARB_shading_language_100 1

#define GL_SHADING_LANGUAGE_VERSION_ARB 0x8B8C

#define GLEW_ARB_shading_language_100 GLEW_GET_VAR(__GLEW_ARB_shading_language_100)

#endif /* GL_ARB_shading_language_100 */

/* -------------------- GL_ARB_shading_language_420pack -------------------- */

#ifndef GL_ARB_shading_language_420pack
#define GL_ARB_shading_language_420pack 1

#define GLEW_ARB_shading_language_420pack GLEW_GET_VAR(__GLEW_ARB_shading_language_420pack)

#endif /* GL_ARB_shading_language_420pack */

/* -------------------- GL_ARB_shading_language_include -------------------- */

#ifndef GL_ARB_shading_language_include
#define GL_ARB_shading_language_include 1

#define GL_SHADER_INCLUDE_ARB 0x8DAE
#define GL_NAMED_STRING_LENGTH_ARB 0x8DE9
#define GL_NAMED_STRING_TYPE_ARB 0x8DEA

typedef void (GLAPIENTRY * PFNGLCOMPILESHADERINCLUDEARBPROC) (GLuint shader, GLsizei count, const GLchar ** path, const GLint *length);
typedef void (GLAPIENTRY * PFNGLDELETENAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETNAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name, GLsizei bufSize, GLint *stringlen, GLchar *string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDSTRINGIVARBPROC) (GLint namelen, const GLchar* name, GLenum pname, GLint *params);
typedef GLboolean (GLAPIENTRY * PFNGLISNAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLNAMEDSTRINGARBPROC) (GLenum type, GLint namelen, const GLchar* name, GLint stringlen, const GLchar *string);


#define GLEW_ARB_shading_language_include GLEW_GET_VAR(__GLEW_ARB_shading_language_include)

#endif /* GL_ARB_shading_language_include */

/* -------------------- GL_ARB_shading_language_packing -------------------- */

#ifndef GL_ARB_shading_language_packing
#define GL_ARB_shading_language_packing 1

#define GLEW_ARB_shading_language_packing GLEW_GET_VAR(__GLEW_ARB_shading_language_packing)

#endif /* GL_ARB_shading_language_packing */

/* ----------------------------- GL_ARB_shadow ----------------------------- */

#ifndef GL_ARB_shadow
#define GL_ARB_shadow 1

#define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E

#define GLEW_ARB_shadow GLEW_GET_VAR(__GLEW_ARB_shadow)

#endif /* GL_ARB_shadow */

/* ------------------------- GL_ARB_shadow_ambient ------------------------- */

#ifndef GL_ARB_shadow_ambient
#define GL_ARB_shadow_ambient 1

#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB 0x80BF

#define GLEW_ARB_shadow_ambient GLEW_GET_VAR(__GLEW_ARB_shadow_ambient)

#endif /* GL_ARB_shadow_ambient */

/* ------------------------ GL_ARB_stencil_texturing ----------------------- */

#ifndef GL_ARB_stencil_texturing
#define GL_ARB_stencil_texturing 1

#define GL_DEPTH_STENCIL_TEXTURE_MODE 0x90EA

#define GLEW_ARB_stencil_texturing GLEW_GET_VAR(__GLEW_ARB_stencil_texturing)

#endif /* GL_ARB_stencil_texturing */

/* ------------------------------ GL_ARB_sync ------------------------------ */

#ifndef GL_ARB_sync
#define GL_ARB_sync 1

#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_OBJECT_TYPE 0x9112
#define GL_SYNC_CONDITION 0x9113
#define GL_SYNC_STATUS 0x9114
#define GL_SYNC_FLAGS 0x9115
#define GL_SYNC_FENCE 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_UNSIGNALED 0x9118
#define GL_SIGNALED 0x9119
#define GL_ALREADY_SIGNALED 0x911A
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_CONDITION_SATISFIED 0x911C
#define GL_WAIT_FAILED 0x911D
#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF

typedef GLenum (GLAPIENTRY * PFNGLCLIENTWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);
typedef void (GLAPIENTRY * PFNGLDELETESYNCPROC) (GLsync GLsync);
typedef GLsync (GLAPIENTRY * PFNGLFENCESYNCPROC) (GLenum condition,GLbitfield flags);
typedef void (GLAPIENTRY * PFNGLGETINTEGER64VPROC) (GLenum pname, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETSYNCIVPROC) (GLsync GLsync,GLenum pname,GLsizei bufSize,GLsizei* length, GLint *values);
typedef GLboolean (GLAPIENTRY * PFNGLISSYNCPROC) (GLsync GLsync);
typedef void (GLAPIENTRY * PFNGLWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);


#define GLEW_ARB_sync GLEW_GET_VAR(__GLEW_ARB_sync)

#endif /* GL_ARB_sync */

/* ----------------------- GL_ARB_tessellation_shader ---------------------- */

#ifndef GL_ARB_tessellation_shader
#define GL_ARB_tessellation_shader 1

#define GL_PATCHES 0xE
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER 0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER 0x84F1
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_PATCH_VERTICES 0x8E72
#define GL_PATCH_DEFAULT_INNER_LEVEL 0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL 0x8E74
#define GL_TESS_CONTROL_OUTPUT_VERTICES 0x8E75
#define GL_TESS_GEN_MODE 0x8E76
#define GL_TESS_GEN_SPACING 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER 0x8E78
#define GL_TESS_GEN_POINT_MODE 0x8E79
#define GL_ISOLINES 0x8E7A
#define GL_FRACTIONAL_ODD 0x8E7B
#define GL_FRACTIONAL_EVEN 0x8E7C
#define GL_MAX_PATCH_VERTICES 0x8E7D
#define GL_MAX_TESS_GEN_LEVEL 0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS 0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A

typedef void (GLAPIENTRY * PFNGLPATCHPARAMETERFVPROC) (GLenum pname, const GLfloat* values);
typedef void (GLAPIENTRY * PFNGLPATCHPARAMETERIPROC) (GLenum pname, GLint value);


#define GLEW_ARB_tessellation_shader GLEW_GET_VAR(__GLEW_ARB_tessellation_shader)

#endif /* GL_ARB_tessellation_shader */

/* ---------------------- GL_ARB_texture_border_clamp ---------------------- */

#ifndef GL_ARB_texture_border_clamp
#define GL_ARB_texture_border_clamp 1

#define GL_CLAMP_TO_BORDER_ARB 0x812D

#define GLEW_ARB_texture_border_clamp GLEW_GET_VAR(__GLEW_ARB_texture_border_clamp)

#endif /* GL_ARB_texture_border_clamp */

/* ---------------------- GL_ARB_texture_buffer_object --------------------- */

#ifndef GL_ARB_texture_buffer_object
#define GL_ARB_texture_buffer_object 1

#define GL_TEXTURE_BUFFER_ARB 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_ARB 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_ARB 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_ARB 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT_ARB 0x8C2E

typedef void (GLAPIENTRY * PFNGLTEXBUFFERARBPROC) (GLenum target, GLenum internalformat, GLuint buffer);


#define GLEW_ARB_texture_buffer_object GLEW_GET_VAR(__GLEW_ARB_texture_buffer_object)

#endif /* GL_ARB_texture_buffer_object */

/* ------------------- GL_ARB_texture_buffer_object_rgb32 ------------------ */

#ifndef GL_ARB_texture_buffer_object_rgb32
#define GL_ARB_texture_buffer_object_rgb32 1

#define GLEW_ARB_texture_buffer_object_rgb32 GLEW_GET_VAR(__GLEW_ARB_texture_buffer_object_rgb32)

#endif /* GL_ARB_texture_buffer_object_rgb32 */

/* ---------------------- GL_ARB_texture_buffer_range ---------------------- */

#ifndef GL_ARB_texture_buffer_range
#define GL_ARB_texture_buffer_range 1

#define GL_TEXTURE_BUFFER_OFFSET 0x919D
#define GL_TEXTURE_BUFFER_SIZE 0x919E
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F

typedef void (GLAPIENTRY * PFNGLTEXBUFFERRANGEPROC) (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFERRANGEEXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);


#define GLEW_ARB_texture_buffer_range GLEW_GET_VAR(__GLEW_ARB_texture_buffer_range)

#endif /* GL_ARB_texture_buffer_range */

/* ----------------------- GL_ARB_texture_compression ---------------------- */

#ifndef GL_ARB_texture_compression
#define GL_ARB_texture_compression 1

#define GL_COMPRESSED_ALPHA_ARB 0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB 0x84EB
#define GL_COMPRESSED_INTENSITY_ARB 0x84EC
#define GL_COMPRESSED_RGB_ARB 0x84ED
#define GL_COMPRESSED_RGBA_ARB 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB 0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB 0x86A0
#define GL_TEXTURE_COMPRESSED_ARB 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A3

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, void* img);


#define GLEW_ARB_texture_compression GLEW_GET_VAR(__GLEW_ARB_texture_compression)

#endif /* GL_ARB_texture_compression */

/* -------------------- GL_ARB_texture_compression_bptc -------------------- */

#ifndef GL_ARB_texture_compression_bptc
#define GL_ARB_texture_compression_bptc 1

#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB 0x8E8F

#define GLEW_ARB_texture_compression_bptc GLEW_GET_VAR(__GLEW_ARB_texture_compression_bptc)

#endif /* GL_ARB_texture_compression_bptc */

/* -------------------- GL_ARB_texture_compression_rgtc -------------------- */

#ifndef GL_ARB_texture_compression_rgtc
#define GL_ARB_texture_compression_rgtc 1

#define GL_COMPRESSED_RED_RGTC1 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1 0x8DBC
#define GL_COMPRESSED_RG_RGTC2 0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2 0x8DBE

#define GLEW_ARB_texture_compression_rgtc GLEW_GET_VAR(__GLEW_ARB_texture_compression_rgtc)

#endif /* GL_ARB_texture_compression_rgtc */

/* ------------------------ GL_ARB_texture_cube_map ------------------------ */

#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map 1

#define GL_NORMAL_MAP_ARB 0x8511
#define GL_REFLECTION_MAP_ARB 0x8512
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB 0x851C

#define GLEW_ARB_texture_cube_map GLEW_GET_VAR(__GLEW_ARB_texture_cube_map)

#endif /* GL_ARB_texture_cube_map */

/* --------------------- GL_ARB_texture_cube_map_array --------------------- */

#ifndef GL_ARB_texture_cube_map_array
#define GL_ARB_texture_cube_map_array 1

#define GL_TEXTURE_CUBE_MAP_ARRAY_ARB 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900F

#define GLEW_ARB_texture_cube_map_array GLEW_GET_VAR(__GLEW_ARB_texture_cube_map_array)

#endif /* GL_ARB_texture_cube_map_array */

/* ------------------------- GL_ARB_texture_env_add ------------------------ */

#ifndef GL_ARB_texture_env_add
#define GL_ARB_texture_env_add 1

#define GLEW_ARB_texture_env_add GLEW_GET_VAR(__GLEW_ARB_texture_env_add)

#endif /* GL_ARB_texture_env_add */

/* ----------------------- GL_ARB_texture_env_combine ---------------------- */

#ifndef GL_ARB_texture_env_combine
#define GL_ARB_texture_env_combine 1

#define GL_SUBTRACT_ARB 0x84E7
#define GL_COMBINE_ARB 0x8570
#define GL_COMBINE_RGB_ARB 0x8571
#define GL_COMBINE_ALPHA_ARB 0x8572
#define GL_RGB_SCALE_ARB 0x8573
#define GL_ADD_SIGNED_ARB 0x8574
#define GL_INTERPOLATE_ARB 0x8575
#define GL_CONSTANT_ARB 0x8576
#define GL_PRIMARY_COLOR_ARB 0x8577
#define GL_PREVIOUS_ARB 0x8578
#define GL_SOURCE0_RGB_ARB 0x8580
#define GL_SOURCE1_RGB_ARB 0x8581
#define GL_SOURCE2_RGB_ARB 0x8582
#define GL_SOURCE0_ALPHA_ARB 0x8588
#define GL_SOURCE1_ALPHA_ARB 0x8589
#define GL_SOURCE2_ALPHA_ARB 0x858A
#define GL_OPERAND0_RGB_ARB 0x8590
#define GL_OPERAND1_RGB_ARB 0x8591
#define GL_OPERAND2_RGB_ARB 0x8592
#define GL_OPERAND0_ALPHA_ARB 0x8598
#define GL_OPERAND1_ALPHA_ARB 0x8599
#define GL_OPERAND2_ALPHA_ARB 0x859A

#define GLEW_ARB_texture_env_combine GLEW_GET_VAR(__GLEW_ARB_texture_env_combine)

#endif /* GL_ARB_texture_env_combine */

/* ---------------------- GL_ARB_texture_env_crossbar ---------------------- */

#ifndef GL_ARB_texture_env_crossbar
#define GL_ARB_texture_env_crossbar 1

#define GLEW_ARB_texture_env_crossbar GLEW_GET_VAR(__GLEW_ARB_texture_env_crossbar)

#endif /* GL_ARB_texture_env_crossbar */

/* ------------------------ GL_ARB_texture_env_dot3 ------------------------ */

#ifndef GL_ARB_texture_env_dot3
#define GL_ARB_texture_env_dot3 1

#define GL_DOT3_RGB_ARB 0x86AE
#define GL_DOT3_RGBA_ARB 0x86AF

#define GLEW_ARB_texture_env_dot3 GLEW_GET_VAR(__GLEW_ARB_texture_env_dot3)

#endif /* GL_ARB_texture_env_dot3 */

/* -------------------------- GL_ARB_texture_float ------------------------- */

#ifndef GL_ARB_texture_float
#define GL_ARB_texture_float 1

#define GL_RGBA32F_ARB 0x8814
#define GL_RGB32F_ARB 0x8815
#define GL_ALPHA32F_ARB 0x8816
#define GL_INTENSITY32F_ARB 0x8817
#define GL_LUMINANCE32F_ARB 0x8818
#define GL_LUMINANCE_ALPHA32F_ARB 0x8819
#define GL_RGBA16F_ARB 0x881A
#define GL_RGB16F_ARB 0x881B
#define GL_ALPHA16F_ARB 0x881C
#define GL_INTENSITY16F_ARB 0x881D
#define GL_LUMINANCE16F_ARB 0x881E
#define GL_LUMINANCE_ALPHA16F_ARB 0x881F
#define GL_TEXTURE_RED_TYPE_ARB 0x8C10
#define GL_TEXTURE_GREEN_TYPE_ARB 0x8C11
#define GL_TEXTURE_BLUE_TYPE_ARB 0x8C12
#define GL_TEXTURE_ALPHA_TYPE_ARB 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE_ARB 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE_ARB 0x8C15
#define GL_TEXTURE_DEPTH_TYPE_ARB 0x8C16
#define GL_UNSIGNED_NORMALIZED_ARB 0x8C17

#define GLEW_ARB_texture_float GLEW_GET_VAR(__GLEW_ARB_texture_float)

#endif /* GL_ARB_texture_float */

/* ------------------------- GL_ARB_texture_gather ------------------------- */

#ifndef GL_ARB_texture_gather
#define GL_ARB_texture_gather 1

#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB 0x8F9F

#define GLEW_ARB_texture_gather GLEW_GET_VAR(__GLEW_ARB_texture_gather)

#endif /* GL_ARB_texture_gather */

/* --------------------- GL_ARB_texture_mirrored_repeat -------------------- */

#ifndef GL_ARB_texture_mirrored_repeat
#define GL_ARB_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_ARB 0x8370

#define GLEW_ARB_texture_mirrored_repeat GLEW_GET_VAR(__GLEW_ARB_texture_mirrored_repeat)

#endif /* GL_ARB_texture_mirrored_repeat */

/* ----------------------- GL_ARB_texture_multisample ---------------------- */

#ifndef GL_ARB_texture_multisample
#define GL_ARB_texture_multisample 1

#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_INTEGER_SAMPLES 0x9110

typedef void (GLAPIENTRY * PFNGLGETMULTISAMPLEFVPROC) (GLenum pname, GLuint index, GLfloat* val);
typedef void (GLAPIENTRY * PFNGLSAMPLEMASKIPROC) (GLuint index, GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);


#define GLEW_ARB_texture_multisample GLEW_GET_VAR(__GLEW_ARB_texture_multisample)

#endif /* GL_ARB_texture_multisample */

/* -------------------- GL_ARB_texture_non_power_of_two -------------------- */

#ifndef GL_ARB_texture_non_power_of_two
#define GL_ARB_texture_non_power_of_two 1

#define GLEW_ARB_texture_non_power_of_two GLEW_GET_VAR(__GLEW_ARB_texture_non_power_of_two)

#endif /* GL_ARB_texture_non_power_of_two */

/* ---------------------- GL_ARB_texture_query_levels ---------------------- */

#ifndef GL_ARB_texture_query_levels
#define GL_ARB_texture_query_levels 1

#define GLEW_ARB_texture_query_levels GLEW_GET_VAR(__GLEW_ARB_texture_query_levels)

#endif /* GL_ARB_texture_query_levels */

/* ------------------------ GL_ARB_texture_query_lod ----------------------- */

#ifndef GL_ARB_texture_query_lod
#define GL_ARB_texture_query_lod 1

#define GLEW_ARB_texture_query_lod GLEW_GET_VAR(__GLEW_ARB_texture_query_lod)

#endif /* GL_ARB_texture_query_lod */

/* ------------------------ GL_ARB_texture_rectangle ----------------------- */

#ifndef GL_ARB_texture_rectangle
#define GL_ARB_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_ARB 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_ARB 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64

#define GLEW_ARB_texture_rectangle GLEW_GET_VAR(__GLEW_ARB_texture_rectangle)

#endif /* GL_ARB_texture_rectangle */

/* --------------------------- GL_ARB_texture_rg --------------------------- */

#ifndef GL_ARB_texture_rg
#define GL_ARB_texture_rg 1

#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RG 0x8226
#define GL_RG 0x8227
#define GL_RG_INTEGER 0x8228
#define GL_R8 0x8229
#define GL_R16 0x822A
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C

#define GLEW_ARB_texture_rg GLEW_GET_VAR(__GLEW_ARB_texture_rg)

#endif /* GL_ARB_texture_rg */

/* ----------------------- GL_ARB_texture_rgb10_a2ui ----------------------- */

#ifndef GL_ARB_texture_rgb10_a2ui
#define GL_ARB_texture_rgb10_a2ui 1

#define GL_RGB10_A2UI 0x906F

#define GLEW_ARB_texture_rgb10_a2ui GLEW_GET_VAR(__GLEW_ARB_texture_rgb10_a2ui)

#endif /* GL_ARB_texture_rgb10_a2ui */

/* ------------------------- GL_ARB_texture_storage ------------------------ */

#ifndef GL_ARB_texture_storage
#define GL_ARB_texture_storage 1

#define GL_TEXTURE_IMMUTABLE_FORMAT 0x912F

typedef void (GLAPIENTRY * PFNGLTEXSTORAGE1DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE1DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);


#define GLEW_ARB_texture_storage GLEW_GET_VAR(__GLEW_ARB_texture_storage)

#endif /* GL_ARB_texture_storage */

/* ------------------- GL_ARB_texture_storage_multisample ------------------ */

#ifndef GL_ARB_texture_storage_multisample
#define GL_ARB_texture_storage_multisample 1

typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);


#define GLEW_ARB_texture_storage_multisample GLEW_GET_VAR(__GLEW_ARB_texture_storage_multisample)

#endif /* GL_ARB_texture_storage_multisample */

/* ------------------------- GL_ARB_texture_swizzle ------------------------ */

#ifndef GL_ARB_texture_swizzle
#define GL_ARB_texture_swizzle 1

#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46

#define GLEW_ARB_texture_swizzle GLEW_GET_VAR(__GLEW_ARB_texture_swizzle)

#endif /* GL_ARB_texture_swizzle */

/* -------------------------- GL_ARB_texture_view -------------------------- */

#ifndef GL_ARB_texture_view
#define GL_ARB_texture_view 1

#define GL_TEXTURE_VIEW_MIN_LEVEL 0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS 0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER 0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS 0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF

typedef void (GLAPIENTRY * PFNGLTEXTUREVIEWPROC) (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);


#define GLEW_ARB_texture_view GLEW_GET_VAR(__GLEW_ARB_texture_view)

#endif /* GL_ARB_texture_view */

/* --------------------------- GL_ARB_timer_query -------------------------- */

#ifndef GL_ARB_timer_query
#define GL_ARB_timer_query 1

#define GL_TIME_ELAPSED 0x88BF
#define GL_TIMESTAMP 0x8E28

typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTI64VPROC) (GLuint id, GLenum pname, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VPROC) (GLuint id, GLenum pname, GLuint64* params);
typedef void (GLAPIENTRY * PFNGLQUERYCOUNTERPROC) (GLuint id, GLenum target);


#define GLEW_ARB_timer_query GLEW_GET_VAR(__GLEW_ARB_timer_query)

#endif /* GL_ARB_timer_query */

/* ----------------------- GL_ARB_transform_feedback2 ---------------------- */

#ifndef GL_ARB_transform_feedback2
#define GL_ARB_transform_feedback2 1

#define GL_TRANSFORM_FEEDBACK 0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING 0x8E25

typedef void (GLAPIENTRY * PFNGLBINDTRANSFORMFEEDBACKPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETETRANSFORMFEEDBACKSPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKPROC) (GLenum mode, GLuint id);
typedef void (GLAPIENTRY * PFNGLGENTRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint* ids);
typedef GLboolean (GLAPIENTRY * PFNGLISTRANSFORMFEEDBACKPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLPAUSETRANSFORMFEEDBACKPROC) (void);
typedef void (GLAPIENTRY * PFNGLRESUMETRANSFORMFEEDBACKPROC) (void);


#define GLEW_ARB_transform_feedback2 GLEW_GET_VAR(__GLEW_ARB_transform_feedback2)

#endif /* GL_ARB_transform_feedback2 */

/* ----------------------- GL_ARB_transform_feedback3 ---------------------- */

#ifndef GL_ARB_transform_feedback3
#define GL_ARB_transform_feedback3 1

#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70
#define GL_MAX_VERTEX_STREAMS 0x8E71

typedef void (GLAPIENTRY * PFNGLBEGINQUERYINDEXEDPROC) (GLenum target, GLuint index, GLuint id);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) (GLenum mode, GLuint id, GLuint stream);
typedef void (GLAPIENTRY * PFNGLENDQUERYINDEXEDPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETQUERYINDEXEDIVPROC) (GLenum target, GLuint index, GLenum pname, GLint* params);


#define GLEW_ARB_transform_feedback3 GLEW_GET_VAR(__GLEW_ARB_transform_feedback3)

#endif /* GL_ARB_transform_feedback3 */

/* ------------------ GL_ARB_transform_feedback_instanced ------------------ */

#ifndef GL_ARB_transform_feedback_instanced
#define GL_ARB_transform_feedback_instanced 1

typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) (GLenum mode, GLuint id, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) (GLenum mode, GLuint id, GLuint stream, GLsizei primcount);


#define GLEW_ARB_transform_feedback_instanced GLEW_GET_VAR(__GLEW_ARB_transform_feedback_instanced)

#endif /* GL_ARB_transform_feedback_instanced */

/* ------------------------ GL_ARB_transpose_matrix ------------------------ */

#ifndef GL_ARB_transpose_matrix
#define GL_ARB_transpose_matrix 1

#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX_ARB 0x84E6

typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);


#define GLEW_ARB_transpose_matrix GLEW_GET_VAR(__GLEW_ARB_transpose_matrix)

#endif /* GL_ARB_transpose_matrix */

/* ---------------------- GL_ARB_uniform_buffer_object --------------------- */

#ifndef GL_ARB_uniform_buffer_object
#define GL_ARB_uniform_buffer_object 1

#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMBLOCKIVPROC) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMNAMEPROC) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMSIVPROC) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETINTEGERI_VPROC) (GLenum target, GLuint index, GLint* data);
typedef GLuint (GLAPIENTRY * PFNGLGETUNIFORMBLOCKINDEXPROC) (GLuint program, const GLchar* uniformBlockName);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMINDICESPROC) (GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices);
typedef void (GLAPIENTRY * PFNGLUNIFORMBLOCKBINDINGPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);


#define GLEW_ARB_uniform_buffer_object GLEW_GET_VAR(__GLEW_ARB_uniform_buffer_object)

#endif /* GL_ARB_uniform_buffer_object */

/* ------------------------ GL_ARB_vertex_array_bgra ----------------------- */

#ifndef GL_ARB_vertex_array_bgra
#define GL_ARB_vertex_array_bgra 1

#define GL_BGRA 0x80E1

#define GLEW_ARB_vertex_array_bgra GLEW_GET_VAR(__GLEW_ARB_vertex_array_bgra)

#endif /* GL_ARB_vertex_array_bgra */

/* ----------------------- GL_ARB_vertex_array_object ---------------------- */

#ifndef GL_ARB_vertex_array_object
#define GL_ARB_vertex_array_object 1

#define GL_VERTEX_ARRAY_BINDING 0x85B5

typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXARRAYPROC) (GLuint array);


#define GLEW_ARB_vertex_array_object GLEW_GET_VAR(__GLEW_ARB_vertex_array_object)

#endif /* GL_ARB_vertex_array_object */

/* ----------------------- GL_ARB_vertex_attrib_64bit ---------------------- */

#ifndef GL_ARB_vertex_attrib_64bit
#define GL_ARB_vertex_attrib_64bit 1

#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLDVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);


#define GLEW_ARB_vertex_attrib_64bit GLEW_GET_VAR(__GLEW_ARB_vertex_attrib_64bit)

#endif /* GL_ARB_vertex_attrib_64bit */

/* ---------------------- GL_ARB_vertex_attrib_binding --------------------- */

#ifndef GL_ARB_vertex_attrib_binding
#define GL_ARB_vertex_attrib_binding 1

#define GL_VERTEX_ATTRIB_BINDING 0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D5
#define GL_VERTEX_BINDING_DIVISOR 0x82D6
#define GL_VERTEX_BINDING_OFFSET 0x82D7
#define GL_VERTEX_BINDING_STRIDE 0x82D8
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA

typedef void (GLAPIENTRY * PFNGLBINDVERTEXBUFFERPROC) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBBINDINGPROC) (GLuint attribindex, GLuint bindingindex);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXBINDINGDIVISORPROC) (GLuint bindingindex, GLuint divisor);


#define GLEW_ARB_vertex_attrib_binding GLEW_GET_VAR(__GLEW_ARB_vertex_attrib_binding)

#endif /* GL_ARB_vertex_attrib_binding */

/* -------------------------- GL_ARB_vertex_blend -------------------------- */

#ifndef GL_ARB_vertex_blend
#define GL_ARB_vertex_blend 1

#define GL_MODELVIEW0_ARB 0x1700
#define GL_MODELVIEW1_ARB 0x850A
#define GL_MAX_VERTEX_UNITS_ARB 0x86A4
#define GL_ACTIVE_VERTEX_UNITS_ARB 0x86A5
#define GL_WEIGHT_SUM_UNITY_ARB 0x86A6
#define GL_VERTEX_BLEND_ARB 0x86A7
#define GL_CURRENT_WEIGHT_ARB 0x86A8
#define GL_WEIGHT_ARRAY_TYPE_ARB 0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_ARB 0x86AA
#define GL_WEIGHT_ARRAY_SIZE_ARB 0x86AB
#define GL_WEIGHT_ARRAY_POINTER_ARB 0x86AC
#define GL_WEIGHT_ARRAY_ARB 0x86AD
#define GL_MODELVIEW2_ARB 0x8722
#define GL_MODELVIEW3_ARB 0x8723
#define GL_MODELVIEW4_ARB 0x8724
#define GL_MODELVIEW5_ARB 0x8725
#define GL_MODELVIEW6_ARB 0x8726
#define GL_MODELVIEW7_ARB 0x8727
#define GL_MODELVIEW8_ARB 0x8728
#define GL_MODELVIEW9_ARB 0x8729
#define GL_MODELVIEW10_ARB 0x872A
#define GL_MODELVIEW11_ARB 0x872B
#define GL_MODELVIEW12_ARB 0x872C
#define GL_MODELVIEW13_ARB 0x872D
#define GL_MODELVIEW14_ARB 0x872E
#define GL_MODELVIEW15_ARB 0x872F
#define GL_MODELVIEW16_ARB 0x8730
#define GL_MODELVIEW17_ARB 0x8731
#define GL_MODELVIEW18_ARB 0x8732
#define GL_MODELVIEW19_ARB 0x8733
#define GL_MODELVIEW20_ARB 0x8734
#define GL_MODELVIEW21_ARB 0x8735
#define GL_MODELVIEW22_ARB 0x8736
#define GL_MODELVIEW23_ARB 0x8737
#define GL_MODELVIEW24_ARB 0x8738
#define GL_MODELVIEW25_ARB 0x8739
#define GL_MODELVIEW26_ARB 0x873A
#define GL_MODELVIEW27_ARB 0x873B
#define GL_MODELVIEW28_ARB 0x873C
#define GL_MODELVIEW29_ARB 0x873D
#define GL_MODELVIEW30_ARB 0x873E
#define GL_MODELVIEW31_ARB 0x873F

typedef void (GLAPIENTRY * PFNGLVERTEXBLENDARBPROC) (GLint count);
typedef void (GLAPIENTRY * PFNGLWEIGHTPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLWEIGHTBVARBPROC) (GLint size, GLbyte *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTDVARBPROC) (GLint size, GLdouble *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTFVARBPROC) (GLint size, GLfloat *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTIVARBPROC) (GLint size, GLint *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTSVARBPROC) (GLint size, GLshort *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUBVARBPROC) (GLint size, GLubyte *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUIVARBPROC) (GLint size, GLuint *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUSVARBPROC) (GLint size, GLushort *weights);


#define GLEW_ARB_vertex_blend GLEW_GET_VAR(__GLEW_ARB_vertex_blend)

#endif /* GL_ARB_vertex_blend */

/* ---------------------- GL_ARB_vertex_buffer_object ---------------------- */

#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object 1

#define GL_BUFFER_SIZE_ARB 0x8764
#define GL_BUFFER_USAGE_ARB 0x8765
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_READ_ONLY_ARB 0x88B8
#define GL_WRITE_ONLY_ARB 0x88B9
#define GL_READ_WRITE_ARB 0x88BA
#define GL_BUFFER_ACCESS_ARB 0x88BB
#define GL_BUFFER_MAPPED_ARB 0x88BC
#define GL_BUFFER_MAP_POINTER_ARB 0x88BD
#define GL_STREAM_DRAW_ARB 0x88E0
#define GL_STREAM_READ_ARB 0x88E1
#define GL_STREAM_COPY_ARB 0x88E2
#define GL_STATIC_DRAW_ARB 0x88E4
#define GL_STATIC_READ_ARB 0x88E5
#define GL_STATIC_COPY_ARB 0x88E6
#define GL_DYNAMIC_DRAW_ARB 0x88E8
#define GL_DYNAMIC_READ_ARB 0x88E9
#define GL_DYNAMIC_COPY_ARB 0x88EA

typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef void (GLAPIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERARBPROC) (GLuint buffer);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERARBPROC) (GLenum target);


#define GLEW_ARB_vertex_buffer_object GLEW_GET_VAR(__GLEW_ARB_vertex_buffer_object)

#endif /* GL_ARB_vertex_buffer_object */

/* ------------------------- GL_ARB_vertex_program ------------------------- */

#ifndef GL_ARB_vertex_program
#define GL_ARB_vertex_program 1

#define GL_COLOR_SUM_ARB 0x8458
#define GL_VERTEX_PROGRAM_ARB 0x8620
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB 0x8625
#define GL_CURRENT_VERTEX_ATTRIB_ARB 0x8626
#define GL_PROGRAM_LENGTH_ARB 0x8627
#define GL_PROGRAM_STRING_ARB 0x8628
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB 0x862E
#define GL_MAX_PROGRAM_MATRICES_ARB 0x862F
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB 0x8640
#define GL_CURRENT_MATRIX_ARB 0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB 0x8645
#define GL_PROGRAM_ERROR_POSITION_ARB 0x864B
#define GL_PROGRAM_BINDING_ARB 0x8677
#define GL_MAX_VERTEX_ATTRIBS_ARB 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB 0x886A
#define GL_PROGRAM_ERROR_STRING_ARB 0x8874
#define GL_PROGRAM_FORMAT_ASCII_ARB 0x8875
#define GL_PROGRAM_FORMAT_ARB 0x8876
#define GL_PROGRAM_INSTRUCTIONS_ARB 0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB 0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB 0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB 0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A7
#define GL_PROGRAM_PARAMETERS_ARB 0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB 0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AB
#define GL_PROGRAM_ATTRIBS_ARB 0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB 0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB 0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB 0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB 0x88B6
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB 0x88B7
#define GL_MATRIX0_ARB 0x88C0
#define GL_MATRIX1_ARB 0x88C1
#define GL_MATRIX2_ARB 0x88C2
#define GL_MATRIX3_ARB 0x88C3
#define GL_MATRIX4_ARB 0x88C4
#define GL_MATRIX5_ARB 0x88C5
#define GL_MATRIX6_ARB 0x88C6
#define GL_MATRIX7_ARB 0x88C7
#define GL_MATRIX8_ARB 0x88C8
#define GL_MATRIX9_ARB 0x88C9
#define GL_MATRIX10_ARB 0x88CA
#define GL_MATRIX11_ARB 0x88CB
#define GL_MATRIX12_ARB 0x88CC
#define GL_MATRIX13_ARB 0x88CD
#define GL_MATRIX14_ARB 0x88CE
#define GL_MATRIX15_ARB 0x88CF
#define GL_MATRIX16_ARB 0x88D0
#define GL_MATRIX17_ARB 0x88D1
#define GL_MATRIX18_ARB 0x88D2
#define GL_MATRIX19_ARB 0x88D3
#define GL_MATRIX20_ARB 0x88D4
#define GL_MATRIX21_ARB 0x88D5
#define GL_MATRIX22_ARB 0x88D6
#define GL_MATRIX23_ARB 0x88D7
#define GL_MATRIX24_ARB 0x88D8
#define GL_MATRIX25_ARB 0x88D9
#define GL_MATRIX26_ARB 0x88DA
#define GL_MATRIX27_ARB 0x88DB
#define GL_MATRIX28_ARB 0x88DC
#define GL_MATRIX29_ARB 0x88DD
#define GL_MATRIX30_ARB 0x88DE
#define GL_MATRIX31_ARB 0x88DF

typedef void (GLAPIENTRY * PFNGLBINDPROGRAMARBPROC) (GLenum target, GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMSARBPROC) (GLsizei n, const GLuint* programs);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMSARBPROC) (GLsizei n, GLuint* programs);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTRINGARBPROC) (GLenum target, GLenum pname, void* string);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVARBPROC) (GLuint index, GLenum pname, GLvoid** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVARBPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVARBPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVARBPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMARBPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMSTRINGARBPROC) (GLenum target, GLenum format, GLsizei len, const void* string);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DARBPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FARBPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SARBPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DARBPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FARBPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SARBPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVARBPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVARBPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBARBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVARBPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVARBPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVARBPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVARBPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVARBPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVARBPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVARBPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVARBPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);


#define GLEW_ARB_vertex_program GLEW_GET_VAR(__GLEW_ARB_vertex_program)

#endif /* GL_ARB_vertex_program */

/* -------------------------- GL_ARB_vertex_shader ------------------------- */

#ifndef GL_ARB_vertex_shader
#define GL_ARB_vertex_shader 1

#define GL_VERTEX_SHADER_ARB 0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB 0x8B4A
#define GL_MAX_VARYING_FLOATS_ARB 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB 0x8B4D
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB 0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB 0x8B8A

typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONARBPROC) (GLhandleARB programObj, GLuint index, const GLcharARB* name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);


#define GLEW_ARB_vertex_shader GLEW_GET_VAR(__GLEW_ARB_vertex_shader)

#endif /* GL_ARB_vertex_shader */

/* ------------------- GL_ARB_vertex_type_2_10_10_10_rev ------------------- */

#ifndef GL_ARB_vertex_type_2_10_10_10_rev
#define GL_ARB_vertex_type_2_10_10_10_rev 1

#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_INT_2_10_10_10_REV 0x8D9F

typedef void (GLAPIENTRY * PFNGLCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLCOLORP4UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLCOLORP4UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP1UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP1UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP2UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP2UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP3UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP3UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP4UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP4UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLNORMALP3UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLNORMALP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP1UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP1UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP2UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP2UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP3UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP4UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP4UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP1UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP1UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP2UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP2UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP3UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP3UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP4UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP4UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP2UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP2UIVPROC) (GLenum type, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP3UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP3UIVPROC) (GLenum type, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP4UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP4UIVPROC) (GLenum type, const GLuint* value);


#define GLEW_ARB_vertex_type_2_10_10_10_rev GLEW_GET_VAR(__GLEW_ARB_vertex_type_2_10_10_10_rev)

#endif /* GL_ARB_vertex_type_2_10_10_10_rev */

/* ------------------------- GL_ARB_viewport_array ------------------------- */

#ifndef GL_ARB_viewport_array
#define GL_ARB_viewport_array 1

#define GL_DEPTH_RANGE 0x0B70
#define GL_VIEWPORT 0x0BA2
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_MAX_VIEWPORTS 0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS 0x825C
#define GL_VIEWPORT_BOUNDS_RANGE 0x825D
#define GL_LAYER_PROVOKING_VERTEX 0x825E
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX 0x825F
#define GL_UNDEFINED_VERTEX 0x8260
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F

typedef void (GLAPIENTRY * PFNGLDEPTHRANGEARRAYVPROC) (GLuint first, GLsizei count, const GLclampd * v);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEINDEXEDPROC) (GLuint index, GLclampd n, GLclampd f);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEI_VPROC) (GLenum target, GLuint index, GLdouble* data);
typedef void (GLAPIENTRY * PFNGLGETFLOATI_VPROC) (GLenum target, GLuint index, GLfloat* data);
typedef void (GLAPIENTRY * PFNGLSCISSORARRAYVPROC) (GLuint first, GLsizei count, const GLint * v);
typedef void (GLAPIENTRY * PFNGLSCISSORINDEXEDPROC) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLSCISSORINDEXEDVPROC) (GLuint index, const GLint * v);
typedef void (GLAPIENTRY * PFNGLVIEWPORTARRAYVPROC) (GLuint first, GLsizei count, const GLfloat * v);
typedef void (GLAPIENTRY * PFNGLVIEWPORTINDEXEDFPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void (GLAPIENTRY * PFNGLVIEWPORTINDEXEDFVPROC) (GLuint index, const GLfloat * v);


#define GLEW_ARB_viewport_array GLEW_GET_VAR(__GLEW_ARB_viewport_array)

#endif /* GL_ARB_viewport_array */

/* --------------------------- GL_ARB_window_pos --------------------------- */

#ifndef GL_ARB_window_pos
#define GL_ARB_window_pos 1

typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DARBPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVARBPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FARBPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVARBPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IARBPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVARBPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SARBPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVARBPROC) (const GLshort* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DARBPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVARBPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FARBPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVARBPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IARBPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVARBPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SARBPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVARBPROC) (const GLshort* p);


#define GLEW_ARB_window_pos GLEW_GET_VAR(__GLEW_ARB_window_pos)

#endif /* GL_ARB_window_pos */

/* ------------------------- GL_ATIX_point_sprites ------------------------- */

#ifndef GL_ATIX_point_sprites
#define GL_ATIX_point_sprites 1

#define GL_TEXTURE_POINT_MODE_ATIX 0x60B0
#define GL_TEXTURE_POINT_ONE_COORD_ATIX 0x60B1
#define GL_TEXTURE_POINT_SPRITE_ATIX 0x60B2
#define GL_POINT_SPRITE_CULL_MODE_ATIX 0x60B3
#define GL_POINT_SPRITE_CULL_CENTER_ATIX 0x60B4
#define GL_POINT_SPRITE_CULL_CLIP_ATIX 0x60B5

#define GLEW_ATIX_point_sprites GLEW_GET_VAR(__GLEW_ATIX_point_sprites)

#endif /* GL_ATIX_point_sprites */

/* ---------------------- GL_ATIX_texture_env_combine3 --------------------- */

#ifndef GL_ATIX_texture_env_combine3
#define GL_ATIX_texture_env_combine3 1

#define GL_MODULATE_ADD_ATIX 0x8744
#define GL_MODULATE_SIGNED_ADD_ATIX 0x8745
#define GL_MODULATE_SUBTRACT_ATIX 0x8746

#define GLEW_ATIX_texture_env_combine3 GLEW_GET_VAR(__GLEW_ATIX_texture_env_combine3)

#endif /* GL_ATIX_texture_env_combine3 */

/* ----------------------- GL_ATIX_texture_env_route ----------------------- */

#ifndef GL_ATIX_texture_env_route
#define GL_ATIX_texture_env_route 1

#define GL_SECONDARY_COLOR_ATIX 0x8747
#define GL_TEXTURE_OUTPUT_RGB_ATIX 0x8748
#define GL_TEXTURE_OUTPUT_ALPHA_ATIX 0x8749

#define GLEW_ATIX_texture_env_route GLEW_GET_VAR(__GLEW_ATIX_texture_env_route)

#endif /* GL_ATIX_texture_env_route */

/* ---------------- GL_ATIX_vertex_shader_output_point_size ---------------- */

#ifndef GL_ATIX_vertex_shader_output_point_size
#define GL_ATIX_vertex_shader_output_point_size 1

#define GL_OUTPUT_POINT_SIZE_ATIX 0x610E

#define GLEW_ATIX_vertex_shader_output_point_size GLEW_GET_VAR(__GLEW_ATIX_vertex_shader_output_point_size)

#endif /* GL_ATIX_vertex_shader_output_point_size */

/* -------------------------- GL_ATI_draw_buffers -------------------------- */

#ifndef GL_ATI_draw_buffers
#define GL_ATI_draw_buffers 1

#define GL_MAX_DRAW_BUFFERS_ATI 0x8824
#define GL_DRAW_BUFFER0_ATI 0x8825
#define GL_DRAW_BUFFER1_ATI 0x8826
#define GL_DRAW_BUFFER2_ATI 0x8827
#define GL_DRAW_BUFFER3_ATI 0x8828
#define GL_DRAW_BUFFER4_ATI 0x8829
#define GL_DRAW_BUFFER5_ATI 0x882A
#define GL_DRAW_BUFFER6_ATI 0x882B
#define GL_DRAW_BUFFER7_ATI 0x882C
#define GL_DRAW_BUFFER8_ATI 0x882D
#define GL_DRAW_BUFFER9_ATI 0x882E
#define GL_DRAW_BUFFER10_ATI 0x882F
#define GL_DRAW_BUFFER11_ATI 0x8830
#define GL_DRAW_BUFFER12_ATI 0x8831
#define GL_DRAW_BUFFER13_ATI 0x8832
#define GL_DRAW_BUFFER14_ATI 0x8833
#define GL_DRAW_BUFFER15_ATI 0x8834

typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSATIPROC) (GLsizei n, const GLenum* bufs);


#define GLEW_ATI_draw_buffers GLEW_GET_VAR(__GLEW_ATI_draw_buffers)

#endif /* GL_ATI_draw_buffers */

/* -------------------------- GL_ATI_element_array ------------------------- */

#ifndef GL_ATI_element_array
#define GL_ATI_element_array 1

#define GL_ELEMENT_ARRAY_ATI 0x8768
#define GL_ELEMENT_ARRAY_TYPE_ATI 0x8769
#define GL_ELEMENT_ARRAY_POINTER_ATI 0x876A

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTARRAYATIPROC) (GLenum mode, GLsizei count);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTARRAYATIPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count);
typedef void (GLAPIENTRY * PFNGLELEMENTPOINTERATIPROC) (GLenum type, const void* pointer);


#define GLEW_ATI_element_array GLEW_GET_VAR(__GLEW_ATI_element_array)

#endif /* GL_ATI_element_array */

/* ------------------------- GL_ATI_envmap_bumpmap ------------------------- */

#ifndef GL_ATI_envmap_bumpmap
#define GL_ATI_envmap_bumpmap 1

#define GL_BUMP_ROT_MATRIX_ATI 0x8775
#define GL_BUMP_ROT_MATRIX_SIZE_ATI 0x8776
#define GL_BUMP_NUM_TEX_UNITS_ATI 0x8777
#define GL_BUMP_TEX_UNITS_ATI 0x8778
#define GL_DUDV_ATI 0x8779
#define GL_DU8DV8_ATI 0x877A
#define GL_BUMP_ENVMAP_ATI 0x877B
#define GL_BUMP_TARGET_ATI 0x877C

typedef void (GLAPIENTRY * PFNGLGETTEXBUMPPARAMETERFVATIPROC) (GLenum pname, GLfloat *param);
typedef void (GLAPIENTRY * PFNGLGETTEXBUMPPARAMETERIVATIPROC) (GLenum pname, GLint *param);
typedef void (GLAPIENTRY * PFNGLTEXBUMPPARAMETERFVATIPROC) (GLenum pname, GLfloat *param);
typedef void (GLAPIENTRY * PFNGLTEXBUMPPARAMETERIVATIPROC) (GLenum pname, GLint *param);


#define GLEW_ATI_envmap_bumpmap GLEW_GET_VAR(__GLEW_ATI_envmap_bumpmap)

#endif /* GL_ATI_envmap_bumpmap */

/* ------------------------- GL_ATI_fragment_shader ------------------------ */

#ifndef GL_ATI_fragment_shader
#define GL_ATI_fragment_shader 1

#define GL_RED_BIT_ATI 0x00000001
#define GL_2X_BIT_ATI 0x00000001
#define GL_4X_BIT_ATI 0x00000002
#define GL_GREEN_BIT_ATI 0x00000002
#define GL_COMP_BIT_ATI 0x00000002
#define GL_BLUE_BIT_ATI 0x00000004
#define GL_8X_BIT_ATI 0x00000004
#define GL_NEGATE_BIT_ATI 0x00000004
#define GL_BIAS_BIT_ATI 0x00000008
#define GL_HALF_BIT_ATI 0x00000008
#define GL_QUARTER_BIT_ATI 0x00000010
#define GL_EIGHTH_BIT_ATI 0x00000020
#define GL_SATURATE_BIT_ATI 0x00000040
#define GL_FRAGMENT_SHADER_ATI 0x8920
#define GL_REG_0_ATI 0x8921
#define GL_REG_1_ATI 0x8922
#define GL_REG_2_ATI 0x8923
#define GL_REG_3_ATI 0x8924
#define GL_REG_4_ATI 0x8925
#define GL_REG_5_ATI 0x8926
#define GL_CON_0_ATI 0x8941
#define GL_CON_1_ATI 0x8942
#define GL_CON_2_ATI 0x8943
#define GL_CON_3_ATI 0x8944
#define GL_CON_4_ATI 0x8945
#define GL_CON_5_ATI 0x8946
#define GL_CON_6_ATI 0x8947
#define GL_CON_7_ATI 0x8948
#define GL_MOV_ATI 0x8961
#define GL_ADD_ATI 0x8963
#define GL_MUL_ATI 0x8964
#define GL_SUB_ATI 0x8965
#define GL_DOT3_ATI 0x8966
#define GL_DOT4_ATI 0x8967
#define GL_MAD_ATI 0x8968
#define GL_LERP_ATI 0x8969
#define GL_CND_ATI 0x896A
#define GL_CND0_ATI 0x896B
#define GL_DOT2_ADD_ATI 0x896C
#define GL_SECONDARY_INTERPOLATOR_ATI 0x896D
#define GL_NUM_FRAGMENT_REGISTERS_ATI 0x896E
#define GL_NUM_FRAGMENT_CONSTANTS_ATI 0x896F
#define GL_NUM_PASSES_ATI 0x8970
#define GL_NUM_INSTRUCTIONS_PER_PASS_ATI 0x8971
#define GL_NUM_INSTRUCTIONS_TOTAL_ATI 0x8972
#define GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI 0x8973
#define GL_NUM_LOOPBACK_COMPONENTS_ATI 0x8974
#define GL_COLOR_ALPHA_PAIRING_ATI 0x8975
#define GL_SWIZZLE_STR_ATI 0x8976
#define GL_SWIZZLE_STQ_ATI 0x8977
#define GL_SWIZZLE_STR_DR_ATI 0x8978
#define GL_SWIZZLE_STQ_DQ_ATI 0x8979
#define GL_SWIZZLE_STRQ_ATI 0x897A
#define GL_SWIZZLE_STRQ_DQ_ATI 0x897B

typedef void (GLAPIENTRY * PFNGLALPHAFRAGMENTOP1ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (GLAPIENTRY * PFNGLALPHAFRAGMENTOP2ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (GLAPIENTRY * PFNGLALPHAFRAGMENTOP3ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (GLAPIENTRY * PFNGLBEGINFRAGMENTSHADERATIPROC) (void);
typedef void (GLAPIENTRY * PFNGLBINDFRAGMENTSHADERATIPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLCOLORFRAGMENTOP1ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (GLAPIENTRY * PFNGLCOLORFRAGMENTOP2ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (GLAPIENTRY * PFNGLCOLORFRAGMENTOP3ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (GLAPIENTRY * PFNGLDELETEFRAGMENTSHADERATIPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENDFRAGMENTSHADERATIPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLGENFRAGMENTSHADERSATIPROC) (GLuint range);
typedef void (GLAPIENTRY * PFNGLPASSTEXCOORDATIPROC) (GLuint dst, GLuint coord, GLenum swizzle);
typedef void (GLAPIENTRY * PFNGLSAMPLEMAPATIPROC) (GLuint dst, GLuint interp, GLenum swizzle);
typedef void (GLAPIENTRY * PFNGLSETFRAGMENTSHADERCONSTANTATIPROC) (GLuint dst, const GLfloat* value);


#define GLEW_ATI_fragment_shader GLEW_GET_VAR(__GLEW_ATI_fragment_shader)

#endif /* GL_ATI_fragment_shader */

/* ------------------------ GL_ATI_map_object_buffer ----------------------- */

#ifndef GL_ATI_map_object_buffer
#define GL_ATI_map_object_buffer 1

typedef GLvoid * (GLAPIENTRY * PFNGLMAPOBJECTBUFFERATIPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLUNMAPOBJECTBUFFERATIPROC) (GLuint buffer);


#define GLEW_ATI_map_object_buffer GLEW_GET_VAR(__GLEW_ATI_map_object_buffer)

#endif /* GL_ATI_map_object_buffer */

/* ----------------------------- GL_ATI_meminfo ---------------------------- */

#ifndef GL_ATI_meminfo
#define GL_ATI_meminfo 1

#define GL_VBO_FREE_MEMORY_ATI 0x87FB
#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
#define GL_RENDERBUFFER_FREE_MEMORY_ATI 0x87FD

#define GLEW_ATI_meminfo GLEW_GET_VAR(__GLEW_ATI_meminfo)

#endif /* GL_ATI_meminfo */

/* -------------------------- GL_ATI_pn_triangles -------------------------- */

#ifndef GL_ATI_pn_triangles
#define GL_ATI_pn_triangles 1

#define GL_PN_TRIANGLES_ATI 0x87F0
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI 0x87F1
#define GL_PN_TRIANGLES_POINT_MODE_ATI 0x87F2
#define GL_PN_TRIANGLES_NORMAL_MODE_ATI 0x87F3
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI 0x87F4
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI 0x87F5
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI 0x87F6
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI 0x87F7
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI 0x87F8

typedef void (GLAPIENTRY * PFNGLPNTRIANGLESFATIPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPNTRIANGLESIATIPROC) (GLenum pname, GLint param);


#define GLEW_ATI_pn_triangles GLEW_GET_VAR(__GLEW_ATI_pn_triangles)

#endif /* GL_ATI_pn_triangles */

/* ------------------------ GL_ATI_separate_stencil ------------------------ */

#ifndef GL_ATI_separate_stencil
#define GL_ATI_separate_stencil 1

#define GL_STENCIL_BACK_FUNC_ATI 0x8800
#define GL_STENCIL_BACK_FAIL_ATI 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI 0x8803

typedef void (GLAPIENTRY * PFNGLSTENCILFUNCSEPARATEATIPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILOPSEPARATEATIPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);


#define GLEW_ATI_separate_stencil GLEW_GET_VAR(__GLEW_ATI_separate_stencil)

#endif /* GL_ATI_separate_stencil */

/* ----------------------- GL_ATI_shader_texture_lod ----------------------- */

#ifndef GL_ATI_shader_texture_lod
#define GL_ATI_shader_texture_lod 1

#define GLEW_ATI_shader_texture_lod GLEW_GET_VAR(__GLEW_ATI_shader_texture_lod)

#endif /* GL_ATI_shader_texture_lod */

/* ---------------------- GL_ATI_text_fragment_shader ---------------------- */

#ifndef GL_ATI_text_fragment_shader
#define GL_ATI_text_fragment_shader 1

#define GL_TEXT_FRAGMENT_SHADER_ATI 0x8200

#define GLEW_ATI_text_fragment_shader GLEW_GET_VAR(__GLEW_ATI_text_fragment_shader)

#endif /* GL_ATI_text_fragment_shader */

/* --------------------- GL_ATI_texture_compression_3dc -------------------- */

#ifndef GL_ATI_texture_compression_3dc
#define GL_ATI_texture_compression_3dc 1

#define GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI 0x8837

#define GLEW_ATI_texture_compression_3dc GLEW_GET_VAR(__GLEW_ATI_texture_compression_3dc)

#endif /* GL_ATI_texture_compression_3dc */

/* ---------------------- GL_ATI_texture_env_combine3 ---------------------- */

#ifndef GL_ATI_texture_env_combine3
#define GL_ATI_texture_env_combine3 1

#define GL_MODULATE_ADD_ATI 0x8744
#define GL_MODULATE_SIGNED_ADD_ATI 0x8745
#define GL_MODULATE_SUBTRACT_ATI 0x8746

#define GLEW_ATI_texture_env_combine3 GLEW_GET_VAR(__GLEW_ATI_texture_env_combine3)

#endif /* GL_ATI_texture_env_combine3 */

/* -------------------------- GL_ATI_texture_float ------------------------- */

#ifndef GL_ATI_texture_float
#define GL_ATI_texture_float 1

#define GL_RGBA_FLOAT32_ATI 0x8814
#define GL_RGB_FLOAT32_ATI 0x8815
#define GL_ALPHA_FLOAT32_ATI 0x8816
#define GL_INTENSITY_FLOAT32_ATI 0x8817
#define GL_LUMINANCE_FLOAT32_ATI 0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI 0x8819
#define GL_RGBA_FLOAT16_ATI 0x881A
#define GL_RGB_FLOAT16_ATI 0x881B
#define GL_ALPHA_FLOAT16_ATI 0x881C
#define GL_INTENSITY_FLOAT16_ATI 0x881D
#define GL_LUMINANCE_FLOAT16_ATI 0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI 0x881F

#define GLEW_ATI_texture_float GLEW_GET_VAR(__GLEW_ATI_texture_float)

#endif /* GL_ATI_texture_float */

/* ----------------------- GL_ATI_texture_mirror_once ---------------------- */

#ifndef GL_ATI_texture_mirror_once
#define GL_ATI_texture_mirror_once 1

#define GL_MIRROR_CLAMP_ATI 0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_ATI 0x8743

#define GLEW_ATI_texture_mirror_once GLEW_GET_VAR(__GLEW_ATI_texture_mirror_once)

#endif /* GL_ATI_texture_mirror_once */

/* ----------------------- GL_ATI_vertex_array_object ---------------------- */

#ifndef GL_ATI_vertex_array_object
#define GL_ATI_vertex_array_object 1

#define GL_STATIC_ATI 0x8760
#define GL_DYNAMIC_ATI 0x8761
#define GL_PRESERVE_ATI 0x8762
#define GL_DISCARD_ATI 0x8763
#define GL_OBJECT_BUFFER_SIZE_ATI 0x8764
#define GL_OBJECT_BUFFER_USAGE_ATI 0x8765
#define GL_ARRAY_OBJECT_BUFFER_ATI 0x8766
#define GL_ARRAY_OBJECT_OFFSET_ATI 0x8767

typedef void (GLAPIENTRY * PFNGLARRAYOBJECTATIPROC) (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
typedef void (GLAPIENTRY * PFNGLFREEOBJECTBUFFERATIPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLGETARRAYOBJECTFVATIPROC) (GLenum array, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETARRAYOBJECTIVATIPROC) (GLenum array, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTBUFFERFVATIPROC) (GLuint buffer, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTBUFFERIVATIPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVARIANTARRAYOBJECTFVATIPROC) (GLuint id, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVARIANTARRAYOBJECTIVATIPROC) (GLuint id, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISOBJECTBUFFERATIPROC) (GLuint buffer);
typedef GLuint (GLAPIENTRY * PFNGLNEWOBJECTBUFFERATIPROC) (GLsizei size, const void* pointer, GLenum usage);
typedef void (GLAPIENTRY * PFNGLUPDATEOBJECTBUFFERATIPROC) (GLuint buffer, GLuint offset, GLsizei size, const void* pointer, GLenum preserve);
typedef void (GLAPIENTRY * PFNGLVARIANTARRAYOBJECTATIPROC) (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);


#define GLEW_ATI_vertex_array_object GLEW_GET_VAR(__GLEW_ATI_vertex_array_object)

#endif /* GL_ATI_vertex_array_object */

/* ------------------- GL_ATI_vertex_attrib_array_object ------------------- */

#ifndef GL_ATI_vertex_attrib_array_object
#define GL_ATI_vertex_attrib_array_object 1

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC) (GLuint index, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBARRAYOBJECTATIPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);


#define GLEW_ATI_vertex_attrib_array_object GLEW_GET_VAR(__GLEW_ATI_vertex_attrib_array_object)

#endif /* GL_ATI_vertex_attrib_array_object */

/* ------------------------- GL_ATI_vertex_streams ------------------------- */

#ifndef GL_ATI_vertex_streams
#define GL_ATI_vertex_streams 1

#define GL_MAX_VERTEX_STREAMS_ATI 0x876B
#define GL_VERTEX_SOURCE_ATI 0x876C
#define GL_VERTEX_STREAM0_ATI 0x876D
#define GL_VERTEX_STREAM1_ATI 0x876E
#define GL_VERTEX_STREAM2_ATI 0x876F
#define GL_VERTEX_STREAM3_ATI 0x8770
#define GL_VERTEX_STREAM4_ATI 0x8771
#define GL_VERTEX_STREAM5_ATI 0x8772
#define GL_VERTEX_STREAM6_ATI 0x8773
#define GL_VERTEX_STREAM7_ATI 0x8774

typedef void (GLAPIENTRY * PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC) (GLenum stream);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3BATIPROC) (GLenum stream, GLbyte x, GLbyte y, GLbyte z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3BVATIPROC) (GLenum stream, const GLbyte *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3IATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXBLENDENVFATIPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLVERTEXBLENDENVIATIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1DATIPROC) (GLenum stream, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1FATIPROC) (GLenum stream, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1IATIPROC) (GLenum stream, GLint x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1SATIPROC) (GLenum stream, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2DATIPROC) (GLenum stream, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2FATIPROC) (GLenum stream, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2IATIPROC) (GLenum stream, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2SATIPROC) (GLenum stream, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3IATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4IATIPROC) (GLenum stream, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4SVATIPROC) (GLenum stream, const GLshort *coords);


#define GLEW_ATI_vertex_streams GLEW_GET_VAR(__GLEW_ATI_vertex_streams)

#endif /* GL_ATI_vertex_streams */

/* --------------------------- GL_EXT_422_pixels --------------------------- */

#ifndef GL_EXT_422_pixels
#define GL_EXT_422_pixels 1

#define GL_422_EXT 0x80CC
#define GL_422_REV_EXT 0x80CD
#define GL_422_AVERAGE_EXT 0x80CE
#define GL_422_REV_AVERAGE_EXT 0x80CF

#define GLEW_EXT_422_pixels GLEW_GET_VAR(__GLEW_EXT_422_pixels)

#endif /* GL_EXT_422_pixels */

/* ---------------------------- GL_EXT_Cg_shader --------------------------- */

#ifndef GL_EXT_Cg_shader
#define GL_EXT_Cg_shader 1

#define GL_CG_VERTEX_SHADER_EXT 0x890E
#define GL_CG_FRAGMENT_SHADER_EXT 0x890F

#define GLEW_EXT_Cg_shader GLEW_GET_VAR(__GLEW_EXT_Cg_shader)

#endif /* GL_EXT_Cg_shader */

/* ------------------------------ GL_EXT_abgr ------------------------------ */

#ifndef GL_EXT_abgr
#define GL_EXT_abgr 1

#define GL_ABGR_EXT 0x8000

#define GLEW_EXT_abgr GLEW_GET_VAR(__GLEW_EXT_abgr)

#endif /* GL_EXT_abgr */

/* ------------------------------ GL_EXT_bgra ------------------------------ */

#ifndef GL_EXT_bgra
#define GL_EXT_bgra 1

#define GL_BGR_EXT 0x80E0
#define GL_BGRA_EXT 0x80E1

#define GLEW_EXT_bgra GLEW_GET_VAR(__GLEW_EXT_bgra)

#endif /* GL_EXT_bgra */

/* ------------------------ GL_EXT_bindable_uniform ------------------------ */

#ifndef GL_EXT_bindable_uniform
#define GL_EXT_bindable_uniform 1

#define GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT 0x8DE2
#define GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT 0x8DE3
#define GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT 0x8DE4
#define GL_MAX_BINDABLE_UNIFORM_SIZE_EXT 0x8DED
#define GL_UNIFORM_BUFFER_EXT 0x8DEE
#define GL_UNIFORM_BUFFER_BINDING_EXT 0x8DEF

typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMBUFFERSIZEEXTPROC) (GLuint program, GLint location);
typedef GLintptr (GLAPIENTRY * PFNGLGETUNIFORMOFFSETEXTPROC) (GLuint program, GLint location);
typedef void (GLAPIENTRY * PFNGLUNIFORMBUFFEREXTPROC) (GLuint program, GLint location, GLuint buffer);


#define GLEW_EXT_bindable_uniform GLEW_GET_VAR(__GLEW_EXT_bindable_uniform)

#endif /* GL_EXT_bindable_uniform */

/* --------------------------- GL_EXT_blend_color -------------------------- */

#ifndef GL_EXT_blend_color
#define GL_EXT_blend_color 1

#define GL_CONSTANT_COLOR_EXT 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT 0x8002
#define GL_CONSTANT_ALPHA_EXT 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT 0x8004
#define GL_BLEND_COLOR_EXT 0x8005

typedef void (GLAPIENTRY * PFNGLBLENDCOLOREXTPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);


#define GLEW_EXT_blend_color GLEW_GET_VAR(__GLEW_EXT_blend_color)

#endif /* GL_EXT_blend_color */

/* --------------------- GL_EXT_blend_equation_separate -------------------- */

#ifndef GL_EXT_blend_equation_separate
#define GL_EXT_blend_equation_separate 1

#define GL_BLEND_EQUATION_RGB_EXT 0x8009
#define GL_BLEND_EQUATION_ALPHA_EXT 0x883D

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEEXTPROC) (GLenum modeRGB, GLenum modeAlpha);


#define GLEW_EXT_blend_equation_separate GLEW_GET_VAR(__GLEW_EXT_blend_equation_separate)

#endif /* GL_EXT_blend_equation_separate */

/* ----------------------- GL_EXT_blend_func_separate ---------------------- */

#ifndef GL_EXT_blend_func_separate
#define GL_EXT_blend_func_separate 1

#define GL_BLEND_DST_RGB_EXT 0x80C8
#define GL_BLEND_SRC_RGB_EXT 0x80C9
#define GL_BLEND_DST_ALPHA_EXT 0x80CA
#define GL_BLEND_SRC_ALPHA_EXT 0x80CB

typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEEXTPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);


#define GLEW_EXT_blend_func_separate GLEW_GET_VAR(__GLEW_EXT_blend_func_separate)

#endif /* GL_EXT_blend_func_separate */

/* ------------------------- GL_EXT_blend_logic_op ------------------------- */

#ifndef GL_EXT_blend_logic_op
#define GL_EXT_blend_logic_op 1

#define GLEW_EXT_blend_logic_op GLEW_GET_VAR(__GLEW_EXT_blend_logic_op)

#endif /* GL_EXT_blend_logic_op */

/* -------------------------- GL_EXT_blend_minmax -------------------------- */

#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1

#define GL_FUNC_ADD_EXT 0x8006
#define GL_MIN_EXT 0x8007
#define GL_MAX_EXT 0x8008
#define GL_BLEND_EQUATION_EXT 0x8009

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONEXTPROC) (GLenum mode);


#define GLEW_EXT_blend_minmax GLEW_GET_VAR(__GLEW_EXT_blend_minmax)

#endif /* GL_EXT_blend_minmax */

/* ------------------------- GL_EXT_blend_subtract ------------------------- */

#ifndef GL_EXT_blend_subtract
#define GL_EXT_blend_subtract 1

#define GL_FUNC_SUBTRACT_EXT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT_EXT 0x800B

#define GLEW_EXT_blend_subtract GLEW_GET_VAR(__GLEW_EXT_blend_subtract)

#endif /* GL_EXT_blend_subtract */

/* ------------------------ GL_EXT_clip_volume_hint ------------------------ */

#ifndef GL_EXT_clip_volume_hint
#define GL_EXT_clip_volume_hint 1

#define GL_CLIP_VOLUME_CLIPPING_HINT_EXT 0x80F0

#define GLEW_EXT_clip_volume_hint GLEW_GET_VAR(__GLEW_EXT_clip_volume_hint)

#endif /* GL_EXT_clip_volume_hint */

/* ------------------------------ GL_EXT_cmyka ----------------------------- */

#ifndef GL_EXT_cmyka
#define GL_EXT_cmyka 1

#define GL_CMYK_EXT 0x800C
#define GL_CMYKA_EXT 0x800D
#define GL_PACK_CMYK_HINT_EXT 0x800E
#define GL_UNPACK_CMYK_HINT_EXT 0x800F

#define GLEW_EXT_cmyka GLEW_GET_VAR(__GLEW_EXT_cmyka)

#endif /* GL_EXT_cmyka */

/* ------------------------- GL_EXT_color_subtable ------------------------- */

#ifndef GL_EXT_color_subtable
#define GL_EXT_color_subtable 1

typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void* data);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);


#define GLEW_EXT_color_subtable GLEW_GET_VAR(__GLEW_EXT_color_subtable)

#endif /* GL_EXT_color_subtable */

/* ---------------------- GL_EXT_compiled_vertex_array --------------------- */

#ifndef GL_EXT_compiled_vertex_array
#define GL_EXT_compiled_vertex_array 1

#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT 0x81A8
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT 0x81A9

typedef void (GLAPIENTRY * PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLUNLOCKARRAYSEXTPROC) (void);


#define GLEW_EXT_compiled_vertex_array GLEW_GET_VAR(__GLEW_EXT_compiled_vertex_array)

#endif /* GL_EXT_compiled_vertex_array */

/* --------------------------- GL_EXT_convolution -------------------------- */

#ifndef GL_EXT_convolution
#define GL_EXT_convolution 1

#define GL_CONVOLUTION_1D_EXT 0x8010
#define GL_CONVOLUTION_2D_EXT 0x8011
#define GL_SEPARABLE_2D_EXT 0x8012
#define GL_CONVOLUTION_BORDER_MODE_EXT 0x8013
#define GL_CONVOLUTION_FILTER_SCALE_EXT 0x8014
#define GL_CONVOLUTION_FILTER_BIAS_EXT 0x8015
#define GL_REDUCE_EXT 0x8016
#define GL_CONVOLUTION_FORMAT_EXT 0x8017
#define GL_CONVOLUTION_WIDTH_EXT 0x8018
#define GL_CONVOLUTION_HEIGHT_EXT 0x8019
#define GL_MAX_CONVOLUTION_WIDTH_EXT 0x801A
#define GL_MAX_CONVOLUTION_HEIGHT_EXT 0x801B
#define GL_POST_CONVOLUTION_RED_SCALE_EXT 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS_EXT 0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT 0x8023

typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFEXTPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIEXTPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void* image);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSEPARABLEFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void* row, void* column, void* span);
typedef void (GLAPIENTRY * PFNGLSEPARABLEFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* row, const void* column);


#define GLEW_EXT_convolution GLEW_GET_VAR(__GLEW_EXT_convolution)

#endif /* GL_EXT_convolution */

/* ------------------------ GL_EXT_coordinate_frame ------------------------ */

#ifndef GL_EXT_coordinate_frame
#define GL_EXT_coordinate_frame 1

#define GL_TANGENT_ARRAY_EXT 0x8439
#define GL_BINORMAL_ARRAY_EXT 0x843A
#define GL_CURRENT_TANGENT_EXT 0x843B
#define GL_CURRENT_BINORMAL_EXT 0x843C
#define GL_TANGENT_ARRAY_TYPE_EXT 0x843E
#define GL_TANGENT_ARRAY_STRIDE_EXT 0x843F
#define GL_BINORMAL_ARRAY_TYPE_EXT 0x8440
#define GL_BINORMAL_ARRAY_STRIDE_EXT 0x8441
#define GL_TANGENT_ARRAY_POINTER_EXT 0x8442
#define GL_BINORMAL_ARRAY_POINTER_EXT 0x8443
#define GL_MAP1_TANGENT_EXT 0x8444
#define GL_MAP2_TANGENT_EXT 0x8445
#define GL_MAP1_BINORMAL_EXT 0x8446
#define GL_MAP2_BINORMAL_EXT 0x8447

typedef void (GLAPIENTRY * PFNGLBINORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, void* pointer);
typedef void (GLAPIENTRY * PFNGLTANGENTPOINTEREXTPROC) (GLenum type, GLsizei stride, void* pointer);


#define GLEW_EXT_coordinate_frame GLEW_GET_VAR(__GLEW_EXT_coordinate_frame)

#endif /* GL_EXT_coordinate_frame */

/* -------------------------- GL_EXT_copy_texture -------------------------- */

#ifndef GL_EXT_copy_texture
#define GL_EXT_copy_texture 1

typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE1DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE2DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);


#define GLEW_EXT_copy_texture GLEW_GET_VAR(__GLEW_EXT_copy_texture)

#endif /* GL_EXT_copy_texture */

/* --------------------------- GL_EXT_cull_vertex -------------------------- */

#ifndef GL_EXT_cull_vertex
#define GL_EXT_cull_vertex 1

#define GL_CULL_VERTEX_EXT 0x81AA
#define GL_CULL_VERTEX_EYE_POSITION_EXT 0x81AB
#define GL_CULL_VERTEX_OBJECT_POSITION_EXT 0x81AC

typedef void (GLAPIENTRY * PFNGLCULLPARAMETERDVEXTPROC) (GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLCULLPARAMETERFVEXTPROC) (GLenum pname, GLfloat* params);


#define GLEW_EXT_cull_vertex GLEW_GET_VAR(__GLEW_EXT_cull_vertex)

#endif /* GL_EXT_cull_vertex */

/* -------------------------- GL_EXT_debug_marker -------------------------- */

#ifndef GL_EXT_debug_marker
#define GL_EXT_debug_marker 1

typedef void (GLAPIENTRY * PFNGLINSERTEVENTMARKEREXTPROC) (GLsizei length, const GLchar* marker);
typedef void (GLAPIENTRY * PFNGLPOPGROUPMARKEREXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLPUSHGROUPMARKEREXTPROC) (GLsizei length, const GLchar* marker);


#define GLEW_EXT_debug_marker GLEW_GET_VAR(__GLEW_EXT_debug_marker)

#endif /* GL_EXT_debug_marker */

/* ------------------------ GL_EXT_depth_bounds_test ----------------------- */

#ifndef GL_EXT_depth_bounds_test
#define GL_EXT_depth_bounds_test 1

#define GL_DEPTH_BOUNDS_TEST_EXT 0x8890
#define GL_DEPTH_BOUNDS_EXT 0x8891

typedef void (GLAPIENTRY * PFNGLDEPTHBOUNDSEXTPROC) (GLclampd zmin, GLclampd zmax);


#define GLEW_EXT_depth_bounds_test GLEW_GET_VAR(__GLEW_EXT_depth_bounds_test)

#endif /* GL_EXT_depth_bounds_test */

/* ----------------------- GL_EXT_direct_state_access ---------------------- */

#ifndef GL_EXT_direct_state_access
#define GL_EXT_direct_state_access 1

#define GL_PROGRAM_MATRIX_EXT 0x8E2D
#define GL_TRANSPOSE_PROGRAM_MATRIX_EXT 0x8E2E
#define GL_PROGRAM_MATRIX_STACK_DEPTH_EXT 0x8E2F

typedef void (GLAPIENTRY * PFNGLBINDMULTITEXTUREEXTPROC) (GLenum texunit, GLenum target, GLuint texture);
typedef GLenum (GLAPIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC) (GLuint framebuffer, GLenum target);
typedef void (GLAPIENTRY * PFNGLCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC) (GLuint framebuffer, GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERREADBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLGENERATEMULTITEXMIPMAPEXTPROC) (GLenum texunit, GLenum target);
typedef void (GLAPIENTRY * PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, void* img);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, void* img);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEINDEXEDVEXTPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEI_VEXTPROC) (GLenum pname, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATINDEXEDVEXTPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATI_VEXTPROC) (GLenum pname, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPOINTERVEXTPROC) (GLuint buffer, GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum pname, void* string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMIVEXTPROC) (GLuint program, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC) (GLuint renderbuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERINDEXEDVEXTPROC) (GLenum target, GLuint index, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERI_VEXTPROC) (GLenum pname, GLuint index, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERVEXTPROC) (GLuint vaobj, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLvoid** param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERVEXTPROC) (GLuint vaobj, GLenum pname, GLvoid** param);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * PFNGLMATRIXFRUSTUMEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADIDENTITYEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXORTHOEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXPOPEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXPUSHEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEDEXTPROC) (GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEFEXTPROC) (GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMULTITEXBUFFEREXTPROC) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDPOINTEREXTPROC) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXRENDERBUFFEREXTPROC) (GLenum texunit, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (GLAPIENTRY * PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum format, GLsizei len, const void* string);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DEXTPROC) (GLuint program, GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FEXTPROC) (GLuint program, GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IEXTPROC) (GLuint program, GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIEXTPROC) (GLuint program, GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLTEXTURERENDERBUFFEREXTPROC) (GLuint texture, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYINDEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYNORMALOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);


#define GLEW_EXT_direct_state_access GLEW_GET_VAR(__GLEW_EXT_direct_state_access)

#endif /* GL_EXT_direct_state_access */

/* -------------------------- GL_EXT_draw_buffers2 ------------------------- */

#ifndef GL_EXT_draw_buffers2
#define GL_EXT_draw_buffers2 1

typedef void (GLAPIENTRY * PFNGLCOLORMASKINDEXEDEXTPROC) (GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAPIENTRY * PFNGLDISABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANINDEXEDVEXTPROC) (GLenum value, GLuint index, GLboolean* data);
typedef void (GLAPIENTRY * PFNGLGETINTEGERINDEXEDVEXTPROC) (GLenum value, GLuint index, GLint* data);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDINDEXEDEXTPROC) (GLenum target, GLuint index);


#define GLEW_EXT_draw_buffers2 GLEW_GET_VAR(__GLEW_EXT_draw_buffers2)

#endif /* GL_EXT_draw_buffers2 */

/* ------------------------- GL_EXT_draw_instanced ------------------------- */

#ifndef GL_EXT_draw_instanced
#define GL_EXT_draw_instanced 1

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDEXTPROC) (GLenum mode, GLint start, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDEXTPROC) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);


#define GLEW_EXT_draw_instanced GLEW_GET_VAR(__GLEW_EXT_draw_instanced)

#endif /* GL_EXT_draw_instanced */

/* ----------------------- GL_EXT_draw_range_elements ---------------------- */

#ifndef GL_EXT_draw_range_elements
#define GL_EXT_draw_range_elements 1

#define GL_MAX_ELEMENTS_VERTICES_EXT 0x80E8
#define GL_MAX_ELEMENTS_INDICES_EXT 0x80E9

typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSEXTPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);


#define GLEW_EXT_draw_range_elements GLEW_GET_VAR(__GLEW_EXT_draw_range_elements)

#endif /* GL_EXT_draw_range_elements */

/* ---------------------------- GL_EXT_fog_coord --------------------------- */

#ifndef GL_EXT_fog_coord
#define GL_EXT_fog_coord 1

#define GL_FOG_COORDINATE_SOURCE_EXT 0x8450
#define GL_FOG_COORDINATE_EXT 0x8451
#define GL_FRAGMENT_DEPTH_EXT 0x8452
#define GL_CURRENT_FOG_COORDINATE_EXT 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT 0x8456
#define GL_FOG_COORDINATE_ARRAY_EXT 0x8457

typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTEREXTPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDEXTPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVEXTPROC) (const GLdouble *coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVEXTPROC) (const GLfloat *coord);


#define GLEW_EXT_fog_coord GLEW_GET_VAR(__GLEW_EXT_fog_coord)

#endif /* GL_EXT_fog_coord */

/* ------------------------ GL_EXT_fragment_lighting ----------------------- */

#ifndef GL_EXT_fragment_lighting
#define GL_EXT_fragment_lighting 1

#define GL_FRAGMENT_LIGHTING_EXT 0x8400
#define GL_FRAGMENT_COLOR_MATERIAL_EXT 0x8401
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_EXT 0x8402
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_EXT 0x8403
#define GL_MAX_FRAGMENT_LIGHTS_EXT 0x8404
#define GL_MAX_ACTIVE_LIGHTS_EXT 0x8405
#define GL_CURRENT_RASTER_NORMAL_EXT 0x8406
#define GL_LIGHT_ENV_MODE_EXT 0x8407
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_EXT 0x8408
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_EXT 0x8409
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_EXT 0x840A
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_EXT 0x840B
#define GL_FRAGMENT_LIGHT0_EXT 0x840C
#define GL_FRAGMENT_LIGHT7_EXT 0x8413

typedef void (GLAPIENTRY * PFNGLFRAGMENTCOLORMATERIALEXTPROC) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFEXTPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFVEXTPROC) (GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIEXTPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIVEXTPROC) (GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFEXTPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIEXTPROC) (GLenum light, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFEXTPROC) (GLenum face, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIEXTPROC) (GLenum face, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLLIGHTENVIEXTPROC) (GLenum pname, GLint param);


#define GLEW_EXT_fragment_lighting GLEW_GET_VAR(__GLEW_EXT_fragment_lighting)

#endif /* GL_EXT_fragment_lighting */

/* ------------------------ GL_EXT_framebuffer_blit ------------------------ */

#ifndef GL_EXT_framebuffer_blit
#define GL_EXT_framebuffer_blit 1

#define GL_DRAW_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_READ_FRAMEBUFFER_EXT 0x8CA8
#define GL_DRAW_FRAMEBUFFER_EXT 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING_EXT 0x8CAA

typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFEREXTPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);


#define GLEW_EXT_framebuffer_blit GLEW_GET_VAR(__GLEW_EXT_framebuffer_blit)

#endif /* GL_EXT_framebuffer_blit */

/* --------------------- GL_EXT_framebuffer_multisample -------------------- */

#ifndef GL_EXT_framebuffer_multisample
#define GL_EXT_framebuffer_multisample 1

#define GL_RENDERBUFFER_SAMPLES_EXT 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#define GL_MAX_SAMPLES_EXT 0x8D57

typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);


#define GLEW_EXT_framebuffer_multisample GLEW_GET_VAR(__GLEW_EXT_framebuffer_multisample)

#endif /* GL_EXT_framebuffer_multisample */

/* --------------- GL_EXT_framebuffer_multisample_blit_scaled -------------- */

#ifndef GL_EXT_framebuffer_multisample_blit_scaled
#define GL_EXT_framebuffer_multisample_blit_scaled 1

#define GL_SCALED_RESOLVE_FASTEST_EXT 0x90BA
#define GL_SCALED_RESOLVE_NICEST_EXT 0x90BB

#define GLEW_EXT_framebuffer_multisample_blit_scaled GLEW_GET_VAR(__GLEW_EXT_framebuffer_multisample_blit_scaled)

#endif /* GL_EXT_framebuffer_multisample_blit_scaled */

/* ----------------------- GL_EXT_framebuffer_object ----------------------- */

#ifndef GL_EXT_framebuffer_object
#define GL_EXT_framebuffer_object 1

#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT 0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT 0x8D00
#define GL_STENCIL_ATTACHMENT_EXT 0x8D20
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_RENDERBUFFER_EXT 0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#define GL_STENCIL_INDEX1_EXT 0x8D46
#define GL_STENCIL_INDEX4_EXT 0x8D47
#define GL_STENCIL_INDEX8_EXT 0x8D48
#define GL_STENCIL_INDEX16_EXT 0x8D49
#define GL_RENDERBUFFER_RED_SIZE_EXT 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_EXT 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_EXT 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT 0x8D55

typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFEREXTPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSEXTPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSEXTPROC) (GLsizei n, GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFEREXTPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFEREXTPROC) (GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);


#define GLEW_EXT_framebuffer_object GLEW_GET_VAR(__GLEW_EXT_framebuffer_object)

#endif /* GL_EXT_framebuffer_object */

/* ------------------------ GL_EXT_framebuffer_sRGB ------------------------ */

#ifndef GL_EXT_framebuffer_sRGB
#define GL_EXT_framebuffer_sRGB 1

#define GL_FRAMEBUFFER_SRGB_EXT 0x8DB9
#define GL_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x8DBA

#define GLEW_EXT_framebuffer_sRGB GLEW_GET_VAR(__GLEW_EXT_framebuffer_sRGB)

#endif /* GL_EXT_framebuffer_sRGB */

/* ------------------------ GL_EXT_geometry_shader4 ------------------------ */

#ifndef GL_EXT_geometry_shader4
#define GL_EXT_geometry_shader4 1

#define GL_LINES_ADJACENCY_EXT 0xA
#define GL_LINE_STRIP_ADJACENCY_EXT 0xB
#define GL_TRIANGLES_ADJACENCY_EXT 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT 0xD
#define GL_PROGRAM_POINT_SIZE_EXT 0x8642
#define GL_MAX_VARYING_COMPONENTS_EXT 0x8B4B
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT 0x8DA9
#define GL_GEOMETRY_SHADER_EXT 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT_EXT 0x8DDA
#define GL_GEOMETRY_INPUT_TYPE_EXT 0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE_EXT 0x8DDC
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT 0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS_EXT 0x8DDE
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIEXTPROC) (GLuint program, GLenum pname, GLint value);


#define GLEW_EXT_geometry_shader4 GLEW_GET_VAR(__GLEW_EXT_geometry_shader4)

#endif /* GL_EXT_geometry_shader4 */

/* --------------------- GL_EXT_gpu_program_parameters --------------------- */

#ifndef GL_EXT_gpu_program_parameters
#define GL_EXT_gpu_program_parameters 1

typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);


#define GLEW_EXT_gpu_program_parameters GLEW_GET_VAR(__GLEW_EXT_gpu_program_parameters)

#endif /* GL_EXT_gpu_program_parameters */

/* --------------------------- GL_EXT_gpu_shader4 -------------------------- */

#ifndef GL_EXT_gpu_shader4
#define GL_EXT_gpu_shader4 1

#define GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT 0x88FD
#define GL_SAMPLER_1D_ARRAY_EXT 0x8DC0
#define GL_SAMPLER_2D_ARRAY_EXT 0x8DC1
#define GL_SAMPLER_BUFFER_EXT 0x8DC2
#define GL_SAMPLER_1D_ARRAY_SHADOW_EXT 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW_EXT 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW_EXT 0x8DC5
#define GL_UNSIGNED_INT_VEC2_EXT 0x8DC6
#define GL_UNSIGNED_INT_VEC3_EXT 0x8DC7
#define GL_UNSIGNED_INT_VEC4_EXT 0x8DC8
#define GL_INT_SAMPLER_1D_EXT 0x8DC9
#define GL_INT_SAMPLER_2D_EXT 0x8DCA
#define GL_INT_SAMPLER_3D_EXT 0x8DCB
#define GL_INT_SAMPLER_CUBE_EXT 0x8DCC
#define GL_INT_SAMPLER_2D_RECT_EXT 0x8DCD
#define GL_INT_SAMPLER_1D_ARRAY_EXT 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY_EXT 0x8DCF
#define GL_INT_SAMPLER_BUFFER_EXT 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_1D_EXT 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D_EXT 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D_EXT 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE_EXT 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT_EXT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY_EXT 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_EXT 0x8DD7
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT 0x8DD8

typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONEXTPROC) (GLuint program, GLuint color, const GLchar *name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONEXTPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVEXTPROC) (GLuint program, GLint location, GLuint *params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVEXTPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVEXTPROC) (GLuint index, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIEXTPROC) (GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIEXTPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IEXTPROC) (GLuint index, GLint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIEXTPROC) (GLuint index, GLuint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IEXTPROC) (GLuint index, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIEXTPROC) (GLuint index, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IEXTPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVEXTPROC) (GLuint index, const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IEXTPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVEXTPROC) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVEXTPROC) (GLuint index, const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVEXTPROC) (GLuint index, const GLushort *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);


#define GLEW_EXT_gpu_shader4 GLEW_GET_VAR(__GLEW_EXT_gpu_shader4)

#endif /* GL_EXT_gpu_shader4 */

/* ---------------------------- GL_EXT_histogram --------------------------- */

#ifndef GL_EXT_histogram
#define GL_EXT_histogram 1

#define GL_HISTOGRAM_EXT 0x8024
#define GL_PROXY_HISTOGRAM_EXT 0x8025
#define GL_HISTOGRAM_WIDTH_EXT 0x8026
#define GL_HISTOGRAM_FORMAT_EXT 0x8027
#define GL_HISTOGRAM_RED_SIZE_EXT 0x8028
#define GL_HISTOGRAM_GREEN_SIZE_EXT 0x8029
#define GL_HISTOGRAM_BLUE_SIZE_EXT 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE_EXT 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT 0x802C
#define GL_HISTOGRAM_SINK_EXT 0x802D
#define GL_MINMAX_EXT 0x802E
#define GL_MINMAX_FORMAT_EXT 0x802F
#define GL_MINMAX_SINK_EXT 0x8030

typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLHISTOGRAMEXTPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLMINMAXEXTPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLRESETHISTOGRAMEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLRESETMINMAXEXTPROC) (GLenum target);


#define GLEW_EXT_histogram GLEW_GET_VAR(__GLEW_EXT_histogram)

#endif /* GL_EXT_histogram */

/* ----------------------- GL_EXT_index_array_formats ---------------------- */

#ifndef GL_EXT_index_array_formats
#define GL_EXT_index_array_formats 1

#define GLEW_EXT_index_array_formats GLEW_GET_VAR(__GLEW_EXT_index_array_formats)

#endif /* GL_EXT_index_array_formats */

/* --------------------------- GL_EXT_index_func --------------------------- */

#ifndef GL_EXT_index_func
#define GL_EXT_index_func 1

typedef void (GLAPIENTRY * PFNGLINDEXFUNCEXTPROC) (GLenum func, GLfloat ref);


#define GLEW_EXT_index_func GLEW_GET_VAR(__GLEW_EXT_index_func)

#endif /* GL_EXT_index_func */

/* ------------------------- GL_EXT_index_material ------------------------- */

#ifndef GL_EXT_index_material
#define GL_EXT_index_material 1

typedef void (GLAPIENTRY * PFNGLINDEXMATERIALEXTPROC) (GLenum face, GLenum mode);


#define GLEW_EXT_index_material GLEW_GET_VAR(__GLEW_EXT_index_material)

#endif /* GL_EXT_index_material */

/* -------------------------- GL_EXT_index_texture ------------------------- */

#ifndef GL_EXT_index_texture
#define GL_EXT_index_texture 1

#define GLEW_EXT_index_texture GLEW_GET_VAR(__GLEW_EXT_index_texture)

#endif /* GL_EXT_index_texture */

/* -------------------------- GL_EXT_light_texture ------------------------- */

#ifndef GL_EXT_light_texture
#define GL_EXT_light_texture 1

#define GL_FRAGMENT_MATERIAL_EXT 0x8349
#define GL_FRAGMENT_NORMAL_EXT 0x834A
#define GL_FRAGMENT_COLOR_EXT 0x834C
#define GL_ATTENUATION_EXT 0x834D
#define GL_SHADOW_ATTENUATION_EXT 0x834E
#define GL_TEXTURE_APPLICATION_MODE_EXT 0x834F
#define GL_TEXTURE_LIGHT_EXT 0x8350
#define GL_TEXTURE_MATERIAL_FACE_EXT 0x8351
#define GL_TEXTURE_MATERIAL_PARAMETER_EXT 0x8352

typedef void (GLAPIENTRY * PFNGLAPPLYTEXTUREEXTPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLTEXTURELIGHTEXTPROC) (GLenum pname);
typedef void (GLAPIENTRY * PFNGLTEXTUREMATERIALEXTPROC) (GLenum face, GLenum mode);


#define GLEW_EXT_light_texture GLEW_GET_VAR(__GLEW_EXT_light_texture)

#endif /* GL_EXT_light_texture */

/* ------------------------- GL_EXT_misc_attribute ------------------------- */

#ifndef GL_EXT_misc_attribute
#define GL_EXT_misc_attribute 1

#define GLEW_EXT_misc_attribute GLEW_GET_VAR(__GLEW_EXT_misc_attribute)

#endif /* GL_EXT_misc_attribute */

/* ------------------------ GL_EXT_multi_draw_arrays ----------------------- */

#ifndef GL_EXT_multi_draw_arrays
#define GL_EXT_multi_draw_arrays 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSEXTPROC) (GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSEXTPROC) (GLenum mode, GLsizei* count, GLenum type, const GLvoid **indices, GLsizei primcount);


#define GLEW_EXT_multi_draw_arrays GLEW_GET_VAR(__GLEW_EXT_multi_draw_arrays)

#endif /* GL_EXT_multi_draw_arrays */

/* --------------------------- GL_EXT_multisample -------------------------- */

#ifndef GL_EXT_multisample
#define GL_EXT_multisample 1

#define GL_MULTISAMPLE_EXT 0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_EXT 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_EXT 0x809F
#define GL_SAMPLE_MASK_EXT 0x80A0
#define GL_1PASS_EXT 0x80A1
#define GL_2PASS_0_EXT 0x80A2
#define GL_2PASS_1_EXT 0x80A3
#define GL_4PASS_0_EXT 0x80A4
#define GL_4PASS_1_EXT 0x80A5
#define GL_4PASS_2_EXT 0x80A6
#define GL_4PASS_3_EXT 0x80A7
#define GL_SAMPLE_BUFFERS_EXT 0x80A8
#define GL_SAMPLES_EXT 0x80A9
#define GL_SAMPLE_MASK_VALUE_EXT 0x80AA
#define GL_SAMPLE_MASK_INVERT_EXT 0x80AB
#define GL_SAMPLE_PATTERN_EXT 0x80AC
#define GL_MULTISAMPLE_BIT_EXT 0x20000000

typedef void (GLAPIENTRY * PFNGLSAMPLEMASKEXTPROC) (GLclampf value, GLboolean invert);
typedef void (GLAPIENTRY * PFNGLSAMPLEPATTERNEXTPROC) (GLenum pattern);


#define GLEW_EXT_multisample GLEW_GET_VAR(__GLEW_EXT_multisample)

#endif /* GL_EXT_multisample */

/* ---------------------- GL_EXT_packed_depth_stencil ---------------------- */

#ifndef GL_EXT_packed_depth_stencil
#define GL_EXT_packed_depth_stencil 1

#define GL_DEPTH_STENCIL_EXT 0x84F9
#define GL_UNSIGNED_INT_24_8_EXT 0x84FA
#define GL_DEPTH24_STENCIL8_EXT 0x88F0
#define GL_TEXTURE_STENCIL_SIZE_EXT 0x88F1

#define GLEW_EXT_packed_depth_stencil GLEW_GET_VAR(__GLEW_EXT_packed_depth_stencil)

#endif /* GL_EXT_packed_depth_stencil */

/* -------------------------- GL_EXT_packed_float -------------------------- */

#ifndef GL_EXT_packed_float
#define GL_EXT_packed_float 1

#define GL_R11F_G11F_B10F_EXT 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV_EXT 0x8C3B
#define GL_RGBA_SIGNED_COMPONENTS_EXT 0x8C3C

#define GLEW_EXT_packed_float GLEW_GET_VAR(__GLEW_EXT_packed_float)

#endif /* GL_EXT_packed_float */

/* -------------------------- GL_EXT_packed_pixels ------------------------- */

#ifndef GL_EXT_packed_pixels
#define GL_EXT_packed_pixels 1

#define GL_UNSIGNED_BYTE_3_3_2_EXT 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT 0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT 0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT 0x8036

#define GLEW_EXT_packed_pixels GLEW_GET_VAR(__GLEW_EXT_packed_pixels)

#endif /* GL_EXT_packed_pixels */

/* ------------------------ GL_EXT_paletted_texture ------------------------ */

#ifndef GL_EXT_paletted_texture
#define GL_EXT_paletted_texture 1

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_COLOR_TABLE_FORMAT_EXT 0x80D8
#define GL_COLOR_TABLE_WIDTH_EXT 0x80D9
#define GL_COLOR_TABLE_RED_SIZE_EXT 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_EXT 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_EXT 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_EXT 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_EXT 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_EXT 0x80DF
#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7
#define GL_TEXTURE_INDEX_SIZE_EXT 0x80ED
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B

typedef void (GLAPIENTRY * PFNGLCOLORTABLEEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void* data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEEXTPROC) (GLenum target, GLenum format, GLenum type, void* data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);


#define GLEW_EXT_paletted_texture GLEW_GET_VAR(__GLEW_EXT_paletted_texture)

#endif /* GL_EXT_paletted_texture */

/* ----------------------- GL_EXT_pixel_buffer_object ---------------------- */

#ifndef GL_EXT_pixel_buffer_object
#define GL_EXT_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_EXT 0x88EB
#define GL_PIXEL_UNPACK_BUFFER_EXT 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_EXT 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_EXT 0x88EF

#define GLEW_EXT_pixel_buffer_object GLEW_GET_VAR(__GLEW_EXT_pixel_buffer_object)

#endif /* GL_EXT_pixel_buffer_object */

/* ------------------------- GL_EXT_pixel_transform ------------------------ */

#ifndef GL_EXT_pixel_transform
#define GL_EXT_pixel_transform 1

#define GL_PIXEL_TRANSFORM_2D_EXT 0x8330
#define GL_PIXEL_MAG_FILTER_EXT 0x8331
#define GL_PIXEL_MIN_FILTER_EXT 0x8332
#define GL_PIXEL_CUBIC_WEIGHT_EXT 0x8333
#define GL_CUBIC_EXT 0x8334
#define GL_AVERAGE_EXT 0x8335
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8336
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8337
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT 0x8338

typedef void (GLAPIENTRY * PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERFEXTPROC) (GLenum target, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERIEXTPROC) (GLenum target, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);


#define GLEW_EXT_pixel_transform GLEW_GET_VAR(__GLEW_EXT_pixel_transform)

#endif /* GL_EXT_pixel_transform */

/* ------------------- GL_EXT_pixel_transform_color_table ------------------ */

#ifndef GL_EXT_pixel_transform_color_table
#define GL_EXT_pixel_transform_color_table 1

#define GLEW_EXT_pixel_transform_color_table GLEW_GET_VAR(__GLEW_EXT_pixel_transform_color_table)

#endif /* GL_EXT_pixel_transform_color_table */

/* ------------------------ GL_EXT_point_parameters ------------------------ */

#ifndef GL_EXT_point_parameters
#define GL_EXT_point_parameters 1

#define GL_POINT_SIZE_MIN_EXT 0x8126
#define GL_POINT_SIZE_MAX_EXT 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT 0x8128
#define GL_DISTANCE_ATTENUATION_EXT 0x8129

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFEXTPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVEXTPROC) (GLenum pname, const GLfloat* params);


#define GLEW_EXT_point_parameters GLEW_GET_VAR(__GLEW_EXT_point_parameters)

#endif /* GL_EXT_point_parameters */

/* ------------------------- GL_EXT_polygon_offset ------------------------- */

#ifndef GL_EXT_polygon_offset
#define GL_EXT_polygon_offset 1

#define GL_POLYGON_OFFSET_EXT 0x8037
#define GL_POLYGON_OFFSET_FACTOR_EXT 0x8038
#define GL_POLYGON_OFFSET_BIAS_EXT 0x8039

typedef void (GLAPIENTRY * PFNGLPOLYGONOFFSETEXTPROC) (GLfloat factor, GLfloat bias);


#define GLEW_EXT_polygon_offset GLEW_GET_VAR(__GLEW_EXT_polygon_offset)

#endif /* GL_EXT_polygon_offset */

/* ------------------------ GL_EXT_provoking_vertex ------------------------ */

#ifndef GL_EXT_provoking_vertex
#define GL_EXT_provoking_vertex 1

#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION_EXT 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION_EXT 0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT 0x8E4E
#define GL_PROVOKING_VERTEX_EXT 0x8E4F

typedef void (GLAPIENTRY * PFNGLPROVOKINGVERTEXEXTPROC) (GLenum mode);


#define GLEW_EXT_provoking_vertex GLEW_GET_VAR(__GLEW_EXT_provoking_vertex)

#endif /* GL_EXT_provoking_vertex */

/* ------------------------- GL_EXT_rescale_normal ------------------------- */

#ifndef GL_EXT_rescale_normal
#define GL_EXT_rescale_normal 1

#define GL_RESCALE_NORMAL_EXT 0x803A

#define GLEW_EXT_rescale_normal GLEW_GET_VAR(__GLEW_EXT_rescale_normal)

#endif /* GL_EXT_rescale_normal */

/* -------------------------- GL_EXT_scene_marker -------------------------- */

#ifndef GL_EXT_scene_marker
#define GL_EXT_scene_marker 1

typedef void (GLAPIENTRY * PFNGLBEGINSCENEEXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLENDSCENEEXTPROC) (void);


#define GLEW_EXT_scene_marker GLEW_GET_VAR(__GLEW_EXT_scene_marker)

#endif /* GL_EXT_scene_marker */

/* ------------------------- GL_EXT_secondary_color ------------------------ */

#ifndef GL_EXT_secondary_color
#define GL_EXT_secondary_color 1

#define GL_COLOR_SUM_EXT 0x8458
#define GL_CURRENT_SECONDARY_COLOR_EXT 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT 0x845D
#define GL_SECONDARY_COLOR_ARRAY_EXT 0x845E

typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BEXTPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVEXTPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DEXTPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVEXTPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FEXTPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVEXTPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IEXTPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVEXTPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SEXTPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVEXTPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBEXTPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVEXTPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIEXTPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVEXTPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USEXTPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVEXTPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);


#define GLEW_EXT_secondary_color GLEW_GET_VAR(__GLEW_EXT_secondary_color)

#endif /* GL_EXT_secondary_color */

/* --------------------- GL_EXT_separate_shader_objects -------------------- */

#ifndef GL_EXT_separate_shader_objects
#define GL_EXT_separate_shader_objects 1

#define GL_ACTIVE_PROGRAM_EXT 0x8B8D

typedef void (GLAPIENTRY * PFNGLACTIVEPROGRAMEXTPROC) (GLuint program);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROGRAMEXTPROC) (GLenum type, const GLchar* string);
typedef void (GLAPIENTRY * PFNGLUSESHADERPROGRAMEXTPROC) (GLenum type, GLuint program);


#define GLEW_EXT_separate_shader_objects GLEW_GET_VAR(__GLEW_EXT_separate_shader_objects)

#endif /* GL_EXT_separate_shader_objects */

/* --------------------- GL_EXT_separate_specular_color -------------------- */

#ifndef GL_EXT_separate_specular_color
#define GL_EXT_separate_specular_color 1

#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT 0x81F8
#define GL_SINGLE_COLOR_EXT 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR_EXT 0x81FA

#define GLEW_EXT_separate_specular_color GLEW_GET_VAR(__GLEW_EXT_separate_specular_color)

#endif /* GL_EXT_separate_specular_color */

/* --------------------- GL_EXT_shader_image_load_store -------------------- */

#ifndef GL_EXT_shader_image_load_store
#define GL_EXT_shader_image_load_store 1

#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT_EXT 0x00000002
#define GL_UNIFORM_BARRIER_BIT_EXT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT_EXT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_EXT 0x00000020
#define GL_COMMAND_BARRIER_BIT_EXT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT_EXT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT_EXT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT_EXT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT_EXT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT_EXT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT_EXT 0x00001000
#define GL_MAX_IMAGE_UNITS_EXT 0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS_EXT 0x8F39
#define GL_IMAGE_BINDING_NAME_EXT 0x8F3A
#define GL_IMAGE_BINDING_LEVEL_EXT 0x8F3B
#define GL_IMAGE_BINDING_LAYERED_EXT 0x8F3C
#define GL_IMAGE_BINDING_LAYER_EXT 0x8F3D
#define GL_IMAGE_BINDING_ACCESS_EXT 0x8F3E
#define GL_IMAGE_1D_EXT 0x904C
#define GL_IMAGE_2D_EXT 0x904D
#define GL_IMAGE_3D_EXT 0x904E
#define GL_IMAGE_2D_RECT_EXT 0x904F
#define GL_IMAGE_CUBE_EXT 0x9050
#define GL_IMAGE_BUFFER_EXT 0x9051
#define GL_IMAGE_1D_ARRAY_EXT 0x9052
#define GL_IMAGE_2D_ARRAY_EXT 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY_EXT 0x9054
#define GL_IMAGE_2D_MULTISAMPLE_EXT 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9056
#define GL_INT_IMAGE_1D_EXT 0x9057
#define GL_INT_IMAGE_2D_EXT 0x9058
#define GL_INT_IMAGE_3D_EXT 0x9059
#define GL_INT_IMAGE_2D_RECT_EXT 0x905A
#define GL_INT_IMAGE_CUBE_EXT 0x905B
#define GL_INT_IMAGE_BUFFER_EXT 0x905C
#define GL_INT_IMAGE_1D_ARRAY_EXT 0x905D
#define GL_INT_IMAGE_2D_ARRAY_EXT 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE_EXT 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D_EXT 0x9062
#define GL_UNSIGNED_INT_IMAGE_2D_EXT 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D_EXT 0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT_EXT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE_EXT 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER_EXT 0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY_EXT 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY_EXT 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_EXT 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x906C
#define GL_MAX_IMAGE_SAMPLES_EXT 0x906D
#define GL_IMAGE_BINDING_FORMAT_EXT 0x906E
#define GL_ALL_BARRIER_BITS_EXT 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDIMAGETEXTUREEXTPROC) (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
typedef void (GLAPIENTRY * PFNGLMEMORYBARRIEREXTPROC) (GLbitfield barriers);


#define GLEW_EXT_shader_image_load_store GLEW_GET_VAR(__GLEW_EXT_shader_image_load_store)

#endif /* GL_EXT_shader_image_load_store */

/* -------------------------- GL_EXT_shadow_funcs -------------------------- */

#ifndef GL_EXT_shadow_funcs
#define GL_EXT_shadow_funcs 1

#define GLEW_EXT_shadow_funcs GLEW_GET_VAR(__GLEW_EXT_shadow_funcs)

#endif /* GL_EXT_shadow_funcs */

/* --------------------- GL_EXT_shared_texture_palette --------------------- */

#ifndef GL_EXT_shared_texture_palette
#define GL_EXT_shared_texture_palette 1

#define GL_SHARED_TEXTURE_PALETTE_EXT 0x81FB

#define GLEW_EXT_shared_texture_palette GLEW_GET_VAR(__GLEW_EXT_shared_texture_palette)

#endif /* GL_EXT_shared_texture_palette */

/* ------------------------ GL_EXT_stencil_clear_tag ----------------------- */

#ifndef GL_EXT_stencil_clear_tag
#define GL_EXT_stencil_clear_tag 1

#define GL_STENCIL_TAG_BITS_EXT 0x88F2
#define GL_STENCIL_CLEAR_TAG_VALUE_EXT 0x88F3

#define GLEW_EXT_stencil_clear_tag GLEW_GET_VAR(__GLEW_EXT_stencil_clear_tag)

#endif /* GL_EXT_stencil_clear_tag */

/* ------------------------ GL_EXT_stencil_two_side ------------------------ */

#ifndef GL_EXT_stencil_two_side
#define GL_EXT_stencil_two_side 1

#define GL_STENCIL_TEST_TWO_SIDE_EXT 0x8910
#define GL_ACTIVE_STENCIL_FACE_EXT 0x8911

typedef void (GLAPIENTRY * PFNGLACTIVESTENCILFACEEXTPROC) (GLenum face);


#define GLEW_EXT_stencil_two_side GLEW_GET_VAR(__GLEW_EXT_stencil_two_side)

#endif /* GL_EXT_stencil_two_side */

/* -------------------------- GL_EXT_stencil_wrap -------------------------- */

#ifndef GL_EXT_stencil_wrap
#define GL_EXT_stencil_wrap 1

#define GL_INCR_WRAP_EXT 0x8507
#define GL_DECR_WRAP_EXT 0x8508

#define GLEW_EXT_stencil_wrap GLEW_GET_VAR(__GLEW_EXT_stencil_wrap)

#endif /* GL_EXT_stencil_wrap */

/* --------------------------- GL_EXT_subtexture --------------------------- */

#ifndef GL_EXT_subtexture
#define GL_EXT_subtexture 1

typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);


#define GLEW_EXT_subtexture GLEW_GET_VAR(__GLEW_EXT_subtexture)

#endif /* GL_EXT_subtexture */

/* ----------------------------- GL_EXT_texture ---------------------------- */

#ifndef GL_EXT_texture
#define GL_EXT_texture 1

#define GL_ALPHA4_EXT 0x803B
#define GL_ALPHA8_EXT 0x803C
#define GL_ALPHA12_EXT 0x803D
#define GL_ALPHA16_EXT 0x803E
#define GL_LUMINANCE4_EXT 0x803F
#define GL_LUMINANCE8_EXT 0x8040
#define GL_LUMINANCE12_EXT 0x8041
#define GL_LUMINANCE16_EXT 0x8042
#define GL_LUMINANCE4_ALPHA4_EXT 0x8043
#define GL_LUMINANCE6_ALPHA2_EXT 0x8044
#define GL_LUMINANCE8_ALPHA8_EXT 0x8045
#define GL_LUMINANCE12_ALPHA4_EXT 0x8046
#define GL_LUMINANCE12_ALPHA12_EXT 0x8047
#define GL_LUMINANCE16_ALPHA16_EXT 0x8048
#define GL_INTENSITY_EXT 0x8049
#define GL_INTENSITY4_EXT 0x804A
#define GL_INTENSITY8_EXT 0x804B
#define GL_INTENSITY12_EXT 0x804C
#define GL_INTENSITY16_EXT 0x804D
#define GL_RGB2_EXT 0x804E
#define GL_RGB4_EXT 0x804F
#define GL_RGB5_EXT 0x8050
#define GL_RGB8_EXT 0x8051
#define GL_RGB10_EXT 0x8052
#define GL_RGB12_EXT 0x8053
#define GL_RGB16_EXT 0x8054
#define GL_RGBA2_EXT 0x8055
#define GL_RGBA4_EXT 0x8056
#define GL_RGB5_A1_EXT 0x8057
#define GL_RGBA8_EXT 0x8058
#define GL_RGB10_A2_EXT 0x8059
#define GL_RGBA12_EXT 0x805A
#define GL_RGBA16_EXT 0x805B
#define GL_TEXTURE_RED_SIZE_EXT 0x805C
#define GL_TEXTURE_GREEN_SIZE_EXT 0x805D
#define GL_TEXTURE_BLUE_SIZE_EXT 0x805E
#define GL_TEXTURE_ALPHA_SIZE_EXT 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE_EXT 0x8060
#define GL_TEXTURE_INTENSITY_SIZE_EXT 0x8061
#define GL_REPLACE_EXT 0x8062
#define GL_PROXY_TEXTURE_1D_EXT 0x8063
#define GL_PROXY_TEXTURE_2D_EXT 0x8064

#define GLEW_EXT_texture GLEW_GET_VAR(__GLEW_EXT_texture)

#endif /* GL_EXT_texture */

/* ---------------------------- GL_EXT_texture3D --------------------------- */

#ifndef GL_EXT_texture3D
#define GL_EXT_texture3D 1

#define GL_PACK_SKIP_IMAGES_EXT 0x806B
#define GL_PACK_IMAGE_HEIGHT_EXT 0x806C
#define GL_UNPACK_SKIP_IMAGES_EXT 0x806D
#define GL_UNPACK_IMAGE_HEIGHT_EXT 0x806E
#define GL_TEXTURE_3D_EXT 0x806F
#define GL_PROXY_TEXTURE_3D_EXT 0x8070
#define GL_TEXTURE_DEPTH_EXT 0x8071
#define GL_TEXTURE_WRAP_R_EXT 0x8072
#define GL_MAX_3D_TEXTURE_SIZE_EXT 0x8073

typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);


#define GLEW_EXT_texture3D GLEW_GET_VAR(__GLEW_EXT_texture3D)

#endif /* GL_EXT_texture3D */

/* -------------------------- GL_EXT_texture_array ------------------------- */

#ifndef GL_EXT_texture_array
#define GL_EXT_texture_array 1

#define GL_COMPARE_REF_DEPTH_TO_TEXTURE_EXT 0x884E
#define GL_MAX_ARRAY_TEXTURE_LAYERS_EXT 0x88FF
#define GL_TEXTURE_1D_ARRAY_EXT 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY_EXT 0x8C19
#define GL_TEXTURE_2D_ARRAY_EXT 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY_EXT 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY_EXT 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY_EXT 0x8C1D

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);


#define GLEW_EXT_texture_array GLEW_GET_VAR(__GLEW_EXT_texture_array)

#endif /* GL_EXT_texture_array */

/* ---------------------- GL_EXT_texture_buffer_object --------------------- */

#ifndef GL_EXT_texture_buffer_object
#define GL_EXT_texture_buffer_object 1

#define GL_TEXTURE_BUFFER_EXT 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_EXT 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_EXT 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT_EXT 0x8C2E

typedef void (GLAPIENTRY * PFNGLTEXBUFFEREXTPROC) (GLenum target, GLenum internalformat, GLuint buffer);


#define GLEW_EXT_texture_buffer_object GLEW_GET_VAR(__GLEW_EXT_texture_buffer_object)

#endif /* GL_EXT_texture_buffer_object */

/* -------------------- GL_EXT_texture_compression_dxt1 -------------------- */

#ifndef GL_EXT_texture_compression_dxt1
#define GL_EXT_texture_compression_dxt1 1

#define GLEW_EXT_texture_compression_dxt1 GLEW_GET_VAR(__GLEW_EXT_texture_compression_dxt1)

#endif /* GL_EXT_texture_compression_dxt1 */

/* -------------------- GL_EXT_texture_compression_latc -------------------- */

#ifndef GL_EXT_texture_compression_latc
#define GL_EXT_texture_compression_latc 1

#define GL_COMPRESSED_LUMINANCE_LATC1_EXT 0x8C70
#define GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT 0x8C71
#define GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT 0x8C72
#define GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT 0x8C73

#define GLEW_EXT_texture_compression_latc GLEW_GET_VAR(__GLEW_EXT_texture_compression_latc)

#endif /* GL_EXT_texture_compression_latc */

/* -------------------- GL_EXT_texture_compression_rgtc -------------------- */

#ifndef GL_EXT_texture_compression_rgtc
#define GL_EXT_texture_compression_rgtc 1

#define GL_COMPRESSED_RED_RGTC1_EXT 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1_EXT 0x8DBC
#define GL_COMPRESSED_RED_GREEN_RGTC2_EXT 0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT 0x8DBE

#define GLEW_EXT_texture_compression_rgtc GLEW_GET_VAR(__GLEW_EXT_texture_compression_rgtc)

#endif /* GL_EXT_texture_compression_rgtc */

/* -------------------- GL_EXT_texture_compression_s3tc -------------------- */

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

#define GLEW_EXT_texture_compression_s3tc GLEW_GET_VAR(__GLEW_EXT_texture_compression_s3tc)

#endif /* GL_EXT_texture_compression_s3tc */

/* ------------------------ GL_EXT_texture_cube_map ------------------------ */

#ifndef GL_EXT_texture_cube_map
#define GL_EXT_texture_cube_map 1

#define GL_NORMAL_MAP_EXT 0x8511
#define GL_REFLECTION_MAP_EXT 0x8512
#define GL_TEXTURE_CUBE_MAP_EXT 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT 0x851C

#define GLEW_EXT_texture_cube_map GLEW_GET_VAR(__GLEW_EXT_texture_cube_map)

#endif /* GL_EXT_texture_cube_map */

/* ----------------------- GL_EXT_texture_edge_clamp ----------------------- */

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE_EXT 0x812F

#define GLEW_EXT_texture_edge_clamp GLEW_GET_VAR(__GLEW_EXT_texture_edge_clamp)

#endif /* GL_EXT_texture_edge_clamp */

/* --------------------------- GL_EXT_texture_env -------------------------- */

#ifndef GL_EXT_texture_env
#define GL_EXT_texture_env 1

#define GL_TEXTURE_ENV0_EXT 0
#define GL_ENV_BLEND_EXT 0
#define GL_TEXTURE_ENV_SHIFT_EXT 0
#define GL_ENV_REPLACE_EXT 0
#define GL_ENV_ADD_EXT 0
#define GL_ENV_SUBTRACT_EXT 0
#define GL_TEXTURE_ENV_MODE_ALPHA_EXT 0
#define GL_ENV_REVERSE_SUBTRACT_EXT 0
#define GL_ENV_REVERSE_BLEND_EXT 0
#define GL_ENV_COPY_EXT 0
#define GL_ENV_MODULATE_EXT 0

#define GLEW_EXT_texture_env GLEW_GET_VAR(__GLEW_EXT_texture_env)

#endif /* GL_EXT_texture_env */

/* ------------------------- GL_EXT_texture_env_add ------------------------ */

#ifndef GL_EXT_texture_env_add
#define GL_EXT_texture_env_add 1

#define GLEW_EXT_texture_env_add GLEW_GET_VAR(__GLEW_EXT_texture_env_add)

#endif /* GL_EXT_texture_env_add */

/* ----------------------- GL_EXT_texture_env_combine ---------------------- */

#ifndef GL_EXT_texture_env_combine
#define GL_EXT_texture_env_combine 1

#define GL_COMBINE_EXT 0x8570
#define GL_COMBINE_RGB_EXT 0x8571
#define GL_COMBINE_ALPHA_EXT 0x8572
#define GL_RGB_SCALE_EXT 0x8573
#define GL_ADD_SIGNED_EXT 0x8574
#define GL_INTERPOLATE_EXT 0x8575
#define GL_CONSTANT_EXT 0x8576
#define GL_PRIMARY_COLOR_EXT 0x8577
#define GL_PREVIOUS_EXT 0x8578
#define GL_SOURCE0_RGB_EXT 0x8580
#define GL_SOURCE1_RGB_EXT 0x8581
#define GL_SOURCE2_RGB_EXT 0x8582
#define GL_SOURCE0_ALPHA_EXT 0x8588
#define GL_SOURCE1_ALPHA_EXT 0x8589
#define GL_SOURCE2_ALPHA_EXT 0x858A
#define GL_OPERAND0_RGB_EXT 0x8590
#define GL_OPERAND1_RGB_EXT 0x8591
#define GL_OPERAND2_RGB_EXT 0x8592
#define GL_OPERAND0_ALPHA_EXT 0x8598
#define GL_OPERAND1_ALPHA_EXT 0x8599
#define GL_OPERAND2_ALPHA_EXT 0x859A

#define GLEW_EXT_texture_env_combine GLEW_GET_VAR(__GLEW_EXT_texture_env_combine)

#endif /* GL_EXT_texture_env_combine */

/* ------------------------ GL_EXT_texture_env_dot3 ------------------------ */

#ifndef GL_EXT_texture_env_dot3
#define GL_EXT_texture_env_dot3 1

#define GL_DOT3_RGB_EXT 0x8740
#define GL_DOT3_RGBA_EXT 0x8741

#define GLEW_EXT_texture_env_dot3 GLEW_GET_VAR(__GLEW_EXT_texture_env_dot3)

#endif /* GL_EXT_texture_env_dot3 */

/* ------------------- GL_EXT_texture_filter_anisotropic ------------------- */

#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

#define GLEW_EXT_texture_filter_anisotropic GLEW_GET_VAR(__GLEW_EXT_texture_filter_anisotropic)

#endif /* GL_EXT_texture_filter_anisotropic */

/* ------------------------- GL_EXT_texture_integer ------------------------ */

#ifndef GL_EXT_texture_integer
#define GL_EXT_texture_integer 1

#define GL_RGBA32UI_EXT 0x8D70
#define GL_RGB32UI_EXT 0x8D71
#define GL_ALPHA32UI_EXT 0x8D72
#define GL_INTENSITY32UI_EXT 0x8D73
#define GL_LUMINANCE32UI_EXT 0x8D74
#define GL_LUMINANCE_ALPHA32UI_EXT 0x8D75
#define GL_RGBA16UI_EXT 0x8D76
#define GL_RGB16UI_EXT 0x8D77
#define GL_ALPHA16UI_EXT 0x8D78
#define GL_INTENSITY16UI_EXT 0x8D79
#define GL_LUMINANCE16UI_EXT 0x8D7A
#define GL_LUMINANCE_ALPHA16UI_EXT 0x8D7B
#define GL_RGBA8UI_EXT 0x8D7C
#define GL_RGB8UI_EXT 0x8D7D
#define GL_ALPHA8UI_EXT 0x8D7E
#define GL_INTENSITY8UI_EXT 0x8D7F
#define GL_LUMINANCE8UI_EXT 0x8D80
#define GL_LUMINANCE_ALPHA8UI_EXT 0x8D81
#define GL_RGBA32I_EXT 0x8D82
#define GL_RGB32I_EXT 0x8D83
#define GL_ALPHA32I_EXT 0x8D84
#define GL_INTENSITY32I_EXT 0x8D85
#define GL_LUMINANCE32I_EXT 0x8D86
#define GL_LUMINANCE_ALPHA32I_EXT 0x8D87
#define GL_RGBA16I_EXT 0x8D88
#define GL_RGB16I_EXT 0x8D89
#define GL_ALPHA16I_EXT 0x8D8A
#define GL_INTENSITY16I_EXT 0x8D8B
#define GL_LUMINANCE16I_EXT 0x8D8C
#define GL_LUMINANCE_ALPHA16I_EXT 0x8D8D
#define GL_RGBA8I_EXT 0x8D8E
#define GL_RGB8I_EXT 0x8D8F
#define GL_ALPHA8I_EXT 0x8D90
#define GL_INTENSITY8I_EXT 0x8D91
#define GL_LUMINANCE8I_EXT 0x8D92
#define GL_LUMINANCE_ALPHA8I_EXT 0x8D93
#define GL_RED_INTEGER_EXT 0x8D94
#define GL_GREEN_INTEGER_EXT 0x8D95
#define GL_BLUE_INTEGER_EXT 0x8D96
#define GL_ALPHA_INTEGER_EXT 0x8D97
#define GL_RGB_INTEGER_EXT 0x8D98
#define GL_RGBA_INTEGER_EXT 0x8D99
#define GL_BGR_INTEGER_EXT 0x8D9A
#define GL_BGRA_INTEGER_EXT 0x8D9B
#define GL_LUMINANCE_INTEGER_EXT 0x8D9C
#define GL_LUMINANCE_ALPHA_INTEGER_EXT 0x8D9D
#define GL_RGBA_INTEGER_MODE_EXT 0x8D9E

typedef void (GLAPIENTRY * PFNGLCLEARCOLORIIEXTPROC) (GLint red, GLint green, GLint blue, GLint alpha);
typedef void (GLAPIENTRY * PFNGLCLEARCOLORIUIEXTPROC) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, const GLuint *params);


#define GLEW_EXT_texture_integer GLEW_GET_VAR(__GLEW_EXT_texture_integer)

#endif /* GL_EXT_texture_integer */

/* ------------------------ GL_EXT_texture_lod_bias ------------------------ */

#ifndef GL_EXT_texture_lod_bias
#define GL_EXT_texture_lod_bias 1

#define GL_MAX_TEXTURE_LOD_BIAS_EXT 0x84FD
#define GL_TEXTURE_FILTER_CONTROL_EXT 0x8500
#define GL_TEXTURE_LOD_BIAS_EXT 0x8501

#define GLEW_EXT_texture_lod_bias GLEW_GET_VAR(__GLEW_EXT_texture_lod_bias)

#endif /* GL_EXT_texture_lod_bias */

/* ---------------------- GL_EXT_texture_mirror_clamp ---------------------- */

#ifndef GL_EXT_texture_mirror_clamp
#define GL_EXT_texture_mirror_clamp 1

#define GL_MIRROR_CLAMP_EXT 0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT 0x8912

#define GLEW_EXT_texture_mirror_clamp GLEW_GET_VAR(__GLEW_EXT_texture_mirror_clamp)

#endif /* GL_EXT_texture_mirror_clamp */

/* ------------------------- GL_EXT_texture_object ------------------------- */

#ifndef GL_EXT_texture_object
#define GL_EXT_texture_object 1

#define GL_TEXTURE_PRIORITY_EXT 0x8066
#define GL_TEXTURE_RESIDENT_EXT 0x8067
#define GL_TEXTURE_1D_BINDING_EXT 0x8068
#define GL_TEXTURE_2D_BINDING_EXT 0x8069
#define GL_TEXTURE_3D_BINDING_EXT 0x806A

typedef GLboolean (GLAPIENTRY * PFNGLARETEXTURESRESIDENTEXTPROC) (GLsizei n, const GLuint* textures, GLboolean* residences);
typedef void (GLAPIENTRY * PFNGLBINDTEXTUREEXTPROC) (GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLDELETETEXTURESEXTPROC) (GLsizei n, const GLuint* textures);
typedef void (GLAPIENTRY * PFNGLGENTEXTURESEXTPROC) (GLsizei n, GLuint* textures);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREEXTPROC) (GLuint texture);
typedef void (GLAPIENTRY * PFNGLPRIORITIZETEXTURESEXTPROC) (GLsizei n, const GLuint* textures, const GLclampf* priorities);


#define GLEW_EXT_texture_object GLEW_GET_VAR(__GLEW_EXT_texture_object)

#endif /* GL_EXT_texture_object */

/* --------------------- GL_EXT_texture_perturb_normal --------------------- */

#ifndef GL_EXT_texture_perturb_normal
#define GL_EXT_texture_perturb_normal 1

#define GL_PERTURB_EXT 0x85AE
#define GL_TEXTURE_NORMAL_EXT 0x85AF

typedef void (GLAPIENTRY * PFNGLTEXTURENORMALEXTPROC) (GLenum mode);


#define GLEW_EXT_texture_perturb_normal GLEW_GET_VAR(__GLEW_EXT_texture_perturb_normal)

#endif /* GL_EXT_texture_perturb_normal */

/* ------------------------ GL_EXT_texture_rectangle ----------------------- */

#ifndef GL_EXT_texture_rectangle
#define GL_EXT_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT 0x84F8

#define GLEW_EXT_texture_rectangle GLEW_GET_VAR(__GLEW_EXT_texture_rectangle)

#endif /* GL_EXT_texture_rectangle */

/* -------------------------- GL_EXT_texture_sRGB -------------------------- */

#ifndef GL_EXT_texture_sRGB
#define GL_EXT_texture_sRGB 1

#define GL_SRGB_EXT 0x8C40
#define GL_SRGB8_EXT 0x8C41
#define GL_SRGB_ALPHA_EXT 0x8C42
#define GL_SRGB8_ALPHA8_EXT 0x8C43
#define GL_SLUMINANCE_ALPHA_EXT 0x8C44
#define GL_SLUMINANCE8_ALPHA8_EXT 0x8C45
#define GL_SLUMINANCE_EXT 0x8C46
#define GL_SLUMINANCE8_EXT 0x8C47
#define GL_COMPRESSED_SRGB_EXT 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA_EXT 0x8C49
#define GL_COMPRESSED_SLUMINANCE_EXT 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA_EXT 0x8C4B
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT 0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F

#define GLEW_EXT_texture_sRGB GLEW_GET_VAR(__GLEW_EXT_texture_sRGB)

#endif /* GL_EXT_texture_sRGB */

/* ----------------------- GL_EXT_texture_sRGB_decode ---------------------- */

#ifndef GL_EXT_texture_sRGB_decode
#define GL_EXT_texture_sRGB_decode 1

#define GL_TEXTURE_SRGB_DECODE_EXT 0x8A48
#define GL_DECODE_EXT 0x8A49
#define GL_SKIP_DECODE_EXT 0x8A4A

#define GLEW_EXT_texture_sRGB_decode GLEW_GET_VAR(__GLEW_EXT_texture_sRGB_decode)

#endif /* GL_EXT_texture_sRGB_decode */

/* --------------------- GL_EXT_texture_shared_exponent -------------------- */

#ifndef GL_EXT_texture_shared_exponent
#define GL_EXT_texture_shared_exponent 1

#define GL_RGB9_E5_EXT 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV_EXT 0x8C3E
#define GL_TEXTURE_SHARED_SIZE_EXT 0x8C3F

#define GLEW_EXT_texture_shared_exponent GLEW_GET_VAR(__GLEW_EXT_texture_shared_exponent)

#endif /* GL_EXT_texture_shared_exponent */

/* -------------------------- GL_EXT_texture_snorm ------------------------- */

#ifndef GL_EXT_texture_snorm
#define GL_EXT_texture_snorm 1

#define GL_RED_SNORM 0x8F90
#define GL_RG_SNORM 0x8F91
#define GL_RGB_SNORM 0x8F92
#define GL_RGBA_SNORM 0x8F93
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_ALPHA_SNORM 0x9010
#define GL_LUMINANCE_SNORM 0x9011
#define GL_LUMINANCE_ALPHA_SNORM 0x9012
#define GL_INTENSITY_SNORM 0x9013
#define GL_ALPHA8_SNORM 0x9014
#define GL_LUMINANCE8_SNORM 0x9015
#define GL_LUMINANCE8_ALPHA8_SNORM 0x9016
#define GL_INTENSITY8_SNORM 0x9017
#define GL_ALPHA16_SNORM 0x9018
#define GL_LUMINANCE16_SNORM 0x9019
#define GL_LUMINANCE16_ALPHA16_SNORM 0x901A
#define GL_INTENSITY16_SNORM 0x901B

#define GLEW_EXT_texture_snorm GLEW_GET_VAR(__GLEW_EXT_texture_snorm)

#endif /* GL_EXT_texture_snorm */

/* ------------------------- GL_EXT_texture_swizzle ------------------------ */

#ifndef GL_EXT_texture_swizzle
#define GL_EXT_texture_swizzle 1

#define GL_TEXTURE_SWIZZLE_R_EXT 0x8E42
#define GL_TEXTURE_SWIZZLE_G_EXT 0x8E43
#define GL_TEXTURE_SWIZZLE_B_EXT 0x8E44
#define GL_TEXTURE_SWIZZLE_A_EXT 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA_EXT 0x8E46

#define GLEW_EXT_texture_swizzle GLEW_GET_VAR(__GLEW_EXT_texture_swizzle)

#endif /* GL_EXT_texture_swizzle */

/* --------------------------- GL_EXT_timer_query -------------------------- */

#ifndef GL_EXT_timer_query
#define GL_EXT_timer_query 1

#define GL_TIME_ELAPSED_EXT 0x88BF

typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTI64VEXTPROC) (GLuint id, GLenum pname, GLint64EXT *params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VEXTPROC) (GLuint id, GLenum pname, GLuint64EXT *params);


#define GLEW_EXT_timer_query GLEW_GET_VAR(__GLEW_EXT_timer_query)

#endif /* GL_EXT_timer_query */

/* ----------------------- GL_EXT_transform_feedback ----------------------- */

#ifndef GL_EXT_transform_feedback
#define GL_EXT_transform_feedback 1

#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS_EXT 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT 0x8C85
#define GL_PRIMITIVES_GENERATED_EXT 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT 0x8C88
#define GL_RASTERIZER_DISCARD_EXT 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT 0x8C8B
#define GL_INTERLEAVED_ATTRIBS_EXT 0x8C8C
#define GL_SEPARATE_ATTRIBS_EXT 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER_EXT 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT 0x8C8F

typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKEXTPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASEEXTPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFEROFFSETEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGEEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKEXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC) (GLuint program, GLsizei count, const GLchar ** varyings, GLenum bufferMode);


#define GLEW_EXT_transform_feedback GLEW_GET_VAR(__GLEW_EXT_transform_feedback)

#endif /* GL_EXT_transform_feedback */

/* -------------------------- GL_EXT_vertex_array -------------------------- */

#ifndef GL_EXT_vertex_array
#define GL_EXT_vertex_array 1

#define GL_DOUBLE_EXT 0x140A
#define GL_VERTEX_ARRAY_EXT 0x8074
#define GL_NORMAL_ARRAY_EXT 0x8075
#define GL_COLOR_ARRAY_EXT 0x8076
#define GL_INDEX_ARRAY_EXT 0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT 0x8078
#define GL_EDGE_FLAG_ARRAY_EXT 0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT 0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT 0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT 0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT 0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT 0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT 0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT 0x8080
#define GL_COLOR_ARRAY_SIZE_EXT 0x8081
#define GL_COLOR_ARRAY_TYPE_EXT 0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT 0x8083
#define GL_COLOR_ARRAY_COUNT_EXT 0x8084
#define GL_INDEX_ARRAY_TYPE_EXT 0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT 0x8086
#define GL_INDEX_ARRAY_COUNT_EXT 0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT 0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT 0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT 0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT 0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT 0x808F
#define GL_COLOR_ARRAY_POINTER_EXT 0x8090
#define GL_INDEX_ARRAY_POINTER_EXT 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT 0x8093

typedef void (GLAPIENTRY * PFNGLARRAYELEMENTEXTPROC) (GLint i);
typedef void (GLAPIENTRY * PFNGLCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSEXTPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPOINTEREXTPROC) (GLsizei stride, GLsizei count, const GLboolean* pointer);
typedef void (GLAPIENTRY * PFNGLINDEXPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);


#define GLEW_EXT_vertex_array GLEW_GET_VAR(__GLEW_EXT_vertex_array)

#endif /* GL_EXT_vertex_array */

/* ------------------------ GL_EXT_vertex_array_bgra ----------------------- */

#ifndef GL_EXT_vertex_array_bgra
#define GL_EXT_vertex_array_bgra 1

#define GL_BGRA 0x80E1

#define GLEW_EXT_vertex_array_bgra GLEW_GET_VAR(__GLEW_EXT_vertex_array_bgra)

#endif /* GL_EXT_vertex_array_bgra */

/* ----------------------- GL_EXT_vertex_attrib_64bit ---------------------- */

#ifndef GL_EXT_vertex_attrib_64bit
#define GL_EXT_vertex_attrib_64bit 1

#define GL_DOUBLE_MAT2_EXT 0x8F46
#define GL_DOUBLE_MAT3_EXT 0x8F47
#define GL_DOUBLE_MAT4_EXT 0x8F48
#define GL_DOUBLE_MAT2x3_EXT 0x8F49
#define GL_DOUBLE_MAT2x4_EXT 0x8F4A
#define GL_DOUBLE_MAT3x2_EXT 0x8F4B
#define GL_DOUBLE_MAT3x4_EXT 0x8F4C
#define GL_DOUBLE_MAT4x2_EXT 0x8F4D
#define GL_DOUBLE_MAT4x3_EXT 0x8F4E
#define GL_DOUBLE_VEC2_EXT 0x8FFC
#define GL_DOUBLE_VEC3_EXT 0x8FFD
#define GL_DOUBLE_VEC4_EXT 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLDVEXTPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DEXTPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DEXTPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);


#define GLEW_EXT_vertex_attrib_64bit GLEW_GET_VAR(__GLEW_EXT_vertex_attrib_64bit)

#endif /* GL_EXT_vertex_attrib_64bit */

/* -------------------------- GL_EXT_vertex_shader ------------------------- */

#ifndef GL_EXT_vertex_shader
#define GL_EXT_vertex_shader 1

#define GL_VERTEX_SHADER_EXT 0x8780
#define GL_VERTEX_SHADER_BINDING_EXT 0x8781
#define GL_OP_INDEX_EXT 0x8782
#define GL_OP_NEGATE_EXT 0x8783
#define GL_OP_DOT3_EXT 0x8784
#define GL_OP_DOT4_EXT 0x8785
#define GL_OP_MUL_EXT 0x8786
#define GL_OP_ADD_EXT 0x8787
#define GL_OP_MADD_EXT 0x8788
#define GL_OP_FRAC_EXT 0x8789
#define GL_OP_MAX_EXT 0x878A
#define GL_OP_MIN_EXT 0x878B
#define GL_OP_SET_GE_EXT 0x878C
#define GL_OP_SET_LT_EXT 0x878D
#define GL_OP_CLAMP_EXT 0x878E
#define GL_OP_FLOOR_EXT 0x878F
#define GL_OP_ROUND_EXT 0x8790
#define GL_OP_EXP_BASE_2_EXT 0x8791
#define GL_OP_LOG_BASE_2_EXT 0x8792
#define GL_OP_POWER_EXT 0x8793
#define GL_OP_RECIP_EXT 0x8794
#define GL_OP_RECIP_SQRT_EXT 0x8795
#define GL_OP_SUB_EXT 0x8796
#define GL_OP_CROSS_PRODUCT_EXT 0x8797
#define GL_OP_MULTIPLY_MATRIX_EXT 0x8798
#define GL_OP_MOV_EXT 0x8799
#define GL_OUTPUT_VERTEX_EXT 0x879A
#define GL_OUTPUT_COLOR0_EXT 0x879B
#define GL_OUTPUT_COLOR1_EXT 0x879C
#define GL_OUTPUT_TEXTURE_COORD0_EXT 0x879D
#define GL_OUTPUT_TEXTURE_COORD1_EXT 0x879E
#define GL_OUTPUT_TEXTURE_COORD2_EXT 0x879F
#define GL_OUTPUT_TEXTURE_COORD3_EXT 0x87A0
#define GL_OUTPUT_TEXTURE_COORD4_EXT 0x87A1
#define GL_OUTPUT_TEXTURE_COORD5_EXT 0x87A2
#define GL_OUTPUT_TEXTURE_COORD6_EXT 0x87A3
#define GL_OUTPUT_TEXTURE_COORD7_EXT 0x87A4
#define GL_OUTPUT_TEXTURE_COORD8_EXT 0x87A5
#define GL_OUTPUT_TEXTURE_COORD9_EXT 0x87A6
#define GL_OUTPUT_TEXTURE_COORD10_EXT 0x87A7
#define GL_OUTPUT_TEXTURE_COORD11_EXT 0x87A8
#define GL_OUTPUT_TEXTURE_COORD12_EXT 0x87A9
#define GL_OUTPUT_TEXTURE_COORD13_EXT 0x87AA
#define GL_OUTPUT_TEXTURE_COORD14_EXT 0x87AB
#define GL_OUTPUT_TEXTURE_COORD15_EXT 0x87AC
#define GL_OUTPUT_TEXTURE_COORD16_EXT 0x87AD
#define GL_OUTPUT_TEXTURE_COORD17_EXT 0x87AE
#define GL_OUTPUT_TEXTURE_COORD18_EXT 0x87AF
#define GL_OUTPUT_TEXTURE_COORD19_EXT 0x87B0
#define GL_OUTPUT_TEXTURE_COORD20_EXT 0x87B1
#define GL_OUTPUT_TEXTURE_COORD21_EXT 0x87B2
#define GL_OUTPUT_TEXTURE_COORD22_EXT 0x87B3
#define GL_OUTPUT_TEXTURE_COORD23_EXT 0x87B4
#define GL_OUTPUT_TEXTURE_COORD24_EXT 0x87B5
#define GL_OUTPUT_TEXTURE_COORD25_EXT 0x87B6
#define GL_OUTPUT_TEXTURE_COORD26_EXT 0x87B7
#define GL_OUTPUT_TEXTURE_COORD27_EXT 0x87B8
#define GL_OUTPUT_TEXTURE_COORD28_EXT 0x87B9
#define GL_OUTPUT_TEXTURE_COORD29_EXT 0x87BA
#define GL_OUTPUT_TEXTURE_COORD30_EXT 0x87BB
#define GL_OUTPUT_TEXTURE_COORD31_EXT 0x87BC
#define GL_OUTPUT_FOG_EXT 0x87BD
#define GL_SCALAR_EXT 0x87BE
#define GL_VECTOR_EXT 0x87BF
#define GL_MATRIX_EXT 0x87C0
#define GL_VARIANT_EXT 0x87C1
#define GL_INVARIANT_EXT 0x87C2
#define GL_LOCAL_CONSTANT_EXT 0x87C3
#define GL_LOCAL_EXT 0x87C4
#define GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87C5
#define GL_MAX_VERTEX_SHADER_VARIANTS_EXT 0x87C6
#define GL_MAX_VERTEX_SHADER_INVARIANTS_EXT 0x87C7
#define GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87C8
#define GL_MAX_VERTEX_SHADER_LOCALS_EXT 0x87C9
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CA
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT 0x87CB
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT 0x87CC
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87CD
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT 0x87CE
#define GL_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CF
#define GL_VERTEX_SHADER_VARIANTS_EXT 0x87D0
#define GL_VERTEX_SHADER_INVARIANTS_EXT 0x87D1
#define GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87D2
#define GL_VERTEX_SHADER_LOCALS_EXT 0x87D3
#define GL_VERTEX_SHADER_OPTIMIZED_EXT 0x87D4
#define GL_X_EXT 0x87D5
#define GL_Y_EXT 0x87D6
#define GL_Z_EXT 0x87D7
#define GL_W_EXT 0x87D8
#define GL_NEGATIVE_X_EXT 0x87D9
#define GL_NEGATIVE_Y_EXT 0x87DA
#define GL_NEGATIVE_Z_EXT 0x87DB
#define GL_NEGATIVE_W_EXT 0x87DC
#define GL_ZERO_EXT 0x87DD
#define GL_ONE_EXT 0x87DE
#define GL_NEGATIVE_ONE_EXT 0x87DF
#define GL_NORMALIZED_RANGE_EXT 0x87E0
#define GL_FULL_RANGE_EXT 0x87E1
#define GL_CURRENT_VERTEX_EXT 0x87E2
#define GL_MVP_MATRIX_EXT 0x87E3
#define GL_VARIANT_VALUE_EXT 0x87E4
#define GL_VARIANT_DATATYPE_EXT 0x87E5
#define GL_VARIANT_ARRAY_STRIDE_EXT 0x87E6
#define GL_VARIANT_ARRAY_TYPE_EXT 0x87E7
#define GL_VARIANT_ARRAY_EXT 0x87E8
#define GL_VARIANT_ARRAY_POINTER_EXT 0x87E9
#define GL_INVARIANT_VALUE_EXT 0x87EA
#define GL_INVARIANT_DATATYPE_EXT 0x87EB
#define GL_LOCAL_CONSTANT_VALUE_EXT 0x87EC
#define GL_LOCAL_CONSTANT_DATATYPE_EXT 0x87ED

typedef void (GLAPIENTRY * PFNGLBEGINVERTEXSHADEREXTPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLBINDLIGHTPARAMETEREXTPROC) (GLenum light, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDMATERIALPARAMETEREXTPROC) (GLenum face, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDPARAMETEREXTPROC) (GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDTEXGENPARAMETEREXTPROC) (GLenum unit, GLenum coord, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDTEXTUREUNITPARAMETEREXTPROC) (GLenum unit, GLenum value);
typedef void (GLAPIENTRY * PFNGLBINDVERTEXSHADEREXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXSHADEREXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENDVERTEXSHADEREXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLEXTRACTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLuint (GLAPIENTRY * PFNGLGENSYMBOLSEXTPROC) (GLenum dataType, GLenum storageType, GLenum range, GLuint components);
typedef GLuint (GLAPIENTRY * PFNGLGENVERTEXSHADERSEXTPROC) (GLuint range);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTPOINTERVEXTPROC) (GLuint id, GLenum value, GLvoid **data);
typedef void (GLAPIENTRY * PFNGLINSERTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLboolean (GLAPIENTRY * PFNGLISVARIANTENABLEDEXTPROC) (GLuint id, GLenum cap);
typedef void (GLAPIENTRY * PFNGLSETINVARIANTEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void (GLAPIENTRY * PFNGLSETLOCALCONSTANTEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void (GLAPIENTRY * PFNGLSHADEROP1EXTPROC) (GLenum op, GLuint res, GLuint arg1);
typedef void (GLAPIENTRY * PFNGLSHADEROP2EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
typedef void (GLAPIENTRY * PFNGLSHADEROP3EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
typedef void (GLAPIENTRY * PFNGLSWIZZLEEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void (GLAPIENTRY * PFNGLVARIANTPOINTEREXTPROC) (GLuint id, GLenum type, GLuint stride, GLvoid *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTBVEXTPROC) (GLuint id, GLbyte *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTDVEXTPROC) (GLuint id, GLdouble *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTFVEXTPROC) (GLuint id, GLfloat *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTIVEXTPROC) (GLuint id, GLint *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTSVEXTPROC) (GLuint id, GLshort *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUBVEXTPROC) (GLuint id, GLubyte *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUIVEXTPROC) (GLuint id, GLuint *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUSVEXTPROC) (GLuint id, GLushort *addr);
typedef void (GLAPIENTRY * PFNGLWRITEMASKEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);


#define GLEW_EXT_vertex_shader GLEW_GET_VAR(__GLEW_EXT_vertex_shader)

#endif /* GL_EXT_vertex_shader */

/* ------------------------ GL_EXT_vertex_weighting ------------------------ */

#ifndef GL_EXT_vertex_weighting
#define GL_EXT_vertex_weighting 1

#define GL_MODELVIEW0_STACK_DEPTH_EXT 0x0BA3
#define GL_MODELVIEW0_MATRIX_EXT 0x0BA6
#define GL_MODELVIEW0_EXT 0x1700
#define GL_MODELVIEW1_STACK_DEPTH_EXT 0x8502
#define GL_MODELVIEW1_MATRIX_EXT 0x8506
#define GL_VERTEX_WEIGHTING_EXT 0x8509
#define GL_MODELVIEW1_EXT 0x850A
#define GL_CURRENT_VERTEX_WEIGHT_EXT 0x850B
#define GL_VERTEX_WEIGHT_ARRAY_EXT 0x850C
#define GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT 0x850D
#define GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT 0x850E
#define GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT 0x850F
#define GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT 0x8510

typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTFEXTPROC) (GLfloat weight);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTFVEXTPROC) (GLfloat* weight);


#define GLEW_EXT_vertex_weighting GLEW_GET_VAR(__GLEW_EXT_vertex_weighting)

#endif /* GL_EXT_vertex_weighting */

/* ------------------------- GL_EXT_x11_sync_object ------------------------ */

#ifndef GL_EXT_x11_sync_object
#define GL_EXT_x11_sync_object 1

#define GL_SYNC_X11_FENCE_EXT 0x90E1

typedef GLsync (GLAPIENTRY * PFNGLIMPORTSYNCEXTPROC) (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);


#define GLEW_EXT_x11_sync_object GLEW_GET_VAR(__GLEW_EXT_x11_sync_object)

#endif /* GL_EXT_x11_sync_object */

/* ---------------------- GL_GREMEDY_frame_terminator ---------------------- */

#ifndef GL_GREMEDY_frame_terminator
#define GL_GREMEDY_frame_terminator 1

typedef void (GLAPIENTRY * PFNGLFRAMETERMINATORGREMEDYPROC) (void);


#define GLEW_GREMEDY_frame_terminator GLEW_GET_VAR(__GLEW_GREMEDY_frame_terminator)

#endif /* GL_GREMEDY_frame_terminator */

/* ------------------------ GL_GREMEDY_string_marker ----------------------- */

#ifndef GL_GREMEDY_string_marker
#define GL_GREMEDY_string_marker 1

typedef void (GLAPIENTRY * PFNGLSTRINGMARKERGREMEDYPROC) (GLsizei len, const void* string);


#define GLEW_GREMEDY_string_marker GLEW_GET_VAR(__GLEW_GREMEDY_string_marker)

#endif /* GL_GREMEDY_string_marker */

/* --------------------- GL_HP_convolution_border_modes -------------------- */

#ifndef GL_HP_convolution_border_modes
#define GL_HP_convolution_border_modes 1

#define GLEW_HP_convolution_border_modes GLEW_GET_VAR(__GLEW_HP_convolution_border_modes)

#endif /* GL_HP_convolution_border_modes */

/* ------------------------- GL_HP_image_transform ------------------------- */

#ifndef GL_HP_image_transform
#define GL_HP_image_transform 1

typedef void (GLAPIENTRY * PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERFHPPROC) (GLenum target, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERFVHPPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERIHPPROC) (GLenum target, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERIVHPPROC) (GLenum target, GLenum pname, const GLint* params);


#define GLEW_HP_image_transform GLEW_GET_VAR(__GLEW_HP_image_transform)

#endif /* GL_HP_image_transform */

/* -------------------------- GL_HP_occlusion_test ------------------------- */

#ifndef GL_HP_occlusion_test
#define GL_HP_occlusion_test 1

#define GL_OCCLUSION_TEST_HP 0x8165
#define GL_OCCLUSION_TEST_RESULT_HP 0x8166

#define GLEW_HP_occlusion_test GLEW_GET_VAR(__GLEW_HP_occlusion_test)

#endif /* GL_HP_occlusion_test */

/* ------------------------- GL_HP_texture_lighting ------------------------ */

#ifndef GL_HP_texture_lighting
#define GL_HP_texture_lighting 1

#define GLEW_HP_texture_lighting GLEW_GET_VAR(__GLEW_HP_texture_lighting)

#endif /* GL_HP_texture_lighting */

/* --------------------------- GL_IBM_cull_vertex -------------------------- */

#ifndef GL_IBM_cull_vertex
#define GL_IBM_cull_vertex 1

#define GL_CULL_VERTEX_IBM 103050

#define GLEW_IBM_cull_vertex GLEW_GET_VAR(__GLEW_IBM_cull_vertex)

#endif /* GL_IBM_cull_vertex */

/* ---------------------- GL_IBM_multimode_draw_arrays --------------------- */

#ifndef GL_IBM_multimode_draw_arrays
#define GL_IBM_multimode_draw_arrays 1

typedef void (GLAPIENTRY * PFNGLMULTIMODEDRAWARRAYSIBMPROC) (const GLenum* mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
typedef void (GLAPIENTRY * PFNGLMULTIMODEDRAWELEMENTSIBMPROC) (const GLenum* mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, GLint modestride);


#define GLEW_IBM_multimode_draw_arrays GLEW_GET_VAR(__GLEW_IBM_multimode_draw_arrays)

#endif /* GL_IBM_multimode_draw_arrays */

/* ------------------------- GL_IBM_rasterpos_clip ------------------------- */

#ifndef GL_IBM_rasterpos_clip
#define GL_IBM_rasterpos_clip 1

#define GL_RASTER_POSITION_UNCLIPPED_IBM 103010

#define GLEW_IBM_rasterpos_clip GLEW_GET_VAR(__GLEW_IBM_rasterpos_clip)

#endif /* GL_IBM_rasterpos_clip */

/* --------------------------- GL_IBM_static_data -------------------------- */

#ifndef GL_IBM_static_data
#define GL_IBM_static_data 1

#define GL_ALL_STATIC_DATA_IBM 103060
#define GL_STATIC_VERTEX_ARRAY_IBM 103061

#define GLEW_IBM_static_data GLEW_GET_VAR(__GLEW_IBM_static_data)

#endif /* GL_IBM_static_data */

/* --------------------- GL_IBM_texture_mirrored_repeat -------------------- */

#ifndef GL_IBM_texture_mirrored_repeat
#define GL_IBM_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_IBM 0x8370

#define GLEW_IBM_texture_mirrored_repeat GLEW_GET_VAR(__GLEW_IBM_texture_mirrored_repeat)

#endif /* GL_IBM_texture_mirrored_repeat */

/* ----------------------- GL_IBM_vertex_array_lists ----------------------- */

#ifndef GL_IBM_vertex_array_lists
#define GL_IBM_vertex_array_lists 1

#define GL_VERTEX_ARRAY_LIST_IBM 103070
#define GL_NORMAL_ARRAY_LIST_IBM 103071
#define GL_COLOR_ARRAY_LIST_IBM 103072
#define GL_INDEX_ARRAY_LIST_IBM 103073
#define GL_TEXTURE_COORD_ARRAY_LIST_IBM 103074
#define GL_EDGE_FLAG_ARRAY_LIST_IBM 103075
#define GL_FOG_COORDINATE_ARRAY_LIST_IBM 103076
#define GL_SECONDARY_COLOR_ARRAY_LIST_IBM 103077
#define GL_VERTEX_ARRAY_LIST_STRIDE_IBM 103080
#define GL_NORMAL_ARRAY_LIST_STRIDE_IBM 103081
#define GL_COLOR_ARRAY_LIST_STRIDE_IBM 103082
#define GL_INDEX_ARRAY_LIST_STRIDE_IBM 103083
#define GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM 103084
#define GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM 103085
#define GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM 103086
#define GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM 103087

typedef void (GLAPIENTRY * PFNGLCOLORPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPOINTERLISTIBMPROC) (GLint stride, const GLboolean ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLINDEXPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);


#define GLEW_IBM_vertex_array_lists GLEW_GET_VAR(__GLEW_IBM_vertex_array_lists)

#endif /* GL_IBM_vertex_array_lists */

/* -------------------------- GL_INGR_color_clamp -------------------------- */

#ifndef GL_INGR_color_clamp
#define GL_INGR_color_clamp 1

#define GL_RED_MIN_CLAMP_INGR 0x8560
#define GL_GREEN_MIN_CLAMP_INGR 0x8561
#define GL_BLUE_MIN_CLAMP_INGR 0x8562
#define GL_ALPHA_MIN_CLAMP_INGR 0x8563
#define GL_RED_MAX_CLAMP_INGR 0x8564
#define GL_GREEN_MAX_CLAMP_INGR 0x8565
#define GL_BLUE_MAX_CLAMP_INGR 0x8566
#define GL_ALPHA_MAX_CLAMP_INGR 0x8567

#define GLEW_INGR_color_clamp GLEW_GET_VAR(__GLEW_INGR_color_clamp)

#endif /* GL_INGR_color_clamp */

/* ------------------------- GL_INGR_interlace_read ------------------------ */

#ifndef GL_INGR_interlace_read
#define GL_INGR_interlace_read 1

#define GL_INTERLACE_READ_INGR 0x8568

#define GLEW_INGR_interlace_read GLEW_GET_VAR(__GLEW_INGR_interlace_read)

#endif /* GL_INGR_interlace_read */

/* ------------------------ GL_INTEL_parallel_arrays ----------------------- */

#ifndef GL_INTEL_parallel_arrays
#define GL_INTEL_parallel_arrays 1

#define GL_PARALLEL_ARRAYS_INTEL 0x83F4
#define GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL 0x83F5
#define GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL 0x83F6
#define GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL 0x83F7
#define GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL 0x83F8

typedef void (GLAPIENTRY * PFNGLCOLORPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTERVINTELPROC) (GLenum type, const void** pointer);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);


#define GLEW_INTEL_parallel_arrays GLEW_GET_VAR(__GLEW_INTEL_parallel_arrays)

#endif /* GL_INTEL_parallel_arrays */

/* ------------------------ GL_INTEL_texture_scissor ----------------------- */

#ifndef GL_INTEL_texture_scissor
#define GL_INTEL_texture_scissor 1

typedef void (GLAPIENTRY * PFNGLTEXSCISSORFUNCINTELPROC) (GLenum target, GLenum lfunc, GLenum hfunc);
typedef void (GLAPIENTRY * PFNGLTEXSCISSORINTELPROC) (GLenum target, GLclampf tlow, GLclampf thigh);


#define GLEW_INTEL_texture_scissor GLEW_GET_VAR(__GLEW_INTEL_texture_scissor)

#endif /* GL_INTEL_texture_scissor */

/* ------------------------------ GL_KHR_debug ----------------------------- */

#ifndef GL_KHR_debug
#define GL_KHR_debug 1

#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_DISPLAY_LIST 0x82E7
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_OUTPUT 0x92E0

typedef void (APIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, void* userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECONTROLPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGPROC) (GLuint count, GLsizei bufsize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
typedef void (GLAPIENTRY * PFNGLGETOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar *label);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPTRLABELPROC) (void* ptr, GLsizei bufSize, GLsizei* length, GLchar *label);
typedef void (GLAPIENTRY * PFNGLGETPOINTERVPROC) (GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
typedef void (GLAPIENTRY * PFNGLOBJECTPTRLABELPROC) (void* ptr, GLsizei length, const GLchar* label);
typedef void (GLAPIENTRY * PFNGLPUSHDEBUGGROUPPROC) (GLenum source, GLuint id, GLsizei length, const GLchar * message);


#define GLEW_KHR_debug GLEW_GET_VAR(__GLEW_KHR_debug)

#endif /* GL_KHR_debug */

/* ------------------ GL_KHR_texture_compression_astc_ldr ------------------ */

#ifndef GL_KHR_texture_compression_astc_ldr
#define GL_KHR_texture_compression_astc_ldr 1

#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR 0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR 0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR 0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR 0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR 0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR 0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93B7
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR 0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR 0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR 0x93BA
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD

#define GLEW_KHR_texture_compression_astc_ldr GLEW_GET_VAR(__GLEW_KHR_texture_compression_astc_ldr)

#endif /* GL_KHR_texture_compression_astc_ldr */

/* -------------------------- GL_KTX_buffer_region ------------------------- */

#ifndef GL_KTX_buffer_region
#define GL_KTX_buffer_region 1

#define GL_KTX_FRONT_REGION 0x0
#define GL_KTX_BACK_REGION 0x1
#define GL_KTX_Z_REGION 0x2
#define GL_KTX_STENCIL_REGION 0x3

typedef GLuint (GLAPIENTRY * PFNGLBUFFERREGIONENABLEDPROC) (void);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERREGIONPROC) (GLenum region);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERREGIONPROC) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
typedef GLuint (GLAPIENTRY * PFNGLNEWBUFFERREGIONPROC) (GLenum region);
typedef void (GLAPIENTRY * PFNGLREADBUFFERREGIONPROC) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);


#define GLEW_KTX_buffer_region GLEW_GET_VAR(__GLEW_KTX_buffer_region)

#endif /* GL_KTX_buffer_region */

/* ------------------------- GL_MESAX_texture_stack ------------------------ */

#ifndef GL_MESAX_texture_stack
#define GL_MESAX_texture_stack 1

#define GL_TEXTURE_1D_STACK_MESAX 0x8759
#define GL_TEXTURE_2D_STACK_MESAX 0x875A
#define GL_PROXY_TEXTURE_1D_STACK_MESAX 0x875B
#define GL_PROXY_TEXTURE_2D_STACK_MESAX 0x875C
#define GL_TEXTURE_1D_STACK_BINDING_MESAX 0x875D
#define GL_TEXTURE_2D_STACK_BINDING_MESAX 0x875E

#define GLEW_MESAX_texture_stack GLEW_GET_VAR(__GLEW_MESAX_texture_stack)

#endif /* GL_MESAX_texture_stack */

/* -------------------------- GL_MESA_pack_invert -------------------------- */

#ifndef GL_MESA_pack_invert
#define GL_MESA_pack_invert 1

#define GL_PACK_INVERT_MESA 0x8758

#define GLEW_MESA_pack_invert GLEW_GET_VAR(__GLEW_MESA_pack_invert)

#endif /* GL_MESA_pack_invert */

/* ------------------------- GL_MESA_resize_buffers ------------------------ */

#ifndef GL_MESA_resize_buffers
#define GL_MESA_resize_buffers 1

typedef void (GLAPIENTRY * PFNGLRESIZEBUFFERSMESAPROC) (void);


#define GLEW_MESA_resize_buffers GLEW_GET_VAR(__GLEW_MESA_resize_buffers)

#endif /* GL_MESA_resize_buffers */

/* --------------------------- GL_MESA_window_pos -------------------------- */

#ifndef GL_MESA_window_pos
#define GL_MESA_window_pos 1

typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DMESAPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVMESAPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FMESAPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVMESAPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IMESAPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVMESAPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SMESAPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVMESAPROC) (const GLshort* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DMESAPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVMESAPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FMESAPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVMESAPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IMESAPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVMESAPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SMESAPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVMESAPROC) (const GLshort* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4DMESAPROC) (GLdouble x, GLdouble y, GLdouble z, GLdouble);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4DVMESAPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4FMESAPROC) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4FVMESAPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4IMESAPROC) (GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4IVMESAPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4SMESAPROC) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4SVMESAPROC) (const GLshort* p);


#define GLEW_MESA_window_pos GLEW_GET_VAR(__GLEW_MESA_window_pos)

#endif /* GL_MESA_window_pos */

/* ------------------------- GL_MESA_ycbcr_texture ------------------------- */

#ifndef GL_MESA_ycbcr_texture
#define GL_MESA_ycbcr_texture 1

#define GL_UNSIGNED_SHORT_8_8_MESA 0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_MESA 0x85BB
#define GL_YCBCR_MESA 0x8757

#define GLEW_MESA_ycbcr_texture GLEW_GET_VAR(__GLEW_MESA_ycbcr_texture)

#endif /* GL_MESA_ycbcr_texture */

/* ------------------------- GL_NVX_gpu_memory_info ------------------------ */

#ifndef GL_NVX_gpu_memory_info
#define GL_NVX_gpu_memory_info 1

#define GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX 0x9047
#define GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX 0x9048
#define GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#define GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX 0x904A
#define GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX 0x904B

#define GLEW_NVX_gpu_memory_info GLEW_GET_VAR(__GLEW_NVX_gpu_memory_info)

#endif /* GL_NVX_gpu_memory_info */

/* ------------------------- GL_NV_bindless_texture ------------------------ */

#ifndef GL_NV_bindless_texture
#define GL_NV_bindless_texture 1

typedef GLuint64 (GLAPIENTRY * PFNGLGETIMAGEHANDLENVPROC) (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
typedef GLuint64 (GLAPIENTRY * PFNGLGETTEXTUREHANDLENVPROC) (GLuint texture);
typedef GLuint64 (GLAPIENTRY * PFNGLGETTEXTURESAMPLERHANDLENVPROC) (GLuint texture, GLuint sampler);
typedef GLboolean (GLAPIENTRY * PFNGLISIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKEIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle, GLenum access);
typedef void (GLAPIENTRY * PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKETEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC) (GLuint program, GLint location, GLuint64 value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* values);
typedef void (GLAPIENTRY * PFNGLUNIFORMHANDLEUI64NVPROC) (GLint location, GLuint64 value);
typedef void (GLAPIENTRY * PFNGLUNIFORMHANDLEUI64VNVPROC) (GLint location, GLsizei count, const GLuint64* value);


#define GLEW_NV_bindless_texture GLEW_GET_VAR(__GLEW_NV_bindless_texture)

#endif /* GL_NV_bindless_texture */

/* --------------------------- GL_NV_blend_square -------------------------- */

#ifndef GL_NV_blend_square
#define GL_NV_blend_square 1

#define GLEW_NV_blend_square GLEW_GET_VAR(__GLEW_NV_blend_square)

#endif /* GL_NV_blend_square */

/* ------------------------ GL_NV_conditional_render ----------------------- */

#ifndef GL_NV_conditional_render
#define GL_NV_conditional_render 1

#define GL_QUERY_WAIT_NV 0x8E13
#define GL_QUERY_NO_WAIT_NV 0x8E14
#define GL_QUERY_BY_REGION_WAIT_NV 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT_NV 0x8E16

typedef void (GLAPIENTRY * PFNGLBEGINCONDITIONALRENDERNVPROC) (GLuint id, GLenum mode);
typedef void (GLAPIENTRY * PFNGLENDCONDITIONALRENDERNVPROC) (void);


#define GLEW_NV_conditional_render GLEW_GET_VAR(__GLEW_NV_conditional_render)

#endif /* GL_NV_conditional_render */

/* ----------------------- GL_NV_copy_depth_to_color ----------------------- */

#ifndef GL_NV_copy_depth_to_color
#define GL_NV_copy_depth_to_color 1

#define GL_DEPTH_STENCIL_TO_RGBA_NV 0x886E
#define GL_DEPTH_STENCIL_TO_BGRA_NV 0x886F

#define GLEW_NV_copy_depth_to_color GLEW_GET_VAR(__GLEW_NV_copy_depth_to_color)

#endif /* GL_NV_copy_depth_to_color */

/* ---------------------------- GL_NV_copy_image --------------------------- */

#ifndef GL_NV_copy_image
#define GL_NV_copy_image 1

typedef void (GLAPIENTRY * PFNGLCOPYIMAGESUBDATANVPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);


#define GLEW_NV_copy_image GLEW_GET_VAR(__GLEW_NV_copy_image)

#endif /* GL_NV_copy_image */

/* ------------------------ GL_NV_depth_buffer_float ----------------------- */

#ifndef GL_NV_depth_buffer_float
#define GL_NV_depth_buffer_float 1

#define GL_DEPTH_COMPONENT32F_NV 0x8DAB
#define GL_DEPTH32F_STENCIL8_NV 0x8DAC
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV 0x8DAD
#define GL_DEPTH_BUFFER_FLOAT_MODE_NV 0x8DAF

typedef void (GLAPIENTRY * PFNGLCLEARDEPTHDNVPROC) (GLdouble depth);
typedef void (GLAPIENTRY * PFNGLDEPTHBOUNDSDNVPROC) (GLdouble zmin, GLdouble zmax);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEDNVPROC) (GLdouble zNear, GLdouble zFar);


#define GLEW_NV_depth_buffer_float GLEW_GET_VAR(__GLEW_NV_depth_buffer_float)

#endif /* GL_NV_depth_buffer_float */

/* --------------------------- GL_NV_depth_clamp --------------------------- */

#ifndef GL_NV_depth_clamp
#define GL_NV_depth_clamp 1

#define GL_DEPTH_CLAMP_NV 0x864F

#define GLEW_NV_depth_clamp GLEW_GET_VAR(__GLEW_NV_depth_clamp)

#endif /* GL_NV_depth_clamp */

/* ---------------------- GL_NV_depth_range_unclamped ---------------------- */

#ifndef GL_NV_depth_range_unclamped
#define GL_NV_depth_range_unclamped 1

#define GL_SAMPLE_COUNT_BITS_NV 0x8864
#define GL_CURRENT_SAMPLE_COUNT_QUERY_NV 0x8865
#define GL_QUERY_RESULT_NV 0x8866
#define GL_QUERY_RESULT_AVAILABLE_NV 0x8867
#define GL_SAMPLE_COUNT_NV 0x8914

#define GLEW_NV_depth_range_unclamped GLEW_GET_VAR(__GLEW_NV_depth_range_unclamped)

#endif /* GL_NV_depth_range_unclamped */

/* ---------------------------- GL_NV_evaluators --------------------------- */

#ifndef GL_NV_evaluators
#define GL_NV_evaluators 1

#define GL_EVAL_2D_NV 0x86C0
#define GL_EVAL_TRIANGULAR_2D_NV 0x86C1
#define GL_MAP_TESSELLATION_NV 0x86C2
#define GL_MAP_ATTRIB_U_ORDER_NV 0x86C3
#define GL_MAP_ATTRIB_V_ORDER_NV 0x86C4
#define GL_EVAL_FRACTIONAL_TESSELLATION_NV 0x86C5
#define GL_EVAL_VERTEX_ATTRIB0_NV 0x86C6
#define GL_EVAL_VERTEX_ATTRIB1_NV 0x86C7
#define GL_EVAL_VERTEX_ATTRIB2_NV 0x86C8
#define GL_EVAL_VERTEX_ATTRIB3_NV 0x86C9
#define GL_EVAL_VERTEX_ATTRIB4_NV 0x86CA
#define GL_EVAL_VERTEX_ATTRIB5_NV 0x86CB
#define GL_EVAL_VERTEX_ATTRIB6_NV 0x86CC
#define GL_EVAL_VERTEX_ATTRIB7_NV 0x86CD
#define GL_EVAL_VERTEX_ATTRIB8_NV 0x86CE
#define GL_EVAL_VERTEX_ATTRIB9_NV 0x86CF
#define GL_EVAL_VERTEX_ATTRIB10_NV 0x86D0
#define GL_EVAL_VERTEX_ATTRIB11_NV 0x86D1
#define GL_EVAL_VERTEX_ATTRIB12_NV 0x86D2
#define GL_EVAL_VERTEX_ATTRIB13_NV 0x86D3
#define GL_EVAL_VERTEX_ATTRIB14_NV 0x86D4
#define GL_EVAL_VERTEX_ATTRIB15_NV 0x86D5
#define GL_MAX_MAP_TESSELLATION_NV 0x86D6
#define GL_MAX_RATIONAL_EVAL_ORDER_NV 0x86D7

typedef void (GLAPIENTRY * PFNGLEVALMAPSNVPROC) (GLenum target, GLenum mode);
typedef void (GLAPIENTRY * PFNGLGETMAPATTRIBPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMAPATTRIBPARAMETERIVNVPROC) (GLenum target, GLuint index, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void* points);
typedef void (GLAPIENTRY * PFNGLGETMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void* points);
typedef void (GLAPIENTRY * PFNGLMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, const GLint* params);


#define GLEW_NV_evaluators GLEW_GET_VAR(__GLEW_NV_evaluators)

#endif /* GL_NV_evaluators */

/* ----------------------- GL_NV_explicit_multisample ---------------------- */

#ifndef GL_NV_explicit_multisample
#define GL_NV_explicit_multisample 1

#define GL_SAMPLE_POSITION_NV 0x8E50
#define GL_SAMPLE_MASK_NV 0x8E51
#define GL_SAMPLE_MASK_VALUE_NV 0x8E52
#define GL_TEXTURE_BINDING_RENDERBUFFER_NV 0x8E53
#define GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV 0x8E54
#define GL_TEXTURE_RENDERBUFFER_NV 0x8E55
#define GL_SAMPLER_RENDERBUFFER_NV 0x8E56
#define GL_INT_SAMPLER_RENDERBUFFER_NV 0x8E57
#define GL_UNSIGNED_INT_SAMPLER_RENDERBUFFER_NV 0x8E58
#define GL_MAX_SAMPLE_MASK_WORDS_NV 0x8E59

typedef void (GLAPIENTRY * PFNGLGETMULTISAMPLEFVNVPROC) (GLenum pname, GLuint index, GLfloat* val);
typedef void (GLAPIENTRY * PFNGLSAMPLEMASKINDEXEDNVPROC) (GLuint index, GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXRENDERBUFFERNVPROC) (GLenum target, GLuint renderbuffer);


#define GLEW_NV_explicit_multisample GLEW_GET_VAR(__GLEW_NV_explicit_multisample)

#endif /* GL_NV_explicit_multisample */

/* ------------------------------ GL_NV_fence ------------------------------ */

#ifndef GL_NV_fence
#define GL_NV_fence 1

#define GL_ALL_COMPLETED_NV 0x84F2
#define GL_FENCE_STATUS_NV 0x84F3
#define GL_FENCE_CONDITION_NV 0x84F4

typedef void (GLAPIENTRY * PFNGLDELETEFENCESNVPROC) (GLsizei n, const GLuint* fences);
typedef void (GLAPIENTRY * PFNGLFINISHFENCENVPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLGENFENCESNVPROC) (GLsizei n, GLuint* fences);
typedef void (GLAPIENTRY * PFNGLGETFENCEIVNVPROC) (GLuint fence, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFENCENVPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLSETFENCENVPROC) (GLuint fence, GLenum condition);
typedef GLboolean (GLAPIENTRY * PFNGLTESTFENCENVPROC) (GLuint fence);


#define GLEW_NV_fence GLEW_GET_VAR(__GLEW_NV_fence)

#endif /* GL_NV_fence */

/* --------------------------- GL_NV_float_buffer -------------------------- */

#ifndef GL_NV_float_buffer
#define GL_NV_float_buffer 1

#define GL_FLOAT_R_NV 0x8880
#define GL_FLOAT_RG_NV 0x8881
#define GL_FLOAT_RGB_NV 0x8882
#define GL_FLOAT_RGBA_NV 0x8883
#define GL_FLOAT_R16_NV 0x8884
#define GL_FLOAT_R32_NV 0x8885
#define GL_FLOAT_RG16_NV 0x8886
#define GL_FLOAT_RG32_NV 0x8887
#define GL_FLOAT_RGB16_NV 0x8888
#define GL_FLOAT_RGB32_NV 0x8889
#define GL_FLOAT_RGBA16_NV 0x888A
#define GL_FLOAT_RGBA32_NV 0x888B
#define GL_TEXTURE_FLOAT_COMPONENTS_NV 0x888C
#define GL_FLOAT_CLEAR_COLOR_VALUE_NV 0x888D
#define GL_FLOAT_RGBA_MODE_NV 0x888E

#define GLEW_NV_float_buffer GLEW_GET_VAR(__GLEW_NV_float_buffer)

#endif /* GL_NV_float_buffer */

/* --------------------------- GL_NV_fog_distance -------------------------- */

#ifndef GL_NV_fog_distance
#define GL_NV_fog_distance 1

#define GL_FOG_DISTANCE_MODE_NV 0x855A
#define GL_EYE_RADIAL_NV 0x855B
#define GL_EYE_PLANE_ABSOLUTE_NV 0x855C

#define GLEW_NV_fog_distance GLEW_GET_VAR(__GLEW_NV_fog_distance)

#endif /* GL_NV_fog_distance */

/* ------------------------- GL_NV_fragment_program ------------------------ */

#ifndef GL_NV_fragment_program
#define GL_NV_fragment_program 1

#define GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV 0x8868
#define GL_FRAGMENT_PROGRAM_NV 0x8870
#define GL_MAX_TEXTURE_COORDS_NV 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_NV 0x8872
#define GL_FRAGMENT_PROGRAM_BINDING_NV 0x8873
#define GL_PROGRAM_ERROR_STRING_NV 0x8874

typedef void (GLAPIENTRY * PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLdouble *params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4DNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, const GLdouble v[]);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4FNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, const GLfloat v[]);


#define GLEW_NV_fragment_program GLEW_GET_VAR(__GLEW_NV_fragment_program)

#endif /* GL_NV_fragment_program */

/* ------------------------ GL_NV_fragment_program2 ------------------------ */

#ifndef GL_NV_fragment_program2
#define GL_NV_fragment_program2 1

#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV 0x88F5
#define GL_MAX_PROGRAM_IF_DEPTH_NV 0x88F6
#define GL_MAX_PROGRAM_LOOP_DEPTH_NV 0x88F7
#define GL_MAX_PROGRAM_LOOP_COUNT_NV 0x88F8

#define GLEW_NV_fragment_program2 GLEW_GET_VAR(__GLEW_NV_fragment_program2)

#endif /* GL_NV_fragment_program2 */

/* ------------------------ GL_NV_fragment_program4 ------------------------ */

#ifndef GL_NV_fragment_program4
#define GL_NV_fragment_program4 1

#define GLEW_NV_fragment_program4 GLEW_GET_VAR(__GLEW_NV_fragment_program4)

#endif /* GL_NV_fragment_program4 */

/* --------------------- GL_NV_fragment_program_option --------------------- */

#ifndef GL_NV_fragment_program_option
#define GL_NV_fragment_program_option 1

#define GLEW_NV_fragment_program_option GLEW_GET_VAR(__GLEW_NV_fragment_program_option)

#endif /* GL_NV_fragment_program_option */

/* ----------------- GL_NV_framebuffer_multisample_coverage ---------------- */

#ifndef GL_NV_framebuffer_multisample_coverage
#define GL_NV_framebuffer_multisample_coverage 1

#define GL_RENDERBUFFER_COVERAGE_SAMPLES_NV 0x8CAB
#define GL_RENDERBUFFER_COLOR_SAMPLES_NV 0x8E10
#define GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV 0x8E11
#define GL_MULTISAMPLE_COVERAGE_MODES_NV 0x8E12

typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);


#define GLEW_NV_framebuffer_multisample_coverage GLEW_GET_VAR(__GLEW_NV_framebuffer_multisample_coverage)

#endif /* GL_NV_framebuffer_multisample_coverage */

/* ------------------------ GL_NV_geometry_program4 ------------------------ */

#ifndef GL_NV_geometry_program4
#define GL_NV_geometry_program4 1

#define GL_GEOMETRY_PROGRAM_NV 0x8C26
#define GL_MAX_PROGRAM_OUTPUT_VERTICES_NV 0x8C27
#define GL_MAX_PROGRAM_TOTAL_OUTPUT_COMPONENTS_NV 0x8C28

typedef void (GLAPIENTRY * PFNGLPROGRAMVERTEXLIMITNVPROC) (GLenum target, GLint limit);


#define GLEW_NV_geometry_program4 GLEW_GET_VAR(__GLEW_NV_geometry_program4)

#endif /* GL_NV_geometry_program4 */

/* ------------------------- GL_NV_geometry_shader4 ------------------------ */

#ifndef GL_NV_geometry_shader4
#define GL_NV_geometry_shader4 1

#define GLEW_NV_geometry_shader4 GLEW_GET_VAR(__GLEW_NV_geometry_shader4)

#endif /* GL_NV_geometry_shader4 */

/* --------------------------- GL_NV_gpu_program4 -------------------------- */

#ifndef GL_NV_gpu_program4
#define GL_NV_gpu_program4 1

#define GL_MIN_PROGRAM_TEXEL_OFFSET_NV 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET_NV 0x8905
#define GL_PROGRAM_ATTRIB_COMPONENTS_NV 0x8906
#define GL_PROGRAM_RESULT_COMPONENTS_NV 0x8907
#define GL_MAX_PROGRAM_ATTRIB_COMPONENTS_NV 0x8908
#define GL_MAX_PROGRAM_RESULT_COMPONENTS_NV 0x8909
#define GL_MAX_PROGRAM_GENERIC_ATTRIBS_NV 0x8DA5
#define GL_MAX_PROGRAM_GENERIC_RESULTS_NV 0x8DA6

typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);


#define GLEW_NV_gpu_program4 GLEW_GET_VAR(__GLEW_NV_gpu_program4)

#endif /* GL_NV_gpu_program4 */

/* --------------------------- GL_NV_gpu_program5 -------------------------- */

#ifndef GL_NV_gpu_program5
#define GL_NV_gpu_program5 1

#define GL_MAX_GEOMETRY_PROGRAM_INVOCATIONS_NV 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET_NV 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET_NV 0x8E5C
#define GL_FRAGMENT_PROGRAM_INTERPOLATION_OFFSET_BITS_NV 0x8E5D
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_NV 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_NV 0x8E5F

#define GLEW_NV_gpu_program5 GLEW_GET_VAR(__GLEW_NV_gpu_program5)

#endif /* GL_NV_gpu_program5 */

/* ------------------------- GL_NV_gpu_program_fp64 ------------------------ */

#ifndef GL_NV_gpu_program_fp64
#define GL_NV_gpu_program_fp64 1

#define GLEW_NV_gpu_program_fp64 GLEW_GET_VAR(__GLEW_NV_gpu_program_fp64)

#endif /* GL_NV_gpu_program_fp64 */

/* --------------------------- GL_NV_gpu_shader5 --------------------------- */

#ifndef GL_NV_gpu_shader5
#define GL_NV_gpu_shader5 1

#define GL_INT64_NV 0x140E
#define GL_UNSIGNED_INT64_NV 0x140F
#define GL_INT8_NV 0x8FE0
#define GL_INT8_VEC2_NV 0x8FE1
#define GL_INT8_VEC3_NV 0x8FE2
#define GL_INT8_VEC4_NV 0x8FE3
#define GL_INT16_NV 0x8FE4
#define GL_INT16_VEC2_NV 0x8FE5
#define GL_INT16_VEC3_NV 0x8FE6
#define GL_INT16_VEC4_NV 0x8FE7
#define GL_INT64_VEC2_NV 0x8FE9
#define GL_INT64_VEC3_NV 0x8FEA
#define GL_INT64_VEC4_NV 0x8FEB
#define GL_UNSIGNED_INT8_NV 0x8FEC
#define GL_UNSIGNED_INT8_VEC2_NV 0x8FED
#define GL_UNSIGNED_INT8_VEC3_NV 0x8FEE
#define GL_UNSIGNED_INT8_VEC4_NV 0x8FEF
#define GL_UNSIGNED_INT16_NV 0x8FF0
#define GL_UNSIGNED_INT16_VEC2_NV 0x8FF1
#define GL_UNSIGNED_INT16_VEC3_NV 0x8FF2
#define GL_UNSIGNED_INT16_VEC4_NV 0x8FF3
#define GL_UNSIGNED_INT64_VEC2_NV 0x8FF5
#define GL_UNSIGNED_INT64_VEC3_NV 0x8FF6
#define GL_UNSIGNED_INT64_VEC4_NV 0x8FF7
#define GL_FLOAT16_NV 0x8FF8
#define GL_FLOAT16_VEC2_NV 0x8FF9
#define GL_FLOAT16_VEC3_NV 0x8FFA
#define GL_FLOAT16_VEC4_NV 0x8FFB

typedef void (GLAPIENTRY * PFNGLGETUNIFORMI64VNVPROC) (GLuint program, GLint location, GLint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1I64NVPROC) (GLuint program, GLint location, GLint64EXT x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1I64NVPROC) (GLint location, GLint64EXT x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UI64NVPROC) (GLint location, GLuint64EXT x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);


#define GLEW_NV_gpu_shader5 GLEW_GET_VAR(__GLEW_NV_gpu_shader5)

#endif /* GL_NV_gpu_shader5 */

/* ---------------------------- GL_NV_half_float --------------------------- */

#ifndef GL_NV_half_float
#define GL_NV_half_float 1

#define GL_HALF_FLOAT_NV 0x140B

typedef unsigned short GLhalf;

typedef void (GLAPIENTRY * PFNGLCOLOR3HNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLCOLOR4HNVPROC) (GLhalf red, GLhalf green, GLhalf blue, GLhalf alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLFOGCOORDHNVPROC) (GLhalf fog);
typedef void (GLAPIENTRY * PFNGLFOGCOORDHVNVPROC) (const GLhalf* fog);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1HNVPROC) (GLenum target, GLhalf s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2HNVPROC) (GLenum target, GLhalf s, GLhalf t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3HNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4HNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLNORMAL3HNVPROC) (GLhalf nx, GLhalf ny, GLhalf nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3HNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1HNVPROC) (GLhalf s);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2HNVPROC) (GLhalf s, GLhalf t);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3HNVPROC) (GLhalf s, GLhalf t, GLhalf r);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4HNVPROC) (GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEX2HNVPROC) (GLhalf x, GLhalf y);
typedef void (GLAPIENTRY * PFNGLVERTEX2HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEX3HNVPROC) (GLhalf x, GLhalf y, GLhalf z);
typedef void (GLAPIENTRY * PFNGLVERTEX3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEX4HNVPROC) (GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void (GLAPIENTRY * PFNGLVERTEX4HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1HNVPROC) (GLuint index, GLhalf x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2HNVPROC) (GLuint index, GLhalf x, GLhalf y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3HNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4HNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTHNVPROC) (GLhalf weight);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTHVNVPROC) (const GLhalf* weight);


#define GLEW_NV_half_float GLEW_GET_VAR(__GLEW_NV_half_float)

#endif /* GL_NV_half_float */

/* ------------------------ GL_NV_light_max_exponent ----------------------- */

#ifndef GL_NV_light_max_exponent
#define GL_NV_light_max_exponent 1

#define GL_MAX_SHININESS_NV 0x8504
#define GL_MAX_SPOT_EXPONENT_NV 0x8505

#define GLEW_NV_light_max_exponent GLEW_GET_VAR(__GLEW_NV_light_max_exponent)

#endif /* GL_NV_light_max_exponent */

/* ----------------------- GL_NV_multisample_coverage ---------------------- */

#ifndef GL_NV_multisample_coverage
#define GL_NV_multisample_coverage 1

#define GL_COVERAGE_SAMPLES_NV 0x80A9
#define GL_COLOR_SAMPLES_NV 0x8E20

#define GLEW_NV_multisample_coverage GLEW_GET_VAR(__GLEW_NV_multisample_coverage)

#endif /* GL_NV_multisample_coverage */

/* --------------------- GL_NV_multisample_filter_hint --------------------- */

#ifndef GL_NV_multisample_filter_hint
#define GL_NV_multisample_filter_hint 1

#define GL_MULTISAMPLE_FILTER_HINT_NV 0x8534

#define GLEW_NV_multisample_filter_hint GLEW_GET_VAR(__GLEW_NV_multisample_filter_hint)

#endif /* GL_NV_multisample_filter_hint */

/* ------------------------- GL_NV_occlusion_query ------------------------- */

#ifndef GL_NV_occlusion_query
#define GL_NV_occlusion_query 1

#define GL_PIXEL_COUNTER_BITS_NV 0x8864
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV 0x8865
#define GL_PIXEL_COUNT_NV 0x8866
#define GL_PIXEL_COUNT_AVAILABLE_NV 0x8867

typedef void (GLAPIENTRY * PFNGLBEGINOCCLUSIONQUERYNVPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEOCCLUSIONQUERIESNVPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDOCCLUSIONQUERYNVPROC) (void);
typedef void (GLAPIENTRY * PFNGLGENOCCLUSIONQUERIESNVPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETOCCLUSIONQUERYIVNVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETOCCLUSIONQUERYUIVNVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISOCCLUSIONQUERYNVPROC) (GLuint id);


#define GLEW_NV_occlusion_query GLEW_GET_VAR(__GLEW_NV_occlusion_query)

#endif /* GL_NV_occlusion_query */

/* ----------------------- GL_NV_packed_depth_stencil ---------------------- */

#ifndef GL_NV_packed_depth_stencil
#define GL_NV_packed_depth_stencil 1

#define GL_DEPTH_STENCIL_NV 0x84F9
#define GL_UNSIGNED_INT_24_8_NV 0x84FA

#define GLEW_NV_packed_depth_stencil GLEW_GET_VAR(__GLEW_NV_packed_depth_stencil)

#endif /* GL_NV_packed_depth_stencil */

/* --------------------- GL_NV_parameter_buffer_object --------------------- */

#ifndef GL_NV_parameter_buffer_object
#define GL_NV_parameter_buffer_object 1

#define GL_MAX_PROGRAM_PARAMETER_BUFFER_BINDINGS_NV 0x8DA0
#define GL_MAX_PROGRAM_PARAMETER_BUFFER_SIZE_NV 0x8DA1
#define GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV 0x8DA2
#define GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV 0x8DA3
#define GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV 0x8DA4

typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLfloat *params);


#define GLEW_NV_parameter_buffer_object GLEW_GET_VAR(__GLEW_NV_parameter_buffer_object)

#endif /* GL_NV_parameter_buffer_object */

/* --------------------- GL_NV_parameter_buffer_object2 -------------------- */

#ifndef GL_NV_parameter_buffer_object2
#define GL_NV_parameter_buffer_object2 1

#define GLEW_NV_parameter_buffer_object2 GLEW_GET_VAR(__GLEW_NV_parameter_buffer_object2)

#endif /* GL_NV_parameter_buffer_object2 */

/* -------------------------- GL_NV_path_rendering ------------------------- */

#ifndef GL_NV_path_rendering
#define GL_NV_path_rendering 1

#define GL_CLOSE_PATH_NV 0x00
#define GL_BOLD_BIT_NV 0x01
#define GL_GLYPH_WIDTH_BIT_NV 0x01
#define GL_GLYPH_HEIGHT_BIT_NV 0x02
#define GL_ITALIC_BIT_NV 0x02
#define GL_MOVE_TO_NV 0x02
#define GL_RELATIVE_MOVE_TO_NV 0x03
#define GL_LINE_TO_NV 0x04
#define GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV 0x04
#define GL_RELATIVE_LINE_TO_NV 0x05
#define GL_HORIZONTAL_LINE_TO_NV 0x06
#define GL_RELATIVE_HORIZONTAL_LINE_TO_NV 0x07
#define GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV 0x08
#define GL_VERTICAL_LINE_TO_NV 0x08
#define GL_RELATIVE_VERTICAL_LINE_TO_NV 0x09
#define GL_QUADRATIC_CURVE_TO_NV 0x0A
#define GL_RELATIVE_QUADRATIC_CURVE_TO_NV 0x0B
#define GL_CUBIC_CURVE_TO_NV 0x0C
#define GL_RELATIVE_CUBIC_CURVE_TO_NV 0x0D
#define GL_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0E
#define GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0F
#define GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV 0x10
#define GL_SMOOTH_CUBIC_CURVE_TO_NV 0x10
#define GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV 0x11
#define GL_SMALL_CCW_ARC_TO_NV 0x12
#define GL_RELATIVE_SMALL_CCW_ARC_TO_NV 0x13
#define GL_SMALL_CW_ARC_TO_NV 0x14
#define GL_RELATIVE_SMALL_CW_ARC_TO_NV 0x15
#define GL_LARGE_CCW_ARC_TO_NV 0x16
#define GL_RELATIVE_LARGE_CCW_ARC_TO_NV 0x17
#define GL_LARGE_CW_ARC_TO_NV 0x18
#define GL_RELATIVE_LARGE_CW_ARC_TO_NV 0x19
#define GL_GLYPH_VERTICAL_BEARING_X_BIT_NV 0x20
#define GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV 0x40
#define GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV 0x80
#define GL_RESTART_PATH_NV 0xF0
#define GL_DUP_FIRST_CUBIC_CURVE_TO_NV 0xF2
#define GL_DUP_LAST_CUBIC_CURVE_TO_NV 0xF4
#define GL_RECT_NV 0xF6
#define GL_CIRCULAR_CCW_ARC_TO_NV 0xF8
#define GL_CIRCULAR_CW_ARC_TO_NV 0xFA
#define GL_CIRCULAR_TANGENT_ARC_TO_NV 0xFC
#define GL_ARC_TO_NV 0xFE
#define GL_RELATIVE_ARC_TO_NV 0xFF
#define GL_GLYPH_HAS_KERNING_BIT_NV 0x100
#define GL_PRIMARY_COLOR_NV 0x852C
#define GL_SECONDARY_COLOR_NV 0x852D
#define GL_PRIMARY_COLOR 0x8577
#define GL_PATH_FORMAT_SVG_NV 0x9070
#define GL_PATH_FORMAT_PS_NV 0x9071
#define GL_STANDARD_FONT_NAME_NV 0x9072
#define GL_SYSTEM_FONT_NAME_NV 0x9073
#define GL_FILE_NAME_NV 0x9074
#define GL_PATH_STROKE_WIDTH_NV 0x9075
#define GL_PATH_END_CAPS_NV 0x9076
#define GL_PATH_INITIAL_END_CAP_NV 0x9077
#define GL_PATH_TERMINAL_END_CAP_NV 0x9078
#define GL_PATH_JOIN_STYLE_NV 0x9079
#define GL_PATH_MITER_LIMIT_NV 0x907A
#define GL_PATH_DASH_CAPS_NV 0x907B
#define GL_PATH_INITIAL_DASH_CAP_NV 0x907C
#define GL_PATH_TERMINAL_DASH_CAP_NV 0x907D
#define GL_PATH_DASH_OFFSET_NV 0x907E
#define GL_PATH_CLIENT_LENGTH_NV 0x907F
#define GL_PATH_FILL_MODE_NV 0x9080
#define GL_PATH_FILL_MASK_NV 0x9081
#define GL_PATH_FILL_COVER_MODE_NV 0x9082
#define GL_PATH_STROKE_COVER_MODE_NV 0x9083
#define GL_PATH_STROKE_MASK_NV 0x9084
#define GL_COUNT_UP_NV 0x9088
#define GL_COUNT_DOWN_NV 0x9089
#define GL_PATH_OBJECT_BOUNDING_BOX_NV 0x908A
#define GL_CONVEX_HULL_NV 0x908B
#define GL_BOUNDING_BOX_NV 0x908D
#define GL_TRANSLATE_X_NV 0x908E
#define GL_TRANSLATE_Y_NV 0x908F
#define GL_TRANSLATE_2D_NV 0x9090
#define GL_TRANSLATE_3D_NV 0x9091
#define GL_AFFINE_2D_NV 0x9092
#define GL_AFFINE_3D_NV 0x9094
#define GL_TRANSPOSE_AFFINE_2D_NV 0x9096
#define GL_TRANSPOSE_AFFINE_3D_NV 0x9098
#define GL_UTF8_NV 0x909A
#define GL_UTF16_NV 0x909B
#define GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV 0x909C
#define GL_PATH_COMMAND_COUNT_NV 0x909D
#define GL_PATH_COORD_COUNT_NV 0x909E
#define GL_PATH_DASH_ARRAY_COUNT_NV 0x909F
#define GL_PATH_COMPUTED_LENGTH_NV 0x90A0
#define GL_PATH_FILL_BOUNDING_BOX_NV 0x90A1
#define GL_PATH_STROKE_BOUNDING_BOX_NV 0x90A2
#define GL_SQUARE_NV 0x90A3
#define GL_ROUND_NV 0x90A4
#define GL_TRIANGULAR_NV 0x90A5
#define GL_BEVEL_NV 0x90A6
#define GL_MITER_REVERT_NV 0x90A7
#define GL_MITER_TRUNCATE_NV 0x90A8
#define GL_SKIP_MISSING_GLYPH_NV 0x90A9
#define GL_USE_MISSING_GLYPH_NV 0x90AA
#define GL_PATH_ERROR_POSITION_NV 0x90AB
#define GL_PATH_FOG_GEN_MODE_NV 0x90AC
#define GL_ACCUM_ADJACENT_PAIRS_NV 0x90AD
#define GL_ADJACENT_PAIRS_NV 0x90AE
#define GL_FIRST_TO_REST_NV 0x90AF
#define GL_PATH_GEN_MODE_NV 0x90B0
#define GL_PATH_GEN_COEFF_NV 0x90B1
#define GL_PATH_GEN_COLOR_FORMAT_NV 0x90B2
#define GL_PATH_GEN_COMPONENTS_NV 0x90B3
#define GL_PATH_DASH_OFFSET_RESET_NV 0x90B4
#define GL_MOVE_TO_RESETS_NV 0x90B5
#define GL_MOVE_TO_CONTINUES_NV 0x90B6
#define GL_PATH_STENCIL_FUNC_NV 0x90B7
#define GL_PATH_STENCIL_REF_NV 0x90B8
#define GL_PATH_STENCIL_VALUE_MASK_NV 0x90B9
#define GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV 0x90BD
#define GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV 0x90BE
#define GL_PATH_COVER_DEPTH_FUNC_NV 0x90BF
#define GL_FONT_X_MIN_BOUNDS_BIT_NV 0x00010000
#define GL_FONT_Y_MIN_BOUNDS_BIT_NV 0x00020000
#define GL_FONT_X_MAX_BOUNDS_BIT_NV 0x00040000
#define GL_FONT_Y_MAX_BOUNDS_BIT_NV 0x00080000
#define GL_FONT_UNITS_PER_EM_BIT_NV 0x00100000
#define GL_FONT_ASCENDER_BIT_NV 0x00200000
#define GL_FONT_DESCENDER_BIT_NV 0x00400000
#define GL_FONT_HEIGHT_BIT_NV 0x00800000
#define GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV 0x01000000
#define GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV 0x02000000
#define GL_FONT_UNDERLINE_POSITION_BIT_NV 0x04000000
#define GL_FONT_UNDERLINE_THICKNESS_BIT_NV 0x08000000
#define GL_FONT_HAS_KERNING_BIT_NV 0x10000000

typedef void (GLAPIENTRY * PFNGLCOPYPATHNVPROC) (GLuint resultPath, GLuint srcPath);
typedef void (GLAPIENTRY * PFNGLCOVERFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLCOVERFILLPATHNVPROC) (GLuint path, GLenum coverMode);
typedef void (GLAPIENTRY * PFNGLCOVERSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLCOVERSTROKEPATHNVPROC) (GLuint name, GLenum coverMode);
typedef void (GLAPIENTRY * PFNGLDELETEPATHSNVPROC) (GLuint path, GLsizei range);
typedef GLuint (GLAPIENTRY * PFNGLGENPATHSNVPROC) (GLsizei range);
typedef void (GLAPIENTRY * PFNGLGETPATHCOLORGENFVNVPROC) (GLenum color, GLenum pname, GLfloat* value);
typedef void (GLAPIENTRY * PFNGLGETPATHCOLORGENIVNVPROC) (GLenum color, GLenum pname, GLint* value);
typedef void (GLAPIENTRY * PFNGLGETPATHCOMMANDSNVPROC) (GLuint name, GLubyte* commands);
typedef void (GLAPIENTRY * PFNGLGETPATHCOORDSNVPROC) (GLuint name, GLfloat* coords);
typedef void (GLAPIENTRY * PFNGLGETPATHDASHARRAYNVPROC) (GLuint name, GLfloat* dashArray);
typedef GLfloat (GLAPIENTRY * PFNGLGETPATHLENGTHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments);
typedef void (GLAPIENTRY * PFNGLGETPATHMETRICRANGENVPROC) (GLbitfield metricQueryMask, GLuint fistPathName, GLsizei numPaths, GLsizei stride, GLfloat* metrics);
typedef void (GLAPIENTRY * PFNGLGETPATHMETRICSNVPROC) (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
typedef void (GLAPIENTRY * PFNGLGETPATHPARAMETERFVNVPROC) (GLuint name, GLenum param, GLfloat* value);
typedef void (GLAPIENTRY * PFNGLGETPATHPARAMETERIVNVPROC) (GLuint name, GLenum param, GLint* value);
typedef void (GLAPIENTRY * PFNGLGETPATHSPACINGNVPROC) (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
typedef void (GLAPIENTRY * PFNGLGETPATHTEXGENFVNVPROC) (GLenum texCoordSet, GLenum pname, GLfloat* value);
typedef void (GLAPIENTRY * PFNGLGETPATHTEXGENIVNVPROC) (GLenum texCoordSet, GLenum pname, GLint* value);
typedef void (GLAPIENTRY * PFNGLINTERPOLATEPATHSNVPROC) (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
typedef GLboolean (GLAPIENTRY * PFNGLISPATHNVPROC) (GLuint path);
typedef GLboolean (GLAPIENTRY * PFNGLISPOINTINFILLPATHNVPROC) (GLuint path, GLuint mask, GLfloat x, GLfloat y);
typedef GLboolean (GLAPIENTRY * PFNGLISPOINTINSTROKEPATHNVPROC) (GLuint path, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLPATHCOLORGENNVPROC) (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat* coeffs);
typedef void (GLAPIENTRY * PFNGLPATHCOMMANDSNVPROC) (GLuint path, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
typedef void (GLAPIENTRY * PFNGLPATHCOORDSNVPROC) (GLuint path, GLsizei numCoords, GLenum coordType, const void* coords);
typedef void (GLAPIENTRY * PFNGLPATHCOVERDEPTHFUNCNVPROC) (GLenum zfunc);
typedef void (GLAPIENTRY * PFNGLPATHDASHARRAYNVPROC) (GLuint path, GLsizei dashCount, const GLfloat* dashArray);
typedef void (GLAPIENTRY * PFNGLPATHFOGGENNVPROC) (GLenum genMode);
typedef void (GLAPIENTRY * PFNGLPATHGLYPHRANGENVPROC) (GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (GLAPIENTRY * PFNGLPATHGLYPHSNVPROC) (GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid*charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERFNVPROC) (GLuint path, GLenum pname, GLfloat value);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERFVNVPROC) (GLuint path, GLenum pname, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERINVPROC) (GLuint path, GLenum pname, GLint value);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERIVNVPROC) (GLuint path, GLenum pname, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPATHSTENCILDEPTHOFFSETNVPROC) (GLfloat factor, GLfloat units);
typedef void (GLAPIENTRY * PFNGLPATHSTENCILFUNCNVPROC) (GLenum func, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLPATHSTRINGNVPROC) (GLuint path, GLenum format, GLsizei length, const void* pathString);
typedef void (GLAPIENTRY * PFNGLPATHSUBCOMMANDSNVPROC) (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
typedef void (GLAPIENTRY * PFNGLPATHSUBCOORDSNVPROC) (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void* coords);
typedef void (GLAPIENTRY * PFNGLPATHTEXGENNVPROC) (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat* coeffs);
typedef GLboolean (GLAPIENTRY * PFNGLPOINTALONGPATHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat* x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);
typedef void (GLAPIENTRY * PFNGLSTENCILFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLSTENCILFILLPATHNVPROC) (GLuint path, GLenum fillMode, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLSTENCILSTROKEPATHNVPROC) (GLuint path, GLint reference, GLuint mask);
typedef void (GLAPIENTRY * PFNGLTRANSFORMPATHNVPROC) (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat* transformValues);
typedef void (GLAPIENTRY * PFNGLWEIGHTPATHSNVPROC) (GLuint resultPath, GLsizei numPaths, const GLuint paths[], const GLfloat weights[]);


#define GLEW_NV_path_rendering GLEW_GET_VAR(__GLEW_NV_path_rendering)

#endif /* GL_NV_path_rendering */

/* ------------------------- GL_NV_pixel_data_range ------------------------ */

#ifndef GL_NV_pixel_data_range
#define GL_NV_pixel_data_range 1

#define GL_WRITE_PIXEL_DATA_RANGE_NV 0x8878
#define GL_READ_PIXEL_DATA_RANGE_NV 0x8879
#define GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV 0x887A
#define GL_READ_PIXEL_DATA_RANGE_LENGTH_NV 0x887B
#define GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV 0x887C
#define GL_READ_PIXEL_DATA_RANGE_POINTER_NV 0x887D

typedef void (GLAPIENTRY * PFNGLFLUSHPIXELDATARANGENVPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLPIXELDATARANGENVPROC) (GLenum target, GLsizei length, void* pointer);


#define GLEW_NV_pixel_data_range GLEW_GET_VAR(__GLEW_NV_pixel_data_range)

#endif /* GL_NV_pixel_data_range */

/* --------------------------- GL_NV_point_sprite -------------------------- */

#ifndef GL_NV_point_sprite
#define GL_NV_point_sprite 1

#define GL_POINT_SPRITE_NV 0x8861
#define GL_COORD_REPLACE_NV 0x8862
#define GL_POINT_SPRITE_R_MODE_NV 0x8863

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIVNVPROC) (GLenum pname, const GLint* params);


#define GLEW_NV_point_sprite GLEW_GET_VAR(__GLEW_NV_point_sprite)

#endif /* GL_NV_point_sprite */

/* -------------------------- GL_NV_present_video -------------------------- */

#ifndef GL_NV_present_video
#define GL_NV_present_video 1

#define GL_FRAME_NV 0x8E26
#define GL_FIELDS_NV 0x8E27
#define GL_CURRENT_TIME_NV 0x8E28
#define GL_NUM_FILL_STREAMS_NV 0x8E29
#define GL_PRESENT_TIME_NV 0x8E2A
#define GL_PRESENT_DURATION_NV 0x8E2B

typedef void (GLAPIENTRY * PFNGLGETVIDEOI64VNVPROC) (GLuint video_slot, GLenum pname, GLint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOIVNVPROC) (GLuint video_slot, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOUI64VNVPROC) (GLuint video_slot, GLenum pname, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOUIVNVPROC) (GLuint video_slot, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLPRESENTFRAMEDUALFILLNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
typedef void (GLAPIENTRY * PFNGLPRESENTFRAMEKEYEDNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);


#define GLEW_NV_present_video GLEW_GET_VAR(__GLEW_NV_present_video)

#endif /* GL_NV_present_video */

/* ------------------------ GL_NV_primitive_restart ------------------------ */

#ifndef GL_NV_primitive_restart
#define GL_NV_primitive_restart 1

#define GL_PRIMITIVE_RESTART_NV 0x8558
#define GL_PRIMITIVE_RESTART_INDEX_NV 0x8559

typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTINDEXNVPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTNVPROC) (void);


#define GLEW_NV_primitive_restart GLEW_GET_VAR(__GLEW_NV_primitive_restart)

#endif /* GL_NV_primitive_restart */

/* ------------------------ GL_NV_register_combiners ----------------------- */

#ifndef GL_NV_register_combiners
#define GL_NV_register_combiners 1

#define GL_REGISTER_COMBINERS_NV 0x8522
#define GL_VARIABLE_A_NV 0x8523
#define GL_VARIABLE_B_NV 0x8524
#define GL_VARIABLE_C_NV 0x8525
#define GL_VARIABLE_D_NV 0x8526
#define GL_VARIABLE_E_NV 0x8527
#define GL_VARIABLE_F_NV 0x8528
#define GL_VARIABLE_G_NV 0x8529
#define GL_CONSTANT_COLOR0_NV 0x852A
#define GL_CONSTANT_COLOR1_NV 0x852B
#define GL_PRIMARY_COLOR_NV 0x852C
#define GL_SECONDARY_COLOR_NV 0x852D
#define GL_SPARE0_NV 0x852E
#define GL_SPARE1_NV 0x852F
#define GL_DISCARD_NV 0x8530
#define GL_E_TIMES_F_NV 0x8531
#define GL_SPARE0_PLUS_SECONDARY_COLOR_NV 0x8532
#define GL_UNSIGNED_IDENTITY_NV 0x8536
#define GL_UNSIGNED_INVERT_NV 0x8537
#define GL_EXPAND_NORMAL_NV 0x8538
#define GL_EXPAND_NEGATE_NV 0x8539
#define GL_HALF_BIAS_NORMAL_NV 0x853A
#define GL_HALF_BIAS_NEGATE_NV 0x853B
#define GL_SIGNED_IDENTITY_NV 0x853C
#define GL_SIGNED_NEGATE_NV 0x853D
#define GL_SCALE_BY_TWO_NV 0x853E
#define GL_SCALE_BY_FOUR_NV 0x853F
#define GL_SCALE_BY_ONE_HALF_NV 0x8540
#define GL_BIAS_BY_NEGATIVE_ONE_HALF_NV 0x8541
#define GL_COMBINER_INPUT_NV 0x8542
#define GL_COMBINER_MAPPING_NV 0x8543
#define GL_COMBINER_COMPONENT_USAGE_NV 0x8544
#define GL_COMBINER_AB_DOT_PRODUCT_NV 0x8545
#define GL_COMBINER_CD_DOT_PRODUCT_NV 0x8546
#define GL_COMBINER_MUX_SUM_NV 0x8547
#define GL_COMBINER_SCALE_NV 0x8548
#define GL_COMBINER_BIAS_NV 0x8549
#define GL_COMBINER_AB_OUTPUT_NV 0x854A
#define GL_COMBINER_CD_OUTPUT_NV 0x854B
#define GL_COMBINER_SUM_OUTPUT_NV 0x854C
#define GL_MAX_GENERAL_COMBINERS_NV 0x854D
#define GL_NUM_GENERAL_COMBINERS_NV 0x854E
#define GL_COLOR_SUM_CLAMP_NV 0x854F
#define GL_COMBINER0_NV 0x8550
#define GL_COMBINER1_NV 0x8551
#define GL_COMBINER2_NV 0x8552
#define GL_COMBINER3_NV 0x8553
#define GL_COMBINER4_NV 0x8554
#define GL_COMBINER5_NV 0x8555
#define GL_COMBINER6_NV 0x8556
#define GL_COMBINER7_NV 0x8557

typedef void (GLAPIENTRY * PFNGLCOMBINERINPUTNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (GLAPIENTRY * PFNGLCOMBINEROUTPUTNVPROC) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERFNVPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERFVNVPROC) (GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERIVNVPROC) (GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLFINALCOMBINERINPUTNVPROC) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC) (GLenum variable, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC) (GLenum variable, GLenum pname, GLint* params);


#define GLEW_NV_register_combiners GLEW_GET_VAR(__GLEW_NV_register_combiners)

#endif /* GL_NV_register_combiners */

/* ----------------------- GL_NV_register_combiners2 ----------------------- */

#ifndef GL_NV_register_combiners2
#define GL_NV_register_combiners2 1

#define GL_PER_STAGE_CONSTANTS_NV 0x8535

typedef void (GLAPIENTRY * PFNGLCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, GLfloat* params);


#define GLEW_NV_register_combiners2 GLEW_GET_VAR(__GLEW_NV_register_combiners2)

#endif /* GL_NV_register_combiners2 */

/* ----------------------- GL_NV_shader_atomic_float ----------------------- */

#ifndef GL_NV_shader_atomic_float
#define GL_NV_shader_atomic_float 1

#define GLEW_NV_shader_atomic_float GLEW_GET_VAR(__GLEW_NV_shader_atomic_float)

#endif /* GL_NV_shader_atomic_float */

/* ------------------------ GL_NV_shader_buffer_load ----------------------- */

#ifndef GL_NV_shader_buffer_load
#define GL_NV_shader_buffer_load 1

#define GL_BUFFER_GPU_ADDRESS_NV 0x8F1D
#define GL_GPU_ADDRESS_NV 0x8F34
#define GL_MAX_SHADER_BUFFER_ADDRESS_NV 0x8F35

typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERUI64VNVPROC) (GLenum target, GLenum pname, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETINTEGERUI64VNVPROC) (GLenum value, GLuint64EXT* result);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC) (GLuint buffer, GLenum pname, GLuint64EXT* params);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERRESIDENTNVPROC) (GLenum target);
typedef GLboolean (GLAPIENTRY * PFNGLISNAMEDBUFFERRESIDENTNVPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMAKEBUFFERNONRESIDENTNVPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLMAKEBUFFERRESIDENTNVPROC) (GLenum target, GLenum access);
typedef void (GLAPIENTRY * PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMAKENAMEDBUFFERRESIDENTNVPROC) (GLuint buffer, GLenum access);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMUI64NVPROC) (GLuint program, GLint location, GLuint64EXT value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMUI64NVPROC) (GLint location, GLuint64EXT value);
typedef void (GLAPIENTRY * PFNGLUNIFORMUI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);


#define GLEW_NV_shader_buffer_load GLEW_GET_VAR(__GLEW_NV_shader_buffer_load)

#endif /* GL_NV_shader_buffer_load */

/* ---------------------- GL_NV_tessellation_program5 ---------------------- */

#ifndef GL_NV_tessellation_program5
#define GL_NV_tessellation_program5 1

#define GL_MAX_PROGRAM_PATCH_ATTRIBS_NV 0x86D8
#define GL_TESS_CONTROL_PROGRAM_NV 0x891E
#define GL_TESS_EVALUATION_PROGRAM_NV 0x891F
#define GL_TESS_CONTROL_PROGRAM_PARAMETER_BUFFER_NV 0x8C74
#define GL_TESS_EVALUATION_PROGRAM_PARAMETER_BUFFER_NV 0x8C75

#define GLEW_NV_tessellation_program5 GLEW_GET_VAR(__GLEW_NV_tessellation_program5)

#endif /* GL_NV_tessellation_program5 */

/* -------------------------- GL_NV_texgen_emboss -------------------------- */

#ifndef GL_NV_texgen_emboss
#define GL_NV_texgen_emboss 1

#define GL_EMBOSS_LIGHT_NV 0x855D
#define GL_EMBOSS_CONSTANT_NV 0x855E
#define GL_EMBOSS_MAP_NV 0x855F

#define GLEW_NV_texgen_emboss GLEW_GET_VAR(__GLEW_NV_texgen_emboss)

#endif /* GL_NV_texgen_emboss */

/* ------------------------ GL_NV_texgen_reflection ------------------------ */

#ifndef GL_NV_texgen_reflection
#define GL_NV_texgen_reflection 1

#define GL_NORMAL_MAP_NV 0x8511
#define GL_REFLECTION_MAP_NV 0x8512

#define GLEW_NV_texgen_reflection GLEW_GET_VAR(__GLEW_NV_texgen_reflection)

#endif /* GL_NV_texgen_reflection */

/* ------------------------- GL_NV_texture_barrier ------------------------- */

#ifndef GL_NV_texture_barrier
#define GL_NV_texture_barrier 1

typedef void (GLAPIENTRY * PFNGLTEXTUREBARRIERNVPROC) (void);


#define GLEW_NV_texture_barrier GLEW_GET_VAR(__GLEW_NV_texture_barrier)

#endif /* GL_NV_texture_barrier */

/* --------------------- GL_NV_texture_compression_vtc --------------------- */

#ifndef GL_NV_texture_compression_vtc
#define GL_NV_texture_compression_vtc 1

#define GLEW_NV_texture_compression_vtc GLEW_GET_VAR(__GLEW_NV_texture_compression_vtc)

#endif /* GL_NV_texture_compression_vtc */

/* ----------------------- GL_NV_texture_env_combine4 ---------------------- */

#ifndef GL_NV_texture_env_combine4
#define GL_NV_texture_env_combine4 1

#define GL_COMBINE4_NV 0x8503
#define GL_SOURCE3_RGB_NV 0x8583
#define GL_SOURCE3_ALPHA_NV 0x858B
#define GL_OPERAND3_RGB_NV 0x8593
#define GL_OPERAND3_ALPHA_NV 0x859B

#define GLEW_NV_texture_env_combine4 GLEW_GET_VAR(__GLEW_NV_texture_env_combine4)

#endif /* GL_NV_texture_env_combine4 */

/* ---------------------- GL_NV_texture_expand_normal ---------------------- */

#ifndef GL_NV_texture_expand_normal
#define GL_NV_texture_expand_normal 1

#define GL_TEXTURE_UNSIGNED_REMAP_MODE_NV 0x888F

#define GLEW_NV_texture_expand_normal GLEW_GET_VAR(__GLEW_NV_texture_expand_normal)

#endif /* GL_NV_texture_expand_normal */

/* ----------------------- GL_NV_texture_multisample ----------------------- */

#ifndef GL_NV_texture_multisample
#define GL_NV_texture_multisample 1

#define GL_TEXTURE_COVERAGE_SAMPLES_NV 0x9045
#define GL_TEXTURE_COLOR_SAMPLES_NV 0x9046

typedef void (GLAPIENTRY * PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);


#define GLEW_NV_texture_multisample GLEW_GET_VAR(__GLEW_NV_texture_multisample)

#endif /* GL_NV_texture_multisample */

/* ------------------------ GL_NV_texture_rectangle ------------------------ */

#ifndef GL_NV_texture_rectangle
#define GL_NV_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_NV 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_NV 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV 0x84F8

#define GLEW_NV_texture_rectangle GLEW_GET_VAR(__GLEW_NV_texture_rectangle)

#endif /* GL_NV_texture_rectangle */

/* -------------------------- GL_NV_texture_shader ------------------------- */

#ifndef GL_NV_texture_shader
#define GL_NV_texture_shader 1

#define GL_OFFSET_TEXTURE_RECTANGLE_NV 0x864C
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV 0x864D
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV 0x864E
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV 0x86D9
#define GL_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#define GL_DSDT_MAG_INTENSITY_NV 0x86DC
#define GL_SHADER_CONSISTENT_NV 0x86DD
#define GL_TEXTURE_SHADER_NV 0x86DE
#define GL_SHADER_OPERATION_NV 0x86DF
#define GL_CULL_MODES_NV 0x86E0
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV 0x86E1
#define GL_OFFSET_TEXTURE_MATRIX_NV 0x86E1
#define GL_OFFSET_TEXTURE_2D_SCALE_NV 0x86E2
#define GL_OFFSET_TEXTURE_SCALE_NV 0x86E2
#define GL_OFFSET_TEXTURE_BIAS_NV 0x86E3
#define GL_OFFSET_TEXTURE_2D_BIAS_NV 0x86E3
#define GL_PREVIOUS_TEXTURE_INPUT_NV 0x86E4
#define GL_CONST_EYE_NV 0x86E5
#define GL_PASS_THROUGH_NV 0x86E6
#define GL_CULL_FRAGMENT_NV 0x86E7
#define GL_OFFSET_TEXTURE_2D_NV 0x86E8
#define GL_DEPENDENT_AR_TEXTURE_2D_NV 0x86E9
#define GL_DEPENDENT_GB_TEXTURE_2D_NV 0x86EA
#define GL_DOT_PRODUCT_NV 0x86EC
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV 0x86ED
#define GL_DOT_PRODUCT_TEXTURE_2D_NV 0x86EE
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV 0x86F0
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV 0x86F1
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV 0x86F2
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV 0x86F3
#define GL_HILO_NV 0x86F4
#define GL_DSDT_NV 0x86F5
#define GL_DSDT_MAG_NV 0x86F6
#define GL_DSDT_MAG_VIB_NV 0x86F7
#define GL_HILO16_NV 0x86F8
#define GL_SIGNED_HILO_NV 0x86F9
#define GL_SIGNED_HILO16_NV 0x86FA
#define GL_SIGNED_RGBA_NV 0x86FB
#define GL_SIGNED_RGBA8_NV 0x86FC
#define GL_SIGNED_RGB_NV 0x86FE
#define GL_SIGNED_RGB8_NV 0x86FF
#define GL_SIGNED_LUMINANCE_NV 0x8701
#define GL_SIGNED_LUMINANCE8_NV 0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#define GL_SIGNED_ALPHA_NV 0x8705
#define GL_SIGNED_ALPHA8_NV 0x8706
#define GL_SIGNED_INTENSITY_NV 0x8707
#define GL_SIGNED_INTENSITY8_NV 0x8708
#define GL_DSDT8_NV 0x8709
#define GL_DSDT8_MAG8_NV 0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV 0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D
#define GL_HI_SCALE_NV 0x870E
#define GL_LO_SCALE_NV 0x870F
#define GL_DS_SCALE_NV 0x8710
#define GL_DT_SCALE_NV 0x8711
#define GL_MAGNITUDE_SCALE_NV 0x8712
#define GL_VIBRANCE_SCALE_NV 0x8713
#define GL_HI_BIAS_NV 0x8714
#define GL_LO_BIAS_NV 0x8715
#define GL_DS_BIAS_NV 0x8716
#define GL_DT_BIAS_NV 0x8717
#define GL_MAGNITUDE_BIAS_NV 0x8718
#define GL_VIBRANCE_BIAS_NV 0x8719
#define GL_TEXTURE_BORDER_VALUES_NV 0x871A
#define GL_TEXTURE_HI_SIZE_NV 0x871B
#define GL_TEXTURE_LO_SIZE_NV 0x871C
#define GL_TEXTURE_DS_SIZE_NV 0x871D
#define GL_TEXTURE_DT_SIZE_NV 0x871E
#define GL_TEXTURE_MAG_SIZE_NV 0x871F

#define GLEW_NV_texture_shader GLEW_GET_VAR(__GLEW_NV_texture_shader)

#endif /* GL_NV_texture_shader */

/* ------------------------- GL_NV_texture_shader2 ------------------------- */

#ifndef GL_NV_texture_shader2
#define GL_NV_texture_shader2 1

#define GL_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#define GL_DSDT_MAG_INTENSITY_NV 0x86DC
#define GL_DOT_PRODUCT_TEXTURE_3D_NV 0x86EF
#define GL_HILO_NV 0x86F4
#define GL_DSDT_NV 0x86F5
#define GL_DSDT_MAG_NV 0x86F6
#define GL_DSDT_MAG_VIB_NV 0x86F7
#define GL_HILO16_NV 0x86F8
#define GL_SIGNED_HILO_NV 0x86F9
#define GL_SIGNED_HILO16_NV 0x86FA
#define GL_SIGNED_RGBA_NV 0x86FB
#define GL_SIGNED_RGBA8_NV 0x86FC
#define GL_SIGNED_RGB_NV 0x86FE
#define GL_SIGNED_RGB8_NV 0x86FF
#define GL_SIGNED_LUMINANCE_NV 0x8701
#define GL_SIGNED_LUMINANCE8_NV 0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#define GL_SIGNED_ALPHA_NV 0x8705
#define GL_SIGNED_ALPHA8_NV 0x8706
#define GL_SIGNED_INTENSITY_NV 0x8707
#define GL_SIGNED_INTENSITY8_NV 0x8708
#define GL_DSDT8_NV 0x8709
#define GL_DSDT8_MAG8_NV 0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV 0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D

#define GLEW_NV_texture_shader2 GLEW_GET_VAR(__GLEW_NV_texture_shader2)

#endif /* GL_NV_texture_shader2 */

/* ------------------------- GL_NV_texture_shader3 ------------------------- */

#ifndef GL_NV_texture_shader3
#define GL_NV_texture_shader3 1

#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV 0x8850
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV 0x8851
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8852
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV 0x8853
#define GL_OFFSET_HILO_TEXTURE_2D_NV 0x8854
#define GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV 0x8855
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV 0x8856
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8857
#define GL_DEPENDENT_HILO_TEXTURE_2D_NV 0x8858
#define GL_DEPENDENT_RGB_TEXTURE_3D_NV 0x8859
#define GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV 0x885A
#define GL_DOT_PRODUCT_PASS_THROUGH_NV 0x885B
#define GL_DOT_PRODUCT_TEXTURE_1D_NV 0x885C
#define GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV 0x885D
#define GL_HILO8_NV 0x885E
#define GL_SIGNED_HILO8_NV 0x885F
#define GL_FORCE_BLUE_TO_ONE_NV 0x8860

#define GLEW_NV_texture_shader3 GLEW_GET_VAR(__GLEW_NV_texture_shader3)

#endif /* GL_NV_texture_shader3 */

/* ------------------------ GL_NV_transform_feedback ----------------------- */

#ifndef GL_NV_transform_feedback
#define GL_NV_transform_feedback 1

#define GL_BACK_PRIMARY_COLOR_NV 0x8C77
#define GL_BACK_SECONDARY_COLOR_NV 0x8C78
#define GL_TEXTURE_COORD_NV 0x8C79
#define GL_CLIP_DISTANCE_NV 0x8C7A
#define GL_VERTEX_ID_NV 0x8C7B
#define GL_PRIMITIVE_ID_NV 0x8C7C
#define GL_GENERIC_ATTRIB_NV 0x8C7D
#define GL_TRANSFORM_FEEDBACK_ATTRIBS_NV 0x8C7E
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE_NV 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_NV 0x8C80
#define GL_ACTIVE_VARYINGS_NV 0x8C81
#define GL_ACTIVE_VARYING_MAX_LENGTH_NV 0x8C82
#define GL_TRANSFORM_FEEDBACK_VARYINGS_NV 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START_NV 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_NV 0x8C85
#define GL_TRANSFORM_FEEDBACK_RECORD_NV 0x8C86
#define GL_PRIMITIVES_GENERATED_NV 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_NV 0x8C88
#define GL_RASTERIZER_DISCARD_NV 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_NV 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_NV 0x8C8B
#define GL_INTERLEAVED_ATTRIBS_NV 0x8C8C
#define GL_SEPARATE_ATTRIBS_NV 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER_NV 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_NV 0x8C8F

typedef void (GLAPIENTRY * PFNGLACTIVEVARYINGNVPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKNVPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASENVPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFEROFFSETNVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGENVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKNVPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETACTIVEVARYINGNVPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC) (GLuint program, GLuint index, GLint *location);
typedef GLint (GLAPIENTRY * PFNGLGETVARYINGLOCATIONNVPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC) (GLuint count, const GLint *attribs, GLenum bufferMode);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC) (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);


#define GLEW_NV_transform_feedback GLEW_GET_VAR(__GLEW_NV_transform_feedback)

#endif /* GL_NV_transform_feedback */

/* ----------------------- GL_NV_transform_feedback2 ----------------------- */

#ifndef GL_NV_transform_feedback2
#define GL_NV_transform_feedback2 1

#define GL_TRANSFORM_FEEDBACK_NV 0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED_NV 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE_NV 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING_NV 0x8E25

typedef void (GLAPIENTRY * PFNGLBINDTRANSFORMFEEDBACKNVPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETETRANSFORMFEEDBACKSNVPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKNVPROC) (GLenum mode, GLuint id);
typedef void (GLAPIENTRY * PFNGLGENTRANSFORMFEEDBACKSNVPROC) (GLsizei n, GLuint* ids);
typedef GLboolean (GLAPIENTRY * PFNGLISTRANSFORMFEEDBACKNVPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLPAUSETRANSFORMFEEDBACKNVPROC) (void);
typedef void (GLAPIENTRY * PFNGLRESUMETRANSFORMFEEDBACKNVPROC) (void);


#define GLEW_NV_transform_feedback2 GLEW_GET_VAR(__GLEW_NV_transform_feedback2)

#endif /* GL_NV_transform_feedback2 */

/* -------------------------- GL_NV_vdpau_interop -------------------------- */

#ifndef GL_NV_vdpau_interop
#define GL_NV_vdpau_interop 1

#define GL_SURFACE_STATE_NV 0x86EB
#define GL_SURFACE_REGISTERED_NV 0x86FD
#define GL_SURFACE_MAPPED_NV 0x8700
#define GL_WRITE_DISCARD_NV 0x88BE

typedef GLintptr GLvdpauSurfaceNV;

typedef void (GLAPIENTRY * PFNGLVDPAUFININVPROC) (void);
typedef void (GLAPIENTRY * PFNGLVDPAUGETSURFACEIVNVPROC) (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei* length, GLint *values);
typedef void (GLAPIENTRY * PFNGLVDPAUINITNVPROC) (const void* vdpDevice, const GLvoid*getProcAddress);
typedef void (GLAPIENTRY * PFNGLVDPAUISSURFACENVPROC) (GLvdpauSurfaceNV surface);
typedef void (GLAPIENTRY * PFNGLVDPAUMAPSURFACESNVPROC) (GLsizei numSurfaces, const GLvdpauSurfaceNV* surfaces);
typedef GLvdpauSurfaceNV (GLAPIENTRY * PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef GLvdpauSurfaceNV (GLAPIENTRY * PFNGLVDPAUREGISTERVIDEOSURFACENVPROC) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef void (GLAPIENTRY * PFNGLVDPAUSURFACEACCESSNVPROC) (GLvdpauSurfaceNV surface, GLenum access);
typedef void (GLAPIENTRY * PFNGLVDPAUUNMAPSURFACESNVPROC) (GLsizei numSurface, const GLvdpauSurfaceNV* surfaces);
typedef void (GLAPIENTRY * PFNGLVDPAUUNREGISTERSURFACENVPROC) (GLvdpauSurfaceNV surface);


#define GLEW_NV_vdpau_interop GLEW_GET_VAR(__GLEW_NV_vdpau_interop)

#endif /* GL_NV_vdpau_interop */

/* ------------------------ GL_NV_vertex_array_range ----------------------- */

#ifndef GL_NV_vertex_array_range
#define GL_NV_vertex_array_range 1

#define GL_VERTEX_ARRAY_RANGE_NV 0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_NV 0x851E
#define GL_VERTEX_ARRAY_RANGE_VALID_NV 0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_NV 0x8521

typedef void (GLAPIENTRY * PFNGLFLUSHVERTEXARRAYRANGENVPROC) (void);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYRANGENVPROC) (GLsizei length, void* pointer);


#define GLEW_NV_vertex_array_range GLEW_GET_VAR(__GLEW_NV_vertex_array_range)

#endif /* GL_NV_vertex_array_range */

/* ----------------------- GL_NV_vertex_array_range2 ----------------------- */

#ifndef GL_NV_vertex_array_range2
#define GL_NV_vertex_array_range2 1

#define GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV 0x8533

#define GLEW_NV_vertex_array_range2 GLEW_GET_VAR(__GLEW_NV_vertex_array_range2)

#endif /* GL_NV_vertex_array_range2 */

/* ------------------- GL_NV_vertex_attrib_integer_64bit ------------------- */

#ifndef GL_NV_vertex_attrib_integer_64bit
#define GL_NV_vertex_attrib_integer_64bit 1

#define GL_INT64_NV 0x140E
#define GL_UNSIGNED_INT64_NV 0x140F

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLI64VNVPROC) (GLuint index, GLenum pname, GLint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLUI64VNVPROC) (GLuint index, GLenum pname, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1I64NVPROC) (GLuint index, GLint64EXT x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1UI64NVPROC) (GLuint index, GLuint64EXT x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);


#define GLEW_NV_vertex_attrib_integer_64bit GLEW_GET_VAR(__GLEW_NV_vertex_attrib_integer_64bit)

#endif /* GL_NV_vertex_attrib_integer_64bit */

/* ------------------- GL_NV_vertex_buffer_unified_memory ------------------ */

#ifndef GL_NV_vertex_buffer_unified_memory
#define GL_NV_vertex_buffer_unified_memory 1

#define GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV 0x8F1E
#define GL_ELEMENT_ARRAY_UNIFIED_NV 0x8F1F
#define GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV 0x8F20
#define GL_VERTEX_ARRAY_ADDRESS_NV 0x8F21
#define GL_NORMAL_ARRAY_ADDRESS_NV 0x8F22
#define GL_COLOR_ARRAY_ADDRESS_NV 0x8F23
#define GL_INDEX_ARRAY_ADDRESS_NV 0x8F24
#define GL_TEXTURE_COORD_ARRAY_ADDRESS_NV 0x8F25
#define GL_EDGE_FLAG_ARRAY_ADDRESS_NV 0x8F26
#define GL_SECONDARY_COLOR_ARRAY_ADDRESS_NV 0x8F27
#define GL_FOG_COORD_ARRAY_ADDRESS_NV 0x8F28
#define GL_ELEMENT_ARRAY_ADDRESS_NV 0x8F29
#define GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV 0x8F2A
#define GL_VERTEX_ARRAY_LENGTH_NV 0x8F2B
#define GL_NORMAL_ARRAY_LENGTH_NV 0x8F2C
#define GL_COLOR_ARRAY_LENGTH_NV 0x8F2D
#define GL_INDEX_ARRAY_LENGTH_NV 0x8F2E
#define GL_TEXTURE_COORD_ARRAY_LENGTH_NV 0x8F2F
#define GL_EDGE_FLAG_ARRAY_LENGTH_NV 0x8F30
#define GL_SECONDARY_COLOR_ARRAY_LENGTH_NV 0x8F31
#define GL_FOG_COORD_ARRAY_LENGTH_NV 0x8F32
#define GL_ELEMENT_ARRAY_LENGTH_NV 0x8F33
#define GL_DRAW_INDIRECT_UNIFIED_NV 0x8F40
#define GL_DRAW_INDIRECT_ADDRESS_NV 0x8F41
#define GL_DRAW_INDIRECT_LENGTH_NV 0x8F42

typedef void (GLAPIENTRY * PFNGLBUFFERADDRESSRANGENVPROC) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGFORMATNVPROC) (GLsizei stride);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLGETINTEGERUI64I_VNVPROC) (GLenum value, GLuint index, GLuint64EXT result[]);
typedef void (GLAPIENTRY * PFNGLINDEXFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLNORMALFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLTEXCOORDFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);


#define GLEW_NV_vertex_buffer_unified_memory GLEW_GET_VAR(__GLEW_NV_vertex_buffer_unified_memory)

#endif /* GL_NV_vertex_buffer_unified_memory */

/* -------------------------- GL_NV_vertex_program ------------------------- */

#ifndef GL_NV_vertex_program
#define GL_NV_vertex_program 1

#define GL_VERTEX_PROGRAM_NV 0x8620
#define GL_VERTEX_STATE_PROGRAM_NV 0x8621
#define GL_ATTRIB_ARRAY_SIZE_NV 0x8623
#define GL_ATTRIB_ARRAY_STRIDE_NV 0x8624
#define GL_ATTRIB_ARRAY_TYPE_NV 0x8625
#define GL_CURRENT_ATTRIB_NV 0x8626
#define GL_PROGRAM_LENGTH_NV 0x8627
#define GL_PROGRAM_STRING_NV 0x8628
#define GL_MODELVIEW_PROJECTION_NV 0x8629
#define GL_IDENTITY_NV 0x862A
#define GL_INVERSE_NV 0x862B
#define GL_TRANSPOSE_NV 0x862C
#define GL_INVERSE_TRANSPOSE_NV 0x862D
#define GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV 0x862E
#define GL_MAX_TRACK_MATRICES_NV 0x862F
#define GL_MATRIX0_NV 0x8630
#define GL_MATRIX1_NV 0x8631
#define GL_MATRIX2_NV 0x8632
#define GL_MATRIX3_NV 0x8633
#define GL_MATRIX4_NV 0x8634
#define GL_MATRIX5_NV 0x8635
#define GL_MATRIX6_NV 0x8636
#define GL_MATRIX7_NV 0x8637
#define GL_CURRENT_MATRIX_STACK_DEPTH_NV 0x8640
#define GL_CURRENT_MATRIX_NV 0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_NV 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_NV 0x8643
#define GL_PROGRAM_PARAMETER_NV 0x8644
#define GL_ATTRIB_ARRAY_POINTER_NV 0x8645
#define GL_PROGRAM_TARGET_NV 0x8646
#define GL_PROGRAM_RESIDENT_NV 0x8647
#define GL_TRACK_MATRIX_NV 0x8648
#define GL_TRACK_MATRIX_TRANSFORM_NV 0x8649
#define GL_VERTEX_PROGRAM_BINDING_NV 0x864A
#define GL_PROGRAM_ERROR_POSITION_NV 0x864B
#define GL_VERTEX_ATTRIB_ARRAY0_NV 0x8650
#define GL_VERTEX_ATTRIB_ARRAY1_NV 0x8651
#define GL_VERTEX_ATTRIB_ARRAY2_NV 0x8652
#define GL_VERTEX_ATTRIB_ARRAY3_NV 0x8653
#define GL_VERTEX_ATTRIB_ARRAY4_NV 0x8654
#define GL_VERTEX_ATTRIB_ARRAY5_NV 0x8655
#define GL_VERTEX_ATTRIB_ARRAY6_NV 0x8656
#define GL_VERTEX_ATTRIB_ARRAY7_NV 0x8657
#define GL_VERTEX_ATTRIB_ARRAY8_NV 0x8658
#define GL_VERTEX_ATTRIB_ARRAY9_NV 0x8659
#define GL_VERTEX_ATTRIB_ARRAY10_NV 0x865A
#define GL_VERTEX_ATTRIB_ARRAY11_NV 0x865B
#define GL_VERTEX_ATTRIB_ARRAY12_NV 0x865C
#define GL_VERTEX_ATTRIB_ARRAY13_NV 0x865D
#define GL_VERTEX_ATTRIB_ARRAY14_NV 0x865E
#define GL_VERTEX_ATTRIB_ARRAY15_NV 0x865F
#define GL_MAP1_VERTEX_ATTRIB0_4_NV 0x8660
#define GL_MAP1_VERTEX_ATTRIB1_4_NV 0x8661
#define GL_MAP1_VERTEX_ATTRIB2_4_NV 0x8662
#define GL_MAP1_VERTEX_ATTRIB3_4_NV 0x8663
#define GL_MAP1_VERTEX_ATTRIB4_4_NV 0x8664
#define GL_MAP1_VERTEX_ATTRIB5_4_NV 0x8665
#define GL_MAP1_VERTEX_ATTRIB6_4_NV 0x8666
#define GL_MAP1_VERTEX_ATTRIB7_4_NV 0x8667
#define GL_MAP1_VERTEX_ATTRIB8_4_NV 0x8668
#define GL_MAP1_VERTEX_ATTRIB9_4_NV 0x8669
#define GL_MAP1_VERTEX_ATTRIB10_4_NV 0x866A
#define GL_MAP1_VERTEX_ATTRIB11_4_NV 0x866B
#define GL_MAP1_VERTEX_ATTRIB12_4_NV 0x866C
#define GL_MAP1_VERTEX_ATTRIB13_4_NV 0x866D
#define GL_MAP1_VERTEX_ATTRIB14_4_NV 0x866E
#define GL_MAP1_VERTEX_ATTRIB15_4_NV 0x866F
#define GL_MAP2_VERTEX_ATTRIB0_4_NV 0x8670
#define GL_MAP2_VERTEX_ATTRIB1_4_NV 0x8671
#define GL_MAP2_VERTEX_ATTRIB2_4_NV 0x8672
#define GL_MAP2_VERTEX_ATTRIB3_4_NV 0x8673
#define GL_MAP2_VERTEX_ATTRIB4_4_NV 0x8674
#define GL_MAP2_VERTEX_ATTRIB5_4_NV 0x8675
#define GL_MAP2_VERTEX_ATTRIB6_4_NV 0x8676
#define GL_MAP2_VERTEX_ATTRIB7_4_NV 0x8677
#define GL_MAP2_VERTEX_ATTRIB8_4_NV 0x8678
#define GL_MAP2_VERTEX_ATTRIB9_4_NV 0x8679
#define GL_MAP2_VERTEX_ATTRIB10_4_NV 0x867A
#define GL_MAP2_VERTEX_ATTRIB11_4_NV 0x867B
#define GL_MAP2_VERTEX_ATTRIB12_4_NV 0x867C
#define GL_MAP2_VERTEX_ATTRIB13_4_NV 0x867D
#define GL_MAP2_VERTEX_ATTRIB14_4_NV 0x867E
#define GL_MAP2_VERTEX_ATTRIB15_4_NV 0x867F

typedef GLboolean (GLAPIENTRY * PFNGLAREPROGRAMSRESIDENTNVPROC) (GLsizei n, const GLuint* ids, GLboolean *residences);
typedef void (GLAPIENTRY * PFNGLBINDPROGRAMNVPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMSNVPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLEXECUTEPROGRAMNVPROC) (GLenum target, GLuint id, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMSNVPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPARAMETERDVNVPROC) (GLenum target, GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTRINGNVPROC) (GLuint id, GLenum pname, GLubyte* program);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVNVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTRACKMATRIXIVNVPROC) (GLenum target, GLuint address, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVNVPROC) (GLuint index, GLenum pname, GLvoid** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVNVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVNVPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVNVPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMNVPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLLOADPROGRAMNVPROC) (GLenum target, GLuint id, GLsizei len, const GLubyte* program);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4DNVPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4DVNVPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4FNVPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4FVNVPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERS4DVNVPROC) (GLenum target, GLuint index, GLsizei num, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERS4FVNVPROC) (GLenum target, GLuint index, GLsizei num, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLREQUESTRESIDENTPROGRAMSNVPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLTRACKMATRIXNVPROC) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DNVPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FNVPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SNVPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DNVPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FNVPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SNVPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBNVPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVNVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4UBVNVPROC) (GLuint index, GLsizei n, const GLubyte* v);


#define GLEW_NV_vertex_program GLEW_GET_VAR(__GLEW_NV_vertex_program)

#endif /* GL_NV_vertex_program */

/* ------------------------ GL_NV_vertex_program1_1 ------------------------ */

#ifndef GL_NV_vertex_program1_1
#define GL_NV_vertex_program1_1 1

#define GLEW_NV_vertex_program1_1 GLEW_GET_VAR(__GLEW_NV_vertex_program1_1)

#endif /* GL_NV_vertex_program1_1 */

/* ------------------------- GL_NV_vertex_program2 ------------------------- */

#ifndef GL_NV_vertex_program2
#define GL_NV_vertex_program2 1

#define GLEW_NV_vertex_program2 GLEW_GET_VAR(__GLEW_NV_vertex_program2)

#endif /* GL_NV_vertex_program2 */

/* ---------------------- GL_NV_vertex_program2_option --------------------- */

#ifndef GL_NV_vertex_program2_option
#define GL_NV_vertex_program2_option 1

#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV 0x88F5

#define GLEW_NV_vertex_program2_option GLEW_GET_VAR(__GLEW_NV_vertex_program2_option)

#endif /* GL_NV_vertex_program2_option */

/* ------------------------- GL_NV_vertex_program3 ------------------------- */

#ifndef GL_NV_vertex_program3
#define GL_NV_vertex_program3 1

#define MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C

#define GLEW_NV_vertex_program3 GLEW_GET_VAR(__GLEW_NV_vertex_program3)

#endif /* GL_NV_vertex_program3 */

/* ------------------------- GL_NV_vertex_program4 ------------------------- */

#ifndef GL_NV_vertex_program4
#define GL_NV_vertex_program4 1

#define GL_VERTEX_ATTRIB_ARRAY_INTEGER_NV 0x88FD

#define GLEW_NV_vertex_program4 GLEW_GET_VAR(__GLEW_NV_vertex_program4)

#endif /* GL_NV_vertex_program4 */

/* -------------------------- GL_NV_video_capture -------------------------- */

#ifndef GL_NV_video_capture
#define GL_NV_video_capture 1

#define GL_VIDEO_BUFFER_NV 0x9020
#define GL_VIDEO_BUFFER_BINDING_NV 0x9021
#define GL_FIELD_UPPER_NV 0x9022
#define GL_FIELD_LOWER_NV 0x9023
#define GL_NUM_VIDEO_CAPTURE_STREAMS_NV 0x9024
#define GL_NEXT_VIDEO_CAPTURE_BUFFER_STATUS_NV 0x9025
#define GL_VIDEO_CAPTURE_TO_422_SUPPORTED_NV 0x9026
#define GL_LAST_VIDEO_CAPTURE_STATUS_NV 0x9027
#define GL_VIDEO_BUFFER_PITCH_NV 0x9028
#define GL_VIDEO_COLOR_CONVERSION_MATRIX_NV 0x9029
#define GL_VIDEO_COLOR_CONVERSION_MAX_NV 0x902A
#define GL_VIDEO_COLOR_CONVERSION_MIN_NV 0x902B
#define GL_VIDEO_COLOR_CONVERSION_OFFSET_NV 0x902C
#define GL_VIDEO_BUFFER_INTERNAL_FORMAT_NV 0x902D
#define GL_PARTIAL_SUCCESS_NV 0x902E
#define GL_SUCCESS_NV 0x902F
#define GL_FAILURE_NV 0x9030
#define GL_YCBYCR8_422_NV 0x9031
#define GL_YCBAYCR8A_4224_NV 0x9032
#define GL_Z6Y10Z6CB10Z6Y10Z6CR10_422_NV 0x9033
#define GL_Z6Y10Z6CB10Z6A10Z6Y10Z6CR10Z6A10_4224_NV 0x9034
#define GL_Z4Y12Z4CB12Z4Y12Z4CR12_422_NV 0x9035
#define GL_Z4Y12Z4CB12Z4A12Z4Y12Z4CR12Z4A12_4224_NV 0x9036
#define GL_Z4Y12Z4CB12Z4CR12_444_NV 0x9037
#define GL_VIDEO_CAPTURE_FRAME_WIDTH_NV 0x9038
#define GL_VIDEO_CAPTURE_FRAME_HEIGHT_NV 0x9039
#define GL_VIDEO_CAPTURE_FIELD_UPPER_HEIGHT_NV 0x903A
#define GL_VIDEO_CAPTURE_FIELD_LOWER_HEIGHT_NV 0x903B
#define GL_VIDEO_CAPTURE_SURFACE_ORIGIN_NV 0x903C

typedef void (GLAPIENTRY * PFNGLBEGINVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void (GLAPIENTRY * PFNGLBINDVIDEOCAPTURESTREAMBUFFERNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
typedef void (GLAPIENTRY * PFNGLBINDVIDEOCAPTURESTREAMTEXTURENVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLENDVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTURESTREAMDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTURESTREAMFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTURESTREAMIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTUREIVNVPROC) (GLuint video_capture_slot, GLenum pname, GLint* params);
typedef GLenum (GLAPIENTRY * PFNGLVIDEOCAPTURENVPROC) (GLuint video_capture_slot, GLuint* sequence_num, GLuint64EXT *capture_time);
typedef void (GLAPIENTRY * PFNGLVIDEOCAPTURESTREAMPARAMETERDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVIDEOCAPTURESTREAMPARAMETERFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLVIDEOCAPTURESTREAMPARAMETERIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint* params);


#define GLEW_NV_video_capture GLEW_GET_VAR(__GLEW_NV_video_capture)

#endif /* GL_NV_video_capture */

/* ------------------------ GL_OES_byte_coordinates ------------------------ */

#ifndef GL_OES_byte_coordinates
#define GL_OES_byte_coordinates 1

#define GL_BYTE 0x1400

#define GLEW_OES_byte_coordinates GLEW_GET_VAR(__GLEW_OES_byte_coordinates)

#endif /* GL_OES_byte_coordinates */

/* ------------------- GL_OES_compressed_paletted_texture ------------------ */

#ifndef GL_OES_compressed_paletted_texture
#define GL_OES_compressed_paletted_texture 1

#define GL_PALETTE4_RGB8_OES 0x8B90
#define GL_PALETTE4_RGBA8_OES 0x8B91
#define GL_PALETTE4_R5_G6_B5_OES 0x8B92
#define GL_PALETTE4_RGBA4_OES 0x8B93
#define GL_PALETTE4_RGB5_A1_OES 0x8B94
#define GL_PALETTE8_RGB8_OES 0x8B95
#define GL_PALETTE8_RGBA8_OES 0x8B96
#define GL_PALETTE8_R5_G6_B5_OES 0x8B97
#define GL_PALETTE8_RGBA4_OES 0x8B98
#define GL_PALETTE8_RGB5_A1_OES 0x8B99

#define GLEW_OES_compressed_paletted_texture GLEW_GET_VAR(__GLEW_OES_compressed_paletted_texture)

#endif /* GL_OES_compressed_paletted_texture */

/* --------------------------- GL_OES_read_format -------------------------- */

#ifndef GL_OES_read_format
#define GL_OES_read_format 1

#define GL_IMPLEMENTATION_COLOR_READ_TYPE_OES 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES 0x8B9B

#define GLEW_OES_read_format GLEW_GET_VAR(__GLEW_OES_read_format)

#endif /* GL_OES_read_format */

/* ------------------------ GL_OES_single_precision ------------------------ */

#ifndef GL_OES_single_precision
#define GL_OES_single_precision 1

typedef void (GLAPIENTRY * PFNGLCLEARDEPTHFOESPROC) (GLclampd depth);
typedef void (GLAPIENTRY * PFNGLCLIPPLANEFOESPROC) (GLenum plane, const GLfloat* equation);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEFOESPROC) (GLclampf n, GLclampf f);
typedef void (GLAPIENTRY * PFNGLFRUSTUMFOESPROC) (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
typedef void (GLAPIENTRY * PFNGLGETCLIPPLANEFOESPROC) (GLenum plane, GLfloat* equation);
typedef void (GLAPIENTRY * PFNGLORTHOFOESPROC) (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);


#define GLEW_OES_single_precision GLEW_GET_VAR(__GLEW_OES_single_precision)

#endif /* GL_OES_single_precision */

/* ---------------------------- GL_OML_interlace --------------------------- */

#ifndef GL_OML_interlace
#define GL_OML_interlace 1

#define GL_INTERLACE_OML 0x8980
#define GL_INTERLACE_READ_OML 0x8981

#define GLEW_OML_interlace GLEW_GET_VAR(__GLEW_OML_interlace)

#endif /* GL_OML_interlace */

/* ---------------------------- GL_OML_resample ---------------------------- */

#ifndef GL_OML_resample
#define GL_OML_resample 1

#define GL_PACK_RESAMPLE_OML 0x8984
#define GL_UNPACK_RESAMPLE_OML 0x8985
#define GL_RESAMPLE_REPLICATE_OML 0x8986
#define GL_RESAMPLE_ZERO_FILL_OML 0x8987
#define GL_RESAMPLE_AVERAGE_OML 0x8988
#define GL_RESAMPLE_DECIMATE_OML 0x8989

#define GLEW_OML_resample GLEW_GET_VAR(__GLEW_OML_resample)

#endif /* GL_OML_resample */

/* ---------------------------- GL_OML_subsample --------------------------- */

#ifndef GL_OML_subsample
#define GL_OML_subsample 1

#define GL_FORMAT_SUBSAMPLE_24_24_OML 0x8982
#define GL_FORMAT_SUBSAMPLE_244_244_OML 0x8983

#define GLEW_OML_subsample GLEW_GET_VAR(__GLEW_OML_subsample)

#endif /* GL_OML_subsample */

/* --------------------------- GL_PGI_misc_hints --------------------------- */

#ifndef GL_PGI_misc_hints
#define GL_PGI_misc_hints 1

#define GL_PREFER_DOUBLEBUFFER_HINT_PGI 107000
#define GL_CONSERVE_MEMORY_HINT_PGI 107005
#define GL_RECLAIM_MEMORY_HINT_PGI 107006
#define GL_NATIVE_GRAPHICS_HANDLE_PGI 107010
#define GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI 107011
#define GL_NATIVE_GRAPHICS_END_HINT_PGI 107012
#define GL_ALWAYS_FAST_HINT_PGI 107020
#define GL_ALWAYS_SOFT_HINT_PGI 107021
#define GL_ALLOW_DRAW_OBJ_HINT_PGI 107022
#define GL_ALLOW_DRAW_WIN_HINT_PGI 107023
#define GL_ALLOW_DRAW_FRG_HINT_PGI 107024
#define GL_ALLOW_DRAW_MEM_HINT_PGI 107025
#define GL_STRICT_DEPTHFUNC_HINT_PGI 107030
#define GL_STRICT_LIGHTING_HINT_PGI 107031
#define GL_STRICT_SCISSOR_HINT_PGI 107032
#define GL_FULL_STIPPLE_HINT_PGI 107033
#define GL_CLIP_NEAR_HINT_PGI 107040
#define GL_CLIP_FAR_HINT_PGI 107041
#define GL_WIDE_LINE_HINT_PGI 107042
#define GL_BACK_NORMALS_HINT_PGI 107043

#define GLEW_PGI_misc_hints GLEW_GET_VAR(__GLEW_PGI_misc_hints)

#endif /* GL_PGI_misc_hints */

/* -------------------------- GL_PGI_vertex_hints -------------------------- */

#ifndef GL_PGI_vertex_hints
#define GL_PGI_vertex_hints 1

#define GL_VERTEX23_BIT_PGI 0x00000004
#define GL_VERTEX4_BIT_PGI 0x00000008
#define GL_COLOR3_BIT_PGI 0x00010000
#define GL_COLOR4_BIT_PGI 0x00020000
#define GL_EDGEFLAG_BIT_PGI 0x00040000
#define GL_INDEX_BIT_PGI 0x00080000
#define GL_MAT_AMBIENT_BIT_PGI 0x00100000
#define GL_VERTEX_DATA_HINT_PGI 107050
#define GL_VERTEX_CONSISTENT_HINT_PGI 107051
#define GL_MATERIAL_SIDE_HINT_PGI 107052
#define GL_MAX_VERTEX_HINT_PGI 107053
#define GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI 0x00200000
#define GL_MAT_DIFFUSE_BIT_PGI 0x00400000
#define GL_MAT_EMISSION_BIT_PGI 0x00800000
#define GL_MAT_COLOR_INDEXES_BIT_PGI 0x01000000
#define GL_MAT_SHININESS_BIT_PGI 0x02000000
#define GL_MAT_SPECULAR_BIT_PGI 0x04000000
#define GL_NORMAL_BIT_PGI 0x08000000
#define GL_TEXCOORD1_BIT_PGI 0x10000000
#define GL_TEXCOORD2_BIT_PGI 0x20000000
#define GL_TEXCOORD3_BIT_PGI 0x40000000
#define GL_TEXCOORD4_BIT_PGI 0x80000000

#define GLEW_PGI_vertex_hints GLEW_GET_VAR(__GLEW_PGI_vertex_hints)

#endif /* GL_PGI_vertex_hints */

/* ------------------------- GL_REGAL_error_string ------------------------- */

#ifndef GL_REGAL_error_string
#define GL_REGAL_error_string 1

typedef const GLchar* (GLAPIENTRY * PFNGLERRORSTRINGREGALPROC) (GLenum error);


#define GLEW_REGAL_error_string GLEW_GET_VAR(__GLEW_REGAL_error_string)

#endif /* GL_REGAL_error_string */

/* ------------------------ GL_REGAL_extension_query ----------------------- */

#ifndef GL_REGAL_extension_query
#define GL_REGAL_extension_query 1

typedef GLboolean (GLAPIENTRY * PFNGLGETEXTENSIONREGALPROC) (const GLchar* ext);
typedef GLboolean (GLAPIENTRY * PFNGLISSUPPORTEDREGALPROC) (const GLchar* ext);


#define GLEW_REGAL_extension_query GLEW_GET_VAR(__GLEW_REGAL_extension_query)

#endif /* GL_REGAL_extension_query */

/* ------------------------------ GL_REGAL_log ----------------------------- */

#ifndef GL_REGAL_log
#define GL_REGAL_log 1

#define GL_LOG_ERROR_REGAL 0x9319
#define GL_LOG_WARNING_REGAL 0x931A
#define GL_LOG_INFO_REGAL 0x931B
#define GL_LOG_APP_REGAL 0x931C
#define GL_LOG_DRIVER_REGAL 0x931D
#define GL_LOG_INTERNAL_REGAL 0x931E
#define GL_LOG_DEBUG_REGAL 0x931F
#define GL_LOG_STATUS_REGAL 0x9320
#define GL_LOG_HTTP_REGAL 0x9321

#define GLEW_REGAL_log GLEW_GET_VAR(__GLEW_REGAL_log)

#endif /* GL_REGAL_log */

/* ----------------------- GL_REND_screen_coordinates ---------------------- */

#ifndef GL_REND_screen_coordinates
#define GL_REND_screen_coordinates 1

#define GL_SCREEN_COORDINATES_REND 0x8490
#define GL_INVERTED_SCREEN_W_REND 0x8491

#define GLEW_REND_screen_coordinates GLEW_GET_VAR(__GLEW_REND_screen_coordinates)

#endif /* GL_REND_screen_coordinates */

/* ------------------------------- GL_S3_s3tc ------------------------------ */

#ifndef GL_S3_s3tc
#define GL_S3_s3tc 1

#define GL_RGB_S3TC 0x83A0
#define GL_RGB4_S3TC 0x83A1
#define GL_RGBA_S3TC 0x83A2
#define GL_RGBA4_S3TC 0x83A3
#define GL_RGBA_DXT5_S3TC 0x83A4
#define GL_RGBA4_DXT5_S3TC 0x83A5

#define GLEW_S3_s3tc GLEW_GET_VAR(__GLEW_S3_s3tc)

#endif /* GL_S3_s3tc */

/* -------------------------- GL_SGIS_color_range -------------------------- */

#ifndef GL_SGIS_color_range
#define GL_SGIS_color_range 1

#define GL_EXTENDED_RANGE_SGIS 0x85A5
#define GL_MIN_RED_SGIS 0x85A6
#define GL_MAX_RED_SGIS 0x85A7
#define GL_MIN_GREEN_SGIS 0x85A8
#define GL_MAX_GREEN_SGIS 0x85A9
#define GL_MIN_BLUE_SGIS 0x85AA
#define GL_MAX_BLUE_SGIS 0x85AB
#define GL_MIN_ALPHA_SGIS 0x85AC
#define GL_MAX_ALPHA_SGIS 0x85AD

#define GLEW_SGIS_color_range GLEW_GET_VAR(__GLEW_SGIS_color_range)

#endif /* GL_SGIS_color_range */

/* ------------------------- GL_SGIS_detail_texture ------------------------ */

#ifndef GL_SGIS_detail_texture
#define GL_SGIS_detail_texture 1

typedef void (GLAPIENTRY * PFNGLDETAILTEXFUNCSGISPROC) (GLenum target, GLsizei n, const GLfloat* points);
typedef void (GLAPIENTRY * PFNGLGETDETAILTEXFUNCSGISPROC) (GLenum target, GLfloat* points);


#define GLEW_SGIS_detail_texture GLEW_GET_VAR(__GLEW_SGIS_detail_texture)

#endif /* GL_SGIS_detail_texture */

/* -------------------------- GL_SGIS_fog_function ------------------------- */

#ifndef GL_SGIS_fog_function
#define GL_SGIS_fog_function 1

typedef void (GLAPIENTRY * PFNGLFOGFUNCSGISPROC) (GLsizei n, const GLfloat* points);
typedef void (GLAPIENTRY * PFNGLGETFOGFUNCSGISPROC) (GLfloat* points);


#define GLEW_SGIS_fog_function GLEW_GET_VAR(__GLEW_SGIS_fog_function)

#endif /* GL_SGIS_fog_function */

/* ------------------------ GL_SGIS_generate_mipmap ------------------------ */

#ifndef GL_SGIS_generate_mipmap
#define GL_SGIS_generate_mipmap 1

#define GL_GENERATE_MIPMAP_SGIS 0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS 0x8192

#define GLEW_SGIS_generate_mipmap GLEW_GET_VAR(__GLEW_SGIS_generate_mipmap)

#endif /* GL_SGIS_generate_mipmap */

/* -------------------------- GL_SGIS_multisample -------------------------- */

#ifndef GL_SGIS_multisample
#define GL_SGIS_multisample 1

#define GL_MULTISAMPLE_SGIS 0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_SGIS 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_SGIS 0x809F
#define GL_SAMPLE_MASK_SGIS 0x80A0
#define GL_1PASS_SGIS 0x80A1
#define GL_2PASS_0_SGIS 0x80A2
#define GL_2PASS_1_SGIS 0x80A3
#define GL_4PASS_0_SGIS 0x80A4
#define GL_4PASS_1_SGIS 0x80A5
#define GL_4PASS_2_SGIS 0x80A6
#define GL_4PASS_3_SGIS 0x80A7
#define GL_SAMPLE_BUFFERS_SGIS 0x80A8
#define GL_SAMPLES_SGIS 0x80A9
#define GL_SAMPLE_MASK_VALUE_SGIS 0x80AA
#define GL_SAMPLE_MASK_INVERT_SGIS 0x80AB
#define GL_SAMPLE_PATTERN_SGIS 0x80AC

typedef void (GLAPIENTRY * PFNGLSAMPLEMASKSGISPROC) (GLclampf value, GLboolean invert);
typedef void (GLAPIENTRY * PFNGLSAMPLEPATTERNSGISPROC) (GLenum pattern);


#define GLEW_SGIS_multisample GLEW_GET_VAR(__GLEW_SGIS_multisample)

#endif /* GL_SGIS_multisample */

/* ------------------------- GL_SGIS_pixel_texture ------------------------- */

#ifndef GL_SGIS_pixel_texture
#define GL_SGIS_pixel_texture 1

#define GLEW_SGIS_pixel_texture GLEW_GET_VAR(__GLEW_SGIS_pixel_texture)

#endif /* GL_SGIS_pixel_texture */

/* ----------------------- GL_SGIS_point_line_texgen ----------------------- */

#ifndef GL_SGIS_point_line_texgen
#define GL_SGIS_point_line_texgen 1

#define GL_EYE_DISTANCE_TO_POINT_SGIS 0x81F0
#define GL_OBJECT_DISTANCE_TO_POINT_SGIS 0x81F1
#define GL_EYE_DISTANCE_TO_LINE_SGIS 0x81F2
#define GL_OBJECT_DISTANCE_TO_LINE_SGIS 0x81F3
#define GL_EYE_POINT_SGIS 0x81F4
#define GL_OBJECT_POINT_SGIS 0x81F5
#define GL_EYE_LINE_SGIS 0x81F6
#define GL_OBJECT_LINE_SGIS 0x81F7

#define GLEW_SGIS_point_line_texgen GLEW_GET_VAR(__GLEW_SGIS_point_line_texgen)

#endif /* GL_SGIS_point_line_texgen */

/* ------------------------ GL_SGIS_sharpen_texture ------------------------ */

#ifndef GL_SGIS_sharpen_texture
#define GL_SGIS_sharpen_texture 1

typedef void (GLAPIENTRY * PFNGLGETSHARPENTEXFUNCSGISPROC) (GLenum target, GLfloat* points);
typedef void (GLAPIENTRY * PFNGLSHARPENTEXFUNCSGISPROC) (GLenum target, GLsizei n, const GLfloat* points);


#define GLEW_SGIS_sharpen_texture GLEW_GET_VAR(__GLEW_SGIS_sharpen_texture)

#endif /* GL_SGIS_sharpen_texture */

/* --------------------------- GL_SGIS_texture4D --------------------------- */

#ifndef GL_SGIS_texture4D
#define GL_SGIS_texture4D 1

typedef void (GLAPIENTRY * PFNGLTEXIMAGE4DSGISPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE4DSGISPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLenum format, GLenum type, const void* pixels);


#define GLEW_SGIS_texture4D GLEW_GET_VAR(__GLEW_SGIS_texture4D)

#endif /* GL_SGIS_texture4D */

/* ---------------------- GL_SGIS_texture_border_clamp --------------------- */

#ifndef GL_SGIS_texture_border_clamp
#define GL_SGIS_texture_border_clamp 1

#define GL_CLAMP_TO_BORDER_SGIS 0x812D

#define GLEW_SGIS_texture_border_clamp GLEW_GET_VAR(__GLEW_SGIS_texture_border_clamp)

#endif /* GL_SGIS_texture_border_clamp */

/* ----------------------- GL_SGIS_texture_edge_clamp ---------------------- */

#ifndef GL_SGIS_texture_edge_clamp
#define GL_SGIS_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE_SGIS 0x812F

#define GLEW_SGIS_texture_edge_clamp GLEW_GET_VAR(__GLEW_SGIS_texture_edge_clamp)

#endif /* GL_SGIS_texture_edge_clamp */

/* ------------------------ GL_SGIS_texture_filter4 ------------------------ */

#ifndef GL_SGIS_texture_filter4
#define GL_SGIS_texture_filter4 1

typedef void (GLAPIENTRY * PFNGLGETTEXFILTERFUNCSGISPROC) (GLenum target, GLenum filter, GLfloat* weights);
typedef void (GLAPIENTRY * PFNGLTEXFILTERFUNCSGISPROC) (GLenum target, GLenum filter, GLsizei n, const GLfloat* weights);


#define GLEW_SGIS_texture_filter4 GLEW_GET_VAR(__GLEW_SGIS_texture_filter4)

#endif /* GL_SGIS_texture_filter4 */

/* -------------------------- GL_SGIS_texture_lod -------------------------- */

#ifndef GL_SGIS_texture_lod
#define GL_SGIS_texture_lod 1

#define GL_TEXTURE_MIN_LOD_SGIS 0x813A
#define GL_TEXTURE_MAX_LOD_SGIS 0x813B
#define GL_TEXTURE_BASE_LEVEL_SGIS 0x813C
#define GL_TEXTURE_MAX_LEVEL_SGIS 0x813D

#define GLEW_SGIS_texture_lod GLEW_GET_VAR(__GLEW_SGIS_texture_lod)

#endif /* GL_SGIS_texture_lod */

/* ------------------------- GL_SGIS_texture_select ------------------------ */

#ifndef GL_SGIS_texture_select
#define GL_SGIS_texture_select 1

#define GLEW_SGIS_texture_select GLEW_GET_VAR(__GLEW_SGIS_texture_select)

#endif /* GL_SGIS_texture_select */

/* ----------------------------- GL_SGIX_async ----------------------------- */

#ifndef GL_SGIX_async
#define GL_SGIX_async 1

#define GL_ASYNC_MARKER_SGIX 0x8329

typedef void (GLAPIENTRY * PFNGLASYNCMARKERSGIXPROC) (GLuint marker);
typedef void (GLAPIENTRY * PFNGLDELETEASYNCMARKERSSGIXPROC) (GLuint marker, GLsizei range);
typedef GLint (GLAPIENTRY * PFNGLFINISHASYNCSGIXPROC) (GLuint* markerp);
typedef GLuint (GLAPIENTRY * PFNGLGENASYNCMARKERSSGIXPROC) (GLsizei range);
typedef GLboolean (GLAPIENTRY * PFNGLISASYNCMARKERSGIXPROC) (GLuint marker);
typedef GLint (GLAPIENTRY * PFNGLPOLLASYNCSGIXPROC) (GLuint* markerp);


#define GLEW_SGIX_async GLEW_GET_VAR(__GLEW_SGIX_async)

#endif /* GL_SGIX_async */

/* ------------------------ GL_SGIX_async_histogram ------------------------ */

#ifndef GL_SGIX_async_histogram
#define GL_SGIX_async_histogram 1

#define GL_ASYNC_HISTOGRAM_SGIX 0x832C
#define GL_MAX_ASYNC_HISTOGRAM_SGIX 0x832D

#define GLEW_SGIX_async_histogram GLEW_GET_VAR(__GLEW_SGIX_async_histogram)

#endif /* GL_SGIX_async_histogram */

/* -------------------------- GL_SGIX_async_pixel -------------------------- */

#ifndef GL_SGIX_async_pixel
#define GL_SGIX_async_pixel 1

#define GL_ASYNC_TEX_IMAGE_SGIX 0x835C
#define GL_ASYNC_DRAW_PIXELS_SGIX 0x835D
#define GL_ASYNC_READ_PIXELS_SGIX 0x835E
#define GL_MAX_ASYNC_TEX_IMAGE_SGIX 0x835F
#define GL_MAX_ASYNC_DRAW_PIXELS_SGIX 0x8360
#define GL_MAX_ASYNC_READ_PIXELS_SGIX 0x8361

#define GLEW_SGIX_async_pixel GLEW_GET_VAR(__GLEW_SGIX_async_pixel)

#endif /* GL_SGIX_async_pixel */

/* ----------------------- GL_SGIX_blend_alpha_minmax ---------------------- */

#ifndef GL_SGIX_blend_alpha_minmax
#define GL_SGIX_blend_alpha_minmax 1

#define GL_ALPHA_MIN_SGIX 0x8320
#define GL_ALPHA_MAX_SGIX 0x8321

#define GLEW_SGIX_blend_alpha_minmax GLEW_GET_VAR(__GLEW_SGIX_blend_alpha_minmax)

#endif /* GL_SGIX_blend_alpha_minmax */

/* ---------------------------- GL_SGIX_clipmap ---------------------------- */

#ifndef GL_SGIX_clipmap
#define GL_SGIX_clipmap 1

#define GLEW_SGIX_clipmap GLEW_GET_VAR(__GLEW_SGIX_clipmap)

#endif /* GL_SGIX_clipmap */

/* ---------------------- GL_SGIX_convolution_accuracy --------------------- */

#ifndef GL_SGIX_convolution_accuracy
#define GL_SGIX_convolution_accuracy 1

#define GL_CONVOLUTION_HINT_SGIX 0x8316

#define GLEW_SGIX_convolution_accuracy GLEW_GET_VAR(__GLEW_SGIX_convolution_accuracy)

#endif /* GL_SGIX_convolution_accuracy */

/* ------------------------- GL_SGIX_depth_texture ------------------------- */

#ifndef GL_SGIX_depth_texture
#define GL_SGIX_depth_texture 1

#define GL_DEPTH_COMPONENT16_SGIX 0x81A5
#define GL_DEPTH_COMPONENT24_SGIX 0x81A6
#define GL_DEPTH_COMPONENT32_SGIX 0x81A7

#define GLEW_SGIX_depth_texture GLEW_GET_VAR(__GLEW_SGIX_depth_texture)

#endif /* GL_SGIX_depth_texture */

/* -------------------------- GL_SGIX_flush_raster ------------------------- */

#ifndef GL_SGIX_flush_raster
#define GL_SGIX_flush_raster 1

typedef void (GLAPIENTRY * PFNGLFLUSHRASTERSGIXPROC) (void);


#define GLEW_SGIX_flush_raster GLEW_GET_VAR(__GLEW_SGIX_flush_raster)

#endif /* GL_SGIX_flush_raster */

/* --------------------------- GL_SGIX_fog_offset -------------------------- */

#ifndef GL_SGIX_fog_offset
#define GL_SGIX_fog_offset 1

#define GL_FOG_OFFSET_SGIX 0x8198
#define GL_FOG_OFFSET_VALUE_SGIX 0x8199

#define GLEW_SGIX_fog_offset GLEW_GET_VAR(__GLEW_SGIX_fog_offset)

#endif /* GL_SGIX_fog_offset */

/* -------------------------- GL_SGIX_fog_texture -------------------------- */

#ifndef GL_SGIX_fog_texture
#define GL_SGIX_fog_texture 1

#define GL_TEXTURE_FOG_SGIX 0
#define GL_FOG_PATCHY_FACTOR_SGIX 0
#define GL_FRAGMENT_FOG_SGIX 0

typedef void (GLAPIENTRY * PFNGLTEXTUREFOGSGIXPROC) (GLenum pname);


#define GLEW_SGIX_fog_texture GLEW_GET_VAR(__GLEW_SGIX_fog_texture)

#endif /* GL_SGIX_fog_texture */

/* ------------------- GL_SGIX_fragment_specular_lighting ------------------ */

#ifndef GL_SGIX_fragment_specular_lighting
#define GL_SGIX_fragment_specular_lighting 1

typedef void (GLAPIENTRY * PFNGLFRAGMENTCOLORMATERIALSGIXPROC) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFSGIXPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFVSGIXPROC) (GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELISGIXPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIVSGIXPROC) (GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFSGIXPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFVSGIXPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTISGIXPROC) (GLenum light, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIVSGIXPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFSGIXPROC) (GLenum face, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFVSGIXPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALISGIXPROC) (GLenum face, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIVSGIXPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTFVSGIXPROC) (GLenum light, GLenum value, GLfloat* data);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTIVSGIXPROC) (GLenum light, GLenum value, GLint* data);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALFVSGIXPROC) (GLenum face, GLenum pname, const GLfloat* data);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALIVSGIXPROC) (GLenum face, GLenum pname, const GLint* data);


#define GLEW_SGIX_fragment_specular_lighting GLEW_GET_VAR(__GLEW_SGIX_fragment_specular_lighting)

#endif /* GL_SGIX_fragment_specular_lighting */

/* --------------------------- GL_SGIX_framezoom --------------------------- */

#ifndef GL_SGIX_framezoom
#define GL_SGIX_framezoom 1

typedef void (GLAPIENTRY * PFNGLFRAMEZOOMSGIXPROC) (GLint factor);


#define GLEW_SGIX_framezoom GLEW_GET_VAR(__GLEW_SGIX_framezoom)

#endif /* GL_SGIX_framezoom */

/* --------------------------- GL_SGIX_interlace --------------------------- */

#ifndef GL_SGIX_interlace
#define GL_SGIX_interlace 1

#define GL_INTERLACE_SGIX 0x8094

#define GLEW_SGIX_interlace GLEW_GET_VAR(__GLEW_SGIX_interlace)

#endif /* GL_SGIX_interlace */

/* ------------------------- GL_SGIX_ir_instrument1 ------------------------ */

#ifndef GL_SGIX_ir_instrument1
#define GL_SGIX_ir_instrument1 1

#define GLEW_SGIX_ir_instrument1 GLEW_GET_VAR(__GLEW_SGIX_ir_instrument1)

#endif /* GL_SGIX_ir_instrument1 */

/* ------------------------- GL_SGIX_list_priority ------------------------- */

#ifndef GL_SGIX_list_priority
#define GL_SGIX_list_priority 1

#define GLEW_SGIX_list_priority GLEW_GET_VAR(__GLEW_SGIX_list_priority)

#endif /* GL_SGIX_list_priority */

/* ------------------------- GL_SGIX_pixel_texture ------------------------- */

#ifndef GL_SGIX_pixel_texture
#define GL_SGIX_pixel_texture 1

typedef void (GLAPIENTRY * PFNGLPIXELTEXGENSGIXPROC) (GLenum mode);


#define GLEW_SGIX_pixel_texture GLEW_GET_VAR(__GLEW_SGIX_pixel_texture)

#endif /* GL_SGIX_pixel_texture */

/* ----------------------- GL_SGIX_pixel_texture_bits ---------------------- */

#ifndef GL_SGIX_pixel_texture_bits
#define GL_SGIX_pixel_texture_bits 1

#define GLEW_SGIX_pixel_texture_bits GLEW_GET_VAR(__GLEW_SGIX_pixel_texture_bits)

#endif /* GL_SGIX_pixel_texture_bits */

/* ------------------------ GL_SGIX_reference_plane ------------------------ */

#ifndef GL_SGIX_reference_plane
#define GL_SGIX_reference_plane 1

typedef void (GLAPIENTRY * PFNGLREFERENCEPLANESGIXPROC) (const GLdouble* equation);


#define GLEW_SGIX_reference_plane GLEW_GET_VAR(__GLEW_SGIX_reference_plane)

#endif /* GL_SGIX_reference_plane */

/* ---------------------------- GL_SGIX_resample --------------------------- */

#ifndef GL_SGIX_resample
#define GL_SGIX_resample 1

#define GL_PACK_RESAMPLE_SGIX 0x842E
#define GL_UNPACK_RESAMPLE_SGIX 0x842F
#define GL_RESAMPLE_DECIMATE_SGIX 0x8430
#define GL_RESAMPLE_REPLICATE_SGIX 0x8433
#define GL_RESAMPLE_ZERO_FILL_SGIX 0x8434

#define GLEW_SGIX_resample GLEW_GET_VAR(__GLEW_SGIX_resample)

#endif /* GL_SGIX_resample */

/* ----------------------------- GL_SGIX_shadow ---------------------------- */

#ifndef GL_SGIX_shadow
#define GL_SGIX_shadow 1

#define GL_TEXTURE_COMPARE_SGIX 0x819A
#define GL_TEXTURE_COMPARE_OPERATOR_SGIX 0x819B
#define GL_TEXTURE_LEQUAL_R_SGIX 0x819C
#define GL_TEXTURE_GEQUAL_R_SGIX 0x819D

#define GLEW_SGIX_shadow GLEW_GET_VAR(__GLEW_SGIX_shadow)

#endif /* GL_SGIX_shadow */

/* ------------------------- GL_SGIX_shadow_ambient ------------------------ */

#ifndef GL_SGIX_shadow_ambient
#define GL_SGIX_shadow_ambient 1

#define GL_SHADOW_AMBIENT_SGIX 0x80BF

#define GLEW_SGIX_shadow_ambient GLEW_GET_VAR(__GLEW_SGIX_shadow_ambient)

#endif /* GL_SGIX_shadow_ambient */

/* ----------------------------- GL_SGIX_sprite ---------------------------- */

#ifndef GL_SGIX_sprite
#define GL_SGIX_sprite 1

typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERFSGIXPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERFVSGIXPROC) (GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERISGIXPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERIVSGIXPROC) (GLenum pname, GLint* params);


#define GLEW_SGIX_sprite GLEW_GET_VAR(__GLEW_SGIX_sprite)

#endif /* GL_SGIX_sprite */

/* ----------------------- GL_SGIX_tag_sample_buffer ----------------------- */

#ifndef GL_SGIX_tag_sample_buffer
#define GL_SGIX_tag_sample_buffer 1

typedef void (GLAPIENTRY * PFNGLTAGSAMPLEBUFFERSGIXPROC) (void);


#define GLEW_SGIX_tag_sample_buffer GLEW_GET_VAR(__GLEW_SGIX_tag_sample_buffer)

#endif /* GL_SGIX_tag_sample_buffer */

/* ------------------------ GL_SGIX_texture_add_env ------------------------ */

#ifndef GL_SGIX_texture_add_env
#define GL_SGIX_texture_add_env 1

#define GLEW_SGIX_texture_add_env GLEW_GET_VAR(__GLEW_SGIX_texture_add_env)

#endif /* GL_SGIX_texture_add_env */

/* -------------------- GL_SGIX_texture_coordinate_clamp ------------------- */

#ifndef GL_SGIX_texture_coordinate_clamp
#define GL_SGIX_texture_coordinate_clamp 1

#define GL_TEXTURE_MAX_CLAMP_S_SGIX 0x8369
#define GL_TEXTURE_MAX_CLAMP_T_SGIX 0x836A
#define GL_TEXTURE_MAX_CLAMP_R_SGIX 0x836B

#define GLEW_SGIX_texture_coordinate_clamp GLEW_GET_VAR(__GLEW_SGIX_texture_coordinate_clamp)

#endif /* GL_SGIX_texture_coordinate_clamp */

/* ------------------------ GL_SGIX_texture_lod_bias ----------------------- */

#ifndef GL_SGIX_texture_lod_bias
#define GL_SGIX_texture_lod_bias 1

#define GLEW_SGIX_texture_lod_bias GLEW_GET_VAR(__GLEW_SGIX_texture_lod_bias)

#endif /* GL_SGIX_texture_lod_bias */

/* ---------------------- GL_SGIX_texture_multi_buffer --------------------- */

#ifndef GL_SGIX_texture_multi_buffer
#define GL_SGIX_texture_multi_buffer 1

#define GL_TEXTURE_MULTI_BUFFER_HINT_SGIX 0x812E

#define GLEW_SGIX_texture_multi_buffer GLEW_GET_VAR(__GLEW_SGIX_texture_multi_buffer)

#endif /* GL_SGIX_texture_multi_buffer */

/* ------------------------- GL_SGIX_texture_range ------------------------- */

#ifndef GL_SGIX_texture_range
#define GL_SGIX_texture_range 1

#define GL_RGB_SIGNED_SGIX 0x85E0
#define GL_RGBA_SIGNED_SGIX 0x85E1
#define GL_ALPHA_SIGNED_SGIX 0x85E2
#define GL_LUMINANCE_SIGNED_SGIX 0x85E3
#define GL_INTENSITY_SIGNED_SGIX 0x85E4
#define GL_LUMINANCE_ALPHA_SIGNED_SGIX 0x85E5
#define GL_RGB16_SIGNED_SGIX 0x85E6
#define GL_RGBA16_SIGNED_SGIX 0x85E7
#define GL_ALPHA16_SIGNED_SGIX 0x85E8
#define GL_LUMINANCE16_SIGNED_SGIX 0x85E9
#define GL_INTENSITY16_SIGNED_SGIX 0x85EA
#define GL_LUMINANCE16_ALPHA16_SIGNED_SGIX 0x85EB
#define GL_RGB_EXTENDED_RANGE_SGIX 0x85EC
#define GL_RGBA_EXTENDED_RANGE_SGIX 0x85ED
#define GL_ALPHA_EXTENDED_RANGE_SGIX 0x85EE
#define GL_LUMINANCE_EXTENDED_RANGE_SGIX 0x85EF
#define GL_INTENSITY_EXTENDED_RANGE_SGIX 0x85F0
#define GL_LUMINANCE_ALPHA_EXTENDED_RANGE_SGIX 0x85F1
#define GL_RGB16_EXTENDED_RANGE_SGIX 0x85F2
#define GL_RGBA16_EXTENDED_RANGE_SGIX 0x85F3
#define GL_ALPHA16_EXTENDED_RANGE_SGIX 0x85F4
#define GL_LUMINANCE16_EXTENDED_RANGE_SGIX 0x85F5
#define GL_INTENSITY16_EXTENDED_RANGE_SGIX 0x85F6
#define GL_LUMINANCE16_ALPHA16_EXTENDED_RANGE_SGIX 0x85F7
#define GL_MIN_LUMINANCE_SGIS 0x85F8
#define GL_MAX_LUMINANCE_SGIS 0x85F9
#define GL_MIN_INTENSITY_SGIS 0x85FA
#define GL_MAX_INTENSITY_SGIS 0x85FB

#define GLEW_SGIX_texture_range GLEW_GET_VAR(__GLEW_SGIX_texture_range)

#endif /* GL_SGIX_texture_range */

/* ----------------------- GL_SGIX_texture_scale_bias ---------------------- */

#ifndef GL_SGIX_texture_scale_bias
#define GL_SGIX_texture_scale_bias 1

#define GL_POST_TEXTURE_FILTER_BIAS_SGIX 0x8179
#define GL_POST_TEXTURE_FILTER_SCALE_SGIX 0x817A
#define GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX 0x817B
#define GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX 0x817C

#define GLEW_SGIX_texture_scale_bias GLEW_GET_VAR(__GLEW_SGIX_texture_scale_bias)

#endif /* GL_SGIX_texture_scale_bias */

/* ------------------------- GL_SGIX_vertex_preclip ------------------------ */

#ifndef GL_SGIX_vertex_preclip
#define GL_SGIX_vertex_preclip 1

#define GL_VERTEX_PRECLIP_SGIX 0x83EE
#define GL_VERTEX_PRECLIP_HINT_SGIX 0x83EF

#define GLEW_SGIX_vertex_preclip GLEW_GET_VAR(__GLEW_SGIX_vertex_preclip)

#endif /* GL_SGIX_vertex_preclip */

/* ---------------------- GL_SGIX_vertex_preclip_hint ---------------------- */

#ifndef GL_SGIX_vertex_preclip_hint
#define GL_SGIX_vertex_preclip_hint 1

#define GL_VERTEX_PRECLIP_SGIX 0x83EE
#define GL_VERTEX_PRECLIP_HINT_SGIX 0x83EF

#define GLEW_SGIX_vertex_preclip_hint GLEW_GET_VAR(__GLEW_SGIX_vertex_preclip_hint)

#endif /* GL_SGIX_vertex_preclip_hint */

/* ----------------------------- GL_SGIX_ycrcb ----------------------------- */

#ifndef GL_SGIX_ycrcb
#define GL_SGIX_ycrcb 1

#define GLEW_SGIX_ycrcb GLEW_GET_VAR(__GLEW_SGIX_ycrcb)

#endif /* GL_SGIX_ycrcb */

/* -------------------------- GL_SGI_color_matrix -------------------------- */

#ifndef GL_SGI_color_matrix
#define GL_SGI_color_matrix 1

#define GL_COLOR_MATRIX_SGI 0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE_SGI 0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI 0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI 0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI 0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS_SGI 0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI 0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI 0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI 0x80BB

#define GLEW_SGI_color_matrix GLEW_GET_VAR(__GLEW_SGI_color_matrix)

#endif /* GL_SGI_color_matrix */

/* --------------------------- GL_SGI_color_table -------------------------- */

#ifndef GL_SGI_color_table
#define GL_SGI_color_table 1

#define GL_COLOR_TABLE_SGI 0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D2
#define GL_PROXY_COLOR_TABLE_SGI 0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D5
#define GL_COLOR_TABLE_SCALE_SGI 0x80D6
#define GL_COLOR_TABLE_BIAS_SGI 0x80D7
#define GL_COLOR_TABLE_FORMAT_SGI 0x80D8
#define GL_COLOR_TABLE_WIDTH_SGI 0x80D9
#define GL_COLOR_TABLE_RED_SIZE_SGI 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_SGI 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_SGI 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_SGI 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_SGI 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_SGI 0x80DF

typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERFVSGIPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERIVSGIPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLESGIPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* table);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORTABLESGIPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVSGIPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVSGIPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLESGIPROC) (GLenum target, GLenum format, GLenum type, void* table);


#define GLEW_SGI_color_table GLEW_GET_VAR(__GLEW_SGI_color_table)

#endif /* GL_SGI_color_table */

/* ----------------------- GL_SGI_texture_color_table ---------------------- */

#ifndef GL_SGI_texture_color_table
#define GL_SGI_texture_color_table 1

#define GL_TEXTURE_COLOR_TABLE_SGI 0x80BC
#define GL_PROXY_TEXTURE_COLOR_TABLE_SGI 0x80BD

#define GLEW_SGI_texture_color_table GLEW_GET_VAR(__GLEW_SGI_texture_color_table)

#endif /* GL_SGI_texture_color_table */

/* ------------------------- GL_SUNX_constant_data ------------------------- */

#ifndef GL_SUNX_constant_data
#define GL_SUNX_constant_data 1

#define GL_UNPACK_CONSTANT_DATA_SUNX 0x81D5
#define GL_TEXTURE_CONSTANT_DATA_SUNX 0x81D6

typedef void (GLAPIENTRY * PFNGLFINISHTEXTURESUNXPROC) (void);


#define GLEW_SUNX_constant_data GLEW_GET_VAR(__GLEW_SUNX_constant_data)

#endif /* GL_SUNX_constant_data */

/* -------------------- GL_SUN_convolution_border_modes -------------------- */

#ifndef GL_SUN_convolution_border_modes
#define GL_SUN_convolution_border_modes 1

#define GL_WRAP_BORDER_SUN 0x81D4

#define GLEW_SUN_convolution_border_modes GLEW_GET_VAR(__GLEW_SUN_convolution_border_modes)

#endif /* GL_SUN_convolution_border_modes */

/* -------------------------- GL_SUN_global_alpha -------------------------- */

#ifndef GL_SUN_global_alpha
#define GL_SUN_global_alpha 1

#define GL_GLOBAL_ALPHA_SUN 0x81D9
#define GL_GLOBAL_ALPHA_FACTOR_SUN 0x81DA

typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORBSUNPROC) (GLbyte factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORDSUNPROC) (GLdouble factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORFSUNPROC) (GLfloat factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORISUNPROC) (GLint factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORSSUNPROC) (GLshort factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORUBSUNPROC) (GLubyte factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORUISUNPROC) (GLuint factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORUSSUNPROC) (GLushort factor);


#define GLEW_SUN_global_alpha GLEW_GET_VAR(__GLEW_SUN_global_alpha)

#endif /* GL_SUN_global_alpha */

/* --------------------------- GL_SUN_mesh_array --------------------------- */

#ifndef GL_SUN_mesh_array
#define GL_SUN_mesh_array 1

#define GL_QUAD_MESH_SUN 0x8614
#define GL_TRIANGLE_MESH_SUN 0x8615

#define GLEW_SUN_mesh_array GLEW_GET_VAR(__GLEW_SUN_mesh_array)

#endif /* GL_SUN_mesh_array */

/* ------------------------ GL_SUN_read_video_pixels ----------------------- */

#ifndef GL_SUN_read_video_pixels
#define GL_SUN_read_video_pixels 1

typedef void (GLAPIENTRY * PFNGLREADVIDEOPIXELSSUNPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);


#define GLEW_SUN_read_video_pixels GLEW_GET_VAR(__GLEW_SUN_read_video_pixels)

#endif /* GL_SUN_read_video_pixels */

/* --------------------------- GL_SUN_slice_accum -------------------------- */

#ifndef GL_SUN_slice_accum
#define GL_SUN_slice_accum 1

#define GL_SLICE_ACCUM_SUN 0x85CC

#define GLEW_SUN_slice_accum GLEW_GET_VAR(__GLEW_SUN_slice_accum)

#endif /* GL_SUN_slice_accum */

/* -------------------------- GL_SUN_triangle_list ------------------------- */

#ifndef GL_SUN_triangle_list
#define GL_SUN_triangle_list 1

#define GL_RESTART_SUN 0x01
#define GL_REPLACE_MIDDLE_SUN 0x02
#define GL_REPLACE_OLDEST_SUN 0x03
#define GL_TRIANGLE_LIST_SUN 0x81D7
#define GL_REPLACEMENT_CODE_SUN 0x81D8
#define GL_REPLACEMENT_CODE_ARRAY_SUN 0x85C0
#define GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN 0x85C1
#define GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN 0x85C2
#define GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN 0x85C3
#define GL_R1UI_V3F_SUN 0x85C4
#define GL_R1UI_C4UB_V3F_SUN 0x85C5
#define GL_R1UI_C3F_V3F_SUN 0x85C6
#define GL_R1UI_N3F_V3F_SUN 0x85C7
#define GL_R1UI_C4F_N3F_V3F_SUN 0x85C8
#define GL_R1UI_T2F_V3F_SUN 0x85C9
#define GL_R1UI_T2F_N3F_V3F_SUN 0x85CA
#define GL_R1UI_T2F_C4F_N3F_V3F_SUN 0x85CB

typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEPOINTERSUNPROC) (GLenum type, GLsizei stride, const void* pointer);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUBSUNPROC) (GLubyte code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUBVSUNPROC) (const GLubyte* code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUISUNPROC) (GLuint code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUIVSUNPROC) (const GLuint* code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUSSUNPROC) (GLushort code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUSVSUNPROC) (const GLushort* code);


#define GLEW_SUN_triangle_list GLEW_GET_VAR(__GLEW_SUN_triangle_list)

#endif /* GL_SUN_triangle_list */

/* ----------------------------- GL_SUN_vertex ----------------------------- */

#ifndef GL_SUN_vertex
#define GL_SUN_vertex 1

typedef void (GLAPIENTRY * PFNGLCOLOR3FVERTEX3FSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLCOLOR3FVERTEX3FVSUNPROC) (const GLfloat* c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX2FSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX2FVSUNPROC) (const GLubyte* c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX3FSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX3FVSUNPROC) (const GLubyte* c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLNORMAL3FVERTEX3FSUNPROC) (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC) (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC) (const GLuint* rc, const GLubyte *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC) (GLuint rc, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC) (const GLfloat* tc, const GLubyte *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FVERTEX4FSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FVERTEX4FVSUNPROC) (const GLfloat* tc, const GLfloat *v);


#define GLEW_SUN_vertex GLEW_GET_VAR(__GLEW_SUN_vertex)

#endif /* GL_SUN_vertex */

/* -------------------------- GL_WIN_phong_shading ------------------------- */

#ifndef GL_WIN_phong_shading
#define GL_WIN_phong_shading 1

#define GL_PHONG_WIN 0x80EA
#define GL_PHONG_HINT_WIN 0x80EB

#define GLEW_WIN_phong_shading GLEW_GET_VAR(__GLEW_WIN_phong_shading)

#endif /* GL_WIN_phong_shading */

/* -------------------------- GL_WIN_specular_fog -------------------------- */

#ifndef GL_WIN_specular_fog
#define GL_WIN_specular_fog 1

#define GL_FOG_SPECULAR_TEXTURE_WIN 0x80EC

#define GLEW_WIN_specular_fog GLEW_GET_VAR(__GLEW_WIN_specular_fog)

#endif /* GL_WIN_specular_fog */

/* ---------------------------- GL_WIN_swap_hint --------------------------- */

#ifndef GL_WIN_swap_hint
#define GL_WIN_swap_hint 1

typedef void (GLAPIENTRY * PFNGLADDSWAPHINTRECTWINPROC) (GLint x, GLint y, GLsizei width, GLsizei height);


#define GLEW_WIN_swap_hint GLEW_GET_VAR(__GLEW_WIN_swap_hint)

#endif /* GL_WIN_swap_hint */

/* ------------------------------------------------------------------------- */

#if defined(_WIN32)
#define GLEW_FUN_EXPORT GLEWAPI
#define GLEW_VAR_EXPORT GLEWAPI
#endif /* GLEW_MX */

GLEWAPI void glAccum(GLenum op, GLfloat value);
GLEWAPI void glAlphaFunc(GLenum func, GLclampf ref);
GLEWAPI GLboolean glAreTexturesResident(GLsizei n, const GLuint *textures, GLboolean *residences);
GLEWAPI void glArrayElement(GLint i);
GLEWAPI void glBegin(GLenum mode);
GLEWAPI void glBindTexture(GLenum target, GLuint texture);
GLEWAPI void glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
GLEWAPI void glBlendFunc(GLenum sfactor, GLenum dfactor);
GLEWAPI void glCallList(GLuint list);
GLEWAPI void glCallLists(GLsizei n, GLenum type, const GLvoid *lists);
GLEWAPI void glClear(GLbitfield mask);
GLEWAPI void glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLEWAPI void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLEWAPI void glClearDepth(GLclampd depth);
GLEWAPI void glClearIndex(GLfloat c);
GLEWAPI void glClearStencil(GLint s);
GLEWAPI void glClipPlane(GLenum plane, const GLdouble *equation);
GLEWAPI void glColor3b(GLbyte red, GLbyte green, GLbyte blue);
GLEWAPI void glColor3bv(const GLbyte *v);
GLEWAPI void glColor3d(GLdouble red, GLdouble green, GLdouble blue);
GLEWAPI void glColor3dv(const GLdouble *v);
GLEWAPI void glColor3f(GLfloat red, GLfloat green, GLfloat blue);
GLEWAPI void glColor3fv(const GLfloat *v);
GLEWAPI void glColor3i(GLint red, GLint green, GLint blue);
GLEWAPI void glColor3iv(const GLint *v);
GLEWAPI void glColor3s(GLshort red, GLshort green, GLshort blue);
GLEWAPI void glColor3sv(const GLshort *v);
GLEWAPI void glColor3ub(GLubyte red, GLubyte green, GLubyte blue);
GLEWAPI void glColor3ubv(const GLubyte *v);
GLEWAPI void glColor3ui(GLuint red, GLuint green, GLuint blue);
GLEWAPI void glColor3uiv(const GLuint *v);
GLEWAPI void glColor3us(GLushort red, GLushort green, GLushort blue);
GLEWAPI void glColor3usv(const GLushort *v);
GLEWAPI void glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
GLEWAPI void glColor4bv(const GLbyte *v);
GLEWAPI void glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
GLEWAPI void glColor4dv(const GLdouble *v);
GLEWAPI void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLEWAPI void glColor4fv(const GLfloat *v);
GLEWAPI void glColor4i(GLint red, GLint green, GLint blue, GLint alpha);
GLEWAPI void glColor4iv(const GLint *v);
GLEWAPI void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);
GLEWAPI void glColor4sv(const GLshort *v);
GLEWAPI void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
GLEWAPI void glColor4ubv(const GLubyte *v);
GLEWAPI void glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha);
GLEWAPI void glColor4uiv(const GLuint *v);
GLEWAPI void glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);
GLEWAPI void glColor4usv(const GLushort *v);
GLEWAPI void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLEWAPI void glColorMaterial(GLenum face, GLenum mode);
GLEWAPI void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
GLEWAPI void glCopyTexImage1D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
GLEWAPI void glCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLEWAPI void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glCullFace(GLenum mode);
GLEWAPI void glDeleteLists(GLuint list, GLsizei range);
GLEWAPI void glDeleteTextures(GLsizei n, const GLuint *textures);
GLEWAPI void glDepthFunc(GLenum func);
GLEWAPI void glDepthMask(GLboolean flag);
GLEWAPI void glDepthRange(GLclampd zNear, GLclampd zFar);
GLEWAPI void glDisable(GLenum cap);
GLEWAPI void glDisableClientState(GLenum array);
GLEWAPI void glDrawArrays(GLenum mode, GLint first, GLsizei count);
GLEWAPI void glDrawBuffer(GLenum mode);
GLEWAPI void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
GLEWAPI void glDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLEWAPI void glEdgeFlag(GLboolean flag);
GLEWAPI void glEdgeFlagPointer(GLsizei stride, const GLvoid *pointer);
GLEWAPI void glEdgeFlagv(const GLboolean *flag);
GLEWAPI void glEnable(GLenum cap);
GLEWAPI void glEnableClientState(GLenum array);
GLEWAPI void glEnd(void);
GLEWAPI void glEndList(void);
GLEWAPI void glEvalCoord1d(GLdouble u);
GLEWAPI void glEvalCoord1dv(const GLdouble *u);
GLEWAPI void glEvalCoord1f(GLfloat u);
GLEWAPI void glEvalCoord1fv(const GLfloat *u);
GLEWAPI void glEvalCoord2d(GLdouble u, GLdouble v);
GLEWAPI void glEvalCoord2dv(const GLdouble *u);
GLEWAPI void glEvalCoord2f(GLfloat u, GLfloat v);
GLEWAPI void glEvalCoord2fv(const GLfloat *u);
GLEWAPI void glEvalMesh1(GLenum mode, GLint i1, GLint i2);
GLEWAPI void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
GLEWAPI void glEvalPoint1(GLint i);
GLEWAPI void glEvalPoint2(GLint i, GLint j);
GLEWAPI void glFeedbackBuffer(GLsizei size, GLenum type, GLfloat *buffer);
GLEWAPI void glFinish(void);
GLEWAPI void glFlush(void);
GLEWAPI void glFogf(GLenum pname, GLfloat param);
GLEWAPI void glFogfv(GLenum pname, const GLfloat *params);
GLEWAPI void glFogi(GLenum pname, GLint param);
GLEWAPI void glFogiv(GLenum pname, const GLint *params);
GLEWAPI void glFrontFace(GLenum mode);
GLEWAPI void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLEWAPI GLuint glGenLists(GLsizei range);
GLEWAPI void glGenTextures(GLsizei n, GLuint *textures);
GLEWAPI void glGetBooleanv(GLenum pname, GLboolean *params);
GLEWAPI void glGetClipPlane(GLenum plane, GLdouble *equation);
GLEWAPI void glGetDoublev(GLenum pname, GLdouble *params);
GLEWAPI GLenum glGetError(void);
GLEWAPI void glGetFloatv(GLenum pname, GLfloat *params);
GLEWAPI void glGetIntegerv(GLenum pname, GLint *params);
GLEWAPI void glGetLightfv(GLenum light, GLenum pname, GLfloat *params);
GLEWAPI void glGetLightiv(GLenum light, GLenum pname, GLint *params);
GLEWAPI void glGetMapdv(GLenum target, GLenum query, GLdouble *v);
GLEWAPI void glGetMapfv(GLenum target, GLenum query, GLfloat *v);
GLEWAPI void glGetMapiv(GLenum target, GLenum query, GLint *v);
GLEWAPI void glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params);
GLEWAPI void glGetMaterialiv(GLenum face, GLenum pname, GLint *params);
GLEWAPI void glGetPixelMapfv(GLenum map, GLfloat *values);
GLEWAPI void glGetPixelMapuiv(GLenum map, GLuint *values);
GLEWAPI void glGetPixelMapusv(GLenum map, GLushort *values);
GLEWAPI void glGetPointerv(GLenum pname, void** params);
GLEWAPI void glGetPolygonStipple(GLubyte *mask);
GLEWAPI GLubyte* glGetString(GLenum s);
GLEWAPI void glGetTexEnvfv(GLenum target, GLenum pname, GLfloat *params);
GLEWAPI void glGetTexEnviv(GLenum target, GLenum pname, GLint *params);
GLEWAPI void glGetTexGendv(GLenum coord, GLenum pname, GLdouble *params);
GLEWAPI void glGetTexGenfv(GLenum coord, GLenum pname, GLfloat *params);
GLEWAPI void glGetTexGeniv(GLenum coord, GLenum pname, GLint *params);
GLEWAPI void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
GLEWAPI void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params);
GLEWAPI void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params);
GLEWAPI void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLEWAPI void glGetTexParameteriv(GLenum target, GLenum pname, GLint *params);
GLEWAPI void glHint(GLenum target, GLenum mode);
GLEWAPI void glIndexMask(GLuint mask);
GLEWAPI void glIndexPointer(GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glIndexd(GLdouble c);
GLEWAPI void glIndexdv(const GLdouble *c);
GLEWAPI void glIndexf(GLfloat c);
GLEWAPI void glIndexfv(const GLfloat *c);
GLEWAPI void glIndexi(GLint c);
GLEWAPI void glIndexiv(const GLint *c);
GLEWAPI void glIndexs(GLshort c);
GLEWAPI void glIndexsv(const GLshort *c);
GLEWAPI void glIndexub(GLubyte c);
GLEWAPI void glIndexubv(const GLubyte *c);
GLEWAPI void glInitNames(void);
GLEWAPI void glInterleavedArrays(GLenum format, GLsizei stride, const GLvoid *pointer);
GLEWAPI GLboolean glIsEnabled(GLenum cap);
GLEWAPI GLboolean glIsList(GLuint list);
GLEWAPI GLboolean glIsTexture(GLuint texture);
GLEWAPI void glLightModelf(GLenum pname, GLfloat param);
GLEWAPI void glLightModelfv(GLenum pname, const GLfloat *params);
GLEWAPI void glLightModeli(GLenum pname, GLint param);
GLEWAPI void glLightModeliv(GLenum pname, const GLint *params);
GLEWAPI void glLightf(GLenum light, GLenum pname, GLfloat param);
GLEWAPI void glLightfv(GLenum light, GLenum pname, const GLfloat *params);
GLEWAPI void glLighti(GLenum light, GLenum pname, GLint param);
GLEWAPI void glLightiv(GLenum light, GLenum pname, const GLint *params);
GLEWAPI void glLineStipple(GLint factor, GLushort pattern);
GLEWAPI void glLineWidth(GLfloat width);
GLEWAPI void glListBase(GLuint base);
GLEWAPI void glLoadIdentity(void);
GLEWAPI void glLoadMatrixd(const GLdouble *m);
GLEWAPI void glLoadMatrixf(const GLfloat *m);
GLEWAPI void glLoadName(GLuint name);
GLEWAPI void glLogicOp(GLenum opcode);
GLEWAPI void glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
GLEWAPI void glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
GLEWAPI void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
GLEWAPI void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
GLEWAPI void glMapGrid1d(GLint un, GLdouble u1, GLdouble u2);
GLEWAPI void glMapGrid1f(GLint un, GLfloat u1, GLfloat u2);
GLEWAPI void glMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
GLEWAPI void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
GLEWAPI void glMaterialf(GLenum face, GLenum pname, GLfloat param);
GLEWAPI void glMaterialfv(GLenum face, GLenum pname, const GLfloat *params);
GLEWAPI void glMateriali(GLenum face, GLenum pname, GLint param);
GLEWAPI void glMaterialiv(GLenum face, GLenum pname, const GLint *params);
GLEWAPI void glMatrixMode(GLenum mode);
GLEWAPI void glMultMatrixd(const GLdouble *m);
GLEWAPI void glMultMatrixf(const GLfloat *m);
GLEWAPI void glNewList(GLuint list, GLenum mode);
GLEWAPI void glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz);
GLEWAPI void glNormal3bv(const GLbyte *v);
GLEWAPI void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz);
GLEWAPI void glNormal3dv(const GLdouble *v);
GLEWAPI void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);
GLEWAPI void glNormal3fv(const GLfloat *v);
GLEWAPI void glNormal3i(GLint nx, GLint ny, GLint nz);
GLEWAPI void glNormal3iv(const GLint *v);
GLEWAPI void glNormal3s(GLshort nx, GLshort ny, GLshort nz);
GLEWAPI void glNormal3sv(const GLshort *v);
GLEWAPI void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLEWAPI void glPassThrough(GLfloat token);
GLEWAPI void glPixelMapfv(GLenum map, GLsizei mapsize, const GLfloat *values);
GLEWAPI void glPixelMapuiv(GLenum map, GLsizei mapsize, const GLuint *values);
GLEWAPI void glPixelMapusv(GLenum map, GLsizei mapsize, const GLushort *values);
GLEWAPI void glPixelStoref(GLenum pname, GLfloat param);
GLEWAPI void glPixelStorei(GLenum pname, GLint param);
GLEWAPI void glPixelTransferf(GLenum pname, GLfloat param);
GLEWAPI void glPixelTransferi(GLenum pname, GLint param);
GLEWAPI void glPixelZoom(GLfloat xfactor, GLfloat yfactor);
GLEWAPI void glPointSize(GLfloat size);
GLEWAPI void glPolygonMode(GLenum face, GLenum mode);
GLEWAPI void glPolygonOffset(GLfloat factor, GLfloat units);
GLEWAPI void glPolygonStipple(const GLubyte *mask);
GLEWAPI void glPopAttrib(void);
GLEWAPI void glPopClientAttrib(void);
GLEWAPI void glPopMatrix(void);
GLEWAPI void glPopName(void);
GLEWAPI void glPrioritizeTextures(GLsizei n, const GLuint *textures, const GLclampf *priorities);
GLEWAPI void glPushAttrib(GLbitfield mask);
GLEWAPI void glPushClientAttrib(GLbitfield mask);
GLEWAPI void glPushMatrix(void);
GLEWAPI void glPushName(GLuint name);
GLEWAPI void glRasterPos2d(GLdouble x, GLdouble y);
GLEWAPI void glRasterPos2dv(const GLdouble *v);
GLEWAPI void glRasterPos2f(GLfloat x, GLfloat y);
GLEWAPI void glRasterPos2fv(const GLfloat *v);
GLEWAPI void glRasterPos2i(GLint x, GLint y);
GLEWAPI void glRasterPos2iv(const GLint *v);
GLEWAPI void glRasterPos2s(GLshort x, GLshort y);
GLEWAPI void glRasterPos2sv(const GLshort *v);
GLEWAPI void glRasterPos3d(GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glRasterPos3dv(const GLdouble *v);
GLEWAPI void glRasterPos3f(GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glRasterPos3fv(const GLfloat *v);
GLEWAPI void glRasterPos3i(GLint x, GLint y, GLint z);
GLEWAPI void glRasterPos3iv(const GLint *v);
GLEWAPI void glRasterPos3s(GLshort x, GLshort y, GLshort z);
GLEWAPI void glRasterPos3sv(const GLshort *v);
GLEWAPI void glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glRasterPos4dv(const GLdouble *v);
GLEWAPI void glRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glRasterPos4fv(const GLfloat *v);
GLEWAPI void glRasterPos4i(GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glRasterPos4iv(const GLint *v);
GLEWAPI void glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w);
GLEWAPI void glRasterPos4sv(const GLshort *v);
GLEWAPI void glReadBuffer(GLenum mode);
GLEWAPI void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
GLEWAPI void glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
GLEWAPI void glRectdv(const GLdouble *v1, const GLdouble *v2);
GLEWAPI void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
GLEWAPI void glRectfv(const GLfloat *v1, const GLfloat *v2);
GLEWAPI void glRecti(GLint x1, GLint y1, GLint x2, GLint y2);
GLEWAPI void glRectiv(const GLint *v1, const GLint *v2);
GLEWAPI void glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
GLEWAPI void glRectsv(const GLshort *v1, const GLshort *v2);
GLEWAPI GLint glRenderMode(GLenum mode);
GLEWAPI void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glScaled(GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glScalef(GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glSelectBuffer(GLsizei size, GLuint *buffer);
GLEWAPI void glShadeModel(GLenum mode);
GLEWAPI void glStencilFunc(GLenum func, GLint ref, GLuint mask);
GLEWAPI void glStencilMask(GLuint mask);
GLEWAPI void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
GLEWAPI void glTexCoord1d(GLdouble s);
GLEWAPI void glTexCoord1dv(const GLdouble *v);
GLEWAPI void glTexCoord1f(GLfloat s);
GLEWAPI void glTexCoord1fv(const GLfloat *v);
GLEWAPI void glTexCoord1i(GLint s);
GLEWAPI void glTexCoord1iv(const GLint *v);
GLEWAPI void glTexCoord1s(GLshort s);
GLEWAPI void glTexCoord1sv(const GLshort *v);
GLEWAPI void glTexCoord2d(GLdouble s, GLdouble t);
GLEWAPI void glTexCoord2dv(const GLdouble *v);
GLEWAPI void glTexCoord2f(GLfloat s, GLfloat t);
GLEWAPI void glTexCoord2fv(const GLfloat *v);
GLEWAPI void glTexCoord2i(GLint s, GLint t);
GLEWAPI void glTexCoord2iv(const GLint *v);
GLEWAPI void glTexCoord2s(GLshort s, GLshort t);
GLEWAPI void glTexCoord2sv(const GLshort *v);
GLEWAPI void glTexCoord3d(GLdouble s, GLdouble t, GLdouble r);
GLEWAPI void glTexCoord3dv(const GLdouble *v);
GLEWAPI void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r);
GLEWAPI void glTexCoord3fv(const GLfloat *v);
GLEWAPI void glTexCoord3i(GLint s, GLint t, GLint r);
GLEWAPI void glTexCoord3iv(const GLint *v);
GLEWAPI void glTexCoord3s(GLshort s, GLshort t, GLshort r);
GLEWAPI void glTexCoord3sv(const GLshort *v);
GLEWAPI void glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLEWAPI void glTexCoord4dv(const GLdouble *v);
GLEWAPI void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLEWAPI void glTexCoord4fv(const GLfloat *v);
GLEWAPI void glTexCoord4i(GLint s, GLint t, GLint r, GLint q);
GLEWAPI void glTexCoord4iv(const GLint *v);
GLEWAPI void glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q);
GLEWAPI void glTexCoord4sv(const GLshort *v);
GLEWAPI void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glTexEnvf(GLenum target, GLenum pname, GLfloat param);
GLEWAPI void glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params);
GLEWAPI void glTexEnvi(GLenum target, GLenum pname, GLint param);
GLEWAPI void glTexEnviv(GLenum target, GLenum pname, const GLint *params);
GLEWAPI void glTexGend(GLenum coord, GLenum pname, GLdouble param);
GLEWAPI void glTexGendv(GLenum coord, GLenum pname, const GLdouble *params);
GLEWAPI void glTexGenf(GLenum coord, GLenum pname, GLfloat param);
GLEWAPI void glTexGenfv(GLenum coord, GLenum pname, const GLfloat *params);
GLEWAPI void glTexGeni(GLenum coord, GLenum pname, GLint param);
GLEWAPI void glTexGeniv(GLenum coord, GLenum pname, const GLint *params);
GLEWAPI void glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLEWAPI void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLEWAPI void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
GLEWAPI void glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params);
GLEWAPI void glTexParameteri(GLenum target, GLenum pname, GLint param);
GLEWAPI void glTexParameteriv(GLenum target, GLenum pname, const GLint *params);
GLEWAPI void glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
GLEWAPI void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLEWAPI void glTranslated(GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glTranslatef(GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glVertex2d(GLdouble x, GLdouble y);
GLEWAPI void glVertex2dv(const GLdouble *v);
GLEWAPI void glVertex2f(GLfloat x, GLfloat y);
GLEWAPI void glVertex2fv(const GLfloat *v);
GLEWAPI void glVertex2i(GLint x, GLint y);
GLEWAPI void glVertex2iv(const GLint *v);
GLEWAPI void glVertex2s(GLshort x, GLshort y);
GLEWAPI void glVertex2sv(const GLshort *v);
GLEWAPI void glVertex3d(GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glVertex3dv(const GLdouble *v);
GLEWAPI void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glVertex3fv(const GLfloat *v);
GLEWAPI void glVertex3i(GLint x, GLint y, GLint z);
GLEWAPI void glVertex3iv(const GLint *v);
GLEWAPI void glVertex3s(GLshort x, GLshort y, GLshort z);
GLEWAPI void glVertex3sv(const GLshort *v);
GLEWAPI void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glVertex4dv(const GLdouble *v);
GLEWAPI void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glVertex4fv(const GLfloat *v);
GLEWAPI void glVertex4i(GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glVertex4iv(const GLint *v);
GLEWAPI void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
GLEWAPI void glVertex4sv(const GLshort *v);
GLEWAPI void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

GLEWAPI void glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
GLEWAPI void glTexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLEWAPI void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

GLEWAPI void glActiveTexture(GLenum texture);
GLEWAPI void glClientActiveTexture(GLenum texture);
GLEWAPI void glCompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
GLEWAPI void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
GLEWAPI void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
GLEWAPI void glCompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
GLEWAPI void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
GLEWAPI void glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
GLEWAPI void glGetCompressedTexImage(GLenum target, GLint lod, GLvoid *img);
GLEWAPI void glLoadTransposeMatrixd(const GLdouble m[16]);
GLEWAPI void glLoadTransposeMatrixf(const GLfloat m[16]);
GLEWAPI void glMultTransposeMatrixd(const GLdouble m[16]);
GLEWAPI void glMultTransposeMatrixf(const GLfloat m[16]);
GLEWAPI void glMultiTexCoord1d(GLenum target, GLdouble s);
GLEWAPI void glMultiTexCoord1dv(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord1f(GLenum target, GLfloat s);
GLEWAPI void glMultiTexCoord1fv(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord1i(GLenum target, GLint s);
GLEWAPI void glMultiTexCoord1iv(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord1s(GLenum target, GLshort s);
GLEWAPI void glMultiTexCoord1sv(GLenum target, const GLshort *v);
GLEWAPI void glMultiTexCoord2d(GLenum target, GLdouble s, GLdouble t);
GLEWAPI void glMultiTexCoord2dv(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t);
GLEWAPI void glMultiTexCoord2fv(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord2i(GLenum target, GLint s, GLint t);
GLEWAPI void glMultiTexCoord2iv(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord2s(GLenum target, GLshort s, GLshort t);
GLEWAPI void glMultiTexCoord2sv(GLenum target, const GLshort *v);
GLEWAPI void glMultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r);
GLEWAPI void glMultiTexCoord3dv(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord3f(GLenum target, GLfloat s, GLfloat t, GLfloat r);
GLEWAPI void glMultiTexCoord3fv(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r);
GLEWAPI void glMultiTexCoord3iv(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r);
GLEWAPI void glMultiTexCoord3sv(GLenum target, const GLshort *v);
GLEWAPI void glMultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLEWAPI void glMultiTexCoord4dv(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLEWAPI void glMultiTexCoord4fv(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q);
GLEWAPI void glMultiTexCoord4iv(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
GLEWAPI void glMultiTexCoord4sv(GLenum target, const GLshort *v);
GLEWAPI void glSampleCoverage(GLclampf value, GLboolean invert);

GLEWAPI void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLEWAPI void glBlendEquation(GLenum mode);
GLEWAPI void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
GLEWAPI void glFogCoordPointer(GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glFogCoordd(GLdouble coord);
GLEWAPI void glFogCoorddv(const GLdouble *coord);
GLEWAPI void glFogCoordf(GLfloat coord);
GLEWAPI void glFogCoordfv(const GLfloat *coord);
GLEWAPI void glMultiDrawArrays(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
GLEWAPI void glMultiDrawElements(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei drawcount);
GLEWAPI void glPointParameterf(GLenum pname, GLfloat param);
GLEWAPI void glPointParameterfv(GLenum pname, const GLfloat *params);
GLEWAPI void glPointParameteri(GLenum pname, GLint param);
GLEWAPI void glPointParameteriv(GLenum pname, const GLint *params);
GLEWAPI void glSecondaryColor3b(GLbyte red, GLbyte green, GLbyte blue);
GLEWAPI void glSecondaryColor3bv(const GLbyte *v);
GLEWAPI void glSecondaryColor3d(GLdouble red, GLdouble green, GLdouble blue);
GLEWAPI void glSecondaryColor3dv(const GLdouble *v);
GLEWAPI void glSecondaryColor3f(GLfloat red, GLfloat green, GLfloat blue);
GLEWAPI void glSecondaryColor3fv(const GLfloat *v);
GLEWAPI void glSecondaryColor3i(GLint red, GLint green, GLint blue);
GLEWAPI void glSecondaryColor3iv(const GLint *v);
GLEWAPI void glSecondaryColor3s(GLshort red, GLshort green, GLshort blue);
GLEWAPI void glSecondaryColor3sv(const GLshort *v);
GLEWAPI void glSecondaryColor3ub(GLubyte red, GLubyte green, GLubyte blue);
GLEWAPI void glSecondaryColor3ubv(const GLubyte *v);
GLEWAPI void glSecondaryColor3ui(GLuint red, GLuint green, GLuint blue);
GLEWAPI void glSecondaryColor3uiv(const GLuint *v);
GLEWAPI void glSecondaryColor3us(GLushort red, GLushort green, GLushort blue);
GLEWAPI void glSecondaryColor3usv(const GLushort *v);
GLEWAPI void glSecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glWindowPos2d(GLdouble x, GLdouble y);
GLEWAPI void glWindowPos2dv(const GLdouble *p);
GLEWAPI void glWindowPos2f(GLfloat x, GLfloat y);
GLEWAPI void glWindowPos2fv(const GLfloat *p);
GLEWAPI void glWindowPos2i(GLint x, GLint y);
GLEWAPI void glWindowPos2iv(const GLint *p);
GLEWAPI void glWindowPos2s(GLshort x, GLshort y);
GLEWAPI void glWindowPos2sv(const GLshort *p);
GLEWAPI void glWindowPos3d(GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glWindowPos3dv(const GLdouble *p);
GLEWAPI void glWindowPos3f(GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glWindowPos3fv(const GLfloat *p);
GLEWAPI void glWindowPos3i(GLint x, GLint y, GLint z);
GLEWAPI void glWindowPos3iv(const GLint *p);
GLEWAPI void glWindowPos3s(GLshort x, GLshort y, GLshort z);
GLEWAPI void glWindowPos3sv(const GLshort *p);

GLEWAPI void glBeginQuery(GLenum target, GLuint id);
GLEWAPI void glBindBuffer(GLenum target, GLuint buffer);
GLEWAPI void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
GLEWAPI void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
GLEWAPI void glDeleteBuffers(GLsizei n, const GLuint* buffers);
GLEWAPI void glDeleteQueries(GLsizei n, const GLuint* ids);
GLEWAPI void glEndQuery(GLenum target);
GLEWAPI void glGenBuffers(GLsizei n, GLuint* buffers);
GLEWAPI void glGenQueries(GLsizei n, GLuint* ids);
GLEWAPI void glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetBufferPointerv(GLenum target, GLenum pname, GLvoid** params);
GLEWAPI void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data);
GLEWAPI void glGetQueryObjectiv(GLuint id, GLenum pname, GLint* params);
GLEWAPI void glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params);
GLEWAPI void glGetQueryiv(GLenum target, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsBuffer(GLuint buffer);
GLEWAPI GLboolean glIsQuery(GLuint id);
GLEWAPI GLvoid* glMapBuffer(GLenum target, GLenum access);
GLEWAPI GLboolean glUnmapBuffer(GLenum target);

GLEWAPI void glAttachShader(GLuint program, GLuint shader);
GLEWAPI void glBindAttribLocation(GLuint program, GLuint index, const GLchar* name);
GLEWAPI void glBlendEquationSeparate(GLenum, GLenum);
GLEWAPI void glCompileShader(GLuint shader);
GLEWAPI GLuint glCreateProgram(void);
GLEWAPI GLuint glCreateShader(GLenum type);
GLEWAPI void glDeleteProgram(GLuint program);
GLEWAPI void glDeleteShader(GLuint shader);
GLEWAPI void glDetachShader(GLuint program, GLuint shader);
GLEWAPI void glDisableVertexAttribArray(GLuint);
GLEWAPI void glDrawBuffers(GLsizei n, const GLenum* bufs);
GLEWAPI void glEnableVertexAttribArray(GLuint);
GLEWAPI void glGetActiveAttrib(GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
GLEWAPI void glGetActiveUniform(GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
GLEWAPI void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
GLEWAPI GLint glGetAttribLocation(GLuint program, const GLchar* name);
GLEWAPI void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GLEWAPI void glGetProgramiv(GLuint program, GLenum pname, GLint* param);
GLEWAPI void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GLEWAPI void glGetShaderSource(GLuint obj, GLsizei maxLength, GLsizei* length, GLchar* source);
GLEWAPI void glGetShaderiv(GLuint shader, GLenum pname, GLint* param);
GLEWAPI GLint glGetUniformLocation(GLuint program, const GLchar* name);
GLEWAPI void glGetUniformfv(GLuint program, GLint location, GLfloat* params);
GLEWAPI void glGetUniformiv(GLuint program, GLint location, GLint* params);
GLEWAPI void glGetVertexAttribPointerv(GLuint, GLenum, GLvoid**);
GLEWAPI void glGetVertexAttribdv(GLuint, GLenum, GLdouble*);
GLEWAPI void glGetVertexAttribfv(GLuint, GLenum, GLfloat*);
GLEWAPI void glGetVertexAttribiv(GLuint, GLenum, GLint*);
GLEWAPI GLboolean glIsProgram(GLuint program);
GLEWAPI GLboolean glIsShader(GLuint shader);
GLEWAPI void glLinkProgram(GLuint program);
GLEWAPI void glShaderSource(GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
GLEWAPI void glStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
GLEWAPI void glStencilMaskSeparate(GLenum, GLuint);
GLEWAPI void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
GLEWAPI void glUniform1f(GLint location, GLfloat v0);
GLEWAPI void glUniform1fv(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform1i(GLint location, GLint v0);
GLEWAPI void glUniform1iv(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
GLEWAPI void glUniform2fv(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform2i(GLint location, GLint v0, GLint v1);
GLEWAPI void glUniform2iv(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLEWAPI void glUniform3fv(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
GLEWAPI void glUniform3iv(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLEWAPI void glUniform4fv(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLEWAPI void glUniform4iv(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glUseProgram(GLuint program);
GLEWAPI void glValidateProgram(GLuint program);
GLEWAPI void glVertexAttrib1d(GLuint index, GLdouble x);
GLEWAPI void glVertexAttrib1dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib1f(GLuint index, GLfloat x);
GLEWAPI void glVertexAttrib1fv(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib1s(GLuint index, GLshort x);
GLEWAPI void glVertexAttrib1sv(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib2d(GLuint index, GLdouble x, GLdouble y);
GLEWAPI void glVertexAttrib2dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
GLEWAPI void glVertexAttrib2fv(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib2s(GLuint index, GLshort x, GLshort y);
GLEWAPI void glVertexAttrib2sv(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glVertexAttrib3dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glVertexAttrib3fv(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z);
GLEWAPI void glVertexAttrib3sv(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4Nbv(GLuint index, const GLbyte* v);
GLEWAPI void glVertexAttrib4Niv(GLuint index, const GLint* v);
GLEWAPI void glVertexAttrib4Nsv(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
GLEWAPI void glVertexAttrib4Nubv(GLuint index, const GLubyte* v);
GLEWAPI void glVertexAttrib4Nuiv(GLuint index, const GLuint* v);
GLEWAPI void glVertexAttrib4Nusv(GLuint index, const GLushort* v);
GLEWAPI void glVertexAttrib4bv(GLuint index, const GLbyte* v);
GLEWAPI void glVertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glVertexAttrib4dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glVertexAttrib4fv(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib4iv(GLuint index, const GLint* v);
GLEWAPI void glVertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
GLEWAPI void glVertexAttrib4sv(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4ubv(GLuint index, const GLubyte* v);
GLEWAPI void glVertexAttrib4uiv(GLuint index, const GLuint* v);
GLEWAPI void glVertexAttrib4usv(GLuint index, const GLushort* v);
GLEWAPI void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

GLEWAPI void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLEWAPI void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLEWAPI void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLEWAPI void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLEWAPI void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLEWAPI void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

GLEWAPI void glBeginConditionalRender(GLuint, GLenum);
GLEWAPI void glBeginTransformFeedback(GLenum);
GLEWAPI void glBindFragDataLocation(GLuint, GLuint, const GLchar*);
GLEWAPI void glClampColor(GLenum, GLenum);
GLEWAPI void glClearBufferfi(GLenum, GLint, GLfloat, GLint);
GLEWAPI void glClearBufferfv(GLenum, GLint, const GLfloat*);
GLEWAPI void glClearBufferiv(GLenum, GLint, const GLint*);
GLEWAPI void glClearBufferuiv(GLenum, GLint, const GLuint*);
GLEWAPI void glColorMaski(GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
GLEWAPI void glDisablei(GLenum, GLuint);
GLEWAPI void glEnablei(GLenum, GLuint);
GLEWAPI void glEndConditionalRender(void);
GLEWAPI void glEndTransformFeedback(void);
GLEWAPI void glGetBooleani_v(GLenum, GLuint, GLboolean*);
GLEWAPI GLint glGetFragDataLocation(GLuint, const GLchar*);
GLEWAPI const GLubyte* glGetStringi(GLenum, GLuint);
GLEWAPI void glGetTexParameterIiv(GLenum, GLenum, GLint*);
GLEWAPI void glGetTexParameterIuiv(GLenum, GLenum, GLuint*);
GLEWAPI void glGetTransformFeedbackVarying(GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *);
GLEWAPI void glGetUniformuiv(GLuint, GLint, GLuint*);
GLEWAPI void glGetVertexAttribIiv(GLuint, GLenum, GLint*);
GLEWAPI void glGetVertexAttribIuiv(GLuint, GLenum, GLuint*);
GLEWAPI GLboolean glIsEnabledi(GLenum, GLuint);
GLEWAPI void glTexParameterIiv(GLenum, GLenum, const GLint*);
GLEWAPI void glTexParameterIuiv(GLenum, GLenum, const GLuint*);
GLEWAPI void glTransformFeedbackVaryings(GLuint, GLsizei, const GLchar **, GLenum);
GLEWAPI void glUniform1ui(GLint, GLuint);
GLEWAPI void glUniform1uiv(GLint, GLsizei, const GLuint*);
GLEWAPI void glUniform2ui(GLint, GLuint, GLuint);
GLEWAPI void glUniform2uiv(GLint, GLsizei, const GLuint*);
GLEWAPI void glUniform3ui(GLint, GLuint, GLuint, GLuint);
GLEWAPI void glUniform3uiv(GLint, GLsizei, const GLuint*);
GLEWAPI void glUniform4ui(GLint, GLuint, GLuint, GLuint, GLuint);
GLEWAPI void glUniform4uiv(GLint, GLsizei, const GLuint*);
GLEWAPI void glVertexAttribI1i(GLuint, GLint);
GLEWAPI void glVertexAttribI1iv(GLuint, const GLint*);
GLEWAPI void glVertexAttribI1ui(GLuint, GLuint);
GLEWAPI void glVertexAttribI1uiv(GLuint, const GLuint*);
GLEWAPI void glVertexAttribI2i(GLuint, GLint, GLint);
GLEWAPI void glVertexAttribI2iv(GLuint, const GLint*);
GLEWAPI void glVertexAttribI2ui(GLuint, GLuint, GLuint);
GLEWAPI void glVertexAttribI2uiv(GLuint, const GLuint*);
GLEWAPI void glVertexAttribI3i(GLuint, GLint, GLint, GLint);
GLEWAPI void glVertexAttribI3iv(GLuint, const GLint*);
GLEWAPI void glVertexAttribI3ui(GLuint, GLuint, GLuint, GLuint);
GLEWAPI void glVertexAttribI3uiv(GLuint, const GLuint*);
GLEWAPI void glVertexAttribI4bv(GLuint, const GLbyte*);
GLEWAPI void glVertexAttribI4i(GLuint, GLint, GLint, GLint, GLint);
GLEWAPI void glVertexAttribI4iv(GLuint, const GLint*);
GLEWAPI void glVertexAttribI4sv(GLuint, const GLshort*);
GLEWAPI void glVertexAttribI4ubv(GLuint, const GLubyte*);
GLEWAPI void glVertexAttribI4ui(GLuint, GLuint, GLuint, GLuint, GLuint);
GLEWAPI void glVertexAttribI4uiv(GLuint, const GLuint*);
GLEWAPI void glVertexAttribI4usv(GLuint, const GLushort*);
GLEWAPI void glVertexAttribIPointer(GLuint, GLint, GLenum, GLsizei, const GLvoid*);

GLEWAPI void glDrawArraysInstanced(GLenum, GLint, GLsizei, GLsizei);
GLEWAPI void glDrawElementsInstanced(GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
GLEWAPI void glPrimitiveRestartIndex(GLuint);
GLEWAPI void glTexBuffer(GLenum, GLenum, GLuint);

GLEWAPI void glFramebufferTexture(GLenum, GLenum, GLuint, GLint);
GLEWAPI void glGetBufferParameteri64v(GLenum, GLenum, GLint64 *);
GLEWAPI void glGetInteger64i_v(GLenum, GLuint, GLint64 *);

GLEWAPI void glVertexAttribDivisor(GLuint index, GLuint divisor);

GLEWAPI void glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLEWAPI void glBlendEquationi(GLuint buf, GLenum mode);
GLEWAPI void glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GLEWAPI void glBlendFunci(GLuint buf, GLenum src, GLenum dst);
GLEWAPI void glMinSampleShading(GLclampf value);

GLEWAPI void glTbufferMask3DFX(GLuint mask);

GLEWAPI void glDebugMessageCallbackAMD(GLDEBUGPROCAMD callback, void* userParam);
GLEWAPI void glDebugMessageEnableAMD(GLenum category, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
GLEWAPI void glDebugMessageInsertAMD(GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar* buf);
GLEWAPI GLuint glGetDebugMessageLogAMD(GLuint count, GLsizei bufsize, GLenum* categories, GLuint* severities, GLuint* ids, GLsizei* lengths, GLchar* message);

GLEWAPI void glBlendEquationIndexedAMD(GLuint buf, GLenum mode);
GLEWAPI void glBlendEquationSeparateIndexedAMD(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLEWAPI void glBlendFuncIndexedAMD(GLuint buf, GLenum src, GLenum dst);
GLEWAPI void glBlendFuncSeparateIndexedAMD(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

GLEWAPI void glMultiDrawArraysIndirectAMD(GLenum mode, const void* indirect, GLsizei primcount, GLsizei stride);
GLEWAPI void glMultiDrawElementsIndirectAMD(GLenum mode, GLenum type, const void* indirect, GLsizei primcount, GLsizei stride);

GLEWAPI void glDeleteNamesAMD(GLenum identifier, GLuint num, const GLuint* names);
GLEWAPI void glGenNamesAMD(GLenum identifier, GLuint num, GLuint* names);
GLEWAPI GLboolean glIsNameAMD(GLenum identifier, GLuint name);

GLEWAPI void glBeginPerfMonitorAMD(GLuint monitor);
GLEWAPI void glDeletePerfMonitorsAMD(GLsizei n, GLuint* monitors);
GLEWAPI void glEndPerfMonitorAMD(GLuint monitor);
GLEWAPI void glGenPerfMonitorsAMD(GLsizei n, GLuint* monitors);
GLEWAPI void glGetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint *bytesWritten);
GLEWAPI void glGetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname, void* data);
GLEWAPI void glGetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, GLchar *counterString);
GLEWAPI void glGetPerfMonitorCountersAMD(GLuint group, GLint* numCounters, GLint *maxActiveCounters, GLsizei countersSize, GLuint *counters);
GLEWAPI void glGetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei* length, GLchar *groupString);
GLEWAPI void glGetPerfMonitorGroupsAMD(GLint* numGroups, GLsizei groupsSize, GLuint *groups);
GLEWAPI void glSelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* counterList);

GLEWAPI void glSetMultisamplefvAMD(GLenum pname, GLuint index, const GLfloat* val);

GLEWAPI void glStencilOpValueAMD(GLenum face, GLuint value);

GLEWAPI void glTessellationFactorAMD(GLfloat factor);
GLEWAPI void glTessellationModeAMD(GLenum mode);

GLEWAPI void glDrawElementArrayAPPLE(GLenum mode, GLint first, GLsizei count);
GLEWAPI void glDrawRangeElementArrayAPPLE(GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
GLEWAPI void glElementPointerAPPLE(GLenum type, const void* pointer);
GLEWAPI void glMultiDrawElementArrayAPPLE(GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
GLEWAPI void glMultiDrawRangeElementArrayAPPLE(GLenum mode, GLuint start, GLuint end, const GLint* first, const GLsizei *count, GLsizei primcount);

GLEWAPI void glDeleteFencesAPPLE(GLsizei n, const GLuint* fences);
GLEWAPI void glFinishFenceAPPLE(GLuint fence);
GLEWAPI void glFinishObjectAPPLE(GLenum object, GLint name);
GLEWAPI void glGenFencesAPPLE(GLsizei n, GLuint* fences);
GLEWAPI GLboolean glIsFenceAPPLE(GLuint fence);
GLEWAPI void glSetFenceAPPLE(GLuint fence);
GLEWAPI GLboolean glTestFenceAPPLE(GLuint fence);
GLEWAPI GLboolean glTestObjectAPPLE(GLenum object, GLuint name);

GLEWAPI void glBufferParameteriAPPLE(GLenum target, GLenum pname, GLint param);
GLEWAPI void glFlushMappedBufferRangeAPPLE(GLenum target, GLintptr offset, GLsizeiptr size);

GLEWAPI void glGetObjectParameterivAPPLE(GLenum objectType, GLuint name, GLenum pname, GLint* params);
GLEWAPI GLenum glObjectPurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);
GLEWAPI GLenum glObjectUnpurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);

GLEWAPI void glGetTexParameterPointervAPPLE(GLenum target, GLenum pname, GLvoid **params);
GLEWAPI void glTextureRangeAPPLE(GLenum target, GLsizei length, GLvoid *pointer);

GLEWAPI void glBindVertexArrayAPPLE(GLuint array);
GLEWAPI void glDeleteVertexArraysAPPLE(GLsizei n, const GLuint* arrays);
GLEWAPI void glGenVertexArraysAPPLE(GLsizei n, const GLuint* arrays);
GLEWAPI GLboolean glIsVertexArrayAPPLE(GLuint array);

GLEWAPI void glFlushVertexArrayRangeAPPLE(GLsizei length, void* pointer);
GLEWAPI void glVertexArrayParameteriAPPLE(GLenum pname, GLint param);
GLEWAPI void glVertexArrayRangeAPPLE(GLsizei length, void* pointer);

GLEWAPI void glDisableVertexAttribAPPLE(GLuint index, GLenum pname);
GLEWAPI void glEnableVertexAttribAPPLE(GLuint index, GLenum pname);
GLEWAPI GLboolean glIsVertexAttribEnabledAPPLE(GLuint index, GLenum pname);
GLEWAPI void glMapVertexAttrib1dAPPLE(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
GLEWAPI void glMapVertexAttrib1fAPPLE(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points);
GLEWAPI void glMapVertexAttrib2dAPPLE(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
GLEWAPI void glMapVertexAttrib2fAPPLE(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points);

GLEWAPI void glClearDepthf(GLclampf d);
GLEWAPI void glDepthRangef(GLclampf n, GLclampf f);
GLEWAPI void glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint *precision);
GLEWAPI void glReleaseShaderCompiler(void);
GLEWAPI void glShaderBinary(GLsizei count, const GLuint* shaders, GLenum binaryformat, const GLvoid*binary, GLsizei length);

GLEWAPI void glDrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
GLEWAPI void glDrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLuint baseinstance);
GLEWAPI void glDrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);

GLEWAPI void glBindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
GLEWAPI GLint glGetFragDataIndex(GLuint program, const GLchar * name);

GLEWAPI GLsync glCreateSyncFromCLeventARB(cl_context context, cl_event event, GLbitfield flags);

GLEWAPI void glClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const GLvoid* data);
GLEWAPI void glClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid* data);
GLEWAPI void glClearNamedBufferDataEXT(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid* data);
GLEWAPI void glClearNamedBufferSubDataEXT(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid* data);

GLEWAPI void glClampColorARB(GLenum target, GLenum clamp);

GLEWAPI void glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
GLEWAPI void glDispatchComputeIndirect(GLintptr indirect);

GLEWAPI void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);

GLEWAPI void glCopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

GLEWAPI void glDebugMessageCallbackARB(GLDEBUGPROCARB callback, void* userParam);
GLEWAPI void glDebugMessageControlARB(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
GLEWAPI void glDebugMessageInsertARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
GLEWAPI GLuint glGetDebugMessageLogARB(GLuint count, GLsizei bufsize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);

GLEWAPI void glDrawBuffersARB(GLsizei n, const GLenum* bufs);

GLEWAPI void glBlendEquationSeparateiARB(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GLEWAPI void glBlendEquationiARB(GLuint buf, GLenum mode);
GLEWAPI void glBlendFuncSeparateiARB(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GLEWAPI void glBlendFunciARB(GLuint buf, GLenum src, GLenum dst);

GLEWAPI void glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex);
GLEWAPI void glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex);
GLEWAPI void glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, void* indices, GLint basevertex);
GLEWAPI void glMultiDrawElementsBaseVertex(GLenum mode, GLsizei* count, GLenum type, GLvoid**indices, GLsizei primcount, GLint *basevertex);

GLEWAPI void glDrawArraysIndirect(GLenum mode, const void* indirect);
GLEWAPI void glDrawElementsIndirect(GLenum mode, GLenum type, const void* indirect);

GLEWAPI void glFramebufferParameteri(GLenum target, GLenum pname, GLint param);
GLEWAPI void glGetFramebufferParameteriv(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetNamedFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint* params);
GLEWAPI void glNamedFramebufferParameteriEXT(GLuint framebuffer, GLenum pname, GLint param);

GLEWAPI void glBindFramebuffer(GLenum target, GLuint framebuffer);
GLEWAPI void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
GLEWAPI void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLEWAPI GLenum glCheckFramebufferStatus(GLenum target);
GLEWAPI void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
GLEWAPI void glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
GLEWAPI void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLEWAPI void glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLEWAPI void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLEWAPI void glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
GLEWAPI void glFramebufferTextureLayer(GLenum target,GLenum attachment, GLuint texture,GLint level,GLint layer);
GLEWAPI void glGenFramebuffers(GLsizei n, GLuint* framebuffers);
GLEWAPI void glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
GLEWAPI void glGenerateMipmap(GLenum target);
GLEWAPI void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
GLEWAPI void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsFramebuffer(GLuint framebuffer);
GLEWAPI GLboolean glIsRenderbuffer(GLuint renderbuffer);
GLEWAPI void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GLEWAPI void glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

GLEWAPI void glFramebufferTextureARB(GLenum target, GLenum attachment, GLuint texture, GLint level);
GLEWAPI void glFramebufferTextureFaceARB(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
GLEWAPI void glFramebufferTextureLayerARB(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLEWAPI void glProgramParameteriARB(GLuint program, GLenum pname, GLint value);

GLEWAPI void glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum *binaryFormat, GLvoid*binary);
GLEWAPI void glProgramBinary(GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
GLEWAPI void glProgramParameteri(GLuint program, GLenum pname, GLint value);

GLEWAPI void glGetUniformdv(GLuint program, GLint location, GLdouble* params);
GLEWAPI void glUniform1d(GLint location, GLdouble x);
GLEWAPI void glUniform1dv(GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glUniform2d(GLint location, GLdouble x, GLdouble y);
GLEWAPI void glUniform2dv(GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glUniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glUniform3dv(GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glUniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glUniform4dv(GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glUniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glUniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);

GLEWAPI void glColorSubTable(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
GLEWAPI void glColorTable(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
GLEWAPI void glColorTableParameterfv(GLenum target, GLenum pname, const GLfloat *params);
GLEWAPI void glColorTableParameteriv(GLenum target, GLenum pname, const GLint *params);
GLEWAPI void glConvolutionFilter1D(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
GLEWAPI void glConvolutionFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
GLEWAPI void glConvolutionParameterf(GLenum target, GLenum pname, GLfloat params);
GLEWAPI void glConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat *params);
GLEWAPI void glConvolutionParameteri(GLenum target, GLenum pname, GLint params);
GLEWAPI void glConvolutionParameteriv(GLenum target, GLenum pname, const GLint *params);
GLEWAPI void glCopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyConvolutionFilter1D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyConvolutionFilter2D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glGetColorTable(GLenum target, GLenum format, GLenum type, GLvoid *table);
GLEWAPI void glGetColorTableParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLEWAPI void glGetColorTableParameteriv(GLenum target, GLenum pname, GLint *params);
GLEWAPI void glGetConvolutionFilter(GLenum target, GLenum format, GLenum type, GLvoid *image);
GLEWAPI void glGetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLEWAPI void glGetConvolutionParameteriv(GLenum target, GLenum pname, GLint *params);
GLEWAPI void glGetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
GLEWAPI void glGetHistogramParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLEWAPI void glGetHistogramParameteriv(GLenum target, GLenum pname, GLint *params);
GLEWAPI void glGetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
GLEWAPI void glGetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat *params);
GLEWAPI void glGetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params);
GLEWAPI void glGetSeparableFilter(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
GLEWAPI void glHistogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
GLEWAPI void glMinmax(GLenum target, GLenum internalformat, GLboolean sink);
GLEWAPI void glResetHistogram(GLenum target);
GLEWAPI void glResetMinmax(GLenum target);
GLEWAPI void glSeparableFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);

GLEWAPI void glDrawArraysInstancedARB(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
GLEWAPI void glDrawElementsInstancedARB(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
GLEWAPI void glVertexAttribDivisorARB(GLuint index, GLuint divisor);

GLEWAPI void glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);

GLEWAPI void glGetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64* params);

GLEWAPI void glInvalidateBufferData(GLuint buffer);
GLEWAPI void glInvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length);
GLEWAPI void glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments);
GLEWAPI void glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glInvalidateTexImage(GLuint texture, GLint level);
GLEWAPI void glInvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);

GLEWAPI void glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
GLEWAPI GLvoid * glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

GLEWAPI void glCurrentPaletteMatrixARB(GLint index);
GLEWAPI void glMatrixIndexPointerARB(GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
GLEWAPI void glMatrixIndexubvARB(GLint size, GLubyte *indices);
GLEWAPI void glMatrixIndexuivARB(GLint size, GLuint *indices);
GLEWAPI void glMatrixIndexusvARB(GLint size, GLushort *indices);

GLEWAPI void glMultiDrawArraysIndirect(GLenum mode, const void* indirect, GLsizei primcount, GLsizei stride);
GLEWAPI void glMultiDrawElementsIndirect(GLenum mode, GLenum type, const void* indirect, GLsizei primcount, GLsizei stride);

GLEWAPI void glSampleCoverageARB(GLclampf value, GLboolean invert);

GLEWAPI void glActiveTextureARB(GLenum texture);
GLEWAPI void glClientActiveTextureARB(GLenum texture);
GLEWAPI void glMultiTexCoord1dARB(GLenum target, GLdouble s);
GLEWAPI void glMultiTexCoord1dvARB(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord1fARB(GLenum target, GLfloat s);
GLEWAPI void glMultiTexCoord1fvARB(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord1iARB(GLenum target, GLint s);
GLEWAPI void glMultiTexCoord1ivARB(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord1sARB(GLenum target, GLshort s);
GLEWAPI void glMultiTexCoord1svARB(GLenum target, const GLshort *v);
GLEWAPI void glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t);
GLEWAPI void glMultiTexCoord2dvARB(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);
GLEWAPI void glMultiTexCoord2fvARB(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord2iARB(GLenum target, GLint s, GLint t);
GLEWAPI void glMultiTexCoord2ivARB(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t);
GLEWAPI void glMultiTexCoord2svARB(GLenum target, const GLshort *v);
GLEWAPI void glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r);
GLEWAPI void glMultiTexCoord3dvARB(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);
GLEWAPI void glMultiTexCoord3fvARB(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r);
GLEWAPI void glMultiTexCoord3ivARB(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r);
GLEWAPI void glMultiTexCoord3svARB(GLenum target, const GLshort *v);
GLEWAPI void glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLEWAPI void glMultiTexCoord4dvARB(GLenum target, const GLdouble *v);
GLEWAPI void glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLEWAPI void glMultiTexCoord4fvARB(GLenum target, const GLfloat *v);
GLEWAPI void glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q);
GLEWAPI void glMultiTexCoord4ivARB(GLenum target, const GLint *v);
GLEWAPI void glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
GLEWAPI void glMultiTexCoord4svARB(GLenum target, const GLshort *v);

GLEWAPI void glBeginQueryARB(GLenum target, GLuint id);
GLEWAPI void glDeleteQueriesARB(GLsizei n, const GLuint* ids);
GLEWAPI void glEndQueryARB(GLenum target);
GLEWAPI void glGenQueriesARB(GLsizei n, GLuint* ids);
GLEWAPI void glGetQueryObjectivARB(GLuint id, GLenum pname, GLint* params);
GLEWAPI void glGetQueryObjectuivARB(GLuint id, GLenum pname, GLuint* params);
GLEWAPI void glGetQueryivARB(GLenum target, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsQueryARB(GLuint id);

GLEWAPI void glPointParameterfARB(GLenum pname, GLfloat param);
GLEWAPI void glPointParameterfvARB(GLenum pname, const GLfloat* params);

GLEWAPI void glGetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint* params);
GLEWAPI GLuint glGetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar* name);
GLEWAPI GLint glGetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar* name);
GLEWAPI GLint glGetProgramResourceLocationIndex(GLuint program, GLenum programInterface, const GLchar* name);
GLEWAPI void glGetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, GLchar *name);
GLEWAPI void glGetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei *length, GLint *params);

GLEWAPI void glProvokingVertex(GLenum mode);

GLEWAPI GLenum glGetGraphicsResetStatusARB(void);
GLEWAPI void glGetnColorTableARB(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* table);
GLEWAPI void glGetnCompressedTexImageARB(GLenum target, GLint lod, GLsizei bufSize, void* img);
GLEWAPI void glGetnConvolutionFilterARB(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* image);
GLEWAPI void glGetnHistogramARB(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
GLEWAPI void glGetnMapdvARB(GLenum target, GLenum query, GLsizei bufSize, GLdouble* v);
GLEWAPI void glGetnMapfvARB(GLenum target, GLenum query, GLsizei bufSize, GLfloat* v);
GLEWAPI void glGetnMapivARB(GLenum target, GLenum query, GLsizei bufSize, GLint* v);
GLEWAPI void glGetnMinmaxARB(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
GLEWAPI void glGetnPixelMapfvARB(GLenum map, GLsizei bufSize, GLfloat* values);
GLEWAPI void glGetnPixelMapuivARB(GLenum map, GLsizei bufSize, GLuint* values);
GLEWAPI void glGetnPixelMapusvARB(GLenum map, GLsizei bufSize, GLushort* values);
GLEWAPI void glGetnPolygonStippleARB(GLsizei bufSize, GLubyte* pattern);
GLEWAPI void glGetnSeparableFilterARB(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void* row, GLsizei columnBufSize, GLvoid*column, GLvoid*span);
GLEWAPI void glGetnTexImageARB(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* img);
GLEWAPI void glGetnUniformdvARB(GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
GLEWAPI void glGetnUniformfvARB(GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
GLEWAPI void glGetnUniformivARB(GLuint program, GLint location, GLsizei bufSize, GLint* params);
GLEWAPI void glGetnUniformuivARB(GLuint program, GLint location, GLsizei bufSize, GLuint* params);
GLEWAPI void glReadnPixelsARB(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);

GLEWAPI void glMinSampleShadingARB(GLclampf value);

GLEWAPI void glBindSampler(GLuint unit, GLuint sampler);
GLEWAPI void glDeleteSamplers(GLsizei count, const GLuint * samplers);
GLEWAPI void glGenSamplers(GLsizei count, GLuint* samplers);
GLEWAPI void glGetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint* params);
GLEWAPI void glGetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint* params);
GLEWAPI void glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat* params);
GLEWAPI void glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsSampler(GLuint sampler);
GLEWAPI void glSamplerParameterIiv(GLuint sampler, GLenum pname, const GLint* params);
GLEWAPI void glSamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint* params);
GLEWAPI void glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
GLEWAPI void glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* params);
GLEWAPI void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
GLEWAPI void glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint* params);

GLEWAPI void glActiveShaderProgram(GLuint pipeline, GLuint program);
GLEWAPI void glBindProgramPipeline(GLuint pipeline);
GLEWAPI GLuint glCreateShaderProgramv(GLenum type, GLsizei count, const GLchar ** strings);
GLEWAPI void glDeleteProgramPipelines(GLsizei n, const GLuint* pipelines);
GLEWAPI void glGenProgramPipelines(GLsizei n, GLuint* pipelines);
GLEWAPI void glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar *infoLog);
GLEWAPI void glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsProgramPipeline(GLuint pipeline);
GLEWAPI void glProgramUniform1d(GLuint program, GLint location, GLdouble x);
GLEWAPI void glProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glProgramUniform1f(GLuint program, GLint location, GLfloat x);
GLEWAPI void glProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform1i(GLuint program, GLint location, GLint x);
GLEWAPI void glProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform1ui(GLuint program, GLint location, GLuint x);
GLEWAPI void glProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniform2d(GLuint program, GLint location, GLdouble x, GLdouble y);
GLEWAPI void glProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glProgramUniform2f(GLuint program, GLint location, GLfloat x, GLfloat y);
GLEWAPI void glProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform2i(GLuint program, GLint location, GLint x, GLint y);
GLEWAPI void glProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform2ui(GLuint program, GLint location, GLuint x, GLuint y);
GLEWAPI void glProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniform3d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glProgramUniform3f(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform3i(GLuint program, GLint location, GLint x, GLint y, GLint z);
GLEWAPI void glProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform3ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
GLEWAPI void glProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniform4d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble* value);
GLEWAPI void glProgramUniform4f(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform4i(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform4ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
GLEWAPI void glProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
GLEWAPI void glProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program);
GLEWAPI void glValidateProgramPipeline(GLuint pipeline);

GLEWAPI void glGetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);

GLEWAPI void glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
GLEWAPI void glMemoryBarrier(GLbitfield barriers);

GLEWAPI void glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj);
GLEWAPI void glCompileShaderARB(GLhandleARB shaderObj);
GLEWAPI GLhandleARB glCreateProgramObjectARB(void);
GLEWAPI GLhandleARB glCreateShaderObjectARB(GLenum shaderType);
GLEWAPI void glDeleteObjectARB(GLhandleARB obj);
GLEWAPI void glDetachObjectARB(GLhandleARB containerObj, GLhandleARB attachedObj);
GLEWAPI void glGetActiveUniformARB(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
GLEWAPI void glGetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxCount, GLsizei* count, GLhandleARB *obj);
GLEWAPI GLhandleARB glGetHandleARB(GLenum pname);
GLEWAPI void glGetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog);
GLEWAPI void glGetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat* params);
GLEWAPI void glGetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint* params);
GLEWAPI void glGetShaderSourceARB(GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *source);
GLEWAPI GLint glGetUniformLocationARB(GLhandleARB programObj, const GLcharARB* name);
GLEWAPI void glGetUniformfvARB(GLhandleARB programObj, GLint location, GLfloat* params);
GLEWAPI void glGetUniformivARB(GLhandleARB programObj, GLint location, GLint* params);
GLEWAPI void glLinkProgramARB(GLhandleARB programObj);
GLEWAPI void glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length);
GLEWAPI void glUniform1fARB(GLint location, GLfloat v0);
GLEWAPI void glUniform1fvARB(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform1iARB(GLint location, GLint v0);
GLEWAPI void glUniform1ivARB(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniform2fARB(GLint location, GLfloat v0, GLfloat v1);
GLEWAPI void glUniform2fvARB(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform2iARB(GLint location, GLint v0, GLint v1);
GLEWAPI void glUniform2ivARB(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniform3fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLEWAPI void glUniform3fvARB(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform3iARB(GLint location, GLint v0, GLint v1, GLint v2);
GLEWAPI void glUniform3ivARB(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniform4fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLEWAPI void glUniform4fvARB(GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glUniform4iARB(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLEWAPI void glUniform4ivARB(GLint location, GLsizei count, const GLint* value);
GLEWAPI void glUniformMatrix2fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glUniformMatrix3fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glUniformMatrix4fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glUseProgramObjectARB(GLhandleARB programObj);
GLEWAPI void glValidateProgramARB(GLhandleARB programObj);

GLEWAPI void glShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);

GLEWAPI void glGetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
GLEWAPI void glGetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
GLEWAPI void glGetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
GLEWAPI void glGetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint* values);
GLEWAPI GLuint glGetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar* name);
GLEWAPI GLint glGetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar* name);
GLEWAPI void glGetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint* params);
GLEWAPI void glUniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint* indices);

GLEWAPI void glCompileShaderIncludeARB(GLuint shader, GLsizei count, const GLchar ** path, const GLint *length);
GLEWAPI void glDeleteNamedStringARB(GLint namelen, const GLchar* name);
GLEWAPI void glGetNamedStringARB(GLint namelen, const GLchar* name, GLsizei bufSize, GLint *stringlen, GLchar *string);
GLEWAPI void glGetNamedStringivARB(GLint namelen, const GLchar* name, GLenum pname, GLint *params);
GLEWAPI GLboolean glIsNamedStringARB(GLint namelen, const GLchar* name);
GLEWAPI void glNamedStringARB(GLenum type, GLint namelen, const GLchar* name, GLint stringlen, const GLchar *string);

GLEWAPI GLenum glClientWaitSync(GLsync GLsync,GLbitfield flags,GLuint64 timeout);
GLEWAPI void glDeleteSync(GLsync GLsync);
GLEWAPI GLsync glFenceSync(GLenum condition,GLbitfield flags);
GLEWAPI void glGetInteger64v(GLenum pname, GLint64* params);
GLEWAPI void glGetSynciv(GLsync GLsync,GLenum pname,GLsizei bufSize,GLsizei* length, GLint *values);
GLEWAPI GLboolean glIsSync(GLsync GLsync);
GLEWAPI void glWaitSync(GLsync GLsync,GLbitfield flags,GLuint64 timeout);

GLEWAPI void glPatchParameterfv(GLenum pname, const GLfloat* values);
GLEWAPI void glPatchParameteri(GLenum pname, GLint value);

GLEWAPI void glTexBufferARB(GLenum target, GLenum internalformat, GLuint buffer);

GLEWAPI void glTexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLEWAPI void glTextureBufferRangeEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);

GLEWAPI void glCompressedTexImage1DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTexImage2DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTexImage3DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTexSubImage1DARB(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTexSubImage2DARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTexSubImage3DARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glGetCompressedTexImageARB(GLenum target, GLint lod, void* img);

GLEWAPI void glGetMultisamplefv(GLenum pname, GLuint index, GLfloat* val);
GLEWAPI void glSampleMaski(GLuint index, GLbitfield mask);
GLEWAPI void glTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLEWAPI void glTexImage3DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

GLEWAPI void glTexStorage1D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
GLEWAPI void glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GLEWAPI void glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
GLEWAPI void glTextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
GLEWAPI void glTextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GLEWAPI void glTextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

GLEWAPI void glTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLEWAPI void glTexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLEWAPI void glTextureStorage2DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLEWAPI void glTextureStorage3DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

GLEWAPI void glTextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);

GLEWAPI void glGetQueryObjecti64v(GLuint id, GLenum pname, GLint64* params);
GLEWAPI void glGetQueryObjectui64v(GLuint id, GLenum pname, GLuint64* params);
GLEWAPI void glQueryCounter(GLuint id, GLenum target);

GLEWAPI void glBindTransformFeedback(GLenum target, GLuint id);
GLEWAPI void glDeleteTransformFeedbacks(GLsizei n, const GLuint* ids);
GLEWAPI void glDrawTransformFeedback(GLenum mode, GLuint id);
GLEWAPI void glGenTransformFeedbacks(GLsizei n, GLuint* ids);
GLEWAPI GLboolean glIsTransformFeedback(GLuint id);
GLEWAPI void glPauseTransformFeedback(void);
GLEWAPI void glResumeTransformFeedback(void);

GLEWAPI void glBeginQueryIndexed(GLenum target, GLuint index, GLuint id);
GLEWAPI void glDrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream);
GLEWAPI void glEndQueryIndexed(GLenum target, GLuint index);
GLEWAPI void glGetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint* params);

GLEWAPI void glDrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei primcount);
GLEWAPI void glDrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei primcount);

GLEWAPI void glLoadTransposeMatrixdARB(GLdouble m[16]);
GLEWAPI void glLoadTransposeMatrixfARB(GLfloat m[16]);
GLEWAPI void glMultTransposeMatrixdARB(GLdouble m[16]);
GLEWAPI void glMultTransposeMatrixfARB(GLfloat m[16]);

GLEWAPI void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
GLEWAPI void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLEWAPI void glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
GLEWAPI void glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
GLEWAPI void glGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
GLEWAPI void glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
GLEWAPI void glGetIntegeri_v(GLenum target, GLuint index, GLint* data);
GLEWAPI GLuint glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);
GLEWAPI void glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices);
GLEWAPI void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

GLEWAPI void glBindVertexArray(GLuint array);
GLEWAPI void glDeleteVertexArrays(GLsizei n, const GLuint* arrays);
GLEWAPI void glGenVertexArrays(GLsizei n, GLuint* arrays);
GLEWAPI GLboolean glIsVertexArray(GLuint array);

GLEWAPI void glGetVertexAttribLdv(GLuint index, GLenum pname, GLdouble* params);
GLEWAPI void glVertexAttribL1d(GLuint index, GLdouble x);
GLEWAPI void glVertexAttribL1dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribL2d(GLuint index, GLdouble x, GLdouble y);
GLEWAPI void glVertexAttribL2dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glVertexAttribL3dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glVertexAttribL4dv(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

GLEWAPI void glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
GLEWAPI void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex);
GLEWAPI void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
GLEWAPI void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLEWAPI void glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLEWAPI void glVertexBindingDivisor(GLuint bindingindex, GLuint divisor);

GLEWAPI void glVertexBlendARB(GLint count);
GLEWAPI void glWeightPointerARB(GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
GLEWAPI void glWeightbvARB(GLint size, GLbyte *weights);
GLEWAPI void glWeightdvARB(GLint size, GLdouble *weights);
GLEWAPI void glWeightfvARB(GLint size, GLfloat *weights);
GLEWAPI void glWeightivARB(GLint size, GLint *weights);
GLEWAPI void glWeightsvARB(GLint size, GLshort *weights);
GLEWAPI void glWeightubvARB(GLint size, GLubyte *weights);
GLEWAPI void glWeightuivARB(GLint size, GLuint *weights);
GLEWAPI void glWeightusvARB(GLint size, GLushort *weights);

GLEWAPI void glBindBufferARB(GLenum target, GLuint buffer);
GLEWAPI void glBufferDataARB(GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
GLEWAPI void glBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid* data);
GLEWAPI void glDeleteBuffersARB(GLsizei n, const GLuint* buffers);
GLEWAPI void glGenBuffersARB(GLsizei n, GLuint* buffers);
GLEWAPI void glGetBufferParameterivARB(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetBufferPointervARB(GLenum target, GLenum pname, GLvoid** params);
GLEWAPI void glGetBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
GLEWAPI GLboolean glIsBufferARB(GLuint buffer);
GLEWAPI GLvoid * glMapBufferARB(GLenum target, GLenum access);
GLEWAPI GLboolean glUnmapBufferARB(GLenum target);

GLEWAPI void glBindProgramARB(GLenum target, GLuint program);
GLEWAPI void glDeleteProgramsARB(GLsizei n, const GLuint* programs);
GLEWAPI void glDisableVertexAttribArrayARB(GLuint index);
GLEWAPI void glEnableVertexAttribArrayARB(GLuint index);
GLEWAPI void glGenProgramsARB(GLsizei n, GLuint* programs);
GLEWAPI void glGetProgramEnvParameterdvARB(GLenum target, GLuint index, GLdouble* params);
GLEWAPI void glGetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat* params);
GLEWAPI void glGetProgramLocalParameterdvARB(GLenum target, GLuint index, GLdouble* params);
GLEWAPI void glGetProgramLocalParameterfvARB(GLenum target, GLuint index, GLfloat* params);
GLEWAPI void glGetProgramStringARB(GLenum target, GLenum pname, void* string);
GLEWAPI void glGetProgramivARB(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetVertexAttribPointervARB(GLuint index, GLenum pname, GLvoid** pointer);
GLEWAPI void glGetVertexAttribdvARB(GLuint index, GLenum pname, GLdouble* params);
GLEWAPI void glGetVertexAttribfvARB(GLuint index, GLenum pname, GLfloat* params);
GLEWAPI void glGetVertexAttribivARB(GLuint index, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsProgramARB(GLuint program);
GLEWAPI void glProgramEnvParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glProgramEnvParameter4dvARB(GLenum target, GLuint index, const GLdouble* params);
GLEWAPI void glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat* params);
GLEWAPI void glProgramLocalParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glProgramLocalParameter4dvARB(GLenum target, GLuint index, const GLdouble* params);
GLEWAPI void glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat* params);
GLEWAPI void glProgramStringARB(GLenum target, GLenum format, GLsizei len, const void* string);
GLEWAPI void glVertexAttrib1dARB(GLuint index, GLdouble x);
GLEWAPI void glVertexAttrib1dvARB(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib1fARB(GLuint index, GLfloat x);
GLEWAPI void glVertexAttrib1fvARB(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib1sARB(GLuint index, GLshort x);
GLEWAPI void glVertexAttrib1svARB(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib2dARB(GLuint index, GLdouble x, GLdouble y);
GLEWAPI void glVertexAttrib2dvARB(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y);
GLEWAPI void glVertexAttrib2fvARB(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib2sARB(GLuint index, GLshort x, GLshort y);
GLEWAPI void glVertexAttrib2svARB(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib3dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glVertexAttrib3dvARB(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glVertexAttrib3fvARB(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib3sARB(GLuint index, GLshort x, GLshort y, GLshort z);
GLEWAPI void glVertexAttrib3svARB(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4NbvARB(GLuint index, const GLbyte* v);
GLEWAPI void glVertexAttrib4NivARB(GLuint index, const GLint* v);
GLEWAPI void glVertexAttrib4NsvARB(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4NubARB(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
GLEWAPI void glVertexAttrib4NubvARB(GLuint index, const GLubyte* v);
GLEWAPI void glVertexAttrib4NuivARB(GLuint index, const GLuint* v);
GLEWAPI void glVertexAttrib4NusvARB(GLuint index, const GLushort* v);
GLEWAPI void glVertexAttrib4bvARB(GLuint index, const GLbyte* v);
GLEWAPI void glVertexAttrib4dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glVertexAttrib4dvARB(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glVertexAttrib4fvARB(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib4ivARB(GLuint index, const GLint* v);
GLEWAPI void glVertexAttrib4sARB(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
GLEWAPI void glVertexAttrib4svARB(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4ubvARB(GLuint index, const GLubyte* v);
GLEWAPI void glVertexAttrib4uivARB(GLuint index, const GLuint* v);
GLEWAPI void glVertexAttrib4usvARB(GLuint index, const GLushort* v);
GLEWAPI void glVertexAttribPointerARB(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

GLEWAPI void glBindAttribLocationARB(GLhandleARB programObj, GLuint index, const GLcharARB* name);
GLEWAPI void glGetActiveAttribARB(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
GLEWAPI GLint glGetAttribLocationARB(GLhandleARB programObj, const GLcharARB* name);

GLEWAPI void glColorP3ui(GLenum type, GLuint color);
GLEWAPI void glColorP3uiv(GLenum type, const GLuint* color);
GLEWAPI void glColorP4ui(GLenum type, GLuint color);
GLEWAPI void glColorP4uiv(GLenum type, const GLuint* color);
GLEWAPI void glMultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords);
GLEWAPI void glMultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint* coords);
GLEWAPI void glMultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords);
GLEWAPI void glMultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint* coords);
GLEWAPI void glMultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords);
GLEWAPI void glMultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint* coords);
GLEWAPI void glMultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords);
GLEWAPI void glMultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint* coords);
GLEWAPI void glNormalP3ui(GLenum type, GLuint coords);
GLEWAPI void glNormalP3uiv(GLenum type, const GLuint* coords);
GLEWAPI void glSecondaryColorP3ui(GLenum type, GLuint color);
GLEWAPI void glSecondaryColorP3uiv(GLenum type, const GLuint* color);
GLEWAPI void glTexCoordP1ui(GLenum type, GLuint coords);
GLEWAPI void glTexCoordP1uiv(GLenum type, const GLuint* coords);
GLEWAPI void glTexCoordP2ui(GLenum type, GLuint coords);
GLEWAPI void glTexCoordP2uiv(GLenum type, const GLuint* coords);
GLEWAPI void glTexCoordP3ui(GLenum type, GLuint coords);
GLEWAPI void glTexCoordP3uiv(GLenum type, const GLuint* coords);
GLEWAPI void glTexCoordP4ui(GLenum type, GLuint coords);
GLEWAPI void glTexCoordP4uiv(GLenum type, const GLuint* coords);
GLEWAPI void glVertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLEWAPI void glVertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
GLEWAPI void glVertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLEWAPI void glVertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
GLEWAPI void glVertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLEWAPI void glVertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
GLEWAPI void glVertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLEWAPI void glVertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
GLEWAPI void glVertexP2ui(GLenum type, GLuint value);
GLEWAPI void glVertexP2uiv(GLenum type, const GLuint* value);
GLEWAPI void glVertexP3ui(GLenum type, GLuint value);
GLEWAPI void glVertexP3uiv(GLenum type, const GLuint* value);
GLEWAPI void glVertexP4ui(GLenum type, GLuint value);
GLEWAPI void glVertexP4uiv(GLenum type, const GLuint* value);

GLEWAPI void glDepthRangeArrayv(GLuint first, GLsizei count, const GLclampd * v);
GLEWAPI void glDepthRangeIndexed(GLuint index, GLclampd n, GLclampd f);
GLEWAPI void glGetDoublei_v(GLenum target, GLuint index, GLdouble* data);
GLEWAPI void glGetFloati_v(GLenum target, GLuint index, GLfloat* data);
GLEWAPI void glScissorArrayv(GLuint first, GLsizei count, const GLint * v);
GLEWAPI void glScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
GLEWAPI void glScissorIndexedv(GLuint index, const GLint * v);
GLEWAPI void glViewportArrayv(GLuint first, GLsizei count, const GLfloat * v);
GLEWAPI void glViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
GLEWAPI void glViewportIndexedfv(GLuint index, const GLfloat * v);

GLEWAPI void glWindowPos2dARB(GLdouble x, GLdouble y);
GLEWAPI void glWindowPos2dvARB(const GLdouble* p);
GLEWAPI void glWindowPos2fARB(GLfloat x, GLfloat y);
GLEWAPI void glWindowPos2fvARB(const GLfloat* p);
GLEWAPI void glWindowPos2iARB(GLint x, GLint y);
GLEWAPI void glWindowPos2ivARB(const GLint* p);
GLEWAPI void glWindowPos2sARB(GLshort x, GLshort y);
GLEWAPI void glWindowPos2svARB(const GLshort* p);
GLEWAPI void glWindowPos3dARB(GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glWindowPos3dvARB(const GLdouble* p);
GLEWAPI void glWindowPos3fARB(GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glWindowPos3fvARB(const GLfloat* p);
GLEWAPI void glWindowPos3iARB(GLint x, GLint y, GLint z);
GLEWAPI void glWindowPos3ivARB(const GLint* p);
GLEWAPI void glWindowPos3sARB(GLshort x, GLshort y, GLshort z);
GLEWAPI void glWindowPos3svARB(const GLshort* p);

GLEWAPI void glDrawBuffersATI(GLsizei n, const GLenum* bufs);

GLEWAPI void glDrawElementArrayATI(GLenum mode, GLsizei count);
GLEWAPI void glDrawRangeElementArrayATI(GLenum mode, GLuint start, GLuint end, GLsizei count);
GLEWAPI void glElementPointerATI(GLenum type, const void* pointer);

GLEWAPI void glGetTexBumpParameterfvATI(GLenum pname, GLfloat *param);
GLEWAPI void glGetTexBumpParameterivATI(GLenum pname, GLint *param);
GLEWAPI void glTexBumpParameterfvATI(GLenum pname, GLfloat *param);
GLEWAPI void glTexBumpParameterivATI(GLenum pname, GLint *param);

GLEWAPI void glAlphaFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
GLEWAPI void glAlphaFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
GLEWAPI void glAlphaFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
GLEWAPI void glBeginFragmentShaderATI(void);
GLEWAPI void glBindFragmentShaderATI(GLuint id);
GLEWAPI void glColorFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
GLEWAPI void glColorFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
GLEWAPI void glColorFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
GLEWAPI void glDeleteFragmentShaderATI(GLuint id);
GLEWAPI void glEndFragmentShaderATI(void);
GLEWAPI GLuint glGenFragmentShadersATI(GLuint range);
GLEWAPI void glPassTexCoordATI(GLuint dst, GLuint coord, GLenum swizzle);
GLEWAPI void glSampleMapATI(GLuint dst, GLuint interp, GLenum swizzle);
GLEWAPI void glSetFragmentShaderConstantATI(GLuint dst, const GLfloat* value);

GLEWAPI GLvoid * glMapObjectBufferATI(GLuint buffer);
GLEWAPI void glUnmapObjectBufferATI(GLuint buffer);

GLEWAPI void glPNTrianglesfATI(GLenum pname, GLfloat param);
GLEWAPI void glPNTrianglesiATI(GLenum pname, GLint param);

GLEWAPI void glStencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
GLEWAPI void glStencilOpSeparateATI(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);

GLEWAPI void glArrayObjectATI(GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
GLEWAPI void glFreeObjectBufferATI(GLuint buffer);
GLEWAPI void glGetArrayObjectfvATI(GLenum array, GLenum pname, GLfloat* params);
GLEWAPI void glGetArrayObjectivATI(GLenum array, GLenum pname, GLint* params);
GLEWAPI void glGetObjectBufferfvATI(GLuint buffer, GLenum pname, GLfloat* params);
GLEWAPI void glGetObjectBufferivATI(GLuint buffer, GLenum pname, GLint* params);
GLEWAPI void glGetVariantArrayObjectfvATI(GLuint id, GLenum pname, GLfloat* params);
GLEWAPI void glGetVariantArrayObjectivATI(GLuint id, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsObjectBufferATI(GLuint buffer);
GLEWAPI GLuint glNewObjectBufferATI(GLsizei size, const void* pointer, GLenum usage);
GLEWAPI void glUpdateObjectBufferATI(GLuint buffer, GLuint offset, GLsizei size, const void* pointer, GLenum preserve);
GLEWAPI void glVariantArrayObjectATI(GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);

GLEWAPI void glGetVertexAttribArrayObjectfvATI(GLuint index, GLenum pname, GLfloat* params);
GLEWAPI void glGetVertexAttribArrayObjectivATI(GLuint index, GLenum pname, GLint* params);
GLEWAPI void glVertexAttribArrayObjectATI(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);

GLEWAPI void glClientActiveVertexStreamATI(GLenum stream);
GLEWAPI void glNormalStream3bATI(GLenum stream, GLbyte x, GLbyte y, GLbyte z);
GLEWAPI void glNormalStream3bvATI(GLenum stream, const GLbyte *coords);
GLEWAPI void glNormalStream3dATI(GLenum stream, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glNormalStream3dvATI(GLenum stream, const GLdouble *coords);
GLEWAPI void glNormalStream3fATI(GLenum stream, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glNormalStream3fvATI(GLenum stream, const GLfloat *coords);
GLEWAPI void glNormalStream3iATI(GLenum stream, GLint x, GLint y, GLint z);
GLEWAPI void glNormalStream3ivATI(GLenum stream, const GLint *coords);
GLEWAPI void glNormalStream3sATI(GLenum stream, GLshort x, GLshort y, GLshort z);
GLEWAPI void glNormalStream3svATI(GLenum stream, const GLshort *coords);
GLEWAPI void glVertexBlendEnvfATI(GLenum pname, GLfloat param);
GLEWAPI void glVertexBlendEnviATI(GLenum pname, GLint param);
GLEWAPI void glVertexStream1dATI(GLenum stream, GLdouble x);
GLEWAPI void glVertexStream1dvATI(GLenum stream, const GLdouble *coords);
GLEWAPI void glVertexStream1fATI(GLenum stream, GLfloat x);
GLEWAPI void glVertexStream1fvATI(GLenum stream, const GLfloat *coords);
GLEWAPI void glVertexStream1iATI(GLenum stream, GLint x);
GLEWAPI void glVertexStream1ivATI(GLenum stream, const GLint *coords);
GLEWAPI void glVertexStream1sATI(GLenum stream, GLshort x);
GLEWAPI void glVertexStream1svATI(GLenum stream, const GLshort *coords);
GLEWAPI void glVertexStream2dATI(GLenum stream, GLdouble x, GLdouble y);
GLEWAPI void glVertexStream2dvATI(GLenum stream, const GLdouble *coords);
GLEWAPI void glVertexStream2fATI(GLenum stream, GLfloat x, GLfloat y);
GLEWAPI void glVertexStream2fvATI(GLenum stream, const GLfloat *coords);
GLEWAPI void glVertexStream2iATI(GLenum stream, GLint x, GLint y);
GLEWAPI void glVertexStream2ivATI(GLenum stream, const GLint *coords);
GLEWAPI void glVertexStream2sATI(GLenum stream, GLshort x, GLshort y);
GLEWAPI void glVertexStream2svATI(GLenum stream, const GLshort *coords);
GLEWAPI void glVertexStream3dATI(GLenum stream, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glVertexStream3dvATI(GLenum stream, const GLdouble *coords);
GLEWAPI void glVertexStream3fATI(GLenum stream, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glVertexStream3fvATI(GLenum stream, const GLfloat *coords);
GLEWAPI void glVertexStream3iATI(GLenum stream, GLint x, GLint y, GLint z);
GLEWAPI void glVertexStream3ivATI(GLenum stream, const GLint *coords);
GLEWAPI void glVertexStream3sATI(GLenum stream, GLshort x, GLshort y, GLshort z);
GLEWAPI void glVertexStream3svATI(GLenum stream, const GLshort *coords);
GLEWAPI void glVertexStream4dATI(GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glVertexStream4dvATI(GLenum stream, const GLdouble *coords);
GLEWAPI void glVertexStream4fATI(GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glVertexStream4fvATI(GLenum stream, const GLfloat *coords);
GLEWAPI void glVertexStream4iATI(GLenum stream, GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glVertexStream4ivATI(GLenum stream, const GLint *coords);
GLEWAPI void glVertexStream4sATI(GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
GLEWAPI void glVertexStream4svATI(GLenum stream, const GLshort *coords);

GLEWAPI GLint glGetUniformBufferSizeEXT(GLuint program, GLint location);
GLEWAPI GLintptr glGetUniformOffsetEXT(GLuint program, GLint location);
GLEWAPI void glUniformBufferEXT(GLuint program, GLint location, GLuint buffer);

GLEWAPI void glBlendColorEXT(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

GLEWAPI void glBlendEquationSeparateEXT(GLenum modeRGB, GLenum modeAlpha);

GLEWAPI void glBlendFuncSeparateEXT(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

GLEWAPI void glBlendEquationEXT(GLenum mode);

GLEWAPI void glColorSubTableEXT(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void* data);
GLEWAPI void glCopyColorSubTableEXT(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

GLEWAPI void glLockArraysEXT(GLint first, GLsizei count);
GLEWAPI void glUnlockArraysEXT(void);

GLEWAPI void glConvolutionFilter1DEXT(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* image);
GLEWAPI void glConvolutionFilter2DEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* image);
GLEWAPI void glConvolutionParameterfEXT(GLenum target, GLenum pname, GLfloat param);
GLEWAPI void glConvolutionParameterfvEXT(GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glConvolutionParameteriEXT(GLenum target, GLenum pname, GLint param);
GLEWAPI void glConvolutionParameterivEXT(GLenum target, GLenum pname, const GLint* params);
GLEWAPI void glCopyConvolutionFilter1DEXT(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyConvolutionFilter2DEXT(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glGetConvolutionFilterEXT(GLenum target, GLenum format, GLenum type, void* image);
GLEWAPI void glGetConvolutionParameterfvEXT(GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetConvolutionParameterivEXT(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetSeparableFilterEXT(GLenum target, GLenum format, GLenum type, void* row, void* column, void* span);
GLEWAPI void glSeparableFilter2DEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* row, const void* column);

GLEWAPI void glBinormalPointerEXT(GLenum type, GLsizei stride, void* pointer);
GLEWAPI void glTangentPointerEXT(GLenum type, GLsizei stride, void* pointer);

GLEWAPI void glCopyTexImage1DEXT(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
GLEWAPI void glCopyTexImage2DEXT(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLEWAPI void glCopyTexSubImage1DEXT(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyTexSubImage2DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glCopyTexSubImage3DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

GLEWAPI void glCullParameterdvEXT(GLenum pname, GLdouble* params);
GLEWAPI void glCullParameterfvEXT(GLenum pname, GLfloat* params);

GLEWAPI void glInsertEventMarkerEXT(GLsizei length, const GLchar* marker);
GLEWAPI void glPopGroupMarkerEXT(void);
GLEWAPI void glPushGroupMarkerEXT(GLsizei length, const GLchar* marker);

GLEWAPI void glDepthBoundsEXT(GLclampd zmin, GLclampd zmax);

GLEWAPI void glBindMultiTextureEXT(GLenum texunit, GLenum target, GLuint texture);
GLEWAPI GLenum glCheckNamedFramebufferStatusEXT(GLuint framebuffer, GLenum target);
GLEWAPI void glClientAttribDefaultEXT(GLbitfield mask);
GLEWAPI void glCompressedMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedMultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCompressedTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
GLEWAPI void glCopyMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
GLEWAPI void glCopyMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLEWAPI void glCopyMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glCopyMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glCopyTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
GLEWAPI void glCopyTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLEWAPI void glCopyTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLEWAPI void glCopyTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glCopyTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLEWAPI void glDisableClientStateIndexedEXT(GLenum array, GLuint index);
GLEWAPI void glDisableClientStateiEXT(GLenum array, GLuint index);
GLEWAPI void glDisableVertexArrayAttribEXT(GLuint vaobj, GLuint index);
GLEWAPI void glDisableVertexArrayEXT(GLuint vaobj, GLenum array);
GLEWAPI void glEnableClientStateIndexedEXT(GLenum array, GLuint index);
GLEWAPI void glEnableClientStateiEXT(GLenum array, GLuint index);
GLEWAPI void glEnableVertexArrayAttribEXT(GLuint vaobj, GLuint index);
GLEWAPI void glEnableVertexArrayEXT(GLuint vaobj, GLenum array);
GLEWAPI void glFlushMappedNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length);
GLEWAPI void glFramebufferDrawBufferEXT(GLuint framebuffer, GLenum mode);
GLEWAPI void glFramebufferDrawBuffersEXT(GLuint framebuffer, GLsizei n, const GLenum* bufs);
GLEWAPI void glFramebufferReadBufferEXT(GLuint framebuffer, GLenum mode);
GLEWAPI void glGenerateMultiTexMipmapEXT(GLenum texunit, GLenum target);
GLEWAPI void glGenerateTextureMipmapEXT(GLuint texture, GLenum target);
GLEWAPI void glGetCompressedMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, void* img);
GLEWAPI void glGetCompressedTextureImageEXT(GLuint texture, GLenum target, GLint level, void* img);
GLEWAPI void glGetDoubleIndexedvEXT(GLenum target, GLuint index, GLdouble* params);
GLEWAPI void glGetDoublei_vEXT(GLenum pname, GLuint index, GLdouble* params);
GLEWAPI void glGetFloatIndexedvEXT(GLenum target, GLuint index, GLfloat* params);
GLEWAPI void glGetFloati_vEXT(GLenum pname, GLuint index, GLfloat* params);
GLEWAPI void glGetFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint* param);
GLEWAPI void glGetMultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetMultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetMultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble* params);
GLEWAPI void glGetMultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat* params);
GLEWAPI void glGetMultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, GLint* params);
GLEWAPI void glGetMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
GLEWAPI void glGetMultiTexLevelParameterfvEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
GLEWAPI void glGetMultiTexLevelParameterivEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
GLEWAPI void glGetMultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetMultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, GLuint* params);
GLEWAPI void glGetMultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetMultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetNamedBufferParameterivEXT(GLuint buffer, GLenum pname, GLint* params);
GLEWAPI void glGetNamedBufferPointervEXT(GLuint buffer, GLenum pname, void** params);
GLEWAPI void glGetNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
GLEWAPI void glGetNamedFramebufferAttachmentParameterivEXT(GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
GLEWAPI void glGetNamedProgramLocalParameterIivEXT(GLuint program, GLenum target, GLuint index, GLint* params);
GLEWAPI void glGetNamedProgramLocalParameterIuivEXT(GLuint program, GLenum target, GLuint index, GLuint* params);
GLEWAPI void glGetNamedProgramLocalParameterdvEXT(GLuint program, GLenum target, GLuint index, GLdouble* params);
GLEWAPI void glGetNamedProgramLocalParameterfvEXT(GLuint program, GLenum target, GLuint index, GLfloat* params);
GLEWAPI void glGetNamedProgramStringEXT(GLuint program, GLenum target, GLenum pname, void* string);
GLEWAPI void glGetNamedProgramivEXT(GLuint program, GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetNamedRenderbufferParameterivEXT(GLuint renderbuffer, GLenum pname, GLint* params);
GLEWAPI void glGetPointerIndexedvEXT(GLenum target, GLuint index, GLvoid** params);
GLEWAPI void glGetPointeri_vEXT(GLenum pname, GLuint index, GLvoid** params);
GLEWAPI void glGetTextureImageEXT(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
GLEWAPI void glGetTextureLevelParameterfvEXT(GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat* params);
GLEWAPI void glGetTextureLevelParameterivEXT(GLuint texture, GLenum target, GLint level, GLenum pname, GLint* params);
GLEWAPI void glGetTextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetTextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, GLuint* params);
GLEWAPI void glGetTextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetTextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetVertexArrayIntegeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLint* param);
GLEWAPI void glGetVertexArrayIntegervEXT(GLuint vaobj, GLenum pname, GLint* param);
GLEWAPI void glGetVertexArrayPointeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLvoid** param);
GLEWAPI void glGetVertexArrayPointervEXT(GLuint vaobj, GLenum pname, GLvoid** param);
GLEWAPI GLvoid * glMapNamedBufferEXT(GLuint buffer, GLenum access);
GLEWAPI GLvoid * glMapNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLEWAPI void glMatrixFrustumEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
GLEWAPI void glMatrixLoadIdentityEXT(GLenum matrixMode);
GLEWAPI void glMatrixLoadTransposedEXT(GLenum matrixMode, const GLdouble* m);
GLEWAPI void glMatrixLoadTransposefEXT(GLenum matrixMode, const GLfloat* m);
GLEWAPI void glMatrixLoaddEXT(GLenum matrixMode, const GLdouble* m);
GLEWAPI void glMatrixLoadfEXT(GLenum matrixMode, const GLfloat* m);
GLEWAPI void glMatrixMultTransposedEXT(GLenum matrixMode, const GLdouble* m);
GLEWAPI void glMatrixMultTransposefEXT(GLenum matrixMode, const GLfloat* m);
GLEWAPI void glMatrixMultdEXT(GLenum matrixMode, const GLdouble* m);
GLEWAPI void glMatrixMultfEXT(GLenum matrixMode, const GLfloat* m);
GLEWAPI void glMatrixOrthoEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
GLEWAPI void glMatrixPopEXT(GLenum matrixMode);
GLEWAPI void glMatrixPushEXT(GLenum matrixMode);
GLEWAPI void glMatrixRotatedEXT(GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glMatrixRotatefEXT(GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glMatrixScaledEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glMatrixScalefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glMatrixTranslatedEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glMatrixTranslatefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glMultiTexBufferEXT(GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
GLEWAPI void glMultiTexCoordPointerEXT(GLenum texunit, GLint size, GLenum type, GLsizei stride, const void* pointer);
GLEWAPI void glMultiTexEnvfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param);
GLEWAPI void glMultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glMultiTexEnviEXT(GLenum texunit, GLenum target, GLenum pname, GLint param);
GLEWAPI void glMultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint* params);
GLEWAPI void glMultiTexGendEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
GLEWAPI void glMultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLdouble* params);
GLEWAPI void glMultiTexGenfEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
GLEWAPI void glMultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLfloat* params);
GLEWAPI void glMultiTexGeniEXT(GLenum texunit, GLenum coord, GLenum pname, GLint param);
GLEWAPI void glMultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, const GLint* params);
GLEWAPI void glMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glMultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glMultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint* params);
GLEWAPI void glMultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
GLEWAPI void glMultiTexParameterfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param);
GLEWAPI void glMultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
GLEWAPI void glMultiTexParameteriEXT(GLenum texunit, GLenum target, GLenum pname, GLint param);
GLEWAPI void glMultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint* param);
GLEWAPI void glMultiTexRenderbufferEXT(GLenum texunit, GLenum target, GLuint renderbuffer);
GLEWAPI void glMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
GLEWAPI void glNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
GLEWAPI void glNamedCopyBufferSubDataEXT(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
GLEWAPI void glNamedFramebufferRenderbufferEXT(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLEWAPI void glNamedFramebufferTexture1DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLEWAPI void glNamedFramebufferTexture2DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLEWAPI void glNamedFramebufferTexture3DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
GLEWAPI void glNamedFramebufferTextureEXT(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
GLEWAPI void glNamedFramebufferTextureFaceEXT(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
GLEWAPI void glNamedFramebufferTextureLayerEXT(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLEWAPI void glNamedProgramLocalParameter4dEXT(GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glNamedProgramLocalParameter4dvEXT(GLuint program, GLenum target, GLuint index, const GLdouble* params);
GLEWAPI void glNamedProgramLocalParameter4fEXT(GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glNamedProgramLocalParameter4fvEXT(GLuint program, GLenum target, GLuint index, const GLfloat* params);
GLEWAPI void glNamedProgramLocalParameterI4iEXT(GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glNamedProgramLocalParameterI4ivEXT(GLuint program, GLenum target, GLuint index, const GLint* params);
GLEWAPI void glNamedProgramLocalParameterI4uiEXT(GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLEWAPI void glNamedProgramLocalParameterI4uivEXT(GLuint program, GLenum target, GLuint index, const GLuint* params);
GLEWAPI void glNamedProgramLocalParameters4fvEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
GLEWAPI void glNamedProgramLocalParametersI4ivEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLint* params);
GLEWAPI void glNamedProgramLocalParametersI4uivEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint* params);
GLEWAPI void glNamedProgramStringEXT(GLuint program, GLenum target, GLenum format, GLsizei len, const void* string);
GLEWAPI void glNamedRenderbufferStorageEXT(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
GLEWAPI void glNamedRenderbufferStorageMultisampleCoverageEXT(GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
GLEWAPI void glNamedRenderbufferStorageMultisampleEXT(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLEWAPI void glProgramUniform1dEXT(GLuint program, GLint location, GLdouble x);
GLEWAPI void glProgramUniform1dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLEWAPI void glProgramUniform1fEXT(GLuint program, GLint location, GLfloat v0);
GLEWAPI void glProgramUniform1fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform1iEXT(GLuint program, GLint location, GLint v0);
GLEWAPI void glProgramUniform1ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform1uiEXT(GLuint program, GLint location, GLuint v0);
GLEWAPI void glProgramUniform1uivEXT(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniform2dEXT(GLuint program, GLint location, GLdouble x, GLdouble y);
GLEWAPI void glProgramUniform2dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLEWAPI void glProgramUniform2fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1);
GLEWAPI void glProgramUniform2fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform2iEXT(GLuint program, GLint location, GLint v0, GLint v1);
GLEWAPI void glProgramUniform2ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform2uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1);
GLEWAPI void glProgramUniform2uivEXT(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniform3dEXT(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glProgramUniform3dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLEWAPI void glProgramUniform3fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLEWAPI void glProgramUniform3fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform3iEXT(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
GLEWAPI void glProgramUniform3ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform3uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
GLEWAPI void glProgramUniform3uivEXT(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniform4dEXT(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glProgramUniform4dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble *value);
GLEWAPI void glProgramUniform4fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLEWAPI void glProgramUniform4fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value);
GLEWAPI void glProgramUniform4iEXT(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLEWAPI void glProgramUniform4ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value);
GLEWAPI void glProgramUniform4uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLEWAPI void glProgramUniform4uivEXT(GLuint program, GLint location, GLsizei count, const GLuint* value);
GLEWAPI void glProgramUniformMatrix2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix2x3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix2x3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix2x4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix2x4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix3x2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix3x2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix3x4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix3x4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix4x2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix4x2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glProgramUniformMatrix4x3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
GLEWAPI void glProgramUniformMatrix4x3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLEWAPI void glPushClientAttribDefaultEXT(GLbitfield mask);
GLEWAPI void glTextureBufferEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
GLEWAPI void glTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, const GLint* params);
GLEWAPI void glTextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, const GLuint* params);
GLEWAPI void glTextureParameterfEXT(GLuint texture, GLenum target, GLenum pname, GLfloat param);
GLEWAPI void glTextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, const GLfloat* param);
GLEWAPI void glTextureParameteriEXT(GLuint texture, GLenum target, GLenum pname, GLint param);
GLEWAPI void glTextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, const GLint* param);
GLEWAPI void glTextureRenderbufferEXT(GLuint texture, GLenum target, GLuint renderbuffer);
GLEWAPI void glTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
GLEWAPI GLboolean glUnmapNamedBufferEXT(GLuint buffer);
GLEWAPI void glVertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayEdgeFlagOffsetEXT(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayVertexAttribIOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayVertexAttribOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);

GLEWAPI void glColorMaskIndexedEXT(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLEWAPI void glDisableIndexedEXT(GLenum target, GLuint index);
GLEWAPI void glEnableIndexedEXT(GLenum target, GLuint index);
GLEWAPI void glGetBooleanIndexedvEXT(GLenum value, GLuint index, GLboolean* data);
GLEWAPI void glGetIntegerIndexedvEXT(GLenum value, GLuint index, GLint* data);
GLEWAPI GLboolean glIsEnabledIndexedEXT(GLenum target, GLuint index);

GLEWAPI void glDrawArraysInstancedEXT(GLenum mode, GLint start, GLsizei count, GLsizei primcount);
GLEWAPI void glDrawElementsInstancedEXT(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);

GLEWAPI void glDrawRangeElementsEXT(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

GLEWAPI void glFogCoordPointerEXT(GLenum type, GLsizei stride, const GLvoid *pointer);
GLEWAPI void glFogCoorddEXT(GLdouble coord);
GLEWAPI void glFogCoorddvEXT(const GLdouble *coord);
GLEWAPI void glFogCoordfEXT(GLfloat coord);
GLEWAPI void glFogCoordfvEXT(const GLfloat *coord);

GLEWAPI void glFragmentColorMaterialEXT(GLenum face, GLenum mode);
GLEWAPI void glFragmentLightModelfEXT(GLenum pname, GLfloat param);
GLEWAPI void glFragmentLightModelfvEXT(GLenum pname, GLfloat* params);
GLEWAPI void glFragmentLightModeliEXT(GLenum pname, GLint param);
GLEWAPI void glFragmentLightModelivEXT(GLenum pname, GLint* params);
GLEWAPI void glFragmentLightfEXT(GLenum light, GLenum pname, GLfloat param);
GLEWAPI void glFragmentLightfvEXT(GLenum light, GLenum pname, GLfloat* params);
GLEWAPI void glFragmentLightiEXT(GLenum light, GLenum pname, GLint param);
GLEWAPI void glFragmentLightivEXT(GLenum light, GLenum pname, GLint* params);
GLEWAPI void glFragmentMaterialfEXT(GLenum face, GLenum pname, const GLfloat param);
GLEWAPI void glFragmentMaterialfvEXT(GLenum face, GLenum pname, const GLfloat* params);
GLEWAPI void glFragmentMaterialiEXT(GLenum face, GLenum pname, const GLint param);
GLEWAPI void glFragmentMaterialivEXT(GLenum face, GLenum pname, const GLint* params);
GLEWAPI void glGetFragmentLightfvEXT(GLenum light, GLenum pname, GLfloat* params);
GLEWAPI void glGetFragmentLightivEXT(GLenum light, GLenum pname, GLint* params);
GLEWAPI void glGetFragmentMaterialfvEXT(GLenum face, GLenum pname, const GLfloat* params);
GLEWAPI void glGetFragmentMaterialivEXT(GLenum face, GLenum pname, const GLint* params);
GLEWAPI void glLightEnviEXT(GLenum pname, GLint param);

GLEWAPI void glBlitFramebufferEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

GLEWAPI void glRenderbufferStorageMultisampleEXT(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

GLEWAPI void glBindFramebufferEXT(GLenum target, GLuint framebuffer);
GLEWAPI void glBindRenderbufferEXT(GLenum target, GLuint renderbuffer);
GLEWAPI GLenum glCheckFramebufferStatusEXT(GLenum target);
GLEWAPI void glDeleteFramebuffersEXT(GLsizei n, const GLuint* framebuffers);
GLEWAPI void glDeleteRenderbuffersEXT(GLsizei n, const GLuint* renderbuffers);
GLEWAPI void glFramebufferRenderbufferEXT(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLEWAPI void glFramebufferTexture1DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLEWAPI void glFramebufferTexture2DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLEWAPI void glFramebufferTexture3DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
GLEWAPI void glGenFramebuffersEXT(GLsizei n, GLuint* framebuffers);
GLEWAPI void glGenRenderbuffersEXT(GLsizei n, GLuint* renderbuffers);
GLEWAPI void glGenerateMipmapEXT(GLenum target);
GLEWAPI void glGetFramebufferAttachmentParameterivEXT(GLenum target, GLenum attachment, GLenum pname, GLint* params);
GLEWAPI void glGetRenderbufferParameterivEXT(GLenum target, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsFramebufferEXT(GLuint framebuffer);
GLEWAPI GLboolean glIsRenderbufferEXT(GLuint renderbuffer);
GLEWAPI void glRenderbufferStorageEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

GLEWAPI void glFramebufferTextureEXT(GLenum target, GLenum attachment, GLuint texture, GLint level);
GLEWAPI void glFramebufferTextureFaceEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
GLEWAPI void glProgramParameteriEXT(GLuint program, GLenum pname, GLint value);

GLEWAPI void glProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat* params);
GLEWAPI void glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat* params);

GLEWAPI void glBindFragDataLocationEXT(GLuint program, GLuint color, const GLchar *name);
GLEWAPI GLint glGetFragDataLocationEXT(GLuint program, const GLchar *name);
GLEWAPI void glGetUniformuivEXT(GLuint program, GLint location, GLuint *params);
GLEWAPI void glGetVertexAttribIivEXT(GLuint index, GLenum pname, GLint *params);
GLEWAPI void glGetVertexAttribIuivEXT(GLuint index, GLenum pname, GLuint *params);
GLEWAPI void glUniform1uiEXT(GLint location, GLuint v0);
GLEWAPI void glUniform1uivEXT(GLint location, GLsizei count, const GLuint *value);
GLEWAPI void glUniform2uiEXT(GLint location, GLuint v0, GLuint v1);
GLEWAPI void glUniform2uivEXT(GLint location, GLsizei count, const GLuint *value);
GLEWAPI void glUniform3uiEXT(GLint location, GLuint v0, GLuint v1, GLuint v2);
GLEWAPI void glUniform3uivEXT(GLint location, GLsizei count, const GLuint *value);
GLEWAPI void glUniform4uiEXT(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLEWAPI void glUniform4uivEXT(GLint location, GLsizei count, const GLuint *value);
GLEWAPI void glVertexAttribI1iEXT(GLuint index, GLint x);
GLEWAPI void glVertexAttribI1ivEXT(GLuint index, const GLint *v);
GLEWAPI void glVertexAttribI1uiEXT(GLuint index, GLuint x);
GLEWAPI void glVertexAttribI1uivEXT(GLuint index, const GLuint *v);
GLEWAPI void glVertexAttribI2iEXT(GLuint index, GLint x, GLint y);
GLEWAPI void glVertexAttribI2ivEXT(GLuint index, const GLint *v);
GLEWAPI void glVertexAttribI2uiEXT(GLuint index, GLuint x, GLuint y);
GLEWAPI void glVertexAttribI2uivEXT(GLuint index, const GLuint *v);
GLEWAPI void glVertexAttribI3iEXT(GLuint index, GLint x, GLint y, GLint z);
GLEWAPI void glVertexAttribI3ivEXT(GLuint index, const GLint *v);
GLEWAPI void glVertexAttribI3uiEXT(GLuint index, GLuint x, GLuint y, GLuint z);
GLEWAPI void glVertexAttribI3uivEXT(GLuint index, const GLuint *v);
GLEWAPI void glVertexAttribI4bvEXT(GLuint index, const GLbyte *v);
GLEWAPI void glVertexAttribI4iEXT(GLuint index, GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glVertexAttribI4ivEXT(GLuint index, const GLint *v);
GLEWAPI void glVertexAttribI4svEXT(GLuint index, const GLshort *v);
GLEWAPI void glVertexAttribI4ubvEXT(GLuint index, const GLubyte *v);
GLEWAPI void glVertexAttribI4uiEXT(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLEWAPI void glVertexAttribI4uivEXT(GLuint index, const GLuint *v);
GLEWAPI void glVertexAttribI4usvEXT(GLuint index, const GLushort *v);
GLEWAPI void glVertexAttribIPointerEXT(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

GLEWAPI void glGetHistogramEXT(GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
GLEWAPI void glGetHistogramParameterfvEXT(GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetHistogramParameterivEXT(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetMinmaxEXT(GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
GLEWAPI void glGetMinmaxParameterfvEXT(GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetMinmaxParameterivEXT(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glHistogramEXT(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
GLEWAPI void glMinmaxEXT(GLenum target, GLenum internalformat, GLboolean sink);
GLEWAPI void glResetHistogramEXT(GLenum target);
GLEWAPI void glResetMinmaxEXT(GLenum target);

GLEWAPI void glIndexFuncEXT(GLenum func, GLfloat ref);

GLEWAPI void glIndexMaterialEXT(GLenum face, GLenum mode);

GLEWAPI void glApplyTextureEXT(GLenum mode);
GLEWAPI void glTextureLightEXT(GLenum pname);
GLEWAPI void glTextureMaterialEXT(GLenum face, GLenum mode);

GLEWAPI void glMultiDrawArraysEXT(GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
GLEWAPI void glMultiDrawElementsEXT(GLenum mode, GLsizei* count, GLenum type, const GLvoid **indices, GLsizei primcount);

GLEWAPI void glSampleMaskEXT(GLclampf value, GLboolean invert);
GLEWAPI void glSamplePatternEXT(GLenum pattern);

GLEWAPI void glColorTableEXT(GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void* data);
GLEWAPI void glGetColorTableEXT(GLenum target, GLenum format, GLenum type, void* data);
GLEWAPI void glGetColorTableParameterfvEXT(GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetColorTableParameterivEXT(GLenum target, GLenum pname, GLint* params);

GLEWAPI void glGetPixelTransformParameterfvEXT(GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glGetPixelTransformParameterivEXT(GLenum target, GLenum pname, const GLint* params);
GLEWAPI void glPixelTransformParameterfEXT(GLenum target, GLenum pname, const GLfloat param);
GLEWAPI void glPixelTransformParameterfvEXT(GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glPixelTransformParameteriEXT(GLenum target, GLenum pname, const GLint param);
GLEWAPI void glPixelTransformParameterivEXT(GLenum target, GLenum pname, const GLint* params);

GLEWAPI void glPointParameterfEXT(GLenum pname, GLfloat param);
GLEWAPI void glPointParameterfvEXT(GLenum pname, const GLfloat* params);

GLEWAPI void glPolygonOffsetEXT(GLfloat factor, GLfloat bias);

GLEWAPI void glProvokingVertexEXT(GLenum mode);

GLEWAPI void glBeginSceneEXT(void);
GLEWAPI void glEndSceneEXT(void);

GLEWAPI void glSecondaryColor3bEXT(GLbyte red, GLbyte green, GLbyte blue);
GLEWAPI void glSecondaryColor3bvEXT(const GLbyte *v);
GLEWAPI void glSecondaryColor3dEXT(GLdouble red, GLdouble green, GLdouble blue);
GLEWAPI void glSecondaryColor3dvEXT(const GLdouble *v);
GLEWAPI void glSecondaryColor3fEXT(GLfloat red, GLfloat green, GLfloat blue);
GLEWAPI void glSecondaryColor3fvEXT(const GLfloat *v);
GLEWAPI void glSecondaryColor3iEXT(GLint red, GLint green, GLint blue);
GLEWAPI void glSecondaryColor3ivEXT(const GLint *v);
GLEWAPI void glSecondaryColor3sEXT(GLshort red, GLshort green, GLshort blue);
GLEWAPI void glSecondaryColor3svEXT(const GLshort *v);
GLEWAPI void glSecondaryColor3ubEXT(GLubyte red, GLubyte green, GLubyte blue);
GLEWAPI void glSecondaryColor3ubvEXT(const GLubyte *v);
GLEWAPI void glSecondaryColor3uiEXT(GLuint red, GLuint green, GLuint blue);
GLEWAPI void glSecondaryColor3uivEXT(const GLuint *v);
GLEWAPI void glSecondaryColor3usEXT(GLushort red, GLushort green, GLushort blue);
GLEWAPI void glSecondaryColor3usvEXT(const GLushort *v);
GLEWAPI void glSecondaryColorPointerEXT(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

GLEWAPI void glActiveProgramEXT(GLuint program);
GLEWAPI GLuint glCreateShaderProgramEXT(GLenum type, const GLchar* string);
GLEWAPI void glUseShaderProgramEXT(GLenum type, GLuint program);

GLEWAPI void glBindImageTextureEXT(GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
GLEWAPI void glMemoryBarrierEXT(GLbitfield barriers);

GLEWAPI void glActiveStencilFaceEXT(GLenum face);

GLEWAPI void glTexSubImage1DEXT(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTexSubImage2DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTexSubImage3DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);

GLEWAPI void glTexImage3DEXT(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);

GLEWAPI void glFramebufferTextureLayerEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

GLEWAPI void glTexBufferEXT(GLenum target, GLenum internalformat, GLuint buffer);

GLEWAPI void glClearColorIiEXT(GLint red, GLint green, GLint blue, GLint alpha);
GLEWAPI void glClearColorIuiEXT(GLuint red, GLuint green, GLuint blue, GLuint alpha);
GLEWAPI void glGetTexParameterIivEXT(GLenum target, GLenum pname, GLint *params);
GLEWAPI void glGetTexParameterIuivEXT(GLenum target, GLenum pname, GLuint *params);
GLEWAPI void glTexParameterIivEXT(GLenum target, GLenum pname, const GLint *params);
GLEWAPI void glTexParameterIuivEXT(GLenum target, GLenum pname, const GLuint *params);

GLEWAPI GLboolean glAreTexturesResidentEXT(GLsizei n, const GLuint* textures, GLboolean* residences);
GLEWAPI void glBindTextureEXT(GLenum target, GLuint texture);
GLEWAPI void glDeleteTexturesEXT(GLsizei n, const GLuint* textures);
GLEWAPI void glGenTexturesEXT(GLsizei n, GLuint* textures);
GLEWAPI GLboolean glIsTextureEXT(GLuint texture);
GLEWAPI void glPrioritizeTexturesEXT(GLsizei n, const GLuint* textures, const GLclampf* priorities);

GLEWAPI void glTextureNormalEXT(GLenum mode);

GLEWAPI void glGetQueryObjecti64vEXT(GLuint id, GLenum pname, GLint64EXT *params);
GLEWAPI void glGetQueryObjectui64vEXT(GLuint id, GLenum pname, GLuint64EXT *params);

GLEWAPI void glBeginTransformFeedbackEXT(GLenum primitiveMode);
GLEWAPI void glBindBufferBaseEXT(GLenum target, GLuint index, GLuint buffer);
GLEWAPI void glBindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset);
GLEWAPI void glBindBufferRangeEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLEWAPI void glEndTransformFeedbackEXT(void);
GLEWAPI void glGetTransformFeedbackVaryingEXT(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei *size, GLenum *type, GLchar *name);
GLEWAPI void glTransformFeedbackVaryingsEXT(GLuint program, GLsizei count, const GLchar ** varyings, GLenum bufferMode);

GLEWAPI void glArrayElementEXT(GLint i);
GLEWAPI void glColorPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
GLEWAPI void glDrawArraysEXT(GLenum mode, GLint first, GLsizei count);
GLEWAPI void glEdgeFlagPointerEXT(GLsizei stride, GLsizei count, const GLboolean* pointer);
GLEWAPI void glIndexPointerEXT(GLenum type, GLsizei stride, GLsizei count, const void* pointer);
GLEWAPI void glNormalPointerEXT(GLenum type, GLsizei stride, GLsizei count, const void* pointer);
GLEWAPI void glTexCoordPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
GLEWAPI void glVertexPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);

GLEWAPI void glGetVertexAttribLdvEXT(GLuint index, GLenum pname, GLdouble* params);
GLEWAPI void glVertexArrayVertexAttribLOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
GLEWAPI void glVertexAttribL1dEXT(GLuint index, GLdouble x);
GLEWAPI void glVertexAttribL1dvEXT(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribL2dEXT(GLuint index, GLdouble x, GLdouble y);
GLEWAPI void glVertexAttribL2dvEXT(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribL3dEXT(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glVertexAttribL3dvEXT(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribL4dEXT(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glVertexAttribL4dvEXT(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttribLPointerEXT(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

GLEWAPI void glBeginVertexShaderEXT(void);
GLEWAPI GLuint glBindLightParameterEXT(GLenum light, GLenum value);
GLEWAPI GLuint glBindMaterialParameterEXT(GLenum face, GLenum value);
GLEWAPI GLuint glBindParameterEXT(GLenum value);
GLEWAPI GLuint glBindTexGenParameterEXT(GLenum unit, GLenum coord, GLenum value);
GLEWAPI GLuint glBindTextureUnitParameterEXT(GLenum unit, GLenum value);
GLEWAPI void glBindVertexShaderEXT(GLuint id);
GLEWAPI void glDeleteVertexShaderEXT(GLuint id);
GLEWAPI void glDisableVariantClientStateEXT(GLuint id);
GLEWAPI void glEnableVariantClientStateEXT(GLuint id);
GLEWAPI void glEndVertexShaderEXT(void);
GLEWAPI void glExtractComponentEXT(GLuint res, GLuint src, GLuint num);
GLEWAPI GLuint glGenSymbolsEXT(GLenum dataType, GLenum storageType, GLenum range, GLuint components);
GLEWAPI GLuint glGenVertexShadersEXT(GLuint range);
GLEWAPI void glGetInvariantBooleanvEXT(GLuint id, GLenum value, GLboolean *data);
GLEWAPI void glGetInvariantFloatvEXT(GLuint id, GLenum value, GLfloat *data);
GLEWAPI void glGetInvariantIntegervEXT(GLuint id, GLenum value, GLint *data);
GLEWAPI void glGetLocalConstantBooleanvEXT(GLuint id, GLenum value, GLboolean *data);
GLEWAPI void glGetLocalConstantFloatvEXT(GLuint id, GLenum value, GLfloat *data);
GLEWAPI void glGetLocalConstantIntegervEXT(GLuint id, GLenum value, GLint *data);
GLEWAPI void glGetVariantBooleanvEXT(GLuint id, GLenum value, GLboolean *data);
GLEWAPI void glGetVariantFloatvEXT(GLuint id, GLenum value, GLfloat *data);
GLEWAPI void glGetVariantIntegervEXT(GLuint id, GLenum value, GLint *data);
GLEWAPI void glGetVariantPointervEXT(GLuint id, GLenum value, GLvoid **data);
GLEWAPI void glInsertComponentEXT(GLuint res, GLuint src, GLuint num);
GLEWAPI GLboolean glIsVariantEnabledEXT(GLuint id, GLenum cap);
GLEWAPI void glSetInvariantEXT(GLuint id, GLenum type, GLvoid *addr);
GLEWAPI void glSetLocalConstantEXT(GLuint id, GLenum type, GLvoid *addr);
GLEWAPI void glShaderOp1EXT(GLenum op, GLuint res, GLuint arg1);
GLEWAPI void glShaderOp2EXT(GLenum op, GLuint res, GLuint arg1, GLuint arg2);
GLEWAPI void glShaderOp3EXT(GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
GLEWAPI void glSwizzleEXT(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
GLEWAPI void glVariantPointerEXT(GLuint id, GLenum type, GLuint stride, GLvoid *addr);
GLEWAPI void glVariantbvEXT(GLuint id, GLbyte *addr);
GLEWAPI void glVariantdvEXT(GLuint id, GLdouble *addr);
GLEWAPI void glVariantfvEXT(GLuint id, GLfloat *addr);
GLEWAPI void glVariantivEXT(GLuint id, GLint *addr);
GLEWAPI void glVariantsvEXT(GLuint id, GLshort *addr);
GLEWAPI void glVariantubvEXT(GLuint id, GLubyte *addr);
GLEWAPI void glVariantuivEXT(GLuint id, GLuint *addr);
GLEWAPI void glVariantusvEXT(GLuint id, GLushort *addr);
GLEWAPI void glWriteMaskEXT(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);

GLEWAPI void glVertexWeightPointerEXT(GLint size, GLenum type, GLsizei stride, void* pointer);
GLEWAPI void glVertexWeightfEXT(GLfloat weight);
GLEWAPI void glVertexWeightfvEXT(GLfloat* weight);

GLEWAPI GLsync glImportSyncEXT(GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);

GLEWAPI void glFrameTerminatorGREMEDY(void);

GLEWAPI void glStringMarkerGREMEDY(GLsizei len, const void* string);

GLEWAPI void glGetImageTransformParameterfvHP(GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glGetImageTransformParameterivHP(GLenum target, GLenum pname, const GLint* params);
GLEWAPI void glImageTransformParameterfHP(GLenum target, GLenum pname, const GLfloat param);
GLEWAPI void glImageTransformParameterfvHP(GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glImageTransformParameteriHP(GLenum target, GLenum pname, const GLint param);
GLEWAPI void glImageTransformParameterivHP(GLenum target, GLenum pname, const GLint* params);

GLEWAPI void glMultiModeDrawArraysIBM(const GLenum* mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
GLEWAPI void glMultiModeDrawElementsIBM(const GLenum* mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, GLint modestride);

GLEWAPI void glColorPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
GLEWAPI void glEdgeFlagPointerListIBM(GLint stride, const GLboolean ** pointer, GLint ptrstride);
GLEWAPI void glFogCoordPointerListIBM(GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
GLEWAPI void glIndexPointerListIBM(GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
GLEWAPI void glNormalPointerListIBM(GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
GLEWAPI void glSecondaryColorPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
GLEWAPI void glTexCoordPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
GLEWAPI void glVertexPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

GLEWAPI void glColorPointervINTEL(GLint size, GLenum type, const void** pointer);
GLEWAPI void glNormalPointervINTEL(GLenum type, const void** pointer);
GLEWAPI void glTexCoordPointervINTEL(GLint size, GLenum type, const void** pointer);
GLEWAPI void glVertexPointervINTEL(GLint size, GLenum type, const void** pointer);

GLEWAPI void glTexScissorFuncINTEL(GLenum target, GLenum lfunc, GLenum hfunc);
GLEWAPI void glTexScissorINTEL(GLenum target, GLclampf tlow, GLclampf thigh);

GLEWAPI void glDebugMessageCallback(GLDEBUGPROC callback, void* userParam);
GLEWAPI void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
GLEWAPI void glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
GLEWAPI GLuint glGetDebugMessageLog(GLuint count, GLsizei bufsize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
GLEWAPI void glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar *label);
GLEWAPI void glGetObjectPtrLabel(void* ptr, GLsizei bufSize, GLsizei* length, GLchar *label);
GLEWAPI void glGetPointerv(GLenum pname, void** params);
GLEWAPI void glObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
GLEWAPI void glObjectPtrLabel(void* ptr, GLsizei length, const GLchar* label);
GLEWAPI void glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar * message);

GLEWAPI GLuint glBufferRegionEnabled(void);
GLEWAPI void glDeleteBufferRegion(GLenum region);
GLEWAPI void glDrawBufferRegion(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
GLEWAPI GLuint glNewBufferRegion(GLenum region);
GLEWAPI void glReadBufferRegion(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);

GLEWAPI void glResizeBuffersMESA(void);

GLEWAPI void glWindowPos2dMESA(GLdouble x, GLdouble y);
GLEWAPI void glWindowPos2dvMESA(const GLdouble* p);
GLEWAPI void glWindowPos2fMESA(GLfloat x, GLfloat y);
GLEWAPI void glWindowPos2fvMESA(const GLfloat* p);
GLEWAPI void glWindowPos2iMESA(GLint x, GLint y);
GLEWAPI void glWindowPos2ivMESA(const GLint* p);
GLEWAPI void glWindowPos2sMESA(GLshort x, GLshort y);
GLEWAPI void glWindowPos2svMESA(const GLshort* p);
GLEWAPI void glWindowPos3dMESA(GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glWindowPos3dvMESA(const GLdouble* p);
GLEWAPI void glWindowPos3fMESA(GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glWindowPos3fvMESA(const GLfloat* p);
GLEWAPI void glWindowPos3iMESA(GLint x, GLint y, GLint z);
GLEWAPI void glWindowPos3ivMESA(const GLint* p);
GLEWAPI void glWindowPos3sMESA(GLshort x, GLshort y, GLshort z);
GLEWAPI void glWindowPos3svMESA(const GLshort* p);
GLEWAPI void glWindowPos4dMESA(GLdouble x, GLdouble y, GLdouble z, GLdouble);
GLEWAPI void glWindowPos4dvMESA(const GLdouble* p);
GLEWAPI void glWindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glWindowPos4fvMESA(const GLfloat* p);
GLEWAPI void glWindowPos4iMESA(GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glWindowPos4ivMESA(const GLint* p);
GLEWAPI void glWindowPos4sMESA(GLshort x, GLshort y, GLshort z, GLshort w);
GLEWAPI void glWindowPos4svMESA(const GLshort* p);

GLEWAPI GLuint64 glGetImageHandleNV(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
GLEWAPI GLuint64 glGetTextureHandleNV(GLuint texture);
GLEWAPI GLuint64 glGetTextureSamplerHandleNV(GLuint texture, GLuint sampler);
GLEWAPI GLboolean glIsImageHandleResidentNV(GLuint64 handle);
GLEWAPI GLboolean glIsTextureHandleResidentNV(GLuint64 handle);
GLEWAPI void glMakeImageHandleNonResidentNV(GLuint64 handle);
GLEWAPI void glMakeImageHandleResidentNV(GLuint64 handle, GLenum access);
GLEWAPI void glMakeTextureHandleNonResidentNV(GLuint64 handle);
GLEWAPI void glMakeTextureHandleResidentNV(GLuint64 handle);
GLEWAPI void glProgramUniformHandleui64NV(GLuint program, GLint location, GLuint64 value);
GLEWAPI void glProgramUniformHandleui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64* values);
GLEWAPI void glUniformHandleui64NV(GLint location, GLuint64 value);
GLEWAPI void glUniformHandleui64vNV(GLint location, GLsizei count, const GLuint64* value);

GLEWAPI void glBeginConditionalRenderNV(GLuint id, GLenum mode);
GLEWAPI void glEndConditionalRenderNV(void);

GLEWAPI void glCopyImageSubDataNV(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);

GLEWAPI void glClearDepthdNV(GLdouble depth);
GLEWAPI void glDepthBoundsdNV(GLdouble zmin, GLdouble zmax);
GLEWAPI void glDepthRangedNV(GLdouble zNear, GLdouble zFar);

GLEWAPI void glEvalMapsNV(GLenum target, GLenum mode);
GLEWAPI void glGetMapAttribParameterfvNV(GLenum target, GLuint index, GLenum pname, GLfloat* params);
GLEWAPI void glGetMapAttribParameterivNV(GLenum target, GLuint index, GLenum pname, GLint* params);
GLEWAPI void glGetMapControlPointsNV(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void* points);
GLEWAPI void glGetMapParameterfvNV(GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetMapParameterivNV(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glMapControlPointsNV(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void* points);
GLEWAPI void glMapParameterfvNV(GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glMapParameterivNV(GLenum target, GLenum pname, const GLint* params);

GLEWAPI void glGetMultisamplefvNV(GLenum pname, GLuint index, GLfloat* val);
GLEWAPI void glSampleMaskIndexedNV(GLuint index, GLbitfield mask);
GLEWAPI void glTexRenderbufferNV(GLenum target, GLuint renderbuffer);

GLEWAPI void glDeleteFencesNV(GLsizei n, const GLuint* fences);
GLEWAPI void glFinishFenceNV(GLuint fence);
GLEWAPI void glGenFencesNV(GLsizei n, GLuint* fences);
GLEWAPI void glGetFenceivNV(GLuint fence, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsFenceNV(GLuint fence);
GLEWAPI void glSetFenceNV(GLuint fence, GLenum condition);
GLEWAPI GLboolean glTestFenceNV(GLuint fence);

GLEWAPI void glGetProgramNamedParameterdvNV(GLuint id, GLsizei len, const GLubyte* name, GLdouble *params);
GLEWAPI void glGetProgramNamedParameterfvNV(GLuint id, GLsizei len, const GLubyte* name, GLfloat *params);
GLEWAPI void glProgramNamedParameter4dNV(GLuint id, GLsizei len, const GLubyte* name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glProgramNamedParameter4dvNV(GLuint id, GLsizei len, const GLubyte* name, const GLdouble v[]);
GLEWAPI void glProgramNamedParameter4fNV(GLuint id, GLsizei len, const GLubyte* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glProgramNamedParameter4fvNV(GLuint id, GLsizei len, const GLubyte* name, const GLfloat v[]);

GLEWAPI void glRenderbufferStorageMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);

GLEWAPI void glProgramVertexLimitNV(GLenum target, GLint limit);

GLEWAPI void glProgramEnvParameterI4iNV(GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glProgramEnvParameterI4ivNV(GLenum target, GLuint index, const GLint *params);
GLEWAPI void glProgramEnvParameterI4uiNV(GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLEWAPI void glProgramEnvParameterI4uivNV(GLenum target, GLuint index, const GLuint *params);
GLEWAPI void glProgramEnvParametersI4ivNV(GLenum target, GLuint index, GLsizei count, const GLint *params);
GLEWAPI void glProgramEnvParametersI4uivNV(GLenum target, GLuint index, GLsizei count, const GLuint *params);
GLEWAPI void glProgramLocalParameterI4iNV(GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
GLEWAPI void glProgramLocalParameterI4ivNV(GLenum target, GLuint index, const GLint *params);
GLEWAPI void glProgramLocalParameterI4uiNV(GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLEWAPI void glProgramLocalParameterI4uivNV(GLenum target, GLuint index, const GLuint *params);
GLEWAPI void glProgramLocalParametersI4ivNV(GLenum target, GLuint index, GLsizei count, const GLint *params);
GLEWAPI void glProgramLocalParametersI4uivNV(GLenum target, GLuint index, GLsizei count, const GLuint *params);

GLEWAPI void glGetUniformi64vNV(GLuint program, GLint location, GLint64EXT* params);
GLEWAPI void glGetUniformui64vNV(GLuint program, GLint location, GLuint64EXT* params);
GLEWAPI void glProgramUniform1i64NV(GLuint program, GLint location, GLint64EXT x);
GLEWAPI void glProgramUniform1i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glProgramUniform1ui64NV(GLuint program, GLint location, GLuint64EXT x);
GLEWAPI void glProgramUniform1ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glProgramUniform2i64NV(GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
GLEWAPI void glProgramUniform2i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glProgramUniform2ui64NV(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
GLEWAPI void glProgramUniform2ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glProgramUniform3i64NV(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLEWAPI void glProgramUniform3i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glProgramUniform3ui64NV(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLEWAPI void glProgramUniform3ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glProgramUniform4i64NV(GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLEWAPI void glProgramUniform4i64vNV(GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glProgramUniform4ui64NV(GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLEWAPI void glProgramUniform4ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glUniform1i64NV(GLint location, GLint64EXT x);
GLEWAPI void glUniform1i64vNV(GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glUniform1ui64NV(GLint location, GLuint64EXT x);
GLEWAPI void glUniform1ui64vNV(GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glUniform2i64NV(GLint location, GLint64EXT x, GLint64EXT y);
GLEWAPI void glUniform2i64vNV(GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glUniform2ui64NV(GLint location, GLuint64EXT x, GLuint64EXT y);
GLEWAPI void glUniform2ui64vNV(GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glUniform3i64NV(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLEWAPI void glUniform3i64vNV(GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glUniform3ui64NV(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLEWAPI void glUniform3ui64vNV(GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glUniform4i64NV(GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLEWAPI void glUniform4i64vNV(GLint location, GLsizei count, const GLint64EXT* value);
GLEWAPI void glUniform4ui64NV(GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLEWAPI void glUniform4ui64vNV(GLint location, GLsizei count, const GLuint64EXT* value);

GLEWAPI void glColor3hNV(GLhalf red, GLhalf green, GLhalf blue);
GLEWAPI void glColor3hvNV(const GLhalf* v);
GLEWAPI void glColor4hNV(GLhalf red, GLhalf green, GLhalf blue, GLhalf alpha);
GLEWAPI void glColor4hvNV(const GLhalf* v);
GLEWAPI void glFogCoordhNV(GLhalf fog);
GLEWAPI void glFogCoordhvNV(const GLhalf* fog);
GLEWAPI void glMultiTexCoord1hNV(GLenum target, GLhalf s);
GLEWAPI void glMultiTexCoord1hvNV(GLenum target, const GLhalf* v);
GLEWAPI void glMultiTexCoord2hNV(GLenum target, GLhalf s, GLhalf t);
GLEWAPI void glMultiTexCoord2hvNV(GLenum target, const GLhalf* v);
GLEWAPI void glMultiTexCoord3hNV(GLenum target, GLhalf s, GLhalf t, GLhalf r);
GLEWAPI void glMultiTexCoord3hvNV(GLenum target, const GLhalf* v);
GLEWAPI void glMultiTexCoord4hNV(GLenum target, GLhalf s, GLhalf t, GLhalf r, GLhalf q);
GLEWAPI void glMultiTexCoord4hvNV(GLenum target, const GLhalf* v);
GLEWAPI void glNormal3hNV(GLhalf nx, GLhalf ny, GLhalf nz);
GLEWAPI void glNormal3hvNV(const GLhalf* v);
GLEWAPI void glSecondaryColor3hNV(GLhalf red, GLhalf green, GLhalf blue);
GLEWAPI void glSecondaryColor3hvNV(const GLhalf* v);
GLEWAPI void glTexCoord1hNV(GLhalf s);
GLEWAPI void glTexCoord1hvNV(const GLhalf* v);
GLEWAPI void glTexCoord2hNV(GLhalf s, GLhalf t);
GLEWAPI void glTexCoord2hvNV(const GLhalf* v);
GLEWAPI void glTexCoord3hNV(GLhalf s, GLhalf t, GLhalf r);
GLEWAPI void glTexCoord3hvNV(const GLhalf* v);
GLEWAPI void glTexCoord4hNV(GLhalf s, GLhalf t, GLhalf r, GLhalf q);
GLEWAPI void glTexCoord4hvNV(const GLhalf* v);
GLEWAPI void glVertex2hNV(GLhalf x, GLhalf y);
GLEWAPI void glVertex2hvNV(const GLhalf* v);
GLEWAPI void glVertex3hNV(GLhalf x, GLhalf y, GLhalf z);
GLEWAPI void glVertex3hvNV(const GLhalf* v);
GLEWAPI void glVertex4hNV(GLhalf x, GLhalf y, GLhalf z, GLhalf w);
GLEWAPI void glVertex4hvNV(const GLhalf* v);
GLEWAPI void glVertexAttrib1hNV(GLuint index, GLhalf x);
GLEWAPI void glVertexAttrib1hvNV(GLuint index, const GLhalf* v);
GLEWAPI void glVertexAttrib2hNV(GLuint index, GLhalf x, GLhalf y);
GLEWAPI void glVertexAttrib2hvNV(GLuint index, const GLhalf* v);
GLEWAPI void glVertexAttrib3hNV(GLuint index, GLhalf x, GLhalf y, GLhalf z);
GLEWAPI void glVertexAttrib3hvNV(GLuint index, const GLhalf* v);
GLEWAPI void glVertexAttrib4hNV(GLuint index, GLhalf x, GLhalf y, GLhalf z, GLhalf w);
GLEWAPI void glVertexAttrib4hvNV(GLuint index, const GLhalf* v);
GLEWAPI void glVertexAttribs1hvNV(GLuint index, GLsizei n, const GLhalf* v);
GLEWAPI void glVertexAttribs2hvNV(GLuint index, GLsizei n, const GLhalf* v);
GLEWAPI void glVertexAttribs3hvNV(GLuint index, GLsizei n, const GLhalf* v);
GLEWAPI void glVertexAttribs4hvNV(GLuint index, GLsizei n, const GLhalf* v);
GLEWAPI void glVertexWeighthNV(GLhalf weight);
GLEWAPI void glVertexWeighthvNV(const GLhalf* weight);

GLEWAPI void glBeginOcclusionQueryNV(GLuint id);
GLEWAPI void glDeleteOcclusionQueriesNV(GLsizei n, const GLuint* ids);
GLEWAPI void glEndOcclusionQueryNV(void);
GLEWAPI void glGenOcclusionQueriesNV(GLsizei n, GLuint* ids);
GLEWAPI void glGetOcclusionQueryivNV(GLuint id, GLenum pname, GLint* params);
GLEWAPI void glGetOcclusionQueryuivNV(GLuint id, GLenum pname, GLuint* params);
GLEWAPI GLboolean glIsOcclusionQueryNV(GLuint id);

GLEWAPI void glProgramBufferParametersIivNV(GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLint *params);
GLEWAPI void glProgramBufferParametersIuivNV(GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLuint *params);
GLEWAPI void glProgramBufferParametersfvNV(GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLfloat *params);

GLEWAPI void glCopyPathNV(GLuint resultPath, GLuint srcPath);
GLEWAPI void glCoverFillPathInstancedNV(GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLEWAPI void glCoverFillPathNV(GLuint path, GLenum coverMode);
GLEWAPI void glCoverStrokePathInstancedNV(GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLEWAPI void glCoverStrokePathNV(GLuint name, GLenum coverMode);
GLEWAPI void glDeletePathsNV(GLuint path, GLsizei range);
GLEWAPI GLuint glGenPathsNV(GLsizei range);
GLEWAPI void glGetPathColorGenfvNV(GLenum color, GLenum pname, GLfloat* value);
GLEWAPI void glGetPathColorGenivNV(GLenum color, GLenum pname, GLint* value);
GLEWAPI void glGetPathCommandsNV(GLuint name, GLubyte* commands);
GLEWAPI void glGetPathCoordsNV(GLuint name, GLfloat* coords);
GLEWAPI void glGetPathDashArrayNV(GLuint name, GLfloat* dashArray);
GLEWAPI GLfloat glGetPathLengthNV(GLuint path, GLsizei startSegment, GLsizei numSegments);
GLEWAPI void glGetPathMetricRangeNV(GLbitfield metricQueryMask, GLuint fistPathName, GLsizei numPaths, GLsizei stride, GLfloat* metrics);
GLEWAPI void glGetPathMetricsNV(GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
GLEWAPI void glGetPathParameterfvNV(GLuint name, GLenum param, GLfloat* value);
GLEWAPI void glGetPathParameterivNV(GLuint name, GLenum param, GLint* value);
GLEWAPI void glGetPathSpacingNV(GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
GLEWAPI void glGetPathTexGenfvNV(GLenum texCoordSet, GLenum pname, GLfloat* value);
GLEWAPI void glGetPathTexGenivNV(GLenum texCoordSet, GLenum pname, GLint* value);
GLEWAPI void glInterpolatePathsNV(GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
GLEWAPI GLboolean glIsPathNV(GLuint path);
GLEWAPI GLboolean glIsPointInFillPathNV(GLuint path, GLuint mask, GLfloat x, GLfloat y);
GLEWAPI GLboolean glIsPointInStrokePathNV(GLuint path, GLfloat x, GLfloat y);
GLEWAPI void glPathColorGenNV(GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat* coeffs);
GLEWAPI void glPathCommandsNV(GLuint path, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
GLEWAPI void glPathCoordsNV(GLuint path, GLsizei numCoords, GLenum coordType, const void* coords);
GLEWAPI void glPathCoverDepthFuncNV(GLenum zfunc);
GLEWAPI void glPathDashArrayNV(GLuint path, GLsizei dashCount, const GLfloat* dashArray);
GLEWAPI void glPathFogGenNV(GLenum genMode);
GLEWAPI void glPathGlyphRangeNV(GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLEWAPI void glPathGlyphsNV(GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid*charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLEWAPI void glPathParameterfNV(GLuint path, GLenum pname, GLfloat value);
GLEWAPI void glPathParameterfvNV(GLuint path, GLenum pname, const GLfloat* value);
GLEWAPI void glPathParameteriNV(GLuint path, GLenum pname, GLint value);
GLEWAPI void glPathParameterivNV(GLuint path, GLenum pname, const GLint* value);
GLEWAPI void glPathStencilDepthOffsetNV(GLfloat factor, GLfloat units);
GLEWAPI void glPathStencilFuncNV(GLenum func, GLint ref, GLuint mask);
GLEWAPI void glPathStringNV(GLuint path, GLenum format, GLsizei length, const void* pathString);
GLEWAPI void glPathSubCommandsNV(GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
GLEWAPI void glPathSubCoordsNV(GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void* coords);
GLEWAPI void glPathTexGenNV(GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat* coeffs);
GLEWAPI GLboolean glPointAlongPathNV(GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat* x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);
GLEWAPI void glStencilFillPathInstancedNV(GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
GLEWAPI void glStencilFillPathNV(GLuint path, GLenum fillMode, GLuint mask);
GLEWAPI void glStencilStrokePathInstancedNV(GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
GLEWAPI void glStencilStrokePathNV(GLuint path, GLint reference, GLuint mask);
GLEWAPI void glTransformPathNV(GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat* transformValues);
GLEWAPI void glWeightPathsNV(GLuint resultPath, GLsizei numPaths, const GLuint paths[], const GLfloat weights[]);

GLEWAPI void glFlushPixelDataRangeNV(GLenum target);
GLEWAPI void glPixelDataRangeNV(GLenum target, GLsizei length, void* pointer);

GLEWAPI void glPointParameteriNV(GLenum pname, GLint param);
GLEWAPI void glPointParameterivNV(GLenum pname, const GLint* params);

GLEWAPI void glGetVideoi64vNV(GLuint video_slot, GLenum pname, GLint64EXT* params);
GLEWAPI void glGetVideoivNV(GLuint video_slot, GLenum pname, GLint* params);
GLEWAPI void glGetVideoui64vNV(GLuint video_slot, GLenum pname, GLuint64EXT* params);
GLEWAPI void glGetVideouivNV(GLuint video_slot, GLenum pname, GLuint* params);
GLEWAPI void glPresentFrameDualFillNV(GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
GLEWAPI void glPresentFrameKeyedNV(GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);

GLEWAPI void glPrimitiveRestartIndexNV(GLuint index);
GLEWAPI void glPrimitiveRestartNV(void);

GLEWAPI void glCombinerInputNV(GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
GLEWAPI void glCombinerOutputNV(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
GLEWAPI void glCombinerParameterfNV(GLenum pname, GLfloat param);
GLEWAPI void glCombinerParameterfvNV(GLenum pname, const GLfloat* params);
GLEWAPI void glCombinerParameteriNV(GLenum pname, GLint param);
GLEWAPI void glCombinerParameterivNV(GLenum pname, const GLint* params);
GLEWAPI void glFinalCombinerInputNV(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
GLEWAPI void glGetCombinerInputParameterfvNV(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat* params);
GLEWAPI void glGetCombinerInputParameterivNV(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint* params);
GLEWAPI void glGetCombinerOutputParameterfvNV(GLenum stage, GLenum portion, GLenum pname, GLfloat* params);
GLEWAPI void glGetCombinerOutputParameterivNV(GLenum stage, GLenum portion, GLenum pname, GLint* params);
GLEWAPI void glGetFinalCombinerInputParameterfvNV(GLenum variable, GLenum pname, GLfloat* params);
GLEWAPI void glGetFinalCombinerInputParameterivNV(GLenum variable, GLenum pname, GLint* params);

GLEWAPI void glCombinerStageParameterfvNV(GLenum stage, GLenum pname, const GLfloat* params);
GLEWAPI void glGetCombinerStageParameterfvNV(GLenum stage, GLenum pname, GLfloat* params);

GLEWAPI void glGetBufferParameterui64vNV(GLenum target, GLenum pname, GLuint64EXT* params);
GLEWAPI void glGetIntegerui64vNV(GLenum value, GLuint64EXT* result);
GLEWAPI void glGetNamedBufferParameterui64vNV(GLuint buffer, GLenum pname, GLuint64EXT* params);
GLEWAPI GLboolean glIsBufferResidentNV(GLenum target);
GLEWAPI GLboolean glIsNamedBufferResidentNV(GLuint buffer);
GLEWAPI void glMakeBufferNonResidentNV(GLenum target);
GLEWAPI void glMakeBufferResidentNV(GLenum target, GLenum access);
GLEWAPI void glMakeNamedBufferNonResidentNV(GLuint buffer);
GLEWAPI void glMakeNamedBufferResidentNV(GLuint buffer, GLenum access);
GLEWAPI void glProgramUniformui64NV(GLuint program, GLint location, GLuint64EXT value);
GLEWAPI void glProgramUniformui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
GLEWAPI void glUniformui64NV(GLint location, GLuint64EXT value);
GLEWAPI void glUniformui64vNV(GLint location, GLsizei count, const GLuint64EXT* value);

GLEWAPI void glTextureBarrierNV(void);

GLEWAPI void glTexImage2DMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
GLEWAPI void glTexImage3DMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
GLEWAPI void glTextureImage2DMultisampleCoverageNV(GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
GLEWAPI void glTextureImage2DMultisampleNV(GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
GLEWAPI void glTextureImage3DMultisampleCoverageNV(GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
GLEWAPI void glTextureImage3DMultisampleNV(GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);

GLEWAPI void glActiveVaryingNV(GLuint program, const GLchar *name);
GLEWAPI void glBeginTransformFeedbackNV(GLenum primitiveMode);
GLEWAPI void glBindBufferBaseNV(GLenum target, GLuint index, GLuint buffer);
GLEWAPI void glBindBufferOffsetNV(GLenum target, GLuint index, GLuint buffer, GLintptr offset);
GLEWAPI void glBindBufferRangeNV(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLEWAPI void glEndTransformFeedbackNV(void);
GLEWAPI void glGetActiveVaryingNV(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
GLEWAPI void glGetTransformFeedbackVaryingNV(GLuint program, GLuint index, GLint *location);
GLEWAPI GLint glGetVaryingLocationNV(GLuint program, const GLchar *name);
GLEWAPI void glTransformFeedbackAttribsNV(GLuint count, const GLint *attribs, GLenum bufferMode);
GLEWAPI void glTransformFeedbackVaryingsNV(GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);

GLEWAPI void glBindTransformFeedbackNV(GLenum target, GLuint id);
GLEWAPI void glDeleteTransformFeedbacksNV(GLsizei n, const GLuint* ids);
GLEWAPI void glDrawTransformFeedbackNV(GLenum mode, GLuint id);
GLEWAPI void glGenTransformFeedbacksNV(GLsizei n, GLuint* ids);
GLEWAPI GLboolean glIsTransformFeedbackNV(GLuint id);
GLEWAPI void glPauseTransformFeedbackNV(void);
GLEWAPI void glResumeTransformFeedbackNV(void);

GLEWAPI void glVDPAUFiniNV(void);
GLEWAPI void glVDPAUGetSurfaceivNV(GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei* length, GLint *values);
GLEWAPI void glVDPAUInitNV(const void* vdpDevice, const GLvoid*getProcAddress);
GLEWAPI void glVDPAUIsSurfaceNV(GLvdpauSurfaceNV surface);
GLEWAPI void glVDPAUMapSurfacesNV(GLsizei numSurfaces, const GLvdpauSurfaceNV* surfaces);
GLEWAPI GLvdpauSurfaceNV glVDPAURegisterOutputSurfaceNV(const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
GLEWAPI GLvdpauSurfaceNV glVDPAURegisterVideoSurfaceNV(const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
GLEWAPI void glVDPAUSurfaceAccessNV(GLvdpauSurfaceNV surface, GLenum access);
GLEWAPI void glVDPAUUnmapSurfacesNV(GLsizei numSurface, const GLvdpauSurfaceNV* surfaces);
GLEWAPI void glVDPAUUnregisterSurfaceNV(GLvdpauSurfaceNV surface);

GLEWAPI void glFlushVertexArrayRangeNV(void);
GLEWAPI void glVertexArrayRangeNV(GLsizei length, void* pointer);

GLEWAPI void glGetVertexAttribLi64vNV(GLuint index, GLenum pname, GLint64EXT* params);
GLEWAPI void glGetVertexAttribLui64vNV(GLuint index, GLenum pname, GLuint64EXT* params);
GLEWAPI void glVertexAttribL1i64NV(GLuint index, GLint64EXT x);
GLEWAPI void glVertexAttribL1i64vNV(GLuint index, const GLint64EXT* v);
GLEWAPI void glVertexAttribL1ui64NV(GLuint index, GLuint64EXT x);
GLEWAPI void glVertexAttribL1ui64vNV(GLuint index, const GLuint64EXT* v);
GLEWAPI void glVertexAttribL2i64NV(GLuint index, GLint64EXT x, GLint64EXT y);
GLEWAPI void glVertexAttribL2i64vNV(GLuint index, const GLint64EXT* v);
GLEWAPI void glVertexAttribL2ui64NV(GLuint index, GLuint64EXT x, GLuint64EXT y);
GLEWAPI void glVertexAttribL2ui64vNV(GLuint index, const GLuint64EXT* v);
GLEWAPI void glVertexAttribL3i64NV(GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLEWAPI void glVertexAttribL3i64vNV(GLuint index, const GLint64EXT* v);
GLEWAPI void glVertexAttribL3ui64NV(GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLEWAPI void glVertexAttribL3ui64vNV(GLuint index, const GLuint64EXT* v);
GLEWAPI void glVertexAttribL4i64NV(GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLEWAPI void glVertexAttribL4i64vNV(GLuint index, const GLint64EXT* v);
GLEWAPI void glVertexAttribL4ui64NV(GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLEWAPI void glVertexAttribL4ui64vNV(GLuint index, const GLuint64EXT* v);
GLEWAPI void glVertexAttribLFormatNV(GLuint index, GLint size, GLenum type, GLsizei stride);

GLEWAPI void glBufferAddressRangeNV(GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
GLEWAPI void glColorFormatNV(GLint size, GLenum type, GLsizei stride);
GLEWAPI void glEdgeFlagFormatNV(GLsizei stride);
GLEWAPI void glFogCoordFormatNV(GLenum type, GLsizei stride);
GLEWAPI void glGetIntegerui64i_vNV(GLenum value, GLuint index, GLuint64EXT result[]);
GLEWAPI void glIndexFormatNV(GLenum type, GLsizei stride);
GLEWAPI void glNormalFormatNV(GLenum type, GLsizei stride);
GLEWAPI void glSecondaryColorFormatNV(GLint size, GLenum type, GLsizei stride);
GLEWAPI void glTexCoordFormatNV(GLint size, GLenum type, GLsizei stride);
GLEWAPI void glVertexAttribFormatNV(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
GLEWAPI void glVertexAttribIFormatNV(GLuint index, GLint size, GLenum type, GLsizei stride);
GLEWAPI void glVertexFormatNV(GLint size, GLenum type, GLsizei stride);

GLEWAPI GLboolean glAreProgramsResidentNV(GLsizei n, const GLuint* ids, GLboolean *residences);
GLEWAPI void glBindProgramNV(GLenum target, GLuint id);
GLEWAPI void glDeleteProgramsNV(GLsizei n, const GLuint* ids);
GLEWAPI void glExecuteProgramNV(GLenum target, GLuint id, const GLfloat* params);
GLEWAPI void glGenProgramsNV(GLsizei n, GLuint* ids);
GLEWAPI void glGetProgramParameterdvNV(GLenum target, GLuint index, GLenum pname, GLdouble* params);
GLEWAPI void glGetProgramParameterfvNV(GLenum target, GLuint index, GLenum pname, GLfloat* params);
GLEWAPI void glGetProgramStringNV(GLuint id, GLenum pname, GLubyte* program);
GLEWAPI void glGetProgramivNV(GLuint id, GLenum pname, GLint* params);
GLEWAPI void glGetTrackMatrixivNV(GLenum target, GLuint address, GLenum pname, GLint* params);
GLEWAPI void glGetVertexAttribPointervNV(GLuint index, GLenum pname, GLvoid** pointer);
GLEWAPI void glGetVertexAttribdvNV(GLuint index, GLenum pname, GLdouble* params);
GLEWAPI void glGetVertexAttribfvNV(GLuint index, GLenum pname, GLfloat* params);
GLEWAPI void glGetVertexAttribivNV(GLuint index, GLenum pname, GLint* params);
GLEWAPI GLboolean glIsProgramNV(GLuint id);
GLEWAPI void glLoadProgramNV(GLenum target, GLuint id, GLsizei len, const GLubyte* program);
GLEWAPI void glProgramParameter4dNV(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glProgramParameter4dvNV(GLenum target, GLuint index, const GLdouble* params);
GLEWAPI void glProgramParameter4fNV(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glProgramParameter4fvNV(GLenum target, GLuint index, const GLfloat* params);
GLEWAPI void glProgramParameters4dvNV(GLenum target, GLuint index, GLsizei num, const GLdouble* params);
GLEWAPI void glProgramParameters4fvNV(GLenum target, GLuint index, GLsizei num, const GLfloat* params);
GLEWAPI void glRequestResidentProgramsNV(GLsizei n, GLuint* ids);
GLEWAPI void glTrackMatrixNV(GLenum target, GLuint address, GLenum matrix, GLenum transform);
GLEWAPI void glVertexAttrib1dNV(GLuint index, GLdouble x);
GLEWAPI void glVertexAttrib1dvNV(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib1fNV(GLuint index, GLfloat x);
GLEWAPI void glVertexAttrib1fvNV(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib1sNV(GLuint index, GLshort x);
GLEWAPI void glVertexAttrib1svNV(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y);
GLEWAPI void glVertexAttrib2dvNV(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y);
GLEWAPI void glVertexAttrib2fvNV(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib2sNV(GLuint index, GLshort x, GLshort y);
GLEWAPI void glVertexAttrib2svNV(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLEWAPI void glVertexAttrib3dvNV(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glVertexAttrib3fvNV(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z);
GLEWAPI void glVertexAttrib3svNV(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLEWAPI void glVertexAttrib4dvNV(GLuint index, const GLdouble* v);
GLEWAPI void glVertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glVertexAttrib4fvNV(GLuint index, const GLfloat* v);
GLEWAPI void glVertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
GLEWAPI void glVertexAttrib4svNV(GLuint index, const GLshort* v);
GLEWAPI void glVertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
GLEWAPI void glVertexAttrib4ubvNV(GLuint index, const GLubyte* v);
GLEWAPI void glVertexAttribPointerNV(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
GLEWAPI void glVertexAttribs1dvNV(GLuint index, GLsizei n, const GLdouble* v);
GLEWAPI void glVertexAttribs1fvNV(GLuint index, GLsizei n, const GLfloat* v);
GLEWAPI void glVertexAttribs1svNV(GLuint index, GLsizei n, const GLshort* v);
GLEWAPI void glVertexAttribs2dvNV(GLuint index, GLsizei n, const GLdouble* v);
GLEWAPI void glVertexAttribs2fvNV(GLuint index, GLsizei n, const GLfloat* v);
GLEWAPI void glVertexAttribs2svNV(GLuint index, GLsizei n, const GLshort* v);
GLEWAPI void glVertexAttribs3dvNV(GLuint index, GLsizei n, const GLdouble* v);
GLEWAPI void glVertexAttribs3fvNV(GLuint index, GLsizei n, const GLfloat* v);
GLEWAPI void glVertexAttribs3svNV(GLuint index, GLsizei n, const GLshort* v);
GLEWAPI void glVertexAttribs4dvNV(GLuint index, GLsizei n, const GLdouble* v);
GLEWAPI void glVertexAttribs4fvNV(GLuint index, GLsizei n, const GLfloat* v);
GLEWAPI void glVertexAttribs4svNV(GLuint index, GLsizei n, const GLshort* v);
GLEWAPI void glVertexAttribs4ubvNV(GLuint index, GLsizei n, const GLubyte* v);

GLEWAPI void glBeginVideoCaptureNV(GLuint video_capture_slot);
GLEWAPI void glBindVideoCaptureStreamBufferNV(GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
GLEWAPI void glBindVideoCaptureStreamTextureNV(GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
GLEWAPI void glEndVideoCaptureNV(GLuint video_capture_slot);
GLEWAPI void glGetVideoCaptureStreamdvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble* params);
GLEWAPI void glGetVideoCaptureStreamfvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat* params);
GLEWAPI void glGetVideoCaptureStreamivNV(GLuint video_capture_slot, GLuint stream, GLenum pname, GLint* params);
GLEWAPI void glGetVideoCaptureivNV(GLuint video_capture_slot, GLenum pname, GLint* params);
GLEWAPI GLenum glVideoCaptureNV(GLuint video_capture_slot, GLuint* sequence_num, GLuint64EXT *capture_time);
GLEWAPI void glVideoCaptureStreamParameterdvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble* params);
GLEWAPI void glVideoCaptureStreamParameterfvNV(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat* params);
GLEWAPI void glVideoCaptureStreamParameterivNV(GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint* params);

GLEWAPI void glClearDepthfOES(GLclampd depth);
GLEWAPI void glClipPlanefOES(GLenum plane, const GLfloat* equation);
GLEWAPI void glDepthRangefOES(GLclampf n, GLclampf f);
GLEWAPI void glFrustumfOES(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
GLEWAPI void glGetClipPlanefOES(GLenum plane, GLfloat* equation);
GLEWAPI void glOrthofOES(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);

GLEWAPI const GLchar* glErrorStringREGAL(GLenum error);

GLEWAPI GLboolean glGetExtensionREGAL(const GLchar* ext);
GLEWAPI GLboolean glIsSupportedREGAL(const GLchar* ext);

GLEWAPI void glDetailTexFuncSGIS(GLenum target, GLsizei n, const GLfloat* points);
GLEWAPI void glGetDetailTexFuncSGIS(GLenum target, GLfloat* points);

GLEWAPI void glFogFuncSGIS(GLsizei n, const GLfloat* points);
GLEWAPI void glGetFogFuncSGIS(GLfloat* points);

GLEWAPI void glSampleMaskSGIS(GLclampf value, GLboolean invert);
GLEWAPI void glSamplePatternSGIS(GLenum pattern);

GLEWAPI void glGetSharpenTexFuncSGIS(GLenum target, GLfloat* points);
GLEWAPI void glSharpenTexFuncSGIS(GLenum target, GLsizei n, const GLfloat* points);

GLEWAPI void glTexImage4DSGIS(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLint border, GLenum format, GLenum type, const void* pixels);
GLEWAPI void glTexSubImage4DSGIS(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLenum format, GLenum type, const void* pixels);

GLEWAPI void glGetTexFilterFuncSGIS(GLenum target, GLenum filter, GLfloat* weights);
GLEWAPI void glTexFilterFuncSGIS(GLenum target, GLenum filter, GLsizei n, const GLfloat* weights);

GLEWAPI void glAsyncMarkerSGIX(GLuint marker);
GLEWAPI void glDeleteAsyncMarkersSGIX(GLuint marker, GLsizei range);
GLEWAPI GLint glFinishAsyncSGIX(GLuint* markerp);
GLEWAPI GLuint glGenAsyncMarkersSGIX(GLsizei range);
GLEWAPI GLboolean glIsAsyncMarkerSGIX(GLuint marker);
GLEWAPI GLint glPollAsyncSGIX(GLuint* markerp);

GLEWAPI void glFlushRasterSGIX(void);

GLEWAPI void glTextureFogSGIX(GLenum pname);

GLEWAPI void glFragmentColorMaterialSGIX(GLenum face, GLenum mode);
GLEWAPI void glFragmentLightModelfSGIX(GLenum pname, GLfloat param);
GLEWAPI void glFragmentLightModelfvSGIX(GLenum pname, GLfloat* params);
GLEWAPI void glFragmentLightModeliSGIX(GLenum pname, GLint param);
GLEWAPI void glFragmentLightModelivSGIX(GLenum pname, GLint* params);
GLEWAPI void glFragmentLightfSGIX(GLenum light, GLenum pname, GLfloat param);
GLEWAPI void glFragmentLightfvSGIX(GLenum light, GLenum pname, GLfloat* params);
GLEWAPI void glFragmentLightiSGIX(GLenum light, GLenum pname, GLint param);
GLEWAPI void glFragmentLightivSGIX(GLenum light, GLenum pname, GLint* params);
GLEWAPI void glFragmentMaterialfSGIX(GLenum face, GLenum pname, const GLfloat param);
GLEWAPI void glFragmentMaterialfvSGIX(GLenum face, GLenum pname, const GLfloat* params);
GLEWAPI void glFragmentMaterialiSGIX(GLenum face, GLenum pname, const GLint param);
GLEWAPI void glFragmentMaterialivSGIX(GLenum face, GLenum pname, const GLint* params);
GLEWAPI void glGetFragmentLightfvSGIX(GLenum light, GLenum value, GLfloat* data);
GLEWAPI void glGetFragmentLightivSGIX(GLenum light, GLenum value, GLint* data);
GLEWAPI void glGetFragmentMaterialfvSGIX(GLenum face, GLenum pname, const GLfloat* data);
GLEWAPI void glGetFragmentMaterialivSGIX(GLenum face, GLenum pname, const GLint* data);

GLEWAPI void glFrameZoomSGIX(GLint factor);

GLEWAPI void glPixelTexGenSGIX(GLenum mode);

GLEWAPI void glReferencePlaneSGIX(const GLdouble* equation);

GLEWAPI void glSpriteParameterfSGIX(GLenum pname, GLfloat param);
GLEWAPI void glSpriteParameterfvSGIX(GLenum pname, GLfloat* params);
GLEWAPI void glSpriteParameteriSGIX(GLenum pname, GLint param);
GLEWAPI void glSpriteParameterivSGIX(GLenum pname, GLint* params);

GLEWAPI void glTagSampleBufferSGIX(void);

GLEWAPI void glColorTableParameterfvSGI(GLenum target, GLenum pname, const GLfloat* params);
GLEWAPI void glColorTableParameterivSGI(GLenum target, GLenum pname, const GLint* params);
GLEWAPI void glColorTableSGI(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* table);
GLEWAPI void glCopyColorTableSGI(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
GLEWAPI void glGetColorTableParameterfvSGI(GLenum target, GLenum pname, GLfloat* params);
GLEWAPI void glGetColorTableParameterivSGI(GLenum target, GLenum pname, GLint* params);
GLEWAPI void glGetColorTableSGI(GLenum target, GLenum format, GLenum type, void* table);

GLEWAPI void glFinishTextureSUNX(void);

GLEWAPI void glGlobalAlphaFactorbSUN(GLbyte factor);
GLEWAPI void glGlobalAlphaFactordSUN(GLdouble factor);
GLEWAPI void glGlobalAlphaFactorfSUN(GLfloat factor);
GLEWAPI void glGlobalAlphaFactoriSUN(GLint factor);
GLEWAPI void glGlobalAlphaFactorsSUN(GLshort factor);
GLEWAPI void glGlobalAlphaFactorubSUN(GLubyte factor);
GLEWAPI void glGlobalAlphaFactoruiSUN(GLuint factor);
GLEWAPI void glGlobalAlphaFactorusSUN(GLushort factor);

GLEWAPI void glReadVideoPixelsSUN(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);

GLEWAPI void glReplacementCodePointerSUN(GLenum type, GLsizei stride, const void* pointer);
GLEWAPI void glReplacementCodeubSUN(GLubyte code);
GLEWAPI void glReplacementCodeubvSUN(const GLubyte* code);
GLEWAPI void glReplacementCodeuiSUN(GLuint code);
GLEWAPI void glReplacementCodeuivSUN(const GLuint* code);
GLEWAPI void glReplacementCodeusSUN(GLushort code);
GLEWAPI void glReplacementCodeusvSUN(const GLushort* code);

GLEWAPI void glColor3fVertex3fSUN(GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glColor3fVertex3fvSUN(const GLfloat* c, const GLfloat *v);
GLEWAPI void glColor4fNormal3fVertex3fSUN(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glColor4fNormal3fVertex3fvSUN(const GLfloat* c, const GLfloat *n, const GLfloat *v);
GLEWAPI void glColor4ubVertex2fSUN(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
GLEWAPI void glColor4ubVertex2fvSUN(const GLubyte* c, const GLfloat *v);
GLEWAPI void glColor4ubVertex3fSUN(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glColor4ubVertex3fvSUN(const GLubyte* c, const GLfloat *v);
GLEWAPI void glNormal3fVertex3fSUN(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glNormal3fVertex3fvSUN(const GLfloat* n, const GLfloat *v);
GLEWAPI void glReplacementCodeuiColor3fVertex3fSUN(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiColor3fVertex3fvSUN(const GLuint* rc, const GLfloat *c, const GLfloat *v);
GLEWAPI void glReplacementCodeuiColor4fNormal3fVertex3fSUN(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiColor4fNormal3fVertex3fvSUN(const GLuint* rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
GLEWAPI void glReplacementCodeuiColor4ubVertex3fSUN(GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiColor4ubVertex3fvSUN(const GLuint* rc, const GLubyte *c, const GLfloat *v);
GLEWAPI void glReplacementCodeuiNormal3fVertex3fSUN(GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiNormal3fVertex3fvSUN(const GLuint* rc, const GLfloat *n, const GLfloat *v);
GLEWAPI void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN(const GLuint* rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
GLEWAPI void glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN(const GLuint* rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
GLEWAPI void glReplacementCodeuiTexCoord2fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiTexCoord2fVertex3fvSUN(const GLuint* rc, const GLfloat *tc, const GLfloat *v);
GLEWAPI void glReplacementCodeuiVertex3fSUN(GLuint rc, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glReplacementCodeuiVertex3fvSUN(const GLuint* rc, const GLfloat *v);
GLEWAPI void glTexCoord2fColor3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glTexCoord2fColor3fVertex3fvSUN(const GLfloat* tc, const GLfloat *c, const GLfloat *v);
GLEWAPI void glTexCoord2fColor4fNormal3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glTexCoord2fColor4fNormal3fVertex3fvSUN(const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
GLEWAPI void glTexCoord2fColor4ubVertex3fSUN(GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glTexCoord2fColor4ubVertex3fvSUN(const GLfloat* tc, const GLubyte *c, const GLfloat *v);
GLEWAPI void glTexCoord2fNormal3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glTexCoord2fNormal3fVertex3fvSUN(const GLfloat* tc, const GLfloat *n, const GLfloat *v);
GLEWAPI void glTexCoord2fVertex3fSUN(GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
GLEWAPI void glTexCoord2fVertex3fvSUN(const GLfloat* tc, const GLfloat *v);
GLEWAPI void glTexCoord4fColor4fNormal3fVertex4fSUN(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glTexCoord4fColor4fNormal3fVertex4fvSUN(const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
GLEWAPI void glTexCoord4fVertex4fSUN(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLEWAPI void glTexCoord4fVertex4fvSUN(const GLfloat* tc, const GLfloat *v);

GLEWAPI void glAddSwapHintRectWIN(GLint x, GLint y, GLsizei width, GLsizei height);

#if defined(GLEW_MX) && !defined(_WIN32)
struct GLEWContextStruct
{
#endif /* GLEW_MX */

GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_2;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_2_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_3;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_4;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_5;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_2_0;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_2_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_0;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_2;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_3;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_0;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_2;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_3;
GLEW_VAR_EXPORT GLboolean __GLEW_3DFX_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_3DFX_tbuffer;
GLEW_VAR_EXPORT GLboolean __GLEW_3DFX_texture_compression_FXT1;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_blend_minmax_factor;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_conservative_depth;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_debug_output;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_depth_clamp_separate;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_draw_buffers_blend;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_multi_draw_indirect;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_name_gen_delete;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_performance_monitor;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_pinned_memory;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_query_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_sample_positions;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_seamless_cubemap_per_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_shader_stencil_export;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_stencil_operation_extended;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_texture_texture4;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_transform_feedback3_lines_triangles;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_vertex_shader_layer;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_vertex_shader_tessellator;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_vertex_shader_viewport_index;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_aux_depth_stencil;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_client_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_element_array;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_fence;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_float_pixels;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_flush_buffer_range;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_object_purgeable;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_pixel_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_rgb_422;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_row_bytes;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_specular_vector;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_texture_range;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_transform_hint;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_vertex_array_object;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_vertex_array_range;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_vertex_program_evaluators;
GLEW_VAR_EXPORT GLboolean __GLEW_APPLE_ycbcr_422;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_ES2_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_ES3_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_arrays_of_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_base_instance;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_blend_func_extended;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_cl_event;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_clear_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_color_buffer_float;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_compressed_texture_pixel_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_compute_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_conservative_depth;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_copy_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_copy_image;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_debug_output;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_depth_buffer_float;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_depth_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_depth_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_buffers;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_buffers_blend;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_elements_base_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_indirect;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_instanced;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_explicit_attrib_location;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_explicit_uniform_location;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_coord_conventions;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_layer_viewport;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_program;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_program_shadow;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_framebuffer_no_attachments;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_framebuffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_framebuffer_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_geometry_shader4;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_get_program_binary;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_gpu_shader5;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_gpu_shader_fp64;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_half_float_pixel;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_half_float_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_imaging;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_instanced_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_internalformat_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_internalformat_query2;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_invalidate_subdata;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_map_buffer_alignment;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_map_buffer_range;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_matrix_palette;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_multi_draw_indirect;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_multitexture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_occlusion_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_occlusion_query2;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_pixel_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_point_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_point_sprite;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_program_interface_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_provoking_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robust_buffer_access_behavior;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robustness;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robustness_application_isolation;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robustness_share_group_isolation;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sample_shading;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sampler_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_seamless_cube_map;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_separate_shader_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_atomic_counters;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_bit_encoding;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_image_load_store;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_image_size;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_precision;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_stencil_export;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_storage_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_subroutine;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_texture_lod;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_100;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_420pack;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_include;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_packing;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shadow;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shadow_ambient;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_stencil_texturing;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sync;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_tessellation_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_border_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_buffer_object_rgb32;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_buffer_range;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_compression;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_compression_bptc;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_compression_rgtc;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_cube_map;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_cube_map_array;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_add;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_combine;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_crossbar;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_dot3;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_float;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_gather;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_mirrored_repeat;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_non_power_of_two;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_query_levels;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_query_lod;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_rectangle;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_rg;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_rgb10_a2ui;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_storage_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_swizzle;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_view;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_timer_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transform_feedback2;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transform_feedback3;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transform_feedback_instanced;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transpose_matrix;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_uniform_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_array_bgra;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_array_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_attrib_64bit;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_attrib_binding;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_blend;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_program;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_type_2_10_10_10_rev;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_viewport_array;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_window_pos;
GLEW_VAR_EXPORT GLboolean __GLEW_ATIX_point_sprites;
GLEW_VAR_EXPORT GLboolean __GLEW_ATIX_texture_env_combine3;
GLEW_VAR_EXPORT GLboolean __GLEW_ATIX_texture_env_route;
GLEW_VAR_EXPORT GLboolean __GLEW_ATIX_vertex_shader_output_point_size;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_draw_buffers;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_element_array;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_envmap_bumpmap;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_fragment_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_map_object_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_meminfo;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_pn_triangles;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_separate_stencil;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_shader_texture_lod;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_text_fragment_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_texture_compression_3dc;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_texture_env_combine3;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_texture_float;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_texture_mirror_once;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_vertex_array_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_vertex_attrib_array_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ATI_vertex_streams;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_422_pixels;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_Cg_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_abgr;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_bgra;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_bindable_uniform;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_color;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_equation_separate;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_func_separate;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_logic_op;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_minmax;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_subtract;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_clip_volume_hint;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_cmyka;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_color_subtable;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_compiled_vertex_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_convolution;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_coordinate_frame;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_copy_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_cull_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_debug_marker;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_depth_bounds_test;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_direct_state_access;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_buffers2;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_instanced;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_range_elements;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_fog_coord;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_fragment_lighting;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_blit;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_multisample_blit_scaled;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_geometry_shader4;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_gpu_program_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_gpu_shader4;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_histogram;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_array_formats;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_func;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_material;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_light_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_misc_attribute;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multi_draw_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_packed_depth_stencil;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_packed_float;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_packed_pixels;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_paletted_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_pixel_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_pixel_transform;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_pixel_transform_color_table;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_point_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_polygon_offset;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_provoking_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_rescale_normal;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_scene_marker;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_secondary_color;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_separate_shader_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_separate_specular_color;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_image_load_store;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shadow_funcs;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shared_texture_palette;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_stencil_clear_tag;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_stencil_two_side;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_stencil_wrap;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_subtexture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture3D;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_dxt1;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_latc;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_rgtc;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_s3tc;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_cube_map;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_edge_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env_add;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env_combine;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env_dot3;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_filter_anisotropic;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_integer;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_lod_bias;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_mirror_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_perturb_normal;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_rectangle;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_sRGB_decode;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_shared_exponent;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_snorm;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_swizzle;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_timer_query;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_transform_feedback;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_array_bgra;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_attrib_64bit;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_weighting;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_x11_sync_object;
GLEW_VAR_EXPORT GLboolean __GLEW_GREMEDY_frame_terminator;
GLEW_VAR_EXPORT GLboolean __GLEW_GREMEDY_string_marker;
GLEW_VAR_EXPORT GLboolean __GLEW_HP_convolution_border_modes;
GLEW_VAR_EXPORT GLboolean __GLEW_HP_image_transform;
GLEW_VAR_EXPORT GLboolean __GLEW_HP_occlusion_test;
GLEW_VAR_EXPORT GLboolean __GLEW_HP_texture_lighting;
GLEW_VAR_EXPORT GLboolean __GLEW_IBM_cull_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_IBM_multimode_draw_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_IBM_rasterpos_clip;
GLEW_VAR_EXPORT GLboolean __GLEW_IBM_static_data;
GLEW_VAR_EXPORT GLboolean __GLEW_IBM_texture_mirrored_repeat;
GLEW_VAR_EXPORT GLboolean __GLEW_IBM_vertex_array_lists;
GLEW_VAR_EXPORT GLboolean __GLEW_INGR_color_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_INGR_interlace_read;
GLEW_VAR_EXPORT GLboolean __GLEW_INTEL_parallel_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_INTEL_texture_scissor;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_debug;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_texture_compression_astc_ldr;
GLEW_VAR_EXPORT GLboolean __GLEW_KTX_buffer_region;
GLEW_VAR_EXPORT GLboolean __GLEW_MESAX_texture_stack;
GLEW_VAR_EXPORT GLboolean __GLEW_MESA_pack_invert;
GLEW_VAR_EXPORT GLboolean __GLEW_MESA_resize_buffers;
GLEW_VAR_EXPORT GLboolean __GLEW_MESA_window_pos;
GLEW_VAR_EXPORT GLboolean __GLEW_MESA_ycbcr_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_NVX_gpu_memory_info;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_bindless_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_blend_square;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_conditional_render;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_copy_depth_to_color;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_copy_image;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_depth_buffer_float;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_depth_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_depth_range_unclamped;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_evaluators;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_explicit_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_fence;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_float_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_fog_distance;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_fragment_program;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_fragment_program2;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_fragment_program4;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_fragment_program_option;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_framebuffer_multisample_coverage;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_geometry_program4;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_geometry_shader4;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_gpu_program4;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_gpu_program5;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_gpu_program_fp64;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_gpu_shader5;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_half_float;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_light_max_exponent;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_multisample_coverage;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_multisample_filter_hint;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_occlusion_query;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_packed_depth_stencil;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_parameter_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_parameter_buffer_object2;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_path_rendering;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_pixel_data_range;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_point_sprite;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_present_video;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_primitive_restart;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_register_combiners;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_register_combiners2;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_shader_atomic_float;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_shader_buffer_load;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_tessellation_program5;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texgen_emboss;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texgen_reflection;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_barrier;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_compression_vtc;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_env_combine4;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_expand_normal;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_rectangle;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_shader2;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_texture_shader3;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_transform_feedback;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_transform_feedback2;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vdpau_interop;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_array_range;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_array_range2;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_attrib_integer_64bit;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_buffer_unified_memory;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_program;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_program1_1;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_program2;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_program2_option;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_program3;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_vertex_program4;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_video_capture;
GLEW_VAR_EXPORT GLboolean __GLEW_OES_byte_coordinates;
GLEW_VAR_EXPORT GLboolean __GLEW_OES_compressed_paletted_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_OES_read_format;
GLEW_VAR_EXPORT GLboolean __GLEW_OES_single_precision;
GLEW_VAR_EXPORT GLboolean __GLEW_OML_interlace;
GLEW_VAR_EXPORT GLboolean __GLEW_OML_resample;
GLEW_VAR_EXPORT GLboolean __GLEW_OML_subsample;
GLEW_VAR_EXPORT GLboolean __GLEW_PGI_misc_hints;
GLEW_VAR_EXPORT GLboolean __GLEW_PGI_vertex_hints;
GLEW_VAR_EXPORT GLboolean __GLEW_REGAL_error_string;
GLEW_VAR_EXPORT GLboolean __GLEW_REGAL_extension_query;
GLEW_VAR_EXPORT GLboolean __GLEW_REGAL_log;
GLEW_VAR_EXPORT GLboolean __GLEW_REND_screen_coordinates;
GLEW_VAR_EXPORT GLboolean __GLEW_S3_s3tc;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_color_range;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_detail_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_fog_function;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_generate_mipmap;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_pixel_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_point_line_texgen;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_sharpen_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_texture4D;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_texture_border_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_texture_edge_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_texture_filter4;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_texture_lod;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIS_texture_select;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_async;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_async_histogram;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_async_pixel;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_blend_alpha_minmax;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_clipmap;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_convolution_accuracy;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_depth_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_flush_raster;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_fog_offset;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_fog_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_fragment_specular_lighting;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_framezoom;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_interlace;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_ir_instrument1;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_list_priority;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_pixel_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_pixel_texture_bits;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_reference_plane;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_resample;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_shadow;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_shadow_ambient;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_sprite;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_tag_sample_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_texture_add_env;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_texture_coordinate_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_texture_lod_bias;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_texture_multi_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_texture_range;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_texture_scale_bias;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_vertex_preclip;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_vertex_preclip_hint;
GLEW_VAR_EXPORT GLboolean __GLEW_SGIX_ycrcb;
GLEW_VAR_EXPORT GLboolean __GLEW_SGI_color_matrix;
GLEW_VAR_EXPORT GLboolean __GLEW_SGI_color_table;
GLEW_VAR_EXPORT GLboolean __GLEW_SGI_texture_color_table;
GLEW_VAR_EXPORT GLboolean __GLEW_SUNX_constant_data;
GLEW_VAR_EXPORT GLboolean __GLEW_SUN_convolution_border_modes;
GLEW_VAR_EXPORT GLboolean __GLEW_SUN_global_alpha;
GLEW_VAR_EXPORT GLboolean __GLEW_SUN_mesh_array;
GLEW_VAR_EXPORT GLboolean __GLEW_SUN_read_video_pixels;
GLEW_VAR_EXPORT GLboolean __GLEW_SUN_slice_accum;
GLEW_VAR_EXPORT GLboolean __GLEW_SUN_triangle_list;
GLEW_VAR_EXPORT GLboolean __GLEW_SUN_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_WIN_phong_shading;
GLEW_VAR_EXPORT GLboolean __GLEW_WIN_specular_fog;
GLEW_VAR_EXPORT GLboolean __GLEW_WIN_swap_hint;


/* ------------------------------------------------------------------------- */

/* error codes */
#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_ERROR_NO_GL_VERSION 1  /* missing GL version */
#define GLEW_ERROR_GL_VERSION_10_ONLY 2  /* Need at least OpenGL 1.1 */
#define GLEW_ERROR_GLX_VERSION_11_ONLY 3  /* Need at least GLX 1.2 */
#define GLEW_ERROR_GLX_VERSION_10_ONLY 4 /* GLX 1.1 and up are not supported */
#define GLEW_ERROR_NO_GLX 5              /* GLX not available. */
#define GLEW_ERROR_DLOPEN 6              /* dlopen(3) call failed. */
#define GLEW_ERROR_GPA 7                 /* could not find GetProcAddr fqn */

/* string codes */
#define GLEW_VERSION 1
#define GLEW_VERSION_MAJOR 2
#define GLEW_VERSION_MINOR 3
#define GLEW_VERSION_MICRO 4

/* Library type; determines which "GetProcAddress" to use. */
enum GL_Library_Type {
  GLEW_LIB_TYPE_NATIVE = 0,
  GLEW_LIB_TYPE_OSMESA
};

/*
 * OpenGL naming convention codes.
 * Used to switch between `standard' OpenGL and mangled Mesa implementations.
 */
enum GL_Name_Convention {
  GLEW_NAME_STANDARD = 0,
  GLEW_NAME_MANGLED
};

/* API */
#ifdef GLEW_MX

typedef struct GLEWContextStruct GLEWContext;
GLEWAPI GLenum GLEWAPIENTRY glewContextInit (GLEWContext *ctx);
GLEWAPI GLboolean GLEWAPIENTRY glewContextIsSupported (const GLEWContext *ctx, const char *name);

#define glewInit() glewContextInit(glewGetContext())
#define glewIsSupported(x) glewContextIsSupported(glewGetContext(), x)
#define glewIsExtensionSupported(x) glewIsSupported(x)

#define GLEW_GET_VAR(x) (*(const GLboolean*)&(glewGetContext()->x))
#ifdef _WIN32
#  define GLEW_GET_FUN(x) glewGetContext()->x
#else
#  define GLEW_GET_FUN(x) x
#endif

#else /* GLEW_MX */

GLEWAPI GLenum GLEWAPIENTRY  glewInitLibrary (const char *name, enum GL_Library_Type, enum GL_Name_Convention);
GLEWAPI GLenum GLEWAPIENTRY  glewInit ();
GLEWAPI GLboolean GLEWAPIENTRY  glewIsSupported (const char* name);
#define glewIsExtensionSupported(x) glewIsSupported(x)

#define GLEW_GET_VAR(x) (*(const GLboolean*)&x)
#define GLEW_GET_FUN(x) x

#endif /* GLEW_MX */

GLEWAPI GLboolean glewExperimental;
GLEWAPI GLboolean GLEWAPIENTRY  glewGetExtension (const char* name);
GLEWAPI const GLubyte* GLEWAPIENTRY glewGetErrorString (GLenum error);
GLEWAPI const GLubyte* GLEWAPIENTRY glewGetString (GLenum name);

#ifdef __cplusplus
}
#endif

#ifdef GLEW_APIENTRY_DEFINED
#undef GLEW_APIENTRY_DEFINED
#undef APIENTRY
#undef GLAPIENTRY
#define GLAPIENTRY
#endif

#ifdef GLEW_CALLBACK_DEFINED
#undef GLEW_CALLBACK_DEFINED
#undef CALLBACK
#endif

#ifdef GLEW_WINGDIAPI_DEFINED
#undef GLEW_WINGDIAPI_DEFINED
#undef WINGDIAPI
#endif

#undef GLAPI
/* #undef GLEWAPI */

#endif /* __glew_h__ */
