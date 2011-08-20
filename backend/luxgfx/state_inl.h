// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxgfx/luxgfx.h>
#include <luxinia/luxcore/scalarmisc.h>

static LUX_INLINE GLenum  lxScalarType_to(lxScalarType_t data)
{
  GLenum standard[LUX_SCALARS] = {
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
