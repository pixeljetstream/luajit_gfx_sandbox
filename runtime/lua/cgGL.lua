local ffi = require "ffi"

ffi.cdef [[
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef void GLvoid;
typedef int GLsizei;
typedef int GLint;

typedef enum
{
  CG_GL_MATRIX_IDENTITY             = 0,
  CG_GL_MATRIX_TRANSPOSE            = 1,
  CG_GL_MATRIX_INVERSE              = 2,
  CG_GL_MATRIX_INVERSE_TRANSPOSE    = 3,
  CG_GL_MODELVIEW_MATRIX            = 4,
  CG_GL_PROJECTION_MATRIX           = 5,
  CG_GL_TEXTURE_MATRIX              = 6,
  CG_GL_MODELVIEW_PROJECTION_MATRIX = 7,
  CG_GL_VERTEX                      = 8,
  CG_GL_FRAGMENT                    = 9,
  CG_GL_GEOMETRY                    = 10,
  CG_GL_TESSELLATION_CONTROL        = 11,
  CG_GL_TESSELLATION_EVALUATION     = 12
} CGGLenum;

 CGbool  cgGLIsProfileSupported(CGprofile profile);
 void  cgGLEnableProfile(CGprofile profile);
 void  cgGLDisableProfile(CGprofile profile);
 CGprofile  cgGLGetLatestProfile(CGGLenum profile_type);
 void  cgGLSetOptimalOptions(CGprofile profile);
 char const **  cgGLGetOptimalOptions(CGprofile profile);
 void  cgGLLoadProgram(CGprogram program);
 void  cgGLUnloadProgram(CGprogram program);
 CGbool  cgGLIsProgramLoaded(CGprogram program);
 void  cgGLBindProgram(CGprogram program);
 void  cgGLUnbindProgram(CGprofile profile);
 GLuint  cgGLGetProgramID(CGprogram program);
 void  cgGLSetParameter1f(CGparameter param, float x);
 void  cgGLSetParameter2f(CGparameter param, float x, float y);
 void  cgGLSetParameter3f(CGparameter param, float x, float y, float z);
 void  cgGLSetParameter4f(CGparameter param, float x, float y, float z, float w);
 void  cgGLSetParameter1fv(CGparameter param, const float *v);
 void  cgGLSetParameter2fv(CGparameter param, const float *v);
 void  cgGLSetParameter3fv(CGparameter param, const float *v);
 void  cgGLSetParameter4fv(CGparameter param, const float *v);
 void  cgGLSetParameter1d(CGparameter param, double x);
 void  cgGLSetParameter2d(CGparameter param, double x, double y);
 void  cgGLSetParameter3d(CGparameter param, double x, double y, double z);
 void  cgGLSetParameter4d(CGparameter param, double x, double y, double z, double w);
 void  cgGLSetParameter1dv(CGparameter param, const double *v);
 void  cgGLSetParameter2dv(CGparameter param, const double *v);
 void  cgGLSetParameter3dv(CGparameter param, const double *v);
 void  cgGLSetParameter4dv(CGparameter param, const double *v);
 void  cgGLGetParameter1f(CGparameter param, float *v);
 void  cgGLGetParameter2f(CGparameter param, float *v);
 void  cgGLGetParameter3f(CGparameter param, float *v);
 void  cgGLGetParameter4f(CGparameter param, float *v);
 void  cgGLGetParameter1d(CGparameter param, double *v);
 void  cgGLGetParameter2d(CGparameter param, double *v);
 void  cgGLGetParameter3d(CGparameter param, double *v);
 void  cgGLGetParameter4d(CGparameter param, double *v);
 void  cgGLSetParameterArray1f(CGparameter param, long offset, long nelements, const float *v);
 void  cgGLSetParameterArray2f(CGparameter param, long offset, long nelements, const float *v);
 void  cgGLSetParameterArray3f(CGparameter param, long offset, long nelements, const float *v);
 void  cgGLSetParameterArray4f(CGparameter param, long offset, long nelements, const float *v);
 void  cgGLSetParameterArray1d(CGparameter param, long offset, long nelements, const double *v);
 void  cgGLSetParameterArray2d(CGparameter param, long offset, long nelements, const double *v);
 void  cgGLSetParameterArray3d(CGparameter param, long offset, long nelements, const double *v);
 void  cgGLSetParameterArray4d(CGparameter param, long offset, long nelements, const double *v);
 void  cgGLGetParameterArray1f(CGparameter param, long offset, long nelements, float *v);
 void  cgGLGetParameterArray2f(CGparameter param, long offset, long nelements, float *v);
 void  cgGLGetParameterArray3f(CGparameter param, long offset, long nelements, float *v);
 void  cgGLGetParameterArray4f(CGparameter param, long offset, long nelements, float *v);
 void  cgGLGetParameterArray1d(CGparameter param, long offset, long nelements, double *v);
 void  cgGLGetParameterArray2d(CGparameter param, long offset, long nelements, double *v);
 void  cgGLGetParameterArray3d(CGparameter param, long offset, long nelements, double *v);
 void  cgGLGetParameterArray4d(CGparameter param, long offset, long nelements, double *v);
 void  cgGLSetParameterPointer(CGparameter param, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer);
 void  cgGLEnableClientState(CGparameter param);
 void  cgGLDisableClientState(CGparameter param);
 void  cgGLSetMatrixParameterdr(CGparameter param, const double *matrix);
 void  cgGLSetMatrixParameterfr(CGparameter param, const float *matrix);
 void  cgGLSetMatrixParameterdc(CGparameter param, const double *matrix);
 void  cgGLSetMatrixParameterfc(CGparameter param, const float *matrix);
 void  cgGLGetMatrixParameterdr(CGparameter param, double *matrix);
 void  cgGLGetMatrixParameterfr(CGparameter param, float *matrix);
 void  cgGLGetMatrixParameterdc(CGparameter param, double *matrix);
 void  cgGLGetMatrixParameterfc(CGparameter param, float *matrix);
 void  cgGLSetStateMatrixParameter(CGparameter param, CGGLenum matrix, CGGLenum transform);
 void  cgGLSetMatrixParameterArrayfc(CGparameter param, long offset, long nelements, const float *matrices);
 void  cgGLSetMatrixParameterArrayfr(CGparameter param, long offset, long nelements, const float *matrices);
 void  cgGLSetMatrixParameterArraydc(CGparameter param, long offset, long nelements, const double *matrices);
 void  cgGLSetMatrixParameterArraydr(CGparameter param, long offset, long nelements, const double *matrices);
 void  cgGLGetMatrixParameterArrayfc(CGparameter param, long offset, long nelements, float *matrices);
 void  cgGLGetMatrixParameterArrayfr(CGparameter param, long offset, long nelements, float *matrices);
 void  cgGLGetMatrixParameterArraydc(CGparameter param, long offset, long nelements, double *matrices);
 void  cgGLGetMatrixParameterArraydr(CGparameter param, long offset, long nelements, double *matrices);
 void  cgGLSetTextureParameter(CGparameter param, GLuint texobj);
 GLuint  cgGLGetTextureParameter(CGparameter param);
 void  cgGLEnableTextureParameter(CGparameter param);
 void  cgGLDisableTextureParameter(CGparameter param);
 GLenum  cgGLGetTextureEnum(CGparameter param);
 void  cgGLSetManageTextureParameters(CGcontext ctx, CGbool flag);
 CGbool  cgGLGetManageTextureParameters(CGcontext ctx);
 void  cgGLSetupSampler(CGparameter param, GLuint texobj);
 void  cgGLRegisterStates(CGcontext ctx);
 void  cgGLEnableProgramProfiles(CGprogram program);
 void  cgGLDisableProgramProfiles(CGprogram program);
 void  cgGLSetDebugMode(CGbool debug);
 CGbuffer  cgGLCreateBuffer(CGcontext context, int size, const void *data, GLenum bufferUsage);
 GLuint  cgGLGetBufferObject(CGbuffer buffer);

]]

return ffi.load( "cgGL" )
