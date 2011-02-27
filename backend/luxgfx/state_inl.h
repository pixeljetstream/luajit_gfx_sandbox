// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/luxgfx.h>
#include <luxcore/scalarmisc.h>

extern lxGLCompareMode_t g_VGLcmpModes[LUXGL_COMPARES+1];

LUX_INLINE lxGLCompareMode_t VGLCompareMode_get(lxGFXCompareMode_t mode)
{
  return g_VGLcmpModes[mode];
}

static LUX_INLINE GLenum  lxScalarType_to(lxScalarType_t data)
{
  GLenum standard[SCALARS] = {
    GL_FLOAT,
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,

    GL_HALF_FLOAT,  
    GL_DOUBLE,
    0,
  };

  return standard[data];
}