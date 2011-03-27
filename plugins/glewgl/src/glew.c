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

#include <GL/glew.h>
#if defined(_WIN32)
#  include <GL/wglew.h>
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
#  include <GL/glxew.h>
#endif

/*
 * Define glewGetContext and related helper macros.
 */
#ifdef GLEW_MX
#  define glewGetContext() ctx
#  ifdef _WIN32
#    define GLEW_CONTEXT_ARG_DEF_INIT GLEWContext* ctx
#    define GLEW_CONTEXT_ARG_VAR_INIT ctx
#    define wglewGetContext() ctx
#    define WGLEW_CONTEXT_ARG_DEF_INIT WGLEWContext* ctx
#    define WGLEW_CONTEXT_ARG_DEF_LIST WGLEWContext* ctx
#  else /* _WIN32 */
#    define GLEW_CONTEXT_ARG_DEF_INIT void
#    define GLEW_CONTEXT_ARG_VAR_INIT
#    define glxewGetContext() ctx
#    define GLXEW_CONTEXT_ARG_DEF_INIT void
#    define GLXEW_CONTEXT_ARG_DEF_LIST GLXEWContext* ctx
#  endif /* _WIN32 */
#  define GLEW_CONTEXT_ARG_DEF_LIST GLEWContext* ctx
#else /* GLEW_MX */
#  define GLEW_CONTEXT_ARG_DEF_INIT void
#  define GLEW_CONTEXT_ARG_VAR_INIT
#  define GLEW_CONTEXT_ARG_DEF_LIST void
#  define WGLEW_CONTEXT_ARG_DEF_INIT void
#  define WGLEW_CONTEXT_ARG_DEF_LIST void
#  define GLXEW_CONTEXT_ARG_DEF_INIT void
#  define GLXEW_CONTEXT_ARG_DEF_LIST void
#endif /* GLEW_MX */

#ifdef _WIN32
typedef void*(WINAPI *GPA)(const GLubyte*);
typedef void*(WINAPI *_GPA)(const char*);
typedef  PROC  (WINAPI* WGLGPA)(LPCSTR);
static WGLGPA _wglewGetProcAddress = NULL;
#else
typedef void*(*GPA)(const GLubyte*);
typedef void*(*_GPA)(const char*);
#endif

static enum GL_Name_Convention glew_convention = GLEW_NAME_STANDARD;
static enum GL_Library_Type gl_lib_type = GLEW_LIB_TYPE_NATIVE;
#ifdef _WIN32
static const char *glew_gl_lib = "opengl32.dll";
#elif defined(__APPLE__)
static const char *glew_gl_lib = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";
#elif defined(_AIX)
static const char *glew_gl_lib = "libGL.a";
#elif defined(__linux__)
static const char *glew_gl_lib = "libGL.so.1";
#endif

static GPA __glewXGetProcAddress = NULL;

#if defined(__APPLE__)
#include <stdlib.h>
#include <string.h>
#include <AvailabilityMacros.h>

#ifdef MAC_OS_X_VERSION_10_3

#include <dlfcn.h>

void* NSGLGetProcAddress (const GLubyte *name)
{
  static void* image = NULL;
  if (NULL == image)
  {
    image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
  }
  return image ? dlsym(image, (const char*)name) : NULL;
}

#endif /* MAC_OS_X_VERSION_10_3 */
#endif /* __APPLE__ */

#if defined(__sgi) || defined (__sun)
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void* dlGetProcAddress (const GLubyte* name)
{
  static void* h = NULL;
  static void* gpa;

  if (h == NULL)
  {
    if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL) return NULL;
    gpa = dlsym(h, "glXGetProcAddress");
  }

  if (gpa != NULL)
    return ((void*(*)(const GLubyte*))gpa)(name);
  else
    return dlsym(h, (const char*)name);
}
#endif /* __sgi || __sun */

#if defined(__linux__) || defined(__APPLE__) || defined(_WIN32) || \
    defined(_AIX)
#if defined (_WIN32)
static HMODULE dso_handle = NULL;
#else
#include <dlfcn.h>
static void *dso_handle = NULL;
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
# define UNUSED __attribute__((unused))
#else
# define UNUSED /* no attribute support */
#endif

/* Give a couple debugging macros which allow us to report issues, but only if
* a debugging define is given.  When defined, GLEW requires the standard C
* library. */
#if defined(GLEW_DEBUG) && defined(__STDC_VERSION__) && \
    __STDC_VERSION__ >= 199901L
# include <stdio.h>
# include <stdlib.h>
# define warning(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
# define error(...) do { fprintf(stderr, __VA_ARGS__); abort(); } while(0)
#else
  /* Some versions of IBM's xlc do not handle an ellipsis for macro arguments,
   * even if the macro definition doesn't use __VA_ARGS__.  Thus we define
   * empty functions for these instead of empty macros. */
  UNUSED static void warning(UNUSED const char* s, ...) { }
  static void error(UNUSED const char* s, ...) { }
#endif

/* Function pointer for OSMesa function loader. */
static _GPA _glewOSMesaGetProcAddress = NULL;

/* The interfaces to GetProcAddress for glX and OSMesa differ in both return
* type and argument type.  To get around this, we create two thunks which
* forward to the appropriate function, casting back and forth. */
static void* _glew_glx_thunk_gpa(const char *name) {
  return (void*) __glewXGetProcAddress((const GLubyte*)name);
}
#ifdef __APPLE__
/* Apple doesn't use a function loader in its proprietary APIs: just load the
 * symbol directly from the object. */
static void* _glew_dlsym_thunk_gpa(const char *name) {
  return (void*) dlsym(dso_handle, name);
}
#endif
static void* _glew_osmesa_thunk_gpa(const char *name) {
  return _glewOSMesaGetProcAddress(name);
}
static void* (*_glewGetProcAddress)(const char*);

#ifdef _WIN32
static void* _glew_wgl_thunk_gpa(const char* name) {
  void* addr = _wglewGetProcAddress((LPCSTR)name);
  if (addr == NULL)
  {
    addr = GetProcAddress(dso_handle, (LPCSTR)name);
  }

  return addr;
}
#endif

/* Initialize our function loader.
 * This loads function pointers for .. functions that load function pointers.
 * This translates to glXGetProcAddress on X-based platforms, and OSMesa
 * otherwise. */
static int _glewInitFunctionLoader()
{
  int name_offset = (glew_convention == GLEW_NAME_MANGLED) ? 0 : 1;
#if defined(_WIN32)
  const char wgl_gpa[] = "mwglGetProcAddress";
  DWORD err = ERROR_SUCCESS;
  if (NULL == dso_handle)
  {
    dso_handle = GetModuleHandle(glew_gl_lib);
    if (dso_handle == NULL)
    {
      dso_handle = LoadLibrary(glew_gl_lib);
    }
    if (dso_handle == NULL)
    {
      error("Could not load library `%s': %d", glew_gl_lib, GetLastError());
      return GLEW_ERROR_DLOPEN;
    }
  }
  SetLastError(ERROR_SUCCESS);
#else
  const char *dl_error;
  const char glx_gpa[] = "mglXGetProcAddressARB";
  if(NULL == dso_handle) {
#ifdef _AIX
    /* AIX needs RTLD_MEMBER to load shared libs; otherwise it returns an
     * `invalid magic number' error.  This apparently can be worked around by
     * building the shared library in a special way, but the system's supplied
     * libGL.a requires this, at least. */
    dso_handle = dlopen(glew_gl_lib, RTLD_LAZY | RTLD_LOCAL | RTLD_MEMBER);
#else
    dso_handle = dlopen(glew_gl_lib, RTLD_LAZY | RTLD_LOCAL);
#endif
    if(NULL == dso_handle) {
      error("Could not load library `%s': %s", glew_gl_lib, dlerror());
      return GLEW_ERROR_DLOPEN;
    }
  }
  dlerror(); /* clear any previous error. */
#endif

  switch (gl_lib_type)
  {
    case GLEW_LIB_TYPE_NATIVE:
#ifdef _WIN32
      _wglewGetProcAddress = (WGLGPA)GetProcAddress(dso_handle,
                                                    wgl_gpa + name_offset);
      _glewGetProcAddress = _glew_wgl_thunk_gpa;
#elif defined(__APPLE__)
      __glewXGetProcAddress = NULL;
      _glewGetProcAddress = _glew_dlsym_thunk_gpa;
#else
      /* In Mesa 7.6 and earlier, glXGPA-ARB was not mangled.  In 7.6.1 and
       * later, it does get mangled.  We'll just try both; if the mangled
       * name exists, great.  Otherwise, fall back to the unmangled name.
       * Google: "glXGPA with mangled Mesa" for more information. */
      __glewXGetProcAddress = (GPA) dlsym(dso_handle, glx_gpa);
      if(__glewXGetProcAddress == NULL) {
        dlerror(); /* clear the error from loading the `mgl' variant. */
        __glewXGetProcAddress = (GPA) dlsym(dso_handle, glx_gpa + name_offset);
      }
      _glewGetProcAddress = _glew_glx_thunk_gpa;
#endif /* _WIN32 */
      break;
    case GLEW_LIB_TYPE_OSMESA:
#ifdef _WIN32
      _glewOSMesaGetProcAddress = (_GPA) GetProcAddress(dso_handle,
                                                        "OSMesaGetProcAddress");
#else
      _glewOSMesaGetProcAddress = (_GPA) dlsym(dso_handle,
                                               "OSMesaGetProcAddress");
#endif
      _glewGetProcAddress = _glew_osmesa_thunk_gpa;
      break;
  }

#ifdef _WIN32
  err = GetLastError();
  if (err != ERROR_SUCCESS)
  {
    error("Could not find dynamic loading function; %d", err);
    FreeLibrary(dso_handle);
    dso_handle = NULL;
    return GLEW_ERROR_GPA;
  }
#else
  if((dl_error = dlerror()) != NULL) {
    error("Could not find dynamic loading function: %s", dl_error);
    dlclose(dso_handle);
    dso_handle = NULL;
    return GLEW_ERROR_GPA;
  }
#endif
  return GLEW_NO_ERROR;
}

#if defined(__GNUC__) && __GNUC__ >= 4
/* If they've got a GNU compiler, install a DSO finalizer.  Useful for
 * valgrinding. */
#include <stdio.h>
__attribute__((destructor))
static void _glewUnloadDL()
{
    if(dso_handle != NULL) {
      dlclose(dso_handle);
      dso_handle = NULL;
    }
}
#endif

#endif /* __linux__ || __APPLE__ || _WIN32 */

#define fqn_from_convention(convention, fqn) \
    (convention == GLEW_NAME_MANGLED ? "m"fqn : fqn)

/*
 * Define glewGetProcAddress.
 */
#if defined(_WIN32)
#  define glewGetProcAddress(name) (*_glewGetProcAddress)(name)
#else
#  if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_3) && !defined(MAC_OS_X_VERSION_10_4)
#    define glewGetProcAddress(name) NSGLGetProcAddress(name)
#  else
#    if defined(__sgi) || defined(__sun)
#      define glewGetProcAddress(name) dlGetProcAddress(name)
#    else /* __linux */
#      define glewGetProcAddress(name) (*_glewGetProcAddress)(name)
#    endif
#  endif
#endif

/*
 * Define GLboolean const cast.
 */
#define CONST_CAST(x) (*(GLboolean*)&x)

/*
 * GLEW, just like OpenGL or GLU, does not rely on the standard C library.
 * These functions implement the functionality required in this file.
 */
static GLuint _glewStrLen (const GLubyte* s)
{
  GLuint i=0;
  if (s == NULL) return 0;
  while (s[i] != '\0') i++;
  return i;
}

static GLuint _glewStrCLen (const GLubyte* s, GLubyte c)
{
  GLuint i=0;
  if (s == NULL) return 0;
  while (s[i] != '\0' && s[i] != c) i++;
  return (s[i] == '\0' || s[i] == c) ? i : 0;
}

static GLboolean _glewStrSame (const GLubyte* a, const GLubyte* b, GLuint n)
{
  GLuint i=0;
  if(a == NULL || b == NULL)
    return (a == NULL && b == NULL && n == 0) ? GL_TRUE : GL_FALSE;
  while (i < n && a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) i++;
  return i == n ? GL_TRUE : GL_FALSE;
}

static GLboolean _glewStrSame1 (GLubyte** a, GLuint* na, const GLubyte* b, GLuint nb)
{
  while (*na > 0 && (**a == ' ' || **a == '\n' || **a == '\r' || **a == '\t'))
  {
    (*a)++;
    (*na)--;
  }
  if(*na >= nb)
  {
    GLuint i=0;
    while (i < nb && (*a)+i != NULL && b+i != NULL && (*a)[i] == b[i]) i++;
    if(i == nb)
    {
      *a = *a + nb;
      *na = *na - nb;
      return GL_TRUE;
    }
  }
  return GL_FALSE;
}

static GLboolean _glewStrSame2 (GLubyte** a, GLuint* na, const GLubyte* b, GLuint nb)
{
  if(*na >= nb)
  {
    GLuint i=0;
    while (i < nb && (*a)+i != NULL && b+i != NULL && (*a)[i] == b[i]) i++;
    if(i == nb)
    {
      *a = *a + nb;
      *na = *na - nb;
      return GL_TRUE;
    }
  }
  return GL_FALSE;
}

static GLboolean _glewStrSame3 (GLubyte** a, GLuint* na, const GLubyte* b, GLuint nb)
{
  if(*na >= nb)
  {
    GLuint i=0;
    while (i < nb && (*a)+i != NULL && b+i != NULL && (*a)[i] == b[i]) i++;
    if (i == nb && (*na == nb || (*a)[i] == ' ' || (*a)[i] == '\n' || (*a)[i] == '\r' || (*a)[i] == '\t'))
    {
      *a = *a + nb;
      *na = *na - nb;
      return GL_TRUE;
    }
  }
  return GL_FALSE;
}

#if !defined(_WIN32) || !defined(GLEW_MX)

PFNGLACCUMPROC glAccum = NULL;
PFNGLALPHAFUNCPROC glAlphaFunc = NULL;
PFNGLARETEXTURESRESIDENTPROC glAreTexturesResident = NULL;
PFNGLARRAYELEMENTPROC glArrayElement = NULL;
PFNGLBEGINPROC glBegin = NULL;
PFNGLBINDTEXTUREPROC glBindTexture = NULL;
PFNGLBITMAPPROC glBitmap = NULL;
PFNGLBLENDFUNCPROC glBlendFunc = NULL;
PFNGLCALLLISTPROC glCallList = NULL;
PFNGLCALLLISTSPROC glCallLists = NULL;
PFNGLCLEARPROC glClear = NULL;
PFNGLCLEARACCUMPROC glClearAccum = NULL;
PFNGLCLEARCOLORPROC glClearColor = NULL;
PFNGLCLEARDEPTHPROC glClearDepth = NULL;
PFNGLCLEARINDEXPROC glClearIndex = NULL;
PFNGLCLEARSTENCILPROC glClearStencil = NULL;
PFNGLCLIPPLANEPROC glClipPlane = NULL;
PFNGLCOLOR3BPROC glColor3b = NULL;
PFNGLCOLOR3BVPROC glColor3bv = NULL;
PFNGLCOLOR3DPROC glColor3d = NULL;
PFNGLCOLOR3DVPROC glColor3dv = NULL;
PFNGLCOLOR3FPROC glColor3f = NULL;
PFNGLCOLOR3FVPROC glColor3fv = NULL;
PFNGLCOLOR3IPROC glColor3i = NULL;
PFNGLCOLOR3IVPROC glColor3iv = NULL;
PFNGLCOLOR3SPROC glColor3s = NULL;
PFNGLCOLOR3SVPROC glColor3sv = NULL;
PFNGLCOLOR3UBPROC glColor3ub = NULL;
PFNGLCOLOR3UBVPROC glColor3ubv = NULL;
PFNGLCOLOR3UIPROC glColor3ui = NULL;
PFNGLCOLOR3UIVPROC glColor3uiv = NULL;
PFNGLCOLOR3USPROC glColor3us = NULL;
PFNGLCOLOR3USVPROC glColor3usv = NULL;
PFNGLCOLOR4BPROC glColor4b = NULL;
PFNGLCOLOR4BVPROC glColor4bv = NULL;
PFNGLCOLOR4DPROC glColor4d = NULL;
PFNGLCOLOR4DVPROC glColor4dv = NULL;
PFNGLCOLOR4FPROC glColor4f = NULL;
PFNGLCOLOR4FVPROC glColor4fv = NULL;
PFNGLCOLOR4IPROC glColor4i = NULL;
PFNGLCOLOR4IVPROC glColor4iv = NULL;
PFNGLCOLOR4SPROC glColor4s = NULL;
PFNGLCOLOR4SVPROC glColor4sv = NULL;
PFNGLCOLOR4UBPROC glColor4ub = NULL;
PFNGLCOLOR4UBVPROC glColor4ubv = NULL;
PFNGLCOLOR4UIPROC glColor4ui = NULL;
PFNGLCOLOR4UIVPROC glColor4uiv = NULL;
PFNGLCOLOR4USPROC glColor4us = NULL;
PFNGLCOLOR4USVPROC glColor4usv = NULL;
PFNGLCOLORMASKPROC glColorMask = NULL;
PFNGLCOLORMATERIALPROC glColorMaterial = NULL;
PFNGLCOLORPOINTERPROC glColorPointer = NULL;
PFNGLCOPYPIXELSPROC glCopyPixels = NULL;
PFNGLCOPYTEXIMAGE1DPROC glCopyTexImage1D = NULL;
PFNGLCOPYTEXIMAGE2DPROC glCopyTexImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE1DPROC glCopyTexSubImage1D = NULL;
PFNGLCOPYTEXSUBIMAGE2DPROC glCopyTexSubImage2D = NULL;
PFNGLCULLFACEPROC glCullFace = NULL;
PFNGLDELETELISTSPROC glDeleteLists = NULL;
PFNGLDELETETEXTURESPROC glDeleteTextures = NULL;
PFNGLDEPTHFUNCPROC glDepthFunc = NULL;
PFNGLDEPTHMASKPROC glDepthMask = NULL;
PFNGLDEPTHRANGEPROC glDepthRange = NULL;
PFNGLDISABLEPROC glDisable = NULL;
PFNGLDISABLECLIENTSTATEPROC glDisableClientState = NULL;
PFNGLDRAWARRAYSPROC glDrawArrays = NULL;
PFNGLDRAWBUFFERPROC glDrawBuffer = NULL;
PFNGLDRAWELEMENTSPROC glDrawElements = NULL;
PFNGLDRAWPIXELSPROC glDrawPixels = NULL;
PFNGLEDGEFLAGPROC glEdgeFlag = NULL;
PFNGLEDGEFLAGPOINTERPROC glEdgeFlagPointer = NULL;
PFNGLEDGEFLAGVPROC glEdgeFlagv = NULL;
PFNGLENABLEPROC glEnable = NULL;
PFNGLENABLECLIENTSTATEPROC glEnableClientState = NULL;
PFNGLENDPROC glEnd = NULL;
PFNGLENDLISTPROC glEndList = NULL;
PFNGLEVALCOORD1DPROC glEvalCoord1d = NULL;
PFNGLEVALCOORD1DVPROC glEvalCoord1dv = NULL;
PFNGLEVALCOORD1FPROC glEvalCoord1f = NULL;
PFNGLEVALCOORD1FVPROC glEvalCoord1fv = NULL;
PFNGLEVALCOORD2DPROC glEvalCoord2d = NULL;
PFNGLEVALCOORD2DVPROC glEvalCoord2dv = NULL;
PFNGLEVALCOORD2FPROC glEvalCoord2f = NULL;
PFNGLEVALCOORD2FVPROC glEvalCoord2fv = NULL;
PFNGLEVALMESH1PROC glEvalMesh1 = NULL;
PFNGLEVALMESH2PROC glEvalMesh2 = NULL;
PFNGLEVALPOINT1PROC glEvalPoint1 = NULL;
PFNGLEVALPOINT2PROC glEvalPoint2 = NULL;
PFNGLFEEDBACKBUFFERPROC glFeedbackBuffer = NULL;
PFNGLFINISHPROC glFinish = NULL;
PFNGLFLUSHPROC glFlush = NULL;
PFNGLFOGFPROC glFogf = NULL;
PFNGLFOGFVPROC glFogfv = NULL;
PFNGLFOGIPROC glFogi = NULL;
PFNGLFOGIVPROC glFogiv = NULL;
PFNGLFRONTFACEPROC glFrontFace = NULL;
PFNGLFRUSTUMPROC glFrustum = NULL;
PFNGLGENLISTSPROC glGenLists = NULL;
PFNGLGENTEXTURESPROC glGenTextures = NULL;
PFNGLGETBOOLEANVPROC glGetBooleanv = NULL;
PFNGLGETCLIPPLANEPROC glGetClipPlane = NULL;
PFNGLGETDOUBLEVPROC glGetDoublev = NULL;
PFNGLGETERRORPROC glGetError = NULL;
PFNGLGETFLOATVPROC glGetFloatv = NULL;
PFNGLGETINTEGERVPROC glGetIntegerv = NULL;
PFNGLGETLIGHTFVPROC glGetLightfv = NULL;
PFNGLGETLIGHTIVPROC glGetLightiv = NULL;
PFNGLGETMAPDVPROC glGetMapdv = NULL;
PFNGLGETMAPFVPROC glGetMapfv = NULL;
PFNGLGETMAPIVPROC glGetMapiv = NULL;
PFNGLGETMATERIALFVPROC glGetMaterialfv = NULL;
PFNGLGETMATERIALIVPROC glGetMaterialiv = NULL;
PFNGLGETPIXELMAPFVPROC glGetPixelMapfv = NULL;
PFNGLGETPIXELMAPUIVPROC glGetPixelMapuiv = NULL;
PFNGLGETPIXELMAPUSVPROC glGetPixelMapusv = NULL;
PFNGLGETPOINTERVPROC glGetPointerv = NULL;
PFNGLGETPOLYGONSTIPPLEPROC glGetPolygonStipple = NULL;
PFNGLGETSTRINGPROC glGetString = NULL;
PFNGLGETTEXENVFVPROC glGetTexEnvfv = NULL;
PFNGLGETTEXENVIVPROC glGetTexEnviv = NULL;
PFNGLGETTEXGENDVPROC glGetTexGendv = NULL;
PFNGLGETTEXGENFVPROC glGetTexGenfv = NULL;
PFNGLGETTEXGENIVPROC glGetTexGeniv = NULL;
PFNGLGETTEXIMAGEPROC glGetTexImage = NULL;
PFNGLGETTEXLEVELPARAMETERFVPROC glGetTexLevelParameterfv = NULL;
PFNGLGETTEXLEVELPARAMETERIVPROC glGetTexLevelParameteriv = NULL;
PFNGLGETTEXPARAMETERFVPROC glGetTexParameterfv = NULL;
PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv = NULL;
PFNGLHINTPROC glHint = NULL;
PFNGLINDEXMASKPROC glIndexMask = NULL;
PFNGLINDEXPOINTERPROC glIndexPointer = NULL;
PFNGLINDEXDPROC glIndexd = NULL;
PFNGLINDEXDVPROC glIndexdv = NULL;
PFNGLINDEXFPROC glIndexf = NULL;
PFNGLINDEXFVPROC glIndexfv = NULL;
PFNGLINDEXIPROC glIndexi = NULL;
PFNGLINDEXIVPROC glIndexiv = NULL;
PFNGLINDEXSPROC glIndexs = NULL;
PFNGLINDEXSVPROC glIndexsv = NULL;
PFNGLINDEXUBPROC glIndexub = NULL;
PFNGLINDEXUBVPROC glIndexubv = NULL;
PFNGLINITNAMESPROC glInitNames = NULL;
PFNGLINTERLEAVEDARRAYSPROC glInterleavedArrays = NULL;
PFNGLISENABLEDPROC glIsEnabled = NULL;
PFNGLISLISTPROC glIsList = NULL;
PFNGLISTEXTUREPROC glIsTexture = NULL;
PFNGLLIGHTMODELFPROC glLightModelf = NULL;
PFNGLLIGHTMODELFVPROC glLightModelfv = NULL;
PFNGLLIGHTMODELIPROC glLightModeli = NULL;
PFNGLLIGHTMODELIVPROC glLightModeliv = NULL;
PFNGLLIGHTFPROC glLightf = NULL;
PFNGLLIGHTFVPROC glLightfv = NULL;
PFNGLLIGHTIPROC glLighti = NULL;
PFNGLLIGHTIVPROC glLightiv = NULL;
PFNGLLINESTIPPLEPROC glLineStipple = NULL;
PFNGLLINEWIDTHPROC glLineWidth = NULL;
PFNGLLISTBASEPROC glListBase = NULL;
PFNGLLOADIDENTITYPROC glLoadIdentity = NULL;
PFNGLLOADMATRIXDPROC glLoadMatrixd = NULL;
PFNGLLOADMATRIXFPROC glLoadMatrixf = NULL;
PFNGLLOADNAMEPROC glLoadName = NULL;
PFNGLLOGICOPPROC glLogicOp = NULL;
PFNGLMAP1DPROC glMap1d = NULL;
PFNGLMAP1FPROC glMap1f = NULL;
PFNGLMAP2DPROC glMap2d = NULL;
PFNGLMAP2FPROC glMap2f = NULL;
PFNGLMAPGRID1DPROC glMapGrid1d = NULL;
PFNGLMAPGRID1FPROC glMapGrid1f = NULL;
PFNGLMAPGRID2DPROC glMapGrid2d = NULL;
PFNGLMAPGRID2FPROC glMapGrid2f = NULL;
PFNGLMATERIALFPROC glMaterialf = NULL;
PFNGLMATERIALFVPROC glMaterialfv = NULL;
PFNGLMATERIALIPROC glMateriali = NULL;
PFNGLMATERIALIVPROC glMaterialiv = NULL;
PFNGLMATRIXMODEPROC glMatrixMode = NULL;
PFNGLMULTMATRIXDPROC glMultMatrixd = NULL;
PFNGLMULTMATRIXFPROC glMultMatrixf = NULL;
PFNGLNEWLISTPROC glNewList = NULL;
PFNGLNORMAL3BPROC glNormal3b = NULL;
PFNGLNORMAL3BVPROC glNormal3bv = NULL;
PFNGLNORMAL3DPROC glNormal3d = NULL;
PFNGLNORMAL3DVPROC glNormal3dv = NULL;
PFNGLNORMAL3FPROC glNormal3f = NULL;
PFNGLNORMAL3FVPROC glNormal3fv = NULL;
PFNGLNORMAL3IPROC glNormal3i = NULL;
PFNGLNORMAL3IVPROC glNormal3iv = NULL;
PFNGLNORMAL3SPROC glNormal3s = NULL;
PFNGLNORMAL3SVPROC glNormal3sv = NULL;
PFNGLNORMALPOINTERPROC glNormalPointer = NULL;
PFNGLORTHOPROC glOrtho = NULL;
PFNGLPASSTHROUGHPROC glPassThrough = NULL;
PFNGLPIXELMAPFVPROC glPixelMapfv = NULL;
PFNGLPIXELMAPUIVPROC glPixelMapuiv = NULL;
PFNGLPIXELMAPUSVPROC glPixelMapusv = NULL;
PFNGLPIXELSTOREFPROC glPixelStoref = NULL;
PFNGLPIXELSTOREIPROC glPixelStorei = NULL;
PFNGLPIXELTRANSFERFPROC glPixelTransferf = NULL;
PFNGLPIXELTRANSFERIPROC glPixelTransferi = NULL;
PFNGLPIXELZOOMPROC glPixelZoom = NULL;
PFNGLPOINTSIZEPROC glPointSize = NULL;
PFNGLPOLYGONMODEPROC glPolygonMode = NULL;
PFNGLPOLYGONOFFSETPROC glPolygonOffset = NULL;
PFNGLPOLYGONSTIPPLEPROC glPolygonStipple = NULL;
PFNGLPOPATTRIBPROC glPopAttrib = NULL;
PFNGLPOPCLIENTATTRIBPROC glPopClientAttrib = NULL;
PFNGLPOPMATRIXPROC glPopMatrix = NULL;
PFNGLPOPNAMEPROC glPopName = NULL;
PFNGLPRIORITIZETEXTURESPROC glPrioritizeTextures = NULL;
PFNGLPUSHATTRIBPROC glPushAttrib = NULL;
PFNGLPUSHCLIENTATTRIBPROC glPushClientAttrib = NULL;
PFNGLPUSHMATRIXPROC glPushMatrix = NULL;
PFNGLPUSHNAMEPROC glPushName = NULL;
PFNGLRASTERPOS2DPROC glRasterPos2d = NULL;
PFNGLRASTERPOS2DVPROC glRasterPos2dv = NULL;
PFNGLRASTERPOS2FPROC glRasterPos2f = NULL;
PFNGLRASTERPOS2FVPROC glRasterPos2fv = NULL;
PFNGLRASTERPOS2IPROC glRasterPos2i = NULL;
PFNGLRASTERPOS2IVPROC glRasterPos2iv = NULL;
PFNGLRASTERPOS2SPROC glRasterPos2s = NULL;
PFNGLRASTERPOS2SVPROC glRasterPos2sv = NULL;
PFNGLRASTERPOS3DPROC glRasterPos3d = NULL;
PFNGLRASTERPOS3DVPROC glRasterPos3dv = NULL;
PFNGLRASTERPOS3FPROC glRasterPos3f = NULL;
PFNGLRASTERPOS3FVPROC glRasterPos3fv = NULL;
PFNGLRASTERPOS3IPROC glRasterPos3i = NULL;
PFNGLRASTERPOS3IVPROC glRasterPos3iv = NULL;
PFNGLRASTERPOS3SPROC glRasterPos3s = NULL;
PFNGLRASTERPOS3SVPROC glRasterPos3sv = NULL;
PFNGLRASTERPOS4DPROC glRasterPos4d = NULL;
PFNGLRASTERPOS4DVPROC glRasterPos4dv = NULL;
PFNGLRASTERPOS4FPROC glRasterPos4f = NULL;
PFNGLRASTERPOS4FVPROC glRasterPos4fv = NULL;
PFNGLRASTERPOS4IPROC glRasterPos4i = NULL;
PFNGLRASTERPOS4IVPROC glRasterPos4iv = NULL;
PFNGLRASTERPOS4SPROC glRasterPos4s = NULL;
PFNGLRASTERPOS4SVPROC glRasterPos4sv = NULL;
PFNGLREADBUFFERPROC glReadBuffer = NULL;
PFNGLREADPIXELSPROC glReadPixels = NULL;
PFNGLRECTDPROC glRectd = NULL;
PFNGLRECTDVPROC glRectdv = NULL;
PFNGLRECTFPROC glRectf = NULL;
PFNGLRECTFVPROC glRectfv = NULL;
PFNGLRECTIPROC glRecti = NULL;
PFNGLRECTIVPROC glRectiv = NULL;
PFNGLRECTSPROC glRects = NULL;
PFNGLRECTSVPROC glRectsv = NULL;
PFNGLRENDERMODEPROC glRenderMode = NULL;
PFNGLROTATEDPROC glRotated = NULL;
PFNGLROTATEFPROC glRotatef = NULL;
PFNGLSCALEDPROC glScaled = NULL;
PFNGLSCALEFPROC glScalef = NULL;
PFNGLSCISSORPROC glScissor = NULL;
PFNGLSELECTBUFFERPROC glSelectBuffer = NULL;
PFNGLSHADEMODELPROC glShadeModel = NULL;
PFNGLSTENCILFUNCPROC glStencilFunc = NULL;
PFNGLSTENCILMASKPROC glStencilMask = NULL;
PFNGLSTENCILOPPROC glStencilOp = NULL;
PFNGLTEXCOORD1DPROC glTexCoord1d = NULL;
PFNGLTEXCOORD1DVPROC glTexCoord1dv = NULL;
PFNGLTEXCOORD1FPROC glTexCoord1f = NULL;
PFNGLTEXCOORD1FVPROC glTexCoord1fv = NULL;
PFNGLTEXCOORD1IPROC glTexCoord1i = NULL;
PFNGLTEXCOORD1IVPROC glTexCoord1iv = NULL;
PFNGLTEXCOORD1SPROC glTexCoord1s = NULL;
PFNGLTEXCOORD1SVPROC glTexCoord1sv = NULL;
PFNGLTEXCOORD2DPROC glTexCoord2d = NULL;
PFNGLTEXCOORD2DVPROC glTexCoord2dv = NULL;
PFNGLTEXCOORD2FPROC glTexCoord2f = NULL;
PFNGLTEXCOORD2FVPROC glTexCoord2fv = NULL;
PFNGLTEXCOORD2IPROC glTexCoord2i = NULL;
PFNGLTEXCOORD2IVPROC glTexCoord2iv = NULL;
PFNGLTEXCOORD2SPROC glTexCoord2s = NULL;
PFNGLTEXCOORD2SVPROC glTexCoord2sv = NULL;
PFNGLTEXCOORD3DPROC glTexCoord3d = NULL;
PFNGLTEXCOORD3DVPROC glTexCoord3dv = NULL;
PFNGLTEXCOORD3FPROC glTexCoord3f = NULL;
PFNGLTEXCOORD3FVPROC glTexCoord3fv = NULL;
PFNGLTEXCOORD3IPROC glTexCoord3i = NULL;
PFNGLTEXCOORD3IVPROC glTexCoord3iv = NULL;
PFNGLTEXCOORD3SPROC glTexCoord3s = NULL;
PFNGLTEXCOORD3SVPROC glTexCoord3sv = NULL;
PFNGLTEXCOORD4DPROC glTexCoord4d = NULL;
PFNGLTEXCOORD4DVPROC glTexCoord4dv = NULL;
PFNGLTEXCOORD4FPROC glTexCoord4f = NULL;
PFNGLTEXCOORD4FVPROC glTexCoord4fv = NULL;
PFNGLTEXCOORD4IPROC glTexCoord4i = NULL;
PFNGLTEXCOORD4IVPROC glTexCoord4iv = NULL;
PFNGLTEXCOORD4SPROC glTexCoord4s = NULL;
PFNGLTEXCOORD4SVPROC glTexCoord4sv = NULL;
PFNGLTEXCOORDPOINTERPROC glTexCoordPointer = NULL;
PFNGLTEXENVFPROC glTexEnvf = NULL;
PFNGLTEXENVFVPROC glTexEnvfv = NULL;
PFNGLTEXENVIPROC glTexEnvi = NULL;
PFNGLTEXENVIVPROC glTexEnviv = NULL;
PFNGLTEXGENDPROC glTexGend = NULL;
PFNGLTEXGENDVPROC glTexGendv = NULL;
PFNGLTEXGENFPROC glTexGenf = NULL;
PFNGLTEXGENFVPROC glTexGenfv = NULL;
PFNGLTEXGENIPROC glTexGeni = NULL;
PFNGLTEXGENIVPROC glTexGeniv = NULL;
PFNGLTEXIMAGE1DPROC glTexImage1D = NULL;
PFNGLTEXIMAGE2DPROC glTexImage2D = NULL;
PFNGLTEXPARAMETERFPROC glTexParameterf = NULL;
PFNGLTEXPARAMETERFVPROC glTexParameterfv = NULL;
PFNGLTEXPARAMETERIPROC glTexParameteri = NULL;
PFNGLTEXPARAMETERIVPROC glTexParameteriv = NULL;
PFNGLTEXSUBIMAGE1DPROC glTexSubImage1D = NULL;
PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D = NULL;
PFNGLTRANSLATEDPROC glTranslated = NULL;
PFNGLTRANSLATEFPROC glTranslatef = NULL;
PFNGLVERTEX2DPROC glVertex2d = NULL;
PFNGLVERTEX2DVPROC glVertex2dv = NULL;
PFNGLVERTEX2FPROC glVertex2f = NULL;
PFNGLVERTEX2FVPROC glVertex2fv = NULL;
PFNGLVERTEX2IPROC glVertex2i = NULL;
PFNGLVERTEX2IVPROC glVertex2iv = NULL;
PFNGLVERTEX2SPROC glVertex2s = NULL;
PFNGLVERTEX2SVPROC glVertex2sv = NULL;
PFNGLVERTEX3DPROC glVertex3d = NULL;
PFNGLVERTEX3DVPROC glVertex3dv = NULL;
PFNGLVERTEX3FPROC glVertex3f = NULL;
PFNGLVERTEX3FVPROC glVertex3fv = NULL;
PFNGLVERTEX3IPROC glVertex3i = NULL;
PFNGLVERTEX3IVPROC glVertex3iv = NULL;
PFNGLVERTEX3SPROC glVertex3s = NULL;
PFNGLVERTEX3SVPROC glVertex3sv = NULL;
PFNGLVERTEX4DPROC glVertex4d = NULL;
PFNGLVERTEX4DVPROC glVertex4dv = NULL;
PFNGLVERTEX4FPROC glVertex4f = NULL;
PFNGLVERTEX4FVPROC glVertex4fv = NULL;
PFNGLVERTEX4IPROC glVertex4i = NULL;
PFNGLVERTEX4IVPROC glVertex4iv = NULL;
PFNGLVERTEX4SPROC glVertex4s = NULL;
PFNGLVERTEX4SVPROC glVertex4sv = NULL;
PFNGLVERTEXPOINTERPROC glVertexPointer = NULL;
PFNGLVIEWPORTPROC glViewport = NULL;

PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D = NULL;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;
PFNGLTEXIMAGE3DPROC glTexImage3D = NULL;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = NULL;

PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = NULL;
PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd = NULL;
PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf = NULL;
PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd = NULL;
PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf = NULL;
PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d = NULL;
PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv = NULL;
PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f = NULL;
PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv = NULL;
PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i = NULL;
PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv = NULL;
PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s = NULL;
PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv = NULL;
PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d = NULL;
PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv = NULL;
PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i = NULL;
PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv = NULL;
PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s = NULL;
PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv = NULL;
PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d = NULL;
PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv = NULL;
PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f = NULL;
PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv = NULL;
PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i = NULL;
PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv = NULL;
PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s = NULL;
PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv = NULL;
PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d = NULL;
PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv = NULL;
PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f = NULL;
PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv = NULL;
PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i = NULL;
PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv = NULL;
PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s = NULL;
PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv = NULL;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = NULL;

PFNGLBLENDCOLORPROC glBlendColor = NULL;
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = NULL;
PFNGLFOGCOORDPOINTERPROC glFogCoordPointer = NULL;
PFNGLFOGCOORDDPROC glFogCoordd = NULL;
PFNGLFOGCOORDDVPROC glFogCoorddv = NULL;
PFNGLFOGCOORDFPROC glFogCoordf = NULL;
PFNGLFOGCOORDFVPROC glFogCoordfv = NULL;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements = NULL;
PFNGLPOINTPARAMETERFPROC glPointParameterf = NULL;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = NULL;
PFNGLPOINTPARAMETERIPROC glPointParameteri = NULL;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv = NULL;
PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b = NULL;
PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv = NULL;
PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d = NULL;
PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv = NULL;
PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f = NULL;
PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv = NULL;
PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i = NULL;
PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv = NULL;
PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s = NULL;
PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv = NULL;
PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub = NULL;
PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv = NULL;
PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui = NULL;
PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv = NULL;
PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us = NULL;
PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv = NULL;
PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer = NULL;
PFNGLWINDOWPOS2DPROC glWindowPos2d = NULL;
PFNGLWINDOWPOS2DVPROC glWindowPos2dv = NULL;
PFNGLWINDOWPOS2FPROC glWindowPos2f = NULL;
PFNGLWINDOWPOS2FVPROC glWindowPos2fv = NULL;
PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;
PFNGLWINDOWPOS2IVPROC glWindowPos2iv = NULL;
PFNGLWINDOWPOS2SPROC glWindowPos2s = NULL;
PFNGLWINDOWPOS2SVPROC glWindowPos2sv = NULL;
PFNGLWINDOWPOS3DPROC glWindowPos3d = NULL;
PFNGLWINDOWPOS3DVPROC glWindowPos3dv = NULL;
PFNGLWINDOWPOS3FPROC glWindowPos3f = NULL;
PFNGLWINDOWPOS3FVPROC glWindowPos3fv = NULL;
PFNGLWINDOWPOS3IPROC glWindowPos3i = NULL;
PFNGLWINDOWPOS3IVPROC glWindowPos3iv = NULL;
PFNGLWINDOWPOS3SPROC glWindowPos3s = NULL;
PFNGLWINDOWPOS3SVPROC glWindowPos3sv = NULL;

PFNGLBEGINQUERYPROC glBeginQuery = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLDELETEQUERIESPROC glDeleteQueries = NULL;
PFNGLENDQUERYPROC glEndQuery = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLGENQUERIESPROC glGenQueries = NULL;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv = NULL;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = NULL;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = NULL;
PFNGLGETQUERYIVPROC glGetQueryiv = NULL;
PFNGLISBUFFERPROC glIsBuffer = NULL;
PFNGLISQUERYPROC glIsQuery = NULL;
PFNGLMAPBUFFERPROC glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = NULL;

PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLDRAWBUFFERSPROC glDrawBuffers = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = NULL;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLGETUNIFORMFVPROC glGetUniformfv = NULL;
PFNGLGETUNIFORMIVPROC glGetUniformiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = NULL;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = NULL;
PFNGLISPROGRAMPROC glIsProgram = NULL;
PFNGLISSHADERPROC glIsShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = NULL;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = NULL;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
PFNGLUNIFORM2FPROC glUniform2f = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
PFNGLUNIFORM2IPROC glUniform2i = NULL;
PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM3IPROC glUniform3i = NULL;
PFNGLUNIFORM3IVPROC glUniform3iv = NULL;
PFNGLUNIFORM4FPROC glUniform4f = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORM4IPROC glUniform4i = NULL;
PFNGLUNIFORM4IVPROC glUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = NULL;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;

PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv = NULL;

PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback = NULL;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = NULL;
PFNGLCLAMPCOLORPROC glClampColor = NULL;
PFNGLCLEARBUFFERFIPROC glClearBufferfi = NULL;
PFNGLCLEARBUFFERFVPROC glClearBufferfv = NULL;
PFNGLCLEARBUFFERIVPROC glClearBufferiv = NULL;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv = NULL;
PFNGLCOLORMASKIPROC glColorMaski = NULL;
PFNGLDISABLEIPROC glDisablei = NULL;
PFNGLENABLEIPROC glEnablei = NULL;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback = NULL;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v = NULL;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation = NULL;
PFNGLGETSTRINGIPROC glGetStringi = NULL;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying = NULL;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv = NULL;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv = NULL;
PFNGLISENABLEDIPROC glIsEnabledi = NULL;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings = NULL;
PFNGLUNIFORM1UIPROC glUniform1ui = NULL;
PFNGLUNIFORM1UIVPROC glUniform1uiv = NULL;
PFNGLUNIFORM2UIPROC glUniform2ui = NULL;
PFNGLUNIFORM2UIVPROC glUniform2uiv = NULL;
PFNGLUNIFORM3UIPROC glUniform3ui = NULL;
PFNGLUNIFORM3UIVPROC glUniform3uiv = NULL;
PFNGLUNIFORM4UIPROC glUniform4ui = NULL;
PFNGLUNIFORM4UIVPROC glUniform4uiv = NULL;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = NULL;

PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = NULL;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex = NULL;
PFNGLTEXBUFFERPROC glTexBuffer = NULL;

PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = NULL;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v = NULL;
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v = NULL;

PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = NULL;

PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei = NULL;
PFNGLBLENDEQUATIONIPROC glBlendEquationi = NULL;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei = NULL;
PFNGLBLENDFUNCIPROC glBlendFunci = NULL;
PFNGLMINSAMPLESHADINGPROC glMinSampleShading = NULL;

PFNGLTBUFFERMASK3DFXPROC glTbufferMask3DFX = NULL;

PFNGLDEBUGMESSAGECALLBACKAMDPROC glDebugMessageCallbackAMD = NULL;
PFNGLDEBUGMESSAGEENABLEAMDPROC glDebugMessageEnableAMD = NULL;
PFNGLDEBUGMESSAGEINSERTAMDPROC glDebugMessageInsertAMD = NULL;
PFNGLGETDEBUGMESSAGELOGAMDPROC glGetDebugMessageLogAMD = NULL;

PFNGLBLENDEQUATIONINDEXEDAMDPROC glBlendEquationIndexedAMD = NULL;
PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC glBlendEquationSeparateIndexedAMD = NULL;
PFNGLBLENDFUNCINDEXEDAMDPROC glBlendFuncIndexedAMD = NULL;
PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC glBlendFuncSeparateIndexedAMD = NULL;

PFNGLDELETENAMESAMDPROC glDeleteNamesAMD = NULL;
PFNGLGENNAMESAMDPROC glGenNamesAMD = NULL;
PFNGLISNAMEAMDPROC glIsNameAMD = NULL;

PFNGLBEGINPERFMONITORAMDPROC glBeginPerfMonitorAMD = NULL;
PFNGLDELETEPERFMONITORSAMDPROC glDeletePerfMonitorsAMD = NULL;
PFNGLENDPERFMONITORAMDPROC glEndPerfMonitorAMD = NULL;
PFNGLGENPERFMONITORSAMDPROC glGenPerfMonitorsAMD = NULL;
PFNGLGETPERFMONITORCOUNTERDATAAMDPROC glGetPerfMonitorCounterDataAMD = NULL;
PFNGLGETPERFMONITORCOUNTERINFOAMDPROC glGetPerfMonitorCounterInfoAMD = NULL;
PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC glGetPerfMonitorCounterStringAMD = NULL;
PFNGLGETPERFMONITORCOUNTERSAMDPROC glGetPerfMonitorCountersAMD = NULL;
PFNGLGETPERFMONITORGROUPSTRINGAMDPROC glGetPerfMonitorGroupStringAMD = NULL;
PFNGLGETPERFMONITORGROUPSAMDPROC glGetPerfMonitorGroupsAMD = NULL;
PFNGLSELECTPERFMONITORCOUNTERSAMDPROC glSelectPerfMonitorCountersAMD = NULL;

PFNGLTESSELLATIONFACTORAMDPROC glTessellationFactorAMD = NULL;
PFNGLTESSELLATIONMODEAMDPROC glTessellationModeAMD = NULL;

PFNGLDRAWELEMENTARRAYAPPLEPROC glDrawElementArrayAPPLE = NULL;
PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC glDrawRangeElementArrayAPPLE = NULL;
PFNGLELEMENTPOINTERAPPLEPROC glElementPointerAPPLE = NULL;
PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC glMultiDrawElementArrayAPPLE = NULL;
PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC glMultiDrawRangeElementArrayAPPLE = NULL;

PFNGLDELETEFENCESAPPLEPROC glDeleteFencesAPPLE = NULL;
PFNGLFINISHFENCEAPPLEPROC glFinishFenceAPPLE = NULL;
PFNGLFINISHOBJECTAPPLEPROC glFinishObjectAPPLE = NULL;
PFNGLGENFENCESAPPLEPROC glGenFencesAPPLE = NULL;
PFNGLISFENCEAPPLEPROC glIsFenceAPPLE = NULL;
PFNGLSETFENCEAPPLEPROC glSetFenceAPPLE = NULL;
PFNGLTESTFENCEAPPLEPROC glTestFenceAPPLE = NULL;
PFNGLTESTOBJECTAPPLEPROC glTestObjectAPPLE = NULL;

PFNGLBUFFERPARAMETERIAPPLEPROC glBufferParameteriAPPLE = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC glFlushMappedBufferRangeAPPLE = NULL;

PFNGLGETOBJECTPARAMETERIVAPPLEPROC glGetObjectParameterivAPPLE = NULL;
PFNGLOBJECTPURGEABLEAPPLEPROC glObjectPurgeableAPPLE = NULL;
PFNGLOBJECTUNPURGEABLEAPPLEPROC glObjectUnpurgeableAPPLE = NULL;

PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC glGetTexParameterPointervAPPLE = NULL;
PFNGLTEXTURERANGEAPPLEPROC glTextureRangeAPPLE = NULL;

PFNGLBINDVERTEXARRAYAPPLEPROC glBindVertexArrayAPPLE = NULL;
PFNGLDELETEVERTEXARRAYSAPPLEPROC glDeleteVertexArraysAPPLE = NULL;
PFNGLGENVERTEXARRAYSAPPLEPROC glGenVertexArraysAPPLE = NULL;
PFNGLISVERTEXARRAYAPPLEPROC glIsVertexArrayAPPLE = NULL;

PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC glFlushVertexArrayRangeAPPLE = NULL;
PFNGLVERTEXARRAYPARAMETERIAPPLEPROC glVertexArrayParameteriAPPLE = NULL;
PFNGLVERTEXARRAYRANGEAPPLEPROC glVertexArrayRangeAPPLE = NULL;

PFNGLDISABLEVERTEXATTRIBAPPLEPROC glDisableVertexAttribAPPLE = NULL;
PFNGLENABLEVERTEXATTRIBAPPLEPROC glEnableVertexAttribAPPLE = NULL;
PFNGLISVERTEXATTRIBENABLEDAPPLEPROC glIsVertexAttribEnabledAPPLE = NULL;
PFNGLMAPVERTEXATTRIB1DAPPLEPROC glMapVertexAttrib1dAPPLE = NULL;
PFNGLMAPVERTEXATTRIB1FAPPLEPROC glMapVertexAttrib1fAPPLE = NULL;
PFNGLMAPVERTEXATTRIB2DAPPLEPROC glMapVertexAttrib2dAPPLE = NULL;
PFNGLMAPVERTEXATTRIB2FAPPLEPROC glMapVertexAttrib2fAPPLE = NULL;

PFNGLCLEARDEPTHFPROC glClearDepthf = NULL;
PFNGLDEPTHRANGEFPROC glDepthRangef = NULL;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat = NULL;
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler = NULL;
PFNGLSHADERBINARYPROC glShaderBinary = NULL;

PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed = NULL;
PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex = NULL;

PFNGLCREATESYNCFROMCLEVENTARBPROC glCreateSyncFromCLeventARB = NULL;

PFNGLCLAMPCOLORARBPROC glClampColorARB = NULL;

PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = NULL;

PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB = NULL;
PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB = NULL;
PFNGLDEBUGMESSAGEINSERTARBPROC glDebugMessageInsertARB = NULL;
PFNGLGETDEBUGMESSAGELOGARBPROC glGetDebugMessageLogARB = NULL;

PFNGLDRAWBUFFERSARBPROC glDrawBuffersARB = NULL;

PFNGLBLENDEQUATIONSEPARATEIARBPROC glBlendEquationSeparateiARB = NULL;
PFNGLBLENDEQUATIONIARBPROC glBlendEquationiARB = NULL;
PFNGLBLENDFUNCSEPARATEIARBPROC glBlendFuncSeparateiARB = NULL;
PFNGLBLENDFUNCIARBPROC glBlendFunciARB = NULL;

PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex = NULL;

PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect = NULL;
PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect = NULL;

PFNGLDRAWARRAYSINSTANCEDARBPROC glDrawArraysInstancedARB = NULL;
PFNGLDRAWELEMENTSINSTANCEDARBPROC glDrawElementsInstancedARB = NULL;

PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = NULL;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer = NULL;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv = NULL;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer = NULL;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = NULL;

PFNGLFRAMEBUFFERTEXTUREARBPROC glFramebufferTextureARB = NULL;
PFNGLFRAMEBUFFERTEXTUREFACEARBPROC glFramebufferTextureFaceARB = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERARBPROC glFramebufferTextureLayerARB = NULL;
PFNGLPROGRAMPARAMETERIARBPROC glProgramParameteriARB = NULL;

PFNGLGETPROGRAMBINARYPROC glGetProgramBinary = NULL;
PFNGLPROGRAMBINARYPROC glProgramBinary = NULL;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = NULL;

PFNGLGETUNIFORMDVPROC glGetUniformdv = NULL;
PFNGLPROGRAMUNIFORM1DEXTPROC glProgramUniform1dEXT = NULL;
PFNGLPROGRAMUNIFORM1DVEXTPROC glProgramUniform1dvEXT = NULL;
PFNGLPROGRAMUNIFORM2DEXTPROC glProgramUniform2dEXT = NULL;
PFNGLPROGRAMUNIFORM2DVEXTPROC glProgramUniform2dvEXT = NULL;
PFNGLPROGRAMUNIFORM3DEXTPROC glProgramUniform3dEXT = NULL;
PFNGLPROGRAMUNIFORM3DVEXTPROC glProgramUniform3dvEXT = NULL;
PFNGLPROGRAMUNIFORM4DEXTPROC glProgramUniform4dEXT = NULL;
PFNGLPROGRAMUNIFORM4DVEXTPROC glProgramUniform4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC glProgramUniformMatrix2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC glProgramUniformMatrix2x3dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC glProgramUniformMatrix2x4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC glProgramUniformMatrix3dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC glProgramUniformMatrix3x2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC glProgramUniformMatrix3x4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC glProgramUniformMatrix4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC glProgramUniformMatrix4x2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC glProgramUniformMatrix4x3dvEXT = NULL;
PFNGLUNIFORM1DPROC glUniform1d = NULL;
PFNGLUNIFORM1DVPROC glUniform1dv = NULL;
PFNGLUNIFORM2DPROC glUniform2d = NULL;
PFNGLUNIFORM2DVPROC glUniform2dv = NULL;
PFNGLUNIFORM3DPROC glUniform3d = NULL;
PFNGLUNIFORM3DVPROC glUniform3dv = NULL;
PFNGLUNIFORM4DPROC glUniform4d = NULL;
PFNGLUNIFORM4DVPROC glUniform4dv = NULL;
PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv = NULL;
PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv = NULL;
PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv = NULL;
PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv = NULL;
PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv = NULL;
PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv = NULL;
PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv = NULL;
PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv = NULL;
PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv = NULL;

PFNGLCOLORSUBTABLEPROC glColorSubTable = NULL;
PFNGLCOLORTABLEPROC glColorTable = NULL;
PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv = NULL;
PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv = NULL;
PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D = NULL;
PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D = NULL;
PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf = NULL;
PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv = NULL;
PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri = NULL;
PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv = NULL;
PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable = NULL;
PFNGLCOPYCOLORTABLEPROC glCopyColorTable = NULL;
PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D = NULL;
PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D = NULL;
PFNGLGETCOLORTABLEPROC glGetColorTable = NULL;
PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv = NULL;
PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv = NULL;
PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter = NULL;
PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv = NULL;
PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv = NULL;
PFNGLGETHISTOGRAMPROC glGetHistogram = NULL;
PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv = NULL;
PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv = NULL;
PFNGLGETMINMAXPROC glGetMinmax = NULL;
PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv = NULL;
PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv = NULL;
PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter = NULL;
PFNGLHISTOGRAMPROC glHistogram = NULL;
PFNGLMINMAXPROC glMinmax = NULL;
PFNGLRESETHISTOGRAMPROC glResetHistogram = NULL;
PFNGLRESETMINMAXPROC glResetMinmax = NULL;
PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D = NULL;

PFNGLVERTEXATTRIBDIVISORARBPROC glVertexAttribDivisorARB = NULL;

PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = NULL;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = NULL;

PFNGLCURRENTPALETTEMATRIXARBPROC glCurrentPaletteMatrixARB = NULL;
PFNGLMATRIXINDEXPOINTERARBPROC glMatrixIndexPointerARB = NULL;
PFNGLMATRIXINDEXUBVARBPROC glMatrixIndexubvARB = NULL;
PFNGLMATRIXINDEXUIVARBPROC glMatrixIndexuivARB = NULL;
PFNGLMATRIXINDEXUSVARBPROC glMatrixIndexusvARB = NULL;

PFNGLSAMPLECOVERAGEARBPROC glSampleCoverageARB = NULL;

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1dARB = NULL;
PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB = NULL;
PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB = NULL;
PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB = NULL;
PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1iARB = NULL;
PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB = NULL;
PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1sARB = NULL;
PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB = NULL;
PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB = NULL;
PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;
PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB = NULL;
PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB = NULL;
PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB = NULL;
PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2sARB = NULL;
PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB = NULL;
PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB = NULL;
PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB = NULL;
PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB = NULL;
PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB = NULL;
PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3iARB = NULL;
PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB = NULL;
PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3sARB = NULL;
PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB = NULL;
PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4dARB = NULL;
PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB = NULL;
PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB = NULL;
PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB = NULL;
PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4iARB = NULL;
PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB = NULL;
PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4sARB = NULL;
PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB = NULL;

PFNGLBEGINQUERYARBPROC glBeginQueryARB = NULL;
PFNGLDELETEQUERIESARBPROC glDeleteQueriesARB = NULL;
PFNGLENDQUERYARBPROC glEndQueryARB = NULL;
PFNGLGENQUERIESARBPROC glGenQueriesARB = NULL;
PFNGLGETQUERYOBJECTIVARBPROC glGetQueryObjectivARB = NULL;
PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB = NULL;
PFNGLGETQUERYIVARBPROC glGetQueryivARB = NULL;
PFNGLISQUERYARBPROC glIsQueryARB = NULL;

PFNGLPOINTPARAMETERFARBPROC glPointParameterfARB = NULL;
PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB = NULL;

PFNGLPROVOKINGVERTEXPROC glProvokingVertex = NULL;

PFNGLGETNCOLORTABLEARBPROC glGetnColorTableARB = NULL;
PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC glGetnCompressedTexImageARB = NULL;
PFNGLGETNCONVOLUTIONFILTERARBPROC glGetnConvolutionFilterARB = NULL;
PFNGLGETNHISTOGRAMARBPROC glGetnHistogramARB = NULL;
PFNGLGETNMAPDVARBPROC glGetnMapdvARB = NULL;
PFNGLGETNMAPFVARBPROC glGetnMapfvARB = NULL;
PFNGLGETNMAPIVARBPROC glGetnMapivARB = NULL;
PFNGLGETNMINMAXARBPROC glGetnMinmaxARB = NULL;
PFNGLGETNPIXELMAPFVARBPROC glGetnPixelMapfvARB = NULL;
PFNGLGETNPIXELMAPUIVARBPROC glGetnPixelMapuivARB = NULL;
PFNGLGETNPIXELMAPUSVARBPROC glGetnPixelMapusvARB = NULL;
PFNGLGETNPOLYGONSTIPPLEARBPROC glGetnPolygonStippleARB = NULL;
PFNGLGETNSEPARABLEFILTERARBPROC glGetnSeparableFilterARB = NULL;
PFNGLGETNTEXIMAGEARBPROC glGetnTexImageARB = NULL;
PFNGLGETNUNIFORMDVARBPROC glGetnUniformdvARB = NULL;
PFNGLGETNUNIFORMFVARBPROC glGetnUniformfvARB = NULL;
PFNGLGETNUNIFORMIVARBPROC glGetnUniformivARB = NULL;
PFNGLGETNUNIFORMUIVARBPROC glGetnUniformuivARB = NULL;
PFNGLREADNPIXELSARBPROC glReadnPixelsARB = NULL;

PFNGLMINSAMPLESHADINGARBPROC glMinSampleShadingARB = NULL;

PFNGLBINDSAMPLERPROC glBindSampler = NULL;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = NULL;
PFNGLGENSAMPLERSPROC glGenSamplers = NULL;
PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv = NULL;
PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv = NULL;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv = NULL;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv = NULL;
PFNGLISSAMPLERPROC glIsSampler = NULL;
PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv = NULL;
PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv = NULL;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = NULL;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = NULL;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv = NULL;

PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram = NULL;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = NULL;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv = NULL;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = NULL;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines = NULL;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog = NULL;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv = NULL;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline = NULL;
PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d = NULL;
PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv = NULL;
PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f = NULL;
PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv = NULL;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i = NULL;
PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv = NULL;
PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui = NULL;
PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv = NULL;
PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d = NULL;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv = NULL;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f = NULL;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv = NULL;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i = NULL;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv = NULL;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui = NULL;
PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv = NULL;
PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d = NULL;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv = NULL;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f = NULL;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv = NULL;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i = NULL;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv = NULL;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui = NULL;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv = NULL;
PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d = NULL;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv = NULL;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f = NULL;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv = NULL;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i = NULL;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv = NULL;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui = NULL;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv = NULL;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = NULL;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline = NULL;

PFNGLATTACHOBJECTARBPROC glAttachObjectARB = NULL;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = NULL;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB = NULL;
PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB = NULL;
PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB = NULL;
PFNGLGETHANDLEARBPROC glGetHandleARB = NULL;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB = NULL;
PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB = NULL;
PFNGLGETUNIFORMIVARBPROC glGetUniformivARB = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = NULL;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = NULL;
PFNGLUNIFORM1FARBPROC glUniform1fARB = NULL;
PFNGLUNIFORM1FVARBPROC glUniform1fvARB = NULL;
PFNGLUNIFORM1IARBPROC glUniform1iARB = NULL;
PFNGLUNIFORM1IVARBPROC glUniform1ivARB = NULL;
PFNGLUNIFORM2FARBPROC glUniform2fARB = NULL;
PFNGLUNIFORM2FVARBPROC glUniform2fvARB = NULL;
PFNGLUNIFORM2IARBPROC glUniform2iARB = NULL;
PFNGLUNIFORM2IVARBPROC glUniform2ivARB = NULL;
PFNGLUNIFORM3FARBPROC glUniform3fARB = NULL;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB = NULL;
PFNGLUNIFORM3IARBPROC glUniform3iARB = NULL;
PFNGLUNIFORM3IVARBPROC glUniform3ivARB = NULL;
PFNGLUNIFORM4FARBPROC glUniform4fARB = NULL;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB = NULL;
PFNGLUNIFORM4IARBPROC glUniform4iARB = NULL;
PFNGLUNIFORM4IVARBPROC glUniform4ivARB = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = NULL;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB = NULL;

PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv = NULL;
PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv = NULL;
PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex = NULL;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation = NULL;
PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv = NULL;
PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv = NULL;

PFNGLCOMPILESHADERINCLUDEARBPROC glCompileShaderIncludeARB = NULL;
PFNGLDELETENAMEDSTRINGARBPROC glDeleteNamedStringARB = NULL;
PFNGLGETNAMEDSTRINGARBPROC glGetNamedStringARB = NULL;
PFNGLGETNAMEDSTRINGIVARBPROC glGetNamedStringivARB = NULL;
PFNGLISNAMEDSTRINGARBPROC glIsNamedStringARB = NULL;
PFNGLNAMEDSTRINGARBPROC glNamedStringARB = NULL;

PFNGLCLIENTWAITSYNCPROC glClientWaitSync = NULL;
PFNGLDELETESYNCPROC glDeleteSync = NULL;
PFNGLFENCESYNCPROC glFenceSync = NULL;
PFNGLGETINTEGER64VPROC glGetInteger64v = NULL;
PFNGLGETSYNCIVPROC glGetSynciv = NULL;
PFNGLISSYNCPROC glIsSync = NULL;
PFNGLWAITSYNCPROC glWaitSync = NULL;

PFNGLPATCHPARAMETERFVPROC glPatchParameterfv = NULL;
PFNGLPATCHPARAMETERIPROC glPatchParameteri = NULL;

PFNGLTEXBUFFERARBPROC glTexBufferARB = NULL;

PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1DARB = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3DARB = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImageARB = NULL;

PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv = NULL;
PFNGLSAMPLEMASKIPROC glSampleMaski = NULL;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample = NULL;

PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v = NULL;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v = NULL;
PFNGLQUERYCOUNTERPROC glQueryCounter = NULL;

PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback = NULL;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks = NULL;
PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback = NULL;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks = NULL;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback = NULL;
PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback = NULL;
PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback = NULL;

PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed = NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream = NULL;
PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed = NULL;
PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv = NULL;

PFNGLLOADTRANSPOSEMATRIXDARBPROC glLoadTransposeMatrixdARB = NULL;
PFNGLLOADTRANSPOSEMATRIXFARBPROC glLoadTransposeMatrixfARB = NULL;
PFNGLMULTTRANSPOSEMATRIXDARBPROC glMultTransposeMatrixdARB = NULL;
PFNGLMULTTRANSPOSEMATRIXFARBPROC glMultTransposeMatrixfARB = NULL;

PFNGLBINDBUFFERBASEPROC glBindBufferBase = NULL;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = NULL;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = NULL;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = NULL;

PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYPROC glIsVertexArray = NULL;

PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv = NULL;
PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d = NULL;
PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv = NULL;
PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d = NULL;
PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv = NULL;
PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d = NULL;
PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv = NULL;
PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d = NULL;
PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv = NULL;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer = NULL;

PFNGLVERTEXBLENDARBPROC glVertexBlendARB = NULL;
PFNGLWEIGHTPOINTERARBPROC glWeightPointerARB = NULL;
PFNGLWEIGHTBVARBPROC glWeightbvARB = NULL;
PFNGLWEIGHTDVARBPROC glWeightdvARB = NULL;
PFNGLWEIGHTFVARBPROC glWeightfvARB = NULL;
PFNGLWEIGHTIVARBPROC glWeightivARB = NULL;
PFNGLWEIGHTSVARBPROC glWeightsvARB = NULL;
PFNGLWEIGHTUBVARBPROC glWeightubvARB = NULL;
PFNGLWEIGHTUIVARBPROC glWeightuivARB = NULL;
PFNGLWEIGHTUSVARBPROC glWeightusvARB = NULL;

PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB = NULL;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB = NULL;
PFNGLISBUFFERARBPROC glIsBufferARB = NULL;
PFNGLMAPBUFFERARBPROC glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = NULL;

PFNGLBINDPROGRAMARBPROC glBindProgramARB = NULL;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB = NULL;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB = NULL;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB = NULL;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB = NULL;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB = NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB = NULL;
PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB = NULL;
PFNGLGETPROGRAMIVARBPROC glGetProgramivARB = NULL;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB = NULL;
PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB = NULL;
PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB = NULL;
PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB = NULL;
PFNGLISPROGRAMARBPROC glIsProgramARB = NULL;
PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB = NULL;
PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB = NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB = NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB = NULL;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB = NULL;
PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB = NULL;
PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB = NULL;
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB = NULL;
PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB = NULL;
PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB = NULL;
PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB = NULL;
PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB = NULL;
PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB = NULL;
PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB = NULL;
PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB = NULL;
PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB = NULL;
PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB = NULL;
PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB = NULL;
PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB = NULL;
PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB = NULL;
PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB = NULL;
PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB = NULL;
PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB = NULL;
PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB = NULL;
PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB = NULL;
PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB = NULL;
PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB = NULL;
PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB = NULL;
PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB = NULL;
PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB = NULL;
PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB = NULL;
PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB = NULL;
PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB = NULL;
PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB = NULL;
PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB = NULL;
PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB = NULL;
PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB = NULL;
PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB = NULL;
PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB = NULL;
PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB = NULL;
PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB = NULL;

PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB = NULL;
PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttribARB = NULL;
PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB = NULL;

PFNGLCOLORP3UIPROC glColorP3ui = NULL;
PFNGLCOLORP3UIVPROC glColorP3uiv = NULL;
PFNGLCOLORP4UIPROC glColorP4ui = NULL;
PFNGLCOLORP4UIVPROC glColorP4uiv = NULL;
PFNGLMULTITEXCOORDP1UIPROC glMultiTexCoordP1ui = NULL;
PFNGLMULTITEXCOORDP1UIVPROC glMultiTexCoordP1uiv = NULL;
PFNGLMULTITEXCOORDP2UIPROC glMultiTexCoordP2ui = NULL;
PFNGLMULTITEXCOORDP2UIVPROC glMultiTexCoordP2uiv = NULL;
PFNGLMULTITEXCOORDP3UIPROC glMultiTexCoordP3ui = NULL;
PFNGLMULTITEXCOORDP3UIVPROC glMultiTexCoordP3uiv = NULL;
PFNGLMULTITEXCOORDP4UIPROC glMultiTexCoordP4ui = NULL;
PFNGLMULTITEXCOORDP4UIVPROC glMultiTexCoordP4uiv = NULL;
PFNGLNORMALP3UIPROC glNormalP3ui = NULL;
PFNGLNORMALP3UIVPROC glNormalP3uiv = NULL;
PFNGLSECONDARYCOLORP3UIPROC glSecondaryColorP3ui = NULL;
PFNGLSECONDARYCOLORP3UIVPROC glSecondaryColorP3uiv = NULL;
PFNGLTEXCOORDP1UIPROC glTexCoordP1ui = NULL;
PFNGLTEXCOORDP1UIVPROC glTexCoordP1uiv = NULL;
PFNGLTEXCOORDP2UIPROC glTexCoordP2ui = NULL;
PFNGLTEXCOORDP2UIVPROC glTexCoordP2uiv = NULL;
PFNGLTEXCOORDP3UIPROC glTexCoordP3ui = NULL;
PFNGLTEXCOORDP3UIVPROC glTexCoordP3uiv = NULL;
PFNGLTEXCOORDP4UIPROC glTexCoordP4ui = NULL;
PFNGLTEXCOORDP4UIVPROC glTexCoordP4uiv = NULL;
PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui = NULL;
PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv = NULL;
PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui = NULL;
PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv = NULL;
PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui = NULL;
PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv = NULL;
PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui = NULL;
PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv = NULL;
PFNGLVERTEXP2UIPROC glVertexP2ui = NULL;
PFNGLVERTEXP2UIVPROC glVertexP2uiv = NULL;
PFNGLVERTEXP3UIPROC glVertexP3ui = NULL;
PFNGLVERTEXP3UIVPROC glVertexP3uiv = NULL;
PFNGLVERTEXP4UIPROC glVertexP4ui = NULL;
PFNGLVERTEXP4UIVPROC glVertexP4uiv = NULL;

PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv = NULL;
PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed = NULL;
PFNGLGETDOUBLEI_VPROC glGetDoublei_v = NULL;
PFNGLGETFLOATI_VPROC glGetFloati_v = NULL;
PFNGLSCISSORARRAYVPROC glScissorArrayv = NULL;
PFNGLSCISSORINDEXEDPROC glScissorIndexed = NULL;
PFNGLSCISSORINDEXEDVPROC glScissorIndexedv = NULL;
PFNGLVIEWPORTARRAYVPROC glViewportArrayv = NULL;
PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf = NULL;
PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv = NULL;

PFNGLWINDOWPOS2DARBPROC glWindowPos2dARB = NULL;
PFNGLWINDOWPOS2DVARBPROC glWindowPos2dvARB = NULL;
PFNGLWINDOWPOS2FARBPROC glWindowPos2fARB = NULL;
PFNGLWINDOWPOS2FVARBPROC glWindowPos2fvARB = NULL;
PFNGLWINDOWPOS2IARBPROC glWindowPos2iARB = NULL;
PFNGLWINDOWPOS2IVARBPROC glWindowPos2ivARB = NULL;
PFNGLWINDOWPOS2SARBPROC glWindowPos2sARB = NULL;
PFNGLWINDOWPOS2SVARBPROC glWindowPos2svARB = NULL;
PFNGLWINDOWPOS3DARBPROC glWindowPos3dARB = NULL;
PFNGLWINDOWPOS3DVARBPROC glWindowPos3dvARB = NULL;
PFNGLWINDOWPOS3FARBPROC glWindowPos3fARB = NULL;
PFNGLWINDOWPOS3FVARBPROC glWindowPos3fvARB = NULL;
PFNGLWINDOWPOS3IARBPROC glWindowPos3iARB = NULL;
PFNGLWINDOWPOS3IVARBPROC glWindowPos3ivARB = NULL;
PFNGLWINDOWPOS3SARBPROC glWindowPos3sARB = NULL;
PFNGLWINDOWPOS3SVARBPROC glWindowPos3svARB = NULL;

PFNGLDRAWBUFFERSATIPROC glDrawBuffersATI = NULL;

PFNGLDRAWELEMENTARRAYATIPROC glDrawElementArrayATI = NULL;
PFNGLDRAWRANGEELEMENTARRAYATIPROC glDrawRangeElementArrayATI = NULL;
PFNGLELEMENTPOINTERATIPROC glElementPointerATI = NULL;

PFNGLGETTEXBUMPPARAMETERFVATIPROC glGetTexBumpParameterfvATI = NULL;
PFNGLGETTEXBUMPPARAMETERIVATIPROC glGetTexBumpParameterivATI = NULL;
PFNGLTEXBUMPPARAMETERFVATIPROC glTexBumpParameterfvATI = NULL;
PFNGLTEXBUMPPARAMETERIVATIPROC glTexBumpParameterivATI = NULL;

PFNGLALPHAFRAGMENTOP1ATIPROC glAlphaFragmentOp1ATI = NULL;
PFNGLALPHAFRAGMENTOP2ATIPROC glAlphaFragmentOp2ATI = NULL;
PFNGLALPHAFRAGMENTOP3ATIPROC glAlphaFragmentOp3ATI = NULL;
PFNGLBEGINFRAGMENTSHADERATIPROC glBeginFragmentShaderATI = NULL;
PFNGLBINDFRAGMENTSHADERATIPROC glBindFragmentShaderATI = NULL;
PFNGLCOLORFRAGMENTOP1ATIPROC glColorFragmentOp1ATI = NULL;
PFNGLCOLORFRAGMENTOP2ATIPROC glColorFragmentOp2ATI = NULL;
PFNGLCOLORFRAGMENTOP3ATIPROC glColorFragmentOp3ATI = NULL;
PFNGLDELETEFRAGMENTSHADERATIPROC glDeleteFragmentShaderATI = NULL;
PFNGLENDFRAGMENTSHADERATIPROC glEndFragmentShaderATI = NULL;
PFNGLGENFRAGMENTSHADERSATIPROC glGenFragmentShadersATI = NULL;
PFNGLPASSTEXCOORDATIPROC glPassTexCoordATI = NULL;
PFNGLSAMPLEMAPATIPROC glSampleMapATI = NULL;
PFNGLSETFRAGMENTSHADERCONSTANTATIPROC glSetFragmentShaderConstantATI = NULL;

PFNGLMAPOBJECTBUFFERATIPROC glMapObjectBufferATI = NULL;
PFNGLUNMAPOBJECTBUFFERATIPROC glUnmapObjectBufferATI = NULL;

PFNGLPNTRIANGLESFATIPROC glPNTrianglesfATI = NULL;
PFNGLPNTRIANGLESIATIPROC glPNTrianglesiATI = NULL;

PFNGLSTENCILFUNCSEPARATEATIPROC glStencilFuncSeparateATI = NULL;
PFNGLSTENCILOPSEPARATEATIPROC glStencilOpSeparateATI = NULL;

PFNGLARRAYOBJECTATIPROC glArrayObjectATI = NULL;
PFNGLFREEOBJECTBUFFERATIPROC glFreeObjectBufferATI = NULL;
PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectfvATI = NULL;
PFNGLGETARRAYOBJECTIVATIPROC glGetArrayObjectivATI = NULL;
PFNGLGETOBJECTBUFFERFVATIPROC glGetObjectBufferfvATI = NULL;
PFNGLGETOBJECTBUFFERIVATIPROC glGetObjectBufferivATI = NULL;
PFNGLGETVARIANTARRAYOBJECTFVATIPROC glGetVariantArrayObjectfvATI = NULL;
PFNGLGETVARIANTARRAYOBJECTIVATIPROC glGetVariantArrayObjectivATI = NULL;
PFNGLISOBJECTBUFFERATIPROC glIsObjectBufferATI = NULL;
PFNGLNEWOBJECTBUFFERATIPROC glNewObjectBufferATI = NULL;
PFNGLUPDATEOBJECTBUFFERATIPROC glUpdateObjectBufferATI = NULL;
PFNGLVARIANTARRAYOBJECTATIPROC glVariantArrayObjectATI = NULL;

PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC glGetVertexAttribArrayObjectfvATI = NULL;
PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC glGetVertexAttribArrayObjectivATI = NULL;
PFNGLVERTEXATTRIBARRAYOBJECTATIPROC glVertexAttribArrayObjectATI = NULL;

PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC glClientActiveVertexStreamATI = NULL;
PFNGLNORMALSTREAM3BATIPROC glNormalStream3bATI = NULL;
PFNGLNORMALSTREAM3BVATIPROC glNormalStream3bvATI = NULL;
PFNGLNORMALSTREAM3DATIPROC glNormalStream3dATI = NULL;
PFNGLNORMALSTREAM3DVATIPROC glNormalStream3dvATI = NULL;
PFNGLNORMALSTREAM3FATIPROC glNormalStream3fATI = NULL;
PFNGLNORMALSTREAM3FVATIPROC glNormalStream3fvATI = NULL;
PFNGLNORMALSTREAM3IATIPROC glNormalStream3iATI = NULL;
PFNGLNORMALSTREAM3IVATIPROC glNormalStream3ivATI = NULL;
PFNGLNORMALSTREAM3SATIPROC glNormalStream3sATI = NULL;
PFNGLNORMALSTREAM3SVATIPROC glNormalStream3svATI = NULL;
PFNGLVERTEXBLENDENVFATIPROC glVertexBlendEnvfATI = NULL;
PFNGLVERTEXBLENDENVIATIPROC glVertexBlendEnviATI = NULL;
PFNGLVERTEXSTREAM2DATIPROC glVertexStream2dATI = NULL;
PFNGLVERTEXSTREAM2DVATIPROC glVertexStream2dvATI = NULL;
PFNGLVERTEXSTREAM2FATIPROC glVertexStream2fATI = NULL;
PFNGLVERTEXSTREAM2FVATIPROC glVertexStream2fvATI = NULL;
PFNGLVERTEXSTREAM2IATIPROC glVertexStream2iATI = NULL;
PFNGLVERTEXSTREAM2IVATIPROC glVertexStream2ivATI = NULL;
PFNGLVERTEXSTREAM2SATIPROC glVertexStream2sATI = NULL;
PFNGLVERTEXSTREAM2SVATIPROC glVertexStream2svATI = NULL;
PFNGLVERTEXSTREAM3DATIPROC glVertexStream3dATI = NULL;
PFNGLVERTEXSTREAM3DVATIPROC glVertexStream3dvATI = NULL;
PFNGLVERTEXSTREAM3FATIPROC glVertexStream3fATI = NULL;
PFNGLVERTEXSTREAM3FVATIPROC glVertexStream3fvATI = NULL;
PFNGLVERTEXSTREAM3IATIPROC glVertexStream3iATI = NULL;
PFNGLVERTEXSTREAM3IVATIPROC glVertexStream3ivATI = NULL;
PFNGLVERTEXSTREAM3SATIPROC glVertexStream3sATI = NULL;
PFNGLVERTEXSTREAM3SVATIPROC glVertexStream3svATI = NULL;
PFNGLVERTEXSTREAM4DATIPROC glVertexStream4dATI = NULL;
PFNGLVERTEXSTREAM4DVATIPROC glVertexStream4dvATI = NULL;
PFNGLVERTEXSTREAM4FATIPROC glVertexStream4fATI = NULL;
PFNGLVERTEXSTREAM4FVATIPROC glVertexStream4fvATI = NULL;
PFNGLVERTEXSTREAM4IATIPROC glVertexStream4iATI = NULL;
PFNGLVERTEXSTREAM4IVATIPROC glVertexStream4ivATI = NULL;
PFNGLVERTEXSTREAM4SATIPROC glVertexStream4sATI = NULL;
PFNGLVERTEXSTREAM4SVATIPROC glVertexStream4svATI = NULL;

PFNGLGETUNIFORMBUFFERSIZEEXTPROC glGetUniformBufferSizeEXT = NULL;
PFNGLGETUNIFORMOFFSETEXTPROC glGetUniformOffsetEXT = NULL;
PFNGLUNIFORMBUFFEREXTPROC glUniformBufferEXT = NULL;

PFNGLBLENDCOLOREXTPROC glBlendColorEXT = NULL;

PFNGLBLENDEQUATIONSEPARATEEXTPROC glBlendEquationSeparateEXT = NULL;

PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT = NULL;

PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT = NULL;

PFNGLCOLORSUBTABLEEXTPROC glColorSubTableEXT = NULL;
PFNGLCOPYCOLORSUBTABLEEXTPROC glCopyColorSubTableEXT = NULL;

PFNGLLOCKARRAYSEXTPROC glLockArraysEXT = NULL;
PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT = NULL;

PFNGLCONVOLUTIONFILTER1DEXTPROC glConvolutionFilter1DEXT = NULL;
PFNGLCONVOLUTIONFILTER2DEXTPROC glConvolutionFilter2DEXT = NULL;
PFNGLCONVOLUTIONPARAMETERFEXTPROC glConvolutionParameterfEXT = NULL;
PFNGLCONVOLUTIONPARAMETERFVEXTPROC glConvolutionParameterfvEXT = NULL;
PFNGLCONVOLUTIONPARAMETERIEXTPROC glConvolutionParameteriEXT = NULL;
PFNGLCONVOLUTIONPARAMETERIVEXTPROC glConvolutionParameterivEXT = NULL;
PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC glCopyConvolutionFilter1DEXT = NULL;
PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC glCopyConvolutionFilter2DEXT = NULL;
PFNGLGETCONVOLUTIONFILTEREXTPROC glGetConvolutionFilterEXT = NULL;
PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC glGetConvolutionParameterfvEXT = NULL;
PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC glGetConvolutionParameterivEXT = NULL;
PFNGLGETSEPARABLEFILTEREXTPROC glGetSeparableFilterEXT = NULL;
PFNGLSEPARABLEFILTER2DEXTPROC glSeparableFilter2DEXT = NULL;

PFNGLBINORMALPOINTEREXTPROC glBinormalPointerEXT = NULL;
PFNGLTANGENTPOINTEREXTPROC glTangentPointerEXT = NULL;

PFNGLCOPYTEXIMAGE1DEXTPROC glCopyTexImage1DEXT = NULL;
PFNGLCOPYTEXIMAGE2DEXTPROC glCopyTexImage2DEXT = NULL;
PFNGLCOPYTEXSUBIMAGE1DEXTPROC glCopyTexSubImage1DEXT = NULL;
PFNGLCOPYTEXSUBIMAGE2DEXTPROC glCopyTexSubImage2DEXT = NULL;
PFNGLCOPYTEXSUBIMAGE3DEXTPROC glCopyTexSubImage3DEXT = NULL;

PFNGLCULLPARAMETERDVEXTPROC glCullParameterdvEXT = NULL;
PFNGLCULLPARAMETERFVEXTPROC glCullParameterfvEXT = NULL;

PFNGLDEPTHBOUNDSEXTPROC glDepthBoundsEXT = NULL;

PFNGLBINDMULTITEXTUREEXTPROC glBindMultiTextureEXT = NULL;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC glCheckNamedFramebufferStatusEXT = NULL;
PFNGLCLIENTATTRIBDEFAULTEXTPROC glClientAttribDefaultEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC glCompressedMultiTexImage1DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC glCompressedMultiTexImage2DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC glCompressedMultiTexImage3DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC glCompressedMultiTexSubImage1DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC glCompressedMultiTexSubImage2DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC glCompressedMultiTexSubImage3DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC glCompressedTextureImage1DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC glCompressedTextureImage2DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC glCompressedTextureImage3DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC glCompressedTextureSubImage1DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC glCompressedTextureSubImage2DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC glCompressedTextureSubImage3DEXT = NULL;
PFNGLCOPYMULTITEXIMAGE1DEXTPROC glCopyMultiTexImage1DEXT = NULL;
PFNGLCOPYMULTITEXIMAGE2DEXTPROC glCopyMultiTexImage2DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC glCopyMultiTexSubImage1DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC glCopyMultiTexSubImage2DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC glCopyMultiTexSubImage3DEXT = NULL;
PFNGLCOPYTEXTUREIMAGE1DEXTPROC glCopyTextureImage1DEXT = NULL;
PFNGLCOPYTEXTUREIMAGE2DEXTPROC glCopyTextureImage2DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC glCopyTextureSubImage1DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC glCopyTextureSubImage2DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC glCopyTextureSubImage3DEXT = NULL;
PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC glDisableClientStateIndexedEXT = NULL;
PFNGLDISABLECLIENTSTATEIEXTPROC glDisableClientStateiEXT = NULL;
PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC glDisableVertexArrayAttribEXT = NULL;
PFNGLDISABLEVERTEXARRAYEXTPROC glDisableVertexArrayEXT = NULL;
PFNGLENABLECLIENTSTATEINDEXEDEXTPROC glEnableClientStateIndexedEXT = NULL;
PFNGLENABLECLIENTSTATEIEXTPROC glEnableClientStateiEXT = NULL;
PFNGLENABLEVERTEXARRAYATTRIBEXTPROC glEnableVertexArrayAttribEXT = NULL;
PFNGLENABLEVERTEXARRAYEXTPROC glEnableVertexArrayEXT = NULL;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC glFlushMappedNamedBufferRangeEXT = NULL;
PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC glFramebufferDrawBufferEXT = NULL;
PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC glFramebufferDrawBuffersEXT = NULL;
PFNGLFRAMEBUFFERREADBUFFEREXTPROC glFramebufferReadBufferEXT = NULL;
PFNGLGENERATEMULTITEXMIPMAPEXTPROC glGenerateMultiTexMipmapEXT = NULL;
PFNGLGENERATETEXTUREMIPMAPEXTPROC glGenerateTextureMipmapEXT = NULL;
PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC glGetCompressedMultiTexImageEXT = NULL;
PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC glGetCompressedTextureImageEXT = NULL;
PFNGLGETDOUBLEINDEXEDVEXTPROC glGetDoubleIndexedvEXT = NULL;
PFNGLGETDOUBLEI_VEXTPROC glGetDoublei_vEXT = NULL;
PFNGLGETFLOATINDEXEDVEXTPROC glGetFloatIndexedvEXT = NULL;
PFNGLGETFLOATI_VEXTPROC glGetFloati_vEXT = NULL;
PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC glGetFramebufferParameterivEXT = NULL;
PFNGLGETMULTITEXENVFVEXTPROC glGetMultiTexEnvfvEXT = NULL;
PFNGLGETMULTITEXENVIVEXTPROC glGetMultiTexEnvivEXT = NULL;
PFNGLGETMULTITEXGENDVEXTPROC glGetMultiTexGendvEXT = NULL;
PFNGLGETMULTITEXGENFVEXTPROC glGetMultiTexGenfvEXT = NULL;
PFNGLGETMULTITEXGENIVEXTPROC glGetMultiTexGenivEXT = NULL;
PFNGLGETMULTITEXIMAGEEXTPROC glGetMultiTexImageEXT = NULL;
PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC glGetMultiTexLevelParameterfvEXT = NULL;
PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC glGetMultiTexLevelParameterivEXT = NULL;
PFNGLGETMULTITEXPARAMETERIIVEXTPROC glGetMultiTexParameterIivEXT = NULL;
PFNGLGETMULTITEXPARAMETERIUIVEXTPROC glGetMultiTexParameterIuivEXT = NULL;
PFNGLGETMULTITEXPARAMETERFVEXTPROC glGetMultiTexParameterfvEXT = NULL;
PFNGLGETMULTITEXPARAMETERIVEXTPROC glGetMultiTexParameterivEXT = NULL;
PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC glGetNamedBufferParameterivEXT = NULL;
PFNGLGETNAMEDBUFFERPOINTERVEXTPROC glGetNamedBufferPointervEXT = NULL;
PFNGLGETNAMEDBUFFERSUBDATAEXTPROC glGetNamedBufferSubDataEXT = NULL;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetNamedFramebufferAttachmentParameterivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC glGetNamedProgramLocalParameterIivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC glGetNamedProgramLocalParameterIuivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC glGetNamedProgramLocalParameterdvEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC glGetNamedProgramLocalParameterfvEXT = NULL;
PFNGLGETNAMEDPROGRAMSTRINGEXTPROC glGetNamedProgramStringEXT = NULL;
PFNGLGETNAMEDPROGRAMIVEXTPROC glGetNamedProgramivEXT = NULL;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC glGetNamedRenderbufferParameterivEXT = NULL;
PFNGLGETPOINTERINDEXEDVEXTPROC glGetPointerIndexedvEXT = NULL;
PFNGLGETPOINTERI_VEXTPROC glGetPointeri_vEXT = NULL;
PFNGLGETTEXTUREIMAGEEXTPROC glGetTextureImageEXT = NULL;
PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC glGetTextureLevelParameterfvEXT = NULL;
PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC glGetTextureLevelParameterivEXT = NULL;
PFNGLGETTEXTUREPARAMETERIIVEXTPROC glGetTextureParameterIivEXT = NULL;
PFNGLGETTEXTUREPARAMETERIUIVEXTPROC glGetTextureParameterIuivEXT = NULL;
PFNGLGETTEXTUREPARAMETERFVEXTPROC glGetTextureParameterfvEXT = NULL;
PFNGLGETTEXTUREPARAMETERIVEXTPROC glGetTextureParameterivEXT = NULL;
PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC glGetVertexArrayIntegeri_vEXT = NULL;
PFNGLGETVERTEXARRAYINTEGERVEXTPROC glGetVertexArrayIntegervEXT = NULL;
PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC glGetVertexArrayPointeri_vEXT = NULL;
PFNGLGETVERTEXARRAYPOINTERVEXTPROC glGetVertexArrayPointervEXT = NULL;
PFNGLMAPNAMEDBUFFEREXTPROC glMapNamedBufferEXT = NULL;
PFNGLMAPNAMEDBUFFERRANGEEXTPROC glMapNamedBufferRangeEXT = NULL;
PFNGLMATRIXFRUSTUMEXTPROC glMatrixFrustumEXT = NULL;
PFNGLMATRIXLOADIDENTITYEXTPROC glMatrixLoadIdentityEXT = NULL;
PFNGLMATRIXLOADTRANSPOSEDEXTPROC glMatrixLoadTransposedEXT = NULL;
PFNGLMATRIXLOADTRANSPOSEFEXTPROC glMatrixLoadTransposefEXT = NULL;
PFNGLMATRIXLOADDEXTPROC glMatrixLoaddEXT = NULL;
PFNGLMATRIXLOADFEXTPROC glMatrixLoadfEXT = NULL;
PFNGLMATRIXMULTTRANSPOSEDEXTPROC glMatrixMultTransposedEXT = NULL;
PFNGLMATRIXMULTTRANSPOSEFEXTPROC glMatrixMultTransposefEXT = NULL;
PFNGLMATRIXMULTDEXTPROC glMatrixMultdEXT = NULL;
PFNGLMATRIXMULTFEXTPROC glMatrixMultfEXT = NULL;
PFNGLMATRIXORTHOEXTPROC glMatrixOrthoEXT = NULL;
PFNGLMATRIXPOPEXTPROC glMatrixPopEXT = NULL;
PFNGLMATRIXPUSHEXTPROC glMatrixPushEXT = NULL;
PFNGLMATRIXROTATEDEXTPROC glMatrixRotatedEXT = NULL;
PFNGLMATRIXROTATEFEXTPROC glMatrixRotatefEXT = NULL;
PFNGLMATRIXSCALEDEXTPROC glMatrixScaledEXT = NULL;
PFNGLMATRIXSCALEFEXTPROC glMatrixScalefEXT = NULL;
PFNGLMATRIXTRANSLATEDEXTPROC glMatrixTranslatedEXT = NULL;
PFNGLMATRIXTRANSLATEFEXTPROC glMatrixTranslatefEXT = NULL;
PFNGLMULTITEXBUFFEREXTPROC glMultiTexBufferEXT = NULL;
PFNGLMULTITEXCOORDPOINTEREXTPROC glMultiTexCoordPointerEXT = NULL;
PFNGLMULTITEXENVFEXTPROC glMultiTexEnvfEXT = NULL;
PFNGLMULTITEXENVFVEXTPROC glMultiTexEnvfvEXT = NULL;
PFNGLMULTITEXENVIEXTPROC glMultiTexEnviEXT = NULL;
PFNGLMULTITEXENVIVEXTPROC glMultiTexEnvivEXT = NULL;
PFNGLMULTITEXGENDEXTPROC glMultiTexGendEXT = NULL;
PFNGLMULTITEXGENDVEXTPROC glMultiTexGendvEXT = NULL;
PFNGLMULTITEXGENFEXTPROC glMultiTexGenfEXT = NULL;
PFNGLMULTITEXGENFVEXTPROC glMultiTexGenfvEXT = NULL;
PFNGLMULTITEXGENIEXTPROC glMultiTexGeniEXT = NULL;
PFNGLMULTITEXGENIVEXTPROC glMultiTexGenivEXT = NULL;
PFNGLMULTITEXIMAGE1DEXTPROC glMultiTexImage1DEXT = NULL;
PFNGLMULTITEXIMAGE2DEXTPROC glMultiTexImage2DEXT = NULL;
PFNGLMULTITEXIMAGE3DEXTPROC glMultiTexImage3DEXT = NULL;
PFNGLMULTITEXPARAMETERIIVEXTPROC glMultiTexParameterIivEXT = NULL;
PFNGLMULTITEXPARAMETERIUIVEXTPROC glMultiTexParameterIuivEXT = NULL;
PFNGLMULTITEXPARAMETERFEXTPROC glMultiTexParameterfEXT = NULL;
PFNGLMULTITEXPARAMETERFVEXTPROC glMultiTexParameterfvEXT = NULL;
PFNGLMULTITEXPARAMETERIEXTPROC glMultiTexParameteriEXT = NULL;
PFNGLMULTITEXPARAMETERIVEXTPROC glMultiTexParameterivEXT = NULL;
PFNGLMULTITEXRENDERBUFFEREXTPROC glMultiTexRenderbufferEXT = NULL;
PFNGLMULTITEXSUBIMAGE1DEXTPROC glMultiTexSubImage1DEXT = NULL;
PFNGLMULTITEXSUBIMAGE2DEXTPROC glMultiTexSubImage2DEXT = NULL;
PFNGLMULTITEXSUBIMAGE3DEXTPROC glMultiTexSubImage3DEXT = NULL;
PFNGLNAMEDBUFFERDATAEXTPROC glNamedBufferDataEXT = NULL;
PFNGLNAMEDBUFFERSUBDATAEXTPROC glNamedBufferSubDataEXT = NULL;
PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC glNamedCopyBufferSubDataEXT = NULL;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC glNamedFramebufferRenderbufferEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC glNamedFramebufferTexture1DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC glNamedFramebufferTexture2DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC glNamedFramebufferTexture3DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC glNamedFramebufferTextureEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC glNamedFramebufferTextureFaceEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC glNamedFramebufferTextureLayerEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC glNamedProgramLocalParameter4dEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC glNamedProgramLocalParameter4dvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC glNamedProgramLocalParameter4fEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC glNamedProgramLocalParameter4fvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC glNamedProgramLocalParameterI4iEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC glNamedProgramLocalParameterI4ivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC glNamedProgramLocalParameterI4uiEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC glNamedProgramLocalParameterI4uivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC glNamedProgramLocalParameters4fvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC glNamedProgramLocalParametersI4ivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC glNamedProgramLocalParametersI4uivEXT = NULL;
PFNGLNAMEDPROGRAMSTRINGEXTPROC glNamedProgramStringEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC glNamedRenderbufferStorageEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC glNamedRenderbufferStorageMultisampleCoverageEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glNamedRenderbufferStorageMultisampleEXT = NULL;
PFNGLPROGRAMUNIFORM1FEXTPROC glProgramUniform1fEXT = NULL;
PFNGLPROGRAMUNIFORM1FVEXTPROC glProgramUniform1fvEXT = NULL;
PFNGLPROGRAMUNIFORM1IEXTPROC glProgramUniform1iEXT = NULL;
PFNGLPROGRAMUNIFORM1IVEXTPROC glProgramUniform1ivEXT = NULL;
PFNGLPROGRAMUNIFORM1UIEXTPROC glProgramUniform1uiEXT = NULL;
PFNGLPROGRAMUNIFORM1UIVEXTPROC glProgramUniform1uivEXT = NULL;
PFNGLPROGRAMUNIFORM2FEXTPROC glProgramUniform2fEXT = NULL;
PFNGLPROGRAMUNIFORM2FVEXTPROC glProgramUniform2fvEXT = NULL;
PFNGLPROGRAMUNIFORM2IEXTPROC glProgramUniform2iEXT = NULL;
PFNGLPROGRAMUNIFORM2IVEXTPROC glProgramUniform2ivEXT = NULL;
PFNGLPROGRAMUNIFORM2UIEXTPROC glProgramUniform2uiEXT = NULL;
PFNGLPROGRAMUNIFORM2UIVEXTPROC glProgramUniform2uivEXT = NULL;
PFNGLPROGRAMUNIFORM3FEXTPROC glProgramUniform3fEXT = NULL;
PFNGLPROGRAMUNIFORM3FVEXTPROC glProgramUniform3fvEXT = NULL;
PFNGLPROGRAMUNIFORM3IEXTPROC glProgramUniform3iEXT = NULL;
PFNGLPROGRAMUNIFORM3IVEXTPROC glProgramUniform3ivEXT = NULL;
PFNGLPROGRAMUNIFORM3UIEXTPROC glProgramUniform3uiEXT = NULL;
PFNGLPROGRAMUNIFORM3UIVEXTPROC glProgramUniform3uivEXT = NULL;
PFNGLPROGRAMUNIFORM4FEXTPROC glProgramUniform4fEXT = NULL;
PFNGLPROGRAMUNIFORM4FVEXTPROC glProgramUniform4fvEXT = NULL;
PFNGLPROGRAMUNIFORM4IEXTPROC glProgramUniform4iEXT = NULL;
PFNGLPROGRAMUNIFORM4IVEXTPROC glProgramUniform4ivEXT = NULL;
PFNGLPROGRAMUNIFORM4UIEXTPROC glProgramUniform4uiEXT = NULL;
PFNGLPROGRAMUNIFORM4UIVEXTPROC glProgramUniform4uivEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC glProgramUniformMatrix2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC glProgramUniformMatrix2x3fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC glProgramUniformMatrix2x4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC glProgramUniformMatrix3fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC glProgramUniformMatrix3x2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC glProgramUniformMatrix3x4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC glProgramUniformMatrix4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC glProgramUniformMatrix4x2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC glProgramUniformMatrix4x3fvEXT = NULL;
PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC glPushClientAttribDefaultEXT = NULL;
PFNGLTEXTUREBUFFEREXTPROC glTextureBufferEXT = NULL;
PFNGLTEXTUREIMAGE1DEXTPROC glTextureImage1DEXT = NULL;
PFNGLTEXTUREIMAGE2DEXTPROC glTextureImage2DEXT = NULL;
PFNGLTEXTUREIMAGE3DEXTPROC glTextureImage3DEXT = NULL;
PFNGLTEXTUREPARAMETERIIVEXTPROC glTextureParameterIivEXT = NULL;
PFNGLTEXTUREPARAMETERIUIVEXTPROC glTextureParameterIuivEXT = NULL;
PFNGLTEXTUREPARAMETERFEXTPROC glTextureParameterfEXT = NULL;
PFNGLTEXTUREPARAMETERFVEXTPROC glTextureParameterfvEXT = NULL;
PFNGLTEXTUREPARAMETERIEXTPROC glTextureParameteriEXT = NULL;
PFNGLTEXTUREPARAMETERIVEXTPROC glTextureParameterivEXT = NULL;
PFNGLTEXTURERENDERBUFFEREXTPROC glTextureRenderbufferEXT = NULL;
PFNGLTEXTURESUBIMAGE1DEXTPROC glTextureSubImage1DEXT = NULL;
PFNGLTEXTURESUBIMAGE2DEXTPROC glTextureSubImage2DEXT = NULL;
PFNGLTEXTURESUBIMAGE3DEXTPROC glTextureSubImage3DEXT = NULL;
PFNGLUNMAPNAMEDBUFFEREXTPROC glUnmapNamedBufferEXT = NULL;
PFNGLVERTEXARRAYCOLOROFFSETEXTPROC glVertexArrayColorOffsetEXT = NULL;
PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC glVertexArrayEdgeFlagOffsetEXT = NULL;
PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC glVertexArrayFogCoordOffsetEXT = NULL;
PFNGLVERTEXARRAYINDEXOFFSETEXTPROC glVertexArrayIndexOffsetEXT = NULL;
PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC glVertexArrayMultiTexCoordOffsetEXT = NULL;
PFNGLVERTEXARRAYNORMALOFFSETEXTPROC glVertexArrayNormalOffsetEXT = NULL;
PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC glVertexArraySecondaryColorOffsetEXT = NULL;
PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC glVertexArrayTexCoordOffsetEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC glVertexArrayVertexAttribIOffsetEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC glVertexArrayVertexAttribOffsetEXT = NULL;
PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC glVertexArrayVertexOffsetEXT = NULL;

PFNGLCOLORMASKINDEXEDEXTPROC glColorMaskIndexedEXT = NULL;
PFNGLDISABLEINDEXEDEXTPROC glDisableIndexedEXT = NULL;
PFNGLENABLEINDEXEDEXTPROC glEnableIndexedEXT = NULL;
PFNGLGETBOOLEANINDEXEDVEXTPROC glGetBooleanIndexedvEXT = NULL;
PFNGLGETINTEGERINDEXEDVEXTPROC glGetIntegerIndexedvEXT = NULL;
PFNGLISENABLEDINDEXEDEXTPROC glIsEnabledIndexedEXT = NULL;

PFNGLDRAWARRAYSINSTANCEDEXTPROC glDrawArraysInstancedEXT = NULL;
PFNGLDRAWELEMENTSINSTANCEDEXTPROC glDrawElementsInstancedEXT = NULL;

PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT = NULL;

PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT = NULL;
PFNGLFOGCOORDDEXTPROC glFogCoorddEXT = NULL;
PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT = NULL;
PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;
PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT = NULL;

PFNGLFRAGMENTCOLORMATERIALEXTPROC glFragmentColorMaterialEXT = NULL;
PFNGLFRAGMENTLIGHTMODELFEXTPROC glFragmentLightModelfEXT = NULL;
PFNGLFRAGMENTLIGHTMODELFVEXTPROC glFragmentLightModelfvEXT = NULL;
PFNGLFRAGMENTLIGHTMODELIEXTPROC glFragmentLightModeliEXT = NULL;
PFNGLFRAGMENTLIGHTMODELIVEXTPROC glFragmentLightModelivEXT = NULL;
PFNGLFRAGMENTLIGHTFEXTPROC glFragmentLightfEXT = NULL;
PFNGLFRAGMENTLIGHTFVEXTPROC glFragmentLightfvEXT = NULL;
PFNGLFRAGMENTLIGHTIEXTPROC glFragmentLightiEXT = NULL;
PFNGLFRAGMENTLIGHTIVEXTPROC glFragmentLightivEXT = NULL;
PFNGLFRAGMENTMATERIALFEXTPROC glFragmentMaterialfEXT = NULL;
PFNGLFRAGMENTMATERIALFVEXTPROC glFragmentMaterialfvEXT = NULL;
PFNGLFRAGMENTMATERIALIEXTPROC glFragmentMaterialiEXT = NULL;
PFNGLFRAGMENTMATERIALIVEXTPROC glFragmentMaterialivEXT = NULL;
PFNGLGETFRAGMENTLIGHTFVEXTPROC glGetFragmentLightfvEXT = NULL;
PFNGLGETFRAGMENTLIGHTIVEXTPROC glGetFragmentLightivEXT = NULL;
PFNGLGETFRAGMENTMATERIALFVEXTPROC glGetFragmentMaterialfvEXT = NULL;
PFNGLGETFRAGMENTMATERIALIVEXTPROC glGetFragmentMaterialivEXT = NULL;
PFNGLLIGHTENVIEXTPROC glLightEnviEXT = NULL;

PFNGLBLITFRAMEBUFFEREXTPROC glBlitFramebufferEXT = NULL;

PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT = NULL;

PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = NULL;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = NULL;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;
PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT = NULL;
PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = NULL;

PFNGLFRAMEBUFFERTEXTUREEXTPROC glFramebufferTextureEXT = NULL;
PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC glFramebufferTextureFaceEXT = NULL;
PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC glFramebufferTextureLayerEXT = NULL;
PFNGLPROGRAMPARAMETERIEXTPROC glProgramParameteriEXT = NULL;

PFNGLPROGRAMENVPARAMETERS4FVEXTPROC glProgramEnvParameters4fvEXT = NULL;
PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC glProgramLocalParameters4fvEXT = NULL;

PFNGLBINDFRAGDATALOCATIONEXTPROC glBindFragDataLocationEXT = NULL;
PFNGLGETFRAGDATALOCATIONEXTPROC glGetFragDataLocationEXT = NULL;
PFNGLGETUNIFORMUIVEXTPROC glGetUniformuivEXT = NULL;
PFNGLGETVERTEXATTRIBIIVEXTPROC glGetVertexAttribIivEXT = NULL;
PFNGLGETVERTEXATTRIBIUIVEXTPROC glGetVertexAttribIuivEXT = NULL;
PFNGLUNIFORM1UIEXTPROC glUniform1uiEXT = NULL;
PFNGLUNIFORM1UIVEXTPROC glUniform1uivEXT = NULL;
PFNGLUNIFORM2UIEXTPROC glUniform2uiEXT = NULL;
PFNGLUNIFORM2UIVEXTPROC glUniform2uivEXT = NULL;
PFNGLUNIFORM3UIEXTPROC glUniform3uiEXT = NULL;
PFNGLUNIFORM3UIVEXTPROC glUniform3uivEXT = NULL;
PFNGLUNIFORM4UIEXTPROC glUniform4uiEXT = NULL;
PFNGLUNIFORM4UIVEXTPROC glUniform4uivEXT = NULL;
PFNGLVERTEXATTRIBI1IEXTPROC glVertexAttribI1iEXT = NULL;
PFNGLVERTEXATTRIBI1IVEXTPROC glVertexAttribI1ivEXT = NULL;
PFNGLVERTEXATTRIBI1UIEXTPROC glVertexAttribI1uiEXT = NULL;
PFNGLVERTEXATTRIBI1UIVEXTPROC glVertexAttribI1uivEXT = NULL;
PFNGLVERTEXATTRIBI2IEXTPROC glVertexAttribI2iEXT = NULL;
PFNGLVERTEXATTRIBI2IVEXTPROC glVertexAttribI2ivEXT = NULL;
PFNGLVERTEXATTRIBI2UIEXTPROC glVertexAttribI2uiEXT = NULL;
PFNGLVERTEXATTRIBI2UIVEXTPROC glVertexAttribI2uivEXT = NULL;
PFNGLVERTEXATTRIBI3IEXTPROC glVertexAttribI3iEXT = NULL;
PFNGLVERTEXATTRIBI3IVEXTPROC glVertexAttribI3ivEXT = NULL;
PFNGLVERTEXATTRIBI3UIEXTPROC glVertexAttribI3uiEXT = NULL;
PFNGLVERTEXATTRIBI3UIVEXTPROC glVertexAttribI3uivEXT = NULL;
PFNGLVERTEXATTRIBI4BVEXTPROC glVertexAttribI4bvEXT = NULL;
PFNGLVERTEXATTRIBI4IEXTPROC glVertexAttribI4iEXT = NULL;
PFNGLVERTEXATTRIBI4IVEXTPROC glVertexAttribI4ivEXT = NULL;
PFNGLVERTEXATTRIBI4SVEXTPROC glVertexAttribI4svEXT = NULL;
PFNGLVERTEXATTRIBI4UBVEXTPROC glVertexAttribI4ubvEXT = NULL;
PFNGLVERTEXATTRIBI4UIEXTPROC glVertexAttribI4uiEXT = NULL;
PFNGLVERTEXATTRIBI4UIVEXTPROC glVertexAttribI4uivEXT = NULL;
PFNGLVERTEXATTRIBI4USVEXTPROC glVertexAttribI4usvEXT = NULL;
PFNGLVERTEXATTRIBIPOINTEREXTPROC glVertexAttribIPointerEXT = NULL;

PFNGLGETHISTOGRAMEXTPROC glGetHistogramEXT = NULL;
PFNGLGETHISTOGRAMPARAMETERFVEXTPROC glGetHistogramParameterfvEXT = NULL;
PFNGLGETHISTOGRAMPARAMETERIVEXTPROC glGetHistogramParameterivEXT = NULL;
PFNGLGETMINMAXEXTPROC glGetMinmaxEXT = NULL;
PFNGLGETMINMAXPARAMETERFVEXTPROC glGetMinmaxParameterfvEXT = NULL;
PFNGLGETMINMAXPARAMETERIVEXTPROC glGetMinmaxParameterivEXT = NULL;
PFNGLHISTOGRAMEXTPROC glHistogramEXT = NULL;
PFNGLMINMAXEXTPROC glMinmaxEXT = NULL;
PFNGLRESETHISTOGRAMEXTPROC glResetHistogramEXT = NULL;
PFNGLRESETMINMAXEXTPROC glResetMinmaxEXT = NULL;

PFNGLINDEXFUNCEXTPROC glIndexFuncEXT = NULL;

PFNGLINDEXMATERIALEXTPROC glIndexMaterialEXT = NULL;

PFNGLAPPLYTEXTUREEXTPROC glApplyTextureEXT = NULL;
PFNGLTEXTURELIGHTEXTPROC glTextureLightEXT = NULL;
PFNGLTEXTUREMATERIALEXTPROC glTextureMaterialEXT = NULL;

PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArraysEXT = NULL;
PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElementsEXT = NULL;

PFNGLSAMPLEMASKEXTPROC glSampleMaskEXT = NULL;
PFNGLSAMPLEPATTERNEXTPROC glSamplePatternEXT = NULL;

PFNGLCOLORTABLEEXTPROC glColorTableEXT = NULL;
PFNGLGETCOLORTABLEEXTPROC glGetColorTableEXT = NULL;
PFNGLGETCOLORTABLEPARAMETERFVEXTPROC glGetColorTableParameterfvEXT = NULL;
PFNGLGETCOLORTABLEPARAMETERIVEXTPROC glGetColorTableParameterivEXT = NULL;

PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC glGetPixelTransformParameterfvEXT = NULL;
PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC glGetPixelTransformParameterivEXT = NULL;
PFNGLPIXELTRANSFORMPARAMETERFEXTPROC glPixelTransformParameterfEXT = NULL;
PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC glPixelTransformParameterfvEXT = NULL;
PFNGLPIXELTRANSFORMPARAMETERIEXTPROC glPixelTransformParameteriEXT = NULL;
PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC glPixelTransformParameterivEXT = NULL;

PFNGLPOINTPARAMETERFEXTPROC glPointParameterfEXT = NULL;
PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvEXT = NULL;

PFNGLPOLYGONOFFSETEXTPROC glPolygonOffsetEXT = NULL;

PFNGLPROVOKINGVERTEXEXTPROC glProvokingVertexEXT = NULL;

PFNGLBEGINSCENEEXTPROC glBeginSceneEXT = NULL;
PFNGLENDSCENEEXTPROC glEndSceneEXT = NULL;

PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT = NULL;
PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT = NULL;
PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT = NULL;
PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT = NULL;
PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT = NULL;
PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT = NULL;
PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT = NULL;
PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT = NULL;
PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT = NULL;
PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT = NULL;
PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT = NULL;
PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT = NULL;
PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT = NULL;
PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT = NULL;
PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT = NULL;
PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT = NULL;
PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT = NULL;

PFNGLACTIVEPROGRAMEXTPROC glActiveProgramEXT = NULL;
PFNGLCREATESHADERPROGRAMEXTPROC glCreateShaderProgramEXT = NULL;
PFNGLUSESHADERPROGRAMEXTPROC glUseShaderProgramEXT = NULL;

PFNGLBINDIMAGETEXTUREEXTPROC glBindImageTextureEXT = NULL;
PFNGLMEMORYBARRIEREXTPROC glMemoryBarrierEXT = NULL;

PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT = NULL;

PFNGLTEXSUBIMAGE1DEXTPROC glTexSubImage1DEXT = NULL;
PFNGLTEXSUBIMAGE2DEXTPROC glTexSubImage2DEXT = NULL;
PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT = NULL;

PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT = NULL;

PFNGLTEXBUFFEREXTPROC glTexBufferEXT = NULL;

PFNGLCLEARCOLORIIEXTPROC glClearColorIiEXT = NULL;
PFNGLCLEARCOLORIUIEXTPROC glClearColorIuiEXT = NULL;
PFNGLGETTEXPARAMETERIIVEXTPROC glGetTexParameterIivEXT = NULL;
PFNGLGETTEXPARAMETERIUIVEXTPROC glGetTexParameterIuivEXT = NULL;
PFNGLTEXPARAMETERIIVEXTPROC glTexParameterIivEXT = NULL;
PFNGLTEXPARAMETERIUIVEXTPROC glTexParameterIuivEXT = NULL;

PFNGLARETEXTURESRESIDENTEXTPROC glAreTexturesResidentEXT = NULL;
PFNGLBINDTEXTUREEXTPROC glBindTextureEXT = NULL;
PFNGLDELETETEXTURESEXTPROC glDeleteTexturesEXT = NULL;
PFNGLGENTEXTURESEXTPROC glGenTexturesEXT = NULL;
PFNGLISTEXTUREEXTPROC glIsTextureEXT = NULL;
PFNGLPRIORITIZETEXTURESEXTPROC glPrioritizeTexturesEXT = NULL;

PFNGLTEXTURENORMALEXTPROC glTextureNormalEXT = NULL;

PFNGLGETQUERYOBJECTI64VEXTPROC glGetQueryObjecti64vEXT = NULL;
PFNGLGETQUERYOBJECTUI64VEXTPROC glGetQueryObjectui64vEXT = NULL;

PFNGLBEGINTRANSFORMFEEDBACKEXTPROC glBeginTransformFeedbackEXT = NULL;
PFNGLBINDBUFFERBASEEXTPROC glBindBufferBaseEXT = NULL;
PFNGLBINDBUFFEROFFSETEXTPROC glBindBufferOffsetEXT = NULL;
PFNGLBINDBUFFERRANGEEXTPROC glBindBufferRangeEXT = NULL;
PFNGLENDTRANSFORMFEEDBACKEXTPROC glEndTransformFeedbackEXT = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC glGetTransformFeedbackVaryingEXT = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC glTransformFeedbackVaryingsEXT = NULL;

PFNGLARRAYELEMENTEXTPROC glArrayElementEXT = NULL;
PFNGLCOLORPOINTEREXTPROC glColorPointerEXT = NULL;
PFNGLDRAWARRAYSEXTPROC glDrawArraysEXT = NULL;
PFNGLEDGEFLAGPOINTEREXTPROC glEdgeFlagPointerEXT = NULL;
PFNGLINDEXPOINTEREXTPROC glIndexPointerEXT = NULL;
PFNGLNORMALPOINTEREXTPROC glNormalPointerEXT = NULL;
PFNGLTEXCOORDPOINTEREXTPROC glTexCoordPointerEXT = NULL;
PFNGLVERTEXPOINTEREXTPROC glVertexPointerEXT = NULL;

PFNGLGETVERTEXATTRIBLDVEXTPROC glGetVertexAttribLdvEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC glVertexArrayVertexAttribLOffsetEXT = NULL;
PFNGLVERTEXATTRIBL1DEXTPROC glVertexAttribL1dEXT = NULL;
PFNGLVERTEXATTRIBL1DVEXTPROC glVertexAttribL1dvEXT = NULL;
PFNGLVERTEXATTRIBL2DEXTPROC glVertexAttribL2dEXT = NULL;
PFNGLVERTEXATTRIBL2DVEXTPROC glVertexAttribL2dvEXT = NULL;
PFNGLVERTEXATTRIBL3DEXTPROC glVertexAttribL3dEXT = NULL;
PFNGLVERTEXATTRIBL3DVEXTPROC glVertexAttribL3dvEXT = NULL;
PFNGLVERTEXATTRIBL4DEXTPROC glVertexAttribL4dEXT = NULL;
PFNGLVERTEXATTRIBL4DVEXTPROC glVertexAttribL4dvEXT = NULL;
PFNGLVERTEXATTRIBLPOINTEREXTPROC glVertexAttribLPointerEXT = NULL;

PFNGLBEGINVERTEXSHADEREXTPROC glBeginVertexShaderEXT = NULL;
PFNGLBINDLIGHTPARAMETEREXTPROC glBindLightParameterEXT = NULL;
PFNGLBINDMATERIALPARAMETEREXTPROC glBindMaterialParameterEXT = NULL;
PFNGLBINDPARAMETEREXTPROC glBindParameterEXT = NULL;
PFNGLBINDTEXGENPARAMETEREXTPROC glBindTexGenParameterEXT = NULL;
PFNGLBINDTEXTUREUNITPARAMETEREXTPROC glBindTextureUnitParameterEXT = NULL;
PFNGLBINDVERTEXSHADEREXTPROC glBindVertexShaderEXT = NULL;
PFNGLDELETEVERTEXSHADEREXTPROC glDeleteVertexShaderEXT = NULL;
PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC glDisableVariantClientStateEXT = NULL;
PFNGLENABLEVARIANTCLIENTSTATEEXTPROC glEnableVariantClientStateEXT = NULL;
PFNGLENDVERTEXSHADEREXTPROC glEndVertexShaderEXT = NULL;
PFNGLEXTRACTCOMPONENTEXTPROC glExtractComponentEXT = NULL;
PFNGLGENSYMBOLSEXTPROC glGenSymbolsEXT = NULL;
PFNGLGENVERTEXSHADERSEXTPROC glGenVertexShadersEXT = NULL;
PFNGLGETINVARIANTBOOLEANVEXTPROC glGetInvariantBooleanvEXT = NULL;
PFNGLGETINVARIANTFLOATVEXTPROC glGetInvariantFloatvEXT = NULL;
PFNGLGETINVARIANTINTEGERVEXTPROC glGetInvariantIntegervEXT = NULL;
PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC glGetLocalConstantBooleanvEXT = NULL;
PFNGLGETLOCALCONSTANTFLOATVEXTPROC glGetLocalConstantFloatvEXT = NULL;
PFNGLGETLOCALCONSTANTINTEGERVEXTPROC glGetLocalConstantIntegervEXT = NULL;
PFNGLGETVARIANTBOOLEANVEXTPROC glGetVariantBooleanvEXT = NULL;
PFNGLGETVARIANTFLOATVEXTPROC glGetVariantFloatvEXT = NULL;
PFNGLGETVARIANTINTEGERVEXTPROC glGetVariantIntegervEXT = NULL;
PFNGLGETVARIANTPOINTERVEXTPROC glGetVariantPointervEXT = NULL;
PFNGLINSERTCOMPONENTEXTPROC glInsertComponentEXT = NULL;
PFNGLISVARIANTENABLEDEXTPROC glIsVariantEnabledEXT = NULL;
PFNGLSETINVARIANTEXTPROC glSetInvariantEXT = NULL;
PFNGLSETLOCALCONSTANTEXTPROC glSetLocalConstantEXT = NULL;
PFNGLSHADEROP1EXTPROC glShaderOp1EXT = NULL;
PFNGLSHADEROP2EXTPROC glShaderOp2EXT = NULL;
PFNGLSHADEROP3EXTPROC glShaderOp3EXT = NULL;
PFNGLSWIZZLEEXTPROC glSwizzleEXT = NULL;
PFNGLVARIANTPOINTEREXTPROC glVariantPointerEXT = NULL;
PFNGLVARIANTBVEXTPROC glVariantbvEXT = NULL;
PFNGLVARIANTDVEXTPROC glVariantdvEXT = NULL;
PFNGLVARIANTFVEXTPROC glVariantfvEXT = NULL;
PFNGLVARIANTIVEXTPROC glVariantivEXT = NULL;
PFNGLVARIANTSVEXTPROC glVariantsvEXT = NULL;
PFNGLVARIANTUBVEXTPROC glVariantubvEXT = NULL;
PFNGLVARIANTUIVEXTPROC glVariantuivEXT = NULL;
PFNGLVARIANTUSVEXTPROC glVariantusvEXT = NULL;
PFNGLWRITEMASKEXTPROC glWriteMaskEXT = NULL;

PFNGLVERTEXWEIGHTPOINTEREXTPROC glVertexWeightPointerEXT = NULL;
PFNGLVERTEXWEIGHTFEXTPROC glVertexWeightfEXT = NULL;
PFNGLVERTEXWEIGHTFVEXTPROC glVertexWeightfvEXT = NULL;

PFNGLFRAMETERMINATORGREMEDYPROC glFrameTerminatorGREMEDY = NULL;

PFNGLSTRINGMARKERGREMEDYPROC glStringMarkerGREMEDY = NULL;

PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC glGetImageTransformParameterfvHP = NULL;
PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC glGetImageTransformParameterivHP = NULL;
PFNGLIMAGETRANSFORMPARAMETERFHPPROC glImageTransformParameterfHP = NULL;
PFNGLIMAGETRANSFORMPARAMETERFVHPPROC glImageTransformParameterfvHP = NULL;
PFNGLIMAGETRANSFORMPARAMETERIHPPROC glImageTransformParameteriHP = NULL;
PFNGLIMAGETRANSFORMPARAMETERIVHPPROC glImageTransformParameterivHP = NULL;

PFNGLMULTIMODEDRAWARRAYSIBMPROC glMultiModeDrawArraysIBM = NULL;
PFNGLMULTIMODEDRAWELEMENTSIBMPROC glMultiModeDrawElementsIBM = NULL;

PFNGLCOLORPOINTERLISTIBMPROC glColorPointerListIBM = NULL;
PFNGLEDGEFLAGPOINTERLISTIBMPROC glEdgeFlagPointerListIBM = NULL;
PFNGLFOGCOORDPOINTERLISTIBMPROC glFogCoordPointerListIBM = NULL;
PFNGLINDEXPOINTERLISTIBMPROC glIndexPointerListIBM = NULL;
PFNGLNORMALPOINTERLISTIBMPROC glNormalPointerListIBM = NULL;
PFNGLSECONDARYCOLORPOINTERLISTIBMPROC glSecondaryColorPointerListIBM = NULL;
PFNGLTEXCOORDPOINTERLISTIBMPROC glTexCoordPointerListIBM = NULL;
PFNGLVERTEXPOINTERLISTIBMPROC glVertexPointerListIBM = NULL;

PFNGLCOLORPOINTERVINTELPROC glColorPointervINTEL = NULL;
PFNGLNORMALPOINTERVINTELPROC glNormalPointervINTEL = NULL;
PFNGLTEXCOORDPOINTERVINTELPROC glTexCoordPointervINTEL = NULL;
PFNGLVERTEXPOINTERVINTELPROC glVertexPointervINTEL = NULL;

PFNGLTEXSCISSORFUNCINTELPROC glTexScissorFuncINTEL = NULL;
PFNGLTEXSCISSORINTELPROC glTexScissorINTEL = NULL;

PFNGLBUFFERREGIONENABLEDEXTPROC glBufferRegionEnabledEXT = NULL;
PFNGLDELETEBUFFERREGIONEXTPROC glDeleteBufferRegionEXT = NULL;
PFNGLDRAWBUFFERREGIONEXTPROC glDrawBufferRegionEXT = NULL;
PFNGLNEWBUFFERREGIONEXTPROC glNewBufferRegionEXT = NULL;
PFNGLREADBUFFERREGIONEXTPROC glReadBufferRegionEXT = NULL;

PFNGLRESIZEBUFFERSMESAPROC glResizeBuffersMESA = NULL;

PFNGLWINDOWPOS2DMESAPROC glWindowPos2dMESA = NULL;
PFNGLWINDOWPOS2DVMESAPROC glWindowPos2dvMESA = NULL;
PFNGLWINDOWPOS2FMESAPROC glWindowPos2fMESA = NULL;
PFNGLWINDOWPOS2FVMESAPROC glWindowPos2fvMESA = NULL;
PFNGLWINDOWPOS2IMESAPROC glWindowPos2iMESA = NULL;
PFNGLWINDOWPOS2IVMESAPROC glWindowPos2ivMESA = NULL;
PFNGLWINDOWPOS2SMESAPROC glWindowPos2sMESA = NULL;
PFNGLWINDOWPOS2SVMESAPROC glWindowPos2svMESA = NULL;
PFNGLWINDOWPOS3DMESAPROC glWindowPos3dMESA = NULL;
PFNGLWINDOWPOS3DVMESAPROC glWindowPos3dvMESA = NULL;
PFNGLWINDOWPOS3FMESAPROC glWindowPos3fMESA = NULL;
PFNGLWINDOWPOS3FVMESAPROC glWindowPos3fvMESA = NULL;
PFNGLWINDOWPOS3IMESAPROC glWindowPos3iMESA = NULL;
PFNGLWINDOWPOS3IVMESAPROC glWindowPos3ivMESA = NULL;
PFNGLWINDOWPOS3SMESAPROC glWindowPos3sMESA = NULL;
PFNGLWINDOWPOS3SVMESAPROC glWindowPos3svMESA = NULL;
PFNGLWINDOWPOS4DMESAPROC glWindowPos4dMESA = NULL;
PFNGLWINDOWPOS4DVMESAPROC glWindowPos4dvMESA = NULL;
PFNGLWINDOWPOS4FMESAPROC glWindowPos4fMESA = NULL;
PFNGLWINDOWPOS4FVMESAPROC glWindowPos4fvMESA = NULL;
PFNGLWINDOWPOS4IMESAPROC glWindowPos4iMESA = NULL;
PFNGLWINDOWPOS4IVMESAPROC glWindowPos4ivMESA = NULL;
PFNGLWINDOWPOS4SMESAPROC glWindowPos4sMESA = NULL;
PFNGLWINDOWPOS4SVMESAPROC glWindowPos4svMESA = NULL;

PFNGLBEGINCONDITIONALRENDERNVPROC glBeginConditionalRenderNV = NULL;
PFNGLENDCONDITIONALRENDERNVPROC glEndConditionalRenderNV = NULL;

PFNGLCOPYIMAGESUBDATANVPROC glCopyImageSubDataNV = NULL;

PFNGLCLEARDEPTHDNVPROC glClearDepthdNV = NULL;
PFNGLDEPTHBOUNDSDNVPROC glDepthBoundsdNV = NULL;
PFNGLDEPTHRANGEDNVPROC glDepthRangedNV = NULL;

PFNGLEVALMAPSNVPROC glEvalMapsNV = NULL;
PFNGLGETMAPATTRIBPARAMETERFVNVPROC glGetMapAttribParameterfvNV = NULL;
PFNGLGETMAPATTRIBPARAMETERIVNVPROC glGetMapAttribParameterivNV = NULL;
PFNGLGETMAPCONTROLPOINTSNVPROC glGetMapControlPointsNV = NULL;
PFNGLGETMAPPARAMETERFVNVPROC glGetMapParameterfvNV = NULL;
PFNGLGETMAPPARAMETERIVNVPROC glGetMapParameterivNV = NULL;
PFNGLMAPCONTROLPOINTSNVPROC glMapControlPointsNV = NULL;
PFNGLMAPPARAMETERFVNVPROC glMapParameterfvNV = NULL;
PFNGLMAPPARAMETERIVNVPROC glMapParameterivNV = NULL;

PFNGLGETMULTISAMPLEFVNVPROC glGetMultisamplefvNV = NULL;
PFNGLSAMPLEMASKINDEXEDNVPROC glSampleMaskIndexedNV = NULL;
PFNGLTEXRENDERBUFFERNVPROC glTexRenderbufferNV = NULL;

PFNGLDELETEFENCESNVPROC glDeleteFencesNV = NULL;
PFNGLFINISHFENCENVPROC glFinishFenceNV = NULL;
PFNGLGENFENCESNVPROC glGenFencesNV = NULL;
PFNGLGETFENCEIVNVPROC glGetFenceivNV = NULL;
PFNGLISFENCENVPROC glIsFenceNV = NULL;
PFNGLSETFENCENVPROC glSetFenceNV = NULL;
PFNGLTESTFENCENVPROC glTestFenceNV = NULL;

PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC glGetProgramNamedParameterdvNV = NULL;
PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC glGetProgramNamedParameterfvNV = NULL;
PFNGLPROGRAMNAMEDPARAMETER4DNVPROC glProgramNamedParameter4dNV = NULL;
PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC glProgramNamedParameter4dvNV = NULL;
PFNGLPROGRAMNAMEDPARAMETER4FNVPROC glProgramNamedParameter4fNV = NULL;
PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC glProgramNamedParameter4fvNV = NULL;

PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC glRenderbufferStorageMultisampleCoverageNV = NULL;

PFNGLPROGRAMVERTEXLIMITNVPROC glProgramVertexLimitNV = NULL;

PFNGLPROGRAMENVPARAMETERI4INVPROC glProgramEnvParameterI4iNV = NULL;
PFNGLPROGRAMENVPARAMETERI4IVNVPROC glProgramEnvParameterI4ivNV = NULL;
PFNGLPROGRAMENVPARAMETERI4UINVPROC glProgramEnvParameterI4uiNV = NULL;
PFNGLPROGRAMENVPARAMETERI4UIVNVPROC glProgramEnvParameterI4uivNV = NULL;
PFNGLPROGRAMENVPARAMETERSI4IVNVPROC glProgramEnvParametersI4ivNV = NULL;
PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC glProgramEnvParametersI4uivNV = NULL;
PFNGLPROGRAMLOCALPARAMETERI4INVPROC glProgramLocalParameterI4iNV = NULL;
PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC glProgramLocalParameterI4ivNV = NULL;
PFNGLPROGRAMLOCALPARAMETERI4UINVPROC glProgramLocalParameterI4uiNV = NULL;
PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC glProgramLocalParameterI4uivNV = NULL;
PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC glProgramLocalParametersI4ivNV = NULL;
PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC glProgramLocalParametersI4uivNV = NULL;

PFNGLGETUNIFORMI64VNVPROC glGetUniformi64vNV = NULL;
PFNGLGETUNIFORMUI64VNVPROC glGetUniformui64vNV = NULL;
PFNGLPROGRAMUNIFORM1I64NVPROC glProgramUniform1i64NV = NULL;
PFNGLPROGRAMUNIFORM1I64VNVPROC glProgramUniform1i64vNV = NULL;
PFNGLPROGRAMUNIFORM1UI64NVPROC glProgramUniform1ui64NV = NULL;
PFNGLPROGRAMUNIFORM1UI64VNVPROC glProgramUniform1ui64vNV = NULL;
PFNGLPROGRAMUNIFORM2I64NVPROC glProgramUniform2i64NV = NULL;
PFNGLPROGRAMUNIFORM2I64VNVPROC glProgramUniform2i64vNV = NULL;
PFNGLPROGRAMUNIFORM2UI64NVPROC glProgramUniform2ui64NV = NULL;
PFNGLPROGRAMUNIFORM2UI64VNVPROC glProgramUniform2ui64vNV = NULL;
PFNGLPROGRAMUNIFORM3I64NVPROC glProgramUniform3i64NV = NULL;
PFNGLPROGRAMUNIFORM3I64VNVPROC glProgramUniform3i64vNV = NULL;
PFNGLPROGRAMUNIFORM3UI64NVPROC glProgramUniform3ui64NV = NULL;
PFNGLPROGRAMUNIFORM3UI64VNVPROC glProgramUniform3ui64vNV = NULL;
PFNGLPROGRAMUNIFORM4I64NVPROC glProgramUniform4i64NV = NULL;
PFNGLPROGRAMUNIFORM4I64VNVPROC glProgramUniform4i64vNV = NULL;
PFNGLPROGRAMUNIFORM4UI64NVPROC glProgramUniform4ui64NV = NULL;
PFNGLPROGRAMUNIFORM4UI64VNVPROC glProgramUniform4ui64vNV = NULL;
PFNGLUNIFORM1I64NVPROC glUniform1i64NV = NULL;
PFNGLUNIFORM1I64VNVPROC glUniform1i64vNV = NULL;
PFNGLUNIFORM1UI64NVPROC glUniform1ui64NV = NULL;
PFNGLUNIFORM1UI64VNVPROC glUniform1ui64vNV = NULL;
PFNGLUNIFORM2I64NVPROC glUniform2i64NV = NULL;
PFNGLUNIFORM2I64VNVPROC glUniform2i64vNV = NULL;
PFNGLUNIFORM2UI64NVPROC glUniform2ui64NV = NULL;
PFNGLUNIFORM2UI64VNVPROC glUniform2ui64vNV = NULL;
PFNGLUNIFORM3I64NVPROC glUniform3i64NV = NULL;
PFNGLUNIFORM3I64VNVPROC glUniform3i64vNV = NULL;
PFNGLUNIFORM3UI64NVPROC glUniform3ui64NV = NULL;
PFNGLUNIFORM3UI64VNVPROC glUniform3ui64vNV = NULL;
PFNGLUNIFORM4I64NVPROC glUniform4i64NV = NULL;
PFNGLUNIFORM4I64VNVPROC glUniform4i64vNV = NULL;
PFNGLUNIFORM4UI64NVPROC glUniform4ui64NV = NULL;
PFNGLUNIFORM4UI64VNVPROC glUniform4ui64vNV = NULL;

PFNGLCOLOR3HNVPROC glColor3hNV = NULL;
PFNGLCOLOR3HVNVPROC glColor3hvNV = NULL;
PFNGLCOLOR4HNVPROC glColor4hNV = NULL;
PFNGLCOLOR4HVNVPROC glColor4hvNV = NULL;
PFNGLFOGCOORDHNVPROC glFogCoordhNV = NULL;
PFNGLFOGCOORDHVNVPROC glFogCoordhvNV = NULL;
PFNGLMULTITEXCOORD1HNVPROC glMultiTexCoord1hNV = NULL;
PFNGLMULTITEXCOORD1HVNVPROC glMultiTexCoord1hvNV = NULL;
PFNGLMULTITEXCOORD2HNVPROC glMultiTexCoord2hNV = NULL;
PFNGLMULTITEXCOORD2HVNVPROC glMultiTexCoord2hvNV = NULL;
PFNGLMULTITEXCOORD3HNVPROC glMultiTexCoord3hNV = NULL;
PFNGLMULTITEXCOORD3HVNVPROC glMultiTexCoord3hvNV = NULL;
PFNGLMULTITEXCOORD4HNVPROC glMultiTexCoord4hNV = NULL;
PFNGLMULTITEXCOORD4HVNVPROC glMultiTexCoord4hvNV = NULL;
PFNGLNORMAL3HNVPROC glNormal3hNV = NULL;
PFNGLNORMAL3HVNVPROC glNormal3hvNV = NULL;
PFNGLSECONDARYCOLOR3HNVPROC glSecondaryColor3hNV = NULL;
PFNGLSECONDARYCOLOR3HVNVPROC glSecondaryColor3hvNV = NULL;
PFNGLTEXCOORD1HNVPROC glTexCoord1hNV = NULL;
PFNGLTEXCOORD1HVNVPROC glTexCoord1hvNV = NULL;
PFNGLTEXCOORD2HNVPROC glTexCoord2hNV = NULL;
PFNGLTEXCOORD2HVNVPROC glTexCoord2hvNV = NULL;
PFNGLTEXCOORD3HNVPROC glTexCoord3hNV = NULL;
PFNGLTEXCOORD3HVNVPROC glTexCoord3hvNV = NULL;
PFNGLTEXCOORD4HNVPROC glTexCoord4hNV = NULL;
PFNGLTEXCOORD4HVNVPROC glTexCoord4hvNV = NULL;
PFNGLVERTEX2HNVPROC glVertex2hNV = NULL;
PFNGLVERTEX2HVNVPROC glVertex2hvNV = NULL;
PFNGLVERTEX3HNVPROC glVertex3hNV = NULL;
PFNGLVERTEX3HVNVPROC glVertex3hvNV = NULL;
PFNGLVERTEX4HNVPROC glVertex4hNV = NULL;
PFNGLVERTEX4HVNVPROC glVertex4hvNV = NULL;
PFNGLVERTEXATTRIB1HNVPROC glVertexAttrib1hNV = NULL;
PFNGLVERTEXATTRIB1HVNVPROC glVertexAttrib1hvNV = NULL;
PFNGLVERTEXATTRIB2HNVPROC glVertexAttrib2hNV = NULL;
PFNGLVERTEXATTRIB2HVNVPROC glVertexAttrib2hvNV = NULL;
PFNGLVERTEXATTRIB3HNVPROC glVertexAttrib3hNV = NULL;
PFNGLVERTEXATTRIB3HVNVPROC glVertexAttrib3hvNV = NULL;
PFNGLVERTEXATTRIB4HNVPROC glVertexAttrib4hNV = NULL;
PFNGLVERTEXATTRIB4HVNVPROC glVertexAttrib4hvNV = NULL;
PFNGLVERTEXATTRIBS1HVNVPROC glVertexAttribs1hvNV = NULL;
PFNGLVERTEXATTRIBS2HVNVPROC glVertexAttribs2hvNV = NULL;
PFNGLVERTEXATTRIBS3HVNVPROC glVertexAttribs3hvNV = NULL;
PFNGLVERTEXATTRIBS4HVNVPROC glVertexAttribs4hvNV = NULL;
PFNGLVERTEXWEIGHTHNVPROC glVertexWeighthNV = NULL;
PFNGLVERTEXWEIGHTHVNVPROC glVertexWeighthvNV = NULL;

PFNGLBEGINOCCLUSIONQUERYNVPROC glBeginOcclusionQueryNV = NULL;
PFNGLDELETEOCCLUSIONQUERIESNVPROC glDeleteOcclusionQueriesNV = NULL;
PFNGLENDOCCLUSIONQUERYNVPROC glEndOcclusionQueryNV = NULL;
PFNGLGENOCCLUSIONQUERIESNVPROC glGenOcclusionQueriesNV = NULL;
PFNGLGETOCCLUSIONQUERYIVNVPROC glGetOcclusionQueryivNV = NULL;
PFNGLGETOCCLUSIONQUERYUIVNVPROC glGetOcclusionQueryuivNV = NULL;
PFNGLISOCCLUSIONQUERYNVPROC glIsOcclusionQueryNV = NULL;

PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC glProgramBufferParametersIivNV = NULL;
PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC glProgramBufferParametersIuivNV = NULL;
PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC glProgramBufferParametersfvNV = NULL;

PFNGLFLUSHPIXELDATARANGENVPROC glFlushPixelDataRangeNV = NULL;
PFNGLPIXELDATARANGENVPROC glPixelDataRangeNV = NULL;

PFNGLPOINTPARAMETERINVPROC glPointParameteriNV = NULL;
PFNGLPOINTPARAMETERIVNVPROC glPointParameterivNV = NULL;

PFNGLGETVIDEOI64VNVPROC glGetVideoi64vNV = NULL;
PFNGLGETVIDEOIVNVPROC glGetVideoivNV = NULL;
PFNGLGETVIDEOUI64VNVPROC glGetVideoui64vNV = NULL;
PFNGLGETVIDEOUIVNVPROC glGetVideouivNV = NULL;
PFNGLPRESENTFRAMEDUALFILLNVPROC glPresentFrameDualFillNV = NULL;
PFNGLPRESENTFRAMEKEYEDNVPROC glPresentFrameKeyedNV = NULL;

PFNGLPRIMITIVERESTARTINDEXNVPROC glPrimitiveRestartIndexNV = NULL;
PFNGLPRIMITIVERESTARTNVPROC glPrimitiveRestartNV = NULL;

PFNGLCOMBINERINPUTNVPROC glCombinerInputNV = NULL;
PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV = NULL;
PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV = NULL;
PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV = NULL;
PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV = NULL;
PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV = NULL;
PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV = NULL;
PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC glGetCombinerInputParameterfvNV = NULL;
PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC glGetCombinerInputParameterivNV = NULL;
PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC glGetCombinerOutputParameterfvNV = NULL;
PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC glGetCombinerOutputParameterivNV = NULL;
PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV = NULL;
PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV = NULL;

PFNGLCOMBINERSTAGEPARAMETERFVNVPROC glCombinerStageParameterfvNV = NULL;
PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC glGetCombinerStageParameterfvNV = NULL;

PFNGLGETBUFFERPARAMETERUI64VNVPROC glGetBufferParameterui64vNV = NULL;
PFNGLGETINTEGERUI64VNVPROC glGetIntegerui64vNV = NULL;
PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC glGetNamedBufferParameterui64vNV = NULL;
PFNGLISBUFFERRESIDENTNVPROC glIsBufferResidentNV = NULL;
PFNGLISNAMEDBUFFERRESIDENTNVPROC glIsNamedBufferResidentNV = NULL;
PFNGLMAKEBUFFERNONRESIDENTNVPROC glMakeBufferNonResidentNV = NULL;
PFNGLMAKEBUFFERRESIDENTNVPROC glMakeBufferResidentNV = NULL;
PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC glMakeNamedBufferNonResidentNV = NULL;
PFNGLMAKENAMEDBUFFERRESIDENTNVPROC glMakeNamedBufferResidentNV = NULL;
PFNGLPROGRAMUNIFORMUI64NVPROC glProgramUniformui64NV = NULL;
PFNGLPROGRAMUNIFORMUI64VNVPROC glProgramUniformui64vNV = NULL;
PFNGLUNIFORMUI64NVPROC glUniformui64NV = NULL;
PFNGLUNIFORMUI64VNVPROC glUniformui64vNV = NULL;

PFNGLTEXTUREBARRIERNVPROC glTextureBarrierNV = NULL;

PFNGLACTIVEVARYINGNVPROC glActiveVaryingNV = NULL;
PFNGLBEGINTRANSFORMFEEDBACKNVPROC glBeginTransformFeedbackNV = NULL;
PFNGLBINDBUFFERBASENVPROC glBindBufferBaseNV = NULL;
PFNGLBINDBUFFEROFFSETNVPROC glBindBufferOffsetNV = NULL;
PFNGLBINDBUFFERRANGENVPROC glBindBufferRangeNV = NULL;
PFNGLENDTRANSFORMFEEDBACKNVPROC glEndTransformFeedbackNV = NULL;
PFNGLGETACTIVEVARYINGNVPROC glGetActiveVaryingNV = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC glGetTransformFeedbackVaryingNV = NULL;
PFNGLGETVARYINGLOCATIONNVPROC glGetVaryingLocationNV = NULL;
PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC glTransformFeedbackAttribsNV = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC glTransformFeedbackVaryingsNV = NULL;

PFNGLBINDTRANSFORMFEEDBACKNVPROC glBindTransformFeedbackNV = NULL;
PFNGLDELETETRANSFORMFEEDBACKSNVPROC glDeleteTransformFeedbacksNV = NULL;
PFNGLDRAWTRANSFORMFEEDBACKNVPROC glDrawTransformFeedbackNV = NULL;
PFNGLGENTRANSFORMFEEDBACKSNVPROC glGenTransformFeedbacksNV = NULL;
PFNGLISTRANSFORMFEEDBACKNVPROC glIsTransformFeedbackNV = NULL;
PFNGLPAUSETRANSFORMFEEDBACKNVPROC glPauseTransformFeedbackNV = NULL;
PFNGLRESUMETRANSFORMFEEDBACKNVPROC glResumeTransformFeedbackNV = NULL;

PFNGLVDPAUFININVPROC glVDPAUFiniNV = NULL;
PFNGLVDPAUGETSURFACEIVNVPROC glVDPAUGetSurfaceivNV = NULL;
PFNGLVDPAUINITNVPROC glVDPAUInitNV = NULL;
PFNGLVDPAUISSURFACENVPROC glVDPAUIsSurfaceNV = NULL;
PFNGLVDPAUMAPSURFACESNVPROC glVDPAUMapSurfacesNV = NULL;
PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC glVDPAURegisterOutputSurfaceNV = NULL;
PFNGLVDPAUREGISTERVIDEOSURFACENVPROC glVDPAURegisterVideoSurfaceNV = NULL;
PFNGLVDPAUSURFACEACCESSNVPROC glVDPAUSurfaceAccessNV = NULL;
PFNGLVDPAUUNMAPSURFACESNVPROC glVDPAUUnmapSurfacesNV = NULL;
PFNGLVDPAUUNREGISTERSURFACENVPROC glVDPAUUnregisterSurfaceNV = NULL;

PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV = NULL;
PFNGLVERTEXARRAYRANGENVPROC glVertexArrayRangeNV = NULL;

PFNGLGETVERTEXATTRIBLI64VNVPROC glGetVertexAttribLi64vNV = NULL;
PFNGLGETVERTEXATTRIBLUI64VNVPROC glGetVertexAttribLui64vNV = NULL;
PFNGLVERTEXATTRIBL1I64NVPROC glVertexAttribL1i64NV = NULL;
PFNGLVERTEXATTRIBL1I64VNVPROC glVertexAttribL1i64vNV = NULL;
PFNGLVERTEXATTRIBL1UI64NVPROC glVertexAttribL1ui64NV = NULL;
PFNGLVERTEXATTRIBL1UI64VNVPROC glVertexAttribL1ui64vNV = NULL;
PFNGLVERTEXATTRIBL2I64NVPROC glVertexAttribL2i64NV = NULL;
PFNGLVERTEXATTRIBL2I64VNVPROC glVertexAttribL2i64vNV = NULL;
PFNGLVERTEXATTRIBL2UI64NVPROC glVertexAttribL2ui64NV = NULL;
PFNGLVERTEXATTRIBL2UI64VNVPROC glVertexAttribL2ui64vNV = NULL;
PFNGLVERTEXATTRIBL3I64NVPROC glVertexAttribL3i64NV = NULL;
PFNGLVERTEXATTRIBL3I64VNVPROC glVertexAttribL3i64vNV = NULL;
PFNGLVERTEXATTRIBL3UI64NVPROC glVertexAttribL3ui64NV = NULL;
PFNGLVERTEXATTRIBL3UI64VNVPROC glVertexAttribL3ui64vNV = NULL;
PFNGLVERTEXATTRIBL4I64NVPROC glVertexAttribL4i64NV = NULL;
PFNGLVERTEXATTRIBL4I64VNVPROC glVertexAttribL4i64vNV = NULL;
PFNGLVERTEXATTRIBL4UI64NVPROC glVertexAttribL4ui64NV = NULL;
PFNGLVERTEXATTRIBL4UI64VNVPROC glVertexAttribL4ui64vNV = NULL;
PFNGLVERTEXATTRIBLFORMATNVPROC glVertexAttribLFormatNV = NULL;

PFNGLBUFFERADDRESSRANGENVPROC glBufferAddressRangeNV = NULL;
PFNGLCOLORFORMATNVPROC glColorFormatNV = NULL;
PFNGLEDGEFLAGFORMATNVPROC glEdgeFlagFormatNV = NULL;
PFNGLFOGCOORDFORMATNVPROC glFogCoordFormatNV = NULL;
PFNGLGETINTEGERUI64I_VNVPROC glGetIntegerui64i_vNV = NULL;
PFNGLINDEXFORMATNVPROC glIndexFormatNV = NULL;
PFNGLNORMALFORMATNVPROC glNormalFormatNV = NULL;
PFNGLSECONDARYCOLORFORMATNVPROC glSecondaryColorFormatNV = NULL;
PFNGLTEXCOORDFORMATNVPROC glTexCoordFormatNV = NULL;
PFNGLVERTEXATTRIBFORMATNVPROC glVertexAttribFormatNV = NULL;
PFNGLVERTEXATTRIBIFORMATNVPROC glVertexAttribIFormatNV = NULL;
PFNGLVERTEXFORMATNVPROC glVertexFormatNV = NULL;

PFNGLAREPROGRAMSRESIDENTNVPROC glAreProgramsResidentNV = NULL;
PFNGLBINDPROGRAMNVPROC glBindProgramNV = NULL;
PFNGLDELETEPROGRAMSNVPROC glDeleteProgramsNV = NULL;
PFNGLEXECUTEPROGRAMNVPROC glExecuteProgramNV = NULL;
PFNGLGENPROGRAMSNVPROC glGenProgramsNV = NULL;
PFNGLGETPROGRAMPARAMETERDVNVPROC glGetProgramParameterdvNV = NULL;
PFNGLGETPROGRAMPARAMETERFVNVPROC glGetProgramParameterfvNV = NULL;
PFNGLGETPROGRAMSTRINGNVPROC glGetProgramStringNV = NULL;
PFNGLGETPROGRAMIVNVPROC glGetProgramivNV = NULL;
PFNGLGETTRACKMATRIXIVNVPROC glGetTrackMatrixivNV = NULL;
PFNGLGETVERTEXATTRIBPOINTERVNVPROC glGetVertexAttribPointervNV = NULL;
PFNGLGETVERTEXATTRIBDVNVPROC glGetVertexAttribdvNV = NULL;
PFNGLGETVERTEXATTRIBFVNVPROC glGetVertexAttribfvNV = NULL;
PFNGLGETVERTEXATTRIBIVNVPROC glGetVertexAttribivNV = NULL;
PFNGLISPROGRAMNVPROC glIsProgramNV = NULL;
PFNGLLOADPROGRAMNVPROC glLoadProgramNV = NULL;
PFNGLPROGRAMPARAMETER4DNVPROC glProgramParameter4dNV = NULL;
PFNGLPROGRAMPARAMETER4DVNVPROC glProgramParameter4dvNV = NULL;
PFNGLPROGRAMPARAMETER4FNVPROC glProgramParameter4fNV = NULL;
PFNGLPROGRAMPARAMETER4FVNVPROC glProgramParameter4fvNV = NULL;
PFNGLPROGRAMPARAMETERS4DVNVPROC glProgramParameters4dvNV = NULL;
PFNGLPROGRAMPARAMETERS4FVNVPROC glProgramParameters4fvNV = NULL;
PFNGLREQUESTRESIDENTPROGRAMSNVPROC glRequestResidentProgramsNV = NULL;
PFNGLTRACKMATRIXNVPROC glTrackMatrixNV = NULL;
PFNGLVERTEXATTRIB1DNVPROC glVertexAttrib1dNV = NULL;
PFNGLVERTEXATTRIB1DVNVPROC glVertexAttrib1dvNV = NULL;
PFNGLVERTEXATTRIB1FNVPROC glVertexAttrib1fNV = NULL;
PFNGLVERTEXATTRIB1FVNVPROC glVertexAttrib1fvNV = NULL;
PFNGLVERTEXATTRIB1SNVPROC glVertexAttrib1sNV = NULL;
PFNGLVERTEXATTRIB1SVNVPROC glVertexAttrib1svNV = NULL;
PFNGLVERTEXATTRIB2DNVPROC glVertexAttrib2dNV = NULL;
PFNGLVERTEXATTRIB2DVNVPROC glVertexAttrib2dvNV = NULL;
PFNGLVERTEXATTRIB2FNVPROC glVertexAttrib2fNV = NULL;
PFNGLVERTEXATTRIB2FVNVPROC glVertexAttrib2fvNV = NULL;
PFNGLVERTEXATTRIB2SNVPROC glVertexAttrib2sNV = NULL;
PFNGLVERTEXATTRIB2SVNVPROC glVertexAttrib2svNV = NULL;
PFNGLVERTEXATTRIB3DNVPROC glVertexAttrib3dNV = NULL;
PFNGLVERTEXATTRIB3DVNVPROC glVertexAttrib3dvNV = NULL;
PFNGLVERTEXATTRIB3FNVPROC glVertexAttrib3fNV = NULL;
PFNGLVERTEXATTRIB3FVNVPROC glVertexAttrib3fvNV = NULL;
PFNGLVERTEXATTRIB3SNVPROC glVertexAttrib3sNV = NULL;
PFNGLVERTEXATTRIB3SVNVPROC glVertexAttrib3svNV = NULL;
PFNGLVERTEXATTRIB4DNVPROC glVertexAttrib4dNV = NULL;
PFNGLVERTEXATTRIB4DVNVPROC glVertexAttrib4dvNV = NULL;
PFNGLVERTEXATTRIB4FNVPROC glVertexAttrib4fNV = NULL;
PFNGLVERTEXATTRIB4FVNVPROC glVertexAttrib4fvNV = NULL;
PFNGLVERTEXATTRIB4SNVPROC glVertexAttrib4sNV = NULL;
PFNGLVERTEXATTRIB4SVNVPROC glVertexAttrib4svNV = NULL;
PFNGLVERTEXATTRIB4UBNVPROC glVertexAttrib4ubNV = NULL;
PFNGLVERTEXATTRIB4UBVNVPROC glVertexAttrib4ubvNV = NULL;
PFNGLVERTEXATTRIBPOINTERNVPROC glVertexAttribPointerNV = NULL;
PFNGLVERTEXATTRIBS1DVNVPROC glVertexAttribs1dvNV = NULL;
PFNGLVERTEXATTRIBS1FVNVPROC glVertexAttribs1fvNV = NULL;
PFNGLVERTEXATTRIBS1SVNVPROC glVertexAttribs1svNV = NULL;
PFNGLVERTEXATTRIBS2DVNVPROC glVertexAttribs2dvNV = NULL;
PFNGLVERTEXATTRIBS2FVNVPROC glVertexAttribs2fvNV = NULL;
PFNGLVERTEXATTRIBS2SVNVPROC glVertexAttribs2svNV = NULL;
PFNGLVERTEXATTRIBS3DVNVPROC glVertexAttribs3dvNV = NULL;
PFNGLVERTEXATTRIBS3FVNVPROC glVertexAttribs3fvNV = NULL;
PFNGLVERTEXATTRIBS3SVNVPROC glVertexAttribs3svNV = NULL;
PFNGLVERTEXATTRIBS4DVNVPROC glVertexAttribs4dvNV = NULL;
PFNGLVERTEXATTRIBS4FVNVPROC glVertexAttribs4fvNV = NULL;
PFNGLVERTEXATTRIBS4SVNVPROC glVertexAttribs4svNV = NULL;
PFNGLVERTEXATTRIBS4UBVNVPROC glVertexAttribs4ubvNV = NULL;

PFNGLCLEARDEPTHFOESPROC glClearDepthfOES = NULL;
PFNGLCLIPPLANEFOESPROC glClipPlanefOES = NULL;
PFNGLDEPTHRANGEFOESPROC glDepthRangefOES = NULL;
PFNGLFRUSTUMFOESPROC glFrustumfOES = NULL;
PFNGLGETCLIPPLANEFOESPROC glGetClipPlanefOES = NULL;
PFNGLORTHOFOESPROC glOrthofOES = NULL;

PFNGLDETAILTEXFUNCSGISPROC glDetailTexFuncSGIS = NULL;
PFNGLGETDETAILTEXFUNCSGISPROC glGetDetailTexFuncSGIS = NULL;

PFNGLFOGFUNCSGISPROC glFogFuncSGIS = NULL;
PFNGLGETFOGFUNCSGISPROC glGetFogFuncSGIS = NULL;

PFNGLSAMPLEMASKSGISPROC glSampleMaskSGIS = NULL;
PFNGLSAMPLEPATTERNSGISPROC glSamplePatternSGIS = NULL;

PFNGLGETSHARPENTEXFUNCSGISPROC glGetSharpenTexFuncSGIS = NULL;
PFNGLSHARPENTEXFUNCSGISPROC glSharpenTexFuncSGIS = NULL;

PFNGLTEXIMAGE4DSGISPROC glTexImage4DSGIS = NULL;
PFNGLTEXSUBIMAGE4DSGISPROC glTexSubImage4DSGIS = NULL;

PFNGLGETTEXFILTERFUNCSGISPROC glGetTexFilterFuncSGIS = NULL;
PFNGLTEXFILTERFUNCSGISPROC glTexFilterFuncSGIS = NULL;

PFNGLASYNCMARKERSGIXPROC glAsyncMarkerSGIX = NULL;
PFNGLDELETEASYNCMARKERSSGIXPROC glDeleteAsyncMarkersSGIX = NULL;
PFNGLFINISHASYNCSGIXPROC glFinishAsyncSGIX = NULL;
PFNGLGENASYNCMARKERSSGIXPROC glGenAsyncMarkersSGIX = NULL;
PFNGLISASYNCMARKERSGIXPROC glIsAsyncMarkerSGIX = NULL;
PFNGLPOLLASYNCSGIXPROC glPollAsyncSGIX = NULL;

PFNGLFLUSHRASTERSGIXPROC glFlushRasterSGIX = NULL;

PFNGLTEXTUREFOGSGIXPROC glTextureFogSGIX = NULL;

PFNGLFRAGMENTCOLORMATERIALSGIXPROC glFragmentColorMaterialSGIX = NULL;
PFNGLFRAGMENTLIGHTMODELFSGIXPROC glFragmentLightModelfSGIX = NULL;
PFNGLFRAGMENTLIGHTMODELFVSGIXPROC glFragmentLightModelfvSGIX = NULL;
PFNGLFRAGMENTLIGHTMODELISGIXPROC glFragmentLightModeliSGIX = NULL;
PFNGLFRAGMENTLIGHTMODELIVSGIXPROC glFragmentLightModelivSGIX = NULL;
PFNGLFRAGMENTLIGHTFSGIXPROC glFragmentLightfSGIX = NULL;
PFNGLFRAGMENTLIGHTFVSGIXPROC glFragmentLightfvSGIX = NULL;
PFNGLFRAGMENTLIGHTISGIXPROC glFragmentLightiSGIX = NULL;
PFNGLFRAGMENTLIGHTIVSGIXPROC glFragmentLightivSGIX = NULL;
PFNGLFRAGMENTMATERIALFSGIXPROC glFragmentMaterialfSGIX = NULL;
PFNGLFRAGMENTMATERIALFVSGIXPROC glFragmentMaterialfvSGIX = NULL;
PFNGLFRAGMENTMATERIALISGIXPROC glFragmentMaterialiSGIX = NULL;
PFNGLFRAGMENTMATERIALIVSGIXPROC glFragmentMaterialivSGIX = NULL;
PFNGLGETFRAGMENTLIGHTFVSGIXPROC glGetFragmentLightfvSGIX = NULL;
PFNGLGETFRAGMENTLIGHTIVSGIXPROC glGetFragmentLightivSGIX = NULL;
PFNGLGETFRAGMENTMATERIALFVSGIXPROC glGetFragmentMaterialfvSGIX = NULL;
PFNGLGETFRAGMENTMATERIALIVSGIXPROC glGetFragmentMaterialivSGIX = NULL;

PFNGLFRAMEZOOMSGIXPROC glFrameZoomSGIX = NULL;

PFNGLPIXELTEXGENSGIXPROC glPixelTexGenSGIX = NULL;

PFNGLREFERENCEPLANESGIXPROC glReferencePlaneSGIX = NULL;

PFNGLSPRITEPARAMETERFSGIXPROC glSpriteParameterfSGIX = NULL;
PFNGLSPRITEPARAMETERFVSGIXPROC glSpriteParameterfvSGIX = NULL;
PFNGLSPRITEPARAMETERISGIXPROC glSpriteParameteriSGIX = NULL;
PFNGLSPRITEPARAMETERIVSGIXPROC glSpriteParameterivSGIX = NULL;

PFNGLTAGSAMPLEBUFFERSGIXPROC glTagSampleBufferSGIX = NULL;

PFNGLCOLORTABLEPARAMETERFVSGIPROC glColorTableParameterfvSGI = NULL;
PFNGLCOLORTABLEPARAMETERIVSGIPROC glColorTableParameterivSGI = NULL;
PFNGLCOLORTABLESGIPROC glColorTableSGI = NULL;
PFNGLCOPYCOLORTABLESGIPROC glCopyColorTableSGI = NULL;
PFNGLGETCOLORTABLEPARAMETERFVSGIPROC glGetColorTableParameterfvSGI = NULL;
PFNGLGETCOLORTABLEPARAMETERIVSGIPROC glGetColorTableParameterivSGI = NULL;
PFNGLGETCOLORTABLESGIPROC glGetColorTableSGI = NULL;

PFNGLFINISHTEXTURESUNXPROC glFinishTextureSUNX = NULL;

PFNGLGLOBALALPHAFACTORBSUNPROC glGlobalAlphaFactorbSUN = NULL;
PFNGLGLOBALALPHAFACTORDSUNPROC glGlobalAlphaFactordSUN = NULL;
PFNGLGLOBALALPHAFACTORFSUNPROC glGlobalAlphaFactorfSUN = NULL;
PFNGLGLOBALALPHAFACTORISUNPROC glGlobalAlphaFactoriSUN = NULL;
PFNGLGLOBALALPHAFACTORSSUNPROC glGlobalAlphaFactorsSUN = NULL;
PFNGLGLOBALALPHAFACTORUBSUNPROC glGlobalAlphaFactorubSUN = NULL;
PFNGLGLOBALALPHAFACTORUISUNPROC glGlobalAlphaFactoruiSUN = NULL;
PFNGLGLOBALALPHAFACTORUSSUNPROC glGlobalAlphaFactorusSUN = NULL;

PFNGLREADVIDEOPIXELSSUNPROC glReadVideoPixelsSUN = NULL;

PFNGLREPLACEMENTCODEPOINTERSUNPROC glReplacementCodePointerSUN = NULL;
PFNGLREPLACEMENTCODEUBSUNPROC glReplacementCodeubSUN = NULL;
PFNGLREPLACEMENTCODEUBVSUNPROC glReplacementCodeubvSUN = NULL;
PFNGLREPLACEMENTCODEUISUNPROC glReplacementCodeuiSUN = NULL;
PFNGLREPLACEMENTCODEUIVSUNPROC glReplacementCodeuivSUN = NULL;
PFNGLREPLACEMENTCODEUSSUNPROC glReplacementCodeusSUN = NULL;
PFNGLREPLACEMENTCODEUSVSUNPROC glReplacementCodeusvSUN = NULL;

PFNGLCOLOR3FVERTEX3FSUNPROC glColor3fVertex3fSUN = NULL;
PFNGLCOLOR3FVERTEX3FVSUNPROC glColor3fVertex3fvSUN = NULL;
PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC glColor4fNormal3fVertex3fSUN = NULL;
PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC glColor4fNormal3fVertex3fvSUN = NULL;
PFNGLCOLOR4UBVERTEX2FSUNPROC glColor4ubVertex2fSUN = NULL;
PFNGLCOLOR4UBVERTEX2FVSUNPROC glColor4ubVertex2fvSUN = NULL;
PFNGLCOLOR4UBVERTEX3FSUNPROC glColor4ubVertex3fSUN = NULL;
PFNGLCOLOR4UBVERTEX3FVSUNPROC glColor4ubVertex3fvSUN = NULL;
PFNGLNORMAL3FVERTEX3FSUNPROC glNormal3fVertex3fSUN = NULL;
PFNGLNORMAL3FVERTEX3FVSUNPROC glNormal3fVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC glReplacementCodeuiColor3fVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC glReplacementCodeuiColor3fVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC glReplacementCodeuiColor4ubVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC glReplacementCodeuiColor4ubVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC glReplacementCodeuiNormal3fVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiNormal3fVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC glReplacementCodeuiTexCoord2fVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC glReplacementCodeuiTexCoord2fVertex3fvSUN = NULL;
PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC glReplacementCodeuiVertex3fSUN = NULL;
PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC glReplacementCodeuiVertex3fvSUN = NULL;
PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC glTexCoord2fColor3fVertex3fSUN = NULL;
PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC glTexCoord2fColor3fVertex3fvSUN = NULL;
PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC glTexCoord2fColor4fNormal3fVertex3fSUN = NULL;
PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC glTexCoord2fColor4fNormal3fVertex3fvSUN = NULL;
PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC glTexCoord2fColor4ubVertex3fSUN = NULL;
PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC glTexCoord2fColor4ubVertex3fvSUN = NULL;
PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC glTexCoord2fNormal3fVertex3fSUN = NULL;
PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC glTexCoord2fNormal3fVertex3fvSUN = NULL;
PFNGLTEXCOORD2FVERTEX3FSUNPROC glTexCoord2fVertex3fSUN = NULL;
PFNGLTEXCOORD2FVERTEX3FVSUNPROC glTexCoord2fVertex3fvSUN = NULL;
PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC glTexCoord4fColor4fNormal3fVertex4fSUN = NULL;
PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC glTexCoord4fColor4fNormal3fVertex4fvSUN = NULL;
PFNGLTEXCOORD4FVERTEX4FSUNPROC glTexCoord4fVertex4fSUN = NULL;
PFNGLTEXCOORD4FVERTEX4FVSUNPROC glTexCoord4fVertex4fvSUN = NULL;

PFNGLADDSWAPHINTRECTWINPROC glAddSwapHintRectWIN = NULL;

#endif /* !WIN32 || !GLEW_MX */

#if !defined(GLEW_MX)
GLboolean __GLEW_VERSION_1_1 = GL_FALSE;
GLboolean __GLEW_VERSION_1_2 = GL_FALSE;
GLboolean __GLEW_VERSION_1_2_1 = GL_FALSE;
GLboolean __GLEW_VERSION_1_3 = GL_FALSE;
GLboolean __GLEW_VERSION_1_4 = GL_FALSE;
GLboolean __GLEW_VERSION_1_5 = GL_FALSE;
GLboolean __GLEW_VERSION_2_0 = GL_FALSE;
GLboolean __GLEW_VERSION_2_1 = GL_FALSE;
GLboolean __GLEW_VERSION_3_0 = GL_FALSE;
GLboolean __GLEW_VERSION_3_1 = GL_FALSE;
GLboolean __GLEW_VERSION_3_2 = GL_FALSE;
GLboolean __GLEW_VERSION_3_3 = GL_FALSE;
GLboolean __GLEW_VERSION_4_0 = GL_FALSE;
GLboolean __GLEW_VERSION_4_1 = GL_FALSE;
GLboolean __GLEW_3DFX_multisample = GL_FALSE;
GLboolean __GLEW_3DFX_tbuffer = GL_FALSE;
GLboolean __GLEW_3DFX_texture_compression_FXT1 = GL_FALSE;
GLboolean __GLEW_AMD_conservative_depth = GL_FALSE;
GLboolean __GLEW_AMD_debug_output = GL_FALSE;
GLboolean __GLEW_AMD_depth_clamp_separate = GL_FALSE;
GLboolean __GLEW_AMD_draw_buffers_blend = GL_FALSE;
GLboolean __GLEW_AMD_name_gen_delete = GL_FALSE;
GLboolean __GLEW_AMD_performance_monitor = GL_FALSE;
GLboolean __GLEW_AMD_seamless_cubemap_per_texture = GL_FALSE;
GLboolean __GLEW_AMD_shader_stencil_export = GL_FALSE;
GLboolean __GLEW_AMD_texture_texture4 = GL_FALSE;
GLboolean __GLEW_AMD_transform_feedback3_lines_triangles = GL_FALSE;
GLboolean __GLEW_AMD_vertex_shader_tessellator = GL_FALSE;
GLboolean __GLEW_APPLE_aux_depth_stencil = GL_FALSE;
GLboolean __GLEW_APPLE_client_storage = GL_FALSE;
GLboolean __GLEW_APPLE_element_array = GL_FALSE;
GLboolean __GLEW_APPLE_fence = GL_FALSE;
GLboolean __GLEW_APPLE_float_pixels = GL_FALSE;
GLboolean __GLEW_APPLE_flush_buffer_range = GL_FALSE;
GLboolean __GLEW_APPLE_object_purgeable = GL_FALSE;
GLboolean __GLEW_APPLE_pixel_buffer = GL_FALSE;
GLboolean __GLEW_APPLE_rgb_422 = GL_FALSE;
GLboolean __GLEW_APPLE_row_bytes = GL_FALSE;
GLboolean __GLEW_APPLE_specular_vector = GL_FALSE;
GLboolean __GLEW_APPLE_texture_range = GL_FALSE;
GLboolean __GLEW_APPLE_transform_hint = GL_FALSE;
GLboolean __GLEW_APPLE_vertex_array_object = GL_FALSE;
GLboolean __GLEW_APPLE_vertex_array_range = GL_FALSE;
GLboolean __GLEW_APPLE_vertex_program_evaluators = GL_FALSE;
GLboolean __GLEW_APPLE_ycbcr_422 = GL_FALSE;
GLboolean __GLEW_ARB_ES2_compatibility = GL_FALSE;
GLboolean __GLEW_ARB_blend_func_extended = GL_FALSE;
GLboolean __GLEW_ARB_cl_event = GL_FALSE;
GLboolean __GLEW_ARB_color_buffer_float = GL_FALSE;
GLboolean __GLEW_ARB_compatibility = GL_FALSE;
GLboolean __GLEW_ARB_copy_buffer = GL_FALSE;
GLboolean __GLEW_ARB_debug_output = GL_FALSE;
GLboolean __GLEW_ARB_depth_buffer_float = GL_FALSE;
GLboolean __GLEW_ARB_depth_clamp = GL_FALSE;
GLboolean __GLEW_ARB_depth_texture = GL_FALSE;
GLboolean __GLEW_ARB_draw_buffers = GL_FALSE;
GLboolean __GLEW_ARB_draw_buffers_blend = GL_FALSE;
GLboolean __GLEW_ARB_draw_elements_base_vertex = GL_FALSE;
GLboolean __GLEW_ARB_draw_indirect = GL_FALSE;
GLboolean __GLEW_ARB_draw_instanced = GL_FALSE;
GLboolean __GLEW_ARB_explicit_attrib_location = GL_FALSE;
GLboolean __GLEW_ARB_fragment_coord_conventions = GL_FALSE;
GLboolean __GLEW_ARB_fragment_program = GL_FALSE;
GLboolean __GLEW_ARB_fragment_program_shadow = GL_FALSE;
GLboolean __GLEW_ARB_fragment_shader = GL_FALSE;
GLboolean __GLEW_ARB_framebuffer_object = GL_FALSE;
GLboolean __GLEW_ARB_framebuffer_sRGB = GL_FALSE;
GLboolean __GLEW_ARB_geometry_shader4 = GL_FALSE;
GLboolean __GLEW_ARB_get_program_binary = GL_FALSE;
GLboolean __GLEW_ARB_gpu_shader5 = GL_FALSE;
GLboolean __GLEW_ARB_gpu_shader_fp64 = GL_FALSE;
GLboolean __GLEW_ARB_half_float_pixel = GL_FALSE;
GLboolean __GLEW_ARB_half_float_vertex = GL_FALSE;
GLboolean __GLEW_ARB_imaging = GL_FALSE;
GLboolean __GLEW_ARB_instanced_arrays = GL_FALSE;
GLboolean __GLEW_ARB_map_buffer_range = GL_FALSE;
GLboolean __GLEW_ARB_matrix_palette = GL_FALSE;
GLboolean __GLEW_ARB_multisample = GL_FALSE;
GLboolean __GLEW_ARB_multitexture = GL_FALSE;
GLboolean __GLEW_ARB_occlusion_query = GL_FALSE;
GLboolean __GLEW_ARB_occlusion_query2 = GL_FALSE;
GLboolean __GLEW_ARB_pixel_buffer_object = GL_FALSE;
GLboolean __GLEW_ARB_point_parameters = GL_FALSE;
GLboolean __GLEW_ARB_point_sprite = GL_FALSE;
GLboolean __GLEW_ARB_provoking_vertex = GL_FALSE;
GLboolean __GLEW_ARB_robustness = GL_FALSE;
GLboolean __GLEW_ARB_sample_shading = GL_FALSE;
GLboolean __GLEW_ARB_sampler_objects = GL_FALSE;
GLboolean __GLEW_ARB_seamless_cube_map = GL_FALSE;
GLboolean __GLEW_ARB_separate_shader_objects = GL_FALSE;
GLboolean __GLEW_ARB_shader_bit_encoding = GL_FALSE;
GLboolean __GLEW_ARB_shader_objects = GL_FALSE;
GLboolean __GLEW_ARB_shader_precision = GL_FALSE;
GLboolean __GLEW_ARB_shader_stencil_export = GL_FALSE;
GLboolean __GLEW_ARB_shader_subroutine = GL_FALSE;
GLboolean __GLEW_ARB_shader_texture_lod = GL_FALSE;
GLboolean __GLEW_ARB_shading_language_100 = GL_FALSE;
GLboolean __GLEW_ARB_shading_language_include = GL_FALSE;
GLboolean __GLEW_ARB_shadow = GL_FALSE;
GLboolean __GLEW_ARB_shadow_ambient = GL_FALSE;
GLboolean __GLEW_ARB_sync = GL_FALSE;
GLboolean __GLEW_ARB_tessellation_shader = GL_FALSE;
GLboolean __GLEW_ARB_texture_border_clamp = GL_FALSE;
GLboolean __GLEW_ARB_texture_buffer_object = GL_FALSE;
GLboolean __GLEW_ARB_texture_buffer_object_rgb32 = GL_FALSE;
GLboolean __GLEW_ARB_texture_compression = GL_FALSE;
GLboolean __GLEW_ARB_texture_compression_bptc = GL_FALSE;
GLboolean __GLEW_ARB_texture_compression_rgtc = GL_FALSE;
GLboolean __GLEW_ARB_texture_cube_map = GL_FALSE;
GLboolean __GLEW_ARB_texture_cube_map_array = GL_FALSE;
GLboolean __GLEW_ARB_texture_env_add = GL_FALSE;
GLboolean __GLEW_ARB_texture_env_combine = GL_FALSE;
GLboolean __GLEW_ARB_texture_env_crossbar = GL_FALSE;
GLboolean __GLEW_ARB_texture_env_dot3 = GL_FALSE;
GLboolean __GLEW_ARB_texture_float = GL_FALSE;
GLboolean __GLEW_ARB_texture_gather = GL_FALSE;
GLboolean __GLEW_ARB_texture_mirrored_repeat = GL_FALSE;
GLboolean __GLEW_ARB_texture_multisample = GL_FALSE;
GLboolean __GLEW_ARB_texture_non_power_of_two = GL_FALSE;
GLboolean __GLEW_ARB_texture_query_lod = GL_FALSE;
GLboolean __GLEW_ARB_texture_rectangle = GL_FALSE;
GLboolean __GLEW_ARB_texture_rg = GL_FALSE;
GLboolean __GLEW_ARB_texture_rgb10_a2ui = GL_FALSE;
GLboolean __GLEW_ARB_texture_swizzle = GL_FALSE;
GLboolean __GLEW_ARB_timer_query = GL_FALSE;
GLboolean __GLEW_ARB_transform_feedback2 = GL_FALSE;
GLboolean __GLEW_ARB_transform_feedback3 = GL_FALSE;
GLboolean __GLEW_ARB_transpose_matrix = GL_FALSE;
GLboolean __GLEW_ARB_uniform_buffer_object = GL_FALSE;
GLboolean __GLEW_ARB_vertex_array_bgra = GL_FALSE;
GLboolean __GLEW_ARB_vertex_array_object = GL_FALSE;
GLboolean __GLEW_ARB_vertex_attrib_64bit = GL_FALSE;
GLboolean __GLEW_ARB_vertex_blend = GL_FALSE;
GLboolean __GLEW_ARB_vertex_buffer_object = GL_FALSE;
GLboolean __GLEW_ARB_vertex_program = GL_FALSE;
GLboolean __GLEW_ARB_vertex_shader = GL_FALSE;
GLboolean __GLEW_ARB_vertex_type_2_10_10_10_rev = GL_FALSE;
GLboolean __GLEW_ARB_viewport_array = GL_FALSE;
GLboolean __GLEW_ARB_window_pos = GL_FALSE;
GLboolean __GLEW_ATIX_point_sprites = GL_FALSE;
GLboolean __GLEW_ATIX_texture_env_combine3 = GL_FALSE;
GLboolean __GLEW_ATIX_texture_env_route = GL_FALSE;
GLboolean __GLEW_ATIX_vertex_shader_output_point_size = GL_FALSE;
GLboolean __GLEW_ATI_draw_buffers = GL_FALSE;
GLboolean __GLEW_ATI_element_array = GL_FALSE;
GLboolean __GLEW_ATI_envmap_bumpmap = GL_FALSE;
GLboolean __GLEW_ATI_fragment_shader = GL_FALSE;
GLboolean __GLEW_ATI_map_object_buffer = GL_FALSE;
GLboolean __GLEW_ATI_meminfo = GL_FALSE;
GLboolean __GLEW_ATI_pn_triangles = GL_FALSE;
GLboolean __GLEW_ATI_separate_stencil = GL_FALSE;
GLboolean __GLEW_ATI_shader_texture_lod = GL_FALSE;
GLboolean __GLEW_ATI_text_fragment_shader = GL_FALSE;
GLboolean __GLEW_ATI_texture_compression_3dc = GL_FALSE;
GLboolean __GLEW_ATI_texture_env_combine3 = GL_FALSE;
GLboolean __GLEW_ATI_texture_float = GL_FALSE;
GLboolean __GLEW_ATI_texture_mirror_once = GL_FALSE;
GLboolean __GLEW_ATI_vertex_array_object = GL_FALSE;
GLboolean __GLEW_ATI_vertex_attrib_array_object = GL_FALSE;
GLboolean __GLEW_ATI_vertex_streams = GL_FALSE;
GLboolean __GLEW_EXT_422_pixels = GL_FALSE;
GLboolean __GLEW_EXT_Cg_shader = GL_FALSE;
GLboolean __GLEW_EXT_abgr = GL_FALSE;
GLboolean __GLEW_EXT_bgra = GL_FALSE;
GLboolean __GLEW_EXT_bindable_uniform = GL_FALSE;
GLboolean __GLEW_EXT_blend_color = GL_FALSE;
GLboolean __GLEW_EXT_blend_equation_separate = GL_FALSE;
GLboolean __GLEW_EXT_blend_func_separate = GL_FALSE;
GLboolean __GLEW_EXT_blend_logic_op = GL_FALSE;
GLboolean __GLEW_EXT_blend_minmax = GL_FALSE;
GLboolean __GLEW_EXT_blend_subtract = GL_FALSE;
GLboolean __GLEW_EXT_clip_volume_hint = GL_FALSE;
GLboolean __GLEW_EXT_cmyka = GL_FALSE;
GLboolean __GLEW_EXT_color_subtable = GL_FALSE;
GLboolean __GLEW_EXT_compiled_vertex_array = GL_FALSE;
GLboolean __GLEW_EXT_convolution = GL_FALSE;
GLboolean __GLEW_EXT_coordinate_frame = GL_FALSE;
GLboolean __GLEW_EXT_copy_texture = GL_FALSE;
GLboolean __GLEW_EXT_cull_vertex = GL_FALSE;
GLboolean __GLEW_EXT_depth_bounds_test = GL_FALSE;
GLboolean __GLEW_EXT_direct_state_access = GL_FALSE;
GLboolean __GLEW_EXT_draw_buffers2 = GL_FALSE;
GLboolean __GLEW_EXT_draw_instanced = GL_FALSE;
GLboolean __GLEW_EXT_draw_range_elements = GL_FALSE;
GLboolean __GLEW_EXT_fog_coord = GL_FALSE;
GLboolean __GLEW_EXT_fragment_lighting = GL_FALSE;
GLboolean __GLEW_EXT_framebuffer_blit = GL_FALSE;
GLboolean __GLEW_EXT_framebuffer_multisample = GL_FALSE;
GLboolean __GLEW_EXT_framebuffer_object = GL_FALSE;
GLboolean __GLEW_EXT_framebuffer_sRGB = GL_FALSE;
GLboolean __GLEW_EXT_geometry_shader4 = GL_FALSE;
GLboolean __GLEW_EXT_gpu_program_parameters = GL_FALSE;
GLboolean __GLEW_EXT_gpu_shader4 = GL_FALSE;
GLboolean __GLEW_EXT_histogram = GL_FALSE;
GLboolean __GLEW_EXT_index_array_formats = GL_FALSE;
GLboolean __GLEW_EXT_index_func = GL_FALSE;
GLboolean __GLEW_EXT_index_material = GL_FALSE;
GLboolean __GLEW_EXT_index_texture = GL_FALSE;
GLboolean __GLEW_EXT_light_texture = GL_FALSE;
GLboolean __GLEW_EXT_misc_attribute = GL_FALSE;
GLboolean __GLEW_EXT_multi_draw_arrays = GL_FALSE;
GLboolean __GLEW_EXT_multisample = GL_FALSE;
GLboolean __GLEW_EXT_packed_depth_stencil = GL_FALSE;
GLboolean __GLEW_EXT_packed_float = GL_FALSE;
GLboolean __GLEW_EXT_packed_pixels = GL_FALSE;
GLboolean __GLEW_EXT_paletted_texture = GL_FALSE;
GLboolean __GLEW_EXT_pixel_buffer_object = GL_FALSE;
GLboolean __GLEW_EXT_pixel_transform = GL_FALSE;
GLboolean __GLEW_EXT_pixel_transform_color_table = GL_FALSE;
GLboolean __GLEW_EXT_point_parameters = GL_FALSE;
GLboolean __GLEW_EXT_polygon_offset = GL_FALSE;
GLboolean __GLEW_EXT_provoking_vertex = GL_FALSE;
GLboolean __GLEW_EXT_rescale_normal = GL_FALSE;
GLboolean __GLEW_EXT_scene_marker = GL_FALSE;
GLboolean __GLEW_EXT_secondary_color = GL_FALSE;
GLboolean __GLEW_EXT_separate_shader_objects = GL_FALSE;
GLboolean __GLEW_EXT_separate_specular_color = GL_FALSE;
GLboolean __GLEW_EXT_shader_image_load_store = GL_FALSE;
GLboolean __GLEW_EXT_shadow_funcs = GL_FALSE;
GLboolean __GLEW_EXT_shared_texture_palette = GL_FALSE;
GLboolean __GLEW_EXT_stencil_clear_tag = GL_FALSE;
GLboolean __GLEW_EXT_stencil_two_side = GL_FALSE;
GLboolean __GLEW_EXT_stencil_wrap = GL_FALSE;
GLboolean __GLEW_EXT_subtexture = GL_FALSE;
GLboolean __GLEW_EXT_texture = GL_FALSE;
GLboolean __GLEW_EXT_texture3D = GL_FALSE;
GLboolean __GLEW_EXT_texture_array = GL_FALSE;
GLboolean __GLEW_EXT_texture_buffer_object = GL_FALSE;
GLboolean __GLEW_EXT_texture_compression_dxt1 = GL_FALSE;
GLboolean __GLEW_EXT_texture_compression_latc = GL_FALSE;
GLboolean __GLEW_EXT_texture_compression_rgtc = GL_FALSE;
GLboolean __GLEW_EXT_texture_compression_s3tc = GL_FALSE;
GLboolean __GLEW_EXT_texture_cube_map = GL_FALSE;
GLboolean __GLEW_EXT_texture_edge_clamp = GL_FALSE;
GLboolean __GLEW_EXT_texture_env = GL_FALSE;
GLboolean __GLEW_EXT_texture_env_add = GL_FALSE;
GLboolean __GLEW_EXT_texture_env_combine = GL_FALSE;
GLboolean __GLEW_EXT_texture_env_dot3 = GL_FALSE;
GLboolean __GLEW_EXT_texture_filter_anisotropic = GL_FALSE;
GLboolean __GLEW_EXT_texture_integer = GL_FALSE;
GLboolean __GLEW_EXT_texture_lod_bias = GL_FALSE;
GLboolean __GLEW_EXT_texture_mirror_clamp = GL_FALSE;
GLboolean __GLEW_EXT_texture_object = GL_FALSE;
GLboolean __GLEW_EXT_texture_perturb_normal = GL_FALSE;
GLboolean __GLEW_EXT_texture_rectangle = GL_FALSE;
GLboolean __GLEW_EXT_texture_sRGB = GL_FALSE;
GLboolean __GLEW_EXT_texture_sRGB_decode = GL_FALSE;
GLboolean __GLEW_EXT_texture_shared_exponent = GL_FALSE;
GLboolean __GLEW_EXT_texture_snorm = GL_FALSE;
GLboolean __GLEW_EXT_texture_swizzle = GL_FALSE;
GLboolean __GLEW_EXT_timer_query = GL_FALSE;
GLboolean __GLEW_EXT_transform_feedback = GL_FALSE;
GLboolean __GLEW_EXT_vertex_array = GL_FALSE;
GLboolean __GLEW_EXT_vertex_array_bgra = GL_FALSE;
GLboolean __GLEW_EXT_vertex_attrib_64bit = GL_FALSE;
GLboolean __GLEW_EXT_vertex_shader = GL_FALSE;
GLboolean __GLEW_EXT_vertex_weighting = GL_FALSE;
GLboolean __GLEW_GREMEDY_frame_terminator = GL_FALSE;
GLboolean __GLEW_GREMEDY_string_marker = GL_FALSE;
GLboolean __GLEW_HP_convolution_border_modes = GL_FALSE;
GLboolean __GLEW_HP_image_transform = GL_FALSE;
GLboolean __GLEW_HP_occlusion_test = GL_FALSE;
GLboolean __GLEW_HP_texture_lighting = GL_FALSE;
GLboolean __GLEW_IBM_cull_vertex = GL_FALSE;
GLboolean __GLEW_IBM_multimode_draw_arrays = GL_FALSE;
GLboolean __GLEW_IBM_rasterpos_clip = GL_FALSE;
GLboolean __GLEW_IBM_static_data = GL_FALSE;
GLboolean __GLEW_IBM_texture_mirrored_repeat = GL_FALSE;
GLboolean __GLEW_IBM_vertex_array_lists = GL_FALSE;
GLboolean __GLEW_INGR_color_clamp = GL_FALSE;
GLboolean __GLEW_INGR_interlace_read = GL_FALSE;
GLboolean __GLEW_INTEL_parallel_arrays = GL_FALSE;
GLboolean __GLEW_INTEL_texture_scissor = GL_FALSE;
GLboolean __GLEW_KTX_buffer_region = GL_FALSE;
GLboolean __GLEW_MESAX_texture_stack = GL_FALSE;
GLboolean __GLEW_MESA_pack_invert = GL_FALSE;
GLboolean __GLEW_MESA_resize_buffers = GL_FALSE;
GLboolean __GLEW_MESA_window_pos = GL_FALSE;
GLboolean __GLEW_MESA_ycbcr_texture = GL_FALSE;
GLboolean __GLEW_NVX_gpu_memory_info = GL_FALSE;
GLboolean __GLEW_NV_blend_square = GL_FALSE;
GLboolean __GLEW_NV_conditional_render = GL_FALSE;
GLboolean __GLEW_NV_copy_depth_to_color = GL_FALSE;
GLboolean __GLEW_NV_copy_image = GL_FALSE;
GLboolean __GLEW_NV_depth_buffer_float = GL_FALSE;
GLboolean __GLEW_NV_depth_clamp = GL_FALSE;
GLboolean __GLEW_NV_depth_range_unclamped = GL_FALSE;
GLboolean __GLEW_NV_evaluators = GL_FALSE;
GLboolean __GLEW_NV_explicit_multisample = GL_FALSE;
GLboolean __GLEW_NV_fence = GL_FALSE;
GLboolean __GLEW_NV_float_buffer = GL_FALSE;
GLboolean __GLEW_NV_fog_distance = GL_FALSE;
GLboolean __GLEW_NV_fragment_program = GL_FALSE;
GLboolean __GLEW_NV_fragment_program2 = GL_FALSE;
GLboolean __GLEW_NV_fragment_program4 = GL_FALSE;
GLboolean __GLEW_NV_fragment_program_option = GL_FALSE;
GLboolean __GLEW_NV_framebuffer_multisample_coverage = GL_FALSE;
GLboolean __GLEW_NV_geometry_program4 = GL_FALSE;
GLboolean __GLEW_NV_geometry_shader4 = GL_FALSE;
GLboolean __GLEW_NV_gpu_program4 = GL_FALSE;
GLboolean __GLEW_NV_gpu_program5 = GL_FALSE;
GLboolean __GLEW_NV_gpu_program_fp64 = GL_FALSE;
GLboolean __GLEW_NV_gpu_shader5 = GL_FALSE;
GLboolean __GLEW_NV_half_float = GL_FALSE;
GLboolean __GLEW_NV_light_max_exponent = GL_FALSE;
GLboolean __GLEW_NV_multisample_coverage = GL_FALSE;
GLboolean __GLEW_NV_multisample_filter_hint = GL_FALSE;
GLboolean __GLEW_NV_occlusion_query = GL_FALSE;
GLboolean __GLEW_NV_packed_depth_stencil = GL_FALSE;
GLboolean __GLEW_NV_parameter_buffer_object = GL_FALSE;
GLboolean __GLEW_NV_parameter_buffer_object2 = GL_FALSE;
GLboolean __GLEW_NV_pixel_data_range = GL_FALSE;
GLboolean __GLEW_NV_point_sprite = GL_FALSE;
GLboolean __GLEW_NV_present_video = GL_FALSE;
GLboolean __GLEW_NV_primitive_restart = GL_FALSE;
GLboolean __GLEW_NV_register_combiners = GL_FALSE;
GLboolean __GLEW_NV_register_combiners2 = GL_FALSE;
GLboolean __GLEW_NV_shader_buffer_load = GL_FALSE;
GLboolean __GLEW_NV_tessellation_program5 = GL_FALSE;
GLboolean __GLEW_NV_texgen_emboss = GL_FALSE;
GLboolean __GLEW_NV_texgen_reflection = GL_FALSE;
GLboolean __GLEW_NV_texture_barrier = GL_FALSE;
GLboolean __GLEW_NV_texture_compression_vtc = GL_FALSE;
GLboolean __GLEW_NV_texture_env_combine4 = GL_FALSE;
GLboolean __GLEW_NV_texture_expand_normal = GL_FALSE;
GLboolean __GLEW_NV_texture_rectangle = GL_FALSE;
GLboolean __GLEW_NV_texture_shader = GL_FALSE;
GLboolean __GLEW_NV_texture_shader2 = GL_FALSE;
GLboolean __GLEW_NV_texture_shader3 = GL_FALSE;
GLboolean __GLEW_NV_transform_feedback = GL_FALSE;
GLboolean __GLEW_NV_transform_feedback2 = GL_FALSE;
GLboolean __GLEW_NV_vdpau_interop = GL_FALSE;
GLboolean __GLEW_NV_vertex_array_range = GL_FALSE;
GLboolean __GLEW_NV_vertex_array_range2 = GL_FALSE;
GLboolean __GLEW_NV_vertex_attrib_integer_64bit = GL_FALSE;
GLboolean __GLEW_NV_vertex_buffer_unified_memory = GL_FALSE;
GLboolean __GLEW_NV_vertex_program = GL_FALSE;
GLboolean __GLEW_NV_vertex_program1_1 = GL_FALSE;
GLboolean __GLEW_NV_vertex_program2 = GL_FALSE;
GLboolean __GLEW_NV_vertex_program2_option = GL_FALSE;
GLboolean __GLEW_NV_vertex_program3 = GL_FALSE;
GLboolean __GLEW_NV_vertex_program4 = GL_FALSE;
GLboolean __GLEW_OES_byte_coordinates = GL_FALSE;
GLboolean __GLEW_OES_compressed_paletted_texture = GL_FALSE;
GLboolean __GLEW_OES_read_format = GL_FALSE;
GLboolean __GLEW_OES_single_precision = GL_FALSE;
GLboolean __GLEW_OML_interlace = GL_FALSE;
GLboolean __GLEW_OML_resample = GL_FALSE;
GLboolean __GLEW_OML_subsample = GL_FALSE;
GLboolean __GLEW_PGI_misc_hints = GL_FALSE;
GLboolean __GLEW_PGI_vertex_hints = GL_FALSE;
GLboolean __GLEW_REND_screen_coordinates = GL_FALSE;
GLboolean __GLEW_S3_s3tc = GL_FALSE;
GLboolean __GLEW_SGIS_color_range = GL_FALSE;
GLboolean __GLEW_SGIS_detail_texture = GL_FALSE;
GLboolean __GLEW_SGIS_fog_function = GL_FALSE;
GLboolean __GLEW_SGIS_generate_mipmap = GL_FALSE;
GLboolean __GLEW_SGIS_multisample = GL_FALSE;
GLboolean __GLEW_SGIS_pixel_texture = GL_FALSE;
GLboolean __GLEW_SGIS_point_line_texgen = GL_FALSE;
GLboolean __GLEW_SGIS_sharpen_texture = GL_FALSE;
GLboolean __GLEW_SGIS_texture4D = GL_FALSE;
GLboolean __GLEW_SGIS_texture_border_clamp = GL_FALSE;
GLboolean __GLEW_SGIS_texture_edge_clamp = GL_FALSE;
GLboolean __GLEW_SGIS_texture_filter4 = GL_FALSE;
GLboolean __GLEW_SGIS_texture_lod = GL_FALSE;
GLboolean __GLEW_SGIS_texture_select = GL_FALSE;
GLboolean __GLEW_SGIX_async = GL_FALSE;
GLboolean __GLEW_SGIX_async_histogram = GL_FALSE;
GLboolean __GLEW_SGIX_async_pixel = GL_FALSE;
GLboolean __GLEW_SGIX_blend_alpha_minmax = GL_FALSE;
GLboolean __GLEW_SGIX_clipmap = GL_FALSE;
GLboolean __GLEW_SGIX_convolution_accuracy = GL_FALSE;
GLboolean __GLEW_SGIX_depth_texture = GL_FALSE;
GLboolean __GLEW_SGIX_flush_raster = GL_FALSE;
GLboolean __GLEW_SGIX_fog_offset = GL_FALSE;
GLboolean __GLEW_SGIX_fog_texture = GL_FALSE;
GLboolean __GLEW_SGIX_fragment_specular_lighting = GL_FALSE;
GLboolean __GLEW_SGIX_framezoom = GL_FALSE;
GLboolean __GLEW_SGIX_interlace = GL_FALSE;
GLboolean __GLEW_SGIX_ir_instrument1 = GL_FALSE;
GLboolean __GLEW_SGIX_list_priority = GL_FALSE;
GLboolean __GLEW_SGIX_pixel_texture = GL_FALSE;
GLboolean __GLEW_SGIX_pixel_texture_bits = GL_FALSE;
GLboolean __GLEW_SGIX_reference_plane = GL_FALSE;
GLboolean __GLEW_SGIX_resample = GL_FALSE;
GLboolean __GLEW_SGIX_shadow = GL_FALSE;
GLboolean __GLEW_SGIX_shadow_ambient = GL_FALSE;
GLboolean __GLEW_SGIX_sprite = GL_FALSE;
GLboolean __GLEW_SGIX_tag_sample_buffer = GL_FALSE;
GLboolean __GLEW_SGIX_texture_add_env = GL_FALSE;
GLboolean __GLEW_SGIX_texture_coordinate_clamp = GL_FALSE;
GLboolean __GLEW_SGIX_texture_lod_bias = GL_FALSE;
GLboolean __GLEW_SGIX_texture_multi_buffer = GL_FALSE;
GLboolean __GLEW_SGIX_texture_range = GL_FALSE;
GLboolean __GLEW_SGIX_texture_scale_bias = GL_FALSE;
GLboolean __GLEW_SGIX_vertex_preclip = GL_FALSE;
GLboolean __GLEW_SGIX_vertex_preclip_hint = GL_FALSE;
GLboolean __GLEW_SGIX_ycrcb = GL_FALSE;
GLboolean __GLEW_SGI_color_matrix = GL_FALSE;
GLboolean __GLEW_SGI_color_table = GL_FALSE;
GLboolean __GLEW_SGI_texture_color_table = GL_FALSE;
GLboolean __GLEW_SUNX_constant_data = GL_FALSE;
GLboolean __GLEW_SUN_convolution_border_modes = GL_FALSE;
GLboolean __GLEW_SUN_global_alpha = GL_FALSE;
GLboolean __GLEW_SUN_mesh_array = GL_FALSE;
GLboolean __GLEW_SUN_read_video_pixels = GL_FALSE;
GLboolean __GLEW_SUN_slice_accum = GL_FALSE;
GLboolean __GLEW_SUN_triangle_list = GL_FALSE;
GLboolean __GLEW_SUN_vertex = GL_FALSE;
GLboolean __GLEW_WIN_phong_shading = GL_FALSE;
GLboolean __GLEW_WIN_specular_fog = GL_FALSE;
GLboolean __GLEW_WIN_swap_hint = GL_FALSE;

#endif /* !GLEW_MX */

#ifdef GL_VERSION_1_1

static GLboolean _glewInit_GL_VERSION_1_1 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAccum = (PFNGLACCUMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAccum"))) == NULL) || r;
  r = ((glAlphaFunc = (PFNGLALPHAFUNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAlphaFunc"))) == NULL) || r;
  r = ((glAreTexturesResident = (PFNGLARETEXTURESRESIDENTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAreTexturesResident"))) == NULL) || r;
  r = ((glArrayElement = (PFNGLARRAYELEMENTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glArrayElement"))) == NULL) || r;
  r = ((glBegin = (PFNGLBEGINPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBegin"))) == NULL) || r;
  r = ((glBindTexture = (PFNGLBINDTEXTUREPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindTexture"))) == NULL) || r;
  r = ((glBitmap = (PFNGLBITMAPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBitmap"))) == NULL) || r;
  r = ((glBlendFunc = (PFNGLBLENDFUNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFunc"))) == NULL) || r;
  r = ((glCallList = (PFNGLCALLLISTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCallList"))) == NULL) || r;
  r = ((glCallLists = (PFNGLCALLLISTSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCallLists"))) == NULL) || r;
  r = ((glClear = (PFNGLCLEARPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClear"))) == NULL) || r;
  r = ((glClearAccum = (PFNGLCLEARACCUMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearAccum"))) == NULL) || r;
  r = ((glClearColor = (PFNGLCLEARCOLORPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearColor"))) == NULL) || r;
  r = ((glClearDepth = (PFNGLCLEARDEPTHPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearDepth"))) == NULL) || r;
  r = ((glClearIndex = (PFNGLCLEARINDEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearIndex"))) == NULL) || r;
  r = ((glClearStencil = (PFNGLCLEARSTENCILPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearStencil"))) == NULL) || r;
  r = ((glClipPlane = (PFNGLCLIPPLANEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClipPlane"))) == NULL) || r;
  r = ((glColor3b = (PFNGLCOLOR3BPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3b"))) == NULL) || r;
  r = ((glColor3bv = (PFNGLCOLOR3BVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3bv"))) == NULL) || r;
  r = ((glColor3d = (PFNGLCOLOR3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3d"))) == NULL) || r;
  r = ((glColor3dv = (PFNGLCOLOR3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3dv"))) == NULL) || r;
  r = ((glColor3f = (PFNGLCOLOR3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3f"))) == NULL) || r;
  r = ((glColor3fv = (PFNGLCOLOR3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3fv"))) == NULL) || r;
  r = ((glColor3i = (PFNGLCOLOR3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3i"))) == NULL) || r;
  r = ((glColor3iv = (PFNGLCOLOR3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3iv"))) == NULL) || r;
  r = ((glColor3s = (PFNGLCOLOR3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3s"))) == NULL) || r;
  r = ((glColor3sv = (PFNGLCOLOR3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3sv"))) == NULL) || r;
  r = ((glColor3ub = (PFNGLCOLOR3UBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3ub"))) == NULL) || r;
  r = ((glColor3ubv = (PFNGLCOLOR3UBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3ubv"))) == NULL) || r;
  r = ((glColor3ui = (PFNGLCOLOR3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3ui"))) == NULL) || r;
  r = ((glColor3uiv = (PFNGLCOLOR3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3uiv"))) == NULL) || r;
  r = ((glColor3us = (PFNGLCOLOR3USPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3us"))) == NULL) || r;
  r = ((glColor3usv = (PFNGLCOLOR3USVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3usv"))) == NULL) || r;
  r = ((glColor4b = (PFNGLCOLOR4BPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4b"))) == NULL) || r;
  r = ((glColor4bv = (PFNGLCOLOR4BVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4bv"))) == NULL) || r;
  r = ((glColor4d = (PFNGLCOLOR4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4d"))) == NULL) || r;
  r = ((glColor4dv = (PFNGLCOLOR4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4dv"))) == NULL) || r;
  r = ((glColor4f = (PFNGLCOLOR4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4f"))) == NULL) || r;
  r = ((glColor4fv = (PFNGLCOLOR4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4fv"))) == NULL) || r;
  r = ((glColor4i = (PFNGLCOLOR4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4i"))) == NULL) || r;
  r = ((glColor4iv = (PFNGLCOLOR4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4iv"))) == NULL) || r;
  r = ((glColor4s = (PFNGLCOLOR4SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4s"))) == NULL) || r;
  r = ((glColor4sv = (PFNGLCOLOR4SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4sv"))) == NULL) || r;
  r = ((glColor4ub = (PFNGLCOLOR4UBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4ub"))) == NULL) || r;
  r = ((glColor4ubv = (PFNGLCOLOR4UBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4ubv"))) == NULL) || r;
  r = ((glColor4ui = (PFNGLCOLOR4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4ui"))) == NULL) || r;
  r = ((glColor4uiv = (PFNGLCOLOR4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4uiv"))) == NULL) || r;
  r = ((glColor4us = (PFNGLCOLOR4USPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4us"))) == NULL) || r;
  r = ((glColor4usv = (PFNGLCOLOR4USVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4usv"))) == NULL) || r;
  r = ((glColorMask = (PFNGLCOLORMASKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorMask"))) == NULL) || r;
  r = ((glColorMaterial = (PFNGLCOLORMATERIALPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorMaterial"))) == NULL) || r;
  r = ((glColorPointer = (PFNGLCOLORPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorPointer"))) == NULL) || r;
  r = ((glCopyPixels = (PFNGLCOPYPIXELSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyPixels"))) == NULL) || r;
  r = ((glCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexImage1D"))) == NULL) || r;
  r = ((glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexImage2D"))) == NULL) || r;
  r = ((glCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexSubImage1D"))) == NULL) || r;
  r = ((glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexSubImage2D"))) == NULL) || r;
  r = ((glCullFace = (PFNGLCULLFACEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCullFace"))) == NULL) || r;
  r = ((glDeleteLists = (PFNGLDELETELISTSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteLists"))) == NULL) || r;
  r = ((glDeleteTextures = (PFNGLDELETETEXTURESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteTextures"))) == NULL) || r;
  r = ((glDepthFunc = (PFNGLDEPTHFUNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthFunc"))) == NULL) || r;
  r = ((glDepthMask = (PFNGLDEPTHMASKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthMask"))) == NULL) || r;
  r = ((glDepthRange = (PFNGLDEPTHRANGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthRange"))) == NULL) || r;
  r = ((glDisable = (PFNGLDISABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisable"))) == NULL) || r;
  r = ((glDisableClientState = (PFNGLDISABLECLIENTSTATEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableClientState"))) == NULL) || r;
  r = ((glDrawArrays = (PFNGLDRAWARRAYSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawArrays"))) == NULL) || r;
  r = ((glDrawBuffer = (PFNGLDRAWBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawBuffer"))) == NULL) || r;
  r = ((glDrawElements = (PFNGLDRAWELEMENTSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElements"))) == NULL) || r;
  r = ((glDrawPixels = (PFNGLDRAWPIXELSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawPixels"))) == NULL) || r;
  r = ((glEdgeFlag = (PFNGLEDGEFLAGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEdgeFlag"))) == NULL) || r;
  r = ((glEdgeFlagPointer = (PFNGLEDGEFLAGPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEdgeFlagPointer"))) == NULL) || r;
  r = ((glEdgeFlagv = (PFNGLEDGEFLAGVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEdgeFlagv"))) == NULL) || r;
  r = ((glEnable = (PFNGLENABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnable"))) == NULL) || r;
  r = ((glEnableClientState = (PFNGLENABLECLIENTSTATEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableClientState"))) == NULL) || r;
  r = ((glEnd = (PFNGLENDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnd"))) == NULL) || r;
  r = ((glEndList = (PFNGLENDLISTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndList"))) == NULL) || r;
  r = ((glEvalCoord1d = (PFNGLEVALCOORD1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord1d"))) == NULL) || r;
  r = ((glEvalCoord1dv = (PFNGLEVALCOORD1DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord1dv"))) == NULL) || r;
  r = ((glEvalCoord1f = (PFNGLEVALCOORD1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord1f"))) == NULL) || r;
  r = ((glEvalCoord1fv = (PFNGLEVALCOORD1FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord1fv"))) == NULL) || r;
  r = ((glEvalCoord2d = (PFNGLEVALCOORD2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord2d"))) == NULL) || r;
  r = ((glEvalCoord2dv = (PFNGLEVALCOORD2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord2dv"))) == NULL) || r;
  r = ((glEvalCoord2f = (PFNGLEVALCOORD2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord2f"))) == NULL) || r;
  r = ((glEvalCoord2fv = (PFNGLEVALCOORD2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalCoord2fv"))) == NULL) || r;
  r = ((glEvalMesh1 = (PFNGLEVALMESH1PROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalMesh1"))) == NULL) || r;
  r = ((glEvalMesh2 = (PFNGLEVALMESH2PROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalMesh2"))) == NULL) || r;
  r = ((glEvalPoint1 = (PFNGLEVALPOINT1PROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalPoint1"))) == NULL) || r;
  r = ((glEvalPoint2 = (PFNGLEVALPOINT2PROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalPoint2"))) == NULL) || r;
  r = ((glFeedbackBuffer = (PFNGLFEEDBACKBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFeedbackBuffer"))) == NULL) || r;
  r = ((glFinish = (PFNGLFINISHPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFinish"))) == NULL) || r;
  r = ((glFlush = (PFNGLFLUSHPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlush"))) == NULL) || r;
  r = ((glFogf = (PFNGLFOGFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogf"))) == NULL) || r;
  r = ((glFogfv = (PFNGLFOGFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogfv"))) == NULL) || r;
  r = ((glFogi = (PFNGLFOGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogi"))) == NULL) || r;
  r = ((glFogiv = (PFNGLFOGIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogiv"))) == NULL) || r;
  r = ((glFrontFace = (PFNGLFRONTFACEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFrontFace"))) == NULL) || r;
  r = ((glFrustum = (PFNGLFRUSTUMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFrustum"))) == NULL) || r;
  r = ((glGenLists = (PFNGLGENLISTSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenLists"))) == NULL) || r;
  r = ((glGenTextures = (PFNGLGENTEXTURESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenTextures"))) == NULL) || r;
  r = ((glGetBooleanv = (PFNGLGETBOOLEANVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBooleanv"))) == NULL) || r;
  r = ((glGetClipPlane = (PFNGLGETCLIPPLANEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetClipPlane"))) == NULL) || r;
  r = ((glGetDoublev = (PFNGLGETDOUBLEVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetDoublev"))) == NULL) || r;
  r = ((glGetError = (PFNGLGETERRORPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetError"))) == NULL) || r;
  r = ((glGetFloatv = (PFNGLGETFLOATVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFloatv"))) == NULL) || r;
  r = ((glGetIntegerv = (PFNGLGETINTEGERVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetIntegerv"))) == NULL) || r;
  r = ((glGetLightfv = (PFNGLGETLIGHTFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetLightfv"))) == NULL) || r;
  r = ((glGetLightiv = (PFNGLGETLIGHTIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetLightiv"))) == NULL) || r;
  r = ((glGetMapdv = (PFNGLGETMAPDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapdv"))) == NULL) || r;
  r = ((glGetMapfv = (PFNGLGETMAPFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapfv"))) == NULL) || r;
  r = ((glGetMapiv = (PFNGLGETMAPIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapiv"))) == NULL) || r;
  r = ((glGetMaterialfv = (PFNGLGETMATERIALFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMaterialfv"))) == NULL) || r;
  r = ((glGetMaterialiv = (PFNGLGETMATERIALIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMaterialiv"))) == NULL) || r;
  r = ((glGetPixelMapfv = (PFNGLGETPIXELMAPFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPixelMapfv"))) == NULL) || r;
  r = ((glGetPixelMapuiv = (PFNGLGETPIXELMAPUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPixelMapuiv"))) == NULL) || r;
  r = ((glGetPixelMapusv = (PFNGLGETPIXELMAPUSVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPixelMapusv"))) == NULL) || r;
  r = ((glGetPointerv = (PFNGLGETPOINTERVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPointerv"))) == NULL) || r;
  r = ((glGetPolygonStipple = (PFNGLGETPOLYGONSTIPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPolygonStipple"))) == NULL) || r;
  r = ((glGetString = (PFNGLGETSTRINGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetString"))) == NULL) || r;
  r = ((glGetTexEnvfv = (PFNGLGETTEXENVFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexEnvfv"))) == NULL) || r;
  r = ((glGetTexEnviv = (PFNGLGETTEXENVIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexEnviv"))) == NULL) || r;
  r = ((glGetTexGendv = (PFNGLGETTEXGENDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexGendv"))) == NULL) || r;
  r = ((glGetTexGenfv = (PFNGLGETTEXGENFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexGenfv"))) == NULL) || r;
  r = ((glGetTexGeniv = (PFNGLGETTEXGENIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexGeniv"))) == NULL) || r;
  r = ((glGetTexImage = (PFNGLGETTEXIMAGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexImage"))) == NULL) || r;
  r = ((glGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexLevelParameterfv"))) == NULL) || r;
  r = ((glGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexLevelParameteriv"))) == NULL) || r;
  r = ((glGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexParameterfv"))) == NULL) || r;
  r = ((glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexParameteriv"))) == NULL) || r;
  r = ((glHint = (PFNGLHINTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glHint"))) == NULL) || r;
  r = ((glIndexMask = (PFNGLINDEXMASKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexMask"))) == NULL) || r;
  r = ((glIndexPointer = (PFNGLINDEXPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexPointer"))) == NULL) || r;
  r = ((glIndexd = (PFNGLINDEXDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexd"))) == NULL) || r;
  r = ((glIndexdv = (PFNGLINDEXDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexdv"))) == NULL) || r;
  r = ((glIndexf = (PFNGLINDEXFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexf"))) == NULL) || r;
  r = ((glIndexfv = (PFNGLINDEXFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexfv"))) == NULL) || r;
  r = ((glIndexi = (PFNGLINDEXIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexi"))) == NULL) || r;
  r = ((glIndexiv = (PFNGLINDEXIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexiv"))) == NULL) || r;
  r = ((glIndexs = (PFNGLINDEXSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexs"))) == NULL) || r;
  r = ((glIndexsv = (PFNGLINDEXSVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexsv"))) == NULL) || r;
  r = ((glIndexub = (PFNGLINDEXUBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexub"))) == NULL) || r;
  r = ((glIndexubv = (PFNGLINDEXUBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexubv"))) == NULL) || r;
  r = ((glInitNames = (PFNGLINITNAMESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glInitNames"))) == NULL) || r;
  r = ((glInterleavedArrays = (PFNGLINTERLEAVEDARRAYSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glInterleavedArrays"))) == NULL) || r;
  r = ((glIsEnabled = (PFNGLISENABLEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsEnabled"))) == NULL) || r;
  r = ((glIsList = (PFNGLISLISTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsList"))) == NULL) || r;
  r = ((glIsTexture = (PFNGLISTEXTUREPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsTexture"))) == NULL) || r;
  r = ((glLightModelf = (PFNGLLIGHTMODELFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightModelf"))) == NULL) || r;
  r = ((glLightModelfv = (PFNGLLIGHTMODELFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightModelfv"))) == NULL) || r;
  r = ((glLightModeli = (PFNGLLIGHTMODELIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightModeli"))) == NULL) || r;
  r = ((glLightModeliv = (PFNGLLIGHTMODELIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightModeliv"))) == NULL) || r;
  r = ((glLightf = (PFNGLLIGHTFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightf"))) == NULL) || r;
  r = ((glLightfv = (PFNGLLIGHTFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightfv"))) == NULL) || r;
  r = ((glLighti = (PFNGLLIGHTIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLighti"))) == NULL) || r;
  r = ((glLightiv = (PFNGLLIGHTIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightiv"))) == NULL) || r;
  r = ((glLineStipple = (PFNGLLINESTIPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLineStipple"))) == NULL) || r;
  r = ((glLineWidth = (PFNGLLINEWIDTHPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLineWidth"))) == NULL) || r;
  r = ((glListBase = (PFNGLLISTBASEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glListBase"))) == NULL) || r;
  r = ((glLoadIdentity = (PFNGLLOADIDENTITYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadIdentity"))) == NULL) || r;
  r = ((glLoadMatrixd = (PFNGLLOADMATRIXDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadMatrixd"))) == NULL) || r;
  r = ((glLoadMatrixf = (PFNGLLOADMATRIXFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadMatrixf"))) == NULL) || r;
  r = ((glLoadName = (PFNGLLOADNAMEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadName"))) == NULL) || r;
  r = ((glLogicOp = (PFNGLLOGICOPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLogicOp"))) == NULL) || r;
  r = ((glMap1d = (PFNGLMAP1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMap1d"))) == NULL) || r;
  r = ((glMap1f = (PFNGLMAP1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMap1f"))) == NULL) || r;
  r = ((glMap2d = (PFNGLMAP2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMap2d"))) == NULL) || r;
  r = ((glMap2f = (PFNGLMAP2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMap2f"))) == NULL) || r;
  r = ((glMapGrid1d = (PFNGLMAPGRID1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapGrid1d"))) == NULL) || r;
  r = ((glMapGrid1f = (PFNGLMAPGRID1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapGrid1f"))) == NULL) || r;
  r = ((glMapGrid2d = (PFNGLMAPGRID2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapGrid2d"))) == NULL) || r;
  r = ((glMapGrid2f = (PFNGLMAPGRID2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapGrid2f"))) == NULL) || r;
  r = ((glMaterialf = (PFNGLMATERIALFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMaterialf"))) == NULL) || r;
  r = ((glMaterialfv = (PFNGLMATERIALFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMaterialfv"))) == NULL) || r;
  r = ((glMateriali = (PFNGLMATERIALIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMateriali"))) == NULL) || r;
  r = ((glMaterialiv = (PFNGLMATERIALIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMaterialiv"))) == NULL) || r;
  r = ((glMatrixMode = (PFNGLMATRIXMODEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixMode"))) == NULL) || r;
  r = ((glMultMatrixd = (PFNGLMULTMATRIXDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultMatrixd"))) == NULL) || r;
  r = ((glMultMatrixf = (PFNGLMULTMATRIXFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultMatrixf"))) == NULL) || r;
  r = ((glNewList = (PFNGLNEWLISTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNewList"))) == NULL) || r;
  r = ((glNormal3b = (PFNGLNORMAL3BPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3b"))) == NULL) || r;
  r = ((glNormal3bv = (PFNGLNORMAL3BVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3bv"))) == NULL) || r;
  r = ((glNormal3d = (PFNGLNORMAL3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3d"))) == NULL) || r;
  r = ((glNormal3dv = (PFNGLNORMAL3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3dv"))) == NULL) || r;
  r = ((glNormal3f = (PFNGLNORMAL3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3f"))) == NULL) || r;
  r = ((glNormal3fv = (PFNGLNORMAL3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3fv"))) == NULL) || r;
  r = ((glNormal3i = (PFNGLNORMAL3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3i"))) == NULL) || r;
  r = ((glNormal3iv = (PFNGLNORMAL3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3iv"))) == NULL) || r;
  r = ((glNormal3s = (PFNGLNORMAL3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3s"))) == NULL) || r;
  r = ((glNormal3sv = (PFNGLNORMAL3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3sv"))) == NULL) || r;
  r = ((glNormalPointer = (PFNGLNORMALPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalPointer"))) == NULL) || r;
  r = ((glOrtho = (PFNGLORTHOPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glOrtho"))) == NULL) || r;
  r = ((glPassThrough = (PFNGLPASSTHROUGHPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPassThrough"))) == NULL) || r;
  r = ((glPixelMapfv = (PFNGLPIXELMAPFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelMapfv"))) == NULL) || r;
  r = ((glPixelMapuiv = (PFNGLPIXELMAPUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelMapuiv"))) == NULL) || r;
  r = ((glPixelMapusv = (PFNGLPIXELMAPUSVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelMapusv"))) == NULL) || r;
  r = ((glPixelStoref = (PFNGLPIXELSTOREFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelStoref"))) == NULL) || r;
  r = ((glPixelStorei = (PFNGLPIXELSTOREIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelStorei"))) == NULL) || r;
  r = ((glPixelTransferf = (PFNGLPIXELTRANSFERFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelTransferf"))) == NULL) || r;
  r = ((glPixelTransferi = (PFNGLPIXELTRANSFERIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelTransferi"))) == NULL) || r;
  r = ((glPixelZoom = (PFNGLPIXELZOOMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelZoom"))) == NULL) || r;
  r = ((glPointSize = (PFNGLPOINTSIZEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointSize"))) == NULL) || r;
  r = ((glPolygonMode = (PFNGLPOLYGONMODEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPolygonMode"))) == NULL) || r;
  r = ((glPolygonOffset = (PFNGLPOLYGONOFFSETPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPolygonOffset"))) == NULL) || r;
  r = ((glPolygonStipple = (PFNGLPOLYGONSTIPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPolygonStipple"))) == NULL) || r;
  r = ((glPopAttrib = (PFNGLPOPATTRIBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPopAttrib"))) == NULL) || r;
  r = ((glPopClientAttrib = (PFNGLPOPCLIENTATTRIBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPopClientAttrib"))) == NULL) || r;
  r = ((glPopMatrix = (PFNGLPOPMATRIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPopMatrix"))) == NULL) || r;
  r = ((glPopName = (PFNGLPOPNAMEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPopName"))) == NULL) || r;
  r = ((glPrioritizeTextures = (PFNGLPRIORITIZETEXTURESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPrioritizeTextures"))) == NULL) || r;
  r = ((glPushAttrib = (PFNGLPUSHATTRIBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPushAttrib"))) == NULL) || r;
  r = ((glPushClientAttrib = (PFNGLPUSHCLIENTATTRIBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPushClientAttrib"))) == NULL) || r;
  r = ((glPushMatrix = (PFNGLPUSHMATRIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPushMatrix"))) == NULL) || r;
  r = ((glPushName = (PFNGLPUSHNAMEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPushName"))) == NULL) || r;
  r = ((glRasterPos2d = (PFNGLRASTERPOS2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2d"))) == NULL) || r;
  r = ((glRasterPos2dv = (PFNGLRASTERPOS2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2dv"))) == NULL) || r;
  r = ((glRasterPos2f = (PFNGLRASTERPOS2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2f"))) == NULL) || r;
  r = ((glRasterPos2fv = (PFNGLRASTERPOS2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2fv"))) == NULL) || r;
  r = ((glRasterPos2i = (PFNGLRASTERPOS2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2i"))) == NULL) || r;
  r = ((glRasterPos2iv = (PFNGLRASTERPOS2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2iv"))) == NULL) || r;
  r = ((glRasterPos2s = (PFNGLRASTERPOS2SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2s"))) == NULL) || r;
  r = ((glRasterPos2sv = (PFNGLRASTERPOS2SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos2sv"))) == NULL) || r;
  r = ((glRasterPos3d = (PFNGLRASTERPOS3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3d"))) == NULL) || r;
  r = ((glRasterPos3dv = (PFNGLRASTERPOS3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3dv"))) == NULL) || r;
  r = ((glRasterPos3f = (PFNGLRASTERPOS3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3f"))) == NULL) || r;
  r = ((glRasterPos3fv = (PFNGLRASTERPOS3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3fv"))) == NULL) || r;
  r = ((glRasterPos3i = (PFNGLRASTERPOS3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3i"))) == NULL) || r;
  r = ((glRasterPos3iv = (PFNGLRASTERPOS3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3iv"))) == NULL) || r;
  r = ((glRasterPos3s = (PFNGLRASTERPOS3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3s"))) == NULL) || r;
  r = ((glRasterPos3sv = (PFNGLRASTERPOS3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos3sv"))) == NULL) || r;
  r = ((glRasterPos4d = (PFNGLRASTERPOS4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4d"))) == NULL) || r;
  r = ((glRasterPos4dv = (PFNGLRASTERPOS4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4dv"))) == NULL) || r;
  r = ((glRasterPos4f = (PFNGLRASTERPOS4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4f"))) == NULL) || r;
  r = ((glRasterPos4fv = (PFNGLRASTERPOS4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4fv"))) == NULL) || r;
  r = ((glRasterPos4i = (PFNGLRASTERPOS4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4i"))) == NULL) || r;
  r = ((glRasterPos4iv = (PFNGLRASTERPOS4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4iv"))) == NULL) || r;
  r = ((glRasterPos4s = (PFNGLRASTERPOS4SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4s"))) == NULL) || r;
  r = ((glRasterPos4sv = (PFNGLRASTERPOS4SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRasterPos4sv"))) == NULL) || r;
  r = ((glReadBuffer = (PFNGLREADBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReadBuffer"))) == NULL) || r;
  r = ((glReadPixels = (PFNGLREADPIXELSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReadPixels"))) == NULL) || r;
  r = ((glRectd = (PFNGLRECTDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRectd"))) == NULL) || r;
  r = ((glRectdv = (PFNGLRECTDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRectdv"))) == NULL) || r;
  r = ((glRectf = (PFNGLRECTFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRectf"))) == NULL) || r;
  r = ((glRectfv = (PFNGLRECTFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRectfv"))) == NULL) || r;
  r = ((glRecti = (PFNGLRECTIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRecti"))) == NULL) || r;
  r = ((glRectiv = (PFNGLRECTIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRectiv"))) == NULL) || r;
  r = ((glRects = (PFNGLRECTSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRects"))) == NULL) || r;
  r = ((glRectsv = (PFNGLRECTSVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRectsv"))) == NULL) || r;
  r = ((glRenderMode = (PFNGLRENDERMODEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRenderMode"))) == NULL) || r;
  r = ((glRotated = (PFNGLROTATEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRotated"))) == NULL) || r;
  r = ((glRotatef = (PFNGLROTATEFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRotatef"))) == NULL) || r;
  r = ((glScaled = (PFNGLSCALEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glScaled"))) == NULL) || r;
  r = ((glScalef = (PFNGLSCALEFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glScalef"))) == NULL) || r;
  r = ((glScissor = (PFNGLSCISSORPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glScissor"))) == NULL) || r;
  r = ((glSelectBuffer = (PFNGLSELECTBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSelectBuffer"))) == NULL) || r;
  r = ((glShadeModel = (PFNGLSHADEMODELPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glShadeModel"))) == NULL) || r;
  r = ((glStencilFunc = (PFNGLSTENCILFUNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilFunc"))) == NULL) || r;
  r = ((glStencilMask = (PFNGLSTENCILMASKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilMask"))) == NULL) || r;
  r = ((glStencilOp = (PFNGLSTENCILOPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilOp"))) == NULL) || r;
  r = ((glTexCoord1d = (PFNGLTEXCOORD1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1d"))) == NULL) || r;
  r = ((glTexCoord1dv = (PFNGLTEXCOORD1DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1dv"))) == NULL) || r;
  r = ((glTexCoord1f = (PFNGLTEXCOORD1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1f"))) == NULL) || r;
  r = ((glTexCoord1fv = (PFNGLTEXCOORD1FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1fv"))) == NULL) || r;
  r = ((glTexCoord1i = (PFNGLTEXCOORD1IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1i"))) == NULL) || r;
  r = ((glTexCoord1iv = (PFNGLTEXCOORD1IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1iv"))) == NULL) || r;
  r = ((glTexCoord1s = (PFNGLTEXCOORD1SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1s"))) == NULL) || r;
  r = ((glTexCoord1sv = (PFNGLTEXCOORD1SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1sv"))) == NULL) || r;
  r = ((glTexCoord2d = (PFNGLTEXCOORD2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2d"))) == NULL) || r;
  r = ((glTexCoord2dv = (PFNGLTEXCOORD2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2dv"))) == NULL) || r;
  r = ((glTexCoord2f = (PFNGLTEXCOORD2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2f"))) == NULL) || r;
  r = ((glTexCoord2fv = (PFNGLTEXCOORD2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fv"))) == NULL) || r;
  r = ((glTexCoord2i = (PFNGLTEXCOORD2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2i"))) == NULL) || r;
  r = ((glTexCoord2iv = (PFNGLTEXCOORD2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2iv"))) == NULL) || r;
  r = ((glTexCoord2s = (PFNGLTEXCOORD2SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2s"))) == NULL) || r;
  r = ((glTexCoord2sv = (PFNGLTEXCOORD2SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2sv"))) == NULL) || r;
  r = ((glTexCoord3d = (PFNGLTEXCOORD3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3d"))) == NULL) || r;
  r = ((glTexCoord3dv = (PFNGLTEXCOORD3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3dv"))) == NULL) || r;
  r = ((glTexCoord3f = (PFNGLTEXCOORD3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3f"))) == NULL) || r;
  r = ((glTexCoord3fv = (PFNGLTEXCOORD3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3fv"))) == NULL) || r;
  r = ((glTexCoord3i = (PFNGLTEXCOORD3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3i"))) == NULL) || r;
  r = ((glTexCoord3iv = (PFNGLTEXCOORD3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3iv"))) == NULL) || r;
  r = ((glTexCoord3s = (PFNGLTEXCOORD3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3s"))) == NULL) || r;
  r = ((glTexCoord3sv = (PFNGLTEXCOORD3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3sv"))) == NULL) || r;
  r = ((glTexCoord4d = (PFNGLTEXCOORD4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4d"))) == NULL) || r;
  r = ((glTexCoord4dv = (PFNGLTEXCOORD4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4dv"))) == NULL) || r;
  r = ((glTexCoord4f = (PFNGLTEXCOORD4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4f"))) == NULL) || r;
  r = ((glTexCoord4fv = (PFNGLTEXCOORD4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4fv"))) == NULL) || r;
  r = ((glTexCoord4i = (PFNGLTEXCOORD4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4i"))) == NULL) || r;
  r = ((glTexCoord4iv = (PFNGLTEXCOORD4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4iv"))) == NULL) || r;
  r = ((glTexCoord4s = (PFNGLTEXCOORD4SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4s"))) == NULL) || r;
  r = ((glTexCoord4sv = (PFNGLTEXCOORD4SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4sv"))) == NULL) || r;
  r = ((glTexCoordPointer = (PFNGLTEXCOORDPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordPointer"))) == NULL) || r;
  r = ((glTexEnvf = (PFNGLTEXENVFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexEnvf"))) == NULL) || r;
  r = ((glTexEnvfv = (PFNGLTEXENVFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexEnvfv"))) == NULL) || r;
  r = ((glTexEnvi = (PFNGLTEXENVIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexEnvi"))) == NULL) || r;
  r = ((glTexEnviv = (PFNGLTEXENVIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexEnviv"))) == NULL) || r;
  r = ((glTexGend = (PFNGLTEXGENDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexGend"))) == NULL) || r;
  r = ((glTexGendv = (PFNGLTEXGENDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexGendv"))) == NULL) || r;
  r = ((glTexGenf = (PFNGLTEXGENFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexGenf"))) == NULL) || r;
  r = ((glTexGenfv = (PFNGLTEXGENFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexGenfv"))) == NULL) || r;
  r = ((glTexGeni = (PFNGLTEXGENIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexGeni"))) == NULL) || r;
  r = ((glTexGeniv = (PFNGLTEXGENIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexGeniv"))) == NULL) || r;
  r = ((glTexImage1D = (PFNGLTEXIMAGE1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexImage1D"))) == NULL) || r;
  r = ((glTexImage2D = (PFNGLTEXIMAGE2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexImage2D"))) == NULL) || r;
  r = ((glTexParameterf = (PFNGLTEXPARAMETERFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameterf"))) == NULL) || r;
  r = ((glTexParameterfv = (PFNGLTEXPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameterfv"))) == NULL) || r;
  r = ((glTexParameteri = (PFNGLTEXPARAMETERIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameteri"))) == NULL) || r;
  r = ((glTexParameteriv = (PFNGLTEXPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameteriv"))) == NULL) || r;
  r = ((glTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexSubImage1D"))) == NULL) || r;
  r = ((glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexSubImage2D"))) == NULL) || r;
  r = ((glTranslated = (PFNGLTRANSLATEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTranslated"))) == NULL) || r;
  r = ((glTranslatef = (PFNGLTRANSLATEFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTranslatef"))) == NULL) || r;
  r = ((glVertex2d = (PFNGLVERTEX2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2d"))) == NULL) || r;
  r = ((glVertex2dv = (PFNGLVERTEX2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2dv"))) == NULL) || r;
  r = ((glVertex2f = (PFNGLVERTEX2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2f"))) == NULL) || r;
  r = ((glVertex2fv = (PFNGLVERTEX2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2fv"))) == NULL) || r;
  r = ((glVertex2i = (PFNGLVERTEX2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2i"))) == NULL) || r;
  r = ((glVertex2iv = (PFNGLVERTEX2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2iv"))) == NULL) || r;
  r = ((glVertex2s = (PFNGLVERTEX2SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2s"))) == NULL) || r;
  r = ((glVertex2sv = (PFNGLVERTEX2SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2sv"))) == NULL) || r;
  r = ((glVertex3d = (PFNGLVERTEX3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3d"))) == NULL) || r;
  r = ((glVertex3dv = (PFNGLVERTEX3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3dv"))) == NULL) || r;
  r = ((glVertex3f = (PFNGLVERTEX3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3f"))) == NULL) || r;
  r = ((glVertex3fv = (PFNGLVERTEX3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3fv"))) == NULL) || r;
  r = ((glVertex3i = (PFNGLVERTEX3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3i"))) == NULL) || r;
  r = ((glVertex3iv = (PFNGLVERTEX3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3iv"))) == NULL) || r;
  r = ((glVertex3s = (PFNGLVERTEX3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3s"))) == NULL) || r;
  r = ((glVertex3sv = (PFNGLVERTEX3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3sv"))) == NULL) || r;
  r = ((glVertex4d = (PFNGLVERTEX4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4d"))) == NULL) || r;
  r = ((glVertex4dv = (PFNGLVERTEX4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4dv"))) == NULL) || r;
  r = ((glVertex4f = (PFNGLVERTEX4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4f"))) == NULL) || r;
  r = ((glVertex4fv = (PFNGLVERTEX4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4fv"))) == NULL) || r;
  r = ((glVertex4i = (PFNGLVERTEX4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4i"))) == NULL) || r;
  r = ((glVertex4iv = (PFNGLVERTEX4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4iv"))) == NULL) || r;
  r = ((glVertex4s = (PFNGLVERTEX4SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4s"))) == NULL) || r;
  r = ((glVertex4sv = (PFNGLVERTEX4SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4sv"))) == NULL) || r;
  r = ((glVertexPointer = (PFNGLVERTEXPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexPointer"))) == NULL) || r;
  r = ((glViewport = (PFNGLVIEWPORTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glViewport"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_1_1 */

#ifdef GL_VERSION_1_2

static GLboolean _glewInit_GL_VERSION_1_2 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexSubImage3D"))) == NULL) || r;
  r = ((glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawRangeElements"))) == NULL) || r;
  r = ((glTexImage3D = (PFNGLTEXIMAGE3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexImage3D"))) == NULL) || r;
  r = ((glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexSubImage3D"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_1_2 */

#ifdef GL_VERSION_1_2_1

#endif /* GL_VERSION_1_2_1 */

#ifdef GL_VERSION_1_3

static GLboolean _glewInit_GL_VERSION_1_3 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glActiveTexture = (PFNGLACTIVETEXTUREPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glActiveTexture"))) == NULL) || r;
  r = ((glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClientActiveTexture"))) == NULL) || r;
  r = ((glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexImage1D"))) == NULL) || r;
  r = ((glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexImage2D"))) == NULL) || r;
  r = ((glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexImage3D"))) == NULL) || r;
  r = ((glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexSubImage1D"))) == NULL) || r;
  r = ((glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexSubImage2D"))) == NULL) || r;
  r = ((glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexSubImage3D"))) == NULL) || r;
  r = ((glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCompressedTexImage"))) == NULL) || r;
  r = ((glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadTransposeMatrixd"))) == NULL) || r;
  r = ((glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadTransposeMatrixf"))) == NULL) || r;
  r = ((glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultTransposeMatrixd"))) == NULL) || r;
  r = ((glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultTransposeMatrixf"))) == NULL) || r;
  r = ((glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1d"))) == NULL) || r;
  r = ((glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1dv"))) == NULL) || r;
  r = ((glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1f"))) == NULL) || r;
  r = ((glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1fv"))) == NULL) || r;
  r = ((glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1i"))) == NULL) || r;
  r = ((glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1iv"))) == NULL) || r;
  r = ((glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1s"))) == NULL) || r;
  r = ((glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1sv"))) == NULL) || r;
  r = ((glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2d"))) == NULL) || r;
  r = ((glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2dv"))) == NULL) || r;
  r = ((glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2f"))) == NULL) || r;
  r = ((glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2fv"))) == NULL) || r;
  r = ((glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2i"))) == NULL) || r;
  r = ((glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2iv"))) == NULL) || r;
  r = ((glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2s"))) == NULL) || r;
  r = ((glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2sv"))) == NULL) || r;
  r = ((glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3d"))) == NULL) || r;
  r = ((glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3dv"))) == NULL) || r;
  r = ((glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3f"))) == NULL) || r;
  r = ((glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3fv"))) == NULL) || r;
  r = ((glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3i"))) == NULL) || r;
  r = ((glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3iv"))) == NULL) || r;
  r = ((glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3s"))) == NULL) || r;
  r = ((glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3sv"))) == NULL) || r;
  r = ((glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4d"))) == NULL) || r;
  r = ((glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4dv"))) == NULL) || r;
  r = ((glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4f"))) == NULL) || r;
  r = ((glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4fv"))) == NULL) || r;
  r = ((glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4i"))) == NULL) || r;
  r = ((glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4iv"))) == NULL) || r;
  r = ((glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4s"))) == NULL) || r;
  r = ((glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4sv"))) == NULL) || r;
  r = ((glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSampleCoverage"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_1_3 */

#ifdef GL_VERSION_1_4

static GLboolean _glewInit_GL_VERSION_1_4 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendColor = (PFNGLBLENDCOLORPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendColor"))) == NULL) || r;
  r = ((glBlendEquation = (PFNGLBLENDEQUATIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquation"))) == NULL) || r;
  r = ((glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFuncSeparate"))) == NULL) || r;
  r = ((glFogCoordPointer = (PFNGLFOGCOORDPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordPointer"))) == NULL) || r;
  r = ((glFogCoordd = (PFNGLFOGCOORDDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordd"))) == NULL) || r;
  r = ((glFogCoorddv = (PFNGLFOGCOORDDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoorddv"))) == NULL) || r;
  r = ((glFogCoordf = (PFNGLFOGCOORDFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordf"))) == NULL) || r;
  r = ((glFogCoordfv = (PFNGLFOGCOORDFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordfv"))) == NULL) || r;
  r = ((glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiDrawArrays"))) == NULL) || r;
  r = ((glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiDrawElements"))) == NULL) || r;
  r = ((glPointParameterf = (PFNGLPOINTPARAMETERFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameterf"))) == NULL) || r;
  r = ((glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameterfv"))) == NULL) || r;
  r = ((glPointParameteri = (PFNGLPOINTPARAMETERIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameteri"))) == NULL) || r;
  r = ((glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameteriv"))) == NULL) || r;
  r = ((glSecondaryColor3b = (PFNGLSECONDARYCOLOR3BPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3b"))) == NULL) || r;
  r = ((glSecondaryColor3bv = (PFNGLSECONDARYCOLOR3BVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3bv"))) == NULL) || r;
  r = ((glSecondaryColor3d = (PFNGLSECONDARYCOLOR3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3d"))) == NULL) || r;
  r = ((glSecondaryColor3dv = (PFNGLSECONDARYCOLOR3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3dv"))) == NULL) || r;
  r = ((glSecondaryColor3f = (PFNGLSECONDARYCOLOR3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3f"))) == NULL) || r;
  r = ((glSecondaryColor3fv = (PFNGLSECONDARYCOLOR3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3fv"))) == NULL) || r;
  r = ((glSecondaryColor3i = (PFNGLSECONDARYCOLOR3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3i"))) == NULL) || r;
  r = ((glSecondaryColor3iv = (PFNGLSECONDARYCOLOR3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3iv"))) == NULL) || r;
  r = ((glSecondaryColor3s = (PFNGLSECONDARYCOLOR3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3s"))) == NULL) || r;
  r = ((glSecondaryColor3sv = (PFNGLSECONDARYCOLOR3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3sv"))) == NULL) || r;
  r = ((glSecondaryColor3ub = (PFNGLSECONDARYCOLOR3UBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3ub"))) == NULL) || r;
  r = ((glSecondaryColor3ubv = (PFNGLSECONDARYCOLOR3UBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3ubv"))) == NULL) || r;
  r = ((glSecondaryColor3ui = (PFNGLSECONDARYCOLOR3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3ui"))) == NULL) || r;
  r = ((glSecondaryColor3uiv = (PFNGLSECONDARYCOLOR3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3uiv"))) == NULL) || r;
  r = ((glSecondaryColor3us = (PFNGLSECONDARYCOLOR3USPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3us"))) == NULL) || r;
  r = ((glSecondaryColor3usv = (PFNGLSECONDARYCOLOR3USVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3usv"))) == NULL) || r;
  r = ((glSecondaryColorPointer = (PFNGLSECONDARYCOLORPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColorPointer"))) == NULL) || r;
  r = ((glWindowPos2d = (PFNGLWINDOWPOS2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2d"))) == NULL) || r;
  r = ((glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2dv"))) == NULL) || r;
  r = ((glWindowPos2f = (PFNGLWINDOWPOS2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2f"))) == NULL) || r;
  r = ((glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2fv"))) == NULL) || r;
  r = ((glWindowPos2i = (PFNGLWINDOWPOS2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2i"))) == NULL) || r;
  r = ((glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2iv"))) == NULL) || r;
  r = ((glWindowPos2s = (PFNGLWINDOWPOS2SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2s"))) == NULL) || r;
  r = ((glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2sv"))) == NULL) || r;
  r = ((glWindowPos3d = (PFNGLWINDOWPOS3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3d"))) == NULL) || r;
  r = ((glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3dv"))) == NULL) || r;
  r = ((glWindowPos3f = (PFNGLWINDOWPOS3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3f"))) == NULL) || r;
  r = ((glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3fv"))) == NULL) || r;
  r = ((glWindowPos3i = (PFNGLWINDOWPOS3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3i"))) == NULL) || r;
  r = ((glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3iv"))) == NULL) || r;
  r = ((glWindowPos3s = (PFNGLWINDOWPOS3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3s"))) == NULL) || r;
  r = ((glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3sv"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_1_4 */

#ifdef GL_VERSION_1_5

static GLboolean _glewInit_GL_VERSION_1_5 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginQuery = (PFNGLBEGINQUERYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginQuery"))) == NULL) || r;
  r = ((glBindBuffer = (PFNGLBINDBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBuffer"))) == NULL) || r;
  r = ((glBufferData = (PFNGLBUFFERDATAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBufferData"))) == NULL) || r;
  r = ((glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBufferSubData"))) == NULL) || r;
  r = ((glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteBuffers"))) == NULL) || r;
  r = ((glDeleteQueries = (PFNGLDELETEQUERIESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteQueries"))) == NULL) || r;
  r = ((glEndQuery = (PFNGLENDQUERYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndQuery"))) == NULL) || r;
  r = ((glGenBuffers = (PFNGLGENBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenBuffers"))) == NULL) || r;
  r = ((glGenQueries = (PFNGLGENQUERIESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenQueries"))) == NULL) || r;
  r = ((glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferParameteriv"))) == NULL) || r;
  r = ((glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferPointerv"))) == NULL) || r;
  r = ((glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferSubData"))) == NULL) || r;
  r = ((glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjectiv"))) == NULL) || r;
  r = ((glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjectuiv"))) == NULL) || r;
  r = ((glGetQueryiv = (PFNGLGETQUERYIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryiv"))) == NULL) || r;
  r = ((glIsBuffer = (PFNGLISBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsBuffer"))) == NULL) || r;
  r = ((glIsQuery = (PFNGLISQUERYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsQuery"))) == NULL) || r;
  r = ((glMapBuffer = (PFNGLMAPBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapBuffer"))) == NULL) || r;
  r = ((glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUnmapBuffer"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_1_5 */

#ifdef GL_VERSION_2_0

static GLboolean _glewInit_GL_VERSION_2_0 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAttachShader = (PFNGLATTACHSHADERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAttachShader"))) == NULL) || r;
  r = ((glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindAttribLocation"))) == NULL) || r;
  r = ((glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationSeparate"))) == NULL) || r;
  r = ((glCompileShader = (PFNGLCOMPILESHADERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompileShader"))) == NULL) || r;
  r = ((glCreateProgram = (PFNGLCREATEPROGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCreateProgram"))) == NULL) || r;
  r = ((glCreateShader = (PFNGLCREATESHADERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCreateShader"))) == NULL) || r;
  r = ((glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteProgram"))) == NULL) || r;
  r = ((glDeleteShader = (PFNGLDELETESHADERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteShader"))) == NULL) || r;
  r = ((glDetachShader = (PFNGLDETACHSHADERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDetachShader"))) == NULL) || r;
  r = ((glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableVertexAttribArray"))) == NULL) || r;
  r = ((glDrawBuffers = (PFNGLDRAWBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawBuffers"))) == NULL) || r;
  r = ((glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableVertexAttribArray"))) == NULL) || r;
  r = ((glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveAttrib"))) == NULL) || r;
  r = ((glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveUniform"))) == NULL) || r;
  r = ((glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetAttachedShaders"))) == NULL) || r;
  r = ((glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetAttribLocation"))) == NULL) || r;
  r = ((glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramInfoLog"))) == NULL) || r;
  r = ((glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramiv"))) == NULL) || r;
  r = ((glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetShaderInfoLog"))) == NULL) || r;
  r = ((glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetShaderSource"))) == NULL) || r;
  r = ((glGetShaderiv = (PFNGLGETSHADERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetShaderiv"))) == NULL) || r;
  r = ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformLocation"))) == NULL) || r;
  r = ((glGetUniformfv = (PFNGLGETUNIFORMFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformfv"))) == NULL) || r;
  r = ((glGetUniformiv = (PFNGLGETUNIFORMIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformiv"))) == NULL) || r;
  r = ((glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribPointerv"))) == NULL) || r;
  r = ((glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribdv"))) == NULL) || r;
  r = ((glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribfv"))) == NULL) || r;
  r = ((glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribiv"))) == NULL) || r;
  r = ((glIsProgram = (PFNGLISPROGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsProgram"))) == NULL) || r;
  r = ((glIsShader = (PFNGLISSHADERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsShader"))) == NULL) || r;
  r = ((glLinkProgram = (PFNGLLINKPROGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLinkProgram"))) == NULL) || r;
  r = ((glShaderSource = (PFNGLSHADERSOURCEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glShaderSource"))) == NULL) || r;
  r = ((glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilFuncSeparate"))) == NULL) || r;
  r = ((glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilMaskSeparate"))) == NULL) || r;
  r = ((glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilOpSeparate"))) == NULL) || r;
  r = ((glUniform1f = (PFNGLUNIFORM1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1f"))) == NULL) || r;
  r = ((glUniform1fv = (PFNGLUNIFORM1FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1fv"))) == NULL) || r;
  r = ((glUniform1i = (PFNGLUNIFORM1IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1i"))) == NULL) || r;
  r = ((glUniform1iv = (PFNGLUNIFORM1IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1iv"))) == NULL) || r;
  r = ((glUniform2f = (PFNGLUNIFORM2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2f"))) == NULL) || r;
  r = ((glUniform2fv = (PFNGLUNIFORM2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2fv"))) == NULL) || r;
  r = ((glUniform2i = (PFNGLUNIFORM2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2i"))) == NULL) || r;
  r = ((glUniform2iv = (PFNGLUNIFORM2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2iv"))) == NULL) || r;
  r = ((glUniform3f = (PFNGLUNIFORM3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3f"))) == NULL) || r;
  r = ((glUniform3fv = (PFNGLUNIFORM3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3fv"))) == NULL) || r;
  r = ((glUniform3i = (PFNGLUNIFORM3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3i"))) == NULL) || r;
  r = ((glUniform3iv = (PFNGLUNIFORM3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3iv"))) == NULL) || r;
  r = ((glUniform4f = (PFNGLUNIFORM4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4f"))) == NULL) || r;
  r = ((glUniform4fv = (PFNGLUNIFORM4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4fv"))) == NULL) || r;
  r = ((glUniform4i = (PFNGLUNIFORM4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4i"))) == NULL) || r;
  r = ((glUniform4iv = (PFNGLUNIFORM4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4iv"))) == NULL) || r;
  r = ((glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix2fv"))) == NULL) || r;
  r = ((glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix3fv"))) == NULL) || r;
  r = ((glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix4fv"))) == NULL) || r;
  r = ((glUseProgram = (PFNGLUSEPROGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUseProgram"))) == NULL) || r;
  r = ((glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glValidateProgram"))) == NULL) || r;
  r = ((glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1d"))) == NULL) || r;
  r = ((glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1dv"))) == NULL) || r;
  r = ((glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1f"))) == NULL) || r;
  r = ((glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1fv"))) == NULL) || r;
  r = ((glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1s"))) == NULL) || r;
  r = ((glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1sv"))) == NULL) || r;
  r = ((glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2d"))) == NULL) || r;
  r = ((glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2dv"))) == NULL) || r;
  r = ((glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2f"))) == NULL) || r;
  r = ((glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2fv"))) == NULL) || r;
  r = ((glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2s"))) == NULL) || r;
  r = ((glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2sv"))) == NULL) || r;
  r = ((glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3d"))) == NULL) || r;
  r = ((glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3dv"))) == NULL) || r;
  r = ((glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3f"))) == NULL) || r;
  r = ((glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3fv"))) == NULL) || r;
  r = ((glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3s"))) == NULL) || r;
  r = ((glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3sv"))) == NULL) || r;
  r = ((glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4Nbv"))) == NULL) || r;
  r = ((glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4Niv"))) == NULL) || r;
  r = ((glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4Nsv"))) == NULL) || r;
  r = ((glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4Nub"))) == NULL) || r;
  r = ((glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4Nubv"))) == NULL) || r;
  r = ((glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4Nuiv"))) == NULL) || r;
  r = ((glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4Nusv"))) == NULL) || r;
  r = ((glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4bv"))) == NULL) || r;
  r = ((glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4d"))) == NULL) || r;
  r = ((glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4dv"))) == NULL) || r;
  r = ((glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4f"))) == NULL) || r;
  r = ((glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4fv"))) == NULL) || r;
  r = ((glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4iv"))) == NULL) || r;
  r = ((glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4s"))) == NULL) || r;
  r = ((glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4sv"))) == NULL) || r;
  r = ((glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4ubv"))) == NULL) || r;
  r = ((glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4uiv"))) == NULL) || r;
  r = ((glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4usv"))) == NULL) || r;
  r = ((glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribPointer"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_2_0 */

#ifdef GL_VERSION_2_1

static GLboolean _glewInit_GL_VERSION_2_1 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix2x3fv"))) == NULL) || r;
  r = ((glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix2x4fv"))) == NULL) || r;
  r = ((glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix3x2fv"))) == NULL) || r;
  r = ((glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix3x4fv"))) == NULL) || r;
  r = ((glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix4x2fv"))) == NULL) || r;
  r = ((glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix4x3fv"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_2_1 */

#ifdef GL_VERSION_3_0

static GLboolean _glewInit_GL_VERSION_3_0 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginConditionalRender"))) == NULL) || r;
  r = ((glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginTransformFeedback"))) == NULL) || r;
  r = ((glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindFragDataLocation"))) == NULL) || r;
  r = ((glClampColor = (PFNGLCLAMPCOLORPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClampColor"))) == NULL) || r;
  r = ((glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearBufferfi"))) == NULL) || r;
  r = ((glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearBufferfv"))) == NULL) || r;
  r = ((glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearBufferiv"))) == NULL) || r;
  r = ((glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearBufferuiv"))) == NULL) || r;
  r = ((glColorMaski = (PFNGLCOLORMASKIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorMaski"))) == NULL) || r;
  r = ((glDisablei = (PFNGLDISABLEIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisablei"))) == NULL) || r;
  r = ((glEnablei = (PFNGLENABLEIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnablei"))) == NULL) || r;
  r = ((glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndConditionalRender"))) == NULL) || r;
  r = ((glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndTransformFeedback"))) == NULL) || r;
  r = ((glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBooleani_v"))) == NULL) || r;
  r = ((glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragDataLocation"))) == NULL) || r;
  r = ((glGetStringi = (PFNGLGETSTRINGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetStringi"))) == NULL) || r;
  r = ((glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexParameterIiv"))) == NULL) || r;
  r = ((glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexParameterIuiv"))) == NULL) || r;
  r = ((glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTransformFeedbackVarying"))) == NULL) || r;
  r = ((glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformuiv"))) == NULL) || r;
  r = ((glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribIiv"))) == NULL) || r;
  r = ((glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribIuiv"))) == NULL) || r;
  r = ((glIsEnabledi = (PFNGLISENABLEDIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsEnabledi"))) == NULL) || r;
  r = ((glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameterIiv"))) == NULL) || r;
  r = ((glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameterIuiv"))) == NULL) || r;
  r = ((glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTransformFeedbackVaryings"))) == NULL) || r;
  r = ((glUniform1ui = (PFNGLUNIFORM1UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1ui"))) == NULL) || r;
  r = ((glUniform1uiv = (PFNGLUNIFORM1UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1uiv"))) == NULL) || r;
  r = ((glUniform2ui = (PFNGLUNIFORM2UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2ui"))) == NULL) || r;
  r = ((glUniform2uiv = (PFNGLUNIFORM2UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2uiv"))) == NULL) || r;
  r = ((glUniform3ui = (PFNGLUNIFORM3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3ui"))) == NULL) || r;
  r = ((glUniform3uiv = (PFNGLUNIFORM3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3uiv"))) == NULL) || r;
  r = ((glUniform4ui = (PFNGLUNIFORM4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4ui"))) == NULL) || r;
  r = ((glUniform4uiv = (PFNGLUNIFORM4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4uiv"))) == NULL) || r;
  r = ((glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1i"))) == NULL) || r;
  r = ((glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1iv"))) == NULL) || r;
  r = ((glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1ui"))) == NULL) || r;
  r = ((glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1uiv"))) == NULL) || r;
  r = ((glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2i"))) == NULL) || r;
  r = ((glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2iv"))) == NULL) || r;
  r = ((glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2ui"))) == NULL) || r;
  r = ((glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2uiv"))) == NULL) || r;
  r = ((glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3i"))) == NULL) || r;
  r = ((glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3iv"))) == NULL) || r;
  r = ((glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3ui"))) == NULL) || r;
  r = ((glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3uiv"))) == NULL) || r;
  r = ((glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4bv"))) == NULL) || r;
  r = ((glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4i"))) == NULL) || r;
  r = ((glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4iv"))) == NULL) || r;
  r = ((glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4sv"))) == NULL) || r;
  r = ((glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4ubv"))) == NULL) || r;
  r = ((glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4ui"))) == NULL) || r;
  r = ((glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4uiv"))) == NULL) || r;
  r = ((glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4usv"))) == NULL) || r;
  r = ((glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribIPointer"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_3_0 */

#ifdef GL_VERSION_3_1

static GLboolean _glewInit_GL_VERSION_3_1 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawArraysInstanced"))) == NULL) || r;
  r = ((glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementsInstanced"))) == NULL) || r;
  r = ((glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPrimitiveRestartIndex"))) == NULL) || r;
  r = ((glTexBuffer = (PFNGLTEXBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexBuffer"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_3_1 */

#ifdef GL_VERSION_3_2

static GLboolean _glewInit_GL_VERSION_3_2 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTexture"))) == NULL) || r;
  r = ((glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferParameteri64v"))) == NULL) || r;
  r = ((glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetInteger64i_v"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_3_2 */

#ifdef GL_VERSION_3_3

static GLboolean _glewInit_GL_VERSION_3_3 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribDivisor"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_3_3 */

#ifdef GL_VERSION_4_0

static GLboolean _glewInit_GL_VERSION_4_0 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationSeparatei"))) == NULL) || r;
  r = ((glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationi"))) == NULL) || r;
  r = ((glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFuncSeparatei"))) == NULL) || r;
  r = ((glBlendFunci = (PFNGLBLENDFUNCIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFunci"))) == NULL) || r;
  r = ((glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMinSampleShading"))) == NULL) || r;

  return r;
}

#endif /* GL_VERSION_4_0 */

#ifdef GL_VERSION_4_1

#endif /* GL_VERSION_4_1 */

#ifdef GL_3DFX_multisample

#endif /* GL_3DFX_multisample */

#ifdef GL_3DFX_tbuffer

static GLboolean _glewInit_GL_3DFX_tbuffer (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTbufferMask3DFX = (PFNGLTBUFFERMASK3DFXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTbufferMask3DFX"))) == NULL) || r;

  return r;
}

#endif /* GL_3DFX_tbuffer */

#ifdef GL_3DFX_texture_compression_FXT1

#endif /* GL_3DFX_texture_compression_FXT1 */

#ifdef GL_AMD_conservative_depth

#endif /* GL_AMD_conservative_depth */

#ifdef GL_AMD_debug_output

static GLboolean _glewInit_GL_AMD_debug_output (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDebugMessageCallbackAMD = (PFNGLDEBUGMESSAGECALLBACKAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDebugMessageCallbackAMD"))) == NULL) || r;
  r = ((glDebugMessageEnableAMD = (PFNGLDEBUGMESSAGEENABLEAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDebugMessageEnableAMD"))) == NULL) || r;
  r = ((glDebugMessageInsertAMD = (PFNGLDEBUGMESSAGEINSERTAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDebugMessageInsertAMD"))) == NULL) || r;
  r = ((glGetDebugMessageLogAMD = (PFNGLGETDEBUGMESSAGELOGAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetDebugMessageLogAMD"))) == NULL) || r;

  return r;
}

#endif /* GL_AMD_debug_output */

#ifdef GL_AMD_depth_clamp_separate

#endif /* GL_AMD_depth_clamp_separate */

#ifdef GL_AMD_draw_buffers_blend

static GLboolean _glewInit_GL_AMD_draw_buffers_blend (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendEquationIndexedAMD = (PFNGLBLENDEQUATIONINDEXEDAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationIndexedAMD"))) == NULL) || r;
  r = ((glBlendEquationSeparateIndexedAMD = (PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationSeparateIndexedAMD"))) == NULL) || r;
  r = ((glBlendFuncIndexedAMD = (PFNGLBLENDFUNCINDEXEDAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFuncIndexedAMD"))) == NULL) || r;
  r = ((glBlendFuncSeparateIndexedAMD = (PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFuncSeparateIndexedAMD"))) == NULL) || r;

  return r;
}

#endif /* GL_AMD_draw_buffers_blend */

#ifdef GL_AMD_name_gen_delete

static GLboolean _glewInit_GL_AMD_name_gen_delete (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDeleteNamesAMD = (PFNGLDELETENAMESAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteNamesAMD"))) == NULL) || r;
  r = ((glGenNamesAMD = (PFNGLGENNAMESAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenNamesAMD"))) == NULL) || r;
  r = ((glIsNameAMD = (PFNGLISNAMEAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsNameAMD"))) == NULL) || r;

  return r;
}

#endif /* GL_AMD_name_gen_delete */

#ifdef GL_AMD_performance_monitor

static GLboolean _glewInit_GL_AMD_performance_monitor (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginPerfMonitorAMD = (PFNGLBEGINPERFMONITORAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginPerfMonitorAMD"))) == NULL) || r;
  r = ((glDeletePerfMonitorsAMD = (PFNGLDELETEPERFMONITORSAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeletePerfMonitorsAMD"))) == NULL) || r;
  r = ((glEndPerfMonitorAMD = (PFNGLENDPERFMONITORAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndPerfMonitorAMD"))) == NULL) || r;
  r = ((glGenPerfMonitorsAMD = (PFNGLGENPERFMONITORSAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenPerfMonitorsAMD"))) == NULL) || r;
  r = ((glGetPerfMonitorCounterDataAMD = (PFNGLGETPERFMONITORCOUNTERDATAAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPerfMonitorCounterDataAMD"))) == NULL) || r;
  r = ((glGetPerfMonitorCounterInfoAMD = (PFNGLGETPERFMONITORCOUNTERINFOAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPerfMonitorCounterInfoAMD"))) == NULL) || r;
  r = ((glGetPerfMonitorCounterStringAMD = (PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPerfMonitorCounterStringAMD"))) == NULL) || r;
  r = ((glGetPerfMonitorCountersAMD = (PFNGLGETPERFMONITORCOUNTERSAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPerfMonitorCountersAMD"))) == NULL) || r;
  r = ((glGetPerfMonitorGroupStringAMD = (PFNGLGETPERFMONITORGROUPSTRINGAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPerfMonitorGroupStringAMD"))) == NULL) || r;
  r = ((glGetPerfMonitorGroupsAMD = (PFNGLGETPERFMONITORGROUPSAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPerfMonitorGroupsAMD"))) == NULL) || r;
  r = ((glSelectPerfMonitorCountersAMD = (PFNGLSELECTPERFMONITORCOUNTERSAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSelectPerfMonitorCountersAMD"))) == NULL) || r;

  return r;
}

#endif /* GL_AMD_performance_monitor */

#ifdef GL_AMD_seamless_cubemap_per_texture

#endif /* GL_AMD_seamless_cubemap_per_texture */

#ifdef GL_AMD_shader_stencil_export

#endif /* GL_AMD_shader_stencil_export */

#ifdef GL_AMD_texture_texture4

#endif /* GL_AMD_texture_texture4 */

#ifdef GL_AMD_transform_feedback3_lines_triangles

#endif /* GL_AMD_transform_feedback3_lines_triangles */

#ifdef GL_AMD_vertex_shader_tessellator

static GLboolean _glewInit_GL_AMD_vertex_shader_tessellator (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTessellationFactorAMD = (PFNGLTESSELLATIONFACTORAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTessellationFactorAMD"))) == NULL) || r;
  r = ((glTessellationModeAMD = (PFNGLTESSELLATIONMODEAMDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTessellationModeAMD"))) == NULL) || r;

  return r;
}

#endif /* GL_AMD_vertex_shader_tessellator */

#ifdef GL_APPLE_aux_depth_stencil

#endif /* GL_APPLE_aux_depth_stencil */

#ifdef GL_APPLE_client_storage

#endif /* GL_APPLE_client_storage */

#ifdef GL_APPLE_element_array

static GLboolean _glewInit_GL_APPLE_element_array (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawElementArrayAPPLE = (PFNGLDRAWELEMENTARRAYAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementArrayAPPLE"))) == NULL) || r;
  r = ((glDrawRangeElementArrayAPPLE = (PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawRangeElementArrayAPPLE"))) == NULL) || r;
  r = ((glElementPointerAPPLE = (PFNGLELEMENTPOINTERAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glElementPointerAPPLE"))) == NULL) || r;
  r = ((glMultiDrawElementArrayAPPLE = (PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiDrawElementArrayAPPLE"))) == NULL) || r;
  r = ((glMultiDrawRangeElementArrayAPPLE = (PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiDrawRangeElementArrayAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_element_array */

#ifdef GL_APPLE_fence

static GLboolean _glewInit_GL_APPLE_fence (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDeleteFencesAPPLE = (PFNGLDELETEFENCESAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteFencesAPPLE"))) == NULL) || r;
  r = ((glFinishFenceAPPLE = (PFNGLFINISHFENCEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFinishFenceAPPLE"))) == NULL) || r;
  r = ((glFinishObjectAPPLE = (PFNGLFINISHOBJECTAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFinishObjectAPPLE"))) == NULL) || r;
  r = ((glGenFencesAPPLE = (PFNGLGENFENCESAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenFencesAPPLE"))) == NULL) || r;
  r = ((glIsFenceAPPLE = (PFNGLISFENCEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsFenceAPPLE"))) == NULL) || r;
  r = ((glSetFenceAPPLE = (PFNGLSETFENCEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSetFenceAPPLE"))) == NULL) || r;
  r = ((glTestFenceAPPLE = (PFNGLTESTFENCEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTestFenceAPPLE"))) == NULL) || r;
  r = ((glTestObjectAPPLE = (PFNGLTESTOBJECTAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTestObjectAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_fence */

#ifdef GL_APPLE_float_pixels

#endif /* GL_APPLE_float_pixels */

#ifdef GL_APPLE_flush_buffer_range

static GLboolean _glewInit_GL_APPLE_flush_buffer_range (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBufferParameteriAPPLE = (PFNGLBUFFERPARAMETERIAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBufferParameteriAPPLE"))) == NULL) || r;
  r = ((glFlushMappedBufferRangeAPPLE = (PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlushMappedBufferRangeAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_flush_buffer_range */

#ifdef GL_APPLE_object_purgeable

static GLboolean _glewInit_GL_APPLE_object_purgeable (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetObjectParameterivAPPLE = (PFNGLGETOBJECTPARAMETERIVAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetObjectParameterivAPPLE"))) == NULL) || r;
  r = ((glObjectPurgeableAPPLE = (PFNGLOBJECTPURGEABLEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glObjectPurgeableAPPLE"))) == NULL) || r;
  r = ((glObjectUnpurgeableAPPLE = (PFNGLOBJECTUNPURGEABLEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glObjectUnpurgeableAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_object_purgeable */

#ifdef GL_APPLE_pixel_buffer

#endif /* GL_APPLE_pixel_buffer */

#ifdef GL_APPLE_rgb_422

#endif /* GL_APPLE_rgb_422 */

#ifdef GL_APPLE_row_bytes

#endif /* GL_APPLE_row_bytes */

#ifdef GL_APPLE_specular_vector

#endif /* GL_APPLE_specular_vector */

#ifdef GL_APPLE_texture_range

static GLboolean _glewInit_GL_APPLE_texture_range (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetTexParameterPointervAPPLE = (PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexParameterPointervAPPLE"))) == NULL) || r;
  r = ((glTextureRangeAPPLE = (PFNGLTEXTURERANGEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureRangeAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_texture_range */

#ifdef GL_APPLE_transform_hint

#endif /* GL_APPLE_transform_hint */

#ifdef GL_APPLE_vertex_array_object

static GLboolean _glewInit_GL_APPLE_vertex_array_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindVertexArrayAPPLE = (PFNGLBINDVERTEXARRAYAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindVertexArrayAPPLE"))) == NULL) || r;
  r = ((glDeleteVertexArraysAPPLE = (PFNGLDELETEVERTEXARRAYSAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteVertexArraysAPPLE"))) == NULL) || r;
  r = ((glGenVertexArraysAPPLE = (PFNGLGENVERTEXARRAYSAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenVertexArraysAPPLE"))) == NULL) || r;
  r = ((glIsVertexArrayAPPLE = (PFNGLISVERTEXARRAYAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsVertexArrayAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_vertex_array_object */

#ifdef GL_APPLE_vertex_array_range

static GLboolean _glewInit_GL_APPLE_vertex_array_range (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFlushVertexArrayRangeAPPLE = (PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlushVertexArrayRangeAPPLE"))) == NULL) || r;
  r = ((glVertexArrayParameteriAPPLE = (PFNGLVERTEXARRAYPARAMETERIAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayParameteriAPPLE"))) == NULL) || r;
  r = ((glVertexArrayRangeAPPLE = (PFNGLVERTEXARRAYRANGEAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayRangeAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_vertex_array_range */

#ifdef GL_APPLE_vertex_program_evaluators

static GLboolean _glewInit_GL_APPLE_vertex_program_evaluators (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDisableVertexAttribAPPLE = (PFNGLDISABLEVERTEXATTRIBAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableVertexAttribAPPLE"))) == NULL) || r;
  r = ((glEnableVertexAttribAPPLE = (PFNGLENABLEVERTEXATTRIBAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableVertexAttribAPPLE"))) == NULL) || r;
  r = ((glIsVertexAttribEnabledAPPLE = (PFNGLISVERTEXATTRIBENABLEDAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsVertexAttribEnabledAPPLE"))) == NULL) || r;
  r = ((glMapVertexAttrib1dAPPLE = (PFNGLMAPVERTEXATTRIB1DAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapVertexAttrib1dAPPLE"))) == NULL) || r;
  r = ((glMapVertexAttrib1fAPPLE = (PFNGLMAPVERTEXATTRIB1FAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapVertexAttrib1fAPPLE"))) == NULL) || r;
  r = ((glMapVertexAttrib2dAPPLE = (PFNGLMAPVERTEXATTRIB2DAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapVertexAttrib2dAPPLE"))) == NULL) || r;
  r = ((glMapVertexAttrib2fAPPLE = (PFNGLMAPVERTEXATTRIB2FAPPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapVertexAttrib2fAPPLE"))) == NULL) || r;

  return r;
}

#endif /* GL_APPLE_vertex_program_evaluators */

#ifdef GL_APPLE_ycbcr_422

#endif /* GL_APPLE_ycbcr_422 */

#ifdef GL_ARB_ES2_compatibility

static GLboolean _glewInit_GL_ARB_ES2_compatibility (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glClearDepthf = (PFNGLCLEARDEPTHFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearDepthf"))) == NULL) || r;
  r = ((glDepthRangef = (PFNGLDEPTHRANGEFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthRangef"))) == NULL) || r;
  r = ((glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetShaderPrecisionFormat"))) == NULL) || r;
  r = ((glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReleaseShaderCompiler"))) == NULL) || r;
  r = ((glShaderBinary = (PFNGLSHADERBINARYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glShaderBinary"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_ES2_compatibility */

#ifdef GL_ARB_blend_func_extended

static GLboolean _glewInit_GL_ARB_blend_func_extended (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindFragDataLocationIndexed"))) == NULL) || r;
  r = ((glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragDataIndex"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_blend_func_extended */

#ifdef GL_ARB_cl_event

static GLboolean _glewInit_GL_ARB_cl_event (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCreateSyncFromCLeventARB = (PFNGLCREATESYNCFROMCLEVENTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCreateSyncFromCLeventARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_cl_event */

#ifdef GL_ARB_color_buffer_float

static GLboolean _glewInit_GL_ARB_color_buffer_float (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glClampColorARB = (PFNGLCLAMPCOLORARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClampColorARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_color_buffer_float */

#ifdef GL_ARB_compatibility

#endif /* GL_ARB_compatibility */

#ifdef GL_ARB_copy_buffer

static GLboolean _glewInit_GL_ARB_copy_buffer (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyBufferSubData"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_copy_buffer */

#ifdef GL_ARB_debug_output

static GLboolean _glewInit_GL_ARB_debug_output (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDebugMessageCallbackARB"))) == NULL) || r;
  r = ((glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDebugMessageControlARB"))) == NULL) || r;
  r = ((glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDebugMessageInsertARB"))) == NULL) || r;
  r = ((glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetDebugMessageLogARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_debug_output */

#ifdef GL_ARB_depth_buffer_float

#endif /* GL_ARB_depth_buffer_float */

#ifdef GL_ARB_depth_clamp

#endif /* GL_ARB_depth_clamp */

#ifdef GL_ARB_depth_texture

#endif /* GL_ARB_depth_texture */

#ifdef GL_ARB_draw_buffers

static GLboolean _glewInit_GL_ARB_draw_buffers (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawBuffersARB = (PFNGLDRAWBUFFERSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawBuffersARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_draw_buffers */

#ifdef GL_ARB_draw_buffers_blend

static GLboolean _glewInit_GL_ARB_draw_buffers_blend (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendEquationSeparateiARB = (PFNGLBLENDEQUATIONSEPARATEIARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationSeparateiARB"))) == NULL) || r;
  r = ((glBlendEquationiARB = (PFNGLBLENDEQUATIONIARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationiARB"))) == NULL) || r;
  r = ((glBlendFuncSeparateiARB = (PFNGLBLENDFUNCSEPARATEIARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFuncSeparateiARB"))) == NULL) || r;
  r = ((glBlendFunciARB = (PFNGLBLENDFUNCIARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFunciARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_draw_buffers_blend */

#ifdef GL_ARB_draw_elements_base_vertex

static GLboolean _glewInit_GL_ARB_draw_elements_base_vertex (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementsBaseVertex"))) == NULL) || r;
  r = ((glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementsInstancedBaseVertex"))) == NULL) || r;
  r = ((glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawRangeElementsBaseVertex"))) == NULL) || r;
  r = ((glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiDrawElementsBaseVertex"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_draw_elements_base_vertex */

#ifdef GL_ARB_draw_indirect

static GLboolean _glewInit_GL_ARB_draw_indirect (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawArraysIndirect"))) == NULL) || r;
  r = ((glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementsIndirect"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_draw_indirect */

#ifdef GL_ARB_draw_instanced

static GLboolean _glewInit_GL_ARB_draw_instanced (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawArraysInstancedARB = (PFNGLDRAWARRAYSINSTANCEDARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawArraysInstancedARB"))) == NULL) || r;
  r = ((glDrawElementsInstancedARB = (PFNGLDRAWELEMENTSINSTANCEDARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementsInstancedARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_draw_instanced */

#ifdef GL_ARB_explicit_attrib_location

#endif /* GL_ARB_explicit_attrib_location */

#ifdef GL_ARB_fragment_coord_conventions

#endif /* GL_ARB_fragment_coord_conventions */

#ifdef GL_ARB_fragment_program

#endif /* GL_ARB_fragment_program */

#ifdef GL_ARB_fragment_program_shadow

#endif /* GL_ARB_fragment_program_shadow */

#ifdef GL_ARB_fragment_shader

#endif /* GL_ARB_fragment_shader */

#ifdef GL_ARB_framebuffer_object

static GLboolean _glewInit_GL_ARB_framebuffer_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindFramebuffer"))) == NULL) || r;
  r = ((glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindRenderbuffer"))) == NULL) || r;
  r = ((glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlitFramebuffer"))) == NULL) || r;
  r = ((glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCheckFramebufferStatus"))) == NULL) || r;
  r = ((glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteFramebuffers"))) == NULL) || r;
  r = ((glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteRenderbuffers"))) == NULL) || r;
  r = ((glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferRenderbuffer"))) == NULL) || r;
  r = ((glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTexture1D"))) == NULL) || r;
  r = ((glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTexture2D"))) == NULL) || r;
  r = ((glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTexture3D"))) == NULL) || r;
  r = ((glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTextureLayer"))) == NULL) || r;
  r = ((glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenFramebuffers"))) == NULL) || r;
  r = ((glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenRenderbuffers"))) == NULL) || r;
  r = ((glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenerateMipmap"))) == NULL) || r;
  r = ((glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFramebufferAttachmentParameteriv"))) == NULL) || r;
  r = ((glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetRenderbufferParameteriv"))) == NULL) || r;
  r = ((glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsFramebuffer"))) == NULL) || r;
  r = ((glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsRenderbuffer"))) == NULL) || r;
  r = ((glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRenderbufferStorage"))) == NULL) || r;
  r = ((glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRenderbufferStorageMultisample"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_framebuffer_object */

#ifdef GL_ARB_framebuffer_sRGB

#endif /* GL_ARB_framebuffer_sRGB */

#ifdef GL_ARB_geometry_shader4

static GLboolean _glewInit_GL_ARB_geometry_shader4 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFramebufferTextureARB = (PFNGLFRAMEBUFFERTEXTUREARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTextureARB"))) == NULL) || r;
  r = ((glFramebufferTextureFaceARB = (PFNGLFRAMEBUFFERTEXTUREFACEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTextureFaceARB"))) == NULL) || r;
  r = ((glFramebufferTextureLayerARB = (PFNGLFRAMEBUFFERTEXTURELAYERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTextureLayerARB"))) == NULL) || r;
  r = ((glProgramParameteriARB = (PFNGLPROGRAMPARAMETERIARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameteriARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_geometry_shader4 */

#ifdef GL_ARB_get_program_binary

static GLboolean _glewInit_GL_ARB_get_program_binary (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramBinary"))) == NULL) || r;
  r = ((glProgramBinary = (PFNGLPROGRAMBINARYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramBinary"))) == NULL) || r;
  r = ((glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameteri"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_get_program_binary */

#ifdef GL_ARB_gpu_shader5

#endif /* GL_ARB_gpu_shader5 */

#ifdef GL_ARB_gpu_shader_fp64

static GLboolean _glewInit_GL_ARB_gpu_shader_fp64 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetUniformdv = (PFNGLGETUNIFORMDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformdv"))) == NULL) || r;
  r = ((glProgramUniform1dEXT = (PFNGLPROGRAMUNIFORM1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1dEXT"))) == NULL) || r;
  r = ((glProgramUniform1dvEXT = (PFNGLPROGRAMUNIFORM1DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1dvEXT"))) == NULL) || r;
  r = ((glProgramUniform2dEXT = (PFNGLPROGRAMUNIFORM2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2dEXT"))) == NULL) || r;
  r = ((glProgramUniform2dvEXT = (PFNGLPROGRAMUNIFORM2DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2dvEXT"))) == NULL) || r;
  r = ((glProgramUniform3dEXT = (PFNGLPROGRAMUNIFORM3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3dEXT"))) == NULL) || r;
  r = ((glProgramUniform3dvEXT = (PFNGLPROGRAMUNIFORM3DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3dvEXT"))) == NULL) || r;
  r = ((glProgramUniform4dEXT = (PFNGLPROGRAMUNIFORM4DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4dEXT"))) == NULL) || r;
  r = ((glProgramUniform4dvEXT = (PFNGLPROGRAMUNIFORM4DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x3dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x4dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x2dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x4dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x2dvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x3dvEXT"))) == NULL) || r;
  r = ((glUniform1d = (PFNGLUNIFORM1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1d"))) == NULL) || r;
  r = ((glUniform1dv = (PFNGLUNIFORM1DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1dv"))) == NULL) || r;
  r = ((glUniform2d = (PFNGLUNIFORM2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2d"))) == NULL) || r;
  r = ((glUniform2dv = (PFNGLUNIFORM2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2dv"))) == NULL) || r;
  r = ((glUniform3d = (PFNGLUNIFORM3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3d"))) == NULL) || r;
  r = ((glUniform3dv = (PFNGLUNIFORM3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3dv"))) == NULL) || r;
  r = ((glUniform4d = (PFNGLUNIFORM4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4d"))) == NULL) || r;
  r = ((glUniform4dv = (PFNGLUNIFORM4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4dv"))) == NULL) || r;
  r = ((glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix2dv"))) == NULL) || r;
  r = ((glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix2x3dv"))) == NULL) || r;
  r = ((glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix2x4dv"))) == NULL) || r;
  r = ((glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix3dv"))) == NULL) || r;
  r = ((glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix3x2dv"))) == NULL) || r;
  r = ((glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix3x4dv"))) == NULL) || r;
  r = ((glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix4dv"))) == NULL) || r;
  r = ((glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix4x2dv"))) == NULL) || r;
  r = ((glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix4x3dv"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_gpu_shader_fp64 */

#ifdef GL_ARB_half_float_pixel

#endif /* GL_ARB_half_float_pixel */

#ifdef GL_ARB_half_float_vertex

#endif /* GL_ARB_half_float_vertex */

#ifdef GL_ARB_imaging

static GLboolean _glewInit_GL_ARB_imaging (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendEquation = (PFNGLBLENDEQUATIONPROC)glewGetProcAddress("glBlendEquation")) == NULL) || r;
  r = ((glColorSubTable = (PFNGLCOLORSUBTABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorSubTable"))) == NULL) || r;
  r = ((glColorTable = (PFNGLCOLORTABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorTable"))) == NULL) || r;
  r = ((glColorTableParameterfv = (PFNGLCOLORTABLEPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorTableParameterfv"))) == NULL) || r;
  r = ((glColorTableParameteriv = (PFNGLCOLORTABLEPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorTableParameteriv"))) == NULL) || r;
  r = ((glConvolutionFilter1D = (PFNGLCONVOLUTIONFILTER1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionFilter1D"))) == NULL) || r;
  r = ((glConvolutionFilter2D = (PFNGLCONVOLUTIONFILTER2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionFilter2D"))) == NULL) || r;
  r = ((glConvolutionParameterf = (PFNGLCONVOLUTIONPARAMETERFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameterf"))) == NULL) || r;
  r = ((glConvolutionParameterfv = (PFNGLCONVOLUTIONPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameterfv"))) == NULL) || r;
  r = ((glConvolutionParameteri = (PFNGLCONVOLUTIONPARAMETERIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameteri"))) == NULL) || r;
  r = ((glConvolutionParameteriv = (PFNGLCONVOLUTIONPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameteriv"))) == NULL) || r;
  r = ((glCopyColorSubTable = (PFNGLCOPYCOLORSUBTABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyColorSubTable"))) == NULL) || r;
  r = ((glCopyColorTable = (PFNGLCOPYCOLORTABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyColorTable"))) == NULL) || r;
  r = ((glCopyConvolutionFilter1D = (PFNGLCOPYCONVOLUTIONFILTER1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyConvolutionFilter1D"))) == NULL) || r;
  r = ((glCopyConvolutionFilter2D = (PFNGLCOPYCONVOLUTIONFILTER2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyConvolutionFilter2D"))) == NULL) || r;
  r = ((glGetColorTable = (PFNGLGETCOLORTABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTable"))) == NULL) || r;
  r = ((glGetColorTableParameterfv = (PFNGLGETCOLORTABLEPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableParameterfv"))) == NULL) || r;
  r = ((glGetColorTableParameteriv = (PFNGLGETCOLORTABLEPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableParameteriv"))) == NULL) || r;
  r = ((glGetConvolutionFilter = (PFNGLGETCONVOLUTIONFILTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetConvolutionFilter"))) == NULL) || r;
  r = ((glGetConvolutionParameterfv = (PFNGLGETCONVOLUTIONPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetConvolutionParameterfv"))) == NULL) || r;
  r = ((glGetConvolutionParameteriv = (PFNGLGETCONVOLUTIONPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetConvolutionParameteriv"))) == NULL) || r;
  r = ((glGetHistogram = (PFNGLGETHISTOGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetHistogram"))) == NULL) || r;
  r = ((glGetHistogramParameterfv = (PFNGLGETHISTOGRAMPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetHistogramParameterfv"))) == NULL) || r;
  r = ((glGetHistogramParameteriv = (PFNGLGETHISTOGRAMPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetHistogramParameteriv"))) == NULL) || r;
  r = ((glGetMinmax = (PFNGLGETMINMAXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMinmax"))) == NULL) || r;
  r = ((glGetMinmaxParameterfv = (PFNGLGETMINMAXPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMinmaxParameterfv"))) == NULL) || r;
  r = ((glGetMinmaxParameteriv = (PFNGLGETMINMAXPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMinmaxParameteriv"))) == NULL) || r;
  r = ((glGetSeparableFilter = (PFNGLGETSEPARABLEFILTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSeparableFilter"))) == NULL) || r;
  r = ((glHistogram = (PFNGLHISTOGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glHistogram"))) == NULL) || r;
  r = ((glMinmax = (PFNGLMINMAXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMinmax"))) == NULL) || r;
  r = ((glResetHistogram = (PFNGLRESETHISTOGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glResetHistogram"))) == NULL) || r;
  r = ((glResetMinmax = (PFNGLRESETMINMAXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glResetMinmax"))) == NULL) || r;
  r = ((glSeparableFilter2D = (PFNGLSEPARABLEFILTER2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSeparableFilter2D"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_imaging */

#ifdef GL_ARB_instanced_arrays

static GLboolean _glewInit_GL_ARB_instanced_arrays (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glVertexAttribDivisorARB = (PFNGLVERTEXATTRIBDIVISORARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribDivisorARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_instanced_arrays */

#ifdef GL_ARB_map_buffer_range

static GLboolean _glewInit_GL_ARB_map_buffer_range (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlushMappedBufferRange"))) == NULL) || r;
  r = ((glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapBufferRange"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_map_buffer_range */

#ifdef GL_ARB_matrix_palette

static GLboolean _glewInit_GL_ARB_matrix_palette (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCurrentPaletteMatrixARB = (PFNGLCURRENTPALETTEMATRIXARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCurrentPaletteMatrixARB"))) == NULL) || r;
  r = ((glMatrixIndexPointerARB = (PFNGLMATRIXINDEXPOINTERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixIndexPointerARB"))) == NULL) || r;
  r = ((glMatrixIndexubvARB = (PFNGLMATRIXINDEXUBVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixIndexubvARB"))) == NULL) || r;
  r = ((glMatrixIndexuivARB = (PFNGLMATRIXINDEXUIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixIndexuivARB"))) == NULL) || r;
  r = ((glMatrixIndexusvARB = (PFNGLMATRIXINDEXUSVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixIndexusvARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_matrix_palette */

#ifdef GL_ARB_multisample

static GLboolean _glewInit_GL_ARB_multisample (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glSampleCoverageARB = (PFNGLSAMPLECOVERAGEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSampleCoverageARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_multisample */

#ifdef GL_ARB_multitexture

static GLboolean _glewInit_GL_ARB_multitexture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glActiveTextureARB"))) == NULL) || r;
  r = ((glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClientActiveTextureARB"))) == NULL) || r;
  r = ((glMultiTexCoord1dARB = (PFNGLMULTITEXCOORD1DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1dARB"))) == NULL) || r;
  r = ((glMultiTexCoord1dvARB = (PFNGLMULTITEXCOORD1DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1dvARB"))) == NULL) || r;
  r = ((glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1fARB"))) == NULL) || r;
  r = ((glMultiTexCoord1fvARB = (PFNGLMULTITEXCOORD1FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1fvARB"))) == NULL) || r;
  r = ((glMultiTexCoord1iARB = (PFNGLMULTITEXCOORD1IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1iARB"))) == NULL) || r;
  r = ((glMultiTexCoord1ivARB = (PFNGLMULTITEXCOORD1IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1ivARB"))) == NULL) || r;
  r = ((glMultiTexCoord1sARB = (PFNGLMULTITEXCOORD1SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1sARB"))) == NULL) || r;
  r = ((glMultiTexCoord1svARB = (PFNGLMULTITEXCOORD1SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1svARB"))) == NULL) || r;
  r = ((glMultiTexCoord2dARB = (PFNGLMULTITEXCOORD2DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2dARB"))) == NULL) || r;
  r = ((glMultiTexCoord2dvARB = (PFNGLMULTITEXCOORD2DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2dvARB"))) == NULL) || r;
  r = ((glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2fARB"))) == NULL) || r;
  r = ((glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2fvARB"))) == NULL) || r;
  r = ((glMultiTexCoord2iARB = (PFNGLMULTITEXCOORD2IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2iARB"))) == NULL) || r;
  r = ((glMultiTexCoord2ivARB = (PFNGLMULTITEXCOORD2IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2ivARB"))) == NULL) || r;
  r = ((glMultiTexCoord2sARB = (PFNGLMULTITEXCOORD2SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2sARB"))) == NULL) || r;
  r = ((glMultiTexCoord2svARB = (PFNGLMULTITEXCOORD2SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2svARB"))) == NULL) || r;
  r = ((glMultiTexCoord3dARB = (PFNGLMULTITEXCOORD3DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3dARB"))) == NULL) || r;
  r = ((glMultiTexCoord3dvARB = (PFNGLMULTITEXCOORD3DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3dvARB"))) == NULL) || r;
  r = ((glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3fARB"))) == NULL) || r;
  r = ((glMultiTexCoord3fvARB = (PFNGLMULTITEXCOORD3FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3fvARB"))) == NULL) || r;
  r = ((glMultiTexCoord3iARB = (PFNGLMULTITEXCOORD3IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3iARB"))) == NULL) || r;
  r = ((glMultiTexCoord3ivARB = (PFNGLMULTITEXCOORD3IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3ivARB"))) == NULL) || r;
  r = ((glMultiTexCoord3sARB = (PFNGLMULTITEXCOORD3SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3sARB"))) == NULL) || r;
  r = ((glMultiTexCoord3svARB = (PFNGLMULTITEXCOORD3SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3svARB"))) == NULL) || r;
  r = ((glMultiTexCoord4dARB = (PFNGLMULTITEXCOORD4DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4dARB"))) == NULL) || r;
  r = ((glMultiTexCoord4dvARB = (PFNGLMULTITEXCOORD4DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4dvARB"))) == NULL) || r;
  r = ((glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4fARB"))) == NULL) || r;
  r = ((glMultiTexCoord4fvARB = (PFNGLMULTITEXCOORD4FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4fvARB"))) == NULL) || r;
  r = ((glMultiTexCoord4iARB = (PFNGLMULTITEXCOORD4IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4iARB"))) == NULL) || r;
  r = ((glMultiTexCoord4ivARB = (PFNGLMULTITEXCOORD4IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4ivARB"))) == NULL) || r;
  r = ((glMultiTexCoord4sARB = (PFNGLMULTITEXCOORD4SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4sARB"))) == NULL) || r;
  r = ((glMultiTexCoord4svARB = (PFNGLMULTITEXCOORD4SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4svARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_multitexture */

#ifdef GL_ARB_occlusion_query

static GLboolean _glewInit_GL_ARB_occlusion_query (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginQueryARB = (PFNGLBEGINQUERYARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginQueryARB"))) == NULL) || r;
  r = ((glDeleteQueriesARB = (PFNGLDELETEQUERIESARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteQueriesARB"))) == NULL) || r;
  r = ((glEndQueryARB = (PFNGLENDQUERYARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndQueryARB"))) == NULL) || r;
  r = ((glGenQueriesARB = (PFNGLGENQUERIESARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenQueriesARB"))) == NULL) || r;
  r = ((glGetQueryObjectivARB = (PFNGLGETQUERYOBJECTIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjectivARB"))) == NULL) || r;
  r = ((glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjectuivARB"))) == NULL) || r;
  r = ((glGetQueryivARB = (PFNGLGETQUERYIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryivARB"))) == NULL) || r;
  r = ((glIsQueryARB = (PFNGLISQUERYARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsQueryARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_occlusion_query */

#ifdef GL_ARB_occlusion_query2

#endif /* GL_ARB_occlusion_query2 */

#ifdef GL_ARB_pixel_buffer_object

#endif /* GL_ARB_pixel_buffer_object */

#ifdef GL_ARB_point_parameters

static GLboolean _glewInit_GL_ARB_point_parameters (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPointParameterfARB = (PFNGLPOINTPARAMETERFARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameterfARB"))) == NULL) || r;
  r = ((glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameterfvARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_point_parameters */

#ifdef GL_ARB_point_sprite

#endif /* GL_ARB_point_sprite */

#ifdef GL_ARB_provoking_vertex

static GLboolean _glewInit_GL_ARB_provoking_vertex (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProvokingVertex"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_provoking_vertex */

#ifdef GL_ARB_robustness

static GLboolean _glewInit_GL_ARB_robustness (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetnColorTableARB = (PFNGLGETNCOLORTABLEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnColorTableARB"))) == NULL) || r;
  r = ((glGetnCompressedTexImageARB = (PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnCompressedTexImageARB"))) == NULL) || r;
  r = ((glGetnConvolutionFilterARB = (PFNGLGETNCONVOLUTIONFILTERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnConvolutionFilterARB"))) == NULL) || r;
  r = ((glGetnHistogramARB = (PFNGLGETNHISTOGRAMARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnHistogramARB"))) == NULL) || r;
  r = ((glGetnMapdvARB = (PFNGLGETNMAPDVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnMapdvARB"))) == NULL) || r;
  r = ((glGetnMapfvARB = (PFNGLGETNMAPFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnMapfvARB"))) == NULL) || r;
  r = ((glGetnMapivARB = (PFNGLGETNMAPIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnMapivARB"))) == NULL) || r;
  r = ((glGetnMinmaxARB = (PFNGLGETNMINMAXARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnMinmaxARB"))) == NULL) || r;
  r = ((glGetnPixelMapfvARB = (PFNGLGETNPIXELMAPFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnPixelMapfvARB"))) == NULL) || r;
  r = ((glGetnPixelMapuivARB = (PFNGLGETNPIXELMAPUIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnPixelMapuivARB"))) == NULL) || r;
  r = ((glGetnPixelMapusvARB = (PFNGLGETNPIXELMAPUSVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnPixelMapusvARB"))) == NULL) || r;
  r = ((glGetnPolygonStippleARB = (PFNGLGETNPOLYGONSTIPPLEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnPolygonStippleARB"))) == NULL) || r;
  r = ((glGetnSeparableFilterARB = (PFNGLGETNSEPARABLEFILTERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnSeparableFilterARB"))) == NULL) || r;
  r = ((glGetnTexImageARB = (PFNGLGETNTEXIMAGEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnTexImageARB"))) == NULL) || r;
  r = ((glGetnUniformdvARB = (PFNGLGETNUNIFORMDVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnUniformdvARB"))) == NULL) || r;
  r = ((glGetnUniformfvARB = (PFNGLGETNUNIFORMFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnUniformfvARB"))) == NULL) || r;
  r = ((glGetnUniformivARB = (PFNGLGETNUNIFORMIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnUniformivARB"))) == NULL) || r;
  r = ((glGetnUniformuivARB = (PFNGLGETNUNIFORMUIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetnUniformuivARB"))) == NULL) || r;
  r = ((glReadnPixelsARB = (PFNGLREADNPIXELSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReadnPixelsARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_robustness */

#ifdef GL_ARB_sample_shading

static GLboolean _glewInit_GL_ARB_sample_shading (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glMinSampleShadingARB = (PFNGLMINSAMPLESHADINGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMinSampleShadingARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_sample_shading */

#ifdef GL_ARB_sampler_objects

static GLboolean _glewInit_GL_ARB_sampler_objects (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindSampler = (PFNGLBINDSAMPLERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindSampler"))) == NULL) || r;
  r = ((glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteSamplers"))) == NULL) || r;
  r = ((glGenSamplers = (PFNGLGENSAMPLERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenSamplers"))) == NULL) || r;
  r = ((glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSamplerParameterIiv"))) == NULL) || r;
  r = ((glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSamplerParameterIuiv"))) == NULL) || r;
  r = ((glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSamplerParameterfv"))) == NULL) || r;
  r = ((glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSamplerParameteriv"))) == NULL) || r;
  r = ((glIsSampler = (PFNGLISSAMPLERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsSampler"))) == NULL) || r;
  r = ((glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplerParameterIiv"))) == NULL) || r;
  r = ((glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplerParameterIuiv"))) == NULL) || r;
  r = ((glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplerParameterf"))) == NULL) || r;
  r = ((glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplerParameterfv"))) == NULL) || r;
  r = ((glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplerParameteri"))) == NULL) || r;
  r = ((glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplerParameteriv"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_sampler_objects */

#ifdef GL_ARB_seamless_cube_map

#endif /* GL_ARB_seamless_cube_map */

#ifdef GL_ARB_separate_shader_objects

static GLboolean _glewInit_GL_ARB_separate_shader_objects (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glActiveShaderProgram"))) == NULL) || r;
  r = ((glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindProgramPipeline"))) == NULL) || r;
  r = ((glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCreateShaderProgramv"))) == NULL) || r;
  r = ((glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteProgramPipelines"))) == NULL) || r;
  r = ((glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenProgramPipelines"))) == NULL) || r;
  r = ((glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramPipelineInfoLog"))) == NULL) || r;
  r = ((glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramPipelineiv"))) == NULL) || r;
  r = ((glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsProgramPipeline"))) == NULL) || r;
  r = ((glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1d"))) == NULL) || r;
  r = ((glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1dv"))) == NULL) || r;
  r = ((glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1f"))) == NULL) || r;
  r = ((glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1fv"))) == NULL) || r;
  r = ((glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1i"))) == NULL) || r;
  r = ((glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1iv"))) == NULL) || r;
  r = ((glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1ui"))) == NULL) || r;
  r = ((glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1uiv"))) == NULL) || r;
  r = ((glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2d"))) == NULL) || r;
  r = ((glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2dv"))) == NULL) || r;
  r = ((glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2f"))) == NULL) || r;
  r = ((glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2fv"))) == NULL) || r;
  r = ((glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2i"))) == NULL) || r;
  r = ((glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2iv"))) == NULL) || r;
  r = ((glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2ui"))) == NULL) || r;
  r = ((glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2uiv"))) == NULL) || r;
  r = ((glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3d"))) == NULL) || r;
  r = ((glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3dv"))) == NULL) || r;
  r = ((glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3f"))) == NULL) || r;
  r = ((glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3fv"))) == NULL) || r;
  r = ((glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3i"))) == NULL) || r;
  r = ((glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3iv"))) == NULL) || r;
  r = ((glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3ui"))) == NULL) || r;
  r = ((glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3uiv"))) == NULL) || r;
  r = ((glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4d"))) == NULL) || r;
  r = ((glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4dv"))) == NULL) || r;
  r = ((glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4f"))) == NULL) || r;
  r = ((glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4fv"))) == NULL) || r;
  r = ((glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4i"))) == NULL) || r;
  r = ((glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4iv"))) == NULL) || r;
  r = ((glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4ui"))) == NULL) || r;
  r = ((glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4uiv"))) == NULL) || r;
  r = ((glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x3dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x3fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x4dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x4fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x2dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x2fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x4dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x4fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x2dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x2fv"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x3dv"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x3fv"))) == NULL) || r;
  r = ((glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUseProgramStages"))) == NULL) || r;
  r = ((glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glValidateProgramPipeline"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_separate_shader_objects */

#ifdef GL_ARB_shader_bit_encoding

#endif /* GL_ARB_shader_bit_encoding */

#ifdef GL_ARB_shader_objects

static GLboolean _glewInit_GL_ARB_shader_objects (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAttachObjectARB"))) == NULL) || r;
  r = ((glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompileShaderARB"))) == NULL) || r;
  r = ((glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCreateProgramObjectARB"))) == NULL) || r;
  r = ((glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCreateShaderObjectARB"))) == NULL) || r;
  r = ((glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteObjectARB"))) == NULL) || r;
  r = ((glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDetachObjectARB"))) == NULL) || r;
  r = ((glGetActiveUniformARB = (PFNGLGETACTIVEUNIFORMARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveUniformARB"))) == NULL) || r;
  r = ((glGetAttachedObjectsARB = (PFNGLGETATTACHEDOBJECTSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetAttachedObjectsARB"))) == NULL) || r;
  r = ((glGetHandleARB = (PFNGLGETHANDLEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetHandleARB"))) == NULL) || r;
  r = ((glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetInfoLogARB"))) == NULL) || r;
  r = ((glGetObjectParameterfvARB = (PFNGLGETOBJECTPARAMETERFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetObjectParameterfvARB"))) == NULL) || r;
  r = ((glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetObjectParameterivARB"))) == NULL) || r;
  r = ((glGetShaderSourceARB = (PFNGLGETSHADERSOURCEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetShaderSourceARB"))) == NULL) || r;
  r = ((glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformLocationARB"))) == NULL) || r;
  r = ((glGetUniformfvARB = (PFNGLGETUNIFORMFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformfvARB"))) == NULL) || r;
  r = ((glGetUniformivARB = (PFNGLGETUNIFORMIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformivARB"))) == NULL) || r;
  r = ((glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLinkProgramARB"))) == NULL) || r;
  r = ((glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glShaderSourceARB"))) == NULL) || r;
  r = ((glUniform1fARB = (PFNGLUNIFORM1FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1fARB"))) == NULL) || r;
  r = ((glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1fvARB"))) == NULL) || r;
  r = ((glUniform1iARB = (PFNGLUNIFORM1IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1iARB"))) == NULL) || r;
  r = ((glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1ivARB"))) == NULL) || r;
  r = ((glUniform2fARB = (PFNGLUNIFORM2FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2fARB"))) == NULL) || r;
  r = ((glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2fvARB"))) == NULL) || r;
  r = ((glUniform2iARB = (PFNGLUNIFORM2IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2iARB"))) == NULL) || r;
  r = ((glUniform2ivARB = (PFNGLUNIFORM2IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2ivARB"))) == NULL) || r;
  r = ((glUniform3fARB = (PFNGLUNIFORM3FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3fARB"))) == NULL) || r;
  r = ((glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3fvARB"))) == NULL) || r;
  r = ((glUniform3iARB = (PFNGLUNIFORM3IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3iARB"))) == NULL) || r;
  r = ((glUniform3ivARB = (PFNGLUNIFORM3IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3ivARB"))) == NULL) || r;
  r = ((glUniform4fARB = (PFNGLUNIFORM4FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4fARB"))) == NULL) || r;
  r = ((glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4fvARB"))) == NULL) || r;
  r = ((glUniform4iARB = (PFNGLUNIFORM4IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4iARB"))) == NULL) || r;
  r = ((glUniform4ivARB = (PFNGLUNIFORM4IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4ivARB"))) == NULL) || r;
  r = ((glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix2fvARB"))) == NULL) || r;
  r = ((glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix3fvARB"))) == NULL) || r;
  r = ((glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformMatrix4fvARB"))) == NULL) || r;
  r = ((glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUseProgramObjectARB"))) == NULL) || r;
  r = ((glValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glValidateProgramARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_shader_objects */

#ifdef GL_ARB_shader_precision

#endif /* GL_ARB_shader_precision */

#ifdef GL_ARB_shader_stencil_export

#endif /* GL_ARB_shader_stencil_export */

#ifdef GL_ARB_shader_subroutine

static GLboolean _glewInit_GL_ARB_shader_subroutine (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveSubroutineName"))) == NULL) || r;
  r = ((glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveSubroutineUniformName"))) == NULL) || r;
  r = ((glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveSubroutineUniformiv"))) == NULL) || r;
  r = ((glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramStageiv"))) == NULL) || r;
  r = ((glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSubroutineIndex"))) == NULL) || r;
  r = ((glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSubroutineUniformLocation"))) == NULL) || r;
  r = ((glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformSubroutineuiv"))) == NULL) || r;
  r = ((glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformSubroutinesuiv"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_shader_subroutine */

#ifdef GL_ARB_shader_texture_lod

#endif /* GL_ARB_shader_texture_lod */

#ifdef GL_ARB_shading_language_100

#endif /* GL_ARB_shading_language_100 */

#ifdef GL_ARB_shading_language_include

static GLboolean _glewInit_GL_ARB_shading_language_include (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCompileShaderIncludeARB = (PFNGLCOMPILESHADERINCLUDEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompileShaderIncludeARB"))) == NULL) || r;
  r = ((glDeleteNamedStringARB = (PFNGLDELETENAMEDSTRINGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteNamedStringARB"))) == NULL) || r;
  r = ((glGetNamedStringARB = (PFNGLGETNAMEDSTRINGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedStringARB"))) == NULL) || r;
  r = ((glGetNamedStringivARB = (PFNGLGETNAMEDSTRINGIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedStringivARB"))) == NULL) || r;
  r = ((glIsNamedStringARB = (PFNGLISNAMEDSTRINGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsNamedStringARB"))) == NULL) || r;
  r = ((glNamedStringARB = (PFNGLNAMEDSTRINGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedStringARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_shading_language_include */

#ifdef GL_ARB_shadow

#endif /* GL_ARB_shadow */

#ifdef GL_ARB_shadow_ambient

#endif /* GL_ARB_shadow_ambient */

#ifdef GL_ARB_sync

static GLboolean _glewInit_GL_ARB_sync (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClientWaitSync"))) == NULL) || r;
  r = ((glDeleteSync = (PFNGLDELETESYNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteSync"))) == NULL) || r;
  r = ((glFenceSync = (PFNGLFENCESYNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFenceSync"))) == NULL) || r;
  r = ((glGetInteger64v = (PFNGLGETINTEGER64VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetInteger64v"))) == NULL) || r;
  r = ((glGetSynciv = (PFNGLGETSYNCIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSynciv"))) == NULL) || r;
  r = ((glIsSync = (PFNGLISSYNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsSync"))) == NULL) || r;
  r = ((glWaitSync = (PFNGLWAITSYNCPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWaitSync"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_sync */

#ifdef GL_ARB_tessellation_shader

static GLboolean _glewInit_GL_ARB_tessellation_shader (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPatchParameterfv"))) == NULL) || r;
  r = ((glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPatchParameteri"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_tessellation_shader */

#ifdef GL_ARB_texture_border_clamp

#endif /* GL_ARB_texture_border_clamp */

#ifdef GL_ARB_texture_buffer_object

static GLboolean _glewInit_GL_ARB_texture_buffer_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTexBufferARB = (PFNGLTEXBUFFERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexBufferARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_texture_buffer_object */

#ifdef GL_ARB_texture_buffer_object_rgb32

#endif /* GL_ARB_texture_buffer_object_rgb32 */

#ifdef GL_ARB_texture_compression

static GLboolean _glewInit_GL_ARB_texture_compression (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCompressedTexImage1DARB = (PFNGLCOMPRESSEDTEXIMAGE1DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexImage1DARB"))) == NULL) || r;
  r = ((glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexImage2DARB"))) == NULL) || r;
  r = ((glCompressedTexImage3DARB = (PFNGLCOMPRESSEDTEXIMAGE3DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexImage3DARB"))) == NULL) || r;
  r = ((glCompressedTexSubImage1DARB = (PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexSubImage1DARB"))) == NULL) || r;
  r = ((glCompressedTexSubImage2DARB = (PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexSubImage2DARB"))) == NULL) || r;
  r = ((glCompressedTexSubImage3DARB = (PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTexSubImage3DARB"))) == NULL) || r;
  r = ((glGetCompressedTexImageARB = (PFNGLGETCOMPRESSEDTEXIMAGEARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCompressedTexImageARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_texture_compression */

#ifdef GL_ARB_texture_compression_bptc

#endif /* GL_ARB_texture_compression_bptc */

#ifdef GL_ARB_texture_compression_rgtc

#endif /* GL_ARB_texture_compression_rgtc */

#ifdef GL_ARB_texture_cube_map

#endif /* GL_ARB_texture_cube_map */

#ifdef GL_ARB_texture_cube_map_array

#endif /* GL_ARB_texture_cube_map_array */

#ifdef GL_ARB_texture_env_add

#endif /* GL_ARB_texture_env_add */

#ifdef GL_ARB_texture_env_combine

#endif /* GL_ARB_texture_env_combine */

#ifdef GL_ARB_texture_env_crossbar

#endif /* GL_ARB_texture_env_crossbar */

#ifdef GL_ARB_texture_env_dot3

#endif /* GL_ARB_texture_env_dot3 */

#ifdef GL_ARB_texture_float

#endif /* GL_ARB_texture_float */

#ifdef GL_ARB_texture_gather

#endif /* GL_ARB_texture_gather */

#ifdef GL_ARB_texture_mirrored_repeat

#endif /* GL_ARB_texture_mirrored_repeat */

#ifdef GL_ARB_texture_multisample

static GLboolean _glewInit_GL_ARB_texture_multisample (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultisamplefv"))) == NULL) || r;
  r = ((glSampleMaski = (PFNGLSAMPLEMASKIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSampleMaski"))) == NULL) || r;
  r = ((glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexImage2DMultisample"))) == NULL) || r;
  r = ((glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexImage3DMultisample"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_texture_multisample */

#ifdef GL_ARB_texture_non_power_of_two

#endif /* GL_ARB_texture_non_power_of_two */

#ifdef GL_ARB_texture_query_lod

#endif /* GL_ARB_texture_query_lod */

#ifdef GL_ARB_texture_rectangle

#endif /* GL_ARB_texture_rectangle */

#ifdef GL_ARB_texture_rg

#endif /* GL_ARB_texture_rg */

#ifdef GL_ARB_texture_rgb10_a2ui

#endif /* GL_ARB_texture_rgb10_a2ui */

#ifdef GL_ARB_texture_swizzle

#endif /* GL_ARB_texture_swizzle */

#ifdef GL_ARB_timer_query

static GLboolean _glewInit_GL_ARB_timer_query (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjecti64v"))) == NULL) || r;
  r = ((glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjectui64v"))) == NULL) || r;
  r = ((glQueryCounter = (PFNGLQUERYCOUNTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glQueryCounter"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_timer_query */

#ifdef GL_ARB_transform_feedback2

static GLboolean _glewInit_GL_ARB_transform_feedback2 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindTransformFeedback"))) == NULL) || r;
  r = ((glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteTransformFeedbacks"))) == NULL) || r;
  r = ((glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawTransformFeedback"))) == NULL) || r;
  r = ((glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenTransformFeedbacks"))) == NULL) || r;
  r = ((glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsTransformFeedback"))) == NULL) || r;
  r = ((glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPauseTransformFeedback"))) == NULL) || r;
  r = ((glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glResumeTransformFeedback"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_transform_feedback2 */

#ifdef GL_ARB_transform_feedback3

static GLboolean _glewInit_GL_ARB_transform_feedback3 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginQueryIndexed"))) == NULL) || r;
  r = ((glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawTransformFeedbackStream"))) == NULL) || r;
  r = ((glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndQueryIndexed"))) == NULL) || r;
  r = ((glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryIndexediv"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_transform_feedback3 */

#ifdef GL_ARB_transpose_matrix

static GLboolean _glewInit_GL_ARB_transpose_matrix (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glLoadTransposeMatrixdARB = (PFNGLLOADTRANSPOSEMATRIXDARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadTransposeMatrixdARB"))) == NULL) || r;
  r = ((glLoadTransposeMatrixfARB = (PFNGLLOADTRANSPOSEMATRIXFARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadTransposeMatrixfARB"))) == NULL) || r;
  r = ((glMultTransposeMatrixdARB = (PFNGLMULTTRANSPOSEMATRIXDARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultTransposeMatrixdARB"))) == NULL) || r;
  r = ((glMultTransposeMatrixfARB = (PFNGLMULTTRANSPOSEMATRIXFARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultTransposeMatrixfARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_transpose_matrix */

#ifdef GL_ARB_uniform_buffer_object

static GLboolean _glewInit_GL_ARB_uniform_buffer_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferBase"))) == NULL) || r;
  r = ((glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferRange"))) == NULL) || r;
  r = ((glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveUniformBlockName"))) == NULL) || r;
  r = ((glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveUniformBlockiv"))) == NULL) || r;
  r = ((glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveUniformName"))) == NULL) || r;
  r = ((glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveUniformsiv"))) == NULL) || r;
  r = ((glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetIntegeri_v"))) == NULL) || r;
  r = ((glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformBlockIndex"))) == NULL) || r;
  r = ((glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformIndices"))) == NULL) || r;
  r = ((glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformBlockBinding"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_uniform_buffer_object */

#ifdef GL_ARB_vertex_array_bgra

#endif /* GL_ARB_vertex_array_bgra */

#ifdef GL_ARB_vertex_array_object

static GLboolean _glewInit_GL_ARB_vertex_array_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindVertexArray"))) == NULL) || r;
  r = ((glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteVertexArrays"))) == NULL) || r;
  r = ((glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenVertexArrays"))) == NULL) || r;
  r = ((glIsVertexArray = (PFNGLISVERTEXARRAYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsVertexArray"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_vertex_array_object */

#ifdef GL_ARB_vertex_attrib_64bit

static GLboolean _glewInit_GL_ARB_vertex_attrib_64bit (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribLdv"))) == NULL) || r;
  r = ((glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1d"))) == NULL) || r;
  r = ((glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1dv"))) == NULL) || r;
  r = ((glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2d"))) == NULL) || r;
  r = ((glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2dv"))) == NULL) || r;
  r = ((glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3d"))) == NULL) || r;
  r = ((glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3dv"))) == NULL) || r;
  r = ((glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4d"))) == NULL) || r;
  r = ((glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4dv"))) == NULL) || r;
  r = ((glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribLPointer"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_vertex_attrib_64bit */

#ifdef GL_ARB_vertex_blend

static GLboolean _glewInit_GL_ARB_vertex_blend (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glVertexBlendARB = (PFNGLVERTEXBLENDARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexBlendARB"))) == NULL) || r;
  r = ((glWeightPointerARB = (PFNGLWEIGHTPOINTERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightPointerARB"))) == NULL) || r;
  r = ((glWeightbvARB = (PFNGLWEIGHTBVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightbvARB"))) == NULL) || r;
  r = ((glWeightdvARB = (PFNGLWEIGHTDVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightdvARB"))) == NULL) || r;
  r = ((glWeightfvARB = (PFNGLWEIGHTFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightfvARB"))) == NULL) || r;
  r = ((glWeightivARB = (PFNGLWEIGHTIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightivARB"))) == NULL) || r;
  r = ((glWeightsvARB = (PFNGLWEIGHTSVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightsvARB"))) == NULL) || r;
  r = ((glWeightubvARB = (PFNGLWEIGHTUBVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightubvARB"))) == NULL) || r;
  r = ((glWeightuivARB = (PFNGLWEIGHTUIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightuivARB"))) == NULL) || r;
  r = ((glWeightusvARB = (PFNGLWEIGHTUSVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWeightusvARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_vertex_blend */

#ifdef GL_ARB_vertex_buffer_object

static GLboolean _glewInit_GL_ARB_vertex_buffer_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindBufferARB = (PFNGLBINDBUFFERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferARB"))) == NULL) || r;
  r = ((glBufferDataARB = (PFNGLBUFFERDATAARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBufferDataARB"))) == NULL) || r;
  r = ((glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBufferSubDataARB"))) == NULL) || r;
  r = ((glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteBuffersARB"))) == NULL) || r;
  r = ((glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenBuffersARB"))) == NULL) || r;
  r = ((glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferParameterivARB"))) == NULL) || r;
  r = ((glGetBufferPointervARB = (PFNGLGETBUFFERPOINTERVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferPointervARB"))) == NULL) || r;
  r = ((glGetBufferSubDataARB = (PFNGLGETBUFFERSUBDATAARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferSubDataARB"))) == NULL) || r;
  r = ((glIsBufferARB = (PFNGLISBUFFERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsBufferARB"))) == NULL) || r;
  r = ((glMapBufferARB = (PFNGLMAPBUFFERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapBufferARB"))) == NULL) || r;
  r = ((glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUnmapBufferARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_vertex_buffer_object */

#ifdef GL_ARB_vertex_program

static GLboolean _glewInit_GL_ARB_vertex_program (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindProgramARB"))) == NULL) || r;
  r = ((glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteProgramsARB"))) == NULL) || r;
  r = ((glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableVertexAttribArrayARB"))) == NULL) || r;
  r = ((glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableVertexAttribArrayARB"))) == NULL) || r;
  r = ((glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenProgramsARB"))) == NULL) || r;
  r = ((glGetProgramEnvParameterdvARB = (PFNGLGETPROGRAMENVPARAMETERDVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramEnvParameterdvARB"))) == NULL) || r;
  r = ((glGetProgramEnvParameterfvARB = (PFNGLGETPROGRAMENVPARAMETERFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramEnvParameterfvARB"))) == NULL) || r;
  r = ((glGetProgramLocalParameterdvARB = (PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramLocalParameterdvARB"))) == NULL) || r;
  r = ((glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramLocalParameterfvARB"))) == NULL) || r;
  r = ((glGetProgramStringARB = (PFNGLGETPROGRAMSTRINGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramStringARB"))) == NULL) || r;
  r = ((glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramivARB"))) == NULL) || r;
  r = ((glGetVertexAttribPointervARB = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribPointervARB"))) == NULL) || r;
  r = ((glGetVertexAttribdvARB = (PFNGLGETVERTEXATTRIBDVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribdvARB"))) == NULL) || r;
  r = ((glGetVertexAttribfvARB = (PFNGLGETVERTEXATTRIBFVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribfvARB"))) == NULL) || r;
  r = ((glGetVertexAttribivARB = (PFNGLGETVERTEXATTRIBIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribivARB"))) == NULL) || r;
  r = ((glIsProgramARB = (PFNGLISPROGRAMARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsProgramARB"))) == NULL) || r;
  r = ((glProgramEnvParameter4dARB = (PFNGLPROGRAMENVPARAMETER4DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameter4dARB"))) == NULL) || r;
  r = ((glProgramEnvParameter4dvARB = (PFNGLPROGRAMENVPARAMETER4DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameter4dvARB"))) == NULL) || r;
  r = ((glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameter4fARB"))) == NULL) || r;
  r = ((glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameter4fvARB"))) == NULL) || r;
  r = ((glProgramLocalParameter4dARB = (PFNGLPROGRAMLOCALPARAMETER4DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameter4dARB"))) == NULL) || r;
  r = ((glProgramLocalParameter4dvARB = (PFNGLPROGRAMLOCALPARAMETER4DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameter4dvARB"))) == NULL) || r;
  r = ((glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameter4fARB"))) == NULL) || r;
  r = ((glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameter4fvARB"))) == NULL) || r;
  r = ((glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramStringARB"))) == NULL) || r;
  r = ((glVertexAttrib1dARB = (PFNGLVERTEXATTRIB1DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1dARB"))) == NULL) || r;
  r = ((glVertexAttrib1dvARB = (PFNGLVERTEXATTRIB1DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1dvARB"))) == NULL) || r;
  r = ((glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1fARB"))) == NULL) || r;
  r = ((glVertexAttrib1fvARB = (PFNGLVERTEXATTRIB1FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1fvARB"))) == NULL) || r;
  r = ((glVertexAttrib1sARB = (PFNGLVERTEXATTRIB1SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1sARB"))) == NULL) || r;
  r = ((glVertexAttrib1svARB = (PFNGLVERTEXATTRIB1SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1svARB"))) == NULL) || r;
  r = ((glVertexAttrib2dARB = (PFNGLVERTEXATTRIB2DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2dARB"))) == NULL) || r;
  r = ((glVertexAttrib2dvARB = (PFNGLVERTEXATTRIB2DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2dvARB"))) == NULL) || r;
  r = ((glVertexAttrib2fARB = (PFNGLVERTEXATTRIB2FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2fARB"))) == NULL) || r;
  r = ((glVertexAttrib2fvARB = (PFNGLVERTEXATTRIB2FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2fvARB"))) == NULL) || r;
  r = ((glVertexAttrib2sARB = (PFNGLVERTEXATTRIB2SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2sARB"))) == NULL) || r;
  r = ((glVertexAttrib2svARB = (PFNGLVERTEXATTRIB2SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2svARB"))) == NULL) || r;
  r = ((glVertexAttrib3dARB = (PFNGLVERTEXATTRIB3DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3dARB"))) == NULL) || r;
  r = ((glVertexAttrib3dvARB = (PFNGLVERTEXATTRIB3DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3dvARB"))) == NULL) || r;
  r = ((glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3fARB"))) == NULL) || r;
  r = ((glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3fvARB"))) == NULL) || r;
  r = ((glVertexAttrib3sARB = (PFNGLVERTEXATTRIB3SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3sARB"))) == NULL) || r;
  r = ((glVertexAttrib3svARB = (PFNGLVERTEXATTRIB3SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3svARB"))) == NULL) || r;
  r = ((glVertexAttrib4NbvARB = (PFNGLVERTEXATTRIB4NBVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4NbvARB"))) == NULL) || r;
  r = ((glVertexAttrib4NivARB = (PFNGLVERTEXATTRIB4NIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4NivARB"))) == NULL) || r;
  r = ((glVertexAttrib4NsvARB = (PFNGLVERTEXATTRIB4NSVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4NsvARB"))) == NULL) || r;
  r = ((glVertexAttrib4NubARB = (PFNGLVERTEXATTRIB4NUBARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4NubARB"))) == NULL) || r;
  r = ((glVertexAttrib4NubvARB = (PFNGLVERTEXATTRIB4NUBVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4NubvARB"))) == NULL) || r;
  r = ((glVertexAttrib4NuivARB = (PFNGLVERTEXATTRIB4NUIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4NuivARB"))) == NULL) || r;
  r = ((glVertexAttrib4NusvARB = (PFNGLVERTEXATTRIB4NUSVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4NusvARB"))) == NULL) || r;
  r = ((glVertexAttrib4bvARB = (PFNGLVERTEXATTRIB4BVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4bvARB"))) == NULL) || r;
  r = ((glVertexAttrib4dARB = (PFNGLVERTEXATTRIB4DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4dARB"))) == NULL) || r;
  r = ((glVertexAttrib4dvARB = (PFNGLVERTEXATTRIB4DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4dvARB"))) == NULL) || r;
  r = ((glVertexAttrib4fARB = (PFNGLVERTEXATTRIB4FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4fARB"))) == NULL) || r;
  r = ((glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4fvARB"))) == NULL) || r;
  r = ((glVertexAttrib4ivARB = (PFNGLVERTEXATTRIB4IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4ivARB"))) == NULL) || r;
  r = ((glVertexAttrib4sARB = (PFNGLVERTEXATTRIB4SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4sARB"))) == NULL) || r;
  r = ((glVertexAttrib4svARB = (PFNGLVERTEXATTRIB4SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4svARB"))) == NULL) || r;
  r = ((glVertexAttrib4ubvARB = (PFNGLVERTEXATTRIB4UBVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4ubvARB"))) == NULL) || r;
  r = ((glVertexAttrib4uivARB = (PFNGLVERTEXATTRIB4UIVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4uivARB"))) == NULL) || r;
  r = ((glVertexAttrib4usvARB = (PFNGLVERTEXATTRIB4USVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4usvARB"))) == NULL) || r;
  r = ((glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribPointerARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_vertex_program */

#ifdef GL_ARB_vertex_shader

static GLboolean _glewInit_GL_ARB_vertex_shader (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindAttribLocationARB = (PFNGLBINDATTRIBLOCATIONARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindAttribLocationARB"))) == NULL) || r;
  r = ((glGetActiveAttribARB = (PFNGLGETACTIVEATTRIBARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveAttribARB"))) == NULL) || r;
  r = ((glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetAttribLocationARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_vertex_shader */

#ifdef GL_ARB_vertex_type_2_10_10_10_rev

static GLboolean _glewInit_GL_ARB_vertex_type_2_10_10_10_rev (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColorP3ui = (PFNGLCOLORP3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorP3ui"))) == NULL) || r;
  r = ((glColorP3uiv = (PFNGLCOLORP3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorP3uiv"))) == NULL) || r;
  r = ((glColorP4ui = (PFNGLCOLORP4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorP4ui"))) == NULL) || r;
  r = ((glColorP4uiv = (PFNGLCOLORP4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorP4uiv"))) == NULL) || r;
  r = ((glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP1ui"))) == NULL) || r;
  r = ((glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP1uiv"))) == NULL) || r;
  r = ((glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP2ui"))) == NULL) || r;
  r = ((glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP2uiv"))) == NULL) || r;
  r = ((glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP3ui"))) == NULL) || r;
  r = ((glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP3uiv"))) == NULL) || r;
  r = ((glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP4ui"))) == NULL) || r;
  r = ((glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordP4uiv"))) == NULL) || r;
  r = ((glNormalP3ui = (PFNGLNORMALP3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalP3ui"))) == NULL) || r;
  r = ((glNormalP3uiv = (PFNGLNORMALP3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalP3uiv"))) == NULL) || r;
  r = ((glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColorP3ui"))) == NULL) || r;
  r = ((glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColorP3uiv"))) == NULL) || r;
  r = ((glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP1ui"))) == NULL) || r;
  r = ((glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP1uiv"))) == NULL) || r;
  r = ((glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP2ui"))) == NULL) || r;
  r = ((glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP2uiv"))) == NULL) || r;
  r = ((glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP3ui"))) == NULL) || r;
  r = ((glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP3uiv"))) == NULL) || r;
  r = ((glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP4ui"))) == NULL) || r;
  r = ((glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordP4uiv"))) == NULL) || r;
  r = ((glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP1ui"))) == NULL) || r;
  r = ((glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP1uiv"))) == NULL) || r;
  r = ((glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP2ui"))) == NULL) || r;
  r = ((glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP2uiv"))) == NULL) || r;
  r = ((glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP3ui"))) == NULL) || r;
  r = ((glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP3uiv"))) == NULL) || r;
  r = ((glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP4ui"))) == NULL) || r;
  r = ((glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribP4uiv"))) == NULL) || r;
  r = ((glVertexP2ui = (PFNGLVERTEXP2UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexP2ui"))) == NULL) || r;
  r = ((glVertexP2uiv = (PFNGLVERTEXP2UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexP2uiv"))) == NULL) || r;
  r = ((glVertexP3ui = (PFNGLVERTEXP3UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexP3ui"))) == NULL) || r;
  r = ((glVertexP3uiv = (PFNGLVERTEXP3UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexP3uiv"))) == NULL) || r;
  r = ((glVertexP4ui = (PFNGLVERTEXP4UIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexP4ui"))) == NULL) || r;
  r = ((glVertexP4uiv = (PFNGLVERTEXP4UIVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexP4uiv"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_vertex_type_2_10_10_10_rev */

#ifdef GL_ARB_viewport_array

static GLboolean _glewInit_GL_ARB_viewport_array (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthRangeArrayv"))) == NULL) || r;
  r = ((glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthRangeIndexed"))) == NULL) || r;
  r = ((glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetDoublei_v"))) == NULL) || r;
  r = ((glGetFloati_v = (PFNGLGETFLOATI_VPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFloati_v"))) == NULL) || r;
  r = ((glScissorArrayv = (PFNGLSCISSORARRAYVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glScissorArrayv"))) == NULL) || r;
  r = ((glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glScissorIndexed"))) == NULL) || r;
  r = ((glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glScissorIndexedv"))) == NULL) || r;
  r = ((glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glViewportArrayv"))) == NULL) || r;
  r = ((glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glViewportIndexedf"))) == NULL) || r;
  r = ((glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glViewportIndexedfv"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_viewport_array */

#ifdef GL_ARB_window_pos

static GLboolean _glewInit_GL_ARB_window_pos (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glWindowPos2dARB = (PFNGLWINDOWPOS2DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2dARB"))) == NULL) || r;
  r = ((glWindowPos2dvARB = (PFNGLWINDOWPOS2DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2dvARB"))) == NULL) || r;
  r = ((glWindowPos2fARB = (PFNGLWINDOWPOS2FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2fARB"))) == NULL) || r;
  r = ((glWindowPos2fvARB = (PFNGLWINDOWPOS2FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2fvARB"))) == NULL) || r;
  r = ((glWindowPos2iARB = (PFNGLWINDOWPOS2IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2iARB"))) == NULL) || r;
  r = ((glWindowPos2ivARB = (PFNGLWINDOWPOS2IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2ivARB"))) == NULL) || r;
  r = ((glWindowPos2sARB = (PFNGLWINDOWPOS2SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2sARB"))) == NULL) || r;
  r = ((glWindowPos2svARB = (PFNGLWINDOWPOS2SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2svARB"))) == NULL) || r;
  r = ((glWindowPos3dARB = (PFNGLWINDOWPOS3DARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3dARB"))) == NULL) || r;
  r = ((glWindowPos3dvARB = (PFNGLWINDOWPOS3DVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3dvARB"))) == NULL) || r;
  r = ((glWindowPos3fARB = (PFNGLWINDOWPOS3FARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3fARB"))) == NULL) || r;
  r = ((glWindowPos3fvARB = (PFNGLWINDOWPOS3FVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3fvARB"))) == NULL) || r;
  r = ((glWindowPos3iARB = (PFNGLWINDOWPOS3IARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3iARB"))) == NULL) || r;
  r = ((glWindowPos3ivARB = (PFNGLWINDOWPOS3IVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3ivARB"))) == NULL) || r;
  r = ((glWindowPos3sARB = (PFNGLWINDOWPOS3SARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3sARB"))) == NULL) || r;
  r = ((glWindowPos3svARB = (PFNGLWINDOWPOS3SVARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3svARB"))) == NULL) || r;

  return r;
}

#endif /* GL_ARB_window_pos */

#ifdef GL_ATIX_point_sprites

#endif /* GL_ATIX_point_sprites */

#ifdef GL_ATIX_texture_env_combine3

#endif /* GL_ATIX_texture_env_combine3 */

#ifdef GL_ATIX_texture_env_route

#endif /* GL_ATIX_texture_env_route */

#ifdef GL_ATIX_vertex_shader_output_point_size

#endif /* GL_ATIX_vertex_shader_output_point_size */

#ifdef GL_ATI_draw_buffers

static GLboolean _glewInit_GL_ATI_draw_buffers (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawBuffersATI = (PFNGLDRAWBUFFERSATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawBuffersATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_draw_buffers */

#ifdef GL_ATI_element_array

static GLboolean _glewInit_GL_ATI_element_array (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawElementArrayATI = (PFNGLDRAWELEMENTARRAYATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementArrayATI"))) == NULL) || r;
  r = ((glDrawRangeElementArrayATI = (PFNGLDRAWRANGEELEMENTARRAYATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawRangeElementArrayATI"))) == NULL) || r;
  r = ((glElementPointerATI = (PFNGLELEMENTPOINTERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glElementPointerATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_element_array */

#ifdef GL_ATI_envmap_bumpmap

static GLboolean _glewInit_GL_ATI_envmap_bumpmap (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetTexBumpParameterfvATI = (PFNGLGETTEXBUMPPARAMETERFVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexBumpParameterfvATI"))) == NULL) || r;
  r = ((glGetTexBumpParameterivATI = (PFNGLGETTEXBUMPPARAMETERIVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexBumpParameterivATI"))) == NULL) || r;
  r = ((glTexBumpParameterfvATI = (PFNGLTEXBUMPPARAMETERFVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexBumpParameterfvATI"))) == NULL) || r;
  r = ((glTexBumpParameterivATI = (PFNGLTEXBUMPPARAMETERIVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexBumpParameterivATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_envmap_bumpmap */

#ifdef GL_ATI_fragment_shader

static GLboolean _glewInit_GL_ATI_fragment_shader (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAlphaFragmentOp1ATI = (PFNGLALPHAFRAGMENTOP1ATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAlphaFragmentOp1ATI"))) == NULL) || r;
  r = ((glAlphaFragmentOp2ATI = (PFNGLALPHAFRAGMENTOP2ATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAlphaFragmentOp2ATI"))) == NULL) || r;
  r = ((glAlphaFragmentOp3ATI = (PFNGLALPHAFRAGMENTOP3ATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAlphaFragmentOp3ATI"))) == NULL) || r;
  r = ((glBeginFragmentShaderATI = (PFNGLBEGINFRAGMENTSHADERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginFragmentShaderATI"))) == NULL) || r;
  r = ((glBindFragmentShaderATI = (PFNGLBINDFRAGMENTSHADERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindFragmentShaderATI"))) == NULL) || r;
  r = ((glColorFragmentOp1ATI = (PFNGLCOLORFRAGMENTOP1ATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorFragmentOp1ATI"))) == NULL) || r;
  r = ((glColorFragmentOp2ATI = (PFNGLCOLORFRAGMENTOP2ATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorFragmentOp2ATI"))) == NULL) || r;
  r = ((glColorFragmentOp3ATI = (PFNGLCOLORFRAGMENTOP3ATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorFragmentOp3ATI"))) == NULL) || r;
  r = ((glDeleteFragmentShaderATI = (PFNGLDELETEFRAGMENTSHADERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteFragmentShaderATI"))) == NULL) || r;
  r = ((glEndFragmentShaderATI = (PFNGLENDFRAGMENTSHADERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndFragmentShaderATI"))) == NULL) || r;
  r = ((glGenFragmentShadersATI = (PFNGLGENFRAGMENTSHADERSATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenFragmentShadersATI"))) == NULL) || r;
  r = ((glPassTexCoordATI = (PFNGLPASSTEXCOORDATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPassTexCoordATI"))) == NULL) || r;
  r = ((glSampleMapATI = (PFNGLSAMPLEMAPATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSampleMapATI"))) == NULL) || r;
  r = ((glSetFragmentShaderConstantATI = (PFNGLSETFRAGMENTSHADERCONSTANTATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSetFragmentShaderConstantATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_fragment_shader */

#ifdef GL_ATI_map_object_buffer

static GLboolean _glewInit_GL_ATI_map_object_buffer (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glMapObjectBufferATI = (PFNGLMAPOBJECTBUFFERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapObjectBufferATI"))) == NULL) || r;
  r = ((glUnmapObjectBufferATI = (PFNGLUNMAPOBJECTBUFFERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUnmapObjectBufferATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_map_object_buffer */

#ifdef GL_ATI_meminfo

#endif /* GL_ATI_meminfo */

#ifdef GL_ATI_pn_triangles

static GLboolean _glewInit_GL_ATI_pn_triangles (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPNTrianglesfATI = (PFNGLPNTRIANGLESFATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPNTrianglesfATI"))) == NULL) || r;
  r = ((glPNTrianglesiATI = (PFNGLPNTRIANGLESIATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPNTrianglesiATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_pn_triangles */

#ifdef GL_ATI_separate_stencil

static GLboolean _glewInit_GL_ATI_separate_stencil (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glStencilFuncSeparateATI = (PFNGLSTENCILFUNCSEPARATEATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilFuncSeparateATI"))) == NULL) || r;
  r = ((glStencilOpSeparateATI = (PFNGLSTENCILOPSEPARATEATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStencilOpSeparateATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_separate_stencil */

#ifdef GL_ATI_shader_texture_lod

#endif /* GL_ATI_shader_texture_lod */

#ifdef GL_ATI_text_fragment_shader

#endif /* GL_ATI_text_fragment_shader */

#ifdef GL_ATI_texture_compression_3dc

#endif /* GL_ATI_texture_compression_3dc */

#ifdef GL_ATI_texture_env_combine3

#endif /* GL_ATI_texture_env_combine3 */

#ifdef GL_ATI_texture_float

#endif /* GL_ATI_texture_float */

#ifdef GL_ATI_texture_mirror_once

#endif /* GL_ATI_texture_mirror_once */

#ifdef GL_ATI_vertex_array_object

static GLboolean _glewInit_GL_ATI_vertex_array_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glArrayObjectATI = (PFNGLARRAYOBJECTATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glArrayObjectATI"))) == NULL) || r;
  r = ((glFreeObjectBufferATI = (PFNGLFREEOBJECTBUFFERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFreeObjectBufferATI"))) == NULL) || r;
  r = ((glGetArrayObjectfvATI = (PFNGLGETARRAYOBJECTFVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetArrayObjectfvATI"))) == NULL) || r;
  r = ((glGetArrayObjectivATI = (PFNGLGETARRAYOBJECTIVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetArrayObjectivATI"))) == NULL) || r;
  r = ((glGetObjectBufferfvATI = (PFNGLGETOBJECTBUFFERFVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetObjectBufferfvATI"))) == NULL) || r;
  r = ((glGetObjectBufferivATI = (PFNGLGETOBJECTBUFFERIVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetObjectBufferivATI"))) == NULL) || r;
  r = ((glGetVariantArrayObjectfvATI = (PFNGLGETVARIANTARRAYOBJECTFVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVariantArrayObjectfvATI"))) == NULL) || r;
  r = ((glGetVariantArrayObjectivATI = (PFNGLGETVARIANTARRAYOBJECTIVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVariantArrayObjectivATI"))) == NULL) || r;
  r = ((glIsObjectBufferATI = (PFNGLISOBJECTBUFFERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsObjectBufferATI"))) == NULL) || r;
  r = ((glNewObjectBufferATI = (PFNGLNEWOBJECTBUFFERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNewObjectBufferATI"))) == NULL) || r;
  r = ((glUpdateObjectBufferATI = (PFNGLUPDATEOBJECTBUFFERATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUpdateObjectBufferATI"))) == NULL) || r;
  r = ((glVariantArrayObjectATI = (PFNGLVARIANTARRAYOBJECTATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantArrayObjectATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_vertex_array_object */

#ifdef GL_ATI_vertex_attrib_array_object

static GLboolean _glewInit_GL_ATI_vertex_attrib_array_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetVertexAttribArrayObjectfvATI = (PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribArrayObjectfvATI"))) == NULL) || r;
  r = ((glGetVertexAttribArrayObjectivATI = (PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribArrayObjectivATI"))) == NULL) || r;
  r = ((glVertexAttribArrayObjectATI = (PFNGLVERTEXATTRIBARRAYOBJECTATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribArrayObjectATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_vertex_attrib_array_object */

#ifdef GL_ATI_vertex_streams

static GLboolean _glewInit_GL_ATI_vertex_streams (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glClientActiveVertexStreamATI = (PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClientActiveVertexStreamATI"))) == NULL) || r;
  r = ((glNormalStream3bATI = (PFNGLNORMALSTREAM3BATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3bATI"))) == NULL) || r;
  r = ((glNormalStream3bvATI = (PFNGLNORMALSTREAM3BVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3bvATI"))) == NULL) || r;
  r = ((glNormalStream3dATI = (PFNGLNORMALSTREAM3DATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3dATI"))) == NULL) || r;
  r = ((glNormalStream3dvATI = (PFNGLNORMALSTREAM3DVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3dvATI"))) == NULL) || r;
  r = ((glNormalStream3fATI = (PFNGLNORMALSTREAM3FATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3fATI"))) == NULL) || r;
  r = ((glNormalStream3fvATI = (PFNGLNORMALSTREAM3FVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3fvATI"))) == NULL) || r;
  r = ((glNormalStream3iATI = (PFNGLNORMALSTREAM3IATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3iATI"))) == NULL) || r;
  r = ((glNormalStream3ivATI = (PFNGLNORMALSTREAM3IVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3ivATI"))) == NULL) || r;
  r = ((glNormalStream3sATI = (PFNGLNORMALSTREAM3SATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3sATI"))) == NULL) || r;
  r = ((glNormalStream3svATI = (PFNGLNORMALSTREAM3SVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalStream3svATI"))) == NULL) || r;
  r = ((glVertexBlendEnvfATI = (PFNGLVERTEXBLENDENVFATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexBlendEnvfATI"))) == NULL) || r;
  r = ((glVertexBlendEnviATI = (PFNGLVERTEXBLENDENVIATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexBlendEnviATI"))) == NULL) || r;
  r = ((glVertexStream2dATI = (PFNGLVERTEXSTREAM2DATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2dATI"))) == NULL) || r;
  r = ((glVertexStream2dvATI = (PFNGLVERTEXSTREAM2DVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2dvATI"))) == NULL) || r;
  r = ((glVertexStream2fATI = (PFNGLVERTEXSTREAM2FATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2fATI"))) == NULL) || r;
  r = ((glVertexStream2fvATI = (PFNGLVERTEXSTREAM2FVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2fvATI"))) == NULL) || r;
  r = ((glVertexStream2iATI = (PFNGLVERTEXSTREAM2IATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2iATI"))) == NULL) || r;
  r = ((glVertexStream2ivATI = (PFNGLVERTEXSTREAM2IVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2ivATI"))) == NULL) || r;
  r = ((glVertexStream2sATI = (PFNGLVERTEXSTREAM2SATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2sATI"))) == NULL) || r;
  r = ((glVertexStream2svATI = (PFNGLVERTEXSTREAM2SVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream2svATI"))) == NULL) || r;
  r = ((glVertexStream3dATI = (PFNGLVERTEXSTREAM3DATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3dATI"))) == NULL) || r;
  r = ((glVertexStream3dvATI = (PFNGLVERTEXSTREAM3DVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3dvATI"))) == NULL) || r;
  r = ((glVertexStream3fATI = (PFNGLVERTEXSTREAM3FATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3fATI"))) == NULL) || r;
  r = ((glVertexStream3fvATI = (PFNGLVERTEXSTREAM3FVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3fvATI"))) == NULL) || r;
  r = ((glVertexStream3iATI = (PFNGLVERTEXSTREAM3IATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3iATI"))) == NULL) || r;
  r = ((glVertexStream3ivATI = (PFNGLVERTEXSTREAM3IVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3ivATI"))) == NULL) || r;
  r = ((glVertexStream3sATI = (PFNGLVERTEXSTREAM3SATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3sATI"))) == NULL) || r;
  r = ((glVertexStream3svATI = (PFNGLVERTEXSTREAM3SVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream3svATI"))) == NULL) || r;
  r = ((glVertexStream4dATI = (PFNGLVERTEXSTREAM4DATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4dATI"))) == NULL) || r;
  r = ((glVertexStream4dvATI = (PFNGLVERTEXSTREAM4DVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4dvATI"))) == NULL) || r;
  r = ((glVertexStream4fATI = (PFNGLVERTEXSTREAM4FATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4fATI"))) == NULL) || r;
  r = ((glVertexStream4fvATI = (PFNGLVERTEXSTREAM4FVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4fvATI"))) == NULL) || r;
  r = ((glVertexStream4iATI = (PFNGLVERTEXSTREAM4IATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4iATI"))) == NULL) || r;
  r = ((glVertexStream4ivATI = (PFNGLVERTEXSTREAM4IVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4ivATI"))) == NULL) || r;
  r = ((glVertexStream4sATI = (PFNGLVERTEXSTREAM4SATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4sATI"))) == NULL) || r;
  r = ((glVertexStream4svATI = (PFNGLVERTEXSTREAM4SVATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexStream4svATI"))) == NULL) || r;

  return r;
}

#endif /* GL_ATI_vertex_streams */

#ifdef GL_EXT_422_pixels

#endif /* GL_EXT_422_pixels */

#ifdef GL_EXT_Cg_shader

#endif /* GL_EXT_Cg_shader */

#ifdef GL_EXT_abgr

#endif /* GL_EXT_abgr */

#ifdef GL_EXT_bgra

#endif /* GL_EXT_bgra */

#ifdef GL_EXT_bindable_uniform

static GLboolean _glewInit_GL_EXT_bindable_uniform (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetUniformBufferSizeEXT = (PFNGLGETUNIFORMBUFFERSIZEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformBufferSizeEXT"))) == NULL) || r;
  r = ((glGetUniformOffsetEXT = (PFNGLGETUNIFORMOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformOffsetEXT"))) == NULL) || r;
  r = ((glUniformBufferEXT = (PFNGLUNIFORMBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformBufferEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_bindable_uniform */

#ifdef GL_EXT_blend_color

static GLboolean _glewInit_GL_EXT_blend_color (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendColorEXT = (PFNGLBLENDCOLOREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendColorEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_blend_color */

#ifdef GL_EXT_blend_equation_separate

static GLboolean _glewInit_GL_EXT_blend_equation_separate (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendEquationSeparateEXT = (PFNGLBLENDEQUATIONSEPARATEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationSeparateEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_blend_equation_separate */

#ifdef GL_EXT_blend_func_separate

static GLboolean _glewInit_GL_EXT_blend_func_separate (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendFuncSeparateEXT = (PFNGLBLENDFUNCSEPARATEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendFuncSeparateEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_blend_func_separate */

#ifdef GL_EXT_blend_logic_op

#endif /* GL_EXT_blend_logic_op */

#ifdef GL_EXT_blend_minmax

static GLboolean _glewInit_GL_EXT_blend_minmax (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlendEquationEXT = (PFNGLBLENDEQUATIONEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlendEquationEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_blend_minmax */

#ifdef GL_EXT_blend_subtract

#endif /* GL_EXT_blend_subtract */

#ifdef GL_EXT_clip_volume_hint

#endif /* GL_EXT_clip_volume_hint */

#ifdef GL_EXT_cmyka

#endif /* GL_EXT_cmyka */

#ifdef GL_EXT_color_subtable

static GLboolean _glewInit_GL_EXT_color_subtable (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColorSubTableEXT = (PFNGLCOLORSUBTABLEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorSubTableEXT"))) == NULL) || r;
  r = ((glCopyColorSubTableEXT = (PFNGLCOPYCOLORSUBTABLEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyColorSubTableEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_color_subtable */

#ifdef GL_EXT_compiled_vertex_array

static GLboolean _glewInit_GL_EXT_compiled_vertex_array (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glLockArraysEXT = (PFNGLLOCKARRAYSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLockArraysEXT"))) == NULL) || r;
  r = ((glUnlockArraysEXT = (PFNGLUNLOCKARRAYSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUnlockArraysEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_compiled_vertex_array */

#ifdef GL_EXT_convolution

static GLboolean _glewInit_GL_EXT_convolution (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glConvolutionFilter1DEXT = (PFNGLCONVOLUTIONFILTER1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionFilter1DEXT"))) == NULL) || r;
  r = ((glConvolutionFilter2DEXT = (PFNGLCONVOLUTIONFILTER2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionFilter2DEXT"))) == NULL) || r;
  r = ((glConvolutionParameterfEXT = (PFNGLCONVOLUTIONPARAMETERFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameterfEXT"))) == NULL) || r;
  r = ((glConvolutionParameterfvEXT = (PFNGLCONVOLUTIONPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameterfvEXT"))) == NULL) || r;
  r = ((glConvolutionParameteriEXT = (PFNGLCONVOLUTIONPARAMETERIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameteriEXT"))) == NULL) || r;
  r = ((glConvolutionParameterivEXT = (PFNGLCONVOLUTIONPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glConvolutionParameterivEXT"))) == NULL) || r;
  r = ((glCopyConvolutionFilter1DEXT = (PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyConvolutionFilter1DEXT"))) == NULL) || r;
  r = ((glCopyConvolutionFilter2DEXT = (PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyConvolutionFilter2DEXT"))) == NULL) || r;
  r = ((glGetConvolutionFilterEXT = (PFNGLGETCONVOLUTIONFILTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetConvolutionFilterEXT"))) == NULL) || r;
  r = ((glGetConvolutionParameterfvEXT = (PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetConvolutionParameterfvEXT"))) == NULL) || r;
  r = ((glGetConvolutionParameterivEXT = (PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetConvolutionParameterivEXT"))) == NULL) || r;
  r = ((glGetSeparableFilterEXT = (PFNGLGETSEPARABLEFILTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSeparableFilterEXT"))) == NULL) || r;
  r = ((glSeparableFilter2DEXT = (PFNGLSEPARABLEFILTER2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSeparableFilter2DEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_convolution */

#ifdef GL_EXT_coordinate_frame

static GLboolean _glewInit_GL_EXT_coordinate_frame (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBinormalPointerEXT = (PFNGLBINORMALPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBinormalPointerEXT"))) == NULL) || r;
  r = ((glTangentPointerEXT = (PFNGLTANGENTPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTangentPointerEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_coordinate_frame */

#ifdef GL_EXT_copy_texture

static GLboolean _glewInit_GL_EXT_copy_texture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCopyTexImage1DEXT = (PFNGLCOPYTEXIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexImage1DEXT"))) == NULL) || r;
  r = ((glCopyTexImage2DEXT = (PFNGLCOPYTEXIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexImage2DEXT"))) == NULL) || r;
  r = ((glCopyTexSubImage1DEXT = (PFNGLCOPYTEXSUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexSubImage1DEXT"))) == NULL) || r;
  r = ((glCopyTexSubImage2DEXT = (PFNGLCOPYTEXSUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexSubImage2DEXT"))) == NULL) || r;
  r = ((glCopyTexSubImage3DEXT = (PFNGLCOPYTEXSUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTexSubImage3DEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_copy_texture */

#ifdef GL_EXT_cull_vertex

static GLboolean _glewInit_GL_EXT_cull_vertex (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCullParameterdvEXT = (PFNGLCULLPARAMETERDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCullParameterdvEXT"))) == NULL) || r;
  r = ((glCullParameterfvEXT = (PFNGLCULLPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCullParameterfvEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_cull_vertex */

#ifdef GL_EXT_depth_bounds_test

static GLboolean _glewInit_GL_EXT_depth_bounds_test (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDepthBoundsEXT = (PFNGLDEPTHBOUNDSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthBoundsEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_depth_bounds_test */

#ifdef GL_EXT_direct_state_access

static GLboolean _glewInit_GL_EXT_direct_state_access (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindMultiTextureEXT = (PFNGLBINDMULTITEXTUREEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindMultiTextureEXT"))) == NULL) || r;
  r = ((glCheckNamedFramebufferStatusEXT = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCheckNamedFramebufferStatusEXT"))) == NULL) || r;
  r = ((glClientAttribDefaultEXT = (PFNGLCLIENTATTRIBDEFAULTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClientAttribDefaultEXT"))) == NULL) || r;
  r = ((glCompressedMultiTexImage1DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedMultiTexImage1DEXT"))) == NULL) || r;
  r = ((glCompressedMultiTexImage2DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedMultiTexImage2DEXT"))) == NULL) || r;
  r = ((glCompressedMultiTexImage3DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedMultiTexImage3DEXT"))) == NULL) || r;
  r = ((glCompressedMultiTexSubImage1DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedMultiTexSubImage1DEXT"))) == NULL) || r;
  r = ((glCompressedMultiTexSubImage2DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedMultiTexSubImage2DEXT"))) == NULL) || r;
  r = ((glCompressedMultiTexSubImage3DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedMultiTexSubImage3DEXT"))) == NULL) || r;
  r = ((glCompressedTextureImage1DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTextureImage1DEXT"))) == NULL) || r;
  r = ((glCompressedTextureImage2DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTextureImage2DEXT"))) == NULL) || r;
  r = ((glCompressedTextureImage3DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTextureImage3DEXT"))) == NULL) || r;
  r = ((glCompressedTextureSubImage1DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTextureSubImage1DEXT"))) == NULL) || r;
  r = ((glCompressedTextureSubImage2DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTextureSubImage2DEXT"))) == NULL) || r;
  r = ((glCompressedTextureSubImage3DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCompressedTextureSubImage3DEXT"))) == NULL) || r;
  r = ((glCopyMultiTexImage1DEXT = (PFNGLCOPYMULTITEXIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyMultiTexImage1DEXT"))) == NULL) || r;
  r = ((glCopyMultiTexImage2DEXT = (PFNGLCOPYMULTITEXIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyMultiTexImage2DEXT"))) == NULL) || r;
  r = ((glCopyMultiTexSubImage1DEXT = (PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyMultiTexSubImage1DEXT"))) == NULL) || r;
  r = ((glCopyMultiTexSubImage2DEXT = (PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyMultiTexSubImage2DEXT"))) == NULL) || r;
  r = ((glCopyMultiTexSubImage3DEXT = (PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyMultiTexSubImage3DEXT"))) == NULL) || r;
  r = ((glCopyTextureImage1DEXT = (PFNGLCOPYTEXTUREIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTextureImage1DEXT"))) == NULL) || r;
  r = ((glCopyTextureImage2DEXT = (PFNGLCOPYTEXTUREIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTextureImage2DEXT"))) == NULL) || r;
  r = ((glCopyTextureSubImage1DEXT = (PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTextureSubImage1DEXT"))) == NULL) || r;
  r = ((glCopyTextureSubImage2DEXT = (PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTextureSubImage2DEXT"))) == NULL) || r;
  r = ((glCopyTextureSubImage3DEXT = (PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyTextureSubImage3DEXT"))) == NULL) || r;
  r = ((glDisableClientStateIndexedEXT = (PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableClientStateIndexedEXT"))) == NULL) || r;
  r = ((glDisableClientStateiEXT = (PFNGLDISABLECLIENTSTATEIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableClientStateiEXT"))) == NULL) || r;
  r = ((glDisableVertexArrayAttribEXT = (PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableVertexArrayAttribEXT"))) == NULL) || r;
  r = ((glDisableVertexArrayEXT = (PFNGLDISABLEVERTEXARRAYEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableVertexArrayEXT"))) == NULL) || r;
  r = ((glEnableClientStateIndexedEXT = (PFNGLENABLECLIENTSTATEINDEXEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableClientStateIndexedEXT"))) == NULL) || r;
  r = ((glEnableClientStateiEXT = (PFNGLENABLECLIENTSTATEIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableClientStateiEXT"))) == NULL) || r;
  r = ((glEnableVertexArrayAttribEXT = (PFNGLENABLEVERTEXARRAYATTRIBEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableVertexArrayAttribEXT"))) == NULL) || r;
  r = ((glEnableVertexArrayEXT = (PFNGLENABLEVERTEXARRAYEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableVertexArrayEXT"))) == NULL) || r;
  r = ((glFlushMappedNamedBufferRangeEXT = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlushMappedNamedBufferRangeEXT"))) == NULL) || r;
  r = ((glFramebufferDrawBufferEXT = (PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferDrawBufferEXT"))) == NULL) || r;
  r = ((glFramebufferDrawBuffersEXT = (PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferDrawBuffersEXT"))) == NULL) || r;
  r = ((glFramebufferReadBufferEXT = (PFNGLFRAMEBUFFERREADBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferReadBufferEXT"))) == NULL) || r;
  r = ((glGenerateMultiTexMipmapEXT = (PFNGLGENERATEMULTITEXMIPMAPEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenerateMultiTexMipmapEXT"))) == NULL) || r;
  r = ((glGenerateTextureMipmapEXT = (PFNGLGENERATETEXTUREMIPMAPEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenerateTextureMipmapEXT"))) == NULL) || r;
  r = ((glGetCompressedMultiTexImageEXT = (PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCompressedMultiTexImageEXT"))) == NULL) || r;
  r = ((glGetCompressedTextureImageEXT = (PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCompressedTextureImageEXT"))) == NULL) || r;
  r = ((glGetDoubleIndexedvEXT = (PFNGLGETDOUBLEINDEXEDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetDoubleIndexedvEXT"))) == NULL) || r;
  r = ((glGetDoublei_vEXT = (PFNGLGETDOUBLEI_VEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetDoublei_vEXT"))) == NULL) || r;
  r = ((glGetFloatIndexedvEXT = (PFNGLGETFLOATINDEXEDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFloatIndexedvEXT"))) == NULL) || r;
  r = ((glGetFloati_vEXT = (PFNGLGETFLOATI_VEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFloati_vEXT"))) == NULL) || r;
  r = ((glGetFramebufferParameterivEXT = (PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFramebufferParameterivEXT"))) == NULL) || r;
  r = ((glGetMultiTexEnvfvEXT = (PFNGLGETMULTITEXENVFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexEnvfvEXT"))) == NULL) || r;
  r = ((glGetMultiTexEnvivEXT = (PFNGLGETMULTITEXENVIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexEnvivEXT"))) == NULL) || r;
  r = ((glGetMultiTexGendvEXT = (PFNGLGETMULTITEXGENDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexGendvEXT"))) == NULL) || r;
  r = ((glGetMultiTexGenfvEXT = (PFNGLGETMULTITEXGENFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexGenfvEXT"))) == NULL) || r;
  r = ((glGetMultiTexGenivEXT = (PFNGLGETMULTITEXGENIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexGenivEXT"))) == NULL) || r;
  r = ((glGetMultiTexImageEXT = (PFNGLGETMULTITEXIMAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexImageEXT"))) == NULL) || r;
  r = ((glGetMultiTexLevelParameterfvEXT = (PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexLevelParameterfvEXT"))) == NULL) || r;
  r = ((glGetMultiTexLevelParameterivEXT = (PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexLevelParameterivEXT"))) == NULL) || r;
  r = ((glGetMultiTexParameterIivEXT = (PFNGLGETMULTITEXPARAMETERIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexParameterIivEXT"))) == NULL) || r;
  r = ((glGetMultiTexParameterIuivEXT = (PFNGLGETMULTITEXPARAMETERIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexParameterIuivEXT"))) == NULL) || r;
  r = ((glGetMultiTexParameterfvEXT = (PFNGLGETMULTITEXPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexParameterfvEXT"))) == NULL) || r;
  r = ((glGetMultiTexParameterivEXT = (PFNGLGETMULTITEXPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultiTexParameterivEXT"))) == NULL) || r;
  r = ((glGetNamedBufferParameterivEXT = (PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedBufferParameterivEXT"))) == NULL) || r;
  r = ((glGetNamedBufferPointervEXT = (PFNGLGETNAMEDBUFFERPOINTERVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedBufferPointervEXT"))) == NULL) || r;
  r = ((glGetNamedBufferSubDataEXT = (PFNGLGETNAMEDBUFFERSUBDATAEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedBufferSubDataEXT"))) == NULL) || r;
  r = ((glGetNamedFramebufferAttachmentParameterivEXT = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedFramebufferAttachmentParameterivEXT"))) == NULL) || r;
  r = ((glGetNamedProgramLocalParameterIivEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedProgramLocalParameterIivEXT"))) == NULL) || r;
  r = ((glGetNamedProgramLocalParameterIuivEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedProgramLocalParameterIuivEXT"))) == NULL) || r;
  r = ((glGetNamedProgramLocalParameterdvEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedProgramLocalParameterdvEXT"))) == NULL) || r;
  r = ((glGetNamedProgramLocalParameterfvEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedProgramLocalParameterfvEXT"))) == NULL) || r;
  r = ((glGetNamedProgramStringEXT = (PFNGLGETNAMEDPROGRAMSTRINGEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedProgramStringEXT"))) == NULL) || r;
  r = ((glGetNamedProgramivEXT = (PFNGLGETNAMEDPROGRAMIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedProgramivEXT"))) == NULL) || r;
  r = ((glGetNamedRenderbufferParameterivEXT = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedRenderbufferParameterivEXT"))) == NULL) || r;
  r = ((glGetPointerIndexedvEXT = (PFNGLGETPOINTERINDEXEDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPointerIndexedvEXT"))) == NULL) || r;
  r = ((glGetPointeri_vEXT = (PFNGLGETPOINTERI_VEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPointeri_vEXT"))) == NULL) || r;
  r = ((glGetTextureImageEXT = (PFNGLGETTEXTUREIMAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTextureImageEXT"))) == NULL) || r;
  r = ((glGetTextureLevelParameterfvEXT = (PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTextureLevelParameterfvEXT"))) == NULL) || r;
  r = ((glGetTextureLevelParameterivEXT = (PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTextureLevelParameterivEXT"))) == NULL) || r;
  r = ((glGetTextureParameterIivEXT = (PFNGLGETTEXTUREPARAMETERIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTextureParameterIivEXT"))) == NULL) || r;
  r = ((glGetTextureParameterIuivEXT = (PFNGLGETTEXTUREPARAMETERIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTextureParameterIuivEXT"))) == NULL) || r;
  r = ((glGetTextureParameterfvEXT = (PFNGLGETTEXTUREPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTextureParameterfvEXT"))) == NULL) || r;
  r = ((glGetTextureParameterivEXT = (PFNGLGETTEXTUREPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTextureParameterivEXT"))) == NULL) || r;
  r = ((glGetVertexArrayIntegeri_vEXT = (PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexArrayIntegeri_vEXT"))) == NULL) || r;
  r = ((glGetVertexArrayIntegervEXT = (PFNGLGETVERTEXARRAYINTEGERVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexArrayIntegervEXT"))) == NULL) || r;
  r = ((glGetVertexArrayPointeri_vEXT = (PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexArrayPointeri_vEXT"))) == NULL) || r;
  r = ((glGetVertexArrayPointervEXT = (PFNGLGETVERTEXARRAYPOINTERVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexArrayPointervEXT"))) == NULL) || r;
  r = ((glMapNamedBufferEXT = (PFNGLMAPNAMEDBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapNamedBufferEXT"))) == NULL) || r;
  r = ((glMapNamedBufferRangeEXT = (PFNGLMAPNAMEDBUFFERRANGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapNamedBufferRangeEXT"))) == NULL) || r;
  r = ((glMatrixFrustumEXT = (PFNGLMATRIXFRUSTUMEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixFrustumEXT"))) == NULL) || r;
  r = ((glMatrixLoadIdentityEXT = (PFNGLMATRIXLOADIDENTITYEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixLoadIdentityEXT"))) == NULL) || r;
  r = ((glMatrixLoadTransposedEXT = (PFNGLMATRIXLOADTRANSPOSEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixLoadTransposedEXT"))) == NULL) || r;
  r = ((glMatrixLoadTransposefEXT = (PFNGLMATRIXLOADTRANSPOSEFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixLoadTransposefEXT"))) == NULL) || r;
  r = ((glMatrixLoaddEXT = (PFNGLMATRIXLOADDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixLoaddEXT"))) == NULL) || r;
  r = ((glMatrixLoadfEXT = (PFNGLMATRIXLOADFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixLoadfEXT"))) == NULL) || r;
  r = ((glMatrixMultTransposedEXT = (PFNGLMATRIXMULTTRANSPOSEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixMultTransposedEXT"))) == NULL) || r;
  r = ((glMatrixMultTransposefEXT = (PFNGLMATRIXMULTTRANSPOSEFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixMultTransposefEXT"))) == NULL) || r;
  r = ((glMatrixMultdEXT = (PFNGLMATRIXMULTDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixMultdEXT"))) == NULL) || r;
  r = ((glMatrixMultfEXT = (PFNGLMATRIXMULTFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixMultfEXT"))) == NULL) || r;
  r = ((glMatrixOrthoEXT = (PFNGLMATRIXORTHOEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixOrthoEXT"))) == NULL) || r;
  r = ((glMatrixPopEXT = (PFNGLMATRIXPOPEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixPopEXT"))) == NULL) || r;
  r = ((glMatrixPushEXT = (PFNGLMATRIXPUSHEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixPushEXT"))) == NULL) || r;
  r = ((glMatrixRotatedEXT = (PFNGLMATRIXROTATEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixRotatedEXT"))) == NULL) || r;
  r = ((glMatrixRotatefEXT = (PFNGLMATRIXROTATEFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixRotatefEXT"))) == NULL) || r;
  r = ((glMatrixScaledEXT = (PFNGLMATRIXSCALEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixScaledEXT"))) == NULL) || r;
  r = ((glMatrixScalefEXT = (PFNGLMATRIXSCALEFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixScalefEXT"))) == NULL) || r;
  r = ((glMatrixTranslatedEXT = (PFNGLMATRIXTRANSLATEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixTranslatedEXT"))) == NULL) || r;
  r = ((glMatrixTranslatefEXT = (PFNGLMATRIXTRANSLATEFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMatrixTranslatefEXT"))) == NULL) || r;
  r = ((glMultiTexBufferEXT = (PFNGLMULTITEXBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexBufferEXT"))) == NULL) || r;
  r = ((glMultiTexCoordPointerEXT = (PFNGLMULTITEXCOORDPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoordPointerEXT"))) == NULL) || r;
  r = ((glMultiTexEnvfEXT = (PFNGLMULTITEXENVFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexEnvfEXT"))) == NULL) || r;
  r = ((glMultiTexEnvfvEXT = (PFNGLMULTITEXENVFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexEnvfvEXT"))) == NULL) || r;
  r = ((glMultiTexEnviEXT = (PFNGLMULTITEXENVIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexEnviEXT"))) == NULL) || r;
  r = ((glMultiTexEnvivEXT = (PFNGLMULTITEXENVIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexEnvivEXT"))) == NULL) || r;
  r = ((glMultiTexGendEXT = (PFNGLMULTITEXGENDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexGendEXT"))) == NULL) || r;
  r = ((glMultiTexGendvEXT = (PFNGLMULTITEXGENDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexGendvEXT"))) == NULL) || r;
  r = ((glMultiTexGenfEXT = (PFNGLMULTITEXGENFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexGenfEXT"))) == NULL) || r;
  r = ((glMultiTexGenfvEXT = (PFNGLMULTITEXGENFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexGenfvEXT"))) == NULL) || r;
  r = ((glMultiTexGeniEXT = (PFNGLMULTITEXGENIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexGeniEXT"))) == NULL) || r;
  r = ((glMultiTexGenivEXT = (PFNGLMULTITEXGENIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexGenivEXT"))) == NULL) || r;
  r = ((glMultiTexImage1DEXT = (PFNGLMULTITEXIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexImage1DEXT"))) == NULL) || r;
  r = ((glMultiTexImage2DEXT = (PFNGLMULTITEXIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexImage2DEXT"))) == NULL) || r;
  r = ((glMultiTexImage3DEXT = (PFNGLMULTITEXIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexImage3DEXT"))) == NULL) || r;
  r = ((glMultiTexParameterIivEXT = (PFNGLMULTITEXPARAMETERIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexParameterIivEXT"))) == NULL) || r;
  r = ((glMultiTexParameterIuivEXT = (PFNGLMULTITEXPARAMETERIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexParameterIuivEXT"))) == NULL) || r;
  r = ((glMultiTexParameterfEXT = (PFNGLMULTITEXPARAMETERFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexParameterfEXT"))) == NULL) || r;
  r = ((glMultiTexParameterfvEXT = (PFNGLMULTITEXPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexParameterfvEXT"))) == NULL) || r;
  r = ((glMultiTexParameteriEXT = (PFNGLMULTITEXPARAMETERIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexParameteriEXT"))) == NULL) || r;
  r = ((glMultiTexParameterivEXT = (PFNGLMULTITEXPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexParameterivEXT"))) == NULL) || r;
  r = ((glMultiTexRenderbufferEXT = (PFNGLMULTITEXRENDERBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexRenderbufferEXT"))) == NULL) || r;
  r = ((glMultiTexSubImage1DEXT = (PFNGLMULTITEXSUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexSubImage1DEXT"))) == NULL) || r;
  r = ((glMultiTexSubImage2DEXT = (PFNGLMULTITEXSUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexSubImage2DEXT"))) == NULL) || r;
  r = ((glMultiTexSubImage3DEXT = (PFNGLMULTITEXSUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexSubImage3DEXT"))) == NULL) || r;
  r = ((glNamedBufferDataEXT = (PFNGLNAMEDBUFFERDATAEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedBufferDataEXT"))) == NULL) || r;
  r = ((glNamedBufferSubDataEXT = (PFNGLNAMEDBUFFERSUBDATAEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedBufferSubDataEXT"))) == NULL) || r;
  r = ((glNamedCopyBufferSubDataEXT = (PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedCopyBufferSubDataEXT"))) == NULL) || r;
  r = ((glNamedFramebufferRenderbufferEXT = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedFramebufferRenderbufferEXT"))) == NULL) || r;
  r = ((glNamedFramebufferTexture1DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedFramebufferTexture1DEXT"))) == NULL) || r;
  r = ((glNamedFramebufferTexture2DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedFramebufferTexture2DEXT"))) == NULL) || r;
  r = ((glNamedFramebufferTexture3DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedFramebufferTexture3DEXT"))) == NULL) || r;
  r = ((glNamedFramebufferTextureEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedFramebufferTextureEXT"))) == NULL) || r;
  r = ((glNamedFramebufferTextureFaceEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedFramebufferTextureFaceEXT"))) == NULL) || r;
  r = ((glNamedFramebufferTextureLayerEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedFramebufferTextureLayerEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameter4dEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameter4dEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameter4dvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameter4dvEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameter4fEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameter4fEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameter4fvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameter4fvEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameterI4iEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameterI4iEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameterI4ivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameterI4ivEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameterI4uiEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameterI4uiEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameterI4uivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameterI4uivEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParameters4fvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParameters4fvEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParametersI4ivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParametersI4ivEXT"))) == NULL) || r;
  r = ((glNamedProgramLocalParametersI4uivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramLocalParametersI4uivEXT"))) == NULL) || r;
  r = ((glNamedProgramStringEXT = (PFNGLNAMEDPROGRAMSTRINGEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedProgramStringEXT"))) == NULL) || r;
  r = ((glNamedRenderbufferStorageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedRenderbufferStorageEXT"))) == NULL) || r;
  r = ((glNamedRenderbufferStorageMultisampleCoverageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedRenderbufferStorageMultisampleCoverageEXT"))) == NULL) || r;
  r = ((glNamedRenderbufferStorageMultisampleEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNamedRenderbufferStorageMultisampleEXT"))) == NULL) || r;
  r = ((glProgramUniform1fEXT = (PFNGLPROGRAMUNIFORM1FEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1fEXT"))) == NULL) || r;
  r = ((glProgramUniform1fvEXT = (PFNGLPROGRAMUNIFORM1FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1fvEXT"))) == NULL) || r;
  r = ((glProgramUniform1iEXT = (PFNGLPROGRAMUNIFORM1IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1iEXT"))) == NULL) || r;
  r = ((glProgramUniform1ivEXT = (PFNGLPROGRAMUNIFORM1IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1ivEXT"))) == NULL) || r;
  r = ((glProgramUniform1uiEXT = (PFNGLPROGRAMUNIFORM1UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1uiEXT"))) == NULL) || r;
  r = ((glProgramUniform1uivEXT = (PFNGLPROGRAMUNIFORM1UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1uivEXT"))) == NULL) || r;
  r = ((glProgramUniform2fEXT = (PFNGLPROGRAMUNIFORM2FEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2fEXT"))) == NULL) || r;
  r = ((glProgramUniform2fvEXT = (PFNGLPROGRAMUNIFORM2FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2fvEXT"))) == NULL) || r;
  r = ((glProgramUniform2iEXT = (PFNGLPROGRAMUNIFORM2IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2iEXT"))) == NULL) || r;
  r = ((glProgramUniform2ivEXT = (PFNGLPROGRAMUNIFORM2IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2ivEXT"))) == NULL) || r;
  r = ((glProgramUniform2uiEXT = (PFNGLPROGRAMUNIFORM2UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2uiEXT"))) == NULL) || r;
  r = ((glProgramUniform2uivEXT = (PFNGLPROGRAMUNIFORM2UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2uivEXT"))) == NULL) || r;
  r = ((glProgramUniform3fEXT = (PFNGLPROGRAMUNIFORM3FEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3fEXT"))) == NULL) || r;
  r = ((glProgramUniform3fvEXT = (PFNGLPROGRAMUNIFORM3FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3fvEXT"))) == NULL) || r;
  r = ((glProgramUniform3iEXT = (PFNGLPROGRAMUNIFORM3IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3iEXT"))) == NULL) || r;
  r = ((glProgramUniform3ivEXT = (PFNGLPROGRAMUNIFORM3IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3ivEXT"))) == NULL) || r;
  r = ((glProgramUniform3uiEXT = (PFNGLPROGRAMUNIFORM3UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3uiEXT"))) == NULL) || r;
  r = ((glProgramUniform3uivEXT = (PFNGLPROGRAMUNIFORM3UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3uivEXT"))) == NULL) || r;
  r = ((glProgramUniform4fEXT = (PFNGLPROGRAMUNIFORM4FEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4fEXT"))) == NULL) || r;
  r = ((glProgramUniform4fvEXT = (PFNGLPROGRAMUNIFORM4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4fvEXT"))) == NULL) || r;
  r = ((glProgramUniform4iEXT = (PFNGLPROGRAMUNIFORM4IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4iEXT"))) == NULL) || r;
  r = ((glProgramUniform4ivEXT = (PFNGLPROGRAMUNIFORM4IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4ivEXT"))) == NULL) || r;
  r = ((glProgramUniform4uiEXT = (PFNGLPROGRAMUNIFORM4UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4uiEXT"))) == NULL) || r;
  r = ((glProgramUniform4uivEXT = (PFNGLPROGRAMUNIFORM4UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4uivEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x3fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix2x4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix2x4fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x2fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix3x4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix3x4fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x2fvEXT"))) == NULL) || r;
  r = ((glProgramUniformMatrix4x3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformMatrix4x3fvEXT"))) == NULL) || r;
  r = ((glPushClientAttribDefaultEXT = (PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPushClientAttribDefaultEXT"))) == NULL) || r;
  r = ((glTextureBufferEXT = (PFNGLTEXTUREBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureBufferEXT"))) == NULL) || r;
  r = ((glTextureImage1DEXT = (PFNGLTEXTUREIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureImage1DEXT"))) == NULL) || r;
  r = ((glTextureImage2DEXT = (PFNGLTEXTUREIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureImage2DEXT"))) == NULL) || r;
  r = ((glTextureImage3DEXT = (PFNGLTEXTUREIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureImage3DEXT"))) == NULL) || r;
  r = ((glTextureParameterIivEXT = (PFNGLTEXTUREPARAMETERIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureParameterIivEXT"))) == NULL) || r;
  r = ((glTextureParameterIuivEXT = (PFNGLTEXTUREPARAMETERIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureParameterIuivEXT"))) == NULL) || r;
  r = ((glTextureParameterfEXT = (PFNGLTEXTUREPARAMETERFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureParameterfEXT"))) == NULL) || r;
  r = ((glTextureParameterfvEXT = (PFNGLTEXTUREPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureParameterfvEXT"))) == NULL) || r;
  r = ((glTextureParameteriEXT = (PFNGLTEXTUREPARAMETERIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureParameteriEXT"))) == NULL) || r;
  r = ((glTextureParameterivEXT = (PFNGLTEXTUREPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureParameterivEXT"))) == NULL) || r;
  r = ((glTextureRenderbufferEXT = (PFNGLTEXTURERENDERBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureRenderbufferEXT"))) == NULL) || r;
  r = ((glTextureSubImage1DEXT = (PFNGLTEXTURESUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureSubImage1DEXT"))) == NULL) || r;
  r = ((glTextureSubImage2DEXT = (PFNGLTEXTURESUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureSubImage2DEXT"))) == NULL) || r;
  r = ((glTextureSubImage3DEXT = (PFNGLTEXTURESUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureSubImage3DEXT"))) == NULL) || r;
  r = ((glUnmapNamedBufferEXT = (PFNGLUNMAPNAMEDBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUnmapNamedBufferEXT"))) == NULL) || r;
  r = ((glVertexArrayColorOffsetEXT = (PFNGLVERTEXARRAYCOLOROFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayColorOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayEdgeFlagOffsetEXT = (PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayEdgeFlagOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayFogCoordOffsetEXT = (PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayFogCoordOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayIndexOffsetEXT = (PFNGLVERTEXARRAYINDEXOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayIndexOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayMultiTexCoordOffsetEXT = (PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayMultiTexCoordOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayNormalOffsetEXT = (PFNGLVERTEXARRAYNORMALOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayNormalOffsetEXT"))) == NULL) || r;
  r = ((glVertexArraySecondaryColorOffsetEXT = (PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArraySecondaryColorOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayTexCoordOffsetEXT = (PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayTexCoordOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayVertexAttribIOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayVertexAttribIOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayVertexAttribOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayVertexAttribOffsetEXT"))) == NULL) || r;
  r = ((glVertexArrayVertexOffsetEXT = (PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayVertexOffsetEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_direct_state_access */

#ifdef GL_EXT_draw_buffers2

static GLboolean _glewInit_GL_EXT_draw_buffers2 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColorMaskIndexedEXT = (PFNGLCOLORMASKINDEXEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorMaskIndexedEXT"))) == NULL) || r;
  r = ((glDisableIndexedEXT = (PFNGLDISABLEINDEXEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableIndexedEXT"))) == NULL) || r;
  r = ((glEnableIndexedEXT = (PFNGLENABLEINDEXEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableIndexedEXT"))) == NULL) || r;
  r = ((glGetBooleanIndexedvEXT = (PFNGLGETBOOLEANINDEXEDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBooleanIndexedvEXT"))) == NULL) || r;
  r = ((glGetIntegerIndexedvEXT = (PFNGLGETINTEGERINDEXEDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetIntegerIndexedvEXT"))) == NULL) || r;
  r = ((glIsEnabledIndexedEXT = (PFNGLISENABLEDINDEXEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsEnabledIndexedEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_draw_buffers2 */

#ifdef GL_EXT_draw_instanced

static GLboolean _glewInit_GL_EXT_draw_instanced (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawArraysInstancedEXT = (PFNGLDRAWARRAYSINSTANCEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawArraysInstancedEXT"))) == NULL) || r;
  r = ((glDrawElementsInstancedEXT = (PFNGLDRAWELEMENTSINSTANCEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawElementsInstancedEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_draw_instanced */

#ifdef GL_EXT_draw_range_elements

static GLboolean _glewInit_GL_EXT_draw_range_elements (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDrawRangeElementsEXT = (PFNGLDRAWRANGEELEMENTSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawRangeElementsEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_draw_range_elements */

#ifdef GL_EXT_fog_coord

static GLboolean _glewInit_GL_EXT_fog_coord (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFogCoordPointerEXT = (PFNGLFOGCOORDPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordPointerEXT"))) == NULL) || r;
  r = ((glFogCoorddEXT = (PFNGLFOGCOORDDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoorddEXT"))) == NULL) || r;
  r = ((glFogCoorddvEXT = (PFNGLFOGCOORDDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoorddvEXT"))) == NULL) || r;
  r = ((glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordfEXT"))) == NULL) || r;
  r = ((glFogCoordfvEXT = (PFNGLFOGCOORDFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordfvEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_fog_coord */

#ifdef GL_EXT_fragment_lighting

static GLboolean _glewInit_GL_EXT_fragment_lighting (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFragmentColorMaterialEXT = (PFNGLFRAGMENTCOLORMATERIALEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentColorMaterialEXT"))) == NULL) || r;
  r = ((glFragmentLightModelfEXT = (PFNGLFRAGMENTLIGHTMODELFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModelfEXT"))) == NULL) || r;
  r = ((glFragmentLightModelfvEXT = (PFNGLFRAGMENTLIGHTMODELFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModelfvEXT"))) == NULL) || r;
  r = ((glFragmentLightModeliEXT = (PFNGLFRAGMENTLIGHTMODELIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModeliEXT"))) == NULL) || r;
  r = ((glFragmentLightModelivEXT = (PFNGLFRAGMENTLIGHTMODELIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModelivEXT"))) == NULL) || r;
  r = ((glFragmentLightfEXT = (PFNGLFRAGMENTLIGHTFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightfEXT"))) == NULL) || r;
  r = ((glFragmentLightfvEXT = (PFNGLFRAGMENTLIGHTFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightfvEXT"))) == NULL) || r;
  r = ((glFragmentLightiEXT = (PFNGLFRAGMENTLIGHTIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightiEXT"))) == NULL) || r;
  r = ((glFragmentLightivEXT = (PFNGLFRAGMENTLIGHTIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightivEXT"))) == NULL) || r;
  r = ((glFragmentMaterialfEXT = (PFNGLFRAGMENTMATERIALFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialfEXT"))) == NULL) || r;
  r = ((glFragmentMaterialfvEXT = (PFNGLFRAGMENTMATERIALFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialfvEXT"))) == NULL) || r;
  r = ((glFragmentMaterialiEXT = (PFNGLFRAGMENTMATERIALIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialiEXT"))) == NULL) || r;
  r = ((glFragmentMaterialivEXT = (PFNGLFRAGMENTMATERIALIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialivEXT"))) == NULL) || r;
  r = ((glGetFragmentLightfvEXT = (PFNGLGETFRAGMENTLIGHTFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentLightfvEXT"))) == NULL) || r;
  r = ((glGetFragmentLightivEXT = (PFNGLGETFRAGMENTLIGHTIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentLightivEXT"))) == NULL) || r;
  r = ((glGetFragmentMaterialfvEXT = (PFNGLGETFRAGMENTMATERIALFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentMaterialfvEXT"))) == NULL) || r;
  r = ((glGetFragmentMaterialivEXT = (PFNGLGETFRAGMENTMATERIALIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentMaterialivEXT"))) == NULL) || r;
  r = ((glLightEnviEXT = (PFNGLLIGHTENVIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLightEnviEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_fragment_lighting */

#ifdef GL_EXT_framebuffer_blit

static GLboolean _glewInit_GL_EXT_framebuffer_blit (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBlitFramebufferEXT = (PFNGLBLITFRAMEBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBlitFramebufferEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_framebuffer_blit */

#ifdef GL_EXT_framebuffer_multisample

static GLboolean _glewInit_GL_EXT_framebuffer_multisample (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glRenderbufferStorageMultisampleEXT = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRenderbufferStorageMultisampleEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_framebuffer_multisample */

#ifdef GL_EXT_framebuffer_object

static GLboolean _glewInit_GL_EXT_framebuffer_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindFramebufferEXT"))) == NULL) || r;
  r = ((glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindRenderbufferEXT"))) == NULL) || r;
  r = ((glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCheckFramebufferStatusEXT"))) == NULL) || r;
  r = ((glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteFramebuffersEXT"))) == NULL) || r;
  r = ((glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteRenderbuffersEXT"))) == NULL) || r;
  r = ((glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferRenderbufferEXT"))) == NULL) || r;
  r = ((glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTexture1DEXT"))) == NULL) || r;
  r = ((glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTexture2DEXT"))) == NULL) || r;
  r = ((glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTexture3DEXT"))) == NULL) || r;
  r = ((glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenFramebuffersEXT"))) == NULL) || r;
  r = ((glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenRenderbuffersEXT"))) == NULL) || r;
  r = ((glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenerateMipmapEXT"))) == NULL) || r;
  r = ((glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFramebufferAttachmentParameterivEXT"))) == NULL) || r;
  r = ((glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetRenderbufferParameterivEXT"))) == NULL) || r;
  r = ((glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsFramebufferEXT"))) == NULL) || r;
  r = ((glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsRenderbufferEXT"))) == NULL) || r;
  r = ((glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRenderbufferStorageEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_framebuffer_object */

#ifdef GL_EXT_framebuffer_sRGB

#endif /* GL_EXT_framebuffer_sRGB */

#ifdef GL_EXT_geometry_shader4

static GLboolean _glewInit_GL_EXT_geometry_shader4 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFramebufferTextureEXT = (PFNGLFRAMEBUFFERTEXTUREEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTextureEXT"))) == NULL) || r;
  r = ((glFramebufferTextureFaceEXT = (PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTextureFaceEXT"))) == NULL) || r;
  r = ((glFramebufferTextureLayerEXT = (PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFramebufferTextureLayerEXT"))) == NULL) || r;
  r = ((glProgramParameteriEXT = (PFNGLPROGRAMPARAMETERIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameteriEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_geometry_shader4 */

#ifdef GL_EXT_gpu_program_parameters

static GLboolean _glewInit_GL_EXT_gpu_program_parameters (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glProgramEnvParameters4fvEXT = (PFNGLPROGRAMENVPARAMETERS4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameters4fvEXT"))) == NULL) || r;
  r = ((glProgramLocalParameters4fvEXT = (PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameters4fvEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_gpu_program_parameters */

#ifdef GL_EXT_gpu_shader4

static GLboolean _glewInit_GL_EXT_gpu_shader4 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindFragDataLocationEXT = (PFNGLBINDFRAGDATALOCATIONEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindFragDataLocationEXT"))) == NULL) || r;
  r = ((glGetFragDataLocationEXT = (PFNGLGETFRAGDATALOCATIONEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragDataLocationEXT"))) == NULL) || r;
  r = ((glGetUniformuivEXT = (PFNGLGETUNIFORMUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformuivEXT"))) == NULL) || r;
  r = ((glGetVertexAttribIivEXT = (PFNGLGETVERTEXATTRIBIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribIivEXT"))) == NULL) || r;
  r = ((glGetVertexAttribIuivEXT = (PFNGLGETVERTEXATTRIBIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribIuivEXT"))) == NULL) || r;
  r = ((glUniform1uiEXT = (PFNGLUNIFORM1UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1uiEXT"))) == NULL) || r;
  r = ((glUniform1uivEXT = (PFNGLUNIFORM1UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1uivEXT"))) == NULL) || r;
  r = ((glUniform2uiEXT = (PFNGLUNIFORM2UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2uiEXT"))) == NULL) || r;
  r = ((glUniform2uivEXT = (PFNGLUNIFORM2UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2uivEXT"))) == NULL) || r;
  r = ((glUniform3uiEXT = (PFNGLUNIFORM3UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3uiEXT"))) == NULL) || r;
  r = ((glUniform3uivEXT = (PFNGLUNIFORM3UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3uivEXT"))) == NULL) || r;
  r = ((glUniform4uiEXT = (PFNGLUNIFORM4UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4uiEXT"))) == NULL) || r;
  r = ((glUniform4uivEXT = (PFNGLUNIFORM4UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4uivEXT"))) == NULL) || r;
  r = ((glVertexAttribI1iEXT = (PFNGLVERTEXATTRIBI1IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1iEXT"))) == NULL) || r;
  r = ((glVertexAttribI1ivEXT = (PFNGLVERTEXATTRIBI1IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1ivEXT"))) == NULL) || r;
  r = ((glVertexAttribI1uiEXT = (PFNGLVERTEXATTRIBI1UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1uiEXT"))) == NULL) || r;
  r = ((glVertexAttribI1uivEXT = (PFNGLVERTEXATTRIBI1UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI1uivEXT"))) == NULL) || r;
  r = ((glVertexAttribI2iEXT = (PFNGLVERTEXATTRIBI2IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2iEXT"))) == NULL) || r;
  r = ((glVertexAttribI2ivEXT = (PFNGLVERTEXATTRIBI2IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2ivEXT"))) == NULL) || r;
  r = ((glVertexAttribI2uiEXT = (PFNGLVERTEXATTRIBI2UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2uiEXT"))) == NULL) || r;
  r = ((glVertexAttribI2uivEXT = (PFNGLVERTEXATTRIBI2UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI2uivEXT"))) == NULL) || r;
  r = ((glVertexAttribI3iEXT = (PFNGLVERTEXATTRIBI3IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3iEXT"))) == NULL) || r;
  r = ((glVertexAttribI3ivEXT = (PFNGLVERTEXATTRIBI3IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3ivEXT"))) == NULL) || r;
  r = ((glVertexAttribI3uiEXT = (PFNGLVERTEXATTRIBI3UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3uiEXT"))) == NULL) || r;
  r = ((glVertexAttribI3uivEXT = (PFNGLVERTEXATTRIBI3UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI3uivEXT"))) == NULL) || r;
  r = ((glVertexAttribI4bvEXT = (PFNGLVERTEXATTRIBI4BVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4bvEXT"))) == NULL) || r;
  r = ((glVertexAttribI4iEXT = (PFNGLVERTEXATTRIBI4IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4iEXT"))) == NULL) || r;
  r = ((glVertexAttribI4ivEXT = (PFNGLVERTEXATTRIBI4IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4ivEXT"))) == NULL) || r;
  r = ((glVertexAttribI4svEXT = (PFNGLVERTEXATTRIBI4SVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4svEXT"))) == NULL) || r;
  r = ((glVertexAttribI4ubvEXT = (PFNGLVERTEXATTRIBI4UBVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4ubvEXT"))) == NULL) || r;
  r = ((glVertexAttribI4uiEXT = (PFNGLVERTEXATTRIBI4UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4uiEXT"))) == NULL) || r;
  r = ((glVertexAttribI4uivEXT = (PFNGLVERTEXATTRIBI4UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4uivEXT"))) == NULL) || r;
  r = ((glVertexAttribI4usvEXT = (PFNGLVERTEXATTRIBI4USVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribI4usvEXT"))) == NULL) || r;
  r = ((glVertexAttribIPointerEXT = (PFNGLVERTEXATTRIBIPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribIPointerEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_gpu_shader4 */

#ifdef GL_EXT_histogram

static GLboolean _glewInit_GL_EXT_histogram (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetHistogramEXT = (PFNGLGETHISTOGRAMEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetHistogramEXT"))) == NULL) || r;
  r = ((glGetHistogramParameterfvEXT = (PFNGLGETHISTOGRAMPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetHistogramParameterfvEXT"))) == NULL) || r;
  r = ((glGetHistogramParameterivEXT = (PFNGLGETHISTOGRAMPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetHistogramParameterivEXT"))) == NULL) || r;
  r = ((glGetMinmaxEXT = (PFNGLGETMINMAXEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMinmaxEXT"))) == NULL) || r;
  r = ((glGetMinmaxParameterfvEXT = (PFNGLGETMINMAXPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMinmaxParameterfvEXT"))) == NULL) || r;
  r = ((glGetMinmaxParameterivEXT = (PFNGLGETMINMAXPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMinmaxParameterivEXT"))) == NULL) || r;
  r = ((glHistogramEXT = (PFNGLHISTOGRAMEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glHistogramEXT"))) == NULL) || r;
  r = ((glMinmaxEXT = (PFNGLMINMAXEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMinmaxEXT"))) == NULL) || r;
  r = ((glResetHistogramEXT = (PFNGLRESETHISTOGRAMEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glResetHistogramEXT"))) == NULL) || r;
  r = ((glResetMinmaxEXT = (PFNGLRESETMINMAXEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glResetMinmaxEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_histogram */

#ifdef GL_EXT_index_array_formats

#endif /* GL_EXT_index_array_formats */

#ifdef GL_EXT_index_func

static GLboolean _glewInit_GL_EXT_index_func (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glIndexFuncEXT = (PFNGLINDEXFUNCEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexFuncEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_index_func */

#ifdef GL_EXT_index_material

static GLboolean _glewInit_GL_EXT_index_material (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glIndexMaterialEXT = (PFNGLINDEXMATERIALEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexMaterialEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_index_material */

#ifdef GL_EXT_index_texture

#endif /* GL_EXT_index_texture */

#ifdef GL_EXT_light_texture

static GLboolean _glewInit_GL_EXT_light_texture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glApplyTextureEXT = (PFNGLAPPLYTEXTUREEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glApplyTextureEXT"))) == NULL) || r;
  r = ((glTextureLightEXT = (PFNGLTEXTURELIGHTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureLightEXT"))) == NULL) || r;
  r = ((glTextureMaterialEXT = (PFNGLTEXTUREMATERIALEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureMaterialEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_light_texture */

#ifdef GL_EXT_misc_attribute

#endif /* GL_EXT_misc_attribute */

#ifdef GL_EXT_multi_draw_arrays

static GLboolean _glewInit_GL_EXT_multi_draw_arrays (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glMultiDrawArraysEXT = (PFNGLMULTIDRAWARRAYSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiDrawArraysEXT"))) == NULL) || r;
  r = ((glMultiDrawElementsEXT = (PFNGLMULTIDRAWELEMENTSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiDrawElementsEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_multi_draw_arrays */

#ifdef GL_EXT_multisample

static GLboolean _glewInit_GL_EXT_multisample (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glSampleMaskEXT = (PFNGLSAMPLEMASKEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSampleMaskEXT"))) == NULL) || r;
  r = ((glSamplePatternEXT = (PFNGLSAMPLEPATTERNEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplePatternEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_multisample */

#ifdef GL_EXT_packed_depth_stencil

#endif /* GL_EXT_packed_depth_stencil */

#ifdef GL_EXT_packed_float

#endif /* GL_EXT_packed_float */

#ifdef GL_EXT_packed_pixels

#endif /* GL_EXT_packed_pixels */

#ifdef GL_EXT_paletted_texture

static GLboolean _glewInit_GL_EXT_paletted_texture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColorTableEXT = (PFNGLCOLORTABLEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorTableEXT"))) == NULL) || r;
  r = ((glGetColorTableEXT = (PFNGLGETCOLORTABLEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableEXT"))) == NULL) || r;
  r = ((glGetColorTableParameterfvEXT = (PFNGLGETCOLORTABLEPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableParameterfvEXT"))) == NULL) || r;
  r = ((glGetColorTableParameterivEXT = (PFNGLGETCOLORTABLEPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableParameterivEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_paletted_texture */

#ifdef GL_EXT_pixel_buffer_object

#endif /* GL_EXT_pixel_buffer_object */

#ifdef GL_EXT_pixel_transform

static GLboolean _glewInit_GL_EXT_pixel_transform (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetPixelTransformParameterfvEXT = (PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPixelTransformParameterfvEXT"))) == NULL) || r;
  r = ((glGetPixelTransformParameterivEXT = (PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetPixelTransformParameterivEXT"))) == NULL) || r;
  r = ((glPixelTransformParameterfEXT = (PFNGLPIXELTRANSFORMPARAMETERFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelTransformParameterfEXT"))) == NULL) || r;
  r = ((glPixelTransformParameterfvEXT = (PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelTransformParameterfvEXT"))) == NULL) || r;
  r = ((glPixelTransformParameteriEXT = (PFNGLPIXELTRANSFORMPARAMETERIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelTransformParameteriEXT"))) == NULL) || r;
  r = ((glPixelTransformParameterivEXT = (PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelTransformParameterivEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_pixel_transform */

#ifdef GL_EXT_pixel_transform_color_table

#endif /* GL_EXT_pixel_transform_color_table */

#ifdef GL_EXT_point_parameters

static GLboolean _glewInit_GL_EXT_point_parameters (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPointParameterfEXT = (PFNGLPOINTPARAMETERFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameterfEXT"))) == NULL) || r;
  r = ((glPointParameterfvEXT = (PFNGLPOINTPARAMETERFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameterfvEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_point_parameters */

#ifdef GL_EXT_polygon_offset

static GLboolean _glewInit_GL_EXT_polygon_offset (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPolygonOffsetEXT = (PFNGLPOLYGONOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPolygonOffsetEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_polygon_offset */

#ifdef GL_EXT_provoking_vertex

static GLboolean _glewInit_GL_EXT_provoking_vertex (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glProvokingVertexEXT = (PFNGLPROVOKINGVERTEXEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProvokingVertexEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_provoking_vertex */

#ifdef GL_EXT_rescale_normal

#endif /* GL_EXT_rescale_normal */

#ifdef GL_EXT_scene_marker

static GLboolean _glewInit_GL_EXT_scene_marker (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginSceneEXT = (PFNGLBEGINSCENEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginSceneEXT"))) == NULL) || r;
  r = ((glEndSceneEXT = (PFNGLENDSCENEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndSceneEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_scene_marker */

#ifdef GL_EXT_secondary_color

static GLboolean _glewInit_GL_EXT_secondary_color (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glSecondaryColor3bEXT = (PFNGLSECONDARYCOLOR3BEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3bEXT"))) == NULL) || r;
  r = ((glSecondaryColor3bvEXT = (PFNGLSECONDARYCOLOR3BVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3bvEXT"))) == NULL) || r;
  r = ((glSecondaryColor3dEXT = (PFNGLSECONDARYCOLOR3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3dEXT"))) == NULL) || r;
  r = ((glSecondaryColor3dvEXT = (PFNGLSECONDARYCOLOR3DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3dvEXT"))) == NULL) || r;
  r = ((glSecondaryColor3fEXT = (PFNGLSECONDARYCOLOR3FEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3fEXT"))) == NULL) || r;
  r = ((glSecondaryColor3fvEXT = (PFNGLSECONDARYCOLOR3FVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3fvEXT"))) == NULL) || r;
  r = ((glSecondaryColor3iEXT = (PFNGLSECONDARYCOLOR3IEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3iEXT"))) == NULL) || r;
  r = ((glSecondaryColor3ivEXT = (PFNGLSECONDARYCOLOR3IVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3ivEXT"))) == NULL) || r;
  r = ((glSecondaryColor3sEXT = (PFNGLSECONDARYCOLOR3SEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3sEXT"))) == NULL) || r;
  r = ((glSecondaryColor3svEXT = (PFNGLSECONDARYCOLOR3SVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3svEXT"))) == NULL) || r;
  r = ((glSecondaryColor3ubEXT = (PFNGLSECONDARYCOLOR3UBEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3ubEXT"))) == NULL) || r;
  r = ((glSecondaryColor3ubvEXT = (PFNGLSECONDARYCOLOR3UBVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3ubvEXT"))) == NULL) || r;
  r = ((glSecondaryColor3uiEXT = (PFNGLSECONDARYCOLOR3UIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3uiEXT"))) == NULL) || r;
  r = ((glSecondaryColor3uivEXT = (PFNGLSECONDARYCOLOR3UIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3uivEXT"))) == NULL) || r;
  r = ((glSecondaryColor3usEXT = (PFNGLSECONDARYCOLOR3USEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3usEXT"))) == NULL) || r;
  r = ((glSecondaryColor3usvEXT = (PFNGLSECONDARYCOLOR3USVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3usvEXT"))) == NULL) || r;
  r = ((glSecondaryColorPointerEXT = (PFNGLSECONDARYCOLORPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColorPointerEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_secondary_color */

#ifdef GL_EXT_separate_shader_objects

static GLboolean _glewInit_GL_EXT_separate_shader_objects (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glActiveProgramEXT = (PFNGLACTIVEPROGRAMEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glActiveProgramEXT"))) == NULL) || r;
  r = ((glCreateShaderProgramEXT = (PFNGLCREATESHADERPROGRAMEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCreateShaderProgramEXT"))) == NULL) || r;
  r = ((glUseShaderProgramEXT = (PFNGLUSESHADERPROGRAMEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUseShaderProgramEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_separate_shader_objects */

#ifdef GL_EXT_separate_specular_color

#endif /* GL_EXT_separate_specular_color */

#ifdef GL_EXT_shader_image_load_store

static GLboolean _glewInit_GL_EXT_shader_image_load_store (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindImageTextureEXT = (PFNGLBINDIMAGETEXTUREEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindImageTextureEXT"))) == NULL) || r;
  r = ((glMemoryBarrierEXT = (PFNGLMEMORYBARRIEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMemoryBarrierEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_shader_image_load_store */

#ifdef GL_EXT_shadow_funcs

#endif /* GL_EXT_shadow_funcs */

#ifdef GL_EXT_shared_texture_palette

#endif /* GL_EXT_shared_texture_palette */

#ifdef GL_EXT_stencil_clear_tag

#endif /* GL_EXT_stencil_clear_tag */

#ifdef GL_EXT_stencil_two_side

static GLboolean _glewInit_GL_EXT_stencil_two_side (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glActiveStencilFaceEXT = (PFNGLACTIVESTENCILFACEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glActiveStencilFaceEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_stencil_two_side */

#ifdef GL_EXT_stencil_wrap

#endif /* GL_EXT_stencil_wrap */

#ifdef GL_EXT_subtexture

static GLboolean _glewInit_GL_EXT_subtexture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTexSubImage1DEXT = (PFNGLTEXSUBIMAGE1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexSubImage1DEXT"))) == NULL) || r;
  r = ((glTexSubImage2DEXT = (PFNGLTEXSUBIMAGE2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexSubImage2DEXT"))) == NULL) || r;
  r = ((glTexSubImage3DEXT = (PFNGLTEXSUBIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexSubImage3DEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_subtexture */

#ifdef GL_EXT_texture

#endif /* GL_EXT_texture */

#ifdef GL_EXT_texture3D

static GLboolean _glewInit_GL_EXT_texture3D (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTexImage3DEXT = (PFNGLTEXIMAGE3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexImage3DEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_texture3D */

#ifdef GL_EXT_texture_array

#endif /* GL_EXT_texture_array */

#ifdef GL_EXT_texture_buffer_object

static GLboolean _glewInit_GL_EXT_texture_buffer_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTexBufferEXT = (PFNGLTEXBUFFEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexBufferEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_texture_buffer_object */

#ifdef GL_EXT_texture_compression_dxt1

#endif /* GL_EXT_texture_compression_dxt1 */

#ifdef GL_EXT_texture_compression_latc

#endif /* GL_EXT_texture_compression_latc */

#ifdef GL_EXT_texture_compression_rgtc

#endif /* GL_EXT_texture_compression_rgtc */

#ifdef GL_EXT_texture_compression_s3tc

#endif /* GL_EXT_texture_compression_s3tc */

#ifdef GL_EXT_texture_cube_map

#endif /* GL_EXT_texture_cube_map */

#ifdef GL_EXT_texture_edge_clamp

#endif /* GL_EXT_texture_edge_clamp */

#ifdef GL_EXT_texture_env

#endif /* GL_EXT_texture_env */

#ifdef GL_EXT_texture_env_add

#endif /* GL_EXT_texture_env_add */

#ifdef GL_EXT_texture_env_combine

#endif /* GL_EXT_texture_env_combine */

#ifdef GL_EXT_texture_env_dot3

#endif /* GL_EXT_texture_env_dot3 */

#ifdef GL_EXT_texture_filter_anisotropic

#endif /* GL_EXT_texture_filter_anisotropic */

#ifdef GL_EXT_texture_integer

static GLboolean _glewInit_GL_EXT_texture_integer (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glClearColorIiEXT = (PFNGLCLEARCOLORIIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearColorIiEXT"))) == NULL) || r;
  r = ((glClearColorIuiEXT = (PFNGLCLEARCOLORIUIEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearColorIuiEXT"))) == NULL) || r;
  r = ((glGetTexParameterIivEXT = (PFNGLGETTEXPARAMETERIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexParameterIivEXT"))) == NULL) || r;
  r = ((glGetTexParameterIuivEXT = (PFNGLGETTEXPARAMETERIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexParameterIuivEXT"))) == NULL) || r;
  r = ((glTexParameterIivEXT = (PFNGLTEXPARAMETERIIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameterIivEXT"))) == NULL) || r;
  r = ((glTexParameterIuivEXT = (PFNGLTEXPARAMETERIUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexParameterIuivEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_texture_integer */

#ifdef GL_EXT_texture_lod_bias

#endif /* GL_EXT_texture_lod_bias */

#ifdef GL_EXT_texture_mirror_clamp

#endif /* GL_EXT_texture_mirror_clamp */

#ifdef GL_EXT_texture_object

static GLboolean _glewInit_GL_EXT_texture_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAreTexturesResidentEXT = (PFNGLARETEXTURESRESIDENTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAreTexturesResidentEXT"))) == NULL) || r;
  r = ((glBindTextureEXT = (PFNGLBINDTEXTUREEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindTextureEXT"))) == NULL) || r;
  r = ((glDeleteTexturesEXT = (PFNGLDELETETEXTURESEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteTexturesEXT"))) == NULL) || r;
  r = ((glGenTexturesEXT = (PFNGLGENTEXTURESEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenTexturesEXT"))) == NULL) || r;
  r = ((glIsTextureEXT = (PFNGLISTEXTUREEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsTextureEXT"))) == NULL) || r;
  r = ((glPrioritizeTexturesEXT = (PFNGLPRIORITIZETEXTURESEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPrioritizeTexturesEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_texture_object */

#ifdef GL_EXT_texture_perturb_normal

static GLboolean _glewInit_GL_EXT_texture_perturb_normal (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTextureNormalEXT = (PFNGLTEXTURENORMALEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureNormalEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_texture_perturb_normal */

#ifdef GL_EXT_texture_rectangle

#endif /* GL_EXT_texture_rectangle */

#ifdef GL_EXT_texture_sRGB

#endif /* GL_EXT_texture_sRGB */

#ifdef GL_EXT_texture_sRGB_decode

#endif /* GL_EXT_texture_sRGB_decode */

#ifdef GL_EXT_texture_shared_exponent

#endif /* GL_EXT_texture_shared_exponent */

#ifdef GL_EXT_texture_snorm

#endif /* GL_EXT_texture_snorm */

#ifdef GL_EXT_texture_swizzle

#endif /* GL_EXT_texture_swizzle */

#ifdef GL_EXT_timer_query

static GLboolean _glewInit_GL_EXT_timer_query (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetQueryObjecti64vEXT = (PFNGLGETQUERYOBJECTI64VEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjecti64vEXT"))) == NULL) || r;
  r = ((glGetQueryObjectui64vEXT = (PFNGLGETQUERYOBJECTUI64VEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetQueryObjectui64vEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_timer_query */

#ifdef GL_EXT_transform_feedback

static GLboolean _glewInit_GL_EXT_transform_feedback (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginTransformFeedbackEXT = (PFNGLBEGINTRANSFORMFEEDBACKEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginTransformFeedbackEXT"))) == NULL) || r;
  r = ((glBindBufferBaseEXT = (PFNGLBINDBUFFERBASEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferBaseEXT"))) == NULL) || r;
  r = ((glBindBufferOffsetEXT = (PFNGLBINDBUFFEROFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferOffsetEXT"))) == NULL) || r;
  r = ((glBindBufferRangeEXT = (PFNGLBINDBUFFERRANGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferRangeEXT"))) == NULL) || r;
  r = ((glEndTransformFeedbackEXT = (PFNGLENDTRANSFORMFEEDBACKEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndTransformFeedbackEXT"))) == NULL) || r;
  r = ((glGetTransformFeedbackVaryingEXT = (PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTransformFeedbackVaryingEXT"))) == NULL) || r;
  r = ((glTransformFeedbackVaryingsEXT = (PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTransformFeedbackVaryingsEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_transform_feedback */

#ifdef GL_EXT_vertex_array

static GLboolean _glewInit_GL_EXT_vertex_array (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glArrayElementEXT = (PFNGLARRAYELEMENTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glArrayElementEXT"))) == NULL) || r;
  r = ((glColorPointerEXT = (PFNGLCOLORPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorPointerEXT"))) == NULL) || r;
  r = ((glDrawArraysEXT = (PFNGLDRAWARRAYSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawArraysEXT"))) == NULL) || r;
  r = ((glEdgeFlagPointerEXT = (PFNGLEDGEFLAGPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEdgeFlagPointerEXT"))) == NULL) || r;
  r = ((glIndexPointerEXT = (PFNGLINDEXPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexPointerEXT"))) == NULL) || r;
  r = ((glNormalPointerEXT = (PFNGLNORMALPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalPointerEXT"))) == NULL) || r;
  r = ((glTexCoordPointerEXT = (PFNGLTEXCOORDPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordPointerEXT"))) == NULL) || r;
  r = ((glVertexPointerEXT = (PFNGLVERTEXPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexPointerEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_vertex_array */

#ifdef GL_EXT_vertex_array_bgra

#endif /* GL_EXT_vertex_array_bgra */

#ifdef GL_EXT_vertex_attrib_64bit

static GLboolean _glewInit_GL_EXT_vertex_attrib_64bit (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetVertexAttribLdvEXT = (PFNGLGETVERTEXATTRIBLDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribLdvEXT"))) == NULL) || r;
  r = ((glVertexArrayVertexAttribLOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayVertexAttribLOffsetEXT"))) == NULL) || r;
  r = ((glVertexAttribL1dEXT = (PFNGLVERTEXATTRIBL1DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1dEXT"))) == NULL) || r;
  r = ((glVertexAttribL1dvEXT = (PFNGLVERTEXATTRIBL1DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1dvEXT"))) == NULL) || r;
  r = ((glVertexAttribL2dEXT = (PFNGLVERTEXATTRIBL2DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2dEXT"))) == NULL) || r;
  r = ((glVertexAttribL2dvEXT = (PFNGLVERTEXATTRIBL2DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2dvEXT"))) == NULL) || r;
  r = ((glVertexAttribL3dEXT = (PFNGLVERTEXATTRIBL3DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3dEXT"))) == NULL) || r;
  r = ((glVertexAttribL3dvEXT = (PFNGLVERTEXATTRIBL3DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3dvEXT"))) == NULL) || r;
  r = ((glVertexAttribL4dEXT = (PFNGLVERTEXATTRIBL4DEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4dEXT"))) == NULL) || r;
  r = ((glVertexAttribL4dvEXT = (PFNGLVERTEXATTRIBL4DVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4dvEXT"))) == NULL) || r;
  r = ((glVertexAttribLPointerEXT = (PFNGLVERTEXATTRIBLPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribLPointerEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_vertex_attrib_64bit */

#ifdef GL_EXT_vertex_shader

static GLboolean _glewInit_GL_EXT_vertex_shader (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginVertexShaderEXT = (PFNGLBEGINVERTEXSHADEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginVertexShaderEXT"))) == NULL) || r;
  r = ((glBindLightParameterEXT = (PFNGLBINDLIGHTPARAMETEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindLightParameterEXT"))) == NULL) || r;
  r = ((glBindMaterialParameterEXT = (PFNGLBINDMATERIALPARAMETEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindMaterialParameterEXT"))) == NULL) || r;
  r = ((glBindParameterEXT = (PFNGLBINDPARAMETEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindParameterEXT"))) == NULL) || r;
  r = ((glBindTexGenParameterEXT = (PFNGLBINDTEXGENPARAMETEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindTexGenParameterEXT"))) == NULL) || r;
  r = ((glBindTextureUnitParameterEXT = (PFNGLBINDTEXTUREUNITPARAMETEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindTextureUnitParameterEXT"))) == NULL) || r;
  r = ((glBindVertexShaderEXT = (PFNGLBINDVERTEXSHADEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindVertexShaderEXT"))) == NULL) || r;
  r = ((glDeleteVertexShaderEXT = (PFNGLDELETEVERTEXSHADEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteVertexShaderEXT"))) == NULL) || r;
  r = ((glDisableVariantClientStateEXT = (PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDisableVariantClientStateEXT"))) == NULL) || r;
  r = ((glEnableVariantClientStateEXT = (PFNGLENABLEVARIANTCLIENTSTATEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEnableVariantClientStateEXT"))) == NULL) || r;
  r = ((glEndVertexShaderEXT = (PFNGLENDVERTEXSHADEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndVertexShaderEXT"))) == NULL) || r;
  r = ((glExtractComponentEXT = (PFNGLEXTRACTCOMPONENTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glExtractComponentEXT"))) == NULL) || r;
  r = ((glGenSymbolsEXT = (PFNGLGENSYMBOLSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenSymbolsEXT"))) == NULL) || r;
  r = ((glGenVertexShadersEXT = (PFNGLGENVERTEXSHADERSEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenVertexShadersEXT"))) == NULL) || r;
  r = ((glGetInvariantBooleanvEXT = (PFNGLGETINVARIANTBOOLEANVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetInvariantBooleanvEXT"))) == NULL) || r;
  r = ((glGetInvariantFloatvEXT = (PFNGLGETINVARIANTFLOATVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetInvariantFloatvEXT"))) == NULL) || r;
  r = ((glGetInvariantIntegervEXT = (PFNGLGETINVARIANTINTEGERVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetInvariantIntegervEXT"))) == NULL) || r;
  r = ((glGetLocalConstantBooleanvEXT = (PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetLocalConstantBooleanvEXT"))) == NULL) || r;
  r = ((glGetLocalConstantFloatvEXT = (PFNGLGETLOCALCONSTANTFLOATVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetLocalConstantFloatvEXT"))) == NULL) || r;
  r = ((glGetLocalConstantIntegervEXT = (PFNGLGETLOCALCONSTANTINTEGERVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetLocalConstantIntegervEXT"))) == NULL) || r;
  r = ((glGetVariantBooleanvEXT = (PFNGLGETVARIANTBOOLEANVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVariantBooleanvEXT"))) == NULL) || r;
  r = ((glGetVariantFloatvEXT = (PFNGLGETVARIANTFLOATVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVariantFloatvEXT"))) == NULL) || r;
  r = ((glGetVariantIntegervEXT = (PFNGLGETVARIANTINTEGERVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVariantIntegervEXT"))) == NULL) || r;
  r = ((glGetVariantPointervEXT = (PFNGLGETVARIANTPOINTERVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVariantPointervEXT"))) == NULL) || r;
  r = ((glInsertComponentEXT = (PFNGLINSERTCOMPONENTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glInsertComponentEXT"))) == NULL) || r;
  r = ((glIsVariantEnabledEXT = (PFNGLISVARIANTENABLEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsVariantEnabledEXT"))) == NULL) || r;
  r = ((glSetInvariantEXT = (PFNGLSETINVARIANTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSetInvariantEXT"))) == NULL) || r;
  r = ((glSetLocalConstantEXT = (PFNGLSETLOCALCONSTANTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSetLocalConstantEXT"))) == NULL) || r;
  r = ((glShaderOp1EXT = (PFNGLSHADEROP1EXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glShaderOp1EXT"))) == NULL) || r;
  r = ((glShaderOp2EXT = (PFNGLSHADEROP2EXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glShaderOp2EXT"))) == NULL) || r;
  r = ((glShaderOp3EXT = (PFNGLSHADEROP3EXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glShaderOp3EXT"))) == NULL) || r;
  r = ((glSwizzleEXT = (PFNGLSWIZZLEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSwizzleEXT"))) == NULL) || r;
  r = ((glVariantPointerEXT = (PFNGLVARIANTPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantPointerEXT"))) == NULL) || r;
  r = ((glVariantbvEXT = (PFNGLVARIANTBVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantbvEXT"))) == NULL) || r;
  r = ((glVariantdvEXT = (PFNGLVARIANTDVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantdvEXT"))) == NULL) || r;
  r = ((glVariantfvEXT = (PFNGLVARIANTFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantfvEXT"))) == NULL) || r;
  r = ((glVariantivEXT = (PFNGLVARIANTIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantivEXT"))) == NULL) || r;
  r = ((glVariantsvEXT = (PFNGLVARIANTSVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantsvEXT"))) == NULL) || r;
  r = ((glVariantubvEXT = (PFNGLVARIANTUBVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantubvEXT"))) == NULL) || r;
  r = ((glVariantuivEXT = (PFNGLVARIANTUIVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantuivEXT"))) == NULL) || r;
  r = ((glVariantusvEXT = (PFNGLVARIANTUSVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVariantusvEXT"))) == NULL) || r;
  r = ((glWriteMaskEXT = (PFNGLWRITEMASKEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWriteMaskEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_vertex_shader */

#ifdef GL_EXT_vertex_weighting

static GLboolean _glewInit_GL_EXT_vertex_weighting (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glVertexWeightPointerEXT = (PFNGLVERTEXWEIGHTPOINTEREXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexWeightPointerEXT"))) == NULL) || r;
  r = ((glVertexWeightfEXT = (PFNGLVERTEXWEIGHTFEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexWeightfEXT"))) == NULL) || r;
  r = ((glVertexWeightfvEXT = (PFNGLVERTEXWEIGHTFVEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexWeightfvEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_EXT_vertex_weighting */

#ifdef GL_GREMEDY_frame_terminator

static GLboolean _glewInit_GL_GREMEDY_frame_terminator (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFrameTerminatorGREMEDY = (PFNGLFRAMETERMINATORGREMEDYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFrameTerminatorGREMEDY"))) == NULL) || r;

  return r;
}

#endif /* GL_GREMEDY_frame_terminator */

#ifdef GL_GREMEDY_string_marker

static GLboolean _glewInit_GL_GREMEDY_string_marker (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glStringMarkerGREMEDY = (PFNGLSTRINGMARKERGREMEDYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glStringMarkerGREMEDY"))) == NULL) || r;

  return r;
}

#endif /* GL_GREMEDY_string_marker */

#ifdef GL_HP_convolution_border_modes

#endif /* GL_HP_convolution_border_modes */

#ifdef GL_HP_image_transform

static GLboolean _glewInit_GL_HP_image_transform (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetImageTransformParameterfvHP = (PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetImageTransformParameterfvHP"))) == NULL) || r;
  r = ((glGetImageTransformParameterivHP = (PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetImageTransformParameterivHP"))) == NULL) || r;
  r = ((glImageTransformParameterfHP = (PFNGLIMAGETRANSFORMPARAMETERFHPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glImageTransformParameterfHP"))) == NULL) || r;
  r = ((glImageTransformParameterfvHP = (PFNGLIMAGETRANSFORMPARAMETERFVHPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glImageTransformParameterfvHP"))) == NULL) || r;
  r = ((glImageTransformParameteriHP = (PFNGLIMAGETRANSFORMPARAMETERIHPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glImageTransformParameteriHP"))) == NULL) || r;
  r = ((glImageTransformParameterivHP = (PFNGLIMAGETRANSFORMPARAMETERIVHPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glImageTransformParameterivHP"))) == NULL) || r;

  return r;
}

#endif /* GL_HP_image_transform */

#ifdef GL_HP_occlusion_test

#endif /* GL_HP_occlusion_test */

#ifdef GL_HP_texture_lighting

#endif /* GL_HP_texture_lighting */

#ifdef GL_IBM_cull_vertex

#endif /* GL_IBM_cull_vertex */

#ifdef GL_IBM_multimode_draw_arrays

static GLboolean _glewInit_GL_IBM_multimode_draw_arrays (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glMultiModeDrawArraysIBM = (PFNGLMULTIMODEDRAWARRAYSIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiModeDrawArraysIBM"))) == NULL) || r;
  r = ((glMultiModeDrawElementsIBM = (PFNGLMULTIMODEDRAWELEMENTSIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiModeDrawElementsIBM"))) == NULL) || r;

  return r;
}

#endif /* GL_IBM_multimode_draw_arrays */

#ifdef GL_IBM_rasterpos_clip

#endif /* GL_IBM_rasterpos_clip */

#ifdef GL_IBM_static_data

#endif /* GL_IBM_static_data */

#ifdef GL_IBM_texture_mirrored_repeat

#endif /* GL_IBM_texture_mirrored_repeat */

#ifdef GL_IBM_vertex_array_lists

static GLboolean _glewInit_GL_IBM_vertex_array_lists (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColorPointerListIBM = (PFNGLCOLORPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorPointerListIBM"))) == NULL) || r;
  r = ((glEdgeFlagPointerListIBM = (PFNGLEDGEFLAGPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEdgeFlagPointerListIBM"))) == NULL) || r;
  r = ((glFogCoordPointerListIBM = (PFNGLFOGCOORDPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordPointerListIBM"))) == NULL) || r;
  r = ((glIndexPointerListIBM = (PFNGLINDEXPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexPointerListIBM"))) == NULL) || r;
  r = ((glNormalPointerListIBM = (PFNGLNORMALPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalPointerListIBM"))) == NULL) || r;
  r = ((glSecondaryColorPointerListIBM = (PFNGLSECONDARYCOLORPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColorPointerListIBM"))) == NULL) || r;
  r = ((glTexCoordPointerListIBM = (PFNGLTEXCOORDPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordPointerListIBM"))) == NULL) || r;
  r = ((glVertexPointerListIBM = (PFNGLVERTEXPOINTERLISTIBMPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexPointerListIBM"))) == NULL) || r;

  return r;
}

#endif /* GL_IBM_vertex_array_lists */

#ifdef GL_INGR_color_clamp

#endif /* GL_INGR_color_clamp */

#ifdef GL_INGR_interlace_read

#endif /* GL_INGR_interlace_read */

#ifdef GL_INTEL_parallel_arrays

static GLboolean _glewInit_GL_INTEL_parallel_arrays (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColorPointervINTEL = (PFNGLCOLORPOINTERVINTELPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorPointervINTEL"))) == NULL) || r;
  r = ((glNormalPointervINTEL = (PFNGLNORMALPOINTERVINTELPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalPointervINTEL"))) == NULL) || r;
  r = ((glTexCoordPointervINTEL = (PFNGLTEXCOORDPOINTERVINTELPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordPointervINTEL"))) == NULL) || r;
  r = ((glVertexPointervINTEL = (PFNGLVERTEXPOINTERVINTELPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexPointervINTEL"))) == NULL) || r;

  return r;
}

#endif /* GL_INTEL_parallel_arrays */

#ifdef GL_INTEL_texture_scissor

static GLboolean _glewInit_GL_INTEL_texture_scissor (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTexScissorFuncINTEL = (PFNGLTEXSCISSORFUNCINTELPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexScissorFuncINTEL"))) == NULL) || r;
  r = ((glTexScissorINTEL = (PFNGLTEXSCISSORINTELPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexScissorINTEL"))) == NULL) || r;

  return r;
}

#endif /* GL_INTEL_texture_scissor */

#ifdef GL_KTX_buffer_region

static GLboolean _glewInit_GL_KTX_buffer_region (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBufferRegionEnabledEXT = (PFNGLBUFFERREGIONENABLEDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBufferRegionEnabledEXT"))) == NULL) || r;
  r = ((glDeleteBufferRegionEXT = (PFNGLDELETEBUFFERREGIONEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteBufferRegionEXT"))) == NULL) || r;
  r = ((glDrawBufferRegionEXT = (PFNGLDRAWBUFFERREGIONEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawBufferRegionEXT"))) == NULL) || r;
  r = ((glNewBufferRegionEXT = (PFNGLNEWBUFFERREGIONEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNewBufferRegionEXT"))) == NULL) || r;
  r = ((glReadBufferRegionEXT = (PFNGLREADBUFFERREGIONEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReadBufferRegionEXT"))) == NULL) || r;

  return r;
}

#endif /* GL_KTX_buffer_region */

#ifdef GL_MESAX_texture_stack

#endif /* GL_MESAX_texture_stack */

#ifdef GL_MESA_pack_invert

#endif /* GL_MESA_pack_invert */

#ifdef GL_MESA_resize_buffers

static GLboolean _glewInit_GL_MESA_resize_buffers (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glResizeBuffersMESA = (PFNGLRESIZEBUFFERSMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glResizeBuffersMESA"))) == NULL) || r;

  return r;
}

#endif /* GL_MESA_resize_buffers */

#ifdef GL_MESA_window_pos

static GLboolean _glewInit_GL_MESA_window_pos (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glWindowPos2dMESA = (PFNGLWINDOWPOS2DMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2dMESA"))) == NULL) || r;
  r = ((glWindowPos2dvMESA = (PFNGLWINDOWPOS2DVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2dvMESA"))) == NULL) || r;
  r = ((glWindowPos2fMESA = (PFNGLWINDOWPOS2FMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2fMESA"))) == NULL) || r;
  r = ((glWindowPos2fvMESA = (PFNGLWINDOWPOS2FVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2fvMESA"))) == NULL) || r;
  r = ((glWindowPos2iMESA = (PFNGLWINDOWPOS2IMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2iMESA"))) == NULL) || r;
  r = ((glWindowPos2ivMESA = (PFNGLWINDOWPOS2IVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2ivMESA"))) == NULL) || r;
  r = ((glWindowPos2sMESA = (PFNGLWINDOWPOS2SMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2sMESA"))) == NULL) || r;
  r = ((glWindowPos2svMESA = (PFNGLWINDOWPOS2SVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos2svMESA"))) == NULL) || r;
  r = ((glWindowPos3dMESA = (PFNGLWINDOWPOS3DMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3dMESA"))) == NULL) || r;
  r = ((glWindowPos3dvMESA = (PFNGLWINDOWPOS3DVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3dvMESA"))) == NULL) || r;
  r = ((glWindowPos3fMESA = (PFNGLWINDOWPOS3FMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3fMESA"))) == NULL) || r;
  r = ((glWindowPos3fvMESA = (PFNGLWINDOWPOS3FVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3fvMESA"))) == NULL) || r;
  r = ((glWindowPos3iMESA = (PFNGLWINDOWPOS3IMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3iMESA"))) == NULL) || r;
  r = ((glWindowPos3ivMESA = (PFNGLWINDOWPOS3IVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3ivMESA"))) == NULL) || r;
  r = ((glWindowPos3sMESA = (PFNGLWINDOWPOS3SMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3sMESA"))) == NULL) || r;
  r = ((glWindowPos3svMESA = (PFNGLWINDOWPOS3SVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos3svMESA"))) == NULL) || r;
  r = ((glWindowPos4dMESA = (PFNGLWINDOWPOS4DMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4dMESA"))) == NULL) || r;
  r = ((glWindowPos4dvMESA = (PFNGLWINDOWPOS4DVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4dvMESA"))) == NULL) || r;
  r = ((glWindowPos4fMESA = (PFNGLWINDOWPOS4FMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4fMESA"))) == NULL) || r;
  r = ((glWindowPos4fvMESA = (PFNGLWINDOWPOS4FVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4fvMESA"))) == NULL) || r;
  r = ((glWindowPos4iMESA = (PFNGLWINDOWPOS4IMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4iMESA"))) == NULL) || r;
  r = ((glWindowPos4ivMESA = (PFNGLWINDOWPOS4IVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4ivMESA"))) == NULL) || r;
  r = ((glWindowPos4sMESA = (PFNGLWINDOWPOS4SMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4sMESA"))) == NULL) || r;
  r = ((glWindowPos4svMESA = (PFNGLWINDOWPOS4SVMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glWindowPos4svMESA"))) == NULL) || r;

  return r;
}

#endif /* GL_MESA_window_pos */

#ifdef GL_MESA_ycbcr_texture

#endif /* GL_MESA_ycbcr_texture */

#ifdef GL_NVX_gpu_memory_info

#endif /* GL_NVX_gpu_memory_info */

#ifdef GL_NV_blend_square

#endif /* GL_NV_blend_square */

#ifdef GL_NV_conditional_render

static GLboolean _glewInit_GL_NV_conditional_render (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginConditionalRenderNV = (PFNGLBEGINCONDITIONALRENDERNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginConditionalRenderNV"))) == NULL) || r;
  r = ((glEndConditionalRenderNV = (PFNGLENDCONDITIONALRENDERNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndConditionalRenderNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_conditional_render */

#ifdef GL_NV_copy_depth_to_color

#endif /* GL_NV_copy_depth_to_color */

#ifdef GL_NV_copy_image

static GLboolean _glewInit_GL_NV_copy_image (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCopyImageSubDataNV = (PFNGLCOPYIMAGESUBDATANVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyImageSubDataNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_copy_image */

#ifdef GL_NV_depth_buffer_float

static GLboolean _glewInit_GL_NV_depth_buffer_float (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glClearDepthdNV = (PFNGLCLEARDEPTHDNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearDepthdNV"))) == NULL) || r;
  r = ((glDepthBoundsdNV = (PFNGLDEPTHBOUNDSDNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthBoundsdNV"))) == NULL) || r;
  r = ((glDepthRangedNV = (PFNGLDEPTHRANGEDNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthRangedNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_depth_buffer_float */

#ifdef GL_NV_depth_clamp

#endif /* GL_NV_depth_clamp */

#ifdef GL_NV_depth_range_unclamped

#endif /* GL_NV_depth_range_unclamped */

#ifdef GL_NV_evaluators

static GLboolean _glewInit_GL_NV_evaluators (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glEvalMapsNV = (PFNGLEVALMAPSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEvalMapsNV"))) == NULL) || r;
  r = ((glGetMapAttribParameterfvNV = (PFNGLGETMAPATTRIBPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapAttribParameterfvNV"))) == NULL) || r;
  r = ((glGetMapAttribParameterivNV = (PFNGLGETMAPATTRIBPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapAttribParameterivNV"))) == NULL) || r;
  r = ((glGetMapControlPointsNV = (PFNGLGETMAPCONTROLPOINTSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapControlPointsNV"))) == NULL) || r;
  r = ((glGetMapParameterfvNV = (PFNGLGETMAPPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapParameterfvNV"))) == NULL) || r;
  r = ((glGetMapParameterivNV = (PFNGLGETMAPPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMapParameterivNV"))) == NULL) || r;
  r = ((glMapControlPointsNV = (PFNGLMAPCONTROLPOINTSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapControlPointsNV"))) == NULL) || r;
  r = ((glMapParameterfvNV = (PFNGLMAPPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapParameterfvNV"))) == NULL) || r;
  r = ((glMapParameterivNV = (PFNGLMAPPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMapParameterivNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_evaluators */

#ifdef GL_NV_explicit_multisample

static GLboolean _glewInit_GL_NV_explicit_multisample (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetMultisamplefvNV = (PFNGLGETMULTISAMPLEFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetMultisamplefvNV"))) == NULL) || r;
  r = ((glSampleMaskIndexedNV = (PFNGLSAMPLEMASKINDEXEDNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSampleMaskIndexedNV"))) == NULL) || r;
  r = ((glTexRenderbufferNV = (PFNGLTEXRENDERBUFFERNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexRenderbufferNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_explicit_multisample */

#ifdef GL_NV_fence

static GLboolean _glewInit_GL_NV_fence (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDeleteFencesNV = (PFNGLDELETEFENCESNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteFencesNV"))) == NULL) || r;
  r = ((glFinishFenceNV = (PFNGLFINISHFENCENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFinishFenceNV"))) == NULL) || r;
  r = ((glGenFencesNV = (PFNGLGENFENCESNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenFencesNV"))) == NULL) || r;
  r = ((glGetFenceivNV = (PFNGLGETFENCEIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFenceivNV"))) == NULL) || r;
  r = ((glIsFenceNV = (PFNGLISFENCENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsFenceNV"))) == NULL) || r;
  r = ((glSetFenceNV = (PFNGLSETFENCENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSetFenceNV"))) == NULL) || r;
  r = ((glTestFenceNV = (PFNGLTESTFENCENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTestFenceNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_fence */

#ifdef GL_NV_float_buffer

#endif /* GL_NV_float_buffer */

#ifdef GL_NV_fog_distance

#endif /* GL_NV_fog_distance */

#ifdef GL_NV_fragment_program

static GLboolean _glewInit_GL_NV_fragment_program (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetProgramNamedParameterdvNV = (PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramNamedParameterdvNV"))) == NULL) || r;
  r = ((glGetProgramNamedParameterfvNV = (PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramNamedParameterfvNV"))) == NULL) || r;
  r = ((glProgramNamedParameter4dNV = (PFNGLPROGRAMNAMEDPARAMETER4DNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramNamedParameter4dNV"))) == NULL) || r;
  r = ((glProgramNamedParameter4dvNV = (PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramNamedParameter4dvNV"))) == NULL) || r;
  r = ((glProgramNamedParameter4fNV = (PFNGLPROGRAMNAMEDPARAMETER4FNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramNamedParameter4fNV"))) == NULL) || r;
  r = ((glProgramNamedParameter4fvNV = (PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramNamedParameter4fvNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_fragment_program */

#ifdef GL_NV_fragment_program2

#endif /* GL_NV_fragment_program2 */

#ifdef GL_NV_fragment_program4

#endif /* GL_NV_fragment_program4 */

#ifdef GL_NV_fragment_program_option

#endif /* GL_NV_fragment_program_option */

#ifdef GL_NV_framebuffer_multisample_coverage

static GLboolean _glewInit_GL_NV_framebuffer_multisample_coverage (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glRenderbufferStorageMultisampleCoverageNV = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRenderbufferStorageMultisampleCoverageNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_framebuffer_multisample_coverage */

#ifdef GL_NV_geometry_program4

static GLboolean _glewInit_GL_NV_geometry_program4 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glProgramVertexLimitNV = (PFNGLPROGRAMVERTEXLIMITNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramVertexLimitNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_geometry_program4 */

#ifdef GL_NV_geometry_shader4

#endif /* GL_NV_geometry_shader4 */

#ifdef GL_NV_gpu_program4

static GLboolean _glewInit_GL_NV_gpu_program4 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glProgramEnvParameterI4iNV = (PFNGLPROGRAMENVPARAMETERI4INVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameterI4iNV"))) == NULL) || r;
  r = ((glProgramEnvParameterI4ivNV = (PFNGLPROGRAMENVPARAMETERI4IVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameterI4ivNV"))) == NULL) || r;
  r = ((glProgramEnvParameterI4uiNV = (PFNGLPROGRAMENVPARAMETERI4UINVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameterI4uiNV"))) == NULL) || r;
  r = ((glProgramEnvParameterI4uivNV = (PFNGLPROGRAMENVPARAMETERI4UIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParameterI4uivNV"))) == NULL) || r;
  r = ((glProgramEnvParametersI4ivNV = (PFNGLPROGRAMENVPARAMETERSI4IVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParametersI4ivNV"))) == NULL) || r;
  r = ((glProgramEnvParametersI4uivNV = (PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramEnvParametersI4uivNV"))) == NULL) || r;
  r = ((glProgramLocalParameterI4iNV = (PFNGLPROGRAMLOCALPARAMETERI4INVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameterI4iNV"))) == NULL) || r;
  r = ((glProgramLocalParameterI4ivNV = (PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameterI4ivNV"))) == NULL) || r;
  r = ((glProgramLocalParameterI4uiNV = (PFNGLPROGRAMLOCALPARAMETERI4UINVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameterI4uiNV"))) == NULL) || r;
  r = ((glProgramLocalParameterI4uivNV = (PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParameterI4uivNV"))) == NULL) || r;
  r = ((glProgramLocalParametersI4ivNV = (PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParametersI4ivNV"))) == NULL) || r;
  r = ((glProgramLocalParametersI4uivNV = (PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramLocalParametersI4uivNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_gpu_program4 */

#ifdef GL_NV_gpu_program5

#endif /* GL_NV_gpu_program5 */

#ifdef GL_NV_gpu_program_fp64

#endif /* GL_NV_gpu_program_fp64 */

#ifdef GL_NV_gpu_shader5

static GLboolean _glewInit_GL_NV_gpu_shader5 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetUniformi64vNV = (PFNGLGETUNIFORMI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformi64vNV"))) == NULL) || r;
  r = ((glGetUniformui64vNV = (PFNGLGETUNIFORMUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetUniformui64vNV"))) == NULL) || r;
  r = ((glProgramUniform1i64NV = (PFNGLPROGRAMUNIFORM1I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1i64NV"))) == NULL) || r;
  r = ((glProgramUniform1i64vNV = (PFNGLPROGRAMUNIFORM1I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1i64vNV"))) == NULL) || r;
  r = ((glProgramUniform1ui64NV = (PFNGLPROGRAMUNIFORM1UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1ui64NV"))) == NULL) || r;
  r = ((glProgramUniform1ui64vNV = (PFNGLPROGRAMUNIFORM1UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform1ui64vNV"))) == NULL) || r;
  r = ((glProgramUniform2i64NV = (PFNGLPROGRAMUNIFORM2I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2i64NV"))) == NULL) || r;
  r = ((glProgramUniform2i64vNV = (PFNGLPROGRAMUNIFORM2I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2i64vNV"))) == NULL) || r;
  r = ((glProgramUniform2ui64NV = (PFNGLPROGRAMUNIFORM2UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2ui64NV"))) == NULL) || r;
  r = ((glProgramUniform2ui64vNV = (PFNGLPROGRAMUNIFORM2UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform2ui64vNV"))) == NULL) || r;
  r = ((glProgramUniform3i64NV = (PFNGLPROGRAMUNIFORM3I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3i64NV"))) == NULL) || r;
  r = ((glProgramUniform3i64vNV = (PFNGLPROGRAMUNIFORM3I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3i64vNV"))) == NULL) || r;
  r = ((glProgramUniform3ui64NV = (PFNGLPROGRAMUNIFORM3UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3ui64NV"))) == NULL) || r;
  r = ((glProgramUniform3ui64vNV = (PFNGLPROGRAMUNIFORM3UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform3ui64vNV"))) == NULL) || r;
  r = ((glProgramUniform4i64NV = (PFNGLPROGRAMUNIFORM4I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4i64NV"))) == NULL) || r;
  r = ((glProgramUniform4i64vNV = (PFNGLPROGRAMUNIFORM4I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4i64vNV"))) == NULL) || r;
  r = ((glProgramUniform4ui64NV = (PFNGLPROGRAMUNIFORM4UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4ui64NV"))) == NULL) || r;
  r = ((glProgramUniform4ui64vNV = (PFNGLPROGRAMUNIFORM4UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniform4ui64vNV"))) == NULL) || r;
  r = ((glUniform1i64NV = (PFNGLUNIFORM1I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1i64NV"))) == NULL) || r;
  r = ((glUniform1i64vNV = (PFNGLUNIFORM1I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1i64vNV"))) == NULL) || r;
  r = ((glUniform1ui64NV = (PFNGLUNIFORM1UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1ui64NV"))) == NULL) || r;
  r = ((glUniform1ui64vNV = (PFNGLUNIFORM1UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform1ui64vNV"))) == NULL) || r;
  r = ((glUniform2i64NV = (PFNGLUNIFORM2I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2i64NV"))) == NULL) || r;
  r = ((glUniform2i64vNV = (PFNGLUNIFORM2I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2i64vNV"))) == NULL) || r;
  r = ((glUniform2ui64NV = (PFNGLUNIFORM2UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2ui64NV"))) == NULL) || r;
  r = ((glUniform2ui64vNV = (PFNGLUNIFORM2UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform2ui64vNV"))) == NULL) || r;
  r = ((glUniform3i64NV = (PFNGLUNIFORM3I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3i64NV"))) == NULL) || r;
  r = ((glUniform3i64vNV = (PFNGLUNIFORM3I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3i64vNV"))) == NULL) || r;
  r = ((glUniform3ui64NV = (PFNGLUNIFORM3UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3ui64NV"))) == NULL) || r;
  r = ((glUniform3ui64vNV = (PFNGLUNIFORM3UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform3ui64vNV"))) == NULL) || r;
  r = ((glUniform4i64NV = (PFNGLUNIFORM4I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4i64NV"))) == NULL) || r;
  r = ((glUniform4i64vNV = (PFNGLUNIFORM4I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4i64vNV"))) == NULL) || r;
  r = ((glUniform4ui64NV = (PFNGLUNIFORM4UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4ui64NV"))) == NULL) || r;
  r = ((glUniform4ui64vNV = (PFNGLUNIFORM4UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniform4ui64vNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_gpu_shader5 */

#ifdef GL_NV_half_float

static GLboolean _glewInit_GL_NV_half_float (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColor3hNV = (PFNGLCOLOR3HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3hNV"))) == NULL) || r;
  r = ((glColor3hvNV = (PFNGLCOLOR3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3hvNV"))) == NULL) || r;
  r = ((glColor4hNV = (PFNGLCOLOR4HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4hNV"))) == NULL) || r;
  r = ((glColor4hvNV = (PFNGLCOLOR4HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4hvNV"))) == NULL) || r;
  r = ((glFogCoordhNV = (PFNGLFOGCOORDHNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordhNV"))) == NULL) || r;
  r = ((glFogCoordhvNV = (PFNGLFOGCOORDHVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordhvNV"))) == NULL) || r;
  r = ((glMultiTexCoord1hNV = (PFNGLMULTITEXCOORD1HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1hNV"))) == NULL) || r;
  r = ((glMultiTexCoord1hvNV = (PFNGLMULTITEXCOORD1HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord1hvNV"))) == NULL) || r;
  r = ((glMultiTexCoord2hNV = (PFNGLMULTITEXCOORD2HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2hNV"))) == NULL) || r;
  r = ((glMultiTexCoord2hvNV = (PFNGLMULTITEXCOORD2HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord2hvNV"))) == NULL) || r;
  r = ((glMultiTexCoord3hNV = (PFNGLMULTITEXCOORD3HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3hNV"))) == NULL) || r;
  r = ((glMultiTexCoord3hvNV = (PFNGLMULTITEXCOORD3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord3hvNV"))) == NULL) || r;
  r = ((glMultiTexCoord4hNV = (PFNGLMULTITEXCOORD4HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4hNV"))) == NULL) || r;
  r = ((glMultiTexCoord4hvNV = (PFNGLMULTITEXCOORD4HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMultiTexCoord4hvNV"))) == NULL) || r;
  r = ((glNormal3hNV = (PFNGLNORMAL3HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3hNV"))) == NULL) || r;
  r = ((glNormal3hvNV = (PFNGLNORMAL3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3hvNV"))) == NULL) || r;
  r = ((glSecondaryColor3hNV = (PFNGLSECONDARYCOLOR3HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3hNV"))) == NULL) || r;
  r = ((glSecondaryColor3hvNV = (PFNGLSECONDARYCOLOR3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColor3hvNV"))) == NULL) || r;
  r = ((glTexCoord1hNV = (PFNGLTEXCOORD1HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1hNV"))) == NULL) || r;
  r = ((glTexCoord1hvNV = (PFNGLTEXCOORD1HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord1hvNV"))) == NULL) || r;
  r = ((glTexCoord2hNV = (PFNGLTEXCOORD2HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2hNV"))) == NULL) || r;
  r = ((glTexCoord2hvNV = (PFNGLTEXCOORD2HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2hvNV"))) == NULL) || r;
  r = ((glTexCoord3hNV = (PFNGLTEXCOORD3HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3hNV"))) == NULL) || r;
  r = ((glTexCoord3hvNV = (PFNGLTEXCOORD3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord3hvNV"))) == NULL) || r;
  r = ((glTexCoord4hNV = (PFNGLTEXCOORD4HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4hNV"))) == NULL) || r;
  r = ((glTexCoord4hvNV = (PFNGLTEXCOORD4HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4hvNV"))) == NULL) || r;
  r = ((glVertex2hNV = (PFNGLVERTEX2HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2hNV"))) == NULL) || r;
  r = ((glVertex2hvNV = (PFNGLVERTEX2HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex2hvNV"))) == NULL) || r;
  r = ((glVertex3hNV = (PFNGLVERTEX3HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3hNV"))) == NULL) || r;
  r = ((glVertex3hvNV = (PFNGLVERTEX3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex3hvNV"))) == NULL) || r;
  r = ((glVertex4hNV = (PFNGLVERTEX4HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4hNV"))) == NULL) || r;
  r = ((glVertex4hvNV = (PFNGLVERTEX4HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertex4hvNV"))) == NULL) || r;
  r = ((glVertexAttrib1hNV = (PFNGLVERTEXATTRIB1HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1hNV"))) == NULL) || r;
  r = ((glVertexAttrib1hvNV = (PFNGLVERTEXATTRIB1HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1hvNV"))) == NULL) || r;
  r = ((glVertexAttrib2hNV = (PFNGLVERTEXATTRIB2HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2hNV"))) == NULL) || r;
  r = ((glVertexAttrib2hvNV = (PFNGLVERTEXATTRIB2HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2hvNV"))) == NULL) || r;
  r = ((glVertexAttrib3hNV = (PFNGLVERTEXATTRIB3HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3hNV"))) == NULL) || r;
  r = ((glVertexAttrib3hvNV = (PFNGLVERTEXATTRIB3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3hvNV"))) == NULL) || r;
  r = ((glVertexAttrib4hNV = (PFNGLVERTEXATTRIB4HNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4hNV"))) == NULL) || r;
  r = ((glVertexAttrib4hvNV = (PFNGLVERTEXATTRIB4HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4hvNV"))) == NULL) || r;
  r = ((glVertexAttribs1hvNV = (PFNGLVERTEXATTRIBS1HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs1hvNV"))) == NULL) || r;
  r = ((glVertexAttribs2hvNV = (PFNGLVERTEXATTRIBS2HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs2hvNV"))) == NULL) || r;
  r = ((glVertexAttribs3hvNV = (PFNGLVERTEXATTRIBS3HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs3hvNV"))) == NULL) || r;
  r = ((glVertexAttribs4hvNV = (PFNGLVERTEXATTRIBS4HVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs4hvNV"))) == NULL) || r;
  r = ((glVertexWeighthNV = (PFNGLVERTEXWEIGHTHNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexWeighthNV"))) == NULL) || r;
  r = ((glVertexWeighthvNV = (PFNGLVERTEXWEIGHTHVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexWeighthvNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_half_float */

#ifdef GL_NV_light_max_exponent

#endif /* GL_NV_light_max_exponent */

#ifdef GL_NV_multisample_coverage

#endif /* GL_NV_multisample_coverage */

#ifdef GL_NV_multisample_filter_hint

#endif /* GL_NV_multisample_filter_hint */

#ifdef GL_NV_occlusion_query

static GLboolean _glewInit_GL_NV_occlusion_query (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBeginOcclusionQueryNV = (PFNGLBEGINOCCLUSIONQUERYNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginOcclusionQueryNV"))) == NULL) || r;
  r = ((glDeleteOcclusionQueriesNV = (PFNGLDELETEOCCLUSIONQUERIESNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteOcclusionQueriesNV"))) == NULL) || r;
  r = ((glEndOcclusionQueryNV = (PFNGLENDOCCLUSIONQUERYNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndOcclusionQueryNV"))) == NULL) || r;
  r = ((glGenOcclusionQueriesNV = (PFNGLGENOCCLUSIONQUERIESNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenOcclusionQueriesNV"))) == NULL) || r;
  r = ((glGetOcclusionQueryivNV = (PFNGLGETOCCLUSIONQUERYIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetOcclusionQueryivNV"))) == NULL) || r;
  r = ((glGetOcclusionQueryuivNV = (PFNGLGETOCCLUSIONQUERYUIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetOcclusionQueryuivNV"))) == NULL) || r;
  r = ((glIsOcclusionQueryNV = (PFNGLISOCCLUSIONQUERYNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsOcclusionQueryNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_occlusion_query */

#ifdef GL_NV_packed_depth_stencil

#endif /* GL_NV_packed_depth_stencil */

#ifdef GL_NV_parameter_buffer_object

static GLboolean _glewInit_GL_NV_parameter_buffer_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glProgramBufferParametersIivNV = (PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramBufferParametersIivNV"))) == NULL) || r;
  r = ((glProgramBufferParametersIuivNV = (PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramBufferParametersIuivNV"))) == NULL) || r;
  r = ((glProgramBufferParametersfvNV = (PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramBufferParametersfvNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_parameter_buffer_object */

#ifdef GL_NV_parameter_buffer_object2

#endif /* GL_NV_parameter_buffer_object2 */

#ifdef GL_NV_pixel_data_range

static GLboolean _glewInit_GL_NV_pixel_data_range (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFlushPixelDataRangeNV = (PFNGLFLUSHPIXELDATARANGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlushPixelDataRangeNV"))) == NULL) || r;
  r = ((glPixelDataRangeNV = (PFNGLPIXELDATARANGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelDataRangeNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_pixel_data_range */

#ifdef GL_NV_point_sprite

static GLboolean _glewInit_GL_NV_point_sprite (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPointParameteriNV = (PFNGLPOINTPARAMETERINVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameteriNV"))) == NULL) || r;
  r = ((glPointParameterivNV = (PFNGLPOINTPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPointParameterivNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_point_sprite */

#ifdef GL_NV_present_video

static GLboolean _glewInit_GL_NV_present_video (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetVideoi64vNV = (PFNGLGETVIDEOI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVideoi64vNV"))) == NULL) || r;
  r = ((glGetVideoivNV = (PFNGLGETVIDEOIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVideoivNV"))) == NULL) || r;
  r = ((glGetVideoui64vNV = (PFNGLGETVIDEOUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVideoui64vNV"))) == NULL) || r;
  r = ((glGetVideouivNV = (PFNGLGETVIDEOUIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVideouivNV"))) == NULL) || r;
  r = ((glPresentFrameDualFillNV = (PFNGLPRESENTFRAMEDUALFILLNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPresentFrameDualFillNV"))) == NULL) || r;
  r = ((glPresentFrameKeyedNV = (PFNGLPRESENTFRAMEKEYEDNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPresentFrameKeyedNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_present_video */

#ifdef GL_NV_primitive_restart

static GLboolean _glewInit_GL_NV_primitive_restart (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPrimitiveRestartIndexNV = (PFNGLPRIMITIVERESTARTINDEXNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPrimitiveRestartIndexNV"))) == NULL) || r;
  r = ((glPrimitiveRestartNV = (PFNGLPRIMITIVERESTARTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPrimitiveRestartNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_primitive_restart */

#ifdef GL_NV_register_combiners

static GLboolean _glewInit_GL_NV_register_combiners (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCombinerInputNV = (PFNGLCOMBINERINPUTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCombinerInputNV"))) == NULL) || r;
  r = ((glCombinerOutputNV = (PFNGLCOMBINEROUTPUTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCombinerOutputNV"))) == NULL) || r;
  r = ((glCombinerParameterfNV = (PFNGLCOMBINERPARAMETERFNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCombinerParameterfNV"))) == NULL) || r;
  r = ((glCombinerParameterfvNV = (PFNGLCOMBINERPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCombinerParameterfvNV"))) == NULL) || r;
  r = ((glCombinerParameteriNV = (PFNGLCOMBINERPARAMETERINVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCombinerParameteriNV"))) == NULL) || r;
  r = ((glCombinerParameterivNV = (PFNGLCOMBINERPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCombinerParameterivNV"))) == NULL) || r;
  r = ((glFinalCombinerInputNV = (PFNGLFINALCOMBINERINPUTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFinalCombinerInputNV"))) == NULL) || r;
  r = ((glGetCombinerInputParameterfvNV = (PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCombinerInputParameterfvNV"))) == NULL) || r;
  r = ((glGetCombinerInputParameterivNV = (PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCombinerInputParameterivNV"))) == NULL) || r;
  r = ((glGetCombinerOutputParameterfvNV = (PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCombinerOutputParameterfvNV"))) == NULL) || r;
  r = ((glGetCombinerOutputParameterivNV = (PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCombinerOutputParameterivNV"))) == NULL) || r;
  r = ((glGetFinalCombinerInputParameterfvNV = (PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFinalCombinerInputParameterfvNV"))) == NULL) || r;
  r = ((glGetFinalCombinerInputParameterivNV = (PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFinalCombinerInputParameterivNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_register_combiners */

#ifdef GL_NV_register_combiners2

static GLboolean _glewInit_GL_NV_register_combiners2 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glCombinerStageParameterfvNV = (PFNGLCOMBINERSTAGEPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCombinerStageParameterfvNV"))) == NULL) || r;
  r = ((glGetCombinerStageParameterfvNV = (PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetCombinerStageParameterfvNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_register_combiners2 */

#ifdef GL_NV_shader_buffer_load

static GLboolean _glewInit_GL_NV_shader_buffer_load (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetBufferParameterui64vNV = (PFNGLGETBUFFERPARAMETERUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetBufferParameterui64vNV"))) == NULL) || r;
  r = ((glGetIntegerui64vNV = (PFNGLGETINTEGERUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetIntegerui64vNV"))) == NULL) || r;
  r = ((glGetNamedBufferParameterui64vNV = (PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetNamedBufferParameterui64vNV"))) == NULL) || r;
  r = ((glIsBufferResidentNV = (PFNGLISBUFFERRESIDENTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsBufferResidentNV"))) == NULL) || r;
  r = ((glIsNamedBufferResidentNV = (PFNGLISNAMEDBUFFERRESIDENTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsNamedBufferResidentNV"))) == NULL) || r;
  r = ((glMakeBufferNonResidentNV = (PFNGLMAKEBUFFERNONRESIDENTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMakeBufferNonResidentNV"))) == NULL) || r;
  r = ((glMakeBufferResidentNV = (PFNGLMAKEBUFFERRESIDENTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMakeBufferResidentNV"))) == NULL) || r;
  r = ((glMakeNamedBufferNonResidentNV = (PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMakeNamedBufferNonResidentNV"))) == NULL) || r;
  r = ((glMakeNamedBufferResidentNV = (PFNGLMAKENAMEDBUFFERRESIDENTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glMakeNamedBufferResidentNV"))) == NULL) || r;
  r = ((glProgramUniformui64NV = (PFNGLPROGRAMUNIFORMUI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformui64NV"))) == NULL) || r;
  r = ((glProgramUniformui64vNV = (PFNGLPROGRAMUNIFORMUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramUniformui64vNV"))) == NULL) || r;
  r = ((glUniformui64NV = (PFNGLUNIFORMUI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformui64NV"))) == NULL) || r;
  r = ((glUniformui64vNV = (PFNGLUNIFORMUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glUniformui64vNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_shader_buffer_load */

#ifdef GL_NV_tessellation_program5

#endif /* GL_NV_tessellation_program5 */

#ifdef GL_NV_texgen_emboss

#endif /* GL_NV_texgen_emboss */

#ifdef GL_NV_texgen_reflection

#endif /* GL_NV_texgen_reflection */

#ifdef GL_NV_texture_barrier

static GLboolean _glewInit_GL_NV_texture_barrier (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTextureBarrierNV = (PFNGLTEXTUREBARRIERNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureBarrierNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_texture_barrier */

#ifdef GL_NV_texture_compression_vtc

#endif /* GL_NV_texture_compression_vtc */

#ifdef GL_NV_texture_env_combine4

#endif /* GL_NV_texture_env_combine4 */

#ifdef GL_NV_texture_expand_normal

#endif /* GL_NV_texture_expand_normal */

#ifdef GL_NV_texture_rectangle

#endif /* GL_NV_texture_rectangle */

#ifdef GL_NV_texture_shader

#endif /* GL_NV_texture_shader */

#ifdef GL_NV_texture_shader2

#endif /* GL_NV_texture_shader2 */

#ifdef GL_NV_texture_shader3

#endif /* GL_NV_texture_shader3 */

#ifdef GL_NV_transform_feedback

static GLboolean _glewInit_GL_NV_transform_feedback (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glActiveVaryingNV = (PFNGLACTIVEVARYINGNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glActiveVaryingNV"))) == NULL) || r;
  r = ((glBeginTransformFeedbackNV = (PFNGLBEGINTRANSFORMFEEDBACKNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBeginTransformFeedbackNV"))) == NULL) || r;
  r = ((glBindBufferBaseNV = (PFNGLBINDBUFFERBASENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferBaseNV"))) == NULL) || r;
  r = ((glBindBufferOffsetNV = (PFNGLBINDBUFFEROFFSETNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferOffsetNV"))) == NULL) || r;
  r = ((glBindBufferRangeNV = (PFNGLBINDBUFFERRANGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindBufferRangeNV"))) == NULL) || r;
  r = ((glEndTransformFeedbackNV = (PFNGLENDTRANSFORMFEEDBACKNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEndTransformFeedbackNV"))) == NULL) || r;
  r = ((glGetActiveVaryingNV = (PFNGLGETACTIVEVARYINGNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetActiveVaryingNV"))) == NULL) || r;
  r = ((glGetTransformFeedbackVaryingNV = (PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTransformFeedbackVaryingNV"))) == NULL) || r;
  r = ((glGetVaryingLocationNV = (PFNGLGETVARYINGLOCATIONNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVaryingLocationNV"))) == NULL) || r;
  r = ((glTransformFeedbackAttribsNV = (PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTransformFeedbackAttribsNV"))) == NULL) || r;
  r = ((glTransformFeedbackVaryingsNV = (PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTransformFeedbackVaryingsNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_transform_feedback */

#ifdef GL_NV_transform_feedback2

static GLboolean _glewInit_GL_NV_transform_feedback2 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBindTransformFeedbackNV = (PFNGLBINDTRANSFORMFEEDBACKNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindTransformFeedbackNV"))) == NULL) || r;
  r = ((glDeleteTransformFeedbacksNV = (PFNGLDELETETRANSFORMFEEDBACKSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteTransformFeedbacksNV"))) == NULL) || r;
  r = ((glDrawTransformFeedbackNV = (PFNGLDRAWTRANSFORMFEEDBACKNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDrawTransformFeedbackNV"))) == NULL) || r;
  r = ((glGenTransformFeedbacksNV = (PFNGLGENTRANSFORMFEEDBACKSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenTransformFeedbacksNV"))) == NULL) || r;
  r = ((glIsTransformFeedbackNV = (PFNGLISTRANSFORMFEEDBACKNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsTransformFeedbackNV"))) == NULL) || r;
  r = ((glPauseTransformFeedbackNV = (PFNGLPAUSETRANSFORMFEEDBACKNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPauseTransformFeedbackNV"))) == NULL) || r;
  r = ((glResumeTransformFeedbackNV = (PFNGLRESUMETRANSFORMFEEDBACKNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glResumeTransformFeedbackNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_transform_feedback2 */

#ifdef GL_NV_vdpau_interop

static GLboolean _glewInit_GL_NV_vdpau_interop (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glVDPAUFiniNV = (PFNGLVDPAUFININVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUFiniNV"))) == NULL) || r;
  r = ((glVDPAUGetSurfaceivNV = (PFNGLVDPAUGETSURFACEIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUGetSurfaceivNV"))) == NULL) || r;
  r = ((glVDPAUInitNV = (PFNGLVDPAUINITNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUInitNV"))) == NULL) || r;
  r = ((glVDPAUIsSurfaceNV = (PFNGLVDPAUISSURFACENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUIsSurfaceNV"))) == NULL) || r;
  r = ((glVDPAUMapSurfacesNV = (PFNGLVDPAUMAPSURFACESNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUMapSurfacesNV"))) == NULL) || r;
  r = ((glVDPAURegisterOutputSurfaceNV = (PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAURegisterOutputSurfaceNV"))) == NULL) || r;
  r = ((glVDPAURegisterVideoSurfaceNV = (PFNGLVDPAUREGISTERVIDEOSURFACENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAURegisterVideoSurfaceNV"))) == NULL) || r;
  r = ((glVDPAUSurfaceAccessNV = (PFNGLVDPAUSURFACEACCESSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUSurfaceAccessNV"))) == NULL) || r;
  r = ((glVDPAUUnmapSurfacesNV = (PFNGLVDPAUUNMAPSURFACESNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUUnmapSurfacesNV"))) == NULL) || r;
  r = ((glVDPAUUnregisterSurfaceNV = (PFNGLVDPAUUNREGISTERSURFACENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVDPAUUnregisterSurfaceNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_vdpau_interop */

#ifdef GL_NV_vertex_array_range

static GLboolean _glewInit_GL_NV_vertex_array_range (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFlushVertexArrayRangeNV = (PFNGLFLUSHVERTEXARRAYRANGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlushVertexArrayRangeNV"))) == NULL) || r;
  r = ((glVertexArrayRangeNV = (PFNGLVERTEXARRAYRANGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexArrayRangeNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_vertex_array_range */

#ifdef GL_NV_vertex_array_range2

#endif /* GL_NV_vertex_array_range2 */

#ifdef GL_NV_vertex_attrib_integer_64bit

static GLboolean _glewInit_GL_NV_vertex_attrib_integer_64bit (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetVertexAttribLi64vNV = (PFNGLGETVERTEXATTRIBLI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribLi64vNV"))) == NULL) || r;
  r = ((glGetVertexAttribLui64vNV = (PFNGLGETVERTEXATTRIBLUI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribLui64vNV"))) == NULL) || r;
  r = ((glVertexAttribL1i64NV = (PFNGLVERTEXATTRIBL1I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1i64NV"))) == NULL) || r;
  r = ((glVertexAttribL1i64vNV = (PFNGLVERTEXATTRIBL1I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1i64vNV"))) == NULL) || r;
  r = ((glVertexAttribL1ui64NV = (PFNGLVERTEXATTRIBL1UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1ui64NV"))) == NULL) || r;
  r = ((glVertexAttribL1ui64vNV = (PFNGLVERTEXATTRIBL1UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL1ui64vNV"))) == NULL) || r;
  r = ((glVertexAttribL2i64NV = (PFNGLVERTEXATTRIBL2I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2i64NV"))) == NULL) || r;
  r = ((glVertexAttribL2i64vNV = (PFNGLVERTEXATTRIBL2I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2i64vNV"))) == NULL) || r;
  r = ((glVertexAttribL2ui64NV = (PFNGLVERTEXATTRIBL2UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2ui64NV"))) == NULL) || r;
  r = ((glVertexAttribL2ui64vNV = (PFNGLVERTEXATTRIBL2UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL2ui64vNV"))) == NULL) || r;
  r = ((glVertexAttribL3i64NV = (PFNGLVERTEXATTRIBL3I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3i64NV"))) == NULL) || r;
  r = ((glVertexAttribL3i64vNV = (PFNGLVERTEXATTRIBL3I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3i64vNV"))) == NULL) || r;
  r = ((glVertexAttribL3ui64NV = (PFNGLVERTEXATTRIBL3UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3ui64NV"))) == NULL) || r;
  r = ((glVertexAttribL3ui64vNV = (PFNGLVERTEXATTRIBL3UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL3ui64vNV"))) == NULL) || r;
  r = ((glVertexAttribL4i64NV = (PFNGLVERTEXATTRIBL4I64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4i64NV"))) == NULL) || r;
  r = ((glVertexAttribL4i64vNV = (PFNGLVERTEXATTRIBL4I64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4i64vNV"))) == NULL) || r;
  r = ((glVertexAttribL4ui64NV = (PFNGLVERTEXATTRIBL4UI64NVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4ui64NV"))) == NULL) || r;
  r = ((glVertexAttribL4ui64vNV = (PFNGLVERTEXATTRIBL4UI64VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribL4ui64vNV"))) == NULL) || r;
  r = ((glVertexAttribLFormatNV = (PFNGLVERTEXATTRIBLFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribLFormatNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_vertex_attrib_integer_64bit */

#ifdef GL_NV_vertex_buffer_unified_memory

static GLboolean _glewInit_GL_NV_vertex_buffer_unified_memory (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glBufferAddressRangeNV = (PFNGLBUFFERADDRESSRANGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBufferAddressRangeNV"))) == NULL) || r;
  r = ((glColorFormatNV = (PFNGLCOLORFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorFormatNV"))) == NULL) || r;
  r = ((glEdgeFlagFormatNV = (PFNGLEDGEFLAGFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glEdgeFlagFormatNV"))) == NULL) || r;
  r = ((glFogCoordFormatNV = (PFNGLFOGCOORDFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogCoordFormatNV"))) == NULL) || r;
  r = ((glGetIntegerui64i_vNV = (PFNGLGETINTEGERUI64I_VNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetIntegerui64i_vNV"))) == NULL) || r;
  r = ((glIndexFormatNV = (PFNGLINDEXFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIndexFormatNV"))) == NULL) || r;
  r = ((glNormalFormatNV = (PFNGLNORMALFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormalFormatNV"))) == NULL) || r;
  r = ((glSecondaryColorFormatNV = (PFNGLSECONDARYCOLORFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSecondaryColorFormatNV"))) == NULL) || r;
  r = ((glTexCoordFormatNV = (PFNGLTEXCOORDFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoordFormatNV"))) == NULL) || r;
  r = ((glVertexAttribFormatNV = (PFNGLVERTEXATTRIBFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribFormatNV"))) == NULL) || r;
  r = ((glVertexAttribIFormatNV = (PFNGLVERTEXATTRIBIFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribIFormatNV"))) == NULL) || r;
  r = ((glVertexFormatNV = (PFNGLVERTEXFORMATNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexFormatNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_vertex_buffer_unified_memory */

#ifdef GL_NV_vertex_program

static GLboolean _glewInit_GL_NV_vertex_program (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAreProgramsResidentNV = (PFNGLAREPROGRAMSRESIDENTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAreProgramsResidentNV"))) == NULL) || r;
  r = ((glBindProgramNV = (PFNGLBINDPROGRAMNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glBindProgramNV"))) == NULL) || r;
  r = ((glDeleteProgramsNV = (PFNGLDELETEPROGRAMSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteProgramsNV"))) == NULL) || r;
  r = ((glExecuteProgramNV = (PFNGLEXECUTEPROGRAMNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glExecuteProgramNV"))) == NULL) || r;
  r = ((glGenProgramsNV = (PFNGLGENPROGRAMSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenProgramsNV"))) == NULL) || r;
  r = ((glGetProgramParameterdvNV = (PFNGLGETPROGRAMPARAMETERDVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramParameterdvNV"))) == NULL) || r;
  r = ((glGetProgramParameterfvNV = (PFNGLGETPROGRAMPARAMETERFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramParameterfvNV"))) == NULL) || r;
  r = ((glGetProgramStringNV = (PFNGLGETPROGRAMSTRINGNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramStringNV"))) == NULL) || r;
  r = ((glGetProgramivNV = (PFNGLGETPROGRAMIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetProgramivNV"))) == NULL) || r;
  r = ((glGetTrackMatrixivNV = (PFNGLGETTRACKMATRIXIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTrackMatrixivNV"))) == NULL) || r;
  r = ((glGetVertexAttribPointervNV = (PFNGLGETVERTEXATTRIBPOINTERVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribPointervNV"))) == NULL) || r;
  r = ((glGetVertexAttribdvNV = (PFNGLGETVERTEXATTRIBDVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribdvNV"))) == NULL) || r;
  r = ((glGetVertexAttribfvNV = (PFNGLGETVERTEXATTRIBFVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribfvNV"))) == NULL) || r;
  r = ((glGetVertexAttribivNV = (PFNGLGETVERTEXATTRIBIVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetVertexAttribivNV"))) == NULL) || r;
  r = ((glIsProgramNV = (PFNGLISPROGRAMNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsProgramNV"))) == NULL) || r;
  r = ((glLoadProgramNV = (PFNGLLOADPROGRAMNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glLoadProgramNV"))) == NULL) || r;
  r = ((glProgramParameter4dNV = (PFNGLPROGRAMPARAMETER4DNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameter4dNV"))) == NULL) || r;
  r = ((glProgramParameter4dvNV = (PFNGLPROGRAMPARAMETER4DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameter4dvNV"))) == NULL) || r;
  r = ((glProgramParameter4fNV = (PFNGLPROGRAMPARAMETER4FNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameter4fNV"))) == NULL) || r;
  r = ((glProgramParameter4fvNV = (PFNGLPROGRAMPARAMETER4FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameter4fvNV"))) == NULL) || r;
  r = ((glProgramParameters4dvNV = (PFNGLPROGRAMPARAMETERS4DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameters4dvNV"))) == NULL) || r;
  r = ((glProgramParameters4fvNV = (PFNGLPROGRAMPARAMETERS4FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glProgramParameters4fvNV"))) == NULL) || r;
  r = ((glRequestResidentProgramsNV = (PFNGLREQUESTRESIDENTPROGRAMSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glRequestResidentProgramsNV"))) == NULL) || r;
  r = ((glTrackMatrixNV = (PFNGLTRACKMATRIXNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTrackMatrixNV"))) == NULL) || r;
  r = ((glVertexAttrib1dNV = (PFNGLVERTEXATTRIB1DNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1dNV"))) == NULL) || r;
  r = ((glVertexAttrib1dvNV = (PFNGLVERTEXATTRIB1DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1dvNV"))) == NULL) || r;
  r = ((glVertexAttrib1fNV = (PFNGLVERTEXATTRIB1FNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1fNV"))) == NULL) || r;
  r = ((glVertexAttrib1fvNV = (PFNGLVERTEXATTRIB1FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1fvNV"))) == NULL) || r;
  r = ((glVertexAttrib1sNV = (PFNGLVERTEXATTRIB1SNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1sNV"))) == NULL) || r;
  r = ((glVertexAttrib1svNV = (PFNGLVERTEXATTRIB1SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib1svNV"))) == NULL) || r;
  r = ((glVertexAttrib2dNV = (PFNGLVERTEXATTRIB2DNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2dNV"))) == NULL) || r;
  r = ((glVertexAttrib2dvNV = (PFNGLVERTEXATTRIB2DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2dvNV"))) == NULL) || r;
  r = ((glVertexAttrib2fNV = (PFNGLVERTEXATTRIB2FNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2fNV"))) == NULL) || r;
  r = ((glVertexAttrib2fvNV = (PFNGLVERTEXATTRIB2FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2fvNV"))) == NULL) || r;
  r = ((glVertexAttrib2sNV = (PFNGLVERTEXATTRIB2SNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2sNV"))) == NULL) || r;
  r = ((glVertexAttrib2svNV = (PFNGLVERTEXATTRIB2SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib2svNV"))) == NULL) || r;
  r = ((glVertexAttrib3dNV = (PFNGLVERTEXATTRIB3DNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3dNV"))) == NULL) || r;
  r = ((glVertexAttrib3dvNV = (PFNGLVERTEXATTRIB3DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3dvNV"))) == NULL) || r;
  r = ((glVertexAttrib3fNV = (PFNGLVERTEXATTRIB3FNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3fNV"))) == NULL) || r;
  r = ((glVertexAttrib3fvNV = (PFNGLVERTEXATTRIB3FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3fvNV"))) == NULL) || r;
  r = ((glVertexAttrib3sNV = (PFNGLVERTEXATTRIB3SNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3sNV"))) == NULL) || r;
  r = ((glVertexAttrib3svNV = (PFNGLVERTEXATTRIB3SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib3svNV"))) == NULL) || r;
  r = ((glVertexAttrib4dNV = (PFNGLVERTEXATTRIB4DNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4dNV"))) == NULL) || r;
  r = ((glVertexAttrib4dvNV = (PFNGLVERTEXATTRIB4DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4dvNV"))) == NULL) || r;
  r = ((glVertexAttrib4fNV = (PFNGLVERTEXATTRIB4FNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4fNV"))) == NULL) || r;
  r = ((glVertexAttrib4fvNV = (PFNGLVERTEXATTRIB4FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4fvNV"))) == NULL) || r;
  r = ((glVertexAttrib4sNV = (PFNGLVERTEXATTRIB4SNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4sNV"))) == NULL) || r;
  r = ((glVertexAttrib4svNV = (PFNGLVERTEXATTRIB4SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4svNV"))) == NULL) || r;
  r = ((glVertexAttrib4ubNV = (PFNGLVERTEXATTRIB4UBNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4ubNV"))) == NULL) || r;
  r = ((glVertexAttrib4ubvNV = (PFNGLVERTEXATTRIB4UBVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttrib4ubvNV"))) == NULL) || r;
  r = ((glVertexAttribPointerNV = (PFNGLVERTEXATTRIBPOINTERNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribPointerNV"))) == NULL) || r;
  r = ((glVertexAttribs1dvNV = (PFNGLVERTEXATTRIBS1DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs1dvNV"))) == NULL) || r;
  r = ((glVertexAttribs1fvNV = (PFNGLVERTEXATTRIBS1FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs1fvNV"))) == NULL) || r;
  r = ((glVertexAttribs1svNV = (PFNGLVERTEXATTRIBS1SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs1svNV"))) == NULL) || r;
  r = ((glVertexAttribs2dvNV = (PFNGLVERTEXATTRIBS2DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs2dvNV"))) == NULL) || r;
  r = ((glVertexAttribs2fvNV = (PFNGLVERTEXATTRIBS2FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs2fvNV"))) == NULL) || r;
  r = ((glVertexAttribs2svNV = (PFNGLVERTEXATTRIBS2SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs2svNV"))) == NULL) || r;
  r = ((glVertexAttribs3dvNV = (PFNGLVERTEXATTRIBS3DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs3dvNV"))) == NULL) || r;
  r = ((glVertexAttribs3fvNV = (PFNGLVERTEXATTRIBS3FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs3fvNV"))) == NULL) || r;
  r = ((glVertexAttribs3svNV = (PFNGLVERTEXATTRIBS3SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs3svNV"))) == NULL) || r;
  r = ((glVertexAttribs4dvNV = (PFNGLVERTEXATTRIBS4DVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs4dvNV"))) == NULL) || r;
  r = ((glVertexAttribs4fvNV = (PFNGLVERTEXATTRIBS4FVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs4fvNV"))) == NULL) || r;
  r = ((glVertexAttribs4svNV = (PFNGLVERTEXATTRIBS4SVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs4svNV"))) == NULL) || r;
  r = ((glVertexAttribs4ubvNV = (PFNGLVERTEXATTRIBS4UBVNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glVertexAttribs4ubvNV"))) == NULL) || r;

  return r;
}

#endif /* GL_NV_vertex_program */

#ifdef GL_NV_vertex_program1_1

#endif /* GL_NV_vertex_program1_1 */

#ifdef GL_NV_vertex_program2

#endif /* GL_NV_vertex_program2 */

#ifdef GL_NV_vertex_program2_option

#endif /* GL_NV_vertex_program2_option */

#ifdef GL_NV_vertex_program3

#endif /* GL_NV_vertex_program3 */

#ifdef GL_NV_vertex_program4

#endif /* GL_NV_vertex_program4 */

#ifdef GL_OES_byte_coordinates

#endif /* GL_OES_byte_coordinates */

#ifdef GL_OES_compressed_paletted_texture

#endif /* GL_OES_compressed_paletted_texture */

#ifdef GL_OES_read_format

#endif /* GL_OES_read_format */

#ifdef GL_OES_single_precision

static GLboolean _glewInit_GL_OES_single_precision (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glClearDepthfOES = (PFNGLCLEARDEPTHFOESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClearDepthfOES"))) == NULL) || r;
  r = ((glClipPlanefOES = (PFNGLCLIPPLANEFOESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glClipPlanefOES"))) == NULL) || r;
  r = ((glDepthRangefOES = (PFNGLDEPTHRANGEFOESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDepthRangefOES"))) == NULL) || r;
  r = ((glFrustumfOES = (PFNGLFRUSTUMFOESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFrustumfOES"))) == NULL) || r;
  r = ((glGetClipPlanefOES = (PFNGLGETCLIPPLANEFOESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetClipPlanefOES"))) == NULL) || r;
  r = ((glOrthofOES = (PFNGLORTHOFOESPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glOrthofOES"))) == NULL) || r;

  return r;
}

#endif /* GL_OES_single_precision */

#ifdef GL_OML_interlace

#endif /* GL_OML_interlace */

#ifdef GL_OML_resample

#endif /* GL_OML_resample */

#ifdef GL_OML_subsample

#endif /* GL_OML_subsample */

#ifdef GL_PGI_misc_hints

#endif /* GL_PGI_misc_hints */

#ifdef GL_PGI_vertex_hints

#endif /* GL_PGI_vertex_hints */

#ifdef GL_REND_screen_coordinates

#endif /* GL_REND_screen_coordinates */

#ifdef GL_S3_s3tc

#endif /* GL_S3_s3tc */

#ifdef GL_SGIS_color_range

#endif /* GL_SGIS_color_range */

#ifdef GL_SGIS_detail_texture

static GLboolean _glewInit_GL_SGIS_detail_texture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glDetailTexFuncSGIS = (PFNGLDETAILTEXFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDetailTexFuncSGIS"))) == NULL) || r;
  r = ((glGetDetailTexFuncSGIS = (PFNGLGETDETAILTEXFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetDetailTexFuncSGIS"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIS_detail_texture */

#ifdef GL_SGIS_fog_function

static GLboolean _glewInit_GL_SGIS_fog_function (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFogFuncSGIS = (PFNGLFOGFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFogFuncSGIS"))) == NULL) || r;
  r = ((glGetFogFuncSGIS = (PFNGLGETFOGFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFogFuncSGIS"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIS_fog_function */

#ifdef GL_SGIS_generate_mipmap

#endif /* GL_SGIS_generate_mipmap */

#ifdef GL_SGIS_multisample

static GLboolean _glewInit_GL_SGIS_multisample (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glSampleMaskSGIS = (PFNGLSAMPLEMASKSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSampleMaskSGIS"))) == NULL) || r;
  r = ((glSamplePatternSGIS = (PFNGLSAMPLEPATTERNSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSamplePatternSGIS"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIS_multisample */

#ifdef GL_SGIS_pixel_texture

#endif /* GL_SGIS_pixel_texture */

#ifdef GL_SGIS_point_line_texgen

#endif /* GL_SGIS_point_line_texgen */

#ifdef GL_SGIS_sharpen_texture

static GLboolean _glewInit_GL_SGIS_sharpen_texture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetSharpenTexFuncSGIS = (PFNGLGETSHARPENTEXFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetSharpenTexFuncSGIS"))) == NULL) || r;
  r = ((glSharpenTexFuncSGIS = (PFNGLSHARPENTEXFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSharpenTexFuncSGIS"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIS_sharpen_texture */

#ifdef GL_SGIS_texture4D

static GLboolean _glewInit_GL_SGIS_texture4D (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTexImage4DSGIS = (PFNGLTEXIMAGE4DSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexImage4DSGIS"))) == NULL) || r;
  r = ((glTexSubImage4DSGIS = (PFNGLTEXSUBIMAGE4DSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexSubImage4DSGIS"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIS_texture4D */

#ifdef GL_SGIS_texture_border_clamp

#endif /* GL_SGIS_texture_border_clamp */

#ifdef GL_SGIS_texture_edge_clamp

#endif /* GL_SGIS_texture_edge_clamp */

#ifdef GL_SGIS_texture_filter4

static GLboolean _glewInit_GL_SGIS_texture_filter4 (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGetTexFilterFuncSGIS = (PFNGLGETTEXFILTERFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetTexFilterFuncSGIS"))) == NULL) || r;
  r = ((glTexFilterFuncSGIS = (PFNGLTEXFILTERFUNCSGISPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexFilterFuncSGIS"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIS_texture_filter4 */

#ifdef GL_SGIS_texture_lod

#endif /* GL_SGIS_texture_lod */

#ifdef GL_SGIS_texture_select

#endif /* GL_SGIS_texture_select */

#ifdef GL_SGIX_async

static GLboolean _glewInit_GL_SGIX_async (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAsyncMarkerSGIX = (PFNGLASYNCMARKERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAsyncMarkerSGIX"))) == NULL) || r;
  r = ((glDeleteAsyncMarkersSGIX = (PFNGLDELETEASYNCMARKERSSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glDeleteAsyncMarkersSGIX"))) == NULL) || r;
  r = ((glFinishAsyncSGIX = (PFNGLFINISHASYNCSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFinishAsyncSGIX"))) == NULL) || r;
  r = ((glGenAsyncMarkersSGIX = (PFNGLGENASYNCMARKERSSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGenAsyncMarkersSGIX"))) == NULL) || r;
  r = ((glIsAsyncMarkerSGIX = (PFNGLISASYNCMARKERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glIsAsyncMarkerSGIX"))) == NULL) || r;
  r = ((glPollAsyncSGIX = (PFNGLPOLLASYNCSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPollAsyncSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_async */

#ifdef GL_SGIX_async_histogram

#endif /* GL_SGIX_async_histogram */

#ifdef GL_SGIX_async_pixel

#endif /* GL_SGIX_async_pixel */

#ifdef GL_SGIX_blend_alpha_minmax

#endif /* GL_SGIX_blend_alpha_minmax */

#ifdef GL_SGIX_clipmap

#endif /* GL_SGIX_clipmap */

#ifdef GL_SGIX_convolution_accuracy

#endif /* GL_SGIX_convolution_accuracy */

#ifdef GL_SGIX_depth_texture

#endif /* GL_SGIX_depth_texture */

#ifdef GL_SGIX_flush_raster

static GLboolean _glewInit_GL_SGIX_flush_raster (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFlushRasterSGIX = (PFNGLFLUSHRASTERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFlushRasterSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_flush_raster */

#ifdef GL_SGIX_fog_offset

#endif /* GL_SGIX_fog_offset */

#ifdef GL_SGIX_fog_texture

static GLboolean _glewInit_GL_SGIX_fog_texture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTextureFogSGIX = (PFNGLTEXTUREFOGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTextureFogSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_fog_texture */

#ifdef GL_SGIX_fragment_specular_lighting

static GLboolean _glewInit_GL_SGIX_fragment_specular_lighting (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFragmentColorMaterialSGIX = (PFNGLFRAGMENTCOLORMATERIALSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentColorMaterialSGIX"))) == NULL) || r;
  r = ((glFragmentLightModelfSGIX = (PFNGLFRAGMENTLIGHTMODELFSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModelfSGIX"))) == NULL) || r;
  r = ((glFragmentLightModelfvSGIX = (PFNGLFRAGMENTLIGHTMODELFVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModelfvSGIX"))) == NULL) || r;
  r = ((glFragmentLightModeliSGIX = (PFNGLFRAGMENTLIGHTMODELISGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModeliSGIX"))) == NULL) || r;
  r = ((glFragmentLightModelivSGIX = (PFNGLFRAGMENTLIGHTMODELIVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightModelivSGIX"))) == NULL) || r;
  r = ((glFragmentLightfSGIX = (PFNGLFRAGMENTLIGHTFSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightfSGIX"))) == NULL) || r;
  r = ((glFragmentLightfvSGIX = (PFNGLFRAGMENTLIGHTFVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightfvSGIX"))) == NULL) || r;
  r = ((glFragmentLightiSGIX = (PFNGLFRAGMENTLIGHTISGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightiSGIX"))) == NULL) || r;
  r = ((glFragmentLightivSGIX = (PFNGLFRAGMENTLIGHTIVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentLightivSGIX"))) == NULL) || r;
  r = ((glFragmentMaterialfSGIX = (PFNGLFRAGMENTMATERIALFSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialfSGIX"))) == NULL) || r;
  r = ((glFragmentMaterialfvSGIX = (PFNGLFRAGMENTMATERIALFVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialfvSGIX"))) == NULL) || r;
  r = ((glFragmentMaterialiSGIX = (PFNGLFRAGMENTMATERIALISGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialiSGIX"))) == NULL) || r;
  r = ((glFragmentMaterialivSGIX = (PFNGLFRAGMENTMATERIALIVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFragmentMaterialivSGIX"))) == NULL) || r;
  r = ((glGetFragmentLightfvSGIX = (PFNGLGETFRAGMENTLIGHTFVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentLightfvSGIX"))) == NULL) || r;
  r = ((glGetFragmentLightivSGIX = (PFNGLGETFRAGMENTLIGHTIVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentLightivSGIX"))) == NULL) || r;
  r = ((glGetFragmentMaterialfvSGIX = (PFNGLGETFRAGMENTMATERIALFVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentMaterialfvSGIX"))) == NULL) || r;
  r = ((glGetFragmentMaterialivSGIX = (PFNGLGETFRAGMENTMATERIALIVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetFragmentMaterialivSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_fragment_specular_lighting */

#ifdef GL_SGIX_framezoom

static GLboolean _glewInit_GL_SGIX_framezoom (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFrameZoomSGIX = (PFNGLFRAMEZOOMSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFrameZoomSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_framezoom */

#ifdef GL_SGIX_interlace

#endif /* GL_SGIX_interlace */

#ifdef GL_SGIX_ir_instrument1

#endif /* GL_SGIX_ir_instrument1 */

#ifdef GL_SGIX_list_priority

#endif /* GL_SGIX_list_priority */

#ifdef GL_SGIX_pixel_texture

static GLboolean _glewInit_GL_SGIX_pixel_texture (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glPixelTexGenSGIX = (PFNGLPIXELTEXGENSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glPixelTexGenSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_pixel_texture */

#ifdef GL_SGIX_pixel_texture_bits

#endif /* GL_SGIX_pixel_texture_bits */

#ifdef GL_SGIX_reference_plane

static GLboolean _glewInit_GL_SGIX_reference_plane (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glReferencePlaneSGIX = (PFNGLREFERENCEPLANESGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReferencePlaneSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_reference_plane */

#ifdef GL_SGIX_resample

#endif /* GL_SGIX_resample */

#ifdef GL_SGIX_shadow

#endif /* GL_SGIX_shadow */

#ifdef GL_SGIX_shadow_ambient

#endif /* GL_SGIX_shadow_ambient */

#ifdef GL_SGIX_sprite

static GLboolean _glewInit_GL_SGIX_sprite (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glSpriteParameterfSGIX = (PFNGLSPRITEPARAMETERFSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSpriteParameterfSGIX"))) == NULL) || r;
  r = ((glSpriteParameterfvSGIX = (PFNGLSPRITEPARAMETERFVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSpriteParameterfvSGIX"))) == NULL) || r;
  r = ((glSpriteParameteriSGIX = (PFNGLSPRITEPARAMETERISGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSpriteParameteriSGIX"))) == NULL) || r;
  r = ((glSpriteParameterivSGIX = (PFNGLSPRITEPARAMETERIVSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glSpriteParameterivSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_sprite */

#ifdef GL_SGIX_tag_sample_buffer

static GLboolean _glewInit_GL_SGIX_tag_sample_buffer (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glTagSampleBufferSGIX = (PFNGLTAGSAMPLEBUFFERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTagSampleBufferSGIX"))) == NULL) || r;

  return r;
}

#endif /* GL_SGIX_tag_sample_buffer */

#ifdef GL_SGIX_texture_add_env

#endif /* GL_SGIX_texture_add_env */

#ifdef GL_SGIX_texture_coordinate_clamp

#endif /* GL_SGIX_texture_coordinate_clamp */

#ifdef GL_SGIX_texture_lod_bias

#endif /* GL_SGIX_texture_lod_bias */

#ifdef GL_SGIX_texture_multi_buffer

#endif /* GL_SGIX_texture_multi_buffer */

#ifdef GL_SGIX_texture_range

#endif /* GL_SGIX_texture_range */

#ifdef GL_SGIX_texture_scale_bias

#endif /* GL_SGIX_texture_scale_bias */

#ifdef GL_SGIX_vertex_preclip

#endif /* GL_SGIX_vertex_preclip */

#ifdef GL_SGIX_vertex_preclip_hint

#endif /* GL_SGIX_vertex_preclip_hint */

#ifdef GL_SGIX_ycrcb

#endif /* GL_SGIX_ycrcb */

#ifdef GL_SGI_color_matrix

#endif /* GL_SGI_color_matrix */

#ifdef GL_SGI_color_table

static GLboolean _glewInit_GL_SGI_color_table (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColorTableParameterfvSGI = (PFNGLCOLORTABLEPARAMETERFVSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorTableParameterfvSGI"))) == NULL) || r;
  r = ((glColorTableParameterivSGI = (PFNGLCOLORTABLEPARAMETERIVSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorTableParameterivSGI"))) == NULL) || r;
  r = ((glColorTableSGI = (PFNGLCOLORTABLESGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColorTableSGI"))) == NULL) || r;
  r = ((glCopyColorTableSGI = (PFNGLCOPYCOLORTABLESGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glCopyColorTableSGI"))) == NULL) || r;
  r = ((glGetColorTableParameterfvSGI = (PFNGLGETCOLORTABLEPARAMETERFVSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableParameterfvSGI"))) == NULL) || r;
  r = ((glGetColorTableParameterivSGI = (PFNGLGETCOLORTABLEPARAMETERIVSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableParameterivSGI"))) == NULL) || r;
  r = ((glGetColorTableSGI = (PFNGLGETCOLORTABLESGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGetColorTableSGI"))) == NULL) || r;

  return r;
}

#endif /* GL_SGI_color_table */

#ifdef GL_SGI_texture_color_table

#endif /* GL_SGI_texture_color_table */

#ifdef GL_SUNX_constant_data

static GLboolean _glewInit_GL_SUNX_constant_data (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glFinishTextureSUNX = (PFNGLFINISHTEXTURESUNXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glFinishTextureSUNX"))) == NULL) || r;

  return r;
}

#endif /* GL_SUNX_constant_data */

#ifdef GL_SUN_convolution_border_modes

#endif /* GL_SUN_convolution_border_modes */

#ifdef GL_SUN_global_alpha

static GLboolean _glewInit_GL_SUN_global_alpha (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glGlobalAlphaFactorbSUN = (PFNGLGLOBALALPHAFACTORBSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactorbSUN"))) == NULL) || r;
  r = ((glGlobalAlphaFactordSUN = (PFNGLGLOBALALPHAFACTORDSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactordSUN"))) == NULL) || r;
  r = ((glGlobalAlphaFactorfSUN = (PFNGLGLOBALALPHAFACTORFSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactorfSUN"))) == NULL) || r;
  r = ((glGlobalAlphaFactoriSUN = (PFNGLGLOBALALPHAFACTORISUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactoriSUN"))) == NULL) || r;
  r = ((glGlobalAlphaFactorsSUN = (PFNGLGLOBALALPHAFACTORSSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactorsSUN"))) == NULL) || r;
  r = ((glGlobalAlphaFactorubSUN = (PFNGLGLOBALALPHAFACTORUBSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactorubSUN"))) == NULL) || r;
  r = ((glGlobalAlphaFactoruiSUN = (PFNGLGLOBALALPHAFACTORUISUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactoruiSUN"))) == NULL) || r;
  r = ((glGlobalAlphaFactorusSUN = (PFNGLGLOBALALPHAFACTORUSSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glGlobalAlphaFactorusSUN"))) == NULL) || r;

  return r;
}

#endif /* GL_SUN_global_alpha */

#ifdef GL_SUN_mesh_array

#endif /* GL_SUN_mesh_array */

#ifdef GL_SUN_read_video_pixels

static GLboolean _glewInit_GL_SUN_read_video_pixels (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glReadVideoPixelsSUN = (PFNGLREADVIDEOPIXELSSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReadVideoPixelsSUN"))) == NULL) || r;

  return r;
}

#endif /* GL_SUN_read_video_pixels */

#ifdef GL_SUN_slice_accum

#endif /* GL_SUN_slice_accum */

#ifdef GL_SUN_triangle_list

static GLboolean _glewInit_GL_SUN_triangle_list (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glReplacementCodePointerSUN = (PFNGLREPLACEMENTCODEPOINTERSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodePointerSUN"))) == NULL) || r;
  r = ((glReplacementCodeubSUN = (PFNGLREPLACEMENTCODEUBSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeubSUN"))) == NULL) || r;
  r = ((glReplacementCodeubvSUN = (PFNGLREPLACEMENTCODEUBVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeubvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiSUN = (PFNGLREPLACEMENTCODEUISUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiSUN"))) == NULL) || r;
  r = ((glReplacementCodeuivSUN = (PFNGLREPLACEMENTCODEUIVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuivSUN"))) == NULL) || r;
  r = ((glReplacementCodeusSUN = (PFNGLREPLACEMENTCODEUSSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeusSUN"))) == NULL) || r;
  r = ((glReplacementCodeusvSUN = (PFNGLREPLACEMENTCODEUSVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeusvSUN"))) == NULL) || r;

  return r;
}

#endif /* GL_SUN_triangle_list */

#ifdef GL_SUN_vertex

static GLboolean _glewInit_GL_SUN_vertex (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glColor3fVertex3fSUN = (PFNGLCOLOR3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3fVertex3fSUN"))) == NULL) || r;
  r = ((glColor3fVertex3fvSUN = (PFNGLCOLOR3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor3fVertex3fvSUN"))) == NULL) || r;
  r = ((glColor4fNormal3fVertex3fSUN = (PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4fNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glColor4fNormal3fVertex3fvSUN = (PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4fNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glColor4ubVertex2fSUN = (PFNGLCOLOR4UBVERTEX2FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4ubVertex2fSUN"))) == NULL) || r;
  r = ((glColor4ubVertex2fvSUN = (PFNGLCOLOR4UBVERTEX2FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4ubVertex2fvSUN"))) == NULL) || r;
  r = ((glColor4ubVertex3fSUN = (PFNGLCOLOR4UBVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4ubVertex3fSUN"))) == NULL) || r;
  r = ((glColor4ubVertex3fvSUN = (PFNGLCOLOR4UBVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glColor4ubVertex3fvSUN"))) == NULL) || r;
  r = ((glNormal3fVertex3fSUN = (PFNGLNORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glNormal3fVertex3fvSUN = (PFNGLNORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiColor3fVertex3fSUN = (PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiColor3fVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiColor3fVertex3fvSUN = (PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiColor3fVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiColor4fNormal3fVertex3fSUN = (PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiColor4fNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiColor4fNormal3fVertex3fvSUN = (PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiColor4fNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiColor4ubVertex3fSUN = (PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiColor4ubVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiColor4ubVertex3fvSUN = (PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiColor4ubVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiNormal3fVertex3fSUN = (PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiNormal3fVertex3fvSUN = (PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = (PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = (PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = (PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = (PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiTexCoord2fVertex3fSUN = (PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiTexCoord2fVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiTexCoord2fVertex3fvSUN = (PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiTexCoord2fVertex3fvSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiVertex3fSUN = (PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiVertex3fSUN"))) == NULL) || r;
  r = ((glReplacementCodeuiVertex3fvSUN = (PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glReplacementCodeuiVertex3fvSUN"))) == NULL) || r;
  r = ((glTexCoord2fColor3fVertex3fSUN = (PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fColor3fVertex3fSUN"))) == NULL) || r;
  r = ((glTexCoord2fColor3fVertex3fvSUN = (PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fColor3fVertex3fvSUN"))) == NULL) || r;
  r = ((glTexCoord2fColor4fNormal3fVertex3fSUN = (PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fColor4fNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glTexCoord2fColor4fNormal3fVertex3fvSUN = (PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fColor4fNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glTexCoord2fColor4ubVertex3fSUN = (PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fColor4ubVertex3fSUN"))) == NULL) || r;
  r = ((glTexCoord2fColor4ubVertex3fvSUN = (PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fColor4ubVertex3fvSUN"))) == NULL) || r;
  r = ((glTexCoord2fNormal3fVertex3fSUN = (PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fNormal3fVertex3fSUN"))) == NULL) || r;
  r = ((glTexCoord2fNormal3fVertex3fvSUN = (PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fNormal3fVertex3fvSUN"))) == NULL) || r;
  r = ((glTexCoord2fVertex3fSUN = (PFNGLTEXCOORD2FVERTEX3FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fVertex3fSUN"))) == NULL) || r;
  r = ((glTexCoord2fVertex3fvSUN = (PFNGLTEXCOORD2FVERTEX3FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord2fVertex3fvSUN"))) == NULL) || r;
  r = ((glTexCoord4fColor4fNormal3fVertex4fSUN = (PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4fColor4fNormal3fVertex4fSUN"))) == NULL) || r;
  r = ((glTexCoord4fColor4fNormal3fVertex4fvSUN = (PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4fColor4fNormal3fVertex4fvSUN"))) == NULL) || r;
  r = ((glTexCoord4fVertex4fSUN = (PFNGLTEXCOORD4FVERTEX4FSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4fVertex4fSUN"))) == NULL) || r;
  r = ((glTexCoord4fVertex4fvSUN = (PFNGLTEXCOORD4FVERTEX4FVSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glTexCoord4fVertex4fvSUN"))) == NULL) || r;

  return r;
}

#endif /* GL_SUN_vertex */

#ifdef GL_WIN_phong_shading

#endif /* GL_WIN_phong_shading */

#ifdef GL_WIN_specular_fog

#endif /* GL_WIN_specular_fog */

#ifdef GL_WIN_swap_hint

static GLboolean _glewInit_GL_WIN_swap_hint (GLEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glAddSwapHintRectWIN = (PFNGLADDSWAPHINTRECTWINPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glAddSwapHintRectWIN"))) == NULL) || r;

  return r;
}

#endif /* GL_WIN_swap_hint */

/* ------------------------------------------------------------------------- */

/* 
 * Search for name in the extensions string. Use of strstr()
 * is not sufficient because extension names can be prefixes of
 * other extension names. Could use strtok() but the constant
 * string returned by glGetString might be in read-only memory.
 */
GLboolean glewGetExtension (const char* name)
{    
  GLubyte* p;
  GLubyte* end;
  GLuint len = _glewStrLen((const GLubyte*)name);
  p = (GLubyte*)glGetString(GL_EXTENSIONS);
  if (0 == p) return GL_FALSE;
  end = p + _glewStrLen(p);
  while (p < end)
  {
    GLuint n = _glewStrCLen(p, ' ');
    if (len == n && _glewStrSame((const GLubyte*)name, p, n)) return GL_TRUE;
    p += n+1;
  }
  return GL_FALSE;
}

/* ------------------------------------------------------------------------- */

#ifndef GLEW_MX
static
#endif
GLenum glewContextInit (GLEW_CONTEXT_ARG_DEF_LIST)
{
  const GLubyte* s;
  GLuint dot;
  GLint major, minor;

  /* Since we're still initializing, the `GetString' function pointer hasn't
   * been loaded yet. */
  glGetString = (PFNGLGETSTRINGPROC) glewGetProcAddress(
                      fqn_from_convention(glew_convention, "glGetString")
                    );
  /* query opengl version */
  s = glGetString(GL_VERSION);

  dot = _glewStrCLen(s, '.');
  if (dot == 0)
    return GLEW_ERROR_NO_GL_VERSION;
  
  major = s[dot-1]-'0';
  minor = s[dot+1]-'0';

  if (minor < 0 || minor > 9)
    minor = 0;
  if (major<0 || major>9)
    return GLEW_ERROR_NO_GL_VERSION;
  

  if (major == 1 && minor == 0)
  {
    return GLEW_ERROR_GL_VERSION_10_ONLY;
  }
  else
  {
    CONST_CAST(GLEW_VERSION_4_1)   = ( major > 4 )                 || ( major == 4 && minor >= 1 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_4_0)   = GLEW_VERSION_4_1   == GL_TRUE || ( major == 4               ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_3_3)   = GLEW_VERSION_4_0   == GL_TRUE || ( major == 3 && minor >= 3 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_3_2)   = GLEW_VERSION_3_3   == GL_TRUE || ( major == 3 && minor >= 2 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_3_1)   = GLEW_VERSION_3_2   == GL_TRUE || ( major == 3 && minor >= 1 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_3_0)   = GLEW_VERSION_3_1   == GL_TRUE || ( major == 3               ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_2_1)   = GLEW_VERSION_3_0   == GL_TRUE || ( major == 2 && minor >= 1 ) ? GL_TRUE : GL_FALSE;    
    CONST_CAST(GLEW_VERSION_2_0)   = GLEW_VERSION_2_1   == GL_TRUE || ( major == 2               ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_1_5)   = GLEW_VERSION_2_0   == GL_TRUE || ( major == 1 && minor >= 5 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_1_4)   = GLEW_VERSION_1_5   == GL_TRUE || ( major == 1 && minor >= 4 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_1_3)   = GLEW_VERSION_1_4   == GL_TRUE || ( major == 1 && minor >= 3 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_1_2_1) = GLEW_VERSION_1_3   == GL_TRUE                                 ? GL_TRUE : GL_FALSE; 
    CONST_CAST(GLEW_VERSION_1_2)   = GLEW_VERSION_1_2_1 == GL_TRUE || ( major == 1 && minor >= 2 ) ? GL_TRUE : GL_FALSE;
    CONST_CAST(GLEW_VERSION_1_1)   = GLEW_VERSION_1_2   == GL_TRUE || ( major == 1 && minor >= 1 ) ? GL_TRUE : GL_FALSE;
  }
  /* initialize extensions */
#ifdef GL_VERSION_1_1
  if (glewExperimental || GLEW_VERSION_1_1) CONST_CAST(GLEW_VERSION_1_1) = !_glewInit_GL_VERSION_1_1(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_1_1 */
#ifdef GL_VERSION_1_2
  if (glewExperimental || GLEW_VERSION_1_2) CONST_CAST(GLEW_VERSION_1_2) = !_glewInit_GL_VERSION_1_2(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_1_2 */
#ifdef GL_VERSION_1_2_1
#endif /* GL_VERSION_1_2_1 */
#ifdef GL_VERSION_1_3
  if (glewExperimental || GLEW_VERSION_1_3) CONST_CAST(GLEW_VERSION_1_3) = !_glewInit_GL_VERSION_1_3(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_1_3 */
#ifdef GL_VERSION_1_4
  if (glewExperimental || GLEW_VERSION_1_4) CONST_CAST(GLEW_VERSION_1_4) = !_glewInit_GL_VERSION_1_4(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_1_4 */
#ifdef GL_VERSION_1_5
  if (glewExperimental || GLEW_VERSION_1_5) CONST_CAST(GLEW_VERSION_1_5) = !_glewInit_GL_VERSION_1_5(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_1_5 */
#ifdef GL_VERSION_2_0
  if (glewExperimental || GLEW_VERSION_2_0) CONST_CAST(GLEW_VERSION_2_0) = !_glewInit_GL_VERSION_2_0(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_2_0 */
#ifdef GL_VERSION_2_1
  if (glewExperimental || GLEW_VERSION_2_1) CONST_CAST(GLEW_VERSION_2_1) = !_glewInit_GL_VERSION_2_1(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_2_1 */
#ifdef GL_VERSION_3_0
  if (glewExperimental || GLEW_VERSION_3_0) CONST_CAST(GLEW_VERSION_3_0) = !_glewInit_GL_VERSION_3_0(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_3_0 */
#ifdef GL_VERSION_3_1
  if (glewExperimental || GLEW_VERSION_3_1) CONST_CAST(GLEW_VERSION_3_1) = !_glewInit_GL_VERSION_3_1(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_3_1 */
#ifdef GL_VERSION_3_2
  if (glewExperimental || GLEW_VERSION_3_2) CONST_CAST(GLEW_VERSION_3_2) = !_glewInit_GL_VERSION_3_2(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_3_2 */
#ifdef GL_VERSION_3_3
  if (glewExperimental || GLEW_VERSION_3_3) CONST_CAST(GLEW_VERSION_3_3) = !_glewInit_GL_VERSION_3_3(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_3_3 */
#ifdef GL_VERSION_4_0
  if (glewExperimental || GLEW_VERSION_4_0) CONST_CAST(GLEW_VERSION_4_0) = !_glewInit_GL_VERSION_4_0(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_VERSION_4_0 */
#ifdef GL_VERSION_4_1
#endif /* GL_VERSION_4_1 */
#ifdef GL_3DFX_multisample
  CONST_CAST(GLEW_3DFX_multisample) = glewGetExtension("GL_3DFX_multisample");
#endif /* GL_3DFX_multisample */
#ifdef GL_3DFX_tbuffer
  CONST_CAST(GLEW_3DFX_tbuffer) = glewGetExtension("GL_3DFX_tbuffer");
  if (glewExperimental || GLEW_3DFX_tbuffer) CONST_CAST(GLEW_3DFX_tbuffer) = !_glewInit_GL_3DFX_tbuffer(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_3DFX_tbuffer */
#ifdef GL_3DFX_texture_compression_FXT1
  CONST_CAST(GLEW_3DFX_texture_compression_FXT1) = glewGetExtension("GL_3DFX_texture_compression_FXT1");
#endif /* GL_3DFX_texture_compression_FXT1 */
#ifdef GL_AMD_conservative_depth
  CONST_CAST(GLEW_AMD_conservative_depth) = glewGetExtension("GL_AMD_conservative_depth");
#endif /* GL_AMD_conservative_depth */
#ifdef GL_AMD_debug_output
  CONST_CAST(GLEW_AMD_debug_output) = glewGetExtension("GL_AMD_debug_output");
  if (glewExperimental || GLEW_AMD_debug_output) CONST_CAST(GLEW_AMD_debug_output) = !_glewInit_GL_AMD_debug_output(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_AMD_debug_output */
#ifdef GL_AMD_depth_clamp_separate
  CONST_CAST(GLEW_AMD_depth_clamp_separate) = glewGetExtension("GL_AMD_depth_clamp_separate");
#endif /* GL_AMD_depth_clamp_separate */
#ifdef GL_AMD_draw_buffers_blend
  CONST_CAST(GLEW_AMD_draw_buffers_blend) = glewGetExtension("GL_AMD_draw_buffers_blend");
  if (glewExperimental || GLEW_AMD_draw_buffers_blend) CONST_CAST(GLEW_AMD_draw_buffers_blend) = !_glewInit_GL_AMD_draw_buffers_blend(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_AMD_draw_buffers_blend */
#ifdef GL_AMD_name_gen_delete
  CONST_CAST(GLEW_AMD_name_gen_delete) = glewGetExtension("GL_AMD_name_gen_delete");
  if (glewExperimental || GLEW_AMD_name_gen_delete) CONST_CAST(GLEW_AMD_name_gen_delete) = !_glewInit_GL_AMD_name_gen_delete(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_AMD_name_gen_delete */
#ifdef GL_AMD_performance_monitor
  CONST_CAST(GLEW_AMD_performance_monitor) = glewGetExtension("GL_AMD_performance_monitor");
  if (glewExperimental || GLEW_AMD_performance_monitor) CONST_CAST(GLEW_AMD_performance_monitor) = !_glewInit_GL_AMD_performance_monitor(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_AMD_performance_monitor */
#ifdef GL_AMD_seamless_cubemap_per_texture
  CONST_CAST(GLEW_AMD_seamless_cubemap_per_texture) = glewGetExtension("GL_AMD_seamless_cubemap_per_texture");
#endif /* GL_AMD_seamless_cubemap_per_texture */
#ifdef GL_AMD_shader_stencil_export
  CONST_CAST(GLEW_AMD_shader_stencil_export) = glewGetExtension("GL_AMD_shader_stencil_export");
#endif /* GL_AMD_shader_stencil_export */
#ifdef GL_AMD_texture_texture4
  CONST_CAST(GLEW_AMD_texture_texture4) = glewGetExtension("GL_AMD_texture_texture4");
#endif /* GL_AMD_texture_texture4 */
#ifdef GL_AMD_transform_feedback3_lines_triangles
  CONST_CAST(GLEW_AMD_transform_feedback3_lines_triangles) = glewGetExtension("GL_AMD_transform_feedback3_lines_triangles");
#endif /* GL_AMD_transform_feedback3_lines_triangles */
#ifdef GL_AMD_vertex_shader_tessellator
  CONST_CAST(GLEW_AMD_vertex_shader_tessellator) = glewGetExtension("GL_AMD_vertex_shader_tessellator");
  if (glewExperimental || GLEW_AMD_vertex_shader_tessellator) CONST_CAST(GLEW_AMD_vertex_shader_tessellator) = !_glewInit_GL_AMD_vertex_shader_tessellator(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_AMD_vertex_shader_tessellator */
#ifdef GL_APPLE_aux_depth_stencil
  CONST_CAST(GLEW_APPLE_aux_depth_stencil) = glewGetExtension("GL_APPLE_aux_depth_stencil");
#endif /* GL_APPLE_aux_depth_stencil */
#ifdef GL_APPLE_client_storage
  CONST_CAST(GLEW_APPLE_client_storage) = glewGetExtension("GL_APPLE_client_storage");
#endif /* GL_APPLE_client_storage */
#ifdef GL_APPLE_element_array
  CONST_CAST(GLEW_APPLE_element_array) = glewGetExtension("GL_APPLE_element_array");
  if (glewExperimental || GLEW_APPLE_element_array) CONST_CAST(GLEW_APPLE_element_array) = !_glewInit_GL_APPLE_element_array(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_element_array */
#ifdef GL_APPLE_fence
  CONST_CAST(GLEW_APPLE_fence) = glewGetExtension("GL_APPLE_fence");
  if (glewExperimental || GLEW_APPLE_fence) CONST_CAST(GLEW_APPLE_fence) = !_glewInit_GL_APPLE_fence(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_fence */
#ifdef GL_APPLE_float_pixels
  CONST_CAST(GLEW_APPLE_float_pixels) = glewGetExtension("GL_APPLE_float_pixels");
#endif /* GL_APPLE_float_pixels */
#ifdef GL_APPLE_flush_buffer_range
  CONST_CAST(GLEW_APPLE_flush_buffer_range) = glewGetExtension("GL_APPLE_flush_buffer_range");
  if (glewExperimental || GLEW_APPLE_flush_buffer_range) CONST_CAST(GLEW_APPLE_flush_buffer_range) = !_glewInit_GL_APPLE_flush_buffer_range(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_flush_buffer_range */
#ifdef GL_APPLE_object_purgeable
  CONST_CAST(GLEW_APPLE_object_purgeable) = glewGetExtension("GL_APPLE_object_purgeable");
  if (glewExperimental || GLEW_APPLE_object_purgeable) CONST_CAST(GLEW_APPLE_object_purgeable) = !_glewInit_GL_APPLE_object_purgeable(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_object_purgeable */
#ifdef GL_APPLE_pixel_buffer
  CONST_CAST(GLEW_APPLE_pixel_buffer) = glewGetExtension("GL_APPLE_pixel_buffer");
#endif /* GL_APPLE_pixel_buffer */
#ifdef GL_APPLE_rgb_422
  CONST_CAST(GLEW_APPLE_rgb_422) = glewGetExtension("GL_APPLE_rgb_422");
#endif /* GL_APPLE_rgb_422 */
#ifdef GL_APPLE_row_bytes
  CONST_CAST(GLEW_APPLE_row_bytes) = glewGetExtension("GL_APPLE_row_bytes");
#endif /* GL_APPLE_row_bytes */
#ifdef GL_APPLE_specular_vector
  CONST_CAST(GLEW_APPLE_specular_vector) = glewGetExtension("GL_APPLE_specular_vector");
#endif /* GL_APPLE_specular_vector */
#ifdef GL_APPLE_texture_range
  CONST_CAST(GLEW_APPLE_texture_range) = glewGetExtension("GL_APPLE_texture_range");
  if (glewExperimental || GLEW_APPLE_texture_range) CONST_CAST(GLEW_APPLE_texture_range) = !_glewInit_GL_APPLE_texture_range(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_texture_range */
#ifdef GL_APPLE_transform_hint
  CONST_CAST(GLEW_APPLE_transform_hint) = glewGetExtension("GL_APPLE_transform_hint");
#endif /* GL_APPLE_transform_hint */
#ifdef GL_APPLE_vertex_array_object
  CONST_CAST(GLEW_APPLE_vertex_array_object) = glewGetExtension("GL_APPLE_vertex_array_object");
  if (glewExperimental || GLEW_APPLE_vertex_array_object) CONST_CAST(GLEW_APPLE_vertex_array_object) = !_glewInit_GL_APPLE_vertex_array_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_vertex_array_object */
#ifdef GL_APPLE_vertex_array_range
  CONST_CAST(GLEW_APPLE_vertex_array_range) = glewGetExtension("GL_APPLE_vertex_array_range");
  if (glewExperimental || GLEW_APPLE_vertex_array_range) CONST_CAST(GLEW_APPLE_vertex_array_range) = !_glewInit_GL_APPLE_vertex_array_range(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_vertex_array_range */
#ifdef GL_APPLE_vertex_program_evaluators
  CONST_CAST(GLEW_APPLE_vertex_program_evaluators) = glewGetExtension("GL_APPLE_vertex_program_evaluators");
  if (glewExperimental || GLEW_APPLE_vertex_program_evaluators) CONST_CAST(GLEW_APPLE_vertex_program_evaluators) = !_glewInit_GL_APPLE_vertex_program_evaluators(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_APPLE_vertex_program_evaluators */
#ifdef GL_APPLE_ycbcr_422
  CONST_CAST(GLEW_APPLE_ycbcr_422) = glewGetExtension("GL_APPLE_ycbcr_422");
#endif /* GL_APPLE_ycbcr_422 */
#ifdef GL_ARB_ES2_compatibility
  CONST_CAST(GLEW_ARB_ES2_compatibility) = glewGetExtension("GL_ARB_ES2_compatibility");
  if (glewExperimental || GLEW_ARB_ES2_compatibility) CONST_CAST(GLEW_ARB_ES2_compatibility) = !_glewInit_GL_ARB_ES2_compatibility(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_ES2_compatibility */
#ifdef GL_ARB_blend_func_extended
  CONST_CAST(GLEW_ARB_blend_func_extended) = glewGetExtension("GL_ARB_blend_func_extended");
  if (glewExperimental || GLEW_ARB_blend_func_extended) CONST_CAST(GLEW_ARB_blend_func_extended) = !_glewInit_GL_ARB_blend_func_extended(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_blend_func_extended */
#ifdef GL_ARB_cl_event
  CONST_CAST(GLEW_ARB_cl_event) = glewGetExtension("GL_ARB_cl_event");
  if (glewExperimental || GLEW_ARB_cl_event) CONST_CAST(GLEW_ARB_cl_event) = !_glewInit_GL_ARB_cl_event(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_cl_event */
#ifdef GL_ARB_color_buffer_float
  CONST_CAST(GLEW_ARB_color_buffer_float) = glewGetExtension("GL_ARB_color_buffer_float");
  if (glewExperimental || GLEW_ARB_color_buffer_float) CONST_CAST(GLEW_ARB_color_buffer_float) = !_glewInit_GL_ARB_color_buffer_float(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_color_buffer_float */
#ifdef GL_ARB_compatibility
  CONST_CAST(GLEW_ARB_compatibility) = glewGetExtension("GL_ARB_compatibility");
#endif /* GL_ARB_compatibility */
#ifdef GL_ARB_copy_buffer
  CONST_CAST(GLEW_ARB_copy_buffer) = glewGetExtension("GL_ARB_copy_buffer");
  if (glewExperimental || GLEW_ARB_copy_buffer) CONST_CAST(GLEW_ARB_copy_buffer) = !_glewInit_GL_ARB_copy_buffer(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_copy_buffer */
#ifdef GL_ARB_debug_output
  CONST_CAST(GLEW_ARB_debug_output) = glewGetExtension("GL_ARB_debug_output");
  if (glewExperimental || GLEW_ARB_debug_output) CONST_CAST(GLEW_ARB_debug_output) = !_glewInit_GL_ARB_debug_output(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_debug_output */
#ifdef GL_ARB_depth_buffer_float
  CONST_CAST(GLEW_ARB_depth_buffer_float) = glewGetExtension("GL_ARB_depth_buffer_float");
#endif /* GL_ARB_depth_buffer_float */
#ifdef GL_ARB_depth_clamp
  CONST_CAST(GLEW_ARB_depth_clamp) = glewGetExtension("GL_ARB_depth_clamp");
#endif /* GL_ARB_depth_clamp */
#ifdef GL_ARB_depth_texture
  CONST_CAST(GLEW_ARB_depth_texture) = glewGetExtension("GL_ARB_depth_texture");
#endif /* GL_ARB_depth_texture */
#ifdef GL_ARB_draw_buffers
  CONST_CAST(GLEW_ARB_draw_buffers) = glewGetExtension("GL_ARB_draw_buffers");
  if (glewExperimental || GLEW_ARB_draw_buffers) CONST_CAST(GLEW_ARB_draw_buffers) = !_glewInit_GL_ARB_draw_buffers(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_draw_buffers */
#ifdef GL_ARB_draw_buffers_blend
  CONST_CAST(GLEW_ARB_draw_buffers_blend) = glewGetExtension("GL_ARB_draw_buffers_blend");
  if (glewExperimental || GLEW_ARB_draw_buffers_blend) CONST_CAST(GLEW_ARB_draw_buffers_blend) = !_glewInit_GL_ARB_draw_buffers_blend(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_draw_buffers_blend */
#ifdef GL_ARB_draw_elements_base_vertex
  CONST_CAST(GLEW_ARB_draw_elements_base_vertex) = glewGetExtension("GL_ARB_draw_elements_base_vertex");
  if (glewExperimental || GLEW_ARB_draw_elements_base_vertex) CONST_CAST(GLEW_ARB_draw_elements_base_vertex) = !_glewInit_GL_ARB_draw_elements_base_vertex(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_draw_elements_base_vertex */
#ifdef GL_ARB_draw_indirect
  CONST_CAST(GLEW_ARB_draw_indirect) = glewGetExtension("GL_ARB_draw_indirect");
  if (glewExperimental || GLEW_ARB_draw_indirect) CONST_CAST(GLEW_ARB_draw_indirect) = !_glewInit_GL_ARB_draw_indirect(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_draw_indirect */
#ifdef GL_ARB_draw_instanced
  CONST_CAST(GLEW_ARB_draw_instanced) = glewGetExtension("GL_ARB_draw_instanced");
  if (glewExperimental || GLEW_ARB_draw_instanced) CONST_CAST(GLEW_ARB_draw_instanced) = !_glewInit_GL_ARB_draw_instanced(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_draw_instanced */
#ifdef GL_ARB_explicit_attrib_location
  CONST_CAST(GLEW_ARB_explicit_attrib_location) = glewGetExtension("GL_ARB_explicit_attrib_location");
#endif /* GL_ARB_explicit_attrib_location */
#ifdef GL_ARB_fragment_coord_conventions
  CONST_CAST(GLEW_ARB_fragment_coord_conventions) = glewGetExtension("GL_ARB_fragment_coord_conventions");
#endif /* GL_ARB_fragment_coord_conventions */
#ifdef GL_ARB_fragment_program
  CONST_CAST(GLEW_ARB_fragment_program) = glewGetExtension("GL_ARB_fragment_program");
#endif /* GL_ARB_fragment_program */
#ifdef GL_ARB_fragment_program_shadow
  CONST_CAST(GLEW_ARB_fragment_program_shadow) = glewGetExtension("GL_ARB_fragment_program_shadow");
#endif /* GL_ARB_fragment_program_shadow */
#ifdef GL_ARB_fragment_shader
  CONST_CAST(GLEW_ARB_fragment_shader) = glewGetExtension("GL_ARB_fragment_shader");
#endif /* GL_ARB_fragment_shader */
#ifdef GL_ARB_framebuffer_object
  CONST_CAST(GLEW_ARB_framebuffer_object) = glewGetExtension("GL_ARB_framebuffer_object");
  if (glewExperimental || GLEW_ARB_framebuffer_object) CONST_CAST(GLEW_ARB_framebuffer_object) = !_glewInit_GL_ARB_framebuffer_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_framebuffer_object */
#ifdef GL_ARB_framebuffer_sRGB
  CONST_CAST(GLEW_ARB_framebuffer_sRGB) = glewGetExtension("GL_ARB_framebuffer_sRGB");
#endif /* GL_ARB_framebuffer_sRGB */
#ifdef GL_ARB_geometry_shader4
  CONST_CAST(GLEW_ARB_geometry_shader4) = glewGetExtension("GL_ARB_geometry_shader4");
  if (glewExperimental || GLEW_ARB_geometry_shader4) CONST_CAST(GLEW_ARB_geometry_shader4) = !_glewInit_GL_ARB_geometry_shader4(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_geometry_shader4 */
#ifdef GL_ARB_get_program_binary
  CONST_CAST(GLEW_ARB_get_program_binary) = glewGetExtension("GL_ARB_get_program_binary");
  if (glewExperimental || GLEW_ARB_get_program_binary) CONST_CAST(GLEW_ARB_get_program_binary) = !_glewInit_GL_ARB_get_program_binary(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_get_program_binary */
#ifdef GL_ARB_gpu_shader5
  CONST_CAST(GLEW_ARB_gpu_shader5) = glewGetExtension("GL_ARB_gpu_shader5");
#endif /* GL_ARB_gpu_shader5 */
#ifdef GL_ARB_gpu_shader_fp64
  CONST_CAST(GLEW_ARB_gpu_shader_fp64) = glewGetExtension("GL_ARB_gpu_shader_fp64");
  if (glewExperimental || GLEW_ARB_gpu_shader_fp64) CONST_CAST(GLEW_ARB_gpu_shader_fp64) = !_glewInit_GL_ARB_gpu_shader_fp64(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_gpu_shader_fp64 */
#ifdef GL_ARB_half_float_pixel
  CONST_CAST(GLEW_ARB_half_float_pixel) = glewGetExtension("GL_ARB_half_float_pixel");
#endif /* GL_ARB_half_float_pixel */
#ifdef GL_ARB_half_float_vertex
  CONST_CAST(GLEW_ARB_half_float_vertex) = glewGetExtension("GL_ARB_half_float_vertex");
#endif /* GL_ARB_half_float_vertex */
#ifdef GL_ARB_imaging
  CONST_CAST(GLEW_ARB_imaging) = glewGetExtension("GL_ARB_imaging");
  if (glewExperimental || GLEW_ARB_imaging) CONST_CAST(GLEW_ARB_imaging) = !_glewInit_GL_ARB_imaging(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_imaging */
#ifdef GL_ARB_instanced_arrays
  CONST_CAST(GLEW_ARB_instanced_arrays) = glewGetExtension("GL_ARB_instanced_arrays");
  if (glewExperimental || GLEW_ARB_instanced_arrays) CONST_CAST(GLEW_ARB_instanced_arrays) = !_glewInit_GL_ARB_instanced_arrays(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_instanced_arrays */
#ifdef GL_ARB_map_buffer_range
  CONST_CAST(GLEW_ARB_map_buffer_range) = glewGetExtension("GL_ARB_map_buffer_range");
  if (glewExperimental || GLEW_ARB_map_buffer_range) CONST_CAST(GLEW_ARB_map_buffer_range) = !_glewInit_GL_ARB_map_buffer_range(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_map_buffer_range */
#ifdef GL_ARB_matrix_palette
  CONST_CAST(GLEW_ARB_matrix_palette) = glewGetExtension("GL_ARB_matrix_palette");
  if (glewExperimental || GLEW_ARB_matrix_palette) CONST_CAST(GLEW_ARB_matrix_palette) = !_glewInit_GL_ARB_matrix_palette(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_matrix_palette */
#ifdef GL_ARB_multisample
  CONST_CAST(GLEW_ARB_multisample) = glewGetExtension("GL_ARB_multisample");
  if (glewExperimental || GLEW_ARB_multisample) CONST_CAST(GLEW_ARB_multisample) = !_glewInit_GL_ARB_multisample(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_multisample */
#ifdef GL_ARB_multitexture
  CONST_CAST(GLEW_ARB_multitexture) = glewGetExtension("GL_ARB_multitexture");
  if (glewExperimental || GLEW_ARB_multitexture) CONST_CAST(GLEW_ARB_multitexture) = !_glewInit_GL_ARB_multitexture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_multitexture */
#ifdef GL_ARB_occlusion_query
  CONST_CAST(GLEW_ARB_occlusion_query) = glewGetExtension("GL_ARB_occlusion_query");
  if (glewExperimental || GLEW_ARB_occlusion_query) CONST_CAST(GLEW_ARB_occlusion_query) = !_glewInit_GL_ARB_occlusion_query(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_occlusion_query */
#ifdef GL_ARB_occlusion_query2
  CONST_CAST(GLEW_ARB_occlusion_query2) = glewGetExtension("GL_ARB_occlusion_query2");
#endif /* GL_ARB_occlusion_query2 */
#ifdef GL_ARB_pixel_buffer_object
  CONST_CAST(GLEW_ARB_pixel_buffer_object) = glewGetExtension("GL_ARB_pixel_buffer_object");
#endif /* GL_ARB_pixel_buffer_object */
#ifdef GL_ARB_point_parameters
  CONST_CAST(GLEW_ARB_point_parameters) = glewGetExtension("GL_ARB_point_parameters");
  if (glewExperimental || GLEW_ARB_point_parameters) CONST_CAST(GLEW_ARB_point_parameters) = !_glewInit_GL_ARB_point_parameters(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_point_parameters */
#ifdef GL_ARB_point_sprite
  CONST_CAST(GLEW_ARB_point_sprite) = glewGetExtension("GL_ARB_point_sprite");
#endif /* GL_ARB_point_sprite */
#ifdef GL_ARB_provoking_vertex
  CONST_CAST(GLEW_ARB_provoking_vertex) = glewGetExtension("GL_ARB_provoking_vertex");
  if (glewExperimental || GLEW_ARB_provoking_vertex) CONST_CAST(GLEW_ARB_provoking_vertex) = !_glewInit_GL_ARB_provoking_vertex(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_provoking_vertex */
#ifdef GL_ARB_robustness
  CONST_CAST(GLEW_ARB_robustness) = glewGetExtension("GL_ARB_robustness");
  if (glewExperimental || GLEW_ARB_robustness) CONST_CAST(GLEW_ARB_robustness) = !_glewInit_GL_ARB_robustness(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_robustness */
#ifdef GL_ARB_sample_shading
  CONST_CAST(GLEW_ARB_sample_shading) = glewGetExtension("GL_ARB_sample_shading");
  if (glewExperimental || GLEW_ARB_sample_shading) CONST_CAST(GLEW_ARB_sample_shading) = !_glewInit_GL_ARB_sample_shading(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_sample_shading */
#ifdef GL_ARB_sampler_objects
  CONST_CAST(GLEW_ARB_sampler_objects) = glewGetExtension("GL_ARB_sampler_objects");
  if (glewExperimental || GLEW_ARB_sampler_objects) CONST_CAST(GLEW_ARB_sampler_objects) = !_glewInit_GL_ARB_sampler_objects(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_sampler_objects */
#ifdef GL_ARB_seamless_cube_map
  CONST_CAST(GLEW_ARB_seamless_cube_map) = glewGetExtension("GL_ARB_seamless_cube_map");
#endif /* GL_ARB_seamless_cube_map */
#ifdef GL_ARB_separate_shader_objects
  CONST_CAST(GLEW_ARB_separate_shader_objects) = glewGetExtension("GL_ARB_separate_shader_objects");
  if (glewExperimental || GLEW_ARB_separate_shader_objects) CONST_CAST(GLEW_ARB_separate_shader_objects) = !_glewInit_GL_ARB_separate_shader_objects(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_separate_shader_objects */
#ifdef GL_ARB_shader_bit_encoding
  CONST_CAST(GLEW_ARB_shader_bit_encoding) = glewGetExtension("GL_ARB_shader_bit_encoding");
#endif /* GL_ARB_shader_bit_encoding */
#ifdef GL_ARB_shader_objects
  CONST_CAST(GLEW_ARB_shader_objects) = glewGetExtension("GL_ARB_shader_objects");
  if (glewExperimental || GLEW_ARB_shader_objects) CONST_CAST(GLEW_ARB_shader_objects) = !_glewInit_GL_ARB_shader_objects(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_shader_objects */
#ifdef GL_ARB_shader_precision
  CONST_CAST(GLEW_ARB_shader_precision) = glewGetExtension("GL_ARB_shader_precision");
#endif /* GL_ARB_shader_precision */
#ifdef GL_ARB_shader_stencil_export
  CONST_CAST(GLEW_ARB_shader_stencil_export) = glewGetExtension("GL_ARB_shader_stencil_export");
#endif /* GL_ARB_shader_stencil_export */
#ifdef GL_ARB_shader_subroutine
  CONST_CAST(GLEW_ARB_shader_subroutine) = glewGetExtension("GL_ARB_shader_subroutine");
  if (glewExperimental || GLEW_ARB_shader_subroutine) CONST_CAST(GLEW_ARB_shader_subroutine) = !_glewInit_GL_ARB_shader_subroutine(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_shader_subroutine */
#ifdef GL_ARB_shader_texture_lod
  CONST_CAST(GLEW_ARB_shader_texture_lod) = glewGetExtension("GL_ARB_shader_texture_lod");
#endif /* GL_ARB_shader_texture_lod */
#ifdef GL_ARB_shading_language_100
  CONST_CAST(GLEW_ARB_shading_language_100) = glewGetExtension("GL_ARB_shading_language_100");
#endif /* GL_ARB_shading_language_100 */
#ifdef GL_ARB_shading_language_include
  CONST_CAST(GLEW_ARB_shading_language_include) = glewGetExtension("GL_ARB_shading_language_include");
  if (glewExperimental || GLEW_ARB_shading_language_include) CONST_CAST(GLEW_ARB_shading_language_include) = !_glewInit_GL_ARB_shading_language_include(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_shading_language_include */
#ifdef GL_ARB_shadow
  CONST_CAST(GLEW_ARB_shadow) = glewGetExtension("GL_ARB_shadow");
#endif /* GL_ARB_shadow */
#ifdef GL_ARB_shadow_ambient
  CONST_CAST(GLEW_ARB_shadow_ambient) = glewGetExtension("GL_ARB_shadow_ambient");
#endif /* GL_ARB_shadow_ambient */
#ifdef GL_ARB_sync
  CONST_CAST(GLEW_ARB_sync) = glewGetExtension("GL_ARB_sync");
  if (glewExperimental || GLEW_ARB_sync) CONST_CAST(GLEW_ARB_sync) = !_glewInit_GL_ARB_sync(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_sync */
#ifdef GL_ARB_tessellation_shader
  CONST_CAST(GLEW_ARB_tessellation_shader) = glewGetExtension("GL_ARB_tessellation_shader");
  if (glewExperimental || GLEW_ARB_tessellation_shader) CONST_CAST(GLEW_ARB_tessellation_shader) = !_glewInit_GL_ARB_tessellation_shader(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_tessellation_shader */
#ifdef GL_ARB_texture_border_clamp
  CONST_CAST(GLEW_ARB_texture_border_clamp) = glewGetExtension("GL_ARB_texture_border_clamp");
#endif /* GL_ARB_texture_border_clamp */
#ifdef GL_ARB_texture_buffer_object
  CONST_CAST(GLEW_ARB_texture_buffer_object) = glewGetExtension("GL_ARB_texture_buffer_object");
  if (glewExperimental || GLEW_ARB_texture_buffer_object) CONST_CAST(GLEW_ARB_texture_buffer_object) = !_glewInit_GL_ARB_texture_buffer_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_texture_buffer_object */
#ifdef GL_ARB_texture_buffer_object_rgb32
  CONST_CAST(GLEW_ARB_texture_buffer_object_rgb32) = glewGetExtension("GL_ARB_texture_buffer_object_rgb32");
#endif /* GL_ARB_texture_buffer_object_rgb32 */
#ifdef GL_ARB_texture_compression
  CONST_CAST(GLEW_ARB_texture_compression) = glewGetExtension("GL_ARB_texture_compression");
  if (glewExperimental || GLEW_ARB_texture_compression) CONST_CAST(GLEW_ARB_texture_compression) = !_glewInit_GL_ARB_texture_compression(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_texture_compression */
#ifdef GL_ARB_texture_compression_bptc
  CONST_CAST(GLEW_ARB_texture_compression_bptc) = glewGetExtension("GL_ARB_texture_compression_bptc");
#endif /* GL_ARB_texture_compression_bptc */
#ifdef GL_ARB_texture_compression_rgtc
  CONST_CAST(GLEW_ARB_texture_compression_rgtc) = glewGetExtension("GL_ARB_texture_compression_rgtc");
#endif /* GL_ARB_texture_compression_rgtc */
#ifdef GL_ARB_texture_cube_map
  CONST_CAST(GLEW_ARB_texture_cube_map) = glewGetExtension("GL_ARB_texture_cube_map");
#endif /* GL_ARB_texture_cube_map */
#ifdef GL_ARB_texture_cube_map_array
  CONST_CAST(GLEW_ARB_texture_cube_map_array) = glewGetExtension("GL_ARB_texture_cube_map_array");
#endif /* GL_ARB_texture_cube_map_array */
#ifdef GL_ARB_texture_env_add
  CONST_CAST(GLEW_ARB_texture_env_add) = glewGetExtension("GL_ARB_texture_env_add");
#endif /* GL_ARB_texture_env_add */
#ifdef GL_ARB_texture_env_combine
  CONST_CAST(GLEW_ARB_texture_env_combine) = glewGetExtension("GL_ARB_texture_env_combine");
#endif /* GL_ARB_texture_env_combine */
#ifdef GL_ARB_texture_env_crossbar
  CONST_CAST(GLEW_ARB_texture_env_crossbar) = glewGetExtension("GL_ARB_texture_env_crossbar");
#endif /* GL_ARB_texture_env_crossbar */
#ifdef GL_ARB_texture_env_dot3
  CONST_CAST(GLEW_ARB_texture_env_dot3) = glewGetExtension("GL_ARB_texture_env_dot3");
#endif /* GL_ARB_texture_env_dot3 */
#ifdef GL_ARB_texture_float
  CONST_CAST(GLEW_ARB_texture_float) = glewGetExtension("GL_ARB_texture_float");
#endif /* GL_ARB_texture_float */
#ifdef GL_ARB_texture_gather
  CONST_CAST(GLEW_ARB_texture_gather) = glewGetExtension("GL_ARB_texture_gather");
#endif /* GL_ARB_texture_gather */
#ifdef GL_ARB_texture_mirrored_repeat
  CONST_CAST(GLEW_ARB_texture_mirrored_repeat) = glewGetExtension("GL_ARB_texture_mirrored_repeat");
#endif /* GL_ARB_texture_mirrored_repeat */
#ifdef GL_ARB_texture_multisample
  CONST_CAST(GLEW_ARB_texture_multisample) = glewGetExtension("GL_ARB_texture_multisample");
  if (glewExperimental || GLEW_ARB_texture_multisample) CONST_CAST(GLEW_ARB_texture_multisample) = !_glewInit_GL_ARB_texture_multisample(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_texture_multisample */
#ifdef GL_ARB_texture_non_power_of_two
  CONST_CAST(GLEW_ARB_texture_non_power_of_two) = glewGetExtension("GL_ARB_texture_non_power_of_two");
#endif /* GL_ARB_texture_non_power_of_two */
#ifdef GL_ARB_texture_query_lod
  CONST_CAST(GLEW_ARB_texture_query_lod) = glewGetExtension("GL_ARB_texture_query_lod");
#endif /* GL_ARB_texture_query_lod */
#ifdef GL_ARB_texture_rectangle
  CONST_CAST(GLEW_ARB_texture_rectangle) = glewGetExtension("GL_ARB_texture_rectangle");
#endif /* GL_ARB_texture_rectangle */
#ifdef GL_ARB_texture_rg
  CONST_CAST(GLEW_ARB_texture_rg) = glewGetExtension("GL_ARB_texture_rg");
#endif /* GL_ARB_texture_rg */
#ifdef GL_ARB_texture_rgb10_a2ui
  CONST_CAST(GLEW_ARB_texture_rgb10_a2ui) = glewGetExtension("GL_ARB_texture_rgb10_a2ui");
#endif /* GL_ARB_texture_rgb10_a2ui */
#ifdef GL_ARB_texture_swizzle
  CONST_CAST(GLEW_ARB_texture_swizzle) = glewGetExtension("GL_ARB_texture_swizzle");
#endif /* GL_ARB_texture_swizzle */
#ifdef GL_ARB_timer_query
  CONST_CAST(GLEW_ARB_timer_query) = glewGetExtension("GL_ARB_timer_query");
  if (glewExperimental || GLEW_ARB_timer_query) CONST_CAST(GLEW_ARB_timer_query) = !_glewInit_GL_ARB_timer_query(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_timer_query */
#ifdef GL_ARB_transform_feedback2
  CONST_CAST(GLEW_ARB_transform_feedback2) = glewGetExtension("GL_ARB_transform_feedback2");
  if (glewExperimental || GLEW_ARB_transform_feedback2) CONST_CAST(GLEW_ARB_transform_feedback2) = !_glewInit_GL_ARB_transform_feedback2(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_transform_feedback2 */
#ifdef GL_ARB_transform_feedback3
  CONST_CAST(GLEW_ARB_transform_feedback3) = glewGetExtension("GL_ARB_transform_feedback3");
  if (glewExperimental || GLEW_ARB_transform_feedback3) CONST_CAST(GLEW_ARB_transform_feedback3) = !_glewInit_GL_ARB_transform_feedback3(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_transform_feedback3 */
#ifdef GL_ARB_transpose_matrix
  CONST_CAST(GLEW_ARB_transpose_matrix) = glewGetExtension("GL_ARB_transpose_matrix");
  if (glewExperimental || GLEW_ARB_transpose_matrix) CONST_CAST(GLEW_ARB_transpose_matrix) = !_glewInit_GL_ARB_transpose_matrix(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_transpose_matrix */
#ifdef GL_ARB_uniform_buffer_object
  CONST_CAST(GLEW_ARB_uniform_buffer_object) = glewGetExtension("GL_ARB_uniform_buffer_object");
  if (glewExperimental || GLEW_ARB_uniform_buffer_object) CONST_CAST(GLEW_ARB_uniform_buffer_object) = !_glewInit_GL_ARB_uniform_buffer_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_uniform_buffer_object */
#ifdef GL_ARB_vertex_array_bgra
  CONST_CAST(GLEW_ARB_vertex_array_bgra) = glewGetExtension("GL_ARB_vertex_array_bgra");
#endif /* GL_ARB_vertex_array_bgra */
#ifdef GL_ARB_vertex_array_object
  CONST_CAST(GLEW_ARB_vertex_array_object) = glewGetExtension("GL_ARB_vertex_array_object");
  if (glewExperimental || GLEW_ARB_vertex_array_object) CONST_CAST(GLEW_ARB_vertex_array_object) = !_glewInit_GL_ARB_vertex_array_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_vertex_array_object */
#ifdef GL_ARB_vertex_attrib_64bit
  CONST_CAST(GLEW_ARB_vertex_attrib_64bit) = glewGetExtension("GL_ARB_vertex_attrib_64bit");
  if (glewExperimental || GLEW_ARB_vertex_attrib_64bit) CONST_CAST(GLEW_ARB_vertex_attrib_64bit) = !_glewInit_GL_ARB_vertex_attrib_64bit(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_vertex_attrib_64bit */
#ifdef GL_ARB_vertex_blend
  CONST_CAST(GLEW_ARB_vertex_blend) = glewGetExtension("GL_ARB_vertex_blend");
  if (glewExperimental || GLEW_ARB_vertex_blend) CONST_CAST(GLEW_ARB_vertex_blend) = !_glewInit_GL_ARB_vertex_blend(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_vertex_blend */
#ifdef GL_ARB_vertex_buffer_object
  CONST_CAST(GLEW_ARB_vertex_buffer_object) = glewGetExtension("GL_ARB_vertex_buffer_object");
  if (glewExperimental || GLEW_ARB_vertex_buffer_object) CONST_CAST(GLEW_ARB_vertex_buffer_object) = !_glewInit_GL_ARB_vertex_buffer_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_vertex_buffer_object */
#ifdef GL_ARB_vertex_program
  CONST_CAST(GLEW_ARB_vertex_program) = glewGetExtension("GL_ARB_vertex_program");
  if (glewExperimental || GLEW_ARB_vertex_program) CONST_CAST(GLEW_ARB_vertex_program) = !_glewInit_GL_ARB_vertex_program(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_vertex_program */
#ifdef GL_ARB_vertex_shader
  CONST_CAST(GLEW_ARB_vertex_shader) = glewGetExtension("GL_ARB_vertex_shader");
  if (glewExperimental || GLEW_ARB_vertex_shader) CONST_CAST(GLEW_ARB_vertex_shader) = !_glewInit_GL_ARB_vertex_shader(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_vertex_shader */
#ifdef GL_ARB_vertex_type_2_10_10_10_rev
  CONST_CAST(GLEW_ARB_vertex_type_2_10_10_10_rev) = glewGetExtension("GL_ARB_vertex_type_2_10_10_10_rev");
  if (glewExperimental || GLEW_ARB_vertex_type_2_10_10_10_rev) CONST_CAST(GLEW_ARB_vertex_type_2_10_10_10_rev) = !_glewInit_GL_ARB_vertex_type_2_10_10_10_rev(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_vertex_type_2_10_10_10_rev */
#ifdef GL_ARB_viewport_array
  CONST_CAST(GLEW_ARB_viewport_array) = glewGetExtension("GL_ARB_viewport_array");
  if (glewExperimental || GLEW_ARB_viewport_array) CONST_CAST(GLEW_ARB_viewport_array) = !_glewInit_GL_ARB_viewport_array(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_viewport_array */
#ifdef GL_ARB_window_pos
  CONST_CAST(GLEW_ARB_window_pos) = glewGetExtension("GL_ARB_window_pos");
  if (glewExperimental || GLEW_ARB_window_pos) CONST_CAST(GLEW_ARB_window_pos) = !_glewInit_GL_ARB_window_pos(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ARB_window_pos */
#ifdef GL_ATIX_point_sprites
  CONST_CAST(GLEW_ATIX_point_sprites) = glewGetExtension("GL_ATIX_point_sprites");
#endif /* GL_ATIX_point_sprites */
#ifdef GL_ATIX_texture_env_combine3
  CONST_CAST(GLEW_ATIX_texture_env_combine3) = glewGetExtension("GL_ATIX_texture_env_combine3");
#endif /* GL_ATIX_texture_env_combine3 */
#ifdef GL_ATIX_texture_env_route
  CONST_CAST(GLEW_ATIX_texture_env_route) = glewGetExtension("GL_ATIX_texture_env_route");
#endif /* GL_ATIX_texture_env_route */
#ifdef GL_ATIX_vertex_shader_output_point_size
  CONST_CAST(GLEW_ATIX_vertex_shader_output_point_size) = glewGetExtension("GL_ATIX_vertex_shader_output_point_size");
#endif /* GL_ATIX_vertex_shader_output_point_size */
#ifdef GL_ATI_draw_buffers
  CONST_CAST(GLEW_ATI_draw_buffers) = glewGetExtension("GL_ATI_draw_buffers");
  if (glewExperimental || GLEW_ATI_draw_buffers) CONST_CAST(GLEW_ATI_draw_buffers) = !_glewInit_GL_ATI_draw_buffers(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_draw_buffers */
#ifdef GL_ATI_element_array
  CONST_CAST(GLEW_ATI_element_array) = glewGetExtension("GL_ATI_element_array");
  if (glewExperimental || GLEW_ATI_element_array) CONST_CAST(GLEW_ATI_element_array) = !_glewInit_GL_ATI_element_array(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_element_array */
#ifdef GL_ATI_envmap_bumpmap
  CONST_CAST(GLEW_ATI_envmap_bumpmap) = glewGetExtension("GL_ATI_envmap_bumpmap");
  if (glewExperimental || GLEW_ATI_envmap_bumpmap) CONST_CAST(GLEW_ATI_envmap_bumpmap) = !_glewInit_GL_ATI_envmap_bumpmap(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_envmap_bumpmap */
#ifdef GL_ATI_fragment_shader
  CONST_CAST(GLEW_ATI_fragment_shader) = glewGetExtension("GL_ATI_fragment_shader");
  if (glewExperimental || GLEW_ATI_fragment_shader) CONST_CAST(GLEW_ATI_fragment_shader) = !_glewInit_GL_ATI_fragment_shader(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_fragment_shader */
#ifdef GL_ATI_map_object_buffer
  CONST_CAST(GLEW_ATI_map_object_buffer) = glewGetExtension("GL_ATI_map_object_buffer");
  if (glewExperimental || GLEW_ATI_map_object_buffer) CONST_CAST(GLEW_ATI_map_object_buffer) = !_glewInit_GL_ATI_map_object_buffer(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_map_object_buffer */
#ifdef GL_ATI_meminfo
  CONST_CAST(GLEW_ATI_meminfo) = glewGetExtension("GL_ATI_meminfo");
#endif /* GL_ATI_meminfo */
#ifdef GL_ATI_pn_triangles
  CONST_CAST(GLEW_ATI_pn_triangles) = glewGetExtension("GL_ATI_pn_triangles");
  if (glewExperimental || GLEW_ATI_pn_triangles) CONST_CAST(GLEW_ATI_pn_triangles) = !_glewInit_GL_ATI_pn_triangles(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_pn_triangles */
#ifdef GL_ATI_separate_stencil
  CONST_CAST(GLEW_ATI_separate_stencil) = glewGetExtension("GL_ATI_separate_stencil");
  if (glewExperimental || GLEW_ATI_separate_stencil) CONST_CAST(GLEW_ATI_separate_stencil) = !_glewInit_GL_ATI_separate_stencil(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_separate_stencil */
#ifdef GL_ATI_shader_texture_lod
  CONST_CAST(GLEW_ATI_shader_texture_lod) = glewGetExtension("GL_ATI_shader_texture_lod");
#endif /* GL_ATI_shader_texture_lod */
#ifdef GL_ATI_text_fragment_shader
  CONST_CAST(GLEW_ATI_text_fragment_shader) = glewGetExtension("GL_ATI_text_fragment_shader");
#endif /* GL_ATI_text_fragment_shader */
#ifdef GL_ATI_texture_compression_3dc
  CONST_CAST(GLEW_ATI_texture_compression_3dc) = glewGetExtension("GL_ATI_texture_compression_3dc");
#endif /* GL_ATI_texture_compression_3dc */
#ifdef GL_ATI_texture_env_combine3
  CONST_CAST(GLEW_ATI_texture_env_combine3) = glewGetExtension("GL_ATI_texture_env_combine3");
#endif /* GL_ATI_texture_env_combine3 */
#ifdef GL_ATI_texture_float
  CONST_CAST(GLEW_ATI_texture_float) = glewGetExtension("GL_ATI_texture_float");
#endif /* GL_ATI_texture_float */
#ifdef GL_ATI_texture_mirror_once
  CONST_CAST(GLEW_ATI_texture_mirror_once) = glewGetExtension("GL_ATI_texture_mirror_once");
#endif /* GL_ATI_texture_mirror_once */
#ifdef GL_ATI_vertex_array_object
  CONST_CAST(GLEW_ATI_vertex_array_object) = glewGetExtension("GL_ATI_vertex_array_object");
  if (glewExperimental || GLEW_ATI_vertex_array_object) CONST_CAST(GLEW_ATI_vertex_array_object) = !_glewInit_GL_ATI_vertex_array_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_vertex_array_object */
#ifdef GL_ATI_vertex_attrib_array_object
  CONST_CAST(GLEW_ATI_vertex_attrib_array_object) = glewGetExtension("GL_ATI_vertex_attrib_array_object");
  if (glewExperimental || GLEW_ATI_vertex_attrib_array_object) CONST_CAST(GLEW_ATI_vertex_attrib_array_object) = !_glewInit_GL_ATI_vertex_attrib_array_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_vertex_attrib_array_object */
#ifdef GL_ATI_vertex_streams
  CONST_CAST(GLEW_ATI_vertex_streams) = glewGetExtension("GL_ATI_vertex_streams");
  if (glewExperimental || GLEW_ATI_vertex_streams) CONST_CAST(GLEW_ATI_vertex_streams) = !_glewInit_GL_ATI_vertex_streams(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_ATI_vertex_streams */
#ifdef GL_EXT_422_pixels
  CONST_CAST(GLEW_EXT_422_pixels) = glewGetExtension("GL_EXT_422_pixels");
#endif /* GL_EXT_422_pixels */
#ifdef GL_EXT_Cg_shader
  CONST_CAST(GLEW_EXT_Cg_shader) = glewGetExtension("GL_EXT_Cg_shader");
#endif /* GL_EXT_Cg_shader */
#ifdef GL_EXT_abgr
  CONST_CAST(GLEW_EXT_abgr) = glewGetExtension("GL_EXT_abgr");
#endif /* GL_EXT_abgr */
#ifdef GL_EXT_bgra
  CONST_CAST(GLEW_EXT_bgra) = glewGetExtension("GL_EXT_bgra");
#endif /* GL_EXT_bgra */
#ifdef GL_EXT_bindable_uniform
  CONST_CAST(GLEW_EXT_bindable_uniform) = glewGetExtension("GL_EXT_bindable_uniform");
  if (glewExperimental || GLEW_EXT_bindable_uniform) CONST_CAST(GLEW_EXT_bindable_uniform) = !_glewInit_GL_EXT_bindable_uniform(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_bindable_uniform */
#ifdef GL_EXT_blend_color
  CONST_CAST(GLEW_EXT_blend_color) = glewGetExtension("GL_EXT_blend_color");
  if (glewExperimental || GLEW_EXT_blend_color) CONST_CAST(GLEW_EXT_blend_color) = !_glewInit_GL_EXT_blend_color(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_blend_color */
#ifdef GL_EXT_blend_equation_separate
  CONST_CAST(GLEW_EXT_blend_equation_separate) = glewGetExtension("GL_EXT_blend_equation_separate");
  if (glewExperimental || GLEW_EXT_blend_equation_separate) CONST_CAST(GLEW_EXT_blend_equation_separate) = !_glewInit_GL_EXT_blend_equation_separate(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_blend_equation_separate */
#ifdef GL_EXT_blend_func_separate
  CONST_CAST(GLEW_EXT_blend_func_separate) = glewGetExtension("GL_EXT_blend_func_separate");
  if (glewExperimental || GLEW_EXT_blend_func_separate) CONST_CAST(GLEW_EXT_blend_func_separate) = !_glewInit_GL_EXT_blend_func_separate(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_blend_func_separate */
#ifdef GL_EXT_blend_logic_op
  CONST_CAST(GLEW_EXT_blend_logic_op) = glewGetExtension("GL_EXT_blend_logic_op");
#endif /* GL_EXT_blend_logic_op */
#ifdef GL_EXT_blend_minmax
  CONST_CAST(GLEW_EXT_blend_minmax) = glewGetExtension("GL_EXT_blend_minmax");
  if (glewExperimental || GLEW_EXT_blend_minmax) CONST_CAST(GLEW_EXT_blend_minmax) = !_glewInit_GL_EXT_blend_minmax(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_blend_minmax */
#ifdef GL_EXT_blend_subtract
  CONST_CAST(GLEW_EXT_blend_subtract) = glewGetExtension("GL_EXT_blend_subtract");
#endif /* GL_EXT_blend_subtract */
#ifdef GL_EXT_clip_volume_hint
  CONST_CAST(GLEW_EXT_clip_volume_hint) = glewGetExtension("GL_EXT_clip_volume_hint");
#endif /* GL_EXT_clip_volume_hint */
#ifdef GL_EXT_cmyka
  CONST_CAST(GLEW_EXT_cmyka) = glewGetExtension("GL_EXT_cmyka");
#endif /* GL_EXT_cmyka */
#ifdef GL_EXT_color_subtable
  CONST_CAST(GLEW_EXT_color_subtable) = glewGetExtension("GL_EXT_color_subtable");
  if (glewExperimental || GLEW_EXT_color_subtable) CONST_CAST(GLEW_EXT_color_subtable) = !_glewInit_GL_EXT_color_subtable(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_color_subtable */
#ifdef GL_EXT_compiled_vertex_array
  CONST_CAST(GLEW_EXT_compiled_vertex_array) = glewGetExtension("GL_EXT_compiled_vertex_array");
  if (glewExperimental || GLEW_EXT_compiled_vertex_array) CONST_CAST(GLEW_EXT_compiled_vertex_array) = !_glewInit_GL_EXT_compiled_vertex_array(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_compiled_vertex_array */
#ifdef GL_EXT_convolution
  CONST_CAST(GLEW_EXT_convolution) = glewGetExtension("GL_EXT_convolution");
  if (glewExperimental || GLEW_EXT_convolution) CONST_CAST(GLEW_EXT_convolution) = !_glewInit_GL_EXT_convolution(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_convolution */
#ifdef GL_EXT_coordinate_frame
  CONST_CAST(GLEW_EXT_coordinate_frame) = glewGetExtension("GL_EXT_coordinate_frame");
  if (glewExperimental || GLEW_EXT_coordinate_frame) CONST_CAST(GLEW_EXT_coordinate_frame) = !_glewInit_GL_EXT_coordinate_frame(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_coordinate_frame */
#ifdef GL_EXT_copy_texture
  CONST_CAST(GLEW_EXT_copy_texture) = glewGetExtension("GL_EXT_copy_texture");
  if (glewExperimental || GLEW_EXT_copy_texture) CONST_CAST(GLEW_EXT_copy_texture) = !_glewInit_GL_EXT_copy_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_copy_texture */
#ifdef GL_EXT_cull_vertex
  CONST_CAST(GLEW_EXT_cull_vertex) = glewGetExtension("GL_EXT_cull_vertex");
  if (glewExperimental || GLEW_EXT_cull_vertex) CONST_CAST(GLEW_EXT_cull_vertex) = !_glewInit_GL_EXT_cull_vertex(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_cull_vertex */
#ifdef GL_EXT_depth_bounds_test
  CONST_CAST(GLEW_EXT_depth_bounds_test) = glewGetExtension("GL_EXT_depth_bounds_test");
  if (glewExperimental || GLEW_EXT_depth_bounds_test) CONST_CAST(GLEW_EXT_depth_bounds_test) = !_glewInit_GL_EXT_depth_bounds_test(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_depth_bounds_test */
#ifdef GL_EXT_direct_state_access
  CONST_CAST(GLEW_EXT_direct_state_access) = glewGetExtension("GL_EXT_direct_state_access");
  if (glewExperimental || GLEW_EXT_direct_state_access) CONST_CAST(GLEW_EXT_direct_state_access) = !_glewInit_GL_EXT_direct_state_access(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_direct_state_access */
#ifdef GL_EXT_draw_buffers2
  CONST_CAST(GLEW_EXT_draw_buffers2) = glewGetExtension("GL_EXT_draw_buffers2");
  if (glewExperimental || GLEW_EXT_draw_buffers2) CONST_CAST(GLEW_EXT_draw_buffers2) = !_glewInit_GL_EXT_draw_buffers2(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_draw_buffers2 */
#ifdef GL_EXT_draw_instanced
  CONST_CAST(GLEW_EXT_draw_instanced) = glewGetExtension("GL_EXT_draw_instanced");
  if (glewExperimental || GLEW_EXT_draw_instanced) CONST_CAST(GLEW_EXT_draw_instanced) = !_glewInit_GL_EXT_draw_instanced(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_draw_instanced */
#ifdef GL_EXT_draw_range_elements
  CONST_CAST(GLEW_EXT_draw_range_elements) = glewGetExtension("GL_EXT_draw_range_elements");
  if (glewExperimental || GLEW_EXT_draw_range_elements) CONST_CAST(GLEW_EXT_draw_range_elements) = !_glewInit_GL_EXT_draw_range_elements(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_draw_range_elements */
#ifdef GL_EXT_fog_coord
  CONST_CAST(GLEW_EXT_fog_coord) = glewGetExtension("GL_EXT_fog_coord");
  if (glewExperimental || GLEW_EXT_fog_coord) CONST_CAST(GLEW_EXT_fog_coord) = !_glewInit_GL_EXT_fog_coord(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_fog_coord */
#ifdef GL_EXT_fragment_lighting
  CONST_CAST(GLEW_EXT_fragment_lighting) = glewGetExtension("GL_EXT_fragment_lighting");
  if (glewExperimental || GLEW_EXT_fragment_lighting) CONST_CAST(GLEW_EXT_fragment_lighting) = !_glewInit_GL_EXT_fragment_lighting(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_fragment_lighting */
#ifdef GL_EXT_framebuffer_blit
  CONST_CAST(GLEW_EXT_framebuffer_blit) = glewGetExtension("GL_EXT_framebuffer_blit");
  if (glewExperimental || GLEW_EXT_framebuffer_blit) CONST_CAST(GLEW_EXT_framebuffer_blit) = !_glewInit_GL_EXT_framebuffer_blit(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_framebuffer_blit */
#ifdef GL_EXT_framebuffer_multisample
  CONST_CAST(GLEW_EXT_framebuffer_multisample) = glewGetExtension("GL_EXT_framebuffer_multisample");
  if (glewExperimental || GLEW_EXT_framebuffer_multisample) CONST_CAST(GLEW_EXT_framebuffer_multisample) = !_glewInit_GL_EXT_framebuffer_multisample(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_framebuffer_multisample */
#ifdef GL_EXT_framebuffer_object
  CONST_CAST(GLEW_EXT_framebuffer_object) = glewGetExtension("GL_EXT_framebuffer_object");
  if (glewExperimental || GLEW_EXT_framebuffer_object) CONST_CAST(GLEW_EXT_framebuffer_object) = !_glewInit_GL_EXT_framebuffer_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_framebuffer_object */
#ifdef GL_EXT_framebuffer_sRGB
  CONST_CAST(GLEW_EXT_framebuffer_sRGB) = glewGetExtension("GL_EXT_framebuffer_sRGB");
#endif /* GL_EXT_framebuffer_sRGB */
#ifdef GL_EXT_geometry_shader4
  CONST_CAST(GLEW_EXT_geometry_shader4) = glewGetExtension("GL_EXT_geometry_shader4");
  if (glewExperimental || GLEW_EXT_geometry_shader4) CONST_CAST(GLEW_EXT_geometry_shader4) = !_glewInit_GL_EXT_geometry_shader4(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_geometry_shader4 */
#ifdef GL_EXT_gpu_program_parameters
  CONST_CAST(GLEW_EXT_gpu_program_parameters) = glewGetExtension("GL_EXT_gpu_program_parameters");
  if (glewExperimental || GLEW_EXT_gpu_program_parameters) CONST_CAST(GLEW_EXT_gpu_program_parameters) = !_glewInit_GL_EXT_gpu_program_parameters(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_gpu_program_parameters */
#ifdef GL_EXT_gpu_shader4
  CONST_CAST(GLEW_EXT_gpu_shader4) = glewGetExtension("GL_EXT_gpu_shader4");
  if (glewExperimental || GLEW_EXT_gpu_shader4) CONST_CAST(GLEW_EXT_gpu_shader4) = !_glewInit_GL_EXT_gpu_shader4(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_gpu_shader4 */
#ifdef GL_EXT_histogram
  CONST_CAST(GLEW_EXT_histogram) = glewGetExtension("GL_EXT_histogram");
  if (glewExperimental || GLEW_EXT_histogram) CONST_CAST(GLEW_EXT_histogram) = !_glewInit_GL_EXT_histogram(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_histogram */
#ifdef GL_EXT_index_array_formats
  CONST_CAST(GLEW_EXT_index_array_formats) = glewGetExtension("GL_EXT_index_array_formats");
#endif /* GL_EXT_index_array_formats */
#ifdef GL_EXT_index_func
  CONST_CAST(GLEW_EXT_index_func) = glewGetExtension("GL_EXT_index_func");
  if (glewExperimental || GLEW_EXT_index_func) CONST_CAST(GLEW_EXT_index_func) = !_glewInit_GL_EXT_index_func(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_index_func */
#ifdef GL_EXT_index_material
  CONST_CAST(GLEW_EXT_index_material) = glewGetExtension("GL_EXT_index_material");
  if (glewExperimental || GLEW_EXT_index_material) CONST_CAST(GLEW_EXT_index_material) = !_glewInit_GL_EXT_index_material(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_index_material */
#ifdef GL_EXT_index_texture
  CONST_CAST(GLEW_EXT_index_texture) = glewGetExtension("GL_EXT_index_texture");
#endif /* GL_EXT_index_texture */
#ifdef GL_EXT_light_texture
  CONST_CAST(GLEW_EXT_light_texture) = glewGetExtension("GL_EXT_light_texture");
  if (glewExperimental || GLEW_EXT_light_texture) CONST_CAST(GLEW_EXT_light_texture) = !_glewInit_GL_EXT_light_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_light_texture */
#ifdef GL_EXT_misc_attribute
  CONST_CAST(GLEW_EXT_misc_attribute) = glewGetExtension("GL_EXT_misc_attribute");
#endif /* GL_EXT_misc_attribute */
#ifdef GL_EXT_multi_draw_arrays
  CONST_CAST(GLEW_EXT_multi_draw_arrays) = glewGetExtension("GL_EXT_multi_draw_arrays");
  if (glewExperimental || GLEW_EXT_multi_draw_arrays) CONST_CAST(GLEW_EXT_multi_draw_arrays) = !_glewInit_GL_EXT_multi_draw_arrays(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_multi_draw_arrays */
#ifdef GL_EXT_multisample
  CONST_CAST(GLEW_EXT_multisample) = glewGetExtension("GL_EXT_multisample");
  if (glewExperimental || GLEW_EXT_multisample) CONST_CAST(GLEW_EXT_multisample) = !_glewInit_GL_EXT_multisample(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_multisample */
#ifdef GL_EXT_packed_depth_stencil
  CONST_CAST(GLEW_EXT_packed_depth_stencil) = glewGetExtension("GL_EXT_packed_depth_stencil");
#endif /* GL_EXT_packed_depth_stencil */
#ifdef GL_EXT_packed_float
  CONST_CAST(GLEW_EXT_packed_float) = glewGetExtension("GL_EXT_packed_float");
#endif /* GL_EXT_packed_float */
#ifdef GL_EXT_packed_pixels
  CONST_CAST(GLEW_EXT_packed_pixels) = glewGetExtension("GL_EXT_packed_pixels");
#endif /* GL_EXT_packed_pixels */
#ifdef GL_EXT_paletted_texture
  CONST_CAST(GLEW_EXT_paletted_texture) = glewGetExtension("GL_EXT_paletted_texture");
  if (glewExperimental || GLEW_EXT_paletted_texture) CONST_CAST(GLEW_EXT_paletted_texture) = !_glewInit_GL_EXT_paletted_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_paletted_texture */
#ifdef GL_EXT_pixel_buffer_object
  CONST_CAST(GLEW_EXT_pixel_buffer_object) = glewGetExtension("GL_EXT_pixel_buffer_object");
#endif /* GL_EXT_pixel_buffer_object */
#ifdef GL_EXT_pixel_transform
  CONST_CAST(GLEW_EXT_pixel_transform) = glewGetExtension("GL_EXT_pixel_transform");
  if (glewExperimental || GLEW_EXT_pixel_transform) CONST_CAST(GLEW_EXT_pixel_transform) = !_glewInit_GL_EXT_pixel_transform(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_pixel_transform */
#ifdef GL_EXT_pixel_transform_color_table
  CONST_CAST(GLEW_EXT_pixel_transform_color_table) = glewGetExtension("GL_EXT_pixel_transform_color_table");
#endif /* GL_EXT_pixel_transform_color_table */
#ifdef GL_EXT_point_parameters
  CONST_CAST(GLEW_EXT_point_parameters) = glewGetExtension("GL_EXT_point_parameters");
  if (glewExperimental || GLEW_EXT_point_parameters) CONST_CAST(GLEW_EXT_point_parameters) = !_glewInit_GL_EXT_point_parameters(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_point_parameters */
#ifdef GL_EXT_polygon_offset
  CONST_CAST(GLEW_EXT_polygon_offset) = glewGetExtension("GL_EXT_polygon_offset");
  if (glewExperimental || GLEW_EXT_polygon_offset) CONST_CAST(GLEW_EXT_polygon_offset) = !_glewInit_GL_EXT_polygon_offset(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_polygon_offset */
#ifdef GL_EXT_provoking_vertex
  CONST_CAST(GLEW_EXT_provoking_vertex) = glewGetExtension("GL_EXT_provoking_vertex");
  if (glewExperimental || GLEW_EXT_provoking_vertex) CONST_CAST(GLEW_EXT_provoking_vertex) = !_glewInit_GL_EXT_provoking_vertex(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_provoking_vertex */
#ifdef GL_EXT_rescale_normal
  CONST_CAST(GLEW_EXT_rescale_normal) = glewGetExtension("GL_EXT_rescale_normal");
#endif /* GL_EXT_rescale_normal */
#ifdef GL_EXT_scene_marker
  CONST_CAST(GLEW_EXT_scene_marker) = glewGetExtension("GL_EXT_scene_marker");
  if (glewExperimental || GLEW_EXT_scene_marker) CONST_CAST(GLEW_EXT_scene_marker) = !_glewInit_GL_EXT_scene_marker(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_scene_marker */
#ifdef GL_EXT_secondary_color
  CONST_CAST(GLEW_EXT_secondary_color) = glewGetExtension("GL_EXT_secondary_color");
  if (glewExperimental || GLEW_EXT_secondary_color) CONST_CAST(GLEW_EXT_secondary_color) = !_glewInit_GL_EXT_secondary_color(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_secondary_color */
#ifdef GL_EXT_separate_shader_objects
  CONST_CAST(GLEW_EXT_separate_shader_objects) = glewGetExtension("GL_EXT_separate_shader_objects");
  if (glewExperimental || GLEW_EXT_separate_shader_objects) CONST_CAST(GLEW_EXT_separate_shader_objects) = !_glewInit_GL_EXT_separate_shader_objects(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_separate_shader_objects */
#ifdef GL_EXT_separate_specular_color
  CONST_CAST(GLEW_EXT_separate_specular_color) = glewGetExtension("GL_EXT_separate_specular_color");
#endif /* GL_EXT_separate_specular_color */
#ifdef GL_EXT_shader_image_load_store
  CONST_CAST(GLEW_EXT_shader_image_load_store) = glewGetExtension("GL_EXT_shader_image_load_store");
  if (glewExperimental || GLEW_EXT_shader_image_load_store) CONST_CAST(GLEW_EXT_shader_image_load_store) = !_glewInit_GL_EXT_shader_image_load_store(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_shader_image_load_store */
#ifdef GL_EXT_shadow_funcs
  CONST_CAST(GLEW_EXT_shadow_funcs) = glewGetExtension("GL_EXT_shadow_funcs");
#endif /* GL_EXT_shadow_funcs */
#ifdef GL_EXT_shared_texture_palette
  CONST_CAST(GLEW_EXT_shared_texture_palette) = glewGetExtension("GL_EXT_shared_texture_palette");
#endif /* GL_EXT_shared_texture_palette */
#ifdef GL_EXT_stencil_clear_tag
  CONST_CAST(GLEW_EXT_stencil_clear_tag) = glewGetExtension("GL_EXT_stencil_clear_tag");
#endif /* GL_EXT_stencil_clear_tag */
#ifdef GL_EXT_stencil_two_side
  CONST_CAST(GLEW_EXT_stencil_two_side) = glewGetExtension("GL_EXT_stencil_two_side");
  if (glewExperimental || GLEW_EXT_stencil_two_side) CONST_CAST(GLEW_EXT_stencil_two_side) = !_glewInit_GL_EXT_stencil_two_side(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_stencil_two_side */
#ifdef GL_EXT_stencil_wrap
  CONST_CAST(GLEW_EXT_stencil_wrap) = glewGetExtension("GL_EXT_stencil_wrap");
#endif /* GL_EXT_stencil_wrap */
#ifdef GL_EXT_subtexture
  CONST_CAST(GLEW_EXT_subtexture) = glewGetExtension("GL_EXT_subtexture");
  if (glewExperimental || GLEW_EXT_subtexture) CONST_CAST(GLEW_EXT_subtexture) = !_glewInit_GL_EXT_subtexture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_subtexture */
#ifdef GL_EXT_texture
  CONST_CAST(GLEW_EXT_texture) = glewGetExtension("GL_EXT_texture");
#endif /* GL_EXT_texture */
#ifdef GL_EXT_texture3D
  CONST_CAST(GLEW_EXT_texture3D) = glewGetExtension("GL_EXT_texture3D");
  if (glewExperimental || GLEW_EXT_texture3D) CONST_CAST(GLEW_EXT_texture3D) = !_glewInit_GL_EXT_texture3D(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_texture3D */
#ifdef GL_EXT_texture_array
  CONST_CAST(GLEW_EXT_texture_array) = glewGetExtension("GL_EXT_texture_array");
#endif /* GL_EXT_texture_array */
#ifdef GL_EXT_texture_buffer_object
  CONST_CAST(GLEW_EXT_texture_buffer_object) = glewGetExtension("GL_EXT_texture_buffer_object");
  if (glewExperimental || GLEW_EXT_texture_buffer_object) CONST_CAST(GLEW_EXT_texture_buffer_object) = !_glewInit_GL_EXT_texture_buffer_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_texture_buffer_object */
#ifdef GL_EXT_texture_compression_dxt1
  CONST_CAST(GLEW_EXT_texture_compression_dxt1) = glewGetExtension("GL_EXT_texture_compression_dxt1");
#endif /* GL_EXT_texture_compression_dxt1 */
#ifdef GL_EXT_texture_compression_latc
  CONST_CAST(GLEW_EXT_texture_compression_latc) = glewGetExtension("GL_EXT_texture_compression_latc");
#endif /* GL_EXT_texture_compression_latc */
#ifdef GL_EXT_texture_compression_rgtc
  CONST_CAST(GLEW_EXT_texture_compression_rgtc) = glewGetExtension("GL_EXT_texture_compression_rgtc");
#endif /* GL_EXT_texture_compression_rgtc */
#ifdef GL_EXT_texture_compression_s3tc
  CONST_CAST(GLEW_EXT_texture_compression_s3tc) = glewGetExtension("GL_EXT_texture_compression_s3tc");
#endif /* GL_EXT_texture_compression_s3tc */
#ifdef GL_EXT_texture_cube_map
  CONST_CAST(GLEW_EXT_texture_cube_map) = glewGetExtension("GL_EXT_texture_cube_map");
#endif /* GL_EXT_texture_cube_map */
#ifdef GL_EXT_texture_edge_clamp
  CONST_CAST(GLEW_EXT_texture_edge_clamp) = glewGetExtension("GL_EXT_texture_edge_clamp");
#endif /* GL_EXT_texture_edge_clamp */
#ifdef GL_EXT_texture_env
  CONST_CAST(GLEW_EXT_texture_env) = glewGetExtension("GL_EXT_texture_env");
#endif /* GL_EXT_texture_env */
#ifdef GL_EXT_texture_env_add
  CONST_CAST(GLEW_EXT_texture_env_add) = glewGetExtension("GL_EXT_texture_env_add");
#endif /* GL_EXT_texture_env_add */
#ifdef GL_EXT_texture_env_combine
  CONST_CAST(GLEW_EXT_texture_env_combine) = glewGetExtension("GL_EXT_texture_env_combine");
#endif /* GL_EXT_texture_env_combine */
#ifdef GL_EXT_texture_env_dot3
  CONST_CAST(GLEW_EXT_texture_env_dot3) = glewGetExtension("GL_EXT_texture_env_dot3");
#endif /* GL_EXT_texture_env_dot3 */
#ifdef GL_EXT_texture_filter_anisotropic
  CONST_CAST(GLEW_EXT_texture_filter_anisotropic) = glewGetExtension("GL_EXT_texture_filter_anisotropic");
#endif /* GL_EXT_texture_filter_anisotropic */
#ifdef GL_EXT_texture_integer
  CONST_CAST(GLEW_EXT_texture_integer) = glewGetExtension("GL_EXT_texture_integer");
  if (glewExperimental || GLEW_EXT_texture_integer) CONST_CAST(GLEW_EXT_texture_integer) = !_glewInit_GL_EXT_texture_integer(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_texture_integer */
#ifdef GL_EXT_texture_lod_bias
  CONST_CAST(GLEW_EXT_texture_lod_bias) = glewGetExtension("GL_EXT_texture_lod_bias");
#endif /* GL_EXT_texture_lod_bias */
#ifdef GL_EXT_texture_mirror_clamp
  CONST_CAST(GLEW_EXT_texture_mirror_clamp) = glewGetExtension("GL_EXT_texture_mirror_clamp");
#endif /* GL_EXT_texture_mirror_clamp */
#ifdef GL_EXT_texture_object
  CONST_CAST(GLEW_EXT_texture_object) = glewGetExtension("GL_EXT_texture_object");
  if (glewExperimental || GLEW_EXT_texture_object) CONST_CAST(GLEW_EXT_texture_object) = !_glewInit_GL_EXT_texture_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_texture_object */
#ifdef GL_EXT_texture_perturb_normal
  CONST_CAST(GLEW_EXT_texture_perturb_normal) = glewGetExtension("GL_EXT_texture_perturb_normal");
  if (glewExperimental || GLEW_EXT_texture_perturb_normal) CONST_CAST(GLEW_EXT_texture_perturb_normal) = !_glewInit_GL_EXT_texture_perturb_normal(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_texture_perturb_normal */
#ifdef GL_EXT_texture_rectangle
  CONST_CAST(GLEW_EXT_texture_rectangle) = glewGetExtension("GL_EXT_texture_rectangle");
#endif /* GL_EXT_texture_rectangle */
#ifdef GL_EXT_texture_sRGB
  CONST_CAST(GLEW_EXT_texture_sRGB) = glewGetExtension("GL_EXT_texture_sRGB");
#endif /* GL_EXT_texture_sRGB */
#ifdef GL_EXT_texture_sRGB_decode
  CONST_CAST(GLEW_EXT_texture_sRGB_decode) = glewGetExtension("GL_EXT_texture_sRGB_decode");
#endif /* GL_EXT_texture_sRGB_decode */
#ifdef GL_EXT_texture_shared_exponent
  CONST_CAST(GLEW_EXT_texture_shared_exponent) = glewGetExtension("GL_EXT_texture_shared_exponent");
#endif /* GL_EXT_texture_shared_exponent */
#ifdef GL_EXT_texture_snorm
  CONST_CAST(GLEW_EXT_texture_snorm) = glewGetExtension("GL_EXT_texture_snorm");
#endif /* GL_EXT_texture_snorm */
#ifdef GL_EXT_texture_swizzle
  CONST_CAST(GLEW_EXT_texture_swizzle) = glewGetExtension("GL_EXT_texture_swizzle");
#endif /* GL_EXT_texture_swizzle */
#ifdef GL_EXT_timer_query
  CONST_CAST(GLEW_EXT_timer_query) = glewGetExtension("GL_EXT_timer_query");
  if (glewExperimental || GLEW_EXT_timer_query) CONST_CAST(GLEW_EXT_timer_query) = !_glewInit_GL_EXT_timer_query(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_timer_query */
#ifdef GL_EXT_transform_feedback
  CONST_CAST(GLEW_EXT_transform_feedback) = glewGetExtension("GL_EXT_transform_feedback");
  if (glewExperimental || GLEW_EXT_transform_feedback) CONST_CAST(GLEW_EXT_transform_feedback) = !_glewInit_GL_EXT_transform_feedback(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_transform_feedback */
#ifdef GL_EXT_vertex_array
  CONST_CAST(GLEW_EXT_vertex_array) = glewGetExtension("GL_EXT_vertex_array");
  if (glewExperimental || GLEW_EXT_vertex_array) CONST_CAST(GLEW_EXT_vertex_array) = !_glewInit_GL_EXT_vertex_array(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_vertex_array */
#ifdef GL_EXT_vertex_array_bgra
  CONST_CAST(GLEW_EXT_vertex_array_bgra) = glewGetExtension("GL_EXT_vertex_array_bgra");
#endif /* GL_EXT_vertex_array_bgra */
#ifdef GL_EXT_vertex_attrib_64bit
  CONST_CAST(GLEW_EXT_vertex_attrib_64bit) = glewGetExtension("GL_EXT_vertex_attrib_64bit");
  if (glewExperimental || GLEW_EXT_vertex_attrib_64bit) CONST_CAST(GLEW_EXT_vertex_attrib_64bit) = !_glewInit_GL_EXT_vertex_attrib_64bit(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_vertex_attrib_64bit */
#ifdef GL_EXT_vertex_shader
  CONST_CAST(GLEW_EXT_vertex_shader) = glewGetExtension("GL_EXT_vertex_shader");
  if (glewExperimental || GLEW_EXT_vertex_shader) CONST_CAST(GLEW_EXT_vertex_shader) = !_glewInit_GL_EXT_vertex_shader(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_vertex_shader */
#ifdef GL_EXT_vertex_weighting
  CONST_CAST(GLEW_EXT_vertex_weighting) = glewGetExtension("GL_EXT_vertex_weighting");
  if (glewExperimental || GLEW_EXT_vertex_weighting) CONST_CAST(GLEW_EXT_vertex_weighting) = !_glewInit_GL_EXT_vertex_weighting(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_EXT_vertex_weighting */
#ifdef GL_GREMEDY_frame_terminator
  CONST_CAST(GLEW_GREMEDY_frame_terminator) = glewGetExtension("GL_GREMEDY_frame_terminator");
  if (glewExperimental || GLEW_GREMEDY_frame_terminator) CONST_CAST(GLEW_GREMEDY_frame_terminator) = !_glewInit_GL_GREMEDY_frame_terminator(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_GREMEDY_frame_terminator */
#ifdef GL_GREMEDY_string_marker
  CONST_CAST(GLEW_GREMEDY_string_marker) = glewGetExtension("GL_GREMEDY_string_marker");
  if (glewExperimental || GLEW_GREMEDY_string_marker) CONST_CAST(GLEW_GREMEDY_string_marker) = !_glewInit_GL_GREMEDY_string_marker(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_GREMEDY_string_marker */
#ifdef GL_HP_convolution_border_modes
  CONST_CAST(GLEW_HP_convolution_border_modes) = glewGetExtension("GL_HP_convolution_border_modes");
#endif /* GL_HP_convolution_border_modes */
#ifdef GL_HP_image_transform
  CONST_CAST(GLEW_HP_image_transform) = glewGetExtension("GL_HP_image_transform");
  if (glewExperimental || GLEW_HP_image_transform) CONST_CAST(GLEW_HP_image_transform) = !_glewInit_GL_HP_image_transform(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_HP_image_transform */
#ifdef GL_HP_occlusion_test
  CONST_CAST(GLEW_HP_occlusion_test) = glewGetExtension("GL_HP_occlusion_test");
#endif /* GL_HP_occlusion_test */
#ifdef GL_HP_texture_lighting
  CONST_CAST(GLEW_HP_texture_lighting) = glewGetExtension("GL_HP_texture_lighting");
#endif /* GL_HP_texture_lighting */
#ifdef GL_IBM_cull_vertex
  CONST_CAST(GLEW_IBM_cull_vertex) = glewGetExtension("GL_IBM_cull_vertex");
#endif /* GL_IBM_cull_vertex */
#ifdef GL_IBM_multimode_draw_arrays
  CONST_CAST(GLEW_IBM_multimode_draw_arrays) = glewGetExtension("GL_IBM_multimode_draw_arrays");
  if (glewExperimental || GLEW_IBM_multimode_draw_arrays) CONST_CAST(GLEW_IBM_multimode_draw_arrays) = !_glewInit_GL_IBM_multimode_draw_arrays(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_IBM_multimode_draw_arrays */
#ifdef GL_IBM_rasterpos_clip
  CONST_CAST(GLEW_IBM_rasterpos_clip) = glewGetExtension("GL_IBM_rasterpos_clip");
#endif /* GL_IBM_rasterpos_clip */
#ifdef GL_IBM_static_data
  CONST_CAST(GLEW_IBM_static_data) = glewGetExtension("GL_IBM_static_data");
#endif /* GL_IBM_static_data */
#ifdef GL_IBM_texture_mirrored_repeat
  CONST_CAST(GLEW_IBM_texture_mirrored_repeat) = glewGetExtension("GL_IBM_texture_mirrored_repeat");
#endif /* GL_IBM_texture_mirrored_repeat */
#ifdef GL_IBM_vertex_array_lists
  CONST_CAST(GLEW_IBM_vertex_array_lists) = glewGetExtension("GL_IBM_vertex_array_lists");
  if (glewExperimental || GLEW_IBM_vertex_array_lists) CONST_CAST(GLEW_IBM_vertex_array_lists) = !_glewInit_GL_IBM_vertex_array_lists(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_IBM_vertex_array_lists */
#ifdef GL_INGR_color_clamp
  CONST_CAST(GLEW_INGR_color_clamp) = glewGetExtension("GL_INGR_color_clamp");
#endif /* GL_INGR_color_clamp */
#ifdef GL_INGR_interlace_read
  CONST_CAST(GLEW_INGR_interlace_read) = glewGetExtension("GL_INGR_interlace_read");
#endif /* GL_INGR_interlace_read */
#ifdef GL_INTEL_parallel_arrays
  CONST_CAST(GLEW_INTEL_parallel_arrays) = glewGetExtension("GL_INTEL_parallel_arrays");
  if (glewExperimental || GLEW_INTEL_parallel_arrays) CONST_CAST(GLEW_INTEL_parallel_arrays) = !_glewInit_GL_INTEL_parallel_arrays(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_INTEL_parallel_arrays */
#ifdef GL_INTEL_texture_scissor
  CONST_CAST(GLEW_INTEL_texture_scissor) = glewGetExtension("GL_INTEL_texture_scissor");
  if (glewExperimental || GLEW_INTEL_texture_scissor) CONST_CAST(GLEW_INTEL_texture_scissor) = !_glewInit_GL_INTEL_texture_scissor(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_INTEL_texture_scissor */
#ifdef GL_KTX_buffer_region
  CONST_CAST(GLEW_KTX_buffer_region) = glewGetExtension("GL_KTX_buffer_region");
  if (glewExperimental || GLEW_KTX_buffer_region) CONST_CAST(GLEW_KTX_buffer_region) = !_glewInit_GL_KTX_buffer_region(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_KTX_buffer_region */
#ifdef GL_MESAX_texture_stack
  CONST_CAST(GLEW_MESAX_texture_stack) = glewGetExtension("GL_MESAX_texture_stack");
#endif /* GL_MESAX_texture_stack */
#ifdef GL_MESA_pack_invert
  CONST_CAST(GLEW_MESA_pack_invert) = glewGetExtension("GL_MESA_pack_invert");
#endif /* GL_MESA_pack_invert */
#ifdef GL_MESA_resize_buffers
  CONST_CAST(GLEW_MESA_resize_buffers) = glewGetExtension("GL_MESA_resize_buffers");
  if (glewExperimental || GLEW_MESA_resize_buffers) CONST_CAST(GLEW_MESA_resize_buffers) = !_glewInit_GL_MESA_resize_buffers(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_MESA_resize_buffers */
#ifdef GL_MESA_window_pos
  CONST_CAST(GLEW_MESA_window_pos) = glewGetExtension("GL_MESA_window_pos");
  if (glewExperimental || GLEW_MESA_window_pos) CONST_CAST(GLEW_MESA_window_pos) = !_glewInit_GL_MESA_window_pos(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_MESA_window_pos */
#ifdef GL_MESA_ycbcr_texture
  CONST_CAST(GLEW_MESA_ycbcr_texture) = glewGetExtension("GL_MESA_ycbcr_texture");
#endif /* GL_MESA_ycbcr_texture */
#ifdef GL_NVX_gpu_memory_info
  CONST_CAST(GLEW_NVX_gpu_memory_info) = glewGetExtension("GL_NVX_gpu_memory_info");
#endif /* GL_NVX_gpu_memory_info */
#ifdef GL_NV_blend_square
  CONST_CAST(GLEW_NV_blend_square) = glewGetExtension("GL_NV_blend_square");
#endif /* GL_NV_blend_square */
#ifdef GL_NV_conditional_render
  CONST_CAST(GLEW_NV_conditional_render) = glewGetExtension("GL_NV_conditional_render");
  if (glewExperimental || GLEW_NV_conditional_render) CONST_CAST(GLEW_NV_conditional_render) = !_glewInit_GL_NV_conditional_render(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_conditional_render */
#ifdef GL_NV_copy_depth_to_color
  CONST_CAST(GLEW_NV_copy_depth_to_color) = glewGetExtension("GL_NV_copy_depth_to_color");
#endif /* GL_NV_copy_depth_to_color */
#ifdef GL_NV_copy_image
  CONST_CAST(GLEW_NV_copy_image) = glewGetExtension("GL_NV_copy_image");
  if (glewExperimental || GLEW_NV_copy_image) CONST_CAST(GLEW_NV_copy_image) = !_glewInit_GL_NV_copy_image(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_copy_image */
#ifdef GL_NV_depth_buffer_float
  CONST_CAST(GLEW_NV_depth_buffer_float) = glewGetExtension("GL_NV_depth_buffer_float");
  if (glewExperimental || GLEW_NV_depth_buffer_float) CONST_CAST(GLEW_NV_depth_buffer_float) = !_glewInit_GL_NV_depth_buffer_float(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_depth_buffer_float */
#ifdef GL_NV_depth_clamp
  CONST_CAST(GLEW_NV_depth_clamp) = glewGetExtension("GL_NV_depth_clamp");
#endif /* GL_NV_depth_clamp */
#ifdef GL_NV_depth_range_unclamped
  CONST_CAST(GLEW_NV_depth_range_unclamped) = glewGetExtension("GL_NV_depth_range_unclamped");
#endif /* GL_NV_depth_range_unclamped */
#ifdef GL_NV_evaluators
  CONST_CAST(GLEW_NV_evaluators) = glewGetExtension("GL_NV_evaluators");
  if (glewExperimental || GLEW_NV_evaluators) CONST_CAST(GLEW_NV_evaluators) = !_glewInit_GL_NV_evaluators(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_evaluators */
#ifdef GL_NV_explicit_multisample
  CONST_CAST(GLEW_NV_explicit_multisample) = glewGetExtension("GL_NV_explicit_multisample");
  if (glewExperimental || GLEW_NV_explicit_multisample) CONST_CAST(GLEW_NV_explicit_multisample) = !_glewInit_GL_NV_explicit_multisample(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_explicit_multisample */
#ifdef GL_NV_fence
  CONST_CAST(GLEW_NV_fence) = glewGetExtension("GL_NV_fence");
  if (glewExperimental || GLEW_NV_fence) CONST_CAST(GLEW_NV_fence) = !_glewInit_GL_NV_fence(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_fence */
#ifdef GL_NV_float_buffer
  CONST_CAST(GLEW_NV_float_buffer) = glewGetExtension("GL_NV_float_buffer");
#endif /* GL_NV_float_buffer */
#ifdef GL_NV_fog_distance
  CONST_CAST(GLEW_NV_fog_distance) = glewGetExtension("GL_NV_fog_distance");
#endif /* GL_NV_fog_distance */
#ifdef GL_NV_fragment_program
  CONST_CAST(GLEW_NV_fragment_program) = glewGetExtension("GL_NV_fragment_program");
  if (glewExperimental || GLEW_NV_fragment_program) CONST_CAST(GLEW_NV_fragment_program) = !_glewInit_GL_NV_fragment_program(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_fragment_program */
#ifdef GL_NV_fragment_program2
  CONST_CAST(GLEW_NV_fragment_program2) = glewGetExtension("GL_NV_fragment_program2");
#endif /* GL_NV_fragment_program2 */
#ifdef GL_NV_fragment_program4
  CONST_CAST(GLEW_NV_fragment_program4) = glewGetExtension("GL_NV_gpu_program4");
#endif /* GL_NV_fragment_program4 */
#ifdef GL_NV_fragment_program_option
  CONST_CAST(GLEW_NV_fragment_program_option) = glewGetExtension("GL_NV_fragment_program_option");
#endif /* GL_NV_fragment_program_option */
#ifdef GL_NV_framebuffer_multisample_coverage
  CONST_CAST(GLEW_NV_framebuffer_multisample_coverage) = glewGetExtension("GL_NV_framebuffer_multisample_coverage");
  if (glewExperimental || GLEW_NV_framebuffer_multisample_coverage) CONST_CAST(GLEW_NV_framebuffer_multisample_coverage) = !_glewInit_GL_NV_framebuffer_multisample_coverage(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_framebuffer_multisample_coverage */
#ifdef GL_NV_geometry_program4
  CONST_CAST(GLEW_NV_geometry_program4) = glewGetExtension("GL_NV_gpu_program4");
  if (glewExperimental || GLEW_NV_geometry_program4) CONST_CAST(GLEW_NV_geometry_program4) = !_glewInit_GL_NV_geometry_program4(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_geometry_program4 */
#ifdef GL_NV_geometry_shader4
  CONST_CAST(GLEW_NV_geometry_shader4) = glewGetExtension("GL_NV_geometry_shader4");
#endif /* GL_NV_geometry_shader4 */
#ifdef GL_NV_gpu_program4
  CONST_CAST(GLEW_NV_gpu_program4) = glewGetExtension("GL_NV_gpu_program4");
  if (glewExperimental || GLEW_NV_gpu_program4) CONST_CAST(GLEW_NV_gpu_program4) = !_glewInit_GL_NV_gpu_program4(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_gpu_program4 */
#ifdef GL_NV_gpu_program5
  CONST_CAST(GLEW_NV_gpu_program5) = glewGetExtension("GL_NV_gpu_program5");
#endif /* GL_NV_gpu_program5 */
#ifdef GL_NV_gpu_program_fp64
  CONST_CAST(GLEW_NV_gpu_program_fp64) = glewGetExtension("GL_NV_gpu_program_fp64");
#endif /* GL_NV_gpu_program_fp64 */
#ifdef GL_NV_gpu_shader5
  CONST_CAST(GLEW_NV_gpu_shader5) = glewGetExtension("GL_NV_gpu_shader5");
  if (glewExperimental || GLEW_NV_gpu_shader5) CONST_CAST(GLEW_NV_gpu_shader5) = !_glewInit_GL_NV_gpu_shader5(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_gpu_shader5 */
#ifdef GL_NV_half_float
  CONST_CAST(GLEW_NV_half_float) = glewGetExtension("GL_NV_half_float");
  if (glewExperimental || GLEW_NV_half_float) CONST_CAST(GLEW_NV_half_float) = !_glewInit_GL_NV_half_float(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_half_float */
#ifdef GL_NV_light_max_exponent
  CONST_CAST(GLEW_NV_light_max_exponent) = glewGetExtension("GL_NV_light_max_exponent");
#endif /* GL_NV_light_max_exponent */
#ifdef GL_NV_multisample_coverage
  CONST_CAST(GLEW_NV_multisample_coverage) = glewGetExtension("GL_NV_multisample_coverage");
#endif /* GL_NV_multisample_coverage */
#ifdef GL_NV_multisample_filter_hint
  CONST_CAST(GLEW_NV_multisample_filter_hint) = glewGetExtension("GL_NV_multisample_filter_hint");
#endif /* GL_NV_multisample_filter_hint */
#ifdef GL_NV_occlusion_query
  CONST_CAST(GLEW_NV_occlusion_query) = glewGetExtension("GL_NV_occlusion_query");
  if (glewExperimental || GLEW_NV_occlusion_query) CONST_CAST(GLEW_NV_occlusion_query) = !_glewInit_GL_NV_occlusion_query(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_occlusion_query */
#ifdef GL_NV_packed_depth_stencil
  CONST_CAST(GLEW_NV_packed_depth_stencil) = glewGetExtension("GL_NV_packed_depth_stencil");
#endif /* GL_NV_packed_depth_stencil */
#ifdef GL_NV_parameter_buffer_object
  CONST_CAST(GLEW_NV_parameter_buffer_object) = glewGetExtension("GL_NV_parameter_buffer_object");
  if (glewExperimental || GLEW_NV_parameter_buffer_object) CONST_CAST(GLEW_NV_parameter_buffer_object) = !_glewInit_GL_NV_parameter_buffer_object(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_parameter_buffer_object */
#ifdef GL_NV_parameter_buffer_object2
  CONST_CAST(GLEW_NV_parameter_buffer_object2) = glewGetExtension("GL_NV_parameter_buffer_object2");
#endif /* GL_NV_parameter_buffer_object2 */
#ifdef GL_NV_pixel_data_range
  CONST_CAST(GLEW_NV_pixel_data_range) = glewGetExtension("GL_NV_pixel_data_range");
  if (glewExperimental || GLEW_NV_pixel_data_range) CONST_CAST(GLEW_NV_pixel_data_range) = !_glewInit_GL_NV_pixel_data_range(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_pixel_data_range */
#ifdef GL_NV_point_sprite
  CONST_CAST(GLEW_NV_point_sprite) = glewGetExtension("GL_NV_point_sprite");
  if (glewExperimental || GLEW_NV_point_sprite) CONST_CAST(GLEW_NV_point_sprite) = !_glewInit_GL_NV_point_sprite(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_point_sprite */
#ifdef GL_NV_present_video
  CONST_CAST(GLEW_NV_present_video) = glewGetExtension("GL_NV_present_video");
  if (glewExperimental || GLEW_NV_present_video) CONST_CAST(GLEW_NV_present_video) = !_glewInit_GL_NV_present_video(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_present_video */
#ifdef GL_NV_primitive_restart
  CONST_CAST(GLEW_NV_primitive_restart) = glewGetExtension("GL_NV_primitive_restart");
  if (glewExperimental || GLEW_NV_primitive_restart) CONST_CAST(GLEW_NV_primitive_restart) = !_glewInit_GL_NV_primitive_restart(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_primitive_restart */
#ifdef GL_NV_register_combiners
  CONST_CAST(GLEW_NV_register_combiners) = glewGetExtension("GL_NV_register_combiners");
  if (glewExperimental || GLEW_NV_register_combiners) CONST_CAST(GLEW_NV_register_combiners) = !_glewInit_GL_NV_register_combiners(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_register_combiners */
#ifdef GL_NV_register_combiners2
  CONST_CAST(GLEW_NV_register_combiners2) = glewGetExtension("GL_NV_register_combiners2");
  if (glewExperimental || GLEW_NV_register_combiners2) CONST_CAST(GLEW_NV_register_combiners2) = !_glewInit_GL_NV_register_combiners2(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_register_combiners2 */
#ifdef GL_NV_shader_buffer_load
  CONST_CAST(GLEW_NV_shader_buffer_load) = glewGetExtension("GL_NV_shader_buffer_load");
  if (glewExperimental || GLEW_NV_shader_buffer_load) CONST_CAST(GLEW_NV_shader_buffer_load) = !_glewInit_GL_NV_shader_buffer_load(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_shader_buffer_load */
#ifdef GL_NV_tessellation_program5
  CONST_CAST(GLEW_NV_tessellation_program5) = glewGetExtension("GL_NV_gpu_program5");
#endif /* GL_NV_tessellation_program5 */
#ifdef GL_NV_texgen_emboss
  CONST_CAST(GLEW_NV_texgen_emboss) = glewGetExtension("GL_NV_texgen_emboss");
#endif /* GL_NV_texgen_emboss */
#ifdef GL_NV_texgen_reflection
  CONST_CAST(GLEW_NV_texgen_reflection) = glewGetExtension("GL_NV_texgen_reflection");
#endif /* GL_NV_texgen_reflection */
#ifdef GL_NV_texture_barrier
  CONST_CAST(GLEW_NV_texture_barrier) = glewGetExtension("GL_NV_texture_barrier");
  if (glewExperimental || GLEW_NV_texture_barrier) CONST_CAST(GLEW_NV_texture_barrier) = !_glewInit_GL_NV_texture_barrier(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_texture_barrier */
#ifdef GL_NV_texture_compression_vtc
  CONST_CAST(GLEW_NV_texture_compression_vtc) = glewGetExtension("GL_NV_texture_compression_vtc");
#endif /* GL_NV_texture_compression_vtc */
#ifdef GL_NV_texture_env_combine4
  CONST_CAST(GLEW_NV_texture_env_combine4) = glewGetExtension("GL_NV_texture_env_combine4");
#endif /* GL_NV_texture_env_combine4 */
#ifdef GL_NV_texture_expand_normal
  CONST_CAST(GLEW_NV_texture_expand_normal) = glewGetExtension("GL_NV_texture_expand_normal");
#endif /* GL_NV_texture_expand_normal */
#ifdef GL_NV_texture_rectangle
  CONST_CAST(GLEW_NV_texture_rectangle) = glewGetExtension("GL_NV_texture_rectangle");
#endif /* GL_NV_texture_rectangle */
#ifdef GL_NV_texture_shader
  CONST_CAST(GLEW_NV_texture_shader) = glewGetExtension("GL_NV_texture_shader");
#endif /* GL_NV_texture_shader */
#ifdef GL_NV_texture_shader2
  CONST_CAST(GLEW_NV_texture_shader2) = glewGetExtension("GL_NV_texture_shader2");
#endif /* GL_NV_texture_shader2 */
#ifdef GL_NV_texture_shader3
  CONST_CAST(GLEW_NV_texture_shader3) = glewGetExtension("GL_NV_texture_shader3");
#endif /* GL_NV_texture_shader3 */
#ifdef GL_NV_transform_feedback
  CONST_CAST(GLEW_NV_transform_feedback) = glewGetExtension("GL_NV_transform_feedback");
  if (glewExperimental || GLEW_NV_transform_feedback) CONST_CAST(GLEW_NV_transform_feedback) = !_glewInit_GL_NV_transform_feedback(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_transform_feedback */
#ifdef GL_NV_transform_feedback2
  CONST_CAST(GLEW_NV_transform_feedback2) = glewGetExtension("GL_NV_transform_feedback2");
  if (glewExperimental || GLEW_NV_transform_feedback2) CONST_CAST(GLEW_NV_transform_feedback2) = !_glewInit_GL_NV_transform_feedback2(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_transform_feedback2 */
#ifdef GL_NV_vdpau_interop
  CONST_CAST(GLEW_NV_vdpau_interop) = glewGetExtension("GL_NV_vdpau_interop");
  if (glewExperimental || GLEW_NV_vdpau_interop) CONST_CAST(GLEW_NV_vdpau_interop) = !_glewInit_GL_NV_vdpau_interop(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_vdpau_interop */
#ifdef GL_NV_vertex_array_range
  CONST_CAST(GLEW_NV_vertex_array_range) = glewGetExtension("GL_NV_vertex_array_range");
  if (glewExperimental || GLEW_NV_vertex_array_range) CONST_CAST(GLEW_NV_vertex_array_range) = !_glewInit_GL_NV_vertex_array_range(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_vertex_array_range */
#ifdef GL_NV_vertex_array_range2
  CONST_CAST(GLEW_NV_vertex_array_range2) = glewGetExtension("GL_NV_vertex_array_range2");
#endif /* GL_NV_vertex_array_range2 */
#ifdef GL_NV_vertex_attrib_integer_64bit
  CONST_CAST(GLEW_NV_vertex_attrib_integer_64bit) = glewGetExtension("GL_NV_vertex_attrib_integer_64bit");
  if (glewExperimental || GLEW_NV_vertex_attrib_integer_64bit) CONST_CAST(GLEW_NV_vertex_attrib_integer_64bit) = !_glewInit_GL_NV_vertex_attrib_integer_64bit(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_vertex_attrib_integer_64bit */
#ifdef GL_NV_vertex_buffer_unified_memory
  CONST_CAST(GLEW_NV_vertex_buffer_unified_memory) = glewGetExtension("GL_NV_vertex_buffer_unified_memory");
  if (glewExperimental || GLEW_NV_vertex_buffer_unified_memory) CONST_CAST(GLEW_NV_vertex_buffer_unified_memory) = !_glewInit_GL_NV_vertex_buffer_unified_memory(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_vertex_buffer_unified_memory */
#ifdef GL_NV_vertex_program
  CONST_CAST(GLEW_NV_vertex_program) = glewGetExtension("GL_NV_vertex_program");
  if (glewExperimental || GLEW_NV_vertex_program) CONST_CAST(GLEW_NV_vertex_program) = !_glewInit_GL_NV_vertex_program(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_NV_vertex_program */
#ifdef GL_NV_vertex_program1_1
  CONST_CAST(GLEW_NV_vertex_program1_1) = glewGetExtension("GL_NV_vertex_program1_1");
#endif /* GL_NV_vertex_program1_1 */
#ifdef GL_NV_vertex_program2
  CONST_CAST(GLEW_NV_vertex_program2) = glewGetExtension("GL_NV_vertex_program2");
#endif /* GL_NV_vertex_program2 */
#ifdef GL_NV_vertex_program2_option
  CONST_CAST(GLEW_NV_vertex_program2_option) = glewGetExtension("GL_NV_vertex_program2_option");
#endif /* GL_NV_vertex_program2_option */
#ifdef GL_NV_vertex_program3
  CONST_CAST(GLEW_NV_vertex_program3) = glewGetExtension("GL_NV_vertex_program3");
#endif /* GL_NV_vertex_program3 */
#ifdef GL_NV_vertex_program4
  CONST_CAST(GLEW_NV_vertex_program4) = glewGetExtension("GL_NV_gpu_program4");
#endif /* GL_NV_vertex_program4 */
#ifdef GL_OES_byte_coordinates
  CONST_CAST(GLEW_OES_byte_coordinates) = glewGetExtension("GL_OES_byte_coordinates");
#endif /* GL_OES_byte_coordinates */
#ifdef GL_OES_compressed_paletted_texture
  CONST_CAST(GLEW_OES_compressed_paletted_texture) = glewGetExtension("GL_OES_compressed_paletted_texture");
#endif /* GL_OES_compressed_paletted_texture */
#ifdef GL_OES_read_format
  CONST_CAST(GLEW_OES_read_format) = glewGetExtension("GL_OES_read_format");
#endif /* GL_OES_read_format */
#ifdef GL_OES_single_precision
  CONST_CAST(GLEW_OES_single_precision) = glewGetExtension("GL_OES_single_precision");
  if (glewExperimental || GLEW_OES_single_precision) CONST_CAST(GLEW_OES_single_precision) = !_glewInit_GL_OES_single_precision(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_OES_single_precision */
#ifdef GL_OML_interlace
  CONST_CAST(GLEW_OML_interlace) = glewGetExtension("GL_OML_interlace");
#endif /* GL_OML_interlace */
#ifdef GL_OML_resample
  CONST_CAST(GLEW_OML_resample) = glewGetExtension("GL_OML_resample");
#endif /* GL_OML_resample */
#ifdef GL_OML_subsample
  CONST_CAST(GLEW_OML_subsample) = glewGetExtension("GL_OML_subsample");
#endif /* GL_OML_subsample */
#ifdef GL_PGI_misc_hints
  CONST_CAST(GLEW_PGI_misc_hints) = glewGetExtension("GL_PGI_misc_hints");
#endif /* GL_PGI_misc_hints */
#ifdef GL_PGI_vertex_hints
  CONST_CAST(GLEW_PGI_vertex_hints) = glewGetExtension("GL_PGI_vertex_hints");
#endif /* GL_PGI_vertex_hints */
#ifdef GL_REND_screen_coordinates
  CONST_CAST(GLEW_REND_screen_coordinates) = glewGetExtension("GL_REND_screen_coordinates");
#endif /* GL_REND_screen_coordinates */
#ifdef GL_S3_s3tc
  CONST_CAST(GLEW_S3_s3tc) = glewGetExtension("GL_S3_s3tc");
#endif /* GL_S3_s3tc */
#ifdef GL_SGIS_color_range
  CONST_CAST(GLEW_SGIS_color_range) = glewGetExtension("GL_SGIS_color_range");
#endif /* GL_SGIS_color_range */
#ifdef GL_SGIS_detail_texture
  CONST_CAST(GLEW_SGIS_detail_texture) = glewGetExtension("GL_SGIS_detail_texture");
  if (glewExperimental || GLEW_SGIS_detail_texture) CONST_CAST(GLEW_SGIS_detail_texture) = !_glewInit_GL_SGIS_detail_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIS_detail_texture */
#ifdef GL_SGIS_fog_function
  CONST_CAST(GLEW_SGIS_fog_function) = glewGetExtension("GL_SGIS_fog_function");
  if (glewExperimental || GLEW_SGIS_fog_function) CONST_CAST(GLEW_SGIS_fog_function) = !_glewInit_GL_SGIS_fog_function(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIS_fog_function */
#ifdef GL_SGIS_generate_mipmap
  CONST_CAST(GLEW_SGIS_generate_mipmap) = glewGetExtension("GL_SGIS_generate_mipmap");
#endif /* GL_SGIS_generate_mipmap */
#ifdef GL_SGIS_multisample
  CONST_CAST(GLEW_SGIS_multisample) = glewGetExtension("GL_SGIS_multisample");
  if (glewExperimental || GLEW_SGIS_multisample) CONST_CAST(GLEW_SGIS_multisample) = !_glewInit_GL_SGIS_multisample(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIS_multisample */
#ifdef GL_SGIS_pixel_texture
  CONST_CAST(GLEW_SGIS_pixel_texture) = glewGetExtension("GL_SGIS_pixel_texture");
#endif /* GL_SGIS_pixel_texture */
#ifdef GL_SGIS_point_line_texgen
  CONST_CAST(GLEW_SGIS_point_line_texgen) = glewGetExtension("GL_SGIS_point_line_texgen");
#endif /* GL_SGIS_point_line_texgen */
#ifdef GL_SGIS_sharpen_texture
  CONST_CAST(GLEW_SGIS_sharpen_texture) = glewGetExtension("GL_SGIS_sharpen_texture");
  if (glewExperimental || GLEW_SGIS_sharpen_texture) CONST_CAST(GLEW_SGIS_sharpen_texture) = !_glewInit_GL_SGIS_sharpen_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIS_sharpen_texture */
#ifdef GL_SGIS_texture4D
  CONST_CAST(GLEW_SGIS_texture4D) = glewGetExtension("GL_SGIS_texture4D");
  if (glewExperimental || GLEW_SGIS_texture4D) CONST_CAST(GLEW_SGIS_texture4D) = !_glewInit_GL_SGIS_texture4D(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIS_texture4D */
#ifdef GL_SGIS_texture_border_clamp
  CONST_CAST(GLEW_SGIS_texture_border_clamp) = glewGetExtension("GL_SGIS_texture_border_clamp");
#endif /* GL_SGIS_texture_border_clamp */
#ifdef GL_SGIS_texture_edge_clamp
  CONST_CAST(GLEW_SGIS_texture_edge_clamp) = glewGetExtension("GL_SGIS_texture_edge_clamp");
#endif /* GL_SGIS_texture_edge_clamp */
#ifdef GL_SGIS_texture_filter4
  CONST_CAST(GLEW_SGIS_texture_filter4) = glewGetExtension("GL_SGIS_texture_filter4");
  if (glewExperimental || GLEW_SGIS_texture_filter4) CONST_CAST(GLEW_SGIS_texture_filter4) = !_glewInit_GL_SGIS_texture_filter4(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIS_texture_filter4 */
#ifdef GL_SGIS_texture_lod
  CONST_CAST(GLEW_SGIS_texture_lod) = glewGetExtension("GL_SGIS_texture_lod");
#endif /* GL_SGIS_texture_lod */
#ifdef GL_SGIS_texture_select
  CONST_CAST(GLEW_SGIS_texture_select) = glewGetExtension("GL_SGIS_texture_select");
#endif /* GL_SGIS_texture_select */
#ifdef GL_SGIX_async
  CONST_CAST(GLEW_SGIX_async) = glewGetExtension("GL_SGIX_async");
  if (glewExperimental || GLEW_SGIX_async) CONST_CAST(GLEW_SGIX_async) = !_glewInit_GL_SGIX_async(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_async */
#ifdef GL_SGIX_async_histogram
  CONST_CAST(GLEW_SGIX_async_histogram) = glewGetExtension("GL_SGIX_async_histogram");
#endif /* GL_SGIX_async_histogram */
#ifdef GL_SGIX_async_pixel
  CONST_CAST(GLEW_SGIX_async_pixel) = glewGetExtension("GL_SGIX_async_pixel");
#endif /* GL_SGIX_async_pixel */
#ifdef GL_SGIX_blend_alpha_minmax
  CONST_CAST(GLEW_SGIX_blend_alpha_minmax) = glewGetExtension("GL_SGIX_blend_alpha_minmax");
#endif /* GL_SGIX_blend_alpha_minmax */
#ifdef GL_SGIX_clipmap
  CONST_CAST(GLEW_SGIX_clipmap) = glewGetExtension("GL_SGIX_clipmap");
#endif /* GL_SGIX_clipmap */
#ifdef GL_SGIX_convolution_accuracy
  CONST_CAST(GLEW_SGIX_convolution_accuracy) = glewGetExtension("GL_SGIX_convolution_accuracy");
#endif /* GL_SGIX_convolution_accuracy */
#ifdef GL_SGIX_depth_texture
  CONST_CAST(GLEW_SGIX_depth_texture) = glewGetExtension("GL_SGIX_depth_texture");
#endif /* GL_SGIX_depth_texture */
#ifdef GL_SGIX_flush_raster
  CONST_CAST(GLEW_SGIX_flush_raster) = glewGetExtension("GL_SGIX_flush_raster");
  if (glewExperimental || GLEW_SGIX_flush_raster) CONST_CAST(GLEW_SGIX_flush_raster) = !_glewInit_GL_SGIX_flush_raster(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_flush_raster */
#ifdef GL_SGIX_fog_offset
  CONST_CAST(GLEW_SGIX_fog_offset) = glewGetExtension("GL_SGIX_fog_offset");
#endif /* GL_SGIX_fog_offset */
#ifdef GL_SGIX_fog_texture
  CONST_CAST(GLEW_SGIX_fog_texture) = glewGetExtension("GL_SGIX_fog_texture");
  if (glewExperimental || GLEW_SGIX_fog_texture) CONST_CAST(GLEW_SGIX_fog_texture) = !_glewInit_GL_SGIX_fog_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_fog_texture */
#ifdef GL_SGIX_fragment_specular_lighting
  CONST_CAST(GLEW_SGIX_fragment_specular_lighting) = glewGetExtension("GL_SGIX_fragment_specular_lighting");
  if (glewExperimental || GLEW_SGIX_fragment_specular_lighting) CONST_CAST(GLEW_SGIX_fragment_specular_lighting) = !_glewInit_GL_SGIX_fragment_specular_lighting(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_fragment_specular_lighting */
#ifdef GL_SGIX_framezoom
  CONST_CAST(GLEW_SGIX_framezoom) = glewGetExtension("GL_SGIX_framezoom");
  if (glewExperimental || GLEW_SGIX_framezoom) CONST_CAST(GLEW_SGIX_framezoom) = !_glewInit_GL_SGIX_framezoom(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_framezoom */
#ifdef GL_SGIX_interlace
  CONST_CAST(GLEW_SGIX_interlace) = glewGetExtension("GL_SGIX_interlace");
#endif /* GL_SGIX_interlace */
#ifdef GL_SGIX_ir_instrument1
  CONST_CAST(GLEW_SGIX_ir_instrument1) = glewGetExtension("GL_SGIX_ir_instrument1");
#endif /* GL_SGIX_ir_instrument1 */
#ifdef GL_SGIX_list_priority
  CONST_CAST(GLEW_SGIX_list_priority) = glewGetExtension("GL_SGIX_list_priority");
#endif /* GL_SGIX_list_priority */
#ifdef GL_SGIX_pixel_texture
  CONST_CAST(GLEW_SGIX_pixel_texture) = glewGetExtension("GL_SGIX_pixel_texture");
  if (glewExperimental || GLEW_SGIX_pixel_texture) CONST_CAST(GLEW_SGIX_pixel_texture) = !_glewInit_GL_SGIX_pixel_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_pixel_texture */
#ifdef GL_SGIX_pixel_texture_bits
  CONST_CAST(GLEW_SGIX_pixel_texture_bits) = glewGetExtension("GL_SGIX_pixel_texture_bits");
#endif /* GL_SGIX_pixel_texture_bits */
#ifdef GL_SGIX_reference_plane
  CONST_CAST(GLEW_SGIX_reference_plane) = glewGetExtension("GL_SGIX_reference_plane");
  if (glewExperimental || GLEW_SGIX_reference_plane) CONST_CAST(GLEW_SGIX_reference_plane) = !_glewInit_GL_SGIX_reference_plane(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_reference_plane */
#ifdef GL_SGIX_resample
  CONST_CAST(GLEW_SGIX_resample) = glewGetExtension("GL_SGIX_resample");
#endif /* GL_SGIX_resample */
#ifdef GL_SGIX_shadow
  CONST_CAST(GLEW_SGIX_shadow) = glewGetExtension("GL_SGIX_shadow");
#endif /* GL_SGIX_shadow */
#ifdef GL_SGIX_shadow_ambient
  CONST_CAST(GLEW_SGIX_shadow_ambient) = glewGetExtension("GL_SGIX_shadow_ambient");
#endif /* GL_SGIX_shadow_ambient */
#ifdef GL_SGIX_sprite
  CONST_CAST(GLEW_SGIX_sprite) = glewGetExtension("GL_SGIX_sprite");
  if (glewExperimental || GLEW_SGIX_sprite) CONST_CAST(GLEW_SGIX_sprite) = !_glewInit_GL_SGIX_sprite(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_sprite */
#ifdef GL_SGIX_tag_sample_buffer
  CONST_CAST(GLEW_SGIX_tag_sample_buffer) = glewGetExtension("GL_SGIX_tag_sample_buffer");
  if (glewExperimental || GLEW_SGIX_tag_sample_buffer) CONST_CAST(GLEW_SGIX_tag_sample_buffer) = !_glewInit_GL_SGIX_tag_sample_buffer(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGIX_tag_sample_buffer */
#ifdef GL_SGIX_texture_add_env
  CONST_CAST(GLEW_SGIX_texture_add_env) = glewGetExtension("GL_SGIX_texture_add_env");
#endif /* GL_SGIX_texture_add_env */
#ifdef GL_SGIX_texture_coordinate_clamp
  CONST_CAST(GLEW_SGIX_texture_coordinate_clamp) = glewGetExtension("GL_SGIX_texture_coordinate_clamp");
#endif /* GL_SGIX_texture_coordinate_clamp */
#ifdef GL_SGIX_texture_lod_bias
  CONST_CAST(GLEW_SGIX_texture_lod_bias) = glewGetExtension("GL_SGIX_texture_lod_bias");
#endif /* GL_SGIX_texture_lod_bias */
#ifdef GL_SGIX_texture_multi_buffer
  CONST_CAST(GLEW_SGIX_texture_multi_buffer) = glewGetExtension("GL_SGIX_texture_multi_buffer");
#endif /* GL_SGIX_texture_multi_buffer */
#ifdef GL_SGIX_texture_range
  CONST_CAST(GLEW_SGIX_texture_range) = glewGetExtension("GL_SGIX_texture_range");
#endif /* GL_SGIX_texture_range */
#ifdef GL_SGIX_texture_scale_bias
  CONST_CAST(GLEW_SGIX_texture_scale_bias) = glewGetExtension("GL_SGIX_texture_scale_bias");
#endif /* GL_SGIX_texture_scale_bias */
#ifdef GL_SGIX_vertex_preclip
  CONST_CAST(GLEW_SGIX_vertex_preclip) = glewGetExtension("GL_SGIX_vertex_preclip");
#endif /* GL_SGIX_vertex_preclip */
#ifdef GL_SGIX_vertex_preclip_hint
  CONST_CAST(GLEW_SGIX_vertex_preclip_hint) = glewGetExtension("GL_SGIX_vertex_preclip_hint");
#endif /* GL_SGIX_vertex_preclip_hint */
#ifdef GL_SGIX_ycrcb
  CONST_CAST(GLEW_SGIX_ycrcb) = glewGetExtension("GL_SGIX_ycrcb");
#endif /* GL_SGIX_ycrcb */
#ifdef GL_SGI_color_matrix
  CONST_CAST(GLEW_SGI_color_matrix) = glewGetExtension("GL_SGI_color_matrix");
#endif /* GL_SGI_color_matrix */
#ifdef GL_SGI_color_table
  CONST_CAST(GLEW_SGI_color_table) = glewGetExtension("GL_SGI_color_table");
  if (glewExperimental || GLEW_SGI_color_table) CONST_CAST(GLEW_SGI_color_table) = !_glewInit_GL_SGI_color_table(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SGI_color_table */
#ifdef GL_SGI_texture_color_table
  CONST_CAST(GLEW_SGI_texture_color_table) = glewGetExtension("GL_SGI_texture_color_table");
#endif /* GL_SGI_texture_color_table */
#ifdef GL_SUNX_constant_data
  CONST_CAST(GLEW_SUNX_constant_data) = glewGetExtension("GL_SUNX_constant_data");
  if (glewExperimental || GLEW_SUNX_constant_data) CONST_CAST(GLEW_SUNX_constant_data) = !_glewInit_GL_SUNX_constant_data(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SUNX_constant_data */
#ifdef GL_SUN_convolution_border_modes
  CONST_CAST(GLEW_SUN_convolution_border_modes) = glewGetExtension("GL_SUN_convolution_border_modes");
#endif /* GL_SUN_convolution_border_modes */
#ifdef GL_SUN_global_alpha
  CONST_CAST(GLEW_SUN_global_alpha) = glewGetExtension("GL_SUN_global_alpha");
  if (glewExperimental || GLEW_SUN_global_alpha) CONST_CAST(GLEW_SUN_global_alpha) = !_glewInit_GL_SUN_global_alpha(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SUN_global_alpha */
#ifdef GL_SUN_mesh_array
  CONST_CAST(GLEW_SUN_mesh_array) = glewGetExtension("GL_SUN_mesh_array");
#endif /* GL_SUN_mesh_array */
#ifdef GL_SUN_read_video_pixels
  CONST_CAST(GLEW_SUN_read_video_pixels) = glewGetExtension("GL_SUN_read_video_pixels");
  if (glewExperimental || GLEW_SUN_read_video_pixels) CONST_CAST(GLEW_SUN_read_video_pixels) = !_glewInit_GL_SUN_read_video_pixels(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SUN_read_video_pixels */
#ifdef GL_SUN_slice_accum
  CONST_CAST(GLEW_SUN_slice_accum) = glewGetExtension("GL_SUN_slice_accum");
#endif /* GL_SUN_slice_accum */
#ifdef GL_SUN_triangle_list
  CONST_CAST(GLEW_SUN_triangle_list) = glewGetExtension("GL_SUN_triangle_list");
  if (glewExperimental || GLEW_SUN_triangle_list) CONST_CAST(GLEW_SUN_triangle_list) = !_glewInit_GL_SUN_triangle_list(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SUN_triangle_list */
#ifdef GL_SUN_vertex
  CONST_CAST(GLEW_SUN_vertex) = glewGetExtension("GL_SUN_vertex");
  if (glewExperimental || GLEW_SUN_vertex) CONST_CAST(GLEW_SUN_vertex) = !_glewInit_GL_SUN_vertex(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_SUN_vertex */
#ifdef GL_WIN_phong_shading
  CONST_CAST(GLEW_WIN_phong_shading) = glewGetExtension("GL_WIN_phong_shading");
#endif /* GL_WIN_phong_shading */
#ifdef GL_WIN_specular_fog
  CONST_CAST(GLEW_WIN_specular_fog) = glewGetExtension("GL_WIN_specular_fog");
#endif /* GL_WIN_specular_fog */
#ifdef GL_WIN_swap_hint
  CONST_CAST(GLEW_WIN_swap_hint) = glewGetExtension("GL_WIN_swap_hint");
  if (glewExperimental || GLEW_WIN_swap_hint) CONST_CAST(GLEW_WIN_swap_hint) = !_glewInit_GL_WIN_swap_hint(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GL_WIN_swap_hint */

  return GLEW_OK;
}


#if defined(_WIN32)

#if !defined(GLEW_MX)

#endif /* !GLEW_MX */


/* ------------------------------------------------------------------------- */

GLboolean wglewGetExtension (const char* name)
{    

  return GL_FALSE;
}

GLenum wglewContextInit (WGLEW_CONTEXT_ARG_DEF_LIST)
{

  return GLEW_OK;
}

#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)

PFNGLXCHOOSEVISUALPROC __glewXChooseVisual = NULL;
PFNGLXCOPYCONTEXTPROC __glewXCopyContext = NULL;
PFNGLXCREATECONTEXTPROC __glewXCreateContext = NULL;
PFNGLXCREATEGLXPIXMAPPROC __glewXCreateGLXPixmap = NULL;
PFNGLXDESTROYCONTEXTPROC __glewXDestroyContext = NULL;
PFNGLXDESTROYGLXPIXMAPPROC __glewXDestroyGLXPixmap = NULL;
PFNGLXGETCONFIGPROC __glewXGetConfig = NULL;
PFNGLXGETCURRENTCONTEXTPROC __glewXGetCurrentContext = NULL;
PFNGLXGETCURRENTDRAWABLEPROC __glewXGetCurrentDrawable = NULL;
PFNGLXISDIRECTPROC __glewXIsDirect = NULL;
PFNGLXMAKECURRENTPROC __glewXMakeCurrent = NULL;
PFNGLXQUERYEXTENSIONPROC __glewXQueryExtension = NULL;
PFNGLXQUERYVERSIONPROC __glewXQueryVersion = NULL;
PFNGLXSWAPBUFFERSPROC __glewXSwapBuffers = NULL;
PFNGLXUSEXFONTPROC __glewXUseXFont = NULL;
PFNGLXWAITGLPROC __glewXWaitGL = NULL;
PFNGLXWAITXPROC __glewXWaitX = NULL;

PFNGLXGETCLIENTSTRINGPROC __glewXGetClientString = NULL;
PFNGLXQUERYEXTENSIONSSTRINGPROC __glewXQueryExtensionsString = NULL;
PFNGLXQUERYSERVERSTRINGPROC __glewXQueryServerString = NULL;

PFNGLXGETCURRENTDISPLAYPROC __glewXGetCurrentDisplay = NULL;

PFNGLXCHOOSEFBCONFIGPROC __glewXChooseFBConfig = NULL;
PFNGLXCREATENEWCONTEXTPROC __glewXCreateNewContext = NULL;
PFNGLXCREATEPBUFFERPROC __glewXCreatePbuffer = NULL;
PFNGLXCREATEPIXMAPPROC __glewXCreatePixmap = NULL;
PFNGLXCREATEWINDOWPROC __glewXCreateWindow = NULL;
PFNGLXDESTROYPBUFFERPROC __glewXDestroyPbuffer = NULL;
PFNGLXDESTROYPIXMAPPROC __glewXDestroyPixmap = NULL;
PFNGLXDESTROYWINDOWPROC __glewXDestroyWindow = NULL;
PFNGLXGETCURRENTREADDRAWABLEPROC __glewXGetCurrentReadDrawable = NULL;
PFNGLXGETFBCONFIGATTRIBPROC __glewXGetFBConfigAttrib = NULL;
PFNGLXGETFBCONFIGSPROC __glewXGetFBConfigs = NULL;
PFNGLXGETSELECTEDEVENTPROC __glewXGetSelectedEvent = NULL;
PFNGLXGETVISUALFROMFBCONFIGPROC __glewXGetVisualFromFBConfig = NULL;
PFNGLXMAKECONTEXTCURRENTPROC __glewXMakeContextCurrent = NULL;
PFNGLXQUERYCONTEXTPROC __glewXQueryContext = NULL;
PFNGLXQUERYDRAWABLEPROC __glewXQueryDrawable = NULL;
PFNGLXSELECTEVENTPROC __glewXSelectEvent = NULL;

PFNGLXCREATECONTEXTATTRIBSARBPROC __glewXCreateContextAttribsARB = NULL;

PFNGLXBINDTEXIMAGEATIPROC __glewXBindTexImageATI = NULL;
PFNGLXDRAWABLEATTRIBATIPROC __glewXDrawableAttribATI = NULL;
PFNGLXRELEASETEXIMAGEATIPROC __glewXReleaseTexImageATI = NULL;

PFNGLXFREECONTEXTEXTPROC __glewXFreeContextEXT = NULL;
PFNGLXGETCONTEXTIDEXTPROC __glewXGetContextIDEXT = NULL;
PFNGLXIMPORTCONTEXTEXTPROC __glewXImportContextEXT = NULL;
PFNGLXQUERYCONTEXTINFOEXTPROC __glewXQueryContextInfoEXT = NULL;

PFNGLXSWAPINTERVALEXTPROC __glewXSwapIntervalEXT = NULL;

PFNGLXBINDTEXIMAGEEXTPROC __glewXBindTexImageEXT = NULL;
PFNGLXRELEASETEXIMAGEEXTPROC __glewXReleaseTexImageEXT = NULL;

PFNGLXGETAGPOFFSETMESAPROC __glewXGetAGPOffsetMESA = NULL;

PFNGLXCOPYSUBBUFFERMESAPROC __glewXCopySubBufferMESA = NULL;

PFNGLXCREATEGLXPIXMAPMESAPROC __glewXCreateGLXPixmapMESA = NULL;

PFNGLXRELEASEBUFFERSMESAPROC __glewXReleaseBuffersMESA = NULL;

PFNGLXSET3DFXMODEMESAPROC __glewXSet3DfxModeMESA = NULL;

PFNGLXCOPYIMAGESUBDATANVPROC __glewXCopyImageSubDataNV = NULL;

PFNGLXBINDVIDEODEVICENVPROC __glewXBindVideoDeviceNV = NULL;
PFNGLXENUMERATEVIDEODEVICESNVPROC __glewXEnumerateVideoDevicesNV = NULL;

PFNGLXBINDSWAPBARRIERNVPROC __glewXBindSwapBarrierNV = NULL;
PFNGLXJOINSWAPGROUPNVPROC __glewXJoinSwapGroupNV = NULL;
PFNGLXQUERYFRAMECOUNTNVPROC __glewXQueryFrameCountNV = NULL;
PFNGLXQUERYMAXSWAPGROUPSNVPROC __glewXQueryMaxSwapGroupsNV = NULL;
PFNGLXQUERYSWAPGROUPNVPROC __glewXQuerySwapGroupNV = NULL;
PFNGLXRESETFRAMECOUNTNVPROC __glewXResetFrameCountNV = NULL;

PFNGLXALLOCATEMEMORYNVPROC __glewXAllocateMemoryNV = NULL;
PFNGLXFREEMEMORYNVPROC __glewXFreeMemoryNV = NULL;

PFNGLXBINDVIDEOIMAGENVPROC __glewXBindVideoImageNV = NULL;
PFNGLXGETVIDEODEVICENVPROC __glewXGetVideoDeviceNV = NULL;
PFNGLXGETVIDEOINFONVPROC __glewXGetVideoInfoNV = NULL;
PFNGLXRELEASEVIDEODEVICENVPROC __glewXReleaseVideoDeviceNV = NULL;
PFNGLXRELEASEVIDEOIMAGENVPROC __glewXReleaseVideoImageNV = NULL;
PFNGLXSENDPBUFFERTOVIDEONVPROC __glewXSendPbufferToVideoNV = NULL;

#ifdef GLX_OML_sync_control
PFNGLXGETMSCRATEOMLPROC __glewXGetMscRateOML = NULL;
PFNGLXGETSYNCVALUESOMLPROC __glewXGetSyncValuesOML = NULL;
PFNGLXSWAPBUFFERSMSCOMLPROC __glewXSwapBuffersMscOML = NULL;
PFNGLXWAITFORMSCOMLPROC __glewXWaitForMscOML = NULL;
PFNGLXWAITFORSBCOMLPROC __glewXWaitForSbcOML = NULL;
#endif

PFNGLXCHOOSEFBCONFIGSGIXPROC __glewXChooseFBConfigSGIX = NULL;
PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC __glewXCreateContextWithConfigSGIX = NULL;
PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC __glewXCreateGLXPixmapWithConfigSGIX = NULL;
PFNGLXGETFBCONFIGATTRIBSGIXPROC __glewXGetFBConfigAttribSGIX = NULL;
PFNGLXGETFBCONFIGFROMVISUALSGIXPROC __glewXGetFBConfigFromVisualSGIX = NULL;
PFNGLXGETVISUALFROMFBCONFIGSGIXPROC __glewXGetVisualFromFBConfigSGIX = NULL;

PFNGLXBINDHYPERPIPESGIXPROC __glewXBindHyperpipeSGIX = NULL;
PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC __glewXDestroyHyperpipeConfigSGIX = NULL;
PFNGLXHYPERPIPEATTRIBSGIXPROC __glewXHyperpipeAttribSGIX = NULL;
PFNGLXHYPERPIPECONFIGSGIXPROC __glewXHyperpipeConfigSGIX = NULL;
PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC __glewXQueryHyperpipeAttribSGIX = NULL;
PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC __glewXQueryHyperpipeBestAttribSGIX = NULL;
PFNGLXQUERYHYPERPIPECONFIGSGIXPROC __glewXQueryHyperpipeConfigSGIX = NULL;
PFNGLXQUERYHYPERPIPENETWORKSGIXPROC __glewXQueryHyperpipeNetworkSGIX = NULL;

PFNGLXCREATEGLXPBUFFERSGIXPROC __glewXCreateGLXPbufferSGIX = NULL;
PFNGLXDESTROYGLXPBUFFERSGIXPROC __glewXDestroyGLXPbufferSGIX = NULL;
PFNGLXGETSELECTEDEVENTSGIXPROC __glewXGetSelectedEventSGIX = NULL;
PFNGLXQUERYGLXPBUFFERSGIXPROC __glewXQueryGLXPbufferSGIX = NULL;
PFNGLXSELECTEVENTSGIXPROC __glewXSelectEventSGIX = NULL;

PFNGLXBINDSWAPBARRIERSGIXPROC __glewXBindSwapBarrierSGIX = NULL;
PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC __glewXQueryMaxSwapBarriersSGIX = NULL;

PFNGLXJOINSWAPGROUPSGIXPROC __glewXJoinSwapGroupSGIX = NULL;

PFNGLXBINDCHANNELTOWINDOWSGIXPROC __glewXBindChannelToWindowSGIX = NULL;
PFNGLXCHANNELRECTSGIXPROC __glewXChannelRectSGIX = NULL;
PFNGLXCHANNELRECTSYNCSGIXPROC __glewXChannelRectSyncSGIX = NULL;
PFNGLXQUERYCHANNELDELTASSGIXPROC __glewXQueryChannelDeltasSGIX = NULL;
PFNGLXQUERYCHANNELRECTSGIXPROC __glewXQueryChannelRectSGIX = NULL;

PFNGLXCUSHIONSGIPROC __glewXCushionSGI = NULL;

PFNGLXGETCURRENTREADDRAWABLESGIPROC __glewXGetCurrentReadDrawableSGI = NULL;
PFNGLXMAKECURRENTREADSGIPROC __glewXMakeCurrentReadSGI = NULL;

PFNGLXSWAPINTERVALSGIPROC __glewXSwapIntervalSGI = NULL;

PFNGLXGETVIDEOSYNCSGIPROC __glewXGetVideoSyncSGI = NULL;
PFNGLXWAITVIDEOSYNCSGIPROC __glewXWaitVideoSyncSGI = NULL;

PFNGLXGETTRANSPARENTINDEXSUNPROC __glewXGetTransparentIndexSUN = NULL;

PFNGLXGETVIDEORESIZESUNPROC __glewXGetVideoResizeSUN = NULL;
PFNGLXVIDEORESIZESUNPROC __glewXVideoResizeSUN = NULL;

#if !defined(GLEW_MX)
GLboolean __GLXEW_VERSION_1_0 = GL_FALSE;
GLboolean __GLXEW_VERSION_1_1 = GL_FALSE;
GLboolean __GLXEW_VERSION_1_2 = GL_FALSE;
GLboolean __GLXEW_VERSION_1_3 = GL_FALSE;
GLboolean __GLXEW_VERSION_1_4 = GL_FALSE;
GLboolean __GLXEW_3DFX_multisample = GL_FALSE;
GLboolean __GLXEW_AMD_gpu_association = GL_FALSE;
GLboolean __GLXEW_ARB_create_context = GL_FALSE;
GLboolean __GLXEW_ARB_create_context_profile = GL_FALSE;
GLboolean __GLXEW_ARB_create_context_robustness = GL_FALSE;
GLboolean __GLXEW_ARB_fbconfig_float = GL_FALSE;
GLboolean __GLXEW_ARB_framebuffer_sRGB = GL_FALSE;
GLboolean __GLXEW_ARB_get_proc_address = GL_FALSE;
GLboolean __GLXEW_ARB_multisample = GL_FALSE;
GLboolean __GLXEW_ARB_vertex_buffer_object = GL_FALSE;
GLboolean __GLXEW_ATI_pixel_format_float = GL_FALSE;
GLboolean __GLXEW_ATI_render_texture = GL_FALSE;
GLboolean __GLXEW_EXT_create_context_es2_profile = GL_FALSE;
GLboolean __GLXEW_EXT_fbconfig_packed_float = GL_FALSE;
GLboolean __GLXEW_EXT_framebuffer_sRGB = GL_FALSE;
GLboolean __GLXEW_EXT_import_context = GL_FALSE;
GLboolean __GLXEW_EXT_scene_marker = GL_FALSE;
GLboolean __GLXEW_EXT_swap_control = GL_FALSE;
GLboolean __GLXEW_EXT_texture_from_pixmap = GL_FALSE;
GLboolean __GLXEW_EXT_visual_info = GL_FALSE;
GLboolean __GLXEW_EXT_visual_rating = GL_FALSE;
GLboolean __GLXEW_INTEL_swap_event = GL_FALSE;
GLboolean __GLXEW_MESA_agp_offset = GL_FALSE;
GLboolean __GLXEW_MESA_copy_sub_buffer = GL_FALSE;
GLboolean __GLXEW_MESA_pixmap_colormap = GL_FALSE;
GLboolean __GLXEW_MESA_release_buffers = GL_FALSE;
GLboolean __GLXEW_MESA_set_3dfx_mode = GL_FALSE;
GLboolean __GLXEW_NV_copy_image = GL_FALSE;
GLboolean __GLXEW_NV_float_buffer = GL_FALSE;
GLboolean __GLXEW_NV_multisample_coverage = GL_FALSE;
GLboolean __GLXEW_NV_present_video = GL_FALSE;
GLboolean __GLXEW_NV_swap_group = GL_FALSE;
GLboolean __GLXEW_NV_vertex_array_range = GL_FALSE;
GLboolean __GLXEW_NV_video_output = GL_FALSE;
GLboolean __GLXEW_OML_swap_method = GL_FALSE;
#ifdef GLX_OML_sync_control
GLboolean __GLXEW_OML_sync_control = GL_FALSE;
#endif
GLboolean __GLXEW_SGIS_blended_overlay = GL_FALSE;
GLboolean __GLXEW_SGIS_color_range = GL_FALSE;
GLboolean __GLXEW_SGIS_multisample = GL_FALSE;
GLboolean __GLXEW_SGIS_shared_multisample = GL_FALSE;
GLboolean __GLXEW_SGIX_fbconfig = GL_FALSE;
GLboolean __GLXEW_SGIX_hyperpipe = GL_FALSE;
GLboolean __GLXEW_SGIX_pbuffer = GL_FALSE;
GLboolean __GLXEW_SGIX_swap_barrier = GL_FALSE;
GLboolean __GLXEW_SGIX_swap_group = GL_FALSE;
GLboolean __GLXEW_SGIX_video_resize = GL_FALSE;
GLboolean __GLXEW_SGIX_visual_select_group = GL_FALSE;
GLboolean __GLXEW_SGI_cushion = GL_FALSE;
GLboolean __GLXEW_SGI_make_current_read = GL_FALSE;
GLboolean __GLXEW_SGI_swap_control = GL_FALSE;
GLboolean __GLXEW_SGI_video_sync = GL_FALSE;
GLboolean __GLXEW_SUN_get_transparent_index = GL_FALSE;
GLboolean __GLXEW_SUN_video_resize = GL_FALSE;

#endif /* !GLEW_MX */

#ifdef GLX_VERSION_1_0

static GLboolean _glewInit_GLX_VERSION_1_0 (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXChooseVisual = (PFNGLXCHOOSEVISUALPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXChooseVisual"))) == NULL) || r;
  r = ((glXCopyContext = (PFNGLXCOPYCONTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCopyContext"))) == NULL) || r;
  r = ((glXCreateContext = (PFNGLXCREATECONTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateContext"))) == NULL) || r;
  r = ((glXCreateGLXPixmap = (PFNGLXCREATEGLXPIXMAPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateGLXPixmap"))) == NULL) || r;
  r = ((glXDestroyContext = (PFNGLXDESTROYCONTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDestroyContext"))) == NULL) || r;
  r = ((glXDestroyGLXPixmap = (PFNGLXDESTROYGLXPIXMAPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDestroyGLXPixmap"))) == NULL) || r;
  r = ((glXGetConfig = (PFNGLXGETCONFIGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetConfig"))) == NULL) || r;
  r = ((glXGetCurrentContext = (PFNGLXGETCURRENTCONTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetCurrentContext"))) == NULL) || r;
  r = ((glXGetCurrentDrawable = (PFNGLXGETCURRENTDRAWABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetCurrentDrawable"))) == NULL) || r;
  r = ((glXIsDirect = (PFNGLXISDIRECTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXIsDirect"))) == NULL) || r;
  r = ((glXMakeCurrent = (PFNGLXMAKECURRENTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXMakeCurrent"))) == NULL) || r;
  r = ((glXQueryExtension = (PFNGLXQUERYEXTENSIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryExtension"))) == NULL) || r;
  r = ((glXQueryVersion = (PFNGLXQUERYVERSIONPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryVersion"))) == NULL) || r;
  r = ((glXSwapBuffers = (PFNGLXSWAPBUFFERSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSwapBuffers"))) == NULL) || r;
  r = ((glXUseXFont = (PFNGLXUSEXFONTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXUseXFont"))) == NULL) || r;
  r = ((glXWaitGL = (PFNGLXWAITGLPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXWaitGL"))) == NULL) || r;
  r = ((glXWaitX = (PFNGLXWAITXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXWaitX"))) == NULL) || r;

  return r;
}

#endif /* GLX_VERSION_1_0 */

#ifdef GLX_VERSION_1_1

static GLboolean _glewInit_GLX_VERSION_1_1 (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetClientString = (PFNGLXGETCLIENTSTRINGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetClientString"))) == NULL) || r;
  r = ((glXQueryExtensionsString = (PFNGLXQUERYEXTENSIONSSTRINGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryExtensionsString"))) == NULL) || r;
  r = ((glXQueryServerString = (PFNGLXQUERYSERVERSTRINGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryServerString"))) == NULL) || r;

  return r;
}

#endif /* GLX_VERSION_1_1 */

#ifdef GLX_VERSION_1_2

static GLboolean _glewInit_GLX_VERSION_1_2 (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetCurrentDisplay = (PFNGLXGETCURRENTDISPLAYPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetCurrentDisplay"))) == NULL) || r;

  return r;
}

#endif /* GLX_VERSION_1_2 */

#ifdef GLX_VERSION_1_3

static GLboolean _glewInit_GLX_VERSION_1_3 (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXChooseFBConfig"))) == NULL) || r;
  r = ((glXCreateNewContext = (PFNGLXCREATENEWCONTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateNewContext"))) == NULL) || r;
  r = ((glXCreatePbuffer = (PFNGLXCREATEPBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreatePbuffer"))) == NULL) || r;
  r = ((glXCreatePixmap = (PFNGLXCREATEPIXMAPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreatePixmap"))) == NULL) || r;
  r = ((glXCreateWindow = (PFNGLXCREATEWINDOWPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateWindow"))) == NULL) || r;
  r = ((glXDestroyPbuffer = (PFNGLXDESTROYPBUFFERPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDestroyPbuffer"))) == NULL) || r;
  r = ((glXDestroyPixmap = (PFNGLXDESTROYPIXMAPPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDestroyPixmap"))) == NULL) || r;
  r = ((glXDestroyWindow = (PFNGLXDESTROYWINDOWPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDestroyWindow"))) == NULL) || r;
  r = ((glXGetCurrentReadDrawable = (PFNGLXGETCURRENTREADDRAWABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetCurrentReadDrawable"))) == NULL) || r;
  r = ((glXGetFBConfigAttrib = (PFNGLXGETFBCONFIGATTRIBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetFBConfigAttrib"))) == NULL) || r;
  r = ((glXGetFBConfigs = (PFNGLXGETFBCONFIGSPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetFBConfigs"))) == NULL) || r;
  r = ((glXGetSelectedEvent = (PFNGLXGETSELECTEDEVENTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetSelectedEvent"))) == NULL) || r;
  r = ((glXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetVisualFromFBConfig"))) == NULL) || r;
  r = ((glXMakeContextCurrent = (PFNGLXMAKECONTEXTCURRENTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXMakeContextCurrent"))) == NULL) || r;
  r = ((glXQueryContext = (PFNGLXQUERYCONTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryContext"))) == NULL) || r;
  r = ((glXQueryDrawable = (PFNGLXQUERYDRAWABLEPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryDrawable"))) == NULL) || r;
  r = ((glXSelectEvent = (PFNGLXSELECTEVENTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSelectEvent"))) == NULL) || r;

  return r;
}

#endif /* GLX_VERSION_1_3 */

#ifdef GLX_VERSION_1_4

#endif /* GLX_VERSION_1_4 */

#ifdef GLX_3DFX_multisample

#endif /* GLX_3DFX_multisample */

#ifdef GLX_AMD_gpu_association

#endif /* GLX_AMD_gpu_association */

#ifdef GLX_ARB_create_context

static GLboolean _glewInit_GLX_ARB_create_context (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateContextAttribsARB"))) == NULL) || r;

  return r;
}

#endif /* GLX_ARB_create_context */

#ifdef GLX_ARB_create_context_profile

#endif /* GLX_ARB_create_context_profile */

#ifdef GLX_ARB_create_context_robustness

#endif /* GLX_ARB_create_context_robustness */

#ifdef GLX_ARB_fbconfig_float

#endif /* GLX_ARB_fbconfig_float */

#ifdef GLX_ARB_framebuffer_sRGB

#endif /* GLX_ARB_framebuffer_sRGB */

#ifdef GLX_ARB_get_proc_address

#endif /* GLX_ARB_get_proc_address */

#ifdef GLX_ARB_multisample

#endif /* GLX_ARB_multisample */

#ifdef GLX_ARB_vertex_buffer_object

#endif /* GLX_ARB_vertex_buffer_object */

#ifdef GLX_ATI_pixel_format_float

#endif /* GLX_ATI_pixel_format_float */

#ifdef GLX_ATI_render_texture

static GLboolean _glewInit_GLX_ATI_render_texture (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindTexImageATI = (PFNGLXBINDTEXIMAGEATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindTexImageATI"))) == NULL) || r;
  r = ((glXDrawableAttribATI = (PFNGLXDRAWABLEATTRIBATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDrawableAttribATI"))) == NULL) || r;
  r = ((glXReleaseTexImageATI = (PFNGLXRELEASETEXIMAGEATIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXReleaseTexImageATI"))) == NULL) || r;

  return r;
}

#endif /* GLX_ATI_render_texture */

#ifdef GLX_EXT_create_context_es2_profile

#endif /* GLX_EXT_create_context_es2_profile */

#ifdef GLX_EXT_fbconfig_packed_float

#endif /* GLX_EXT_fbconfig_packed_float */

#ifdef GLX_EXT_framebuffer_sRGB

#endif /* GLX_EXT_framebuffer_sRGB */

#ifdef GLX_EXT_import_context

static GLboolean _glewInit_GLX_EXT_import_context (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXFreeContextEXT = (PFNGLXFREECONTEXTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXFreeContextEXT"))) == NULL) || r;
  r = ((glXGetContextIDEXT = (PFNGLXGETCONTEXTIDEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetContextIDEXT"))) == NULL) || r;
  r = ((glXImportContextEXT = (PFNGLXIMPORTCONTEXTEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXImportContextEXT"))) == NULL) || r;
  r = ((glXQueryContextInfoEXT = (PFNGLXQUERYCONTEXTINFOEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryContextInfoEXT"))) == NULL) || r;

  return r;
}

#endif /* GLX_EXT_import_context */

#ifdef GLX_EXT_scene_marker

#endif /* GLX_EXT_scene_marker */

#ifdef GLX_EXT_swap_control

static GLboolean _glewInit_GLX_EXT_swap_control (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSwapIntervalEXT"))) == NULL) || r;

  return r;
}

#endif /* GLX_EXT_swap_control */

#ifdef GLX_EXT_texture_from_pixmap

static GLboolean _glewInit_GLX_EXT_texture_from_pixmap (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindTexImageEXT = (PFNGLXBINDTEXIMAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindTexImageEXT"))) == NULL) || r;
  r = ((glXReleaseTexImageEXT = (PFNGLXRELEASETEXIMAGEEXTPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXReleaseTexImageEXT"))) == NULL) || r;

  return r;
}

#endif /* GLX_EXT_texture_from_pixmap */

#ifdef GLX_EXT_visual_info

#endif /* GLX_EXT_visual_info */

#ifdef GLX_EXT_visual_rating

#endif /* GLX_EXT_visual_rating */

#ifdef GLX_INTEL_swap_event

#endif /* GLX_INTEL_swap_event */

#ifdef GLX_MESA_agp_offset

static GLboolean _glewInit_GLX_MESA_agp_offset (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetAGPOffsetMESA = (PFNGLXGETAGPOFFSETMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetAGPOffsetMESA"))) == NULL) || r;

  return r;
}

#endif /* GLX_MESA_agp_offset */

#ifdef GLX_MESA_copy_sub_buffer

static GLboolean _glewInit_GLX_MESA_copy_sub_buffer (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXCopySubBufferMESA = (PFNGLXCOPYSUBBUFFERMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCopySubBufferMESA"))) == NULL) || r;

  return r;
}

#endif /* GLX_MESA_copy_sub_buffer */

#ifdef GLX_MESA_pixmap_colormap

static GLboolean _glewInit_GLX_MESA_pixmap_colormap (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXCreateGLXPixmapMESA = (PFNGLXCREATEGLXPIXMAPMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateGLXPixmapMESA"))) == NULL) || r;

  return r;
}

#endif /* GLX_MESA_pixmap_colormap */

#ifdef GLX_MESA_release_buffers

static GLboolean _glewInit_GLX_MESA_release_buffers (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXReleaseBuffersMESA = (PFNGLXRELEASEBUFFERSMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXReleaseBuffersMESA"))) == NULL) || r;

  return r;
}

#endif /* GLX_MESA_release_buffers */

#ifdef GLX_MESA_set_3dfx_mode

static GLboolean _glewInit_GLX_MESA_set_3dfx_mode (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXSet3DfxModeMESA = (PFNGLXSET3DFXMODEMESAPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSet3DfxModeMESA"))) == NULL) || r;

  return r;
}

#endif /* GLX_MESA_set_3dfx_mode */

#ifdef GLX_NV_copy_image

static GLboolean _glewInit_GLX_NV_copy_image (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXCopyImageSubDataNV = (PFNGLXCOPYIMAGESUBDATANVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCopyImageSubDataNV"))) == NULL) || r;

  return r;
}

#endif /* GLX_NV_copy_image */

#ifdef GLX_NV_float_buffer

#endif /* GLX_NV_float_buffer */

#ifdef GLX_NV_multisample_coverage

#endif /* GLX_NV_multisample_coverage */

#ifdef GLX_NV_present_video

static GLboolean _glewInit_GLX_NV_present_video (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindVideoDeviceNV = (PFNGLXBINDVIDEODEVICENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindVideoDeviceNV"))) == NULL) || r;
  r = ((glXEnumerateVideoDevicesNV = (PFNGLXENUMERATEVIDEODEVICESNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXEnumerateVideoDevicesNV"))) == NULL) || r;

  return r;
}

#endif /* GLX_NV_present_video */

#ifdef GLX_NV_swap_group

static GLboolean _glewInit_GLX_NV_swap_group (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindSwapBarrierNV = (PFNGLXBINDSWAPBARRIERNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindSwapBarrierNV"))) == NULL) || r;
  r = ((glXJoinSwapGroupNV = (PFNGLXJOINSWAPGROUPNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXJoinSwapGroupNV"))) == NULL) || r;
  r = ((glXQueryFrameCountNV = (PFNGLXQUERYFRAMECOUNTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryFrameCountNV"))) == NULL) || r;
  r = ((glXQueryMaxSwapGroupsNV = (PFNGLXQUERYMAXSWAPGROUPSNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryMaxSwapGroupsNV"))) == NULL) || r;
  r = ((glXQuerySwapGroupNV = (PFNGLXQUERYSWAPGROUPNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQuerySwapGroupNV"))) == NULL) || r;
  r = ((glXResetFrameCountNV = (PFNGLXRESETFRAMECOUNTNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXResetFrameCountNV"))) == NULL) || r;

  return r;
}

#endif /* GLX_NV_swap_group */

#ifdef GLX_NV_vertex_array_range

static GLboolean _glewInit_GLX_NV_vertex_array_range (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXAllocateMemoryNV = (PFNGLXALLOCATEMEMORYNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXAllocateMemoryNV"))) == NULL) || r;
  r = ((glXFreeMemoryNV = (PFNGLXFREEMEMORYNVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXFreeMemoryNV"))) == NULL) || r;

  return r;
}

#endif /* GLX_NV_vertex_array_range */

#ifdef GLX_NV_video_output

static GLboolean _glewInit_GLX_NV_video_output (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindVideoImageNV = (PFNGLXBINDVIDEOIMAGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindVideoImageNV"))) == NULL) || r;
  r = ((glXGetVideoDeviceNV = (PFNGLXGETVIDEODEVICENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetVideoDeviceNV"))) == NULL) || r;
  r = ((glXGetVideoInfoNV = (PFNGLXGETVIDEOINFONVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetVideoInfoNV"))) == NULL) || r;
  r = ((glXReleaseVideoDeviceNV = (PFNGLXRELEASEVIDEODEVICENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXReleaseVideoDeviceNV"))) == NULL) || r;
  r = ((glXReleaseVideoImageNV = (PFNGLXRELEASEVIDEOIMAGENVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXReleaseVideoImageNV"))) == NULL) || r;
  r = ((glXSendPbufferToVideoNV = (PFNGLXSENDPBUFFERTOVIDEONVPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSendPbufferToVideoNV"))) == NULL) || r;

  return r;
}

#endif /* GLX_NV_video_output */

#ifdef GLX_OML_swap_method

#endif /* GLX_OML_swap_method */

#if defined(GLX_OML_sync_control) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <inttypes.h>

static GLboolean _glewInit_GLX_OML_sync_control (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetMscRateOML = (PFNGLXGETMSCRATEOMLPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetMscRateOML"))) == NULL) || r;
  r = ((glXGetSyncValuesOML = (PFNGLXGETSYNCVALUESOMLPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetSyncValuesOML"))) == NULL) || r;
  r = ((glXSwapBuffersMscOML = (PFNGLXSWAPBUFFERSMSCOMLPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSwapBuffersMscOML"))) == NULL) || r;
  r = ((glXWaitForMscOML = (PFNGLXWAITFORMSCOMLPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXWaitForMscOML"))) == NULL) || r;
  r = ((glXWaitForSbcOML = (PFNGLXWAITFORSBCOMLPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXWaitForSbcOML"))) == NULL) || r;

  return r;
}

#endif /* GLX_OML_sync_control */

#ifdef GLX_SGIS_blended_overlay

#endif /* GLX_SGIS_blended_overlay */

#ifdef GLX_SGIS_color_range

#endif /* GLX_SGIS_color_range */

#ifdef GLX_SGIS_multisample

#endif /* GLX_SGIS_multisample */

#ifdef GLX_SGIS_shared_multisample

#endif /* GLX_SGIS_shared_multisample */

#ifdef GLX_SGIX_fbconfig

static GLboolean _glewInit_GLX_SGIX_fbconfig (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXChooseFBConfigSGIX = (PFNGLXCHOOSEFBCONFIGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXChooseFBConfigSGIX"))) == NULL) || r;
  r = ((glXCreateContextWithConfigSGIX = (PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateContextWithConfigSGIX"))) == NULL) || r;
  r = ((glXCreateGLXPixmapWithConfigSGIX = (PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateGLXPixmapWithConfigSGIX"))) == NULL) || r;
  r = ((glXGetFBConfigAttribSGIX = (PFNGLXGETFBCONFIGATTRIBSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetFBConfigAttribSGIX"))) == NULL) || r;
  r = ((glXGetFBConfigFromVisualSGIX = (PFNGLXGETFBCONFIGFROMVISUALSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetFBConfigFromVisualSGIX"))) == NULL) || r;
  r = ((glXGetVisualFromFBConfigSGIX = (PFNGLXGETVISUALFROMFBCONFIGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetVisualFromFBConfigSGIX"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGIX_fbconfig */

#ifdef GLX_SGIX_hyperpipe

static GLboolean _glewInit_GLX_SGIX_hyperpipe (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindHyperpipeSGIX = (PFNGLXBINDHYPERPIPESGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindHyperpipeSGIX"))) == NULL) || r;
  r = ((glXDestroyHyperpipeConfigSGIX = (PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDestroyHyperpipeConfigSGIX"))) == NULL) || r;
  r = ((glXHyperpipeAttribSGIX = (PFNGLXHYPERPIPEATTRIBSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXHyperpipeAttribSGIX"))) == NULL) || r;
  r = ((glXHyperpipeConfigSGIX = (PFNGLXHYPERPIPECONFIGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXHyperpipeConfigSGIX"))) == NULL) || r;
  r = ((glXQueryHyperpipeAttribSGIX = (PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryHyperpipeAttribSGIX"))) == NULL) || r;
  r = ((glXQueryHyperpipeBestAttribSGIX = (PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryHyperpipeBestAttribSGIX"))) == NULL) || r;
  r = ((glXQueryHyperpipeConfigSGIX = (PFNGLXQUERYHYPERPIPECONFIGSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryHyperpipeConfigSGIX"))) == NULL) || r;
  r = ((glXQueryHyperpipeNetworkSGIX = (PFNGLXQUERYHYPERPIPENETWORKSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryHyperpipeNetworkSGIX"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGIX_hyperpipe */

#ifdef GLX_SGIX_pbuffer

static GLboolean _glewInit_GLX_SGIX_pbuffer (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXCreateGLXPbufferSGIX = (PFNGLXCREATEGLXPBUFFERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCreateGLXPbufferSGIX"))) == NULL) || r;
  r = ((glXDestroyGLXPbufferSGIX = (PFNGLXDESTROYGLXPBUFFERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXDestroyGLXPbufferSGIX"))) == NULL) || r;
  r = ((glXGetSelectedEventSGIX = (PFNGLXGETSELECTEDEVENTSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetSelectedEventSGIX"))) == NULL) || r;
  r = ((glXQueryGLXPbufferSGIX = (PFNGLXQUERYGLXPBUFFERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryGLXPbufferSGIX"))) == NULL) || r;
  r = ((glXSelectEventSGIX = (PFNGLXSELECTEVENTSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSelectEventSGIX"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGIX_pbuffer */

#ifdef GLX_SGIX_swap_barrier

static GLboolean _glewInit_GLX_SGIX_swap_barrier (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindSwapBarrierSGIX = (PFNGLXBINDSWAPBARRIERSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindSwapBarrierSGIX"))) == NULL) || r;
  r = ((glXQueryMaxSwapBarriersSGIX = (PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryMaxSwapBarriersSGIX"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGIX_swap_barrier */

#ifdef GLX_SGIX_swap_group

static GLboolean _glewInit_GLX_SGIX_swap_group (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXJoinSwapGroupSGIX = (PFNGLXJOINSWAPGROUPSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXJoinSwapGroupSGIX"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGIX_swap_group */

#ifdef GLX_SGIX_video_resize

static GLboolean _glewInit_GLX_SGIX_video_resize (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXBindChannelToWindowSGIX = (PFNGLXBINDCHANNELTOWINDOWSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXBindChannelToWindowSGIX"))) == NULL) || r;
  r = ((glXChannelRectSGIX = (PFNGLXCHANNELRECTSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXChannelRectSGIX"))) == NULL) || r;
  r = ((glXChannelRectSyncSGIX = (PFNGLXCHANNELRECTSYNCSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXChannelRectSyncSGIX"))) == NULL) || r;
  r = ((glXQueryChannelDeltasSGIX = (PFNGLXQUERYCHANNELDELTASSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryChannelDeltasSGIX"))) == NULL) || r;
  r = ((glXQueryChannelRectSGIX = (PFNGLXQUERYCHANNELRECTSGIXPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXQueryChannelRectSGIX"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGIX_video_resize */

#ifdef GLX_SGIX_visual_select_group

#endif /* GLX_SGIX_visual_select_group */

#ifdef GLX_SGI_cushion

static GLboolean _glewInit_GLX_SGI_cushion (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXCushionSGI = (PFNGLXCUSHIONSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXCushionSGI"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGI_cushion */

#ifdef GLX_SGI_make_current_read

static GLboolean _glewInit_GLX_SGI_make_current_read (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetCurrentReadDrawableSGI = (PFNGLXGETCURRENTREADDRAWABLESGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetCurrentReadDrawableSGI"))) == NULL) || r;
  r = ((glXMakeCurrentReadSGI = (PFNGLXMAKECURRENTREADSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXMakeCurrentReadSGI"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGI_make_current_read */

#ifdef GLX_SGI_swap_control

static GLboolean _glewInit_GLX_SGI_swap_control (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXSwapIntervalSGI"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGI_swap_control */

#ifdef GLX_SGI_video_sync

static GLboolean _glewInit_GLX_SGI_video_sync (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetVideoSyncSGI = (PFNGLXGETVIDEOSYNCSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetVideoSyncSGI"))) == NULL) || r;
  r = ((glXWaitVideoSyncSGI = (PFNGLXWAITVIDEOSYNCSGIPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXWaitVideoSyncSGI"))) == NULL) || r;

  return r;
}

#endif /* GLX_SGI_video_sync */

#ifdef GLX_SUN_get_transparent_index

static GLboolean _glewInit_GLX_SUN_get_transparent_index (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetTransparentIndexSUN = (PFNGLXGETTRANSPARENTINDEXSUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetTransparentIndexSUN"))) == NULL) || r;

  return r;
}

#endif /* GLX_SUN_get_transparent_index */

#ifdef GLX_SUN_video_resize

static GLboolean _glewInit_GLX_SUN_video_resize (GLXEW_CONTEXT_ARG_DEF_INIT)
{
  GLboolean r = GL_FALSE;

  r = ((glXGetVideoResizeSUN = (PFNGLXGETVIDEORESIZESUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXGetVideoResizeSUN"))) == NULL) || r;
  r = ((glXVideoResizeSUN = (PFNGLXVIDEORESIZESUNPROC)glewGetProcAddress(fqn_from_convention(glew_convention, "glXVideoResizeSUN"))) == NULL) || r;

  return r;
}

#endif /* GLX_SUN_video_resize */

/* ------------------------------------------------------------------------ */

GLboolean glxewGetExtension (const char* name)
{    
  GLubyte* p;
  GLubyte* end;
  GLuint len;

  if (glXGetCurrentDisplay == NULL) return GL_FALSE;
  len = _glewStrLen((const GLubyte*)name);
  p = (GLubyte*)glXGetClientString(glXGetCurrentDisplay(), GLX_EXTENSIONS);
  if (0 == p) return GL_FALSE;
  end = p + _glewStrLen(p);
  while (p < end)
  {
    GLuint n = _glewStrCLen(p, ' ');
    if (len == n && _glewStrSame((const GLubyte*)name, p, n)) return GL_TRUE;
    p += n+1;
  }
  return GL_FALSE;
}

GLenum glxewContextInit (GLXEW_CONTEXT_ARG_DEF_LIST)
{
  int major, minor;

  /* Initialize GLX versions. */
  if(_glewInit_GLX_VERSION_1_0(GLEW_CONTEXT_ARG_VAR_INIT)) {
    return GLEW_ERROR_NO_GLX;
  }
  if (_glewInit_GLX_VERSION_1_1(GLEW_CONTEXT_ARG_VAR_INIT)) {
    return GLEW_ERROR_GLX_VERSION_10_ONLY;
  }
  if (_glewInit_GLX_VERSION_1_2(GLEW_CONTEXT_ARG_VAR_INIT)) {
    return GLEW_ERROR_GLX_VERSION_11_ONLY;
  }

  /* initialize flags */
  CONST_CAST(GLXEW_VERSION_1_0) = GL_TRUE;
  CONST_CAST(GLXEW_VERSION_1_1) = GL_TRUE;
  CONST_CAST(GLXEW_VERSION_1_2) = GL_TRUE;
  CONST_CAST(GLXEW_VERSION_1_3) = GL_TRUE;
  CONST_CAST(GLXEW_VERSION_1_4) = GL_TRUE;
  /* query GLX version */
  glXQueryVersion(glXGetCurrentDisplay(), &major, &minor);
  if (major == 1 && minor <= 3)
  {
    switch (minor)
    {
      case 3:
      CONST_CAST(GLXEW_VERSION_1_4) = GL_FALSE;
      break;
      case 2:
      CONST_CAST(GLXEW_VERSION_1_4) = GL_FALSE;
      CONST_CAST(GLXEW_VERSION_1_3) = GL_FALSE;
      break;
      default:
      return GLEW_ERROR_GLX_VERSION_11_ONLY;
      break;
    }
  }
  /* initialize extensions */
#ifdef GLX_VERSION_1_3
  if (glewExperimental || GLXEW_VERSION_1_3) CONST_CAST(GLXEW_VERSION_1_3) = !_glewInit_GLX_VERSION_1_3(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_VERSION_1_3 */
#ifdef GLX_3DFX_multisample
  CONST_CAST(GLXEW_3DFX_multisample) = glxewGetExtension("GLX_3DFX_multisample");
#endif /* GLX_3DFX_multisample */
#ifdef GLX_AMD_gpu_association
  CONST_CAST(GLXEW_AMD_gpu_association) = glxewGetExtension("GLX_AMD_gpu_association");
#endif /* GLX_AMD_gpu_association */
#ifdef GLX_ARB_create_context
  CONST_CAST(GLXEW_ARB_create_context) = glxewGetExtension("GLX_ARB_create_context");
  if (glewExperimental || GLXEW_ARB_create_context) CONST_CAST(GLXEW_ARB_create_context) = !_glewInit_GLX_ARB_create_context(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_ARB_create_context */
#ifdef GLX_ARB_create_context_profile
  CONST_CAST(GLXEW_ARB_create_context_profile) = glxewGetExtension("GLX_ARB_create_context_profile");
#endif /* GLX_ARB_create_context_profile */
#ifdef GLX_ARB_create_context_robustness
  CONST_CAST(GLXEW_ARB_create_context_robustness) = glxewGetExtension("GLX_ARB_create_context_robustness");
#endif /* GLX_ARB_create_context_robustness */
#ifdef GLX_ARB_fbconfig_float
  CONST_CAST(GLXEW_ARB_fbconfig_float) = glxewGetExtension("GLX_ARB_fbconfig_float");
#endif /* GLX_ARB_fbconfig_float */
#ifdef GLX_ARB_framebuffer_sRGB
  CONST_CAST(GLXEW_ARB_framebuffer_sRGB) = glxewGetExtension("GLX_ARB_framebuffer_sRGB");
#endif /* GLX_ARB_framebuffer_sRGB */
#ifdef GLX_ARB_get_proc_address
  CONST_CAST(GLXEW_ARB_get_proc_address) = glxewGetExtension("GLX_ARB_get_proc_address");
#endif /* GLX_ARB_get_proc_address */
#ifdef GLX_ARB_multisample
  CONST_CAST(GLXEW_ARB_multisample) = glxewGetExtension("GLX_ARB_multisample");
#endif /* GLX_ARB_multisample */
#ifdef GLX_ARB_vertex_buffer_object
  CONST_CAST(GLXEW_ARB_vertex_buffer_object) = glxewGetExtension("GLX_ARB_vertex_buffer_object");
#endif /* GLX_ARB_vertex_buffer_object */
#ifdef GLX_ATI_pixel_format_float
  CONST_CAST(GLXEW_ATI_pixel_format_float) = glxewGetExtension("GLX_ATI_pixel_format_float");
#endif /* GLX_ATI_pixel_format_float */
#ifdef GLX_ATI_render_texture
  CONST_CAST(GLXEW_ATI_render_texture) = glxewGetExtension("GLX_ATI_render_texture");
  if (glewExperimental || GLXEW_ATI_render_texture) CONST_CAST(GLXEW_ATI_render_texture) = !_glewInit_GLX_ATI_render_texture(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_ATI_render_texture */
#ifdef GLX_EXT_create_context_es2_profile
  CONST_CAST(GLXEW_EXT_create_context_es2_profile) = glxewGetExtension("GLX_EXT_create_context_es2_profile");
#endif /* GLX_EXT_create_context_es2_profile */
#ifdef GLX_EXT_fbconfig_packed_float
  CONST_CAST(GLXEW_EXT_fbconfig_packed_float) = glxewGetExtension("GLX_EXT_fbconfig_packed_float");
#endif /* GLX_EXT_fbconfig_packed_float */
#ifdef GLX_EXT_framebuffer_sRGB
  CONST_CAST(GLXEW_EXT_framebuffer_sRGB) = glxewGetExtension("GLX_EXT_framebuffer_sRGB");
#endif /* GLX_EXT_framebuffer_sRGB */
#ifdef GLX_EXT_import_context
  CONST_CAST(GLXEW_EXT_import_context) = glxewGetExtension("GLX_EXT_import_context");
  if (glewExperimental || GLXEW_EXT_import_context) CONST_CAST(GLXEW_EXT_import_context) = !_glewInit_GLX_EXT_import_context(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_EXT_import_context */
#ifdef GLX_EXT_scene_marker
  CONST_CAST(GLXEW_EXT_scene_marker) = glxewGetExtension("GLX_EXT_scene_marker");
#endif /* GLX_EXT_scene_marker */
#ifdef GLX_EXT_swap_control
  CONST_CAST(GLXEW_EXT_swap_control) = glxewGetExtension("GLX_EXT_swap_control");
  if (glewExperimental || GLXEW_EXT_swap_control) CONST_CAST(GLXEW_EXT_swap_control) = !_glewInit_GLX_EXT_swap_control(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_EXT_swap_control */
#ifdef GLX_EXT_texture_from_pixmap
  CONST_CAST(GLXEW_EXT_texture_from_pixmap) = glxewGetExtension("GLX_EXT_texture_from_pixmap");
  if (glewExperimental || GLXEW_EXT_texture_from_pixmap) CONST_CAST(GLXEW_EXT_texture_from_pixmap) = !_glewInit_GLX_EXT_texture_from_pixmap(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_EXT_texture_from_pixmap */
#ifdef GLX_EXT_visual_info
  CONST_CAST(GLXEW_EXT_visual_info) = glxewGetExtension("GLX_EXT_visual_info");
#endif /* GLX_EXT_visual_info */
#ifdef GLX_EXT_visual_rating
  CONST_CAST(GLXEW_EXT_visual_rating) = glxewGetExtension("GLX_EXT_visual_rating");
#endif /* GLX_EXT_visual_rating */
#ifdef GLX_INTEL_swap_event
  CONST_CAST(GLXEW_INTEL_swap_event) = glxewGetExtension("GLX_INTEL_swap_event");
#endif /* GLX_INTEL_swap_event */
#ifdef GLX_MESA_agp_offset
  CONST_CAST(GLXEW_MESA_agp_offset) = glxewGetExtension("GLX_MESA_agp_offset");
  if (glewExperimental || GLXEW_MESA_agp_offset) CONST_CAST(GLXEW_MESA_agp_offset) = !_glewInit_GLX_MESA_agp_offset(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_MESA_agp_offset */
#ifdef GLX_MESA_copy_sub_buffer
  CONST_CAST(GLXEW_MESA_copy_sub_buffer) = glxewGetExtension("GLX_MESA_copy_sub_buffer");
  if (glewExperimental || GLXEW_MESA_copy_sub_buffer) CONST_CAST(GLXEW_MESA_copy_sub_buffer) = !_glewInit_GLX_MESA_copy_sub_buffer(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_MESA_copy_sub_buffer */
#ifdef GLX_MESA_pixmap_colormap
  CONST_CAST(GLXEW_MESA_pixmap_colormap) = glxewGetExtension("GLX_MESA_pixmap_colormap");
  if (glewExperimental || GLXEW_MESA_pixmap_colormap) CONST_CAST(GLXEW_MESA_pixmap_colormap) = !_glewInit_GLX_MESA_pixmap_colormap(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_MESA_pixmap_colormap */
#ifdef GLX_MESA_release_buffers
  CONST_CAST(GLXEW_MESA_release_buffers) = glxewGetExtension("GLX_MESA_release_buffers");
  if (glewExperimental || GLXEW_MESA_release_buffers) CONST_CAST(GLXEW_MESA_release_buffers) = !_glewInit_GLX_MESA_release_buffers(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_MESA_release_buffers */
#ifdef GLX_MESA_set_3dfx_mode
  CONST_CAST(GLXEW_MESA_set_3dfx_mode) = glxewGetExtension("GLX_MESA_set_3dfx_mode");
  if (glewExperimental || GLXEW_MESA_set_3dfx_mode) CONST_CAST(GLXEW_MESA_set_3dfx_mode) = !_glewInit_GLX_MESA_set_3dfx_mode(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_MESA_set_3dfx_mode */
#ifdef GLX_NV_copy_image
  CONST_CAST(GLXEW_NV_copy_image) = glxewGetExtension("GLX_NV_copy_image");
  if (glewExperimental || GLXEW_NV_copy_image) CONST_CAST(GLXEW_NV_copy_image) = !_glewInit_GLX_NV_copy_image(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_NV_copy_image */
#ifdef GLX_NV_float_buffer
  CONST_CAST(GLXEW_NV_float_buffer) = glxewGetExtension("GLX_NV_float_buffer");
#endif /* GLX_NV_float_buffer */
#ifdef GLX_NV_multisample_coverage
  CONST_CAST(GLXEW_NV_multisample_coverage) = glxewGetExtension("GLX_NV_multisample_coverage");
#endif /* GLX_NV_multisample_coverage */
#ifdef GLX_NV_present_video
  CONST_CAST(GLXEW_NV_present_video) = glxewGetExtension("GLX_NV_present_video");
  if (glewExperimental || GLXEW_NV_present_video) CONST_CAST(GLXEW_NV_present_video) = !_glewInit_GLX_NV_present_video(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_NV_present_video */
#ifdef GLX_NV_swap_group
  CONST_CAST(GLXEW_NV_swap_group) = glxewGetExtension("GLX_NV_swap_group");
  if (glewExperimental || GLXEW_NV_swap_group) CONST_CAST(GLXEW_NV_swap_group) = !_glewInit_GLX_NV_swap_group(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_NV_swap_group */
#ifdef GLX_NV_vertex_array_range
  CONST_CAST(GLXEW_NV_vertex_array_range) = glxewGetExtension("GLX_NV_vertex_array_range");
  if (glewExperimental || GLXEW_NV_vertex_array_range) CONST_CAST(GLXEW_NV_vertex_array_range) = !_glewInit_GLX_NV_vertex_array_range(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_NV_vertex_array_range */
#ifdef GLX_NV_video_output
  CONST_CAST(GLXEW_NV_video_output) = glxewGetExtension("GLX_NV_video_output");
  if (glewExperimental || GLXEW_NV_video_output) CONST_CAST(GLXEW_NV_video_output) = !_glewInit_GLX_NV_video_output(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_NV_video_output */
#ifdef GLX_OML_swap_method
  CONST_CAST(GLXEW_OML_swap_method) = glxewGetExtension("GLX_OML_swap_method");
#endif /* GLX_OML_swap_method */
#if defined(GLX_OML_sync_control) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <inttypes.h>
  CONST_CAST(GLXEW_OML_sync_control) = glxewGetExtension("GLX_OML_sync_control");
  if (glewExperimental || GLXEW_OML_sync_control) CONST_CAST(GLXEW_OML_sync_control) = !_glewInit_GLX_OML_sync_control(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_OML_sync_control */
#ifdef GLX_SGIS_blended_overlay
  CONST_CAST(GLXEW_SGIS_blended_overlay) = glxewGetExtension("GLX_SGIS_blended_overlay");
#endif /* GLX_SGIS_blended_overlay */
#ifdef GLX_SGIS_color_range
  CONST_CAST(GLXEW_SGIS_color_range) = glxewGetExtension("GLX_SGIS_color_range");
#endif /* GLX_SGIS_color_range */
#ifdef GLX_SGIS_multisample
  CONST_CAST(GLXEW_SGIS_multisample) = glxewGetExtension("GLX_SGIS_multisample");
#endif /* GLX_SGIS_multisample */
#ifdef GLX_SGIS_shared_multisample
  CONST_CAST(GLXEW_SGIS_shared_multisample) = glxewGetExtension("GLX_SGIS_shared_multisample");
#endif /* GLX_SGIS_shared_multisample */
#ifdef GLX_SGIX_fbconfig
  CONST_CAST(GLXEW_SGIX_fbconfig) = glxewGetExtension("GLX_SGIX_fbconfig");
  if (glewExperimental || GLXEW_SGIX_fbconfig) CONST_CAST(GLXEW_SGIX_fbconfig) = !_glewInit_GLX_SGIX_fbconfig(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGIX_fbconfig */
#ifdef GLX_SGIX_hyperpipe
  CONST_CAST(GLXEW_SGIX_hyperpipe) = glxewGetExtension("GLX_SGIX_hyperpipe");
  if (glewExperimental || GLXEW_SGIX_hyperpipe) CONST_CAST(GLXEW_SGIX_hyperpipe) = !_glewInit_GLX_SGIX_hyperpipe(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGIX_hyperpipe */
#ifdef GLX_SGIX_pbuffer
  CONST_CAST(GLXEW_SGIX_pbuffer) = glxewGetExtension("GLX_SGIX_pbuffer");
  if (glewExperimental || GLXEW_SGIX_pbuffer) CONST_CAST(GLXEW_SGIX_pbuffer) = !_glewInit_GLX_SGIX_pbuffer(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGIX_pbuffer */
#ifdef GLX_SGIX_swap_barrier
  CONST_CAST(GLXEW_SGIX_swap_barrier) = glxewGetExtension("GLX_SGIX_swap_barrier");
  if (glewExperimental || GLXEW_SGIX_swap_barrier) CONST_CAST(GLXEW_SGIX_swap_barrier) = !_glewInit_GLX_SGIX_swap_barrier(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGIX_swap_barrier */
#ifdef GLX_SGIX_swap_group
  CONST_CAST(GLXEW_SGIX_swap_group) = glxewGetExtension("GLX_SGIX_swap_group");
  if (glewExperimental || GLXEW_SGIX_swap_group) CONST_CAST(GLXEW_SGIX_swap_group) = !_glewInit_GLX_SGIX_swap_group(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGIX_swap_group */
#ifdef GLX_SGIX_video_resize
  CONST_CAST(GLXEW_SGIX_video_resize) = glxewGetExtension("GLX_SGIX_video_resize");
  if (glewExperimental || GLXEW_SGIX_video_resize) CONST_CAST(GLXEW_SGIX_video_resize) = !_glewInit_GLX_SGIX_video_resize(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGIX_video_resize */
#ifdef GLX_SGIX_visual_select_group
  CONST_CAST(GLXEW_SGIX_visual_select_group) = glxewGetExtension("GLX_SGIX_visual_select_group");
#endif /* GLX_SGIX_visual_select_group */
#ifdef GLX_SGI_cushion
  CONST_CAST(GLXEW_SGI_cushion) = glxewGetExtension("GLX_SGI_cushion");
  if (glewExperimental || GLXEW_SGI_cushion) CONST_CAST(GLXEW_SGI_cushion) = !_glewInit_GLX_SGI_cushion(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGI_cushion */
#ifdef GLX_SGI_make_current_read
  CONST_CAST(GLXEW_SGI_make_current_read) = glxewGetExtension("GLX_SGI_make_current_read");
  if (glewExperimental || GLXEW_SGI_make_current_read) CONST_CAST(GLXEW_SGI_make_current_read) = !_glewInit_GLX_SGI_make_current_read(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGI_make_current_read */
#ifdef GLX_SGI_swap_control
  CONST_CAST(GLXEW_SGI_swap_control) = glxewGetExtension("GLX_SGI_swap_control");
  if (glewExperimental || GLXEW_SGI_swap_control) CONST_CAST(GLXEW_SGI_swap_control) = !_glewInit_GLX_SGI_swap_control(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGI_swap_control */
#ifdef GLX_SGI_video_sync
  CONST_CAST(GLXEW_SGI_video_sync) = glxewGetExtension("GLX_SGI_video_sync");
  if (glewExperimental || GLXEW_SGI_video_sync) CONST_CAST(GLXEW_SGI_video_sync) = !_glewInit_GLX_SGI_video_sync(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SGI_video_sync */
#ifdef GLX_SUN_get_transparent_index
  CONST_CAST(GLXEW_SUN_get_transparent_index) = glxewGetExtension("GLX_SUN_get_transparent_index");
  if (glewExperimental || GLXEW_SUN_get_transparent_index) CONST_CAST(GLXEW_SUN_get_transparent_index) = !_glewInit_GLX_SUN_get_transparent_index(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SUN_get_transparent_index */
#ifdef GLX_SUN_video_resize
  CONST_CAST(GLXEW_SUN_video_resize) = glxewGetExtension("GLX_SUN_video_resize");
  if (glewExperimental || GLXEW_SUN_video_resize) CONST_CAST(GLXEW_SUN_video_resize) = !_glewInit_GLX_SUN_video_resize(GLEW_CONTEXT_ARG_VAR_INIT);
#endif /* GLX_SUN_video_resize */

  return GLEW_OK;
}

#endif /* !__APPLE__ || GLEW_APPLE_GLX */

/* ------------------------------------------------------------------------ */

const GLubyte* glewGetErrorString (GLenum error)
{
  static const GLubyte* _glewErrorString[] =
  {
    (const GLubyte*)"No error",
    (const GLubyte*)"Missing GL version",
    (const GLubyte*)"GL 1.1 and up are not supported",
    (const GLubyte*)"GLX 1.2 and up are not supported",
    (const GLubyte*)"GLX 1.1 and up are not supported",
    (const GLubyte*)"GLX not available",
    (const GLubyte*)"Library dlopen failed",
    (const GLubyte*)"Could not find appropriate GL function loader",
    (const GLubyte*)"Unknown error"
  };
  const int max_error = sizeof(_glewErrorString)/sizeof(*_glewErrorString) - 1;
  return _glewErrorString[(int)error > max_error ? max_error : (int)error];
}

const GLubyte* glewGetString (GLenum name)
{
  static const GLubyte* _glewString[] =
  {
    (const GLubyte*)NULL,
    (const GLubyte*)"1.5.7",
    (const GLubyte*)"1",
    (const GLubyte*)"5",
    (const GLubyte*)"7"
  };
  const int max_string = sizeof(_glewString)/sizeof(*_glewString) - 1;
  return _glewString[(int)name > max_string ? 0 : (int)name];
}

/* ------------------------------------------------------------------------ */

GLboolean glewExperimental = GL_FALSE;

#if !defined(GLEW_MX)

#if defined(_WIN32)
extern GLenum wglewContextInit (void);
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX) /* _UNIX */
extern GLenum glxewContextInit (void);
#endif /* _WIN32 */

GLenum glewInitLibrary (const char *name, enum GL_Library_Type lib_type,
                        enum GL_Name_Convention name_conv)
{
  glew_gl_lib = name;
  gl_lib_type = lib_type;
  glew_convention = name_conv;

  return glewInit();
}

GLenum glewInit ()
{
  GLenum r;

  r = _glewInitFunctionLoader();
  if(GLEW_NO_ERROR != r) {
    return r;
  }

  r = glewContextInit();
#if defined(_WIN32)
  if(gl_lib_type != GLEW_LIB_TYPE_OSMESA) {
    return wglewContextInit();
  }
  return GLEW_OK;
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX) /* _UNIX */
  /* We'll need GLX to setup contexts / etc. if we're not using OSMesa. */
  if(gl_lib_type != GLEW_LIB_TYPE_OSMESA) {
    return glxewContextInit();
  }
  return r;
#else
  return r;
#endif /* _WIN32 */
}

#endif /* !GLEW_MX */
#ifdef GLEW_MX
GLboolean glewContextIsSupported (const GLEWContext* ctx, const char* name)
#else
GLboolean glewIsSupported (const char* name)
#endif
{
  GLubyte* pos = (GLubyte*)name;
  GLuint len = _glewStrLen(pos);
  GLboolean ret = GL_TRUE;
  while (ret && len > 0)
  {
    if (_glewStrSame1(&pos, &len, (const GLubyte*)"GL_", 3))
    {
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"VERSION_", 8))
      {
#ifdef GL_VERSION_1_1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_1", 3))
        {
          ret = GLEW_VERSION_1_1;
          continue;
        }
#endif
#ifdef GL_VERSION_1_2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_2", 3))
        {
          ret = GLEW_VERSION_1_2;
          continue;
        }
#endif
#ifdef GL_VERSION_1_2_1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_2_1", 5))
        {
          ret = GLEW_VERSION_1_2_1;
          continue;
        }
#endif
#ifdef GL_VERSION_1_3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_3", 3))
        {
          ret = GLEW_VERSION_1_3;
          continue;
        }
#endif
#ifdef GL_VERSION_1_4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_4", 3))
        {
          ret = GLEW_VERSION_1_4;
          continue;
        }
#endif
#ifdef GL_VERSION_1_5
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_5", 3))
        {
          ret = GLEW_VERSION_1_5;
          continue;
        }
#endif
#ifdef GL_VERSION_2_0
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"2_0", 3))
        {
          ret = GLEW_VERSION_2_0;
          continue;
        }
#endif
#ifdef GL_VERSION_2_1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"2_1", 3))
        {
          ret = GLEW_VERSION_2_1;
          continue;
        }
#endif
#ifdef GL_VERSION_3_0
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"3_0", 3))
        {
          ret = GLEW_VERSION_3_0;
          continue;
        }
#endif
#ifdef GL_VERSION_3_1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"3_1", 3))
        {
          ret = GLEW_VERSION_3_1;
          continue;
        }
#endif
#ifdef GL_VERSION_3_2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"3_2", 3))
        {
          ret = GLEW_VERSION_3_2;
          continue;
        }
#endif
#ifdef GL_VERSION_3_3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"3_3", 3))
        {
          ret = GLEW_VERSION_3_3;
          continue;
        }
#endif
#ifdef GL_VERSION_4_0
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"4_0", 3))
        {
          ret = GLEW_VERSION_4_0;
          continue;
        }
#endif
#ifdef GL_VERSION_4_1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"4_1", 3))
        {
          ret = GLEW_VERSION_4_1;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"3DFX_", 5))
      {
#ifdef GL_3DFX_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample", 11))
        {
          ret = GLEW_3DFX_multisample;
          continue;
        }
#endif
#ifdef GL_3DFX_tbuffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"tbuffer", 7))
        {
          ret = GLEW_3DFX_tbuffer;
          continue;
        }
#endif
#ifdef GL_3DFX_texture_compression_FXT1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_FXT1", 24))
        {
          ret = GLEW_3DFX_texture_compression_FXT1;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"AMD_", 4))
      {
#ifdef GL_AMD_conservative_depth
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"conservative_depth", 18))
        {
          ret = GLEW_AMD_conservative_depth;
          continue;
        }
#endif
#ifdef GL_AMD_debug_output
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"debug_output", 12))
        {
          ret = GLEW_AMD_debug_output;
          continue;
        }
#endif
#ifdef GL_AMD_depth_clamp_separate
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_clamp_separate", 20))
        {
          ret = GLEW_AMD_depth_clamp_separate;
          continue;
        }
#endif
#ifdef GL_AMD_draw_buffers_blend
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_buffers_blend", 18))
        {
          ret = GLEW_AMD_draw_buffers_blend;
          continue;
        }
#endif
#ifdef GL_AMD_name_gen_delete
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"name_gen_delete", 15))
        {
          ret = GLEW_AMD_name_gen_delete;
          continue;
        }
#endif
#ifdef GL_AMD_performance_monitor
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"performance_monitor", 19))
        {
          ret = GLEW_AMD_performance_monitor;
          continue;
        }
#endif
#ifdef GL_AMD_seamless_cubemap_per_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"seamless_cubemap_per_texture", 28))
        {
          ret = GLEW_AMD_seamless_cubemap_per_texture;
          continue;
        }
#endif
#ifdef GL_AMD_shader_stencil_export
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_stencil_export", 21))
        {
          ret = GLEW_AMD_shader_stencil_export;
          continue;
        }
#endif
#ifdef GL_AMD_texture_texture4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_texture4", 16))
        {
          ret = GLEW_AMD_texture_texture4;
          continue;
        }
#endif
#ifdef GL_AMD_transform_feedback3_lines_triangles
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transform_feedback3_lines_triangles", 35))
        {
          ret = GLEW_AMD_transform_feedback3_lines_triangles;
          continue;
        }
#endif
#ifdef GL_AMD_vertex_shader_tessellator
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_shader_tessellator", 25))
        {
          ret = GLEW_AMD_vertex_shader_tessellator;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"APPLE_", 6))
      {
#ifdef GL_APPLE_aux_depth_stencil
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"aux_depth_stencil", 17))
        {
          ret = GLEW_APPLE_aux_depth_stencil;
          continue;
        }
#endif
#ifdef GL_APPLE_client_storage
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"client_storage", 14))
        {
          ret = GLEW_APPLE_client_storage;
          continue;
        }
#endif
#ifdef GL_APPLE_element_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"element_array", 13))
        {
          ret = GLEW_APPLE_element_array;
          continue;
        }
#endif
#ifdef GL_APPLE_fence
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fence", 5))
        {
          ret = GLEW_APPLE_fence;
          continue;
        }
#endif
#ifdef GL_APPLE_float_pixels
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"float_pixels", 12))
        {
          ret = GLEW_APPLE_float_pixels;
          continue;
        }
#endif
#ifdef GL_APPLE_flush_buffer_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"flush_buffer_range", 18))
        {
          ret = GLEW_APPLE_flush_buffer_range;
          continue;
        }
#endif
#ifdef GL_APPLE_object_purgeable
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"object_purgeable", 16))
        {
          ret = GLEW_APPLE_object_purgeable;
          continue;
        }
#endif
#ifdef GL_APPLE_pixel_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_buffer", 12))
        {
          ret = GLEW_APPLE_pixel_buffer;
          continue;
        }
#endif
#ifdef GL_APPLE_rgb_422
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"rgb_422", 7))
        {
          ret = GLEW_APPLE_rgb_422;
          continue;
        }
#endif
#ifdef GL_APPLE_row_bytes
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"row_bytes", 9))
        {
          ret = GLEW_APPLE_row_bytes;
          continue;
        }
#endif
#ifdef GL_APPLE_specular_vector
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"specular_vector", 15))
        {
          ret = GLEW_APPLE_specular_vector;
          continue;
        }
#endif
#ifdef GL_APPLE_texture_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_range", 13))
        {
          ret = GLEW_APPLE_texture_range;
          continue;
        }
#endif
#ifdef GL_APPLE_transform_hint
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transform_hint", 14))
        {
          ret = GLEW_APPLE_transform_hint;
          continue;
        }
#endif
#ifdef GL_APPLE_vertex_array_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_object", 19))
        {
          ret = GLEW_APPLE_vertex_array_object;
          continue;
        }
#endif
#ifdef GL_APPLE_vertex_array_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_range", 18))
        {
          ret = GLEW_APPLE_vertex_array_range;
          continue;
        }
#endif
#ifdef GL_APPLE_vertex_program_evaluators
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program_evaluators", 25))
        {
          ret = GLEW_APPLE_vertex_program_evaluators;
          continue;
        }
#endif
#ifdef GL_APPLE_ycbcr_422
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"ycbcr_422", 9))
        {
          ret = GLEW_APPLE_ycbcr_422;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"ARB_", 4))
      {
#ifdef GL_ARB_ES2_compatibility
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"ES2_compatibility", 17))
        {
          ret = GLEW_ARB_ES2_compatibility;
          continue;
        }
#endif
#ifdef GL_ARB_blend_func_extended
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_func_extended", 19))
        {
          ret = GLEW_ARB_blend_func_extended;
          continue;
        }
#endif
#ifdef GL_ARB_cl_event
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"cl_event", 8))
        {
          ret = GLEW_ARB_cl_event;
          continue;
        }
#endif
#ifdef GL_ARB_color_buffer_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"color_buffer_float", 18))
        {
          ret = GLEW_ARB_color_buffer_float;
          continue;
        }
#endif
#ifdef GL_ARB_compatibility
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"compatibility", 13))
        {
          ret = GLEW_ARB_compatibility;
          continue;
        }
#endif
#ifdef GL_ARB_copy_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"copy_buffer", 11))
        {
          ret = GLEW_ARB_copy_buffer;
          continue;
        }
#endif
#ifdef GL_ARB_debug_output
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"debug_output", 12))
        {
          ret = GLEW_ARB_debug_output;
          continue;
        }
#endif
#ifdef GL_ARB_depth_buffer_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_buffer_float", 18))
        {
          ret = GLEW_ARB_depth_buffer_float;
          continue;
        }
#endif
#ifdef GL_ARB_depth_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_clamp", 11))
        {
          ret = GLEW_ARB_depth_clamp;
          continue;
        }
#endif
#ifdef GL_ARB_depth_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_texture", 13))
        {
          ret = GLEW_ARB_depth_texture;
          continue;
        }
#endif
#ifdef GL_ARB_draw_buffers
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_buffers", 12))
        {
          ret = GLEW_ARB_draw_buffers;
          continue;
        }
#endif
#ifdef GL_ARB_draw_buffers_blend
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_buffers_blend", 18))
        {
          ret = GLEW_ARB_draw_buffers_blend;
          continue;
        }
#endif
#ifdef GL_ARB_draw_elements_base_vertex
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_elements_base_vertex", 25))
        {
          ret = GLEW_ARB_draw_elements_base_vertex;
          continue;
        }
#endif
#ifdef GL_ARB_draw_indirect
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_indirect", 13))
        {
          ret = GLEW_ARB_draw_indirect;
          continue;
        }
#endif
#ifdef GL_ARB_draw_instanced
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_instanced", 14))
        {
          ret = GLEW_ARB_draw_instanced;
          continue;
        }
#endif
#ifdef GL_ARB_explicit_attrib_location
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"explicit_attrib_location", 24))
        {
          ret = GLEW_ARB_explicit_attrib_location;
          continue;
        }
#endif
#ifdef GL_ARB_fragment_coord_conventions
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_coord_conventions", 26))
        {
          ret = GLEW_ARB_fragment_coord_conventions;
          continue;
        }
#endif
#ifdef GL_ARB_fragment_program
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_program", 16))
        {
          ret = GLEW_ARB_fragment_program;
          continue;
        }
#endif
#ifdef GL_ARB_fragment_program_shadow
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_program_shadow", 23))
        {
          ret = GLEW_ARB_fragment_program_shadow;
          continue;
        }
#endif
#ifdef GL_ARB_fragment_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_shader", 15))
        {
          ret = GLEW_ARB_fragment_shader;
          continue;
        }
#endif
#ifdef GL_ARB_framebuffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_object", 18))
        {
          ret = GLEW_ARB_framebuffer_object;
          continue;
        }
#endif
#ifdef GL_ARB_framebuffer_sRGB
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_sRGB", 16))
        {
          ret = GLEW_ARB_framebuffer_sRGB;
          continue;
        }
#endif
#ifdef GL_ARB_geometry_shader4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"geometry_shader4", 16))
        {
          ret = GLEW_ARB_geometry_shader4;
          continue;
        }
#endif
#ifdef GL_ARB_get_program_binary
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"get_program_binary", 18))
        {
          ret = GLEW_ARB_get_program_binary;
          continue;
        }
#endif
#ifdef GL_ARB_gpu_shader5
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_shader5", 11))
        {
          ret = GLEW_ARB_gpu_shader5;
          continue;
        }
#endif
#ifdef GL_ARB_gpu_shader_fp64
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_shader_fp64", 15))
        {
          ret = GLEW_ARB_gpu_shader_fp64;
          continue;
        }
#endif
#ifdef GL_ARB_half_float_pixel
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"half_float_pixel", 16))
        {
          ret = GLEW_ARB_half_float_pixel;
          continue;
        }
#endif
#ifdef GL_ARB_half_float_vertex
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"half_float_vertex", 17))
        {
          ret = GLEW_ARB_half_float_vertex;
          continue;
        }
#endif
#ifdef GL_ARB_imaging
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"imaging", 7))
        {
          ret = GLEW_ARB_imaging;
          continue;
        }
#endif
#ifdef GL_ARB_instanced_arrays
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"instanced_arrays", 16))
        {
          ret = GLEW_ARB_instanced_arrays;
          continue;
        }
#endif
#ifdef GL_ARB_map_buffer_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"map_buffer_range", 16))
        {
          ret = GLEW_ARB_map_buffer_range;
          continue;
        }
#endif
#ifdef GL_ARB_matrix_palette
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"matrix_palette", 14))
        {
          ret = GLEW_ARB_matrix_palette;
          continue;
        }
#endif
#ifdef GL_ARB_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample", 11))
        {
          ret = GLEW_ARB_multisample;
          continue;
        }
#endif
#ifdef GL_ARB_multitexture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multitexture", 12))
        {
          ret = GLEW_ARB_multitexture;
          continue;
        }
#endif
#ifdef GL_ARB_occlusion_query
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"occlusion_query", 15))
        {
          ret = GLEW_ARB_occlusion_query;
          continue;
        }
#endif
#ifdef GL_ARB_occlusion_query2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"occlusion_query2", 16))
        {
          ret = GLEW_ARB_occlusion_query2;
          continue;
        }
#endif
#ifdef GL_ARB_pixel_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_buffer_object", 19))
        {
          ret = GLEW_ARB_pixel_buffer_object;
          continue;
        }
#endif
#ifdef GL_ARB_point_parameters
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"point_parameters", 16))
        {
          ret = GLEW_ARB_point_parameters;
          continue;
        }
#endif
#ifdef GL_ARB_point_sprite
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"point_sprite", 12))
        {
          ret = GLEW_ARB_point_sprite;
          continue;
        }
#endif
#ifdef GL_ARB_provoking_vertex
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"provoking_vertex", 16))
        {
          ret = GLEW_ARB_provoking_vertex;
          continue;
        }
#endif
#ifdef GL_ARB_robustness
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"robustness", 10))
        {
          ret = GLEW_ARB_robustness;
          continue;
        }
#endif
#ifdef GL_ARB_sample_shading
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"sample_shading", 14))
        {
          ret = GLEW_ARB_sample_shading;
          continue;
        }
#endif
#ifdef GL_ARB_sampler_objects
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"sampler_objects", 15))
        {
          ret = GLEW_ARB_sampler_objects;
          continue;
        }
#endif
#ifdef GL_ARB_seamless_cube_map
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"seamless_cube_map", 17))
        {
          ret = GLEW_ARB_seamless_cube_map;
          continue;
        }
#endif
#ifdef GL_ARB_separate_shader_objects
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"separate_shader_objects", 23))
        {
          ret = GLEW_ARB_separate_shader_objects;
          continue;
        }
#endif
#ifdef GL_ARB_shader_bit_encoding
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_bit_encoding", 19))
        {
          ret = GLEW_ARB_shader_bit_encoding;
          continue;
        }
#endif
#ifdef GL_ARB_shader_objects
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_objects", 14))
        {
          ret = GLEW_ARB_shader_objects;
          continue;
        }
#endif
#ifdef GL_ARB_shader_precision
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_precision", 16))
        {
          ret = GLEW_ARB_shader_precision;
          continue;
        }
#endif
#ifdef GL_ARB_shader_stencil_export
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_stencil_export", 21))
        {
          ret = GLEW_ARB_shader_stencil_export;
          continue;
        }
#endif
#ifdef GL_ARB_shader_subroutine
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_subroutine", 17))
        {
          ret = GLEW_ARB_shader_subroutine;
          continue;
        }
#endif
#ifdef GL_ARB_shader_texture_lod
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_texture_lod", 18))
        {
          ret = GLEW_ARB_shader_texture_lod;
          continue;
        }
#endif
#ifdef GL_ARB_shading_language_100
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shading_language_100", 20))
        {
          ret = GLEW_ARB_shading_language_100;
          continue;
        }
#endif
#ifdef GL_ARB_shading_language_include
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shading_language_include", 24))
        {
          ret = GLEW_ARB_shading_language_include;
          continue;
        }
#endif
#ifdef GL_ARB_shadow
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shadow", 6))
        {
          ret = GLEW_ARB_shadow;
          continue;
        }
#endif
#ifdef GL_ARB_shadow_ambient
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shadow_ambient", 14))
        {
          ret = GLEW_ARB_shadow_ambient;
          continue;
        }
#endif
#ifdef GL_ARB_sync
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"sync", 4))
        {
          ret = GLEW_ARB_sync;
          continue;
        }
#endif
#ifdef GL_ARB_tessellation_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"tessellation_shader", 19))
        {
          ret = GLEW_ARB_tessellation_shader;
          continue;
        }
#endif
#ifdef GL_ARB_texture_border_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_border_clamp", 20))
        {
          ret = GLEW_ARB_texture_border_clamp;
          continue;
        }
#endif
#ifdef GL_ARB_texture_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_buffer_object", 21))
        {
          ret = GLEW_ARB_texture_buffer_object;
          continue;
        }
#endif
#ifdef GL_ARB_texture_buffer_object_rgb32
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_buffer_object_rgb32", 27))
        {
          ret = GLEW_ARB_texture_buffer_object_rgb32;
          continue;
        }
#endif
#ifdef GL_ARB_texture_compression
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression", 19))
        {
          ret = GLEW_ARB_texture_compression;
          continue;
        }
#endif
#ifdef GL_ARB_texture_compression_bptc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_bptc", 24))
        {
          ret = GLEW_ARB_texture_compression_bptc;
          continue;
        }
#endif
#ifdef GL_ARB_texture_compression_rgtc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_rgtc", 24))
        {
          ret = GLEW_ARB_texture_compression_rgtc;
          continue;
        }
#endif
#ifdef GL_ARB_texture_cube_map
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_cube_map", 16))
        {
          ret = GLEW_ARB_texture_cube_map;
          continue;
        }
#endif
#ifdef GL_ARB_texture_cube_map_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_cube_map_array", 22))
        {
          ret = GLEW_ARB_texture_cube_map_array;
          continue;
        }
#endif
#ifdef GL_ARB_texture_env_add
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_add", 15))
        {
          ret = GLEW_ARB_texture_env_add;
          continue;
        }
#endif
#ifdef GL_ARB_texture_env_combine
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_combine", 19))
        {
          ret = GLEW_ARB_texture_env_combine;
          continue;
        }
#endif
#ifdef GL_ARB_texture_env_crossbar
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_crossbar", 20))
        {
          ret = GLEW_ARB_texture_env_crossbar;
          continue;
        }
#endif
#ifdef GL_ARB_texture_env_dot3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_dot3", 16))
        {
          ret = GLEW_ARB_texture_env_dot3;
          continue;
        }
#endif
#ifdef GL_ARB_texture_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_float", 13))
        {
          ret = GLEW_ARB_texture_float;
          continue;
        }
#endif
#ifdef GL_ARB_texture_gather
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_gather", 14))
        {
          ret = GLEW_ARB_texture_gather;
          continue;
        }
#endif
#ifdef GL_ARB_texture_mirrored_repeat
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_mirrored_repeat", 23))
        {
          ret = GLEW_ARB_texture_mirrored_repeat;
          continue;
        }
#endif
#ifdef GL_ARB_texture_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_multisample", 19))
        {
          ret = GLEW_ARB_texture_multisample;
          continue;
        }
#endif
#ifdef GL_ARB_texture_non_power_of_two
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_non_power_of_two", 24))
        {
          ret = GLEW_ARB_texture_non_power_of_two;
          continue;
        }
#endif
#ifdef GL_ARB_texture_query_lod
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_query_lod", 17))
        {
          ret = GLEW_ARB_texture_query_lod;
          continue;
        }
#endif
#ifdef GL_ARB_texture_rectangle
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_rectangle", 17))
        {
          ret = GLEW_ARB_texture_rectangle;
          continue;
        }
#endif
#ifdef GL_ARB_texture_rg
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_rg", 10))
        {
          ret = GLEW_ARB_texture_rg;
          continue;
        }
#endif
#ifdef GL_ARB_texture_rgb10_a2ui
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_rgb10_a2ui", 18))
        {
          ret = GLEW_ARB_texture_rgb10_a2ui;
          continue;
        }
#endif
#ifdef GL_ARB_texture_swizzle
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_swizzle", 15))
        {
          ret = GLEW_ARB_texture_swizzle;
          continue;
        }
#endif
#ifdef GL_ARB_timer_query
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"timer_query", 11))
        {
          ret = GLEW_ARB_timer_query;
          continue;
        }
#endif
#ifdef GL_ARB_transform_feedback2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transform_feedback2", 19))
        {
          ret = GLEW_ARB_transform_feedback2;
          continue;
        }
#endif
#ifdef GL_ARB_transform_feedback3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transform_feedback3", 19))
        {
          ret = GLEW_ARB_transform_feedback3;
          continue;
        }
#endif
#ifdef GL_ARB_transpose_matrix
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transpose_matrix", 16))
        {
          ret = GLEW_ARB_transpose_matrix;
          continue;
        }
#endif
#ifdef GL_ARB_uniform_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"uniform_buffer_object", 21))
        {
          ret = GLEW_ARB_uniform_buffer_object;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_array_bgra
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_bgra", 17))
        {
          ret = GLEW_ARB_vertex_array_bgra;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_array_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_object", 19))
        {
          ret = GLEW_ARB_vertex_array_object;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_attrib_64bit
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_attrib_64bit", 19))
        {
          ret = GLEW_ARB_vertex_attrib_64bit;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_blend
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_blend", 12))
        {
          ret = GLEW_ARB_vertex_blend;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_buffer_object", 20))
        {
          ret = GLEW_ARB_vertex_buffer_object;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_program
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program", 14))
        {
          ret = GLEW_ARB_vertex_program;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_shader", 13))
        {
          ret = GLEW_ARB_vertex_shader;
          continue;
        }
#endif
#ifdef GL_ARB_vertex_type_2_10_10_10_rev
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_type_2_10_10_10_rev", 26))
        {
          ret = GLEW_ARB_vertex_type_2_10_10_10_rev;
          continue;
        }
#endif
#ifdef GL_ARB_viewport_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"viewport_array", 14))
        {
          ret = GLEW_ARB_viewport_array;
          continue;
        }
#endif
#ifdef GL_ARB_window_pos
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"window_pos", 10))
        {
          ret = GLEW_ARB_window_pos;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"ATIX_", 5))
      {
#ifdef GL_ATIX_point_sprites
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"point_sprites", 13))
        {
          ret = GLEW_ATIX_point_sprites;
          continue;
        }
#endif
#ifdef GL_ATIX_texture_env_combine3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_combine3", 20))
        {
          ret = GLEW_ATIX_texture_env_combine3;
          continue;
        }
#endif
#ifdef GL_ATIX_texture_env_route
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_route", 17))
        {
          ret = GLEW_ATIX_texture_env_route;
          continue;
        }
#endif
#ifdef GL_ATIX_vertex_shader_output_point_size
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_shader_output_point_size", 31))
        {
          ret = GLEW_ATIX_vertex_shader_output_point_size;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"ATI_", 4))
      {
#ifdef GL_ATI_draw_buffers
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_buffers", 12))
        {
          ret = GLEW_ATI_draw_buffers;
          continue;
        }
#endif
#ifdef GL_ATI_element_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"element_array", 13))
        {
          ret = GLEW_ATI_element_array;
          continue;
        }
#endif
#ifdef GL_ATI_envmap_bumpmap
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"envmap_bumpmap", 14))
        {
          ret = GLEW_ATI_envmap_bumpmap;
          continue;
        }
#endif
#ifdef GL_ATI_fragment_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_shader", 15))
        {
          ret = GLEW_ATI_fragment_shader;
          continue;
        }
#endif
#ifdef GL_ATI_map_object_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"map_object_buffer", 17))
        {
          ret = GLEW_ATI_map_object_buffer;
          continue;
        }
#endif
#ifdef GL_ATI_meminfo
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"meminfo", 7))
        {
          ret = GLEW_ATI_meminfo;
          continue;
        }
#endif
#ifdef GL_ATI_pn_triangles
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pn_triangles", 12))
        {
          ret = GLEW_ATI_pn_triangles;
          continue;
        }
#endif
#ifdef GL_ATI_separate_stencil
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"separate_stencil", 16))
        {
          ret = GLEW_ATI_separate_stencil;
          continue;
        }
#endif
#ifdef GL_ATI_shader_texture_lod
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_texture_lod", 18))
        {
          ret = GLEW_ATI_shader_texture_lod;
          continue;
        }
#endif
#ifdef GL_ATI_text_fragment_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"text_fragment_shader", 20))
        {
          ret = GLEW_ATI_text_fragment_shader;
          continue;
        }
#endif
#ifdef GL_ATI_texture_compression_3dc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_3dc", 23))
        {
          ret = GLEW_ATI_texture_compression_3dc;
          continue;
        }
#endif
#ifdef GL_ATI_texture_env_combine3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_combine3", 20))
        {
          ret = GLEW_ATI_texture_env_combine3;
          continue;
        }
#endif
#ifdef GL_ATI_texture_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_float", 13))
        {
          ret = GLEW_ATI_texture_float;
          continue;
        }
#endif
#ifdef GL_ATI_texture_mirror_once
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_mirror_once", 19))
        {
          ret = GLEW_ATI_texture_mirror_once;
          continue;
        }
#endif
#ifdef GL_ATI_vertex_array_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_object", 19))
        {
          ret = GLEW_ATI_vertex_array_object;
          continue;
        }
#endif
#ifdef GL_ATI_vertex_attrib_array_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_attrib_array_object", 26))
        {
          ret = GLEW_ATI_vertex_attrib_array_object;
          continue;
        }
#endif
#ifdef GL_ATI_vertex_streams
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_streams", 14))
        {
          ret = GLEW_ATI_vertex_streams;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"EXT_", 4))
      {
#ifdef GL_EXT_422_pixels
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"422_pixels", 10))
        {
          ret = GLEW_EXT_422_pixels;
          continue;
        }
#endif
#ifdef GL_EXT_Cg_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"Cg_shader", 9))
        {
          ret = GLEW_EXT_Cg_shader;
          continue;
        }
#endif
#ifdef GL_EXT_abgr
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"abgr", 4))
        {
          ret = GLEW_EXT_abgr;
          continue;
        }
#endif
#ifdef GL_EXT_bgra
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"bgra", 4))
        {
          ret = GLEW_EXT_bgra;
          continue;
        }
#endif
#ifdef GL_EXT_bindable_uniform
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"bindable_uniform", 16))
        {
          ret = GLEW_EXT_bindable_uniform;
          continue;
        }
#endif
#ifdef GL_EXT_blend_color
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_color", 11))
        {
          ret = GLEW_EXT_blend_color;
          continue;
        }
#endif
#ifdef GL_EXT_blend_equation_separate
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_equation_separate", 23))
        {
          ret = GLEW_EXT_blend_equation_separate;
          continue;
        }
#endif
#ifdef GL_EXT_blend_func_separate
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_func_separate", 19))
        {
          ret = GLEW_EXT_blend_func_separate;
          continue;
        }
#endif
#ifdef GL_EXT_blend_logic_op
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_logic_op", 14))
        {
          ret = GLEW_EXT_blend_logic_op;
          continue;
        }
#endif
#ifdef GL_EXT_blend_minmax
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_minmax", 12))
        {
          ret = GLEW_EXT_blend_minmax;
          continue;
        }
#endif
#ifdef GL_EXT_blend_subtract
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_subtract", 14))
        {
          ret = GLEW_EXT_blend_subtract;
          continue;
        }
#endif
#ifdef GL_EXT_clip_volume_hint
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"clip_volume_hint", 16))
        {
          ret = GLEW_EXT_clip_volume_hint;
          continue;
        }
#endif
#ifdef GL_EXT_cmyka
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"cmyka", 5))
        {
          ret = GLEW_EXT_cmyka;
          continue;
        }
#endif
#ifdef GL_EXT_color_subtable
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"color_subtable", 14))
        {
          ret = GLEW_EXT_color_subtable;
          continue;
        }
#endif
#ifdef GL_EXT_compiled_vertex_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"compiled_vertex_array", 21))
        {
          ret = GLEW_EXT_compiled_vertex_array;
          continue;
        }
#endif
#ifdef GL_EXT_convolution
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"convolution", 11))
        {
          ret = GLEW_EXT_convolution;
          continue;
        }
#endif
#ifdef GL_EXT_coordinate_frame
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"coordinate_frame", 16))
        {
          ret = GLEW_EXT_coordinate_frame;
          continue;
        }
#endif
#ifdef GL_EXT_copy_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"copy_texture", 12))
        {
          ret = GLEW_EXT_copy_texture;
          continue;
        }
#endif
#ifdef GL_EXT_cull_vertex
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"cull_vertex", 11))
        {
          ret = GLEW_EXT_cull_vertex;
          continue;
        }
#endif
#ifdef GL_EXT_depth_bounds_test
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_bounds_test", 17))
        {
          ret = GLEW_EXT_depth_bounds_test;
          continue;
        }
#endif
#ifdef GL_EXT_direct_state_access
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"direct_state_access", 19))
        {
          ret = GLEW_EXT_direct_state_access;
          continue;
        }
#endif
#ifdef GL_EXT_draw_buffers2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_buffers2", 13))
        {
          ret = GLEW_EXT_draw_buffers2;
          continue;
        }
#endif
#ifdef GL_EXT_draw_instanced
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_instanced", 14))
        {
          ret = GLEW_EXT_draw_instanced;
          continue;
        }
#endif
#ifdef GL_EXT_draw_range_elements
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"draw_range_elements", 19))
        {
          ret = GLEW_EXT_draw_range_elements;
          continue;
        }
#endif
#ifdef GL_EXT_fog_coord
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fog_coord", 9))
        {
          ret = GLEW_EXT_fog_coord;
          continue;
        }
#endif
#ifdef GL_EXT_fragment_lighting
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_lighting", 17))
        {
          ret = GLEW_EXT_fragment_lighting;
          continue;
        }
#endif
#ifdef GL_EXT_framebuffer_blit
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_blit", 16))
        {
          ret = GLEW_EXT_framebuffer_blit;
          continue;
        }
#endif
#ifdef GL_EXT_framebuffer_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_multisample", 23))
        {
          ret = GLEW_EXT_framebuffer_multisample;
          continue;
        }
#endif
#ifdef GL_EXT_framebuffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_object", 18))
        {
          ret = GLEW_EXT_framebuffer_object;
          continue;
        }
#endif
#ifdef GL_EXT_framebuffer_sRGB
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_sRGB", 16))
        {
          ret = GLEW_EXT_framebuffer_sRGB;
          continue;
        }
#endif
#ifdef GL_EXT_geometry_shader4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"geometry_shader4", 16))
        {
          ret = GLEW_EXT_geometry_shader4;
          continue;
        }
#endif
#ifdef GL_EXT_gpu_program_parameters
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_program_parameters", 22))
        {
          ret = GLEW_EXT_gpu_program_parameters;
          continue;
        }
#endif
#ifdef GL_EXT_gpu_shader4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_shader4", 11))
        {
          ret = GLEW_EXT_gpu_shader4;
          continue;
        }
#endif
#ifdef GL_EXT_histogram
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"histogram", 9))
        {
          ret = GLEW_EXT_histogram;
          continue;
        }
#endif
#ifdef GL_EXT_index_array_formats
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"index_array_formats", 19))
        {
          ret = GLEW_EXT_index_array_formats;
          continue;
        }
#endif
#ifdef GL_EXT_index_func
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"index_func", 10))
        {
          ret = GLEW_EXT_index_func;
          continue;
        }
#endif
#ifdef GL_EXT_index_material
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"index_material", 14))
        {
          ret = GLEW_EXT_index_material;
          continue;
        }
#endif
#ifdef GL_EXT_index_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"index_texture", 13))
        {
          ret = GLEW_EXT_index_texture;
          continue;
        }
#endif
#ifdef GL_EXT_light_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"light_texture", 13))
        {
          ret = GLEW_EXT_light_texture;
          continue;
        }
#endif
#ifdef GL_EXT_misc_attribute
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"misc_attribute", 14))
        {
          ret = GLEW_EXT_misc_attribute;
          continue;
        }
#endif
#ifdef GL_EXT_multi_draw_arrays
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multi_draw_arrays", 17))
        {
          ret = GLEW_EXT_multi_draw_arrays;
          continue;
        }
#endif
#ifdef GL_EXT_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample", 11))
        {
          ret = GLEW_EXT_multisample;
          continue;
        }
#endif
#ifdef GL_EXT_packed_depth_stencil
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"packed_depth_stencil", 20))
        {
          ret = GLEW_EXT_packed_depth_stencil;
          continue;
        }
#endif
#ifdef GL_EXT_packed_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"packed_float", 12))
        {
          ret = GLEW_EXT_packed_float;
          continue;
        }
#endif
#ifdef GL_EXT_packed_pixels
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"packed_pixels", 13))
        {
          ret = GLEW_EXT_packed_pixels;
          continue;
        }
#endif
#ifdef GL_EXT_paletted_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"paletted_texture", 16))
        {
          ret = GLEW_EXT_paletted_texture;
          continue;
        }
#endif
#ifdef GL_EXT_pixel_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_buffer_object", 19))
        {
          ret = GLEW_EXT_pixel_buffer_object;
          continue;
        }
#endif
#ifdef GL_EXT_pixel_transform
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_transform", 15))
        {
          ret = GLEW_EXT_pixel_transform;
          continue;
        }
#endif
#ifdef GL_EXT_pixel_transform_color_table
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_transform_color_table", 27))
        {
          ret = GLEW_EXT_pixel_transform_color_table;
          continue;
        }
#endif
#ifdef GL_EXT_point_parameters
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"point_parameters", 16))
        {
          ret = GLEW_EXT_point_parameters;
          continue;
        }
#endif
#ifdef GL_EXT_polygon_offset
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"polygon_offset", 14))
        {
          ret = GLEW_EXT_polygon_offset;
          continue;
        }
#endif
#ifdef GL_EXT_provoking_vertex
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"provoking_vertex", 16))
        {
          ret = GLEW_EXT_provoking_vertex;
          continue;
        }
#endif
#ifdef GL_EXT_rescale_normal
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"rescale_normal", 14))
        {
          ret = GLEW_EXT_rescale_normal;
          continue;
        }
#endif
#ifdef GL_EXT_scene_marker
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"scene_marker", 12))
        {
          ret = GLEW_EXT_scene_marker;
          continue;
        }
#endif
#ifdef GL_EXT_secondary_color
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"secondary_color", 15))
        {
          ret = GLEW_EXT_secondary_color;
          continue;
        }
#endif
#ifdef GL_EXT_separate_shader_objects
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"separate_shader_objects", 23))
        {
          ret = GLEW_EXT_separate_shader_objects;
          continue;
        }
#endif
#ifdef GL_EXT_separate_specular_color
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"separate_specular_color", 23))
        {
          ret = GLEW_EXT_separate_specular_color;
          continue;
        }
#endif
#ifdef GL_EXT_shader_image_load_store
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_image_load_store", 23))
        {
          ret = GLEW_EXT_shader_image_load_store;
          continue;
        }
#endif
#ifdef GL_EXT_shadow_funcs
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shadow_funcs", 12))
        {
          ret = GLEW_EXT_shadow_funcs;
          continue;
        }
#endif
#ifdef GL_EXT_shared_texture_palette
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shared_texture_palette", 22))
        {
          ret = GLEW_EXT_shared_texture_palette;
          continue;
        }
#endif
#ifdef GL_EXT_stencil_clear_tag
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"stencil_clear_tag", 17))
        {
          ret = GLEW_EXT_stencil_clear_tag;
          continue;
        }
#endif
#ifdef GL_EXT_stencil_two_side
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"stencil_two_side", 16))
        {
          ret = GLEW_EXT_stencil_two_side;
          continue;
        }
#endif
#ifdef GL_EXT_stencil_wrap
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"stencil_wrap", 12))
        {
          ret = GLEW_EXT_stencil_wrap;
          continue;
        }
#endif
#ifdef GL_EXT_subtexture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"subtexture", 10))
        {
          ret = GLEW_EXT_subtexture;
          continue;
        }
#endif
#ifdef GL_EXT_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture", 7))
        {
          ret = GLEW_EXT_texture;
          continue;
        }
#endif
#ifdef GL_EXT_texture3D
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture3D", 9))
        {
          ret = GLEW_EXT_texture3D;
          continue;
        }
#endif
#ifdef GL_EXT_texture_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_array", 13))
        {
          ret = GLEW_EXT_texture_array;
          continue;
        }
#endif
#ifdef GL_EXT_texture_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_buffer_object", 21))
        {
          ret = GLEW_EXT_texture_buffer_object;
          continue;
        }
#endif
#ifdef GL_EXT_texture_compression_dxt1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_dxt1", 24))
        {
          ret = GLEW_EXT_texture_compression_dxt1;
          continue;
        }
#endif
#ifdef GL_EXT_texture_compression_latc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_latc", 24))
        {
          ret = GLEW_EXT_texture_compression_latc;
          continue;
        }
#endif
#ifdef GL_EXT_texture_compression_rgtc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_rgtc", 24))
        {
          ret = GLEW_EXT_texture_compression_rgtc;
          continue;
        }
#endif
#ifdef GL_EXT_texture_compression_s3tc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_s3tc", 24))
        {
          ret = GLEW_EXT_texture_compression_s3tc;
          continue;
        }
#endif
#ifdef GL_EXT_texture_cube_map
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_cube_map", 16))
        {
          ret = GLEW_EXT_texture_cube_map;
          continue;
        }
#endif
#ifdef GL_EXT_texture_edge_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_edge_clamp", 18))
        {
          ret = GLEW_EXT_texture_edge_clamp;
          continue;
        }
#endif
#ifdef GL_EXT_texture_env
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env", 11))
        {
          ret = GLEW_EXT_texture_env;
          continue;
        }
#endif
#ifdef GL_EXT_texture_env_add
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_add", 15))
        {
          ret = GLEW_EXT_texture_env_add;
          continue;
        }
#endif
#ifdef GL_EXT_texture_env_combine
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_combine", 19))
        {
          ret = GLEW_EXT_texture_env_combine;
          continue;
        }
#endif
#ifdef GL_EXT_texture_env_dot3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_dot3", 16))
        {
          ret = GLEW_EXT_texture_env_dot3;
          continue;
        }
#endif
#ifdef GL_EXT_texture_filter_anisotropic
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_filter_anisotropic", 26))
        {
          ret = GLEW_EXT_texture_filter_anisotropic;
          continue;
        }
#endif
#ifdef GL_EXT_texture_integer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_integer", 15))
        {
          ret = GLEW_EXT_texture_integer;
          continue;
        }
#endif
#ifdef GL_EXT_texture_lod_bias
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_lod_bias", 16))
        {
          ret = GLEW_EXT_texture_lod_bias;
          continue;
        }
#endif
#ifdef GL_EXT_texture_mirror_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_mirror_clamp", 20))
        {
          ret = GLEW_EXT_texture_mirror_clamp;
          continue;
        }
#endif
#ifdef GL_EXT_texture_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_object", 14))
        {
          ret = GLEW_EXT_texture_object;
          continue;
        }
#endif
#ifdef GL_EXT_texture_perturb_normal
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_perturb_normal", 22))
        {
          ret = GLEW_EXT_texture_perturb_normal;
          continue;
        }
#endif
#ifdef GL_EXT_texture_rectangle
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_rectangle", 17))
        {
          ret = GLEW_EXT_texture_rectangle;
          continue;
        }
#endif
#ifdef GL_EXT_texture_sRGB
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_sRGB", 12))
        {
          ret = GLEW_EXT_texture_sRGB;
          continue;
        }
#endif
#ifdef GL_EXT_texture_sRGB_decode
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_sRGB_decode", 19))
        {
          ret = GLEW_EXT_texture_sRGB_decode;
          continue;
        }
#endif
#ifdef GL_EXT_texture_shared_exponent
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_shared_exponent", 23))
        {
          ret = GLEW_EXT_texture_shared_exponent;
          continue;
        }
#endif
#ifdef GL_EXT_texture_snorm
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_snorm", 13))
        {
          ret = GLEW_EXT_texture_snorm;
          continue;
        }
#endif
#ifdef GL_EXT_texture_swizzle
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_swizzle", 15))
        {
          ret = GLEW_EXT_texture_swizzle;
          continue;
        }
#endif
#ifdef GL_EXT_timer_query
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"timer_query", 11))
        {
          ret = GLEW_EXT_timer_query;
          continue;
        }
#endif
#ifdef GL_EXT_transform_feedback
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transform_feedback", 18))
        {
          ret = GLEW_EXT_transform_feedback;
          continue;
        }
#endif
#ifdef GL_EXT_vertex_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array", 12))
        {
          ret = GLEW_EXT_vertex_array;
          continue;
        }
#endif
#ifdef GL_EXT_vertex_array_bgra
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_bgra", 17))
        {
          ret = GLEW_EXT_vertex_array_bgra;
          continue;
        }
#endif
#ifdef GL_EXT_vertex_attrib_64bit
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_attrib_64bit", 19))
        {
          ret = GLEW_EXT_vertex_attrib_64bit;
          continue;
        }
#endif
#ifdef GL_EXT_vertex_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_shader", 13))
        {
          ret = GLEW_EXT_vertex_shader;
          continue;
        }
#endif
#ifdef GL_EXT_vertex_weighting
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_weighting", 16))
        {
          ret = GLEW_EXT_vertex_weighting;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"GREMEDY_", 8))
      {
#ifdef GL_GREMEDY_frame_terminator
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"frame_terminator", 16))
        {
          ret = GLEW_GREMEDY_frame_terminator;
          continue;
        }
#endif
#ifdef GL_GREMEDY_string_marker
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"string_marker", 13))
        {
          ret = GLEW_GREMEDY_string_marker;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"HP_", 3))
      {
#ifdef GL_HP_convolution_border_modes
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"convolution_border_modes", 24))
        {
          ret = GLEW_HP_convolution_border_modes;
          continue;
        }
#endif
#ifdef GL_HP_image_transform
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"image_transform", 15))
        {
          ret = GLEW_HP_image_transform;
          continue;
        }
#endif
#ifdef GL_HP_occlusion_test
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"occlusion_test", 14))
        {
          ret = GLEW_HP_occlusion_test;
          continue;
        }
#endif
#ifdef GL_HP_texture_lighting
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_lighting", 16))
        {
          ret = GLEW_HP_texture_lighting;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"IBM_", 4))
      {
#ifdef GL_IBM_cull_vertex
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"cull_vertex", 11))
        {
          ret = GLEW_IBM_cull_vertex;
          continue;
        }
#endif
#ifdef GL_IBM_multimode_draw_arrays
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multimode_draw_arrays", 21))
        {
          ret = GLEW_IBM_multimode_draw_arrays;
          continue;
        }
#endif
#ifdef GL_IBM_rasterpos_clip
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"rasterpos_clip", 14))
        {
          ret = GLEW_IBM_rasterpos_clip;
          continue;
        }
#endif
#ifdef GL_IBM_static_data
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"static_data", 11))
        {
          ret = GLEW_IBM_static_data;
          continue;
        }
#endif
#ifdef GL_IBM_texture_mirrored_repeat
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_mirrored_repeat", 23))
        {
          ret = GLEW_IBM_texture_mirrored_repeat;
          continue;
        }
#endif
#ifdef GL_IBM_vertex_array_lists
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_lists", 18))
        {
          ret = GLEW_IBM_vertex_array_lists;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"INGR_", 5))
      {
#ifdef GL_INGR_color_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"color_clamp", 11))
        {
          ret = GLEW_INGR_color_clamp;
          continue;
        }
#endif
#ifdef GL_INGR_interlace_read
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"interlace_read", 14))
        {
          ret = GLEW_INGR_interlace_read;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"INTEL_", 6))
      {
#ifdef GL_INTEL_parallel_arrays
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"parallel_arrays", 15))
        {
          ret = GLEW_INTEL_parallel_arrays;
          continue;
        }
#endif
#ifdef GL_INTEL_texture_scissor
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_scissor", 15))
        {
          ret = GLEW_INTEL_texture_scissor;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"KTX_", 4))
      {
#ifdef GL_KTX_buffer_region
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"buffer_region", 13))
        {
          ret = GLEW_KTX_buffer_region;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"MESAX_", 6))
      {
#ifdef GL_MESAX_texture_stack
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_stack", 13))
        {
          ret = GLEW_MESAX_texture_stack;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"MESA_", 5))
      {
#ifdef GL_MESA_pack_invert
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pack_invert", 11))
        {
          ret = GLEW_MESA_pack_invert;
          continue;
        }
#endif
#ifdef GL_MESA_resize_buffers
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"resize_buffers", 14))
        {
          ret = GLEW_MESA_resize_buffers;
          continue;
        }
#endif
#ifdef GL_MESA_window_pos
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"window_pos", 10))
        {
          ret = GLEW_MESA_window_pos;
          continue;
        }
#endif
#ifdef GL_MESA_ycbcr_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"ycbcr_texture", 13))
        {
          ret = GLEW_MESA_ycbcr_texture;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"NVX_", 4))
      {
#ifdef GL_NVX_gpu_memory_info
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_memory_info", 15))
        {
          ret = GLEW_NVX_gpu_memory_info;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"NV_", 3))
      {
#ifdef GL_NV_blend_square
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_square", 12))
        {
          ret = GLEW_NV_blend_square;
          continue;
        }
#endif
#ifdef GL_NV_conditional_render
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"conditional_render", 18))
        {
          ret = GLEW_NV_conditional_render;
          continue;
        }
#endif
#ifdef GL_NV_copy_depth_to_color
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"copy_depth_to_color", 19))
        {
          ret = GLEW_NV_copy_depth_to_color;
          continue;
        }
#endif
#ifdef GL_NV_copy_image
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"copy_image", 10))
        {
          ret = GLEW_NV_copy_image;
          continue;
        }
#endif
#ifdef GL_NV_depth_buffer_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_buffer_float", 18))
        {
          ret = GLEW_NV_depth_buffer_float;
          continue;
        }
#endif
#ifdef GL_NV_depth_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_clamp", 11))
        {
          ret = GLEW_NV_depth_clamp;
          continue;
        }
#endif
#ifdef GL_NV_depth_range_unclamped
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_range_unclamped", 21))
        {
          ret = GLEW_NV_depth_range_unclamped;
          continue;
        }
#endif
#ifdef GL_NV_evaluators
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"evaluators", 10))
        {
          ret = GLEW_NV_evaluators;
          continue;
        }
#endif
#ifdef GL_NV_explicit_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"explicit_multisample", 20))
        {
          ret = GLEW_NV_explicit_multisample;
          continue;
        }
#endif
#ifdef GL_NV_fence
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fence", 5))
        {
          ret = GLEW_NV_fence;
          continue;
        }
#endif
#ifdef GL_NV_float_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"float_buffer", 12))
        {
          ret = GLEW_NV_float_buffer;
          continue;
        }
#endif
#ifdef GL_NV_fog_distance
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fog_distance", 12))
        {
          ret = GLEW_NV_fog_distance;
          continue;
        }
#endif
#ifdef GL_NV_fragment_program
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_program", 16))
        {
          ret = GLEW_NV_fragment_program;
          continue;
        }
#endif
#ifdef GL_NV_fragment_program2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_program2", 17))
        {
          ret = GLEW_NV_fragment_program2;
          continue;
        }
#endif
#ifdef GL_NV_fragment_program4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_program4", 17))
        {
          ret = GLEW_NV_fragment_program4;
          continue;
        }
#endif
#ifdef GL_NV_fragment_program_option
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_program_option", 23))
        {
          ret = GLEW_NV_fragment_program_option;
          continue;
        }
#endif
#ifdef GL_NV_framebuffer_multisample_coverage
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_multisample_coverage", 32))
        {
          ret = GLEW_NV_framebuffer_multisample_coverage;
          continue;
        }
#endif
#ifdef GL_NV_geometry_program4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"geometry_program4", 17))
        {
          ret = GLEW_NV_geometry_program4;
          continue;
        }
#endif
#ifdef GL_NV_geometry_shader4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"geometry_shader4", 16))
        {
          ret = GLEW_NV_geometry_shader4;
          continue;
        }
#endif
#ifdef GL_NV_gpu_program4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_program4", 12))
        {
          ret = GLEW_NV_gpu_program4;
          continue;
        }
#endif
#ifdef GL_NV_gpu_program5
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_program5", 12))
        {
          ret = GLEW_NV_gpu_program5;
          continue;
        }
#endif
#ifdef GL_NV_gpu_program_fp64
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_program_fp64", 16))
        {
          ret = GLEW_NV_gpu_program_fp64;
          continue;
        }
#endif
#ifdef GL_NV_gpu_shader5
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_shader5", 11))
        {
          ret = GLEW_NV_gpu_shader5;
          continue;
        }
#endif
#ifdef GL_NV_half_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"half_float", 10))
        {
          ret = GLEW_NV_half_float;
          continue;
        }
#endif
#ifdef GL_NV_light_max_exponent
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"light_max_exponent", 18))
        {
          ret = GLEW_NV_light_max_exponent;
          continue;
        }
#endif
#ifdef GL_NV_multisample_coverage
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample_coverage", 20))
        {
          ret = GLEW_NV_multisample_coverage;
          continue;
        }
#endif
#ifdef GL_NV_multisample_filter_hint
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample_filter_hint", 23))
        {
          ret = GLEW_NV_multisample_filter_hint;
          continue;
        }
#endif
#ifdef GL_NV_occlusion_query
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"occlusion_query", 15))
        {
          ret = GLEW_NV_occlusion_query;
          continue;
        }
#endif
#ifdef GL_NV_packed_depth_stencil
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"packed_depth_stencil", 20))
        {
          ret = GLEW_NV_packed_depth_stencil;
          continue;
        }
#endif
#ifdef GL_NV_parameter_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"parameter_buffer_object", 23))
        {
          ret = GLEW_NV_parameter_buffer_object;
          continue;
        }
#endif
#ifdef GL_NV_parameter_buffer_object2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"parameter_buffer_object2", 24))
        {
          ret = GLEW_NV_parameter_buffer_object2;
          continue;
        }
#endif
#ifdef GL_NV_pixel_data_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_data_range", 16))
        {
          ret = GLEW_NV_pixel_data_range;
          continue;
        }
#endif
#ifdef GL_NV_point_sprite
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"point_sprite", 12))
        {
          ret = GLEW_NV_point_sprite;
          continue;
        }
#endif
#ifdef GL_NV_present_video
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"present_video", 13))
        {
          ret = GLEW_NV_present_video;
          continue;
        }
#endif
#ifdef GL_NV_primitive_restart
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"primitive_restart", 17))
        {
          ret = GLEW_NV_primitive_restart;
          continue;
        }
#endif
#ifdef GL_NV_register_combiners
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"register_combiners", 18))
        {
          ret = GLEW_NV_register_combiners;
          continue;
        }
#endif
#ifdef GL_NV_register_combiners2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"register_combiners2", 19))
        {
          ret = GLEW_NV_register_combiners2;
          continue;
        }
#endif
#ifdef GL_NV_shader_buffer_load
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shader_buffer_load", 18))
        {
          ret = GLEW_NV_shader_buffer_load;
          continue;
        }
#endif
#ifdef GL_NV_tessellation_program5
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"tessellation_program5", 21))
        {
          ret = GLEW_NV_tessellation_program5;
          continue;
        }
#endif
#ifdef GL_NV_texgen_emboss
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texgen_emboss", 13))
        {
          ret = GLEW_NV_texgen_emboss;
          continue;
        }
#endif
#ifdef GL_NV_texgen_reflection
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texgen_reflection", 17))
        {
          ret = GLEW_NV_texgen_reflection;
          continue;
        }
#endif
#ifdef GL_NV_texture_barrier
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_barrier", 15))
        {
          ret = GLEW_NV_texture_barrier;
          continue;
        }
#endif
#ifdef GL_NV_texture_compression_vtc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_compression_vtc", 23))
        {
          ret = GLEW_NV_texture_compression_vtc;
          continue;
        }
#endif
#ifdef GL_NV_texture_env_combine4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_env_combine4", 20))
        {
          ret = GLEW_NV_texture_env_combine4;
          continue;
        }
#endif
#ifdef GL_NV_texture_expand_normal
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_expand_normal", 21))
        {
          ret = GLEW_NV_texture_expand_normal;
          continue;
        }
#endif
#ifdef GL_NV_texture_rectangle
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_rectangle", 17))
        {
          ret = GLEW_NV_texture_rectangle;
          continue;
        }
#endif
#ifdef GL_NV_texture_shader
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_shader", 14))
        {
          ret = GLEW_NV_texture_shader;
          continue;
        }
#endif
#ifdef GL_NV_texture_shader2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_shader2", 15))
        {
          ret = GLEW_NV_texture_shader2;
          continue;
        }
#endif
#ifdef GL_NV_texture_shader3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_shader3", 15))
        {
          ret = GLEW_NV_texture_shader3;
          continue;
        }
#endif
#ifdef GL_NV_transform_feedback
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transform_feedback", 18))
        {
          ret = GLEW_NV_transform_feedback;
          continue;
        }
#endif
#ifdef GL_NV_transform_feedback2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"transform_feedback2", 19))
        {
          ret = GLEW_NV_transform_feedback2;
          continue;
        }
#endif
#ifdef GL_NV_vdpau_interop
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vdpau_interop", 13))
        {
          ret = GLEW_NV_vdpau_interop;
          continue;
        }
#endif
#ifdef GL_NV_vertex_array_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_range", 18))
        {
          ret = GLEW_NV_vertex_array_range;
          continue;
        }
#endif
#ifdef GL_NV_vertex_array_range2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_range2", 19))
        {
          ret = GLEW_NV_vertex_array_range2;
          continue;
        }
#endif
#ifdef GL_NV_vertex_attrib_integer_64bit
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_attrib_integer_64bit", 27))
        {
          ret = GLEW_NV_vertex_attrib_integer_64bit;
          continue;
        }
#endif
#ifdef GL_NV_vertex_buffer_unified_memory
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_buffer_unified_memory", 28))
        {
          ret = GLEW_NV_vertex_buffer_unified_memory;
          continue;
        }
#endif
#ifdef GL_NV_vertex_program
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program", 14))
        {
          ret = GLEW_NV_vertex_program;
          continue;
        }
#endif
#ifdef GL_NV_vertex_program1_1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program1_1", 17))
        {
          ret = GLEW_NV_vertex_program1_1;
          continue;
        }
#endif
#ifdef GL_NV_vertex_program2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program2", 15))
        {
          ret = GLEW_NV_vertex_program2;
          continue;
        }
#endif
#ifdef GL_NV_vertex_program2_option
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program2_option", 22))
        {
          ret = GLEW_NV_vertex_program2_option;
          continue;
        }
#endif
#ifdef GL_NV_vertex_program3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program3", 15))
        {
          ret = GLEW_NV_vertex_program3;
          continue;
        }
#endif
#ifdef GL_NV_vertex_program4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_program4", 15))
        {
          ret = GLEW_NV_vertex_program4;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"OES_", 4))
      {
#ifdef GL_OES_byte_coordinates
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"byte_coordinates", 16))
        {
          ret = GLEW_OES_byte_coordinates;
          continue;
        }
#endif
#ifdef GL_OES_compressed_paletted_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"compressed_paletted_texture", 27))
        {
          ret = GLEW_OES_compressed_paletted_texture;
          continue;
        }
#endif
#ifdef GL_OES_read_format
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"read_format", 11))
        {
          ret = GLEW_OES_read_format;
          continue;
        }
#endif
#ifdef GL_OES_single_precision
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"single_precision", 16))
        {
          ret = GLEW_OES_single_precision;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"OML_", 4))
      {
#ifdef GL_OML_interlace
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"interlace", 9))
        {
          ret = GLEW_OML_interlace;
          continue;
        }
#endif
#ifdef GL_OML_resample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"resample", 8))
        {
          ret = GLEW_OML_resample;
          continue;
        }
#endif
#ifdef GL_OML_subsample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"subsample", 9))
        {
          ret = GLEW_OML_subsample;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"PGI_", 4))
      {
#ifdef GL_PGI_misc_hints
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"misc_hints", 10))
        {
          ret = GLEW_PGI_misc_hints;
          continue;
        }
#endif
#ifdef GL_PGI_vertex_hints
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_hints", 12))
        {
          ret = GLEW_PGI_vertex_hints;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"REND_", 5))
      {
#ifdef GL_REND_screen_coordinates
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"screen_coordinates", 18))
        {
          ret = GLEW_REND_screen_coordinates;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"S3_", 3))
      {
#ifdef GL_S3_s3tc
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"s3tc", 4))
        {
          ret = GLEW_S3_s3tc;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SGIS_", 5))
      {
#ifdef GL_SGIS_color_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"color_range", 11))
        {
          ret = GLEW_SGIS_color_range;
          continue;
        }
#endif
#ifdef GL_SGIS_detail_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"detail_texture", 14))
        {
          ret = GLEW_SGIS_detail_texture;
          continue;
        }
#endif
#ifdef GL_SGIS_fog_function
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fog_function", 12))
        {
          ret = GLEW_SGIS_fog_function;
          continue;
        }
#endif
#ifdef GL_SGIS_generate_mipmap
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"generate_mipmap", 15))
        {
          ret = GLEW_SGIS_generate_mipmap;
          continue;
        }
#endif
#ifdef GL_SGIS_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample", 11))
        {
          ret = GLEW_SGIS_multisample;
          continue;
        }
#endif
#ifdef GL_SGIS_pixel_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_texture", 13))
        {
          ret = GLEW_SGIS_pixel_texture;
          continue;
        }
#endif
#ifdef GL_SGIS_point_line_texgen
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"point_line_texgen", 17))
        {
          ret = GLEW_SGIS_point_line_texgen;
          continue;
        }
#endif
#ifdef GL_SGIS_sharpen_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"sharpen_texture", 15))
        {
          ret = GLEW_SGIS_sharpen_texture;
          continue;
        }
#endif
#ifdef GL_SGIS_texture4D
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture4D", 9))
        {
          ret = GLEW_SGIS_texture4D;
          continue;
        }
#endif
#ifdef GL_SGIS_texture_border_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_border_clamp", 20))
        {
          ret = GLEW_SGIS_texture_border_clamp;
          continue;
        }
#endif
#ifdef GL_SGIS_texture_edge_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_edge_clamp", 18))
        {
          ret = GLEW_SGIS_texture_edge_clamp;
          continue;
        }
#endif
#ifdef GL_SGIS_texture_filter4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_filter4", 15))
        {
          ret = GLEW_SGIS_texture_filter4;
          continue;
        }
#endif
#ifdef GL_SGIS_texture_lod
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_lod", 11))
        {
          ret = GLEW_SGIS_texture_lod;
          continue;
        }
#endif
#ifdef GL_SGIS_texture_select
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_select", 14))
        {
          ret = GLEW_SGIS_texture_select;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SGIX_", 5))
      {
#ifdef GL_SGIX_async
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"async", 5))
        {
          ret = GLEW_SGIX_async;
          continue;
        }
#endif
#ifdef GL_SGIX_async_histogram
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"async_histogram", 15))
        {
          ret = GLEW_SGIX_async_histogram;
          continue;
        }
#endif
#ifdef GL_SGIX_async_pixel
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"async_pixel", 11))
        {
          ret = GLEW_SGIX_async_pixel;
          continue;
        }
#endif
#ifdef GL_SGIX_blend_alpha_minmax
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blend_alpha_minmax", 18))
        {
          ret = GLEW_SGIX_blend_alpha_minmax;
          continue;
        }
#endif
#ifdef GL_SGIX_clipmap
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"clipmap", 7))
        {
          ret = GLEW_SGIX_clipmap;
          continue;
        }
#endif
#ifdef GL_SGIX_convolution_accuracy
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"convolution_accuracy", 20))
        {
          ret = GLEW_SGIX_convolution_accuracy;
          continue;
        }
#endif
#ifdef GL_SGIX_depth_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"depth_texture", 13))
        {
          ret = GLEW_SGIX_depth_texture;
          continue;
        }
#endif
#ifdef GL_SGIX_flush_raster
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"flush_raster", 12))
        {
          ret = GLEW_SGIX_flush_raster;
          continue;
        }
#endif
#ifdef GL_SGIX_fog_offset
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fog_offset", 10))
        {
          ret = GLEW_SGIX_fog_offset;
          continue;
        }
#endif
#ifdef GL_SGIX_fog_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fog_texture", 11))
        {
          ret = GLEW_SGIX_fog_texture;
          continue;
        }
#endif
#ifdef GL_SGIX_fragment_specular_lighting
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fragment_specular_lighting", 26))
        {
          ret = GLEW_SGIX_fragment_specular_lighting;
          continue;
        }
#endif
#ifdef GL_SGIX_framezoom
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framezoom", 9))
        {
          ret = GLEW_SGIX_framezoom;
          continue;
        }
#endif
#ifdef GL_SGIX_interlace
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"interlace", 9))
        {
          ret = GLEW_SGIX_interlace;
          continue;
        }
#endif
#ifdef GL_SGIX_ir_instrument1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"ir_instrument1", 14))
        {
          ret = GLEW_SGIX_ir_instrument1;
          continue;
        }
#endif
#ifdef GL_SGIX_list_priority
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"list_priority", 13))
        {
          ret = GLEW_SGIX_list_priority;
          continue;
        }
#endif
#ifdef GL_SGIX_pixel_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_texture", 13))
        {
          ret = GLEW_SGIX_pixel_texture;
          continue;
        }
#endif
#ifdef GL_SGIX_pixel_texture_bits
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_texture_bits", 18))
        {
          ret = GLEW_SGIX_pixel_texture_bits;
          continue;
        }
#endif
#ifdef GL_SGIX_reference_plane
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"reference_plane", 15))
        {
          ret = GLEW_SGIX_reference_plane;
          continue;
        }
#endif
#ifdef GL_SGIX_resample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"resample", 8))
        {
          ret = GLEW_SGIX_resample;
          continue;
        }
#endif
#ifdef GL_SGIX_shadow
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shadow", 6))
        {
          ret = GLEW_SGIX_shadow;
          continue;
        }
#endif
#ifdef GL_SGIX_shadow_ambient
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shadow_ambient", 14))
        {
          ret = GLEW_SGIX_shadow_ambient;
          continue;
        }
#endif
#ifdef GL_SGIX_sprite
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"sprite", 6))
        {
          ret = GLEW_SGIX_sprite;
          continue;
        }
#endif
#ifdef GL_SGIX_tag_sample_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"tag_sample_buffer", 17))
        {
          ret = GLEW_SGIX_tag_sample_buffer;
          continue;
        }
#endif
#ifdef GL_SGIX_texture_add_env
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_add_env", 15))
        {
          ret = GLEW_SGIX_texture_add_env;
          continue;
        }
#endif
#ifdef GL_SGIX_texture_coordinate_clamp
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_coordinate_clamp", 24))
        {
          ret = GLEW_SGIX_texture_coordinate_clamp;
          continue;
        }
#endif
#ifdef GL_SGIX_texture_lod_bias
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_lod_bias", 16))
        {
          ret = GLEW_SGIX_texture_lod_bias;
          continue;
        }
#endif
#ifdef GL_SGIX_texture_multi_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_multi_buffer", 20))
        {
          ret = GLEW_SGIX_texture_multi_buffer;
          continue;
        }
#endif
#ifdef GL_SGIX_texture_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_range", 13))
        {
          ret = GLEW_SGIX_texture_range;
          continue;
        }
#endif
#ifdef GL_SGIX_texture_scale_bias
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_scale_bias", 18))
        {
          ret = GLEW_SGIX_texture_scale_bias;
          continue;
        }
#endif
#ifdef GL_SGIX_vertex_preclip
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_preclip", 14))
        {
          ret = GLEW_SGIX_vertex_preclip;
          continue;
        }
#endif
#ifdef GL_SGIX_vertex_preclip_hint
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_preclip_hint", 19))
        {
          ret = GLEW_SGIX_vertex_preclip_hint;
          continue;
        }
#endif
#ifdef GL_SGIX_ycrcb
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"ycrcb", 5))
        {
          ret = GLEW_SGIX_ycrcb;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SGI_", 4))
      {
#ifdef GL_SGI_color_matrix
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"color_matrix", 12))
        {
          ret = GLEW_SGI_color_matrix;
          continue;
        }
#endif
#ifdef GL_SGI_color_table
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"color_table", 11))
        {
          ret = GLEW_SGI_color_table;
          continue;
        }
#endif
#ifdef GL_SGI_texture_color_table
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_color_table", 19))
        {
          ret = GLEW_SGI_texture_color_table;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SUNX_", 5))
      {
#ifdef GL_SUNX_constant_data
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"constant_data", 13))
        {
          ret = GLEW_SUNX_constant_data;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SUN_", 4))
      {
#ifdef GL_SUN_convolution_border_modes
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"convolution_border_modes", 24))
        {
          ret = GLEW_SUN_convolution_border_modes;
          continue;
        }
#endif
#ifdef GL_SUN_global_alpha
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"global_alpha", 12))
        {
          ret = GLEW_SUN_global_alpha;
          continue;
        }
#endif
#ifdef GL_SUN_mesh_array
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"mesh_array", 10))
        {
          ret = GLEW_SUN_mesh_array;
          continue;
        }
#endif
#ifdef GL_SUN_read_video_pixels
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"read_video_pixels", 17))
        {
          ret = GLEW_SUN_read_video_pixels;
          continue;
        }
#endif
#ifdef GL_SUN_slice_accum
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"slice_accum", 11))
        {
          ret = GLEW_SUN_slice_accum;
          continue;
        }
#endif
#ifdef GL_SUN_triangle_list
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"triangle_list", 13))
        {
          ret = GLEW_SUN_triangle_list;
          continue;
        }
#endif
#ifdef GL_SUN_vertex
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex", 6))
        {
          ret = GLEW_SUN_vertex;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"WIN_", 4))
      {
#ifdef GL_WIN_phong_shading
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"phong_shading", 13))
        {
          ret = GLEW_WIN_phong_shading;
          continue;
        }
#endif
#ifdef GL_WIN_specular_fog
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"specular_fog", 12))
        {
          ret = GLEW_WIN_specular_fog;
          continue;
        }
#endif
#ifdef GL_WIN_swap_hint
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_hint", 9))
        {
          ret = GLEW_WIN_swap_hint;
          continue;
        }
#endif
      }
    }
    ret = (len == 0);
  }
  return ret;
}

#if defined(_WIN32)

#if defined(GLEW_MX)
GLboolean wglewContextIsSupported (const WGLEWContext* ctx, const char* name)
#else
GLboolean wglewIsSupported (const char* name)
#endif
{
  GLubyte* pos = (GLubyte*)name;
  GLuint len = _glewStrLen(pos);
  GLboolean ret = GL_TRUE;
  while (ret && len > 0)
  {
    if (_glewStrSame1(&pos, &len, (const GLubyte*)"WGL_", 4))
    {
    }
    ret = (len == 0);
  }
  return ret;
}

#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)

#if defined(GLEW_MX)
GLboolean glxewContextIsSupported (const GLXEWContext* ctx, const char* name)
#else
GLboolean glxewIsSupported (const char* name)
#endif
{
  GLubyte* pos = (GLubyte*)name;
  GLuint len = _glewStrLen(pos);
  GLboolean ret = GL_TRUE;
  while (ret && len > 0)
  {
    if(_glewStrSame1(&pos, &len, (const GLubyte*)"GLX_", 4))
    {
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"VERSION_", 8))
      {
#ifdef GLX_VERSION_1_0
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_0", 3))
        {
          ret = GLXEW_VERSION_1_0;
          continue;
        }
#endif
#ifdef GLX_VERSION_1_1
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_1", 3))
        {
          ret = GLXEW_VERSION_1_1;
          continue;
        }
#endif
#ifdef GLX_VERSION_1_2
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_2", 3))
        {
          ret = GLXEW_VERSION_1_2;
          continue;
        }
#endif
#ifdef GLX_VERSION_1_3
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_3", 3))
        {
          ret = GLXEW_VERSION_1_3;
          continue;
        }
#endif
#ifdef GLX_VERSION_1_4
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"1_4", 3))
        {
          ret = GLXEW_VERSION_1_4;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"3DFX_", 5))
      {
#ifdef GLX_3DFX_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample", 11))
        {
          ret = GLXEW_3DFX_multisample;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"AMD_", 4))
      {
#ifdef GLX_AMD_gpu_association
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"gpu_association", 15))
        {
          ret = GLXEW_AMD_gpu_association;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"ARB_", 4))
      {
#ifdef GLX_ARB_create_context
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"create_context", 14))
        {
          ret = GLXEW_ARB_create_context;
          continue;
        }
#endif
#ifdef GLX_ARB_create_context_profile
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"create_context_profile", 22))
        {
          ret = GLXEW_ARB_create_context_profile;
          continue;
        }
#endif
#ifdef GLX_ARB_create_context_robustness
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"create_context_robustness", 25))
        {
          ret = GLXEW_ARB_create_context_robustness;
          continue;
        }
#endif
#ifdef GLX_ARB_fbconfig_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fbconfig_float", 14))
        {
          ret = GLXEW_ARB_fbconfig_float;
          continue;
        }
#endif
#ifdef GLX_ARB_framebuffer_sRGB
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_sRGB", 16))
        {
          ret = GLXEW_ARB_framebuffer_sRGB;
          continue;
        }
#endif
#ifdef GLX_ARB_get_proc_address
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"get_proc_address", 16))
        {
          ret = GLXEW_ARB_get_proc_address;
          continue;
        }
#endif
#ifdef GLX_ARB_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample", 11))
        {
          ret = GLXEW_ARB_multisample;
          continue;
        }
#endif
#ifdef GLX_ARB_vertex_buffer_object
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_buffer_object", 20))
        {
          ret = GLXEW_ARB_vertex_buffer_object;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"ATI_", 4))
      {
#ifdef GLX_ATI_pixel_format_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixel_format_float", 18))
        {
          ret = GLXEW_ATI_pixel_format_float;
          continue;
        }
#endif
#ifdef GLX_ATI_render_texture
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"render_texture", 14))
        {
          ret = GLXEW_ATI_render_texture;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"EXT_", 4))
      {
#ifdef GLX_EXT_create_context_es2_profile
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"create_context_es2_profile", 26))
        {
          ret = GLXEW_EXT_create_context_es2_profile;
          continue;
        }
#endif
#ifdef GLX_EXT_fbconfig_packed_float
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fbconfig_packed_float", 21))
        {
          ret = GLXEW_EXT_fbconfig_packed_float;
          continue;
        }
#endif
#ifdef GLX_EXT_framebuffer_sRGB
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"framebuffer_sRGB", 16))
        {
          ret = GLXEW_EXT_framebuffer_sRGB;
          continue;
        }
#endif
#ifdef GLX_EXT_import_context
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"import_context", 14))
        {
          ret = GLXEW_EXT_import_context;
          continue;
        }
#endif
#ifdef GLX_EXT_scene_marker
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"scene_marker", 12))
        {
          ret = GLXEW_EXT_scene_marker;
          continue;
        }
#endif
#ifdef GLX_EXT_swap_control
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_control", 12))
        {
          ret = GLXEW_EXT_swap_control;
          continue;
        }
#endif
#ifdef GLX_EXT_texture_from_pixmap
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"texture_from_pixmap", 19))
        {
          ret = GLXEW_EXT_texture_from_pixmap;
          continue;
        }
#endif
#ifdef GLX_EXT_visual_info
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"visual_info", 11))
        {
          ret = GLXEW_EXT_visual_info;
          continue;
        }
#endif
#ifdef GLX_EXT_visual_rating
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"visual_rating", 13))
        {
          ret = GLXEW_EXT_visual_rating;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"INTEL_", 6))
      {
#ifdef GLX_INTEL_swap_event
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_event", 10))
        {
          ret = GLXEW_INTEL_swap_event;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"MESA_", 5))
      {
#ifdef GLX_MESA_agp_offset
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"agp_offset", 10))
        {
          ret = GLXEW_MESA_agp_offset;
          continue;
        }
#endif
#ifdef GLX_MESA_copy_sub_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"copy_sub_buffer", 15))
        {
          ret = GLXEW_MESA_copy_sub_buffer;
          continue;
        }
#endif
#ifdef GLX_MESA_pixmap_colormap
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pixmap_colormap", 15))
        {
          ret = GLXEW_MESA_pixmap_colormap;
          continue;
        }
#endif
#ifdef GLX_MESA_release_buffers
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"release_buffers", 15))
        {
          ret = GLXEW_MESA_release_buffers;
          continue;
        }
#endif
#ifdef GLX_MESA_set_3dfx_mode
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"set_3dfx_mode", 13))
        {
          ret = GLXEW_MESA_set_3dfx_mode;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"NV_", 3))
      {
#ifdef GLX_NV_copy_image
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"copy_image", 10))
        {
          ret = GLXEW_NV_copy_image;
          continue;
        }
#endif
#ifdef GLX_NV_float_buffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"float_buffer", 12))
        {
          ret = GLXEW_NV_float_buffer;
          continue;
        }
#endif
#ifdef GLX_NV_multisample_coverage
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample_coverage", 20))
        {
          ret = GLXEW_NV_multisample_coverage;
          continue;
        }
#endif
#ifdef GLX_NV_present_video
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"present_video", 13))
        {
          ret = GLXEW_NV_present_video;
          continue;
        }
#endif
#ifdef GLX_NV_swap_group
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_group", 10))
        {
          ret = GLXEW_NV_swap_group;
          continue;
        }
#endif
#ifdef GLX_NV_vertex_array_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"vertex_array_range", 18))
        {
          ret = GLXEW_NV_vertex_array_range;
          continue;
        }
#endif
#ifdef GLX_NV_video_output
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"video_output", 12))
        {
          ret = GLXEW_NV_video_output;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"OML_", 4))
      {
#ifdef GLX_OML_swap_method
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_method", 11))
        {
          ret = GLXEW_OML_swap_method;
          continue;
        }
#endif
#if defined(GLX_OML_sync_control) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <inttypes.h>
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"sync_control", 12))
        {
          ret = GLXEW_OML_sync_control;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SGIS_", 5))
      {
#ifdef GLX_SGIS_blended_overlay
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"blended_overlay", 15))
        {
          ret = GLXEW_SGIS_blended_overlay;
          continue;
        }
#endif
#ifdef GLX_SGIS_color_range
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"color_range", 11))
        {
          ret = GLXEW_SGIS_color_range;
          continue;
        }
#endif
#ifdef GLX_SGIS_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"multisample", 11))
        {
          ret = GLXEW_SGIS_multisample;
          continue;
        }
#endif
#ifdef GLX_SGIS_shared_multisample
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"shared_multisample", 18))
        {
          ret = GLXEW_SGIS_shared_multisample;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SGIX_", 5))
      {
#ifdef GLX_SGIX_fbconfig
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"fbconfig", 8))
        {
          ret = GLXEW_SGIX_fbconfig;
          continue;
        }
#endif
#ifdef GLX_SGIX_hyperpipe
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"hyperpipe", 9))
        {
          ret = GLXEW_SGIX_hyperpipe;
          continue;
        }
#endif
#ifdef GLX_SGIX_pbuffer
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"pbuffer", 7))
        {
          ret = GLXEW_SGIX_pbuffer;
          continue;
        }
#endif
#ifdef GLX_SGIX_swap_barrier
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_barrier", 12))
        {
          ret = GLXEW_SGIX_swap_barrier;
          continue;
        }
#endif
#ifdef GLX_SGIX_swap_group
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_group", 10))
        {
          ret = GLXEW_SGIX_swap_group;
          continue;
        }
#endif
#ifdef GLX_SGIX_video_resize
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"video_resize", 12))
        {
          ret = GLXEW_SGIX_video_resize;
          continue;
        }
#endif
#ifdef GLX_SGIX_visual_select_group
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"visual_select_group", 19))
        {
          ret = GLXEW_SGIX_visual_select_group;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SGI_", 4))
      {
#ifdef GLX_SGI_cushion
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"cushion", 7))
        {
          ret = GLXEW_SGI_cushion;
          continue;
        }
#endif
#ifdef GLX_SGI_make_current_read
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"make_current_read", 17))
        {
          ret = GLXEW_SGI_make_current_read;
          continue;
        }
#endif
#ifdef GLX_SGI_swap_control
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"swap_control", 12))
        {
          ret = GLXEW_SGI_swap_control;
          continue;
        }
#endif
#ifdef GLX_SGI_video_sync
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"video_sync", 10))
        {
          ret = GLXEW_SGI_video_sync;
          continue;
        }
#endif
      }
      if (_glewStrSame2(&pos, &len, (const GLubyte*)"SUN_", 4))
      {
#ifdef GLX_SUN_get_transparent_index
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"get_transparent_index", 21))
        {
          ret = GLXEW_SUN_get_transparent_index;
          continue;
        }
#endif
#ifdef GLX_SUN_video_resize
        if (_glewStrSame3(&pos, &len, (const GLubyte*)"video_resize", 12))
        {
          ret = GLXEW_SUN_video_resize;
          continue;
        }
#endif
      }
    }
    ret = (len == 0);
  }
  return ret;
}

#endif /* _WIN32 */
