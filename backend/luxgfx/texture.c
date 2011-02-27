// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/texture.h>
#include <luxgfx/luxgfx.h>
#include <luxplatform/debug.h>
#include <luxmath/misc.h>
#include <luxcore/scalarmisc.h>

#include "state_inl.h"

#define VGLTARGET(tex)  ((tex)->vgl.target)

//////////////////////////////////////////////////////////////////////////
// VGLTextureTarget_t

lxGLTextureTarget_t VGLTextureTarget_get(lxGFXTextureType_t type)
{
  switch (type)
  {
  case LUXGFX_TEXTURE_1D:
    return LUXGL_TEXTURE_1D;
  case LUXGFX_TEXTURE_2D:
    return LUXGL_TEXTURE_2D;
  case LUXGFX_TEXTURE_3D:
    return LUXGL_TEXTURE_3D;
  case LUXGFX_TEXTURE_RECT:
    return LUXGL_TEXTURE_RECT;
  case LUXGFX_TEXTURE_CUBE:
    return LUXGL_TEXTURE_CUBE;
  case LUXGFX_TEXTURE_1DARRAY:
    return LUXGL_TEXTURE_1DARRAY;
  case LUXGFX_TEXTURE_2DARRAY:
    return LUXGL_TEXTURE_2DARRAY;
  case LUXGFX_TEXTURE_CUBEARRAY:
    return LUXGL_TEXTURE_CUBEARRAY;
  case LUXGFX_TEXTURE_2DMS:
    return LUXGL_TEXTURE_2DMS;
  case LUXGFX_TEXTURE_2DMSARRAY:
    return LUXGL_TEXTURE_2DMSARRAY;
  case LUXGFX_TEXTURE_BUFFER:
    return LUXGL_TEXTURE_BUFFER;
  }

  LUX_DEBUGASSERT(0);
  return LUXGL_TEXTURE_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// VGLTextureData_t

static GLenum  GetBase(lxGFXTextureChannel_t type)
{
  switch(type){
  case LUXGFX_TEXTURECHANNEL_RGB:
    return GL_RGB;

  case LUXGFX_TEXTURECHANNEL_RGBA:
    return GL_RGBA;


  case LUXGFX_TEXTURECHANNEL_R:
    return GL_R;

  case LUXGFX_TEXTURECHANNEL_RG:
    return GL_RG;

  case LUXGFX_TEXTURECHANNEL_SRGB:
    return GL_SRGB;

  case LUXGFX_TEXTURECHANNEL_SRGBA:
    return GL_SRGB_ALPHA;

  case LUXGFX_TEXTURECHANNEL_DEPTH:
    return GL_DEPTH_COMPONENT;

  case LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL:
    return GL_DEPTH_STENCIL;

  case LUXGFX_TEXTURECHANNEL_ALPHA:
    return GL_ALPHA;

  case LUXGFX_TEXTURECHANNEL_INTENSITY:
    return GL_INTENSITY;

  case LUXGFX_TEXTURECHANNEL_LUMINANCE:
    return GL_LUMINANCE;

  case LUXGFX_TEXTURECHANNEL_LUMINANCEALPHA:
    return GL_LUMINANCE_ALPHA;

  default:
    return 0;
  }
}

static uint  GetComponents(lxGFXTextureChannel_t type)
{
  switch(type){
  case LUXGFX_TEXTURECHANNEL_RGB:
    return 3;

  case LUXGFX_TEXTURECHANNEL_RGBA:
    return 4;


  case LUXGFX_TEXTURECHANNEL_R:
    return 1;

  case LUXGFX_TEXTURECHANNEL_RG:
    return 2;

  case LUXGFX_TEXTURECHANNEL_SRGB:
    return 3;

  case LUXGFX_TEXTURECHANNEL_SRGBA:
    return 4;

  case LUXGFX_TEXTURECHANNEL_DEPTH:
    return 1;

  case LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL:
    return 2;

  case LUXGFX_TEXTURECHANNEL_ALPHA:
    return 1;

  case LUXGFX_TEXTURECHANNEL_INTENSITY:
    return 1;

  case LUXGFX_TEXTURECHANNEL_LUMINANCE:
    return 1;

  case LUXGFX_TEXTURECHANNEL_LUMINANCEALPHA:
    return 2;

  default:
    return 0;
  }
}
static GLenum  GetDataTypeSize(lxGFXTextureDataType_t data, booln depthstencil)
{
  static GLenum standard[LUXGFX_TEXTUREDATAS] = {
    1,
    1,
    2,
    1,
    2,

    2,
    4,

    1,
    1,
    2,
    2,
    4,
    4,
  };

  if (data < LUXGFX_TEXTUREDATAS){
    return standard[data];
  }

  switch (data)
  {
  case LUXGFX_TEXTUREDATA_DEPTH16:
    return 2;
  case LUXGFX_TEXTUREDATA_DEPTH24:
    return 4;
  case LUXGFX_TEXTUREDATA_DEPTH32:
    return 4;
  case LUXGFX_TEXTUREDATA_DEPTH32F:
    return depthstencil ? 8 : 4;

  case LUXGFX_TEXTUREDATA_UNORM1010102:
  case LUXGFX_TEXTUREDATA_UINT1010102:
    return 4;

  case LUXGFX_TEXTUREDATA_FLOAT111110:
    return 4;
  case LUXGFX_TEXTUREDATA_EXP999:
    return 4;
  }

  return 0;
}


static GLenum  GetDataType(lxGFXTextureDataType_t data, booln rev, booln depthstencil)
{
  static GLenum standard[LUXGFX_TEXTUREDATAS] = {
    GL_UNSIGNED_BYTE,

    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    GL_BYTE,
    GL_SHORT,

    GL_HALF_FLOAT,
    GL_FLOAT,

    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
  };

  if (data < LUXGFX_TEXTUREDATAS){
    return standard[data];
  }

  switch (data)
  {
    case LUXGFX_TEXTUREDATA_DEPTH16:
      return GL_UNSIGNED_SHORT;
    case LUXGFX_TEXTUREDATA_DEPTH24:
      return depthstencil ? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_INT;
    case LUXGFX_TEXTUREDATA_DEPTH32:
      return GL_UNSIGNED_INT;
    case LUXGFX_TEXTUREDATA_DEPTH32F:
      return depthstencil ? GL_FLOAT_32_UNSIGNED_INT_24_8_REV : GL_FLOAT;

    case LUXGFX_TEXTUREDATA_UNORM1010102:
    case LUXGFX_TEXTUREDATA_UINT1010102:
      return rev ? GL_UNSIGNED_INT_2_10_10_10_REV: GL_UNSIGNED_INT_10_10_10_2;
    
    case LUXGFX_TEXTUREDATA_FLOAT111110:
      return GL_UNSIGNED_INT_10F_11F_11F_REV;
    case LUXGFX_TEXTUREDATA_EXP999:
      return GL_UNSIGNED_INT_5_9_9_9_REV;
    case LUXGFX_TEXTUREDATA_COMPRESSED:
    case LUXGFX_TEXTUREDATA_COMPRESSED_DXT1:
    case LUXGFX_TEXTUREDATA_COMPRESSED_DXT3:
    case LUXGFX_TEXTUREDATA_COMPRESSED_DXT5:
    case LUXGFX_TEXTUREDATA_COMPRESSED_TC:
      return GL_UNSIGNED_BYTE;
    case LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC:
      return GL_BYTE;
  }

  return 0;
}

static GLenum  GetDataFormat(lxGFXTextureChannel_t type, booln rev, booln integer)
{
  switch(type){
  case LUXGFX_TEXTURECHANNEL_RGB:
    if (integer) return rev ? GL_BGR : GL_RGB;
    else return rev ? GL_BGR_INTEGER : GL_RGB_INTEGER;

  case LUXGFX_TEXTURECHANNEL_RGBA:
    if (integer) return rev ? GL_BGRA : GL_RGBA;
    else return rev ? GL_BGRA_INTEGER : GL_RGBA_INTEGER;

  case LUXGFX_TEXTURECHANNEL_R:
    return integer ? GL_RED_INTEGER : GL_RED;

  case LUXGFX_TEXTURECHANNEL_RG:
    return integer ? GL_RED_INTEGER : GL_RED;

  case LUXGFX_TEXTURECHANNEL_SRGB:
    return GL_RGB;

  case LUXGFX_TEXTURECHANNEL_SRGBA:
    return GL_RGBA;

  case LUXGFX_TEXTURECHANNEL_DEPTH:
    return GL_DEPTH_COMPONENT;

  case LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL:
    return GL_DEPTH_STENCIL;

  case LUXGFX_TEXTURECHANNEL_ALPHA:
    return integer ? GL_ALPHA_INTEGER_EXT : GL_ALPHA;

  case LUXGFX_TEXTURECHANNEL_INTENSITY:
    return integer ? GL_LUMINANCE_INTEGER_EXT : GL_LUMINANCE;

  case LUXGFX_TEXTURECHANNEL_LUMINANCE:
    return integer ? GL_LUMINANCE_INTEGER_EXT : GL_LUMINANCE;

  case LUXGFX_TEXTURECHANNEL_LUMINANCEALPHA:
    return integer ? GL_LUMINANCE_ALPHA_INTEGER_EXT : GL_LUMINANCE_ALPHA;

  default:
    return 0;
  }
}

static GLenum  GetCompressed(lxGFXTextureChannel_t type)
{
  switch(type){
  case LUXGFX_TEXTURECHANNEL_RGB:
    return GL_COMPRESSED_RGB;

  case LUXGFX_TEXTURECHANNEL_RGBA:
    return GL_COMPRESSED_RGBA;

  case LUXGFX_TEXTURECHANNEL_R:
    return GL_COMPRESSED_RED;

  case LUXGFX_TEXTURECHANNEL_RG:
    return GL_COMPRESSED_RG;

  case LUXGFX_TEXTURECHANNEL_SRGB:
    return GL_COMPRESSED_SRGB;

  case LUXGFX_TEXTURECHANNEL_SRGBA:
    return GL_COMPRESSED_SRGB_ALPHA;

  case LUXGFX_TEXTURECHANNEL_ALPHA:
    return GL_COMPRESSED_ALPHA;

  case LUXGFX_TEXTURECHANNEL_INTENSITY:
    return GL_COMPRESSED_INTENSITY;

  case LUXGFX_TEXTURECHANNEL_LUMINANCE:
    return GL_COMPRESSED_LUMINANCE;

  case LUXGFX_TEXTURECHANNEL_LUMINANCEALPHA:
    return GL_COMPRESSED_LUMINANCE_ALPHA;
  default:
    return 0;
  }
}

static GLenum  GetInternal(lxGFXTextureChannel_t type, lxGFXTextureDataType_t data)
{
  static GLenum rgb[LUXGFX_TEXTUREDATAS] = {
    GL_RGB,
    GL_RGB8, GL_RGB16, GL_RGB8_SNORM, GL_RGB16_SNORM, GL_RGB_FLOAT16_ATI, GL_RGB_FLOAT32_ATI,
    GL_RGB8I_EXT, GL_RGB8UI_EXT, GL_RGB16I_EXT, GL_RGB16UI_EXT,
    GL_RGB32I_EXT, GL_RGB32UI_EXT, 
  };
  static GLenum rgba[LUXGFX_TEXTUREDATAS] = {
    GL_RGBA,
    GL_RGBA8, GL_RGBA16, GL_RGBA8_SNORM, GL_RGBA16_SNORM, GL_RGBA_FLOAT16_ATI, GL_RGBA_FLOAT32_ATI,
    GL_RGBA8I_EXT, GL_RGBA8UI_EXT, GL_RGBA16I_EXT, GL_RGBA16UI_EXT,
    GL_RGBA32I_EXT, GL_RGBA32UI_EXT, 
  };
  static GLenum alpha[LUXGFX_TEXTUREDATAS] = {
    GL_ALPHA,
    GL_ALPHA8, GL_ALPHA16, GL_ALPHA_SNORM, GL_ALPHA16_SNORM, GL_ALPHA_FLOAT16_ATI, GL_ALPHA_FLOAT32_ATI,
    GL_ALPHA8I_EXT, GL_ALPHA8UI_EXT, GL_ALPHA16I_EXT, GL_ALPHA16UI_EXT,
    GL_ALPHA32I_EXT, GL_ALPHA32UI_EXT, 
  };
  static GLenum intensity[LUXGFX_TEXTUREDATAS] = {
    GL_INTENSITY,
    GL_INTENSITY8, GL_INTENSITY16, GL_INTENSITY_SNORM, GL_INTENSITY16_SNORM, GL_INTENSITY_FLOAT16_ATI, GL_INTENSITY_FLOAT32_ATI,
    GL_INTENSITY8I_EXT, GL_INTENSITY8UI_EXT, GL_INTENSITY16I_EXT, GL_INTENSITY16UI_EXT,
    GL_INTENSITY32I_EXT, GL_INTENSITY32UI_EXT, 
  };
  static GLenum lum[LUXGFX_TEXTUREDATAS] = {
    GL_LUMINANCE,
    GL_LUMINANCE8, GL_LUMINANCE16, GL_LUMINANCE_SNORM, GL_LUMINANCE16_SNORM, GL_LUMINANCE_FLOAT16_ATI, GL_LUMINANCE_FLOAT32_ATI,
    GL_LUMINANCE8I_EXT, GL_LUMINANCE8UI_EXT, GL_LUMINANCE16I_EXT, GL_LUMINANCE16UI_EXT,
    GL_LUMINANCE32I_EXT, GL_LUMINANCE32UI_EXT, 
  };
  static GLenum lumalpha[LUXGFX_TEXTUREDATAS] = {
    GL_LUMINANCE_ALPHA,
    GL_LUMINANCE8_ALPHA8, GL_LUMINANCE16_ALPHA16, GL_LUMINANCE_ALPHA_SNORM, GL_LUMINANCE16_ALPHA16_SNORM, GL_LUMINANCE_ALPHA_FLOAT16_ATI, GL_LUMINANCE_ALPHA_FLOAT32_ATI,
    GL_LUMINANCE_ALPHA8I_EXT, GL_LUMINANCE_ALPHA8UI_EXT, GL_LUMINANCE_ALPHA16I_EXT, GL_LUMINANCE_ALPHA16UI_EXT,
    GL_LUMINANCE_ALPHA32I_EXT, GL_LUMINANCE_ALPHA32UI_EXT,
  };
  static GLenum red[LUXGFX_TEXTUREDATAS] = {
    GL_R,
    GL_R8, GL_R16, GL_R8_SNORM,GL_R16_SNORM, GL_R16F, GL_R32F,
    GL_R8I, GL_R8UI, GL_R16I, GL_R16UI,
    GL_R32I, GL_R32UI,
  };
  static GLenum redgreen[LUXGFX_TEXTUREDATAS] = {
    GL_RG,
    GL_RG8, GL_RG16,GL_RG8_SNORM,GL_RG16_SNORM, GL_RG16F, GL_RG32F,
    GL_RG8I, GL_RG8UI, GL_RG16I, GL_RG16UI,
    GL_RG32I, GL_RG32UI,
  };

  if (data < LUXGFX_TEXTUREDATAS){
    switch(type){
    case LUXGFX_TEXTURECHANNEL_RGB:
      return rgb[data];

    case LUXGFX_TEXTURECHANNEL_RGBA:
      return rgba[data];

    case LUXGFX_TEXTURECHANNEL_R:
      return red[data];

    case LUXGFX_TEXTURECHANNEL_RG:
      return redgreen[data];

    case LUXGFX_TEXTURECHANNEL_ALPHA:
      return alpha[data];

    case LUXGFX_TEXTURECHANNEL_INTENSITY:
      return intensity[data];

    case LUXGFX_TEXTURECHANNEL_LUMINANCE:
      return intensity[data];

    case LUXGFX_TEXTURECHANNEL_LUMINANCEALPHA:
      return lumalpha[data];
    default:
      break;
    }
  }
  switch(type){
    case LUXGFX_TEXTURECHANNEL_RGB:
      switch(data){
      case LUXGFX_TEXTUREDATA_EXP999:
        return GL_RGB9_E5;
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT1:
        return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_FLOAT_BPTC:
        return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB;
      case LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_FLOAT_BPTC:
        return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_RGBA:
      switch(data){
      case LUXGFX_TEXTUREDATA_UNORM1010102:
        return GL_RGB10_A2;
      case LUXGFX_TEXTUREDATA_UINT1010102:
        return GL_RGB10_A2UI;
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT1:
        return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT3:
        return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT5:
        return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_UNORM_BPTC:
        return GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_R:
      switch(data){
      case LUXGFX_TEXTUREDATA_COMPRESSED_TC:
        return GL_COMPRESSED_RED_RGTC1;
      case LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC:
        return GL_COMPRESSED_SIGNED_RED_RGTC1;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_RG:
      switch(data){
      case LUXGFX_TEXTUREDATA_COMPRESSED_TC:
        return GL_COMPRESSED_RG_RGTC2;
      case LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC:
        return GL_COMPRESSED_SIGNED_RG_RGTC2;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_SRGB:
      switch(data){
      case LUXGFX_TEXTUREDATA_BASE:
        return GL_SRGB;
      case LUXGFX_TEXTUREDATA_UNORM8:
        return GL_SRGB8;
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT1:
        return GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_SRGBA:
      switch(data){
      case LUXGFX_TEXTUREDATA_BASE:
        return GL_SRGB_ALPHA;
      case LUXGFX_TEXTUREDATA_UNORM8:
        return GL_SRGB8_ALPHA8; 
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT1:
        return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT3:
        return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_DXT5:
        return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_UNORM_BPTC:
        return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_DEPTH:
      switch(data){
      case LUXGFX_TEXTUREDATA_BASE:
        return GL_DEPTH_COMPONENT;
      case LUXGFX_TEXTUREDATA_DEPTH16:
        return GL_DEPTH_COMPONENT16;
      case LUXGFX_TEXTUREDATA_DEPTH24:
        return GL_DEPTH_COMPONENT24;
      case LUXGFX_TEXTUREDATA_DEPTH32:
        return GL_DEPTH_COMPONENT32;
      case LUXGFX_TEXTUREDATA_DEPTH32F:
        return GL_DEPTH_COMPONENT32F;
      }
      return 0;

    case LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL:
      switch(data){
      case LUXGFX_TEXTUREDATA_BASE:
        return GL_DEPTH_STENCIL;
      case LUXGFX_TEXTUREDATA_DEPTH24:
        return GL_DEPTH24_STENCIL8;
      case LUXGFX_TEXTUREDATA_DEPTH32F:
        return GL_DEPTH32F_STENCIL8;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_LUMINANCE:
      switch(data){
      case LUXGFX_TEXTUREDATA_COMPRESSED_TC:
        return GL_COMPRESSED_LUMINANCE_LATC1_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC:
        return GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT;
      }
      return 0;
    case LUXGFX_TEXTURECHANNEL_LUMINANCEALPHA:
      switch(data){
      case LUXGFX_TEXTUREDATA_COMPRESSED_TC:
        return GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
      case LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC:
        return GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT;
      }
      return 0;
    default:
      return 0;
  }

  return 0;
}

static uint GetCompressedSize(GLenum internalformat, uint size)
{
  switch (internalformat)
  {
  case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    return size/2;
  case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
  case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
    return size;
  case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    return size/2;
  case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
  case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
  case GL_COMPRESSED_RGBA_BPTC_UNORM_ARB:
    return size;
  case GL_COMPRESSED_RED_RGTC1:
  case GL_COMPRESSED_SIGNED_RED_RGTC1:
    return size/2;
  case GL_COMPRESSED_RG_RGTC2:
  case GL_COMPRESSED_SIGNED_RG_RGTC2:
    return size;
  case GL_COMPRESSED_SRGB_S3TC_DXT1_EXT:
  case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT:
    return size/2;
  case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT:
  case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT:
    return size;
  case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
    return size;
  case GL_COMPRESSED_LUMINANCE_LATC1_EXT:
  case GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT:
    return size/2;
  case GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT:
  case GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT:
    return size;
  }

  return 0;
}

static booln VGLTextureData_init(lxGFXContextPTR ctx, lxGLTextureData_t* vgldata,
  lxGFXTextureChannel_t format, lxGFXTextureDataType_t data)
{
  if (data == LUXGFX_TEXTUREDATA_CUSTOM){
    return LUX_TRUE;
  }
  if ((data == LUXGFX_TEXTUREDATA_DEPTH32F && !(ctx->capbits & LUXGFX_CAP_DEPTHFLOAT ))||
    (data >= LUXGFX_TEXTUREDATA_FLOAT16 && data <= LUXGFX_TEXTUREDATA_FLOAT32 && !(ctx->capbits & LUXGFX_CAP_SM2EXT ))||
    (data >= LUXGFX_TEXTUREDATA_SINT8 && data <= LUXGFX_TEXTUREDATA_UINT32 && !(ctx->capbits & LUXGFX_CAP_SM4 ))||
    (data >= LUXGFX_TEXTUREDATA_UNORM1010102 && data <= LUXGFX_TEXTUREDATA_EXP999 && !(ctx->capbits & LUXGFX_CAP_SM4 ))||
    (data >= LUXGFX_TEXTUREDATA_COMPRESSED_DXT1 && data <= LUXGFX_TEXTUREDATA_COMPRESSED_DXT5 && !GLEW_EXT_texture_compression_s3tc)||
    (data >= LUXGFX_TEXTUREDATA_COMPRESSED_UNORM_BPTC && data <= LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_FLOAT_BPTC && !GLEW_ARB_texture_compression_bptc)||
    (data >= LUXGFX_TEXTUREDATA_COMPRESSED_TC && data <= LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC && !GLEW_EXT_texture_compression_rgtc)
      ){
    return LUX_FALSE;
  }
  vgldata->dataformat = GetDataFormat(format,LUX_FALSE,data > LUXGFX_TEXTUREDATA_FLOAT32);
  vgldata->datatype = GetDataType(data,LUX_FALSE,format == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL);
  vgldata->internalformat = GetInternal(format,data);
  if (!vgldata->dataformat || !vgldata->datatype || !vgldata->internalformat)
    return LUX_FALSE;

  return LUX_TRUE;
}

//////////////////////////////////////////////////////////////////////////
// lxGFXTexture

LUX_API void lxGFXTexture_apply( lxGFXContextPTR ctx, uint imageunit, lxGFXTexturePTR tex)
{
  ctx->textures[imageunit] = tex;
  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  glBindTexture(tex->vgl.target,tex->vgl.id);
}

LUX_API void  lxGFXTextures_apply(lxGFXContextPTR ctx, uint start, uint num, lxGFXTexturePTR *texs)
{
  int i;
  for (i = num-1; i >= 0; i--){
    ctx->textures[i+start] = texs[i];
    glActiveTexture(GL_TEXTURE0_ARB + i);
    glBindTexture(texs[i]->vgl.target,texs[i]->vgl.id);
  }
}

LUX_API booln lxGFXTextureType_valid(lxGFXContextPTR ctx, lxGFXTextureType_t type)
{
  if (((type == LUXGFX_TEXTURE_1DARRAY ||type == LUXGFX_TEXTURE_2DARRAY ) && !(ctx->capbits & LUXGFX_CAP_SM4))||
    (type == LUXGFX_TEXTURE_CUBEARRAY && !(ctx->capbits & LUXGFX_CAP_TEXCUBEARRAY))||
    (type == LUXGFX_TEXTURE_3D && !(ctx->capbits & LUXGFX_CAP_TEX3D))||
    (type == LUXGFX_TEXTURE_RECT && !(ctx->capbits & LUXGFX_CAP_TEXRECT))||
    ((type == LUXGFX_TEXTURE_2DMS || type == LUXGFX_TEXTURE_2DMSARRAY ) && !(ctx->capbits & LUXGFX_CAP_SM4))||
    (type == LUXGFX_TEXTURE_BUFFER && !(ctx->capbits & LUXGFX_CAP_SM4)))
  {
    return LUX_FALSE;
  }
  return LUX_TRUE;
}

LUX_API booln lxGFXTextureChannel_valid(lxGFXContextPTR ctx, lxGFXTextureChannel_t channel)
{
  if ((channel == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL  && !(ctx->capbits & LUXGFX_CAP_FBO))||
    ((channel == LUXGFX_TEXTURECHANNEL_SRGB || channel == LUXGFX_TEXTURECHANNEL_SRGBA ) && !(ctx->capbits & LUXGFX_CAP_SM4)))
  {
    return LUX_FALSE;
  }
  return LUX_TRUE;
}

typedef struct TexUpload_s{
  enum32 target;
  int level;
  enum32 internal;
  int w;
  int h;
  int d;
  int border;
  enum32 format;
  enum32 type;
  int size;
}TexUpload_t;
typedef void (TexUpload_fn) (TexUpload_t* tu);

typedef enum TexUploadFuncType_e
{
  TEXUPLOAD_1D,
  TEXUPLOAD_1D_COMPRESSED,

  TEXUPLOAD_2D,
  TEXUPLOAD_2D_COMPRESSED,

  TEXUPLOAD_3D,
  TEXUPLOAD_3D_COMPRESSED,

  TEXUPLOAD_NONE,
}TexUploadFuncType_t;

static void TexUpload_1D(TexUpload_t*  tu)
{
  glTexImage1D(tu->target,tu->level,tu->internal,tu->w,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_1DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage1DARB(tu->target,tu->level,tu->format,tu->w,tu->border,tu->size,NULL);
}
static void TexUpload_2D(TexUpload_t*  tu)
{
  glTexImage2D(tu->target,tu->level,tu->internal,tu->w,tu->h,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_2DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage2DARB(tu->target,tu->level,tu->format,tu->w,tu->h,tu->border,tu->size,NULL);
}
static void TexUpload_3D(TexUpload_t*  tu)
{
  glTexImage3D(tu->target,tu->level,tu->internal,tu->w,tu->h,tu->d,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_3DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage3DARB(tu->target,tu->level,tu->format,tu->w,tu->h,tu->d,tu->border,tu->size,NULL);
}

static TexUpload_fn* l_uploadfuncs[] =
{
  TexUpload_1D,
  TexUpload_1DCompressed,
  TexUpload_2D,
  TexUpload_2DCompressed,
  TexUpload_3D,
  TexUpload_3DCompressed,
};

static uint GetMipSizes(uint* mippixels, lxVec3iPTR mipsizes, int w, int h, int d, flags32 nochange)
{
  uint lastsize = 0;
  uint mips = 0;

  do {
    lastsize = w*h*d;
    mippixels[mips] = lastsize;
    mipsizes[mips].x = w;
    mipsizes[mips].y = h;
    mipsizes[mips].z = d;

    w /= 2;
    h /= (nochange&1) ? 1 : 2;
    d /= (nochange&2) ? 1 : 2;
    w = LUX_MAX(1,w);
    h = LUX_MAX(1,h);
    d = LUX_MAX(1,d);

    mips++;
  } while(w*h*d != lastsize);

  return mips;
}

static uint TexUpload_run(TexUpload_t* base, booln mipmaps, flags32 nochange, TexUploadFuncType_t uploadfunctype)
{
  uint n = 0;
  if (uploadfunctype == TEXUPLOAD_NONE) return n;
  if (!mipmaps){
    base->size = GetCompressedSize(base->internal,base->w*base->h*base->d);
    l_uploadfuncs[uploadfunctype](base);
  }
  else {
    TexUpload_t up = *base;
    int mipwidth = base->w;
    int mipheight = base->h;
    int mipdepth = base->d;
    int lastsize = 0;

    do {
      lastsize = mipwidth*mipheight*mipdepth;
      up.level = n;
      up.w = mipwidth;
      up.h = mipheight;
      up.d = mipdepth;
      up.size = GetCompressedSize(up.internal,lastsize);
      l_uploadfuncs[uploadfunctype](&up);
      mipwidth /= 2;
      mipheight/= (nochange&1) ? 1 : 2;
      mipdepth /= (nochange&2) ? 1 : 2;
      mipwidth = LUX_MAX(1,mipwidth);
      mipheight = LUX_MAX(1,mipheight);
      mipdepth = LUX_MAX(1,mipdepth);
    } while(mipwidth*mipheight*mipdepth != lastsize);
  }

  return n;
}

LUX_API booln lxGFXTexture_init(lxGFXContextPTR ctx, lxGFXTexturePTR tex,
  lxGFXTextureType_t type, lxGFXTextureChannel_t format, lxGFXTextureDataType_t data,
  int w, int h, int d, int arraysize, flags32 flags)
{
  booln compressed = flags & LUXGFX_TEXTUREFLAG_COMPRESSED;

  tex->ctx = ctx;
  if (!(lxGFXTextureType_valid(ctx,type) && lxGFXTextureChannel_valid(ctx,format)))
    return LUX_FALSE;

  if (type == LUXGFX_TEXTURE_BUFFER){
    flags &= ~LUXGFX_TEXTUREFLAG_AUTOMIPMAP;
  }
  if ((!compressed) && (flags & LUXGFX_TEXTUREFLAG_COMPRESS))
  {
    tex->vgldata.internalformat = GetCompressed(format);
    tex->vgldata.datatype = GetDataType(data,LUX_FALSE,LUX_FALSE);
    tex->vgldata.dataformat = GetDataFormat(format,LUX_FALSE,LUX_FALSE);
  }

  // convert to GL internalformat
  if (!VGLTextureData_init(ctx,&tex->vgldata,format,data)){
    return LUX_FALSE;
  }

  if (!tex->vgl.id){
    glGenTextures(1,&tex->vgl.id);
  }
  tex->vgl.target = VGLTextureTarget_get(type);
  lxGFXTexture_apply(ctx,0,tex);
  if (flags & LUXGFX_TEXTUREFLAG_AUTOMIPMAP){
    glTexParameteri(VGLTARGET(tex),GL_GENERATE_MIPMAP,GL_TRUE);
  }

  {
    TexUpload_t up;
    TexUploadFuncType_t uptype = TEXUPLOAD_NONE;
    flags32 nochange = 0;

    lxGFXBuffer_apply(ctx,LUXGL_BUFFER_PIXELREAD,NULL);
    switch (type)
    {
      case LUXGFX_TEXTURE_1D:
        h = d = arraysize = 1;
        uptype = TEXUPLOAD_1D;
        break;
      case LUXGFX_TEXTURE_2D:
      case LUXGFX_TEXTURE_RECT:
        d = arraysize = 1;
        uptype = TEXUPLOAD_2D;
        break;
      case LUXGFX_TEXTURE_3D:
        arraysize = 1;
        uptype = TEXUPLOAD_3D;
        break;
      case LUXGFX_TEXTURE_CUBE:
        d = arraysize = 1;
        uptype = TEXUPLOAD_2D;
        break;
      case LUXGFX_TEXTURE_2DMS:
        uptype = TEXUPLOAD_NONE;
        glTexImage2DMultisample(VGLTARGET(tex),d,tex->vgldata.internalformat,
          w,h,flags & LUXGFX_TEXTUREFLAG_SAMPLESFIXED ? GL_TRUE : GL_FALSE);
        break;
      case LUXGFX_TEXTURE_1DARRAY:
        h = d = 1;
        uptype = TEXUPLOAD_2D;
        nochange |= 3;
        break;
      case LUXGFX_TEXTURE_2DARRAY:
        d = 1;
        nochange |= 2;
        uptype = TEXUPLOAD_NONE;
        glTexImage3D(VGLTARGET(tex),0,tex->vgldata.internalformat,
          w,h,arraysize,0,tex->vgldata.dataformat,tex->vgldata.datatype,NULL);
        break;
      case LUXGFX_TEXTURE_CUBEARRAY:
        nochange |= 2;
        d = 1;
        uptype = TEXUPLOAD_3D;
        break;
      case LUXGFX_TEXTURE_2DMSARRAY:
        nochange |= 2;
        d = 1;
        uptype = TEXUPLOAD_NONE;
        glTexImage3DMultisample(VGLTARGET(tex),d,tex->vgldata.internalformat,
          w,h,arraysize,flags & LUXGFX_TEXTUREFLAG_SAMPLESFIXED ? GL_TRUE : GL_FALSE);
        break;
      case LUXGFX_TEXTURE_BUFFER:
        uptype = TEXUPLOAD_NONE;
        w = h = d = arraysize = 0;
        break;
    }

    tex->miplevels = GetMipSizes(tex->pixelsizes,tex->mipsizes,w,h,d,nochange);

    up.border = 0;
    up.target = VGLTARGET(tex);
    up.level = 0;
    up.w = w;
    up.h = h;
    up.d = d;
    up.internal = tex->vgldata.internalformat;
    up.format = tex->vgldata.dataformat;
    up.type = tex->vgldata.datatype;

    if (arraysize > 1){
      up.d = arraysize;
      up.d *= type == LUXGFX_TEXTURE_CUBEARRAY ? 6 : 1;
    }
    if (type == LUXGFX_TEXTURE_CUBE){
      int i;
      for (i = 0; i < 6; ++i){
        up.target = GL_TEXTURE_CUBE_MAP_POSITIVE_X+i;
        TexUpload_run(&up,flags & LUXGFX_TEXTUREFLAG_MANMIPMAP,arraysize > 1,uptype);
      }
    }
    else{
      TexUpload_run(&up,flags & LUXGFX_TEXTUREFLAG_MANMIPMAP,arraysize > 1,uptype);
    }
  }

  if (flags & LUXGFX_TEXTUREFLAG_AUTOMIPMAP){
    tex->mipsdefined |= (1<<tex->miplevels)-1;
  }

  tex->flags = flags;
  tex->formattype = format;
  tex->datatype = data;
  tex->type = type;

  tex->width = w;
  tex->height = h;
  tex->depth = d;
  tex->arraysize = arraysize;
  tex->components = GetComponents(format);

  if (!(flags & LUXGFX_TEXTUREFLAG_COMPRESSED )){
    uint i;
    for (i = 0; i < tex->miplevels; i++){
      tex->nativesizes[i] = tex->pixelsizes[i];
      if (format != LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL){
        tex->nativesizes[i] *= tex->components;
        tex->nativesizes[i] *= GetDataTypeSize(data,LUX_FALSE);
      }
      else{
        tex->nativesizes[i] *= GetDataTypeSize(data,LUX_TRUE);
      }
    }
  }
  else{
    uint i;
    for (i = 0; i < tex->miplevels; i++){
      tex->nativesizes[i] = GetCompressedSize(tex->vgldata.internalformat,tex->pixelsizes[i]);
    }
  }

  tex->flags |= (ctx->capbits & LUXGFX_CAP_SM4) ? LUXGFX_TEXTUREFLAG_HASLOD : 0;

  lxGFXSampler_init(&tex->sampler);
  if (format == LUXGFX_TEXTURECHANNEL_DEPTH || format == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL){
    glTexParameteri(VGLTARGET(tex),GL_DEPTH_TEXTURE_MODE,GL_LUMINANCE);
    tex->sampler.filter = (lxGFXSamplerFilter_t)((uint)tex->sampler.filter+4);
    tex->flags |= LUXGFX_TEXTUREFLAG_HASCOMPARE;
  }
  lxGFXTextureUnit_setSampler(ctx,0,&tex->sampler,LUXGFX_SAMPLERATTRIB_ALL);
  return LUX_TRUE;
}

LUX_API const lxVec3iPTR  lxGFXTexture_getMipSize(const lxGFXTexturePTR tex, uint mipLevel)
{
  return &tex->mipsizes[mipLevel];
}

LUX_API void lxGFXTexture_deinit(lxGFXContextPTR ctx,  lxGFXTexturePTR tex )
{
  glDeleteTextures(1,&tex->vgl.id);
  tex->vgl.id = 0;
}


LUX_API booln lxGFXTexture_readFrame(lxGFXContextPTR ctx, lxGFXTexturePTR tex,
  const lxGFXTextureUpdatePTR update, uint mip)
{
  GLenum target = tex->vgl.target;
  int depth = tex->depth;

  lxGFXTexture_checked(ctx,0,tex);
  
  switch(tex->type){
    case LUXGFX_TEXTURE_1D:
      if (LUXGFX_VALIDITY &&(mip >= tex->miplevels ||
        tex->width < update->to.x+update->size.x))
      {
        return LUX_FALSE;
      }
      glCopyTexSubImage1D(target,mip,update->to.x,update->from.x,update->from.y,update->size.x);
      break;
    case LUXGFX_TEXTURE_CUBE:
      target = GL_TEXTURE_CUBE_MAP_POSITIVE_X+update->to.z;
    case LUXGFX_TEXTURE_1DARRAY:
    case LUXGFX_TEXTURE_2D:
    case LUXGFX_TEXTURE_RECT:
    case LUXGFX_TEXTURE_2DMS:
      if (LUXGFX_VALIDITY && (mip >= tex->miplevels ||
        tex->width < update->to.x+update->size.x || 
        tex->height < update->to.y+update->size.y))
      {
        return LUX_FALSE;
      }
      glCopyTexSubImage2D(target,mip,update->to.x,update->to.y,update->from.x,update->from.y,update->size.x,update->size.y);
      break;
    case LUXGFX_TEXTURE_CUBEARRAY:
      depth = tex->arraysize * 6;
    case LUXGFX_TEXTURE_2DARRAY:
      depth = tex->arraysize;
    case LUXGFX_TEXTURE_3D:
      if (LUXGFX_VALIDITY && (mip >= tex->miplevels || 
        tex->width < update->to.x+update->size.x || 
        tex->height < update->to.y+update->size.y ||
        depth < update->to.z))
      {
        return LUX_FALSE;
      }
      glCopyTexSubImage3D(target,mip,update->to.x,update->to.y,update->to.z,update->from.x,update->from.y,update->size.x,update->size.y);
      break;
  }
  return LUX_TRUE;
}

// if scalartype is set to invalid, we assume "native" data (matching
// internal)
LUX_API booln lxGFXTexture_readData(lxGFXContextPTR ctx, lxGFXTexturePTR tex,
    const lxGFXTextureUpdatePTR update,uint mip,
    enum lxScalarType_e d, const void* data, uint size)
{
  GLenum target = tex->vgl.target;
  GLenum dataformat = tex->vgldata.dataformat;
  GLenum datatype = tex->vgldata.datatype;
  booln compressed = (tex->flags & LUXGFX_TEXTUREFLAG_COMPRESSED);
  int depth = tex->depth;
  size_t nativesize = tex->nativesizes[mip];
  
  lxGFXTexture_checked(ctx,0,tex);

  if (LUXGFX_VALIDITY && (mip >= tex->miplevels) || (compressed && 
    (update->to.x%4 != 0  || update->to.y%4 != 0 ||
     update->size.x%4 != 0 || update->size.y%4 != 0))
     || !(tex->mipsdefined & (1<<mip)))
  {
    return LUX_FALSE;
  }

  switch(tex->type){
  case LUXGFX_TEXTURE_1D:
    if (LUXGFX_VALIDITY && (
      tex->width < update->to.x+update->size.x))
    {
      return LUX_FALSE;
    }
    if (!compressed)
      glTexSubImage1D(target,mip,update->to.x,update->size.x,dataformat,datatype,data);
    else
      glCompressedTexSubImage1D(target,mip,update->to.x,update->size.x,dataformat,nativesize,data);
    break;
  case LUXGFX_TEXTURE_CUBE:
    target = GL_TEXTURE_CUBE_MAP_POSITIVE_X+update->to.z;
  case LUXGFX_TEXTURE_1DARRAY:
  case LUXGFX_TEXTURE_2D:
  case LUXGFX_TEXTURE_RECT:
  case LUXGFX_TEXTURE_2DMS:
    if (LUXGFX_VALIDITY && ( 
      tex->width < update->to.x+update->size.x || 
      tex->height < update->to.y+update->size.y))
    {
      return LUX_FALSE;
    }
    if (!compressed)
      glTexSubImage2D(target,mip,update->to.x,update->to.y,update->size.x,update->size.y,dataformat,datatype,data);
    else
      glCompressedTexSubImage2D(target,mip,update->to.x,update->to.y,update->size.x,update->size.y,dataformat,nativesize,data);
    break;
  case LUXGFX_TEXTURE_CUBEARRAY:
    depth *= 6;
  case LUXGFX_TEXTURE_2DARRAY:
    depth *= tex->arraysize;
  case LUXGFX_TEXTURE_3D:
    if (LUXGFX_VALIDITY && ( 
      tex->width < update->to.x+update->size.x || 
      tex->height < update->to.y+update->size.y ||
      depth < update->to.z+update->size.z))
    {
      return LUX_FALSE;
    }
    if (!compressed)
      glTexSubImage3D(target,mip,update->to.x,update->to.y,update->to.z,update->size.x,update->size.y,update->size.z,dataformat,datatype,data);
    else
      glCompressedTexSubImage3D(target,mip,update->to.x,update->to.y,update->to.z,update->size.x,update->size.y,update->size.z,dataformat,nativesize,data);
    break;
  }

  return LUX_TRUE;

}

LUX_API booln lxGFXTexture_readBuffer(lxGFXContextPTR ctx, lxGFXTexturePTR tex, 
    const lxGFXTextureUpdatePTR update,uint miplevel,
    enum lxScalarType_e scalar, const lxGFXBufferPTR buffer, uint bufferoffset)
{
  if (LUXGFX_VALIDITY && buffer->size < bufferoffset) return LUX_FALSE;

  lxGFXBuffer_apply(ctx,LUXGL_BUFFER_PIXELREAD,buffer);
  return lxGFXTexture_readData(ctx,tex,update,miplevel,scalar,(void*)bufferoffset,buffer->size - bufferoffset);
}

LUX_API booln lxGFXTexture_resize(lxGFXContextPTR ctx, lxGFXTexturePTR tex, int width, int height)
{
  return LUX_FALSE;
}

LUX_API booln lxGFXTexture_writeData(lxGFXContextPTR ctx, lxGFXTexturePTR tex, uint side, booln ascompressed, booln onlydepth, 
    uint mip, enum lxScalarType_e d, void* buffer, uint buffersize)
{
  GLenum target = tex->vgl.target;
  GLenum dataformat = tex->vgldata.dataformat;
  GLenum datatype = tex->vgldata.datatype;
  booln compressed = (tex->flags & (LUXGFX_TEXTUREFLAG_COMPRESSED | LUXGFX_TEXTUREFLAG_COMPRESS));
  int depth = tex->depth;
  size_t nativesize = tex->nativesizes[mip]; // CORRECT BASED ON MIP

  lxGFXTexture_checked(ctx,0,tex);

  if (LUXGFX_VALIDITY && (mip >= tex->miplevels ))
  {
    return LUX_FALSE;
  }

  if (tex->formattype == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL){
    dataformat = onlydepth ? GL_DEPTH_COMPONENT : GL_DEPTH_STENCIL;
  }

  if (d != SCALAR_ILLEGAL){
    datatype = lxScalarType_to(d);
  }

  switch(tex->type){
  case LUXGFX_TEXTURE_CUBE:
    target = GL_TEXTURE_CUBE_MAP_POSITIVE_X+side;
  case LUXGFX_TEXTURE_1DARRAY:
  case LUXGFX_TEXTURE_2DARRAY:
  case LUXGFX_TEXTURE_CUBEARRAY:
  case LUXGFX_TEXTURE_2D:
  case LUXGFX_TEXTURE_RECT:
  case LUXGFX_TEXTURE_2DMS:
  case LUXGFX_TEXTURE_3D:
    if (!compressed || !ascompressed)
      glGetTexImage(target,mip,dataformat,datatype,buffer);
    else
      glGetCompressedTexImage(target,mip,buffer);
    break;
  }

  return LUX_TRUE;
}

LUX_API booln lxGFXTexture_writeBuffer(lxGFXContextPTR ctx, lxGFXTexturePTR tex, uint side,
    booln ascompressed, booln onlydepth, uint mip, enum lxScalarType_e d, lxGFXBufferPTR buffer, uint bufferoffset)
{
  if (LUXGFX_VALIDITY && buffer->size < bufferoffset) return LUX_FALSE;

  lxGFXBuffer_apply(ctx,LUXGL_BUFFER_PIXELWRITE,buffer);
  return lxGFXTexture_writeData(ctx,tex,side,ascompressed,onlydepth,mip,d,(void*)bufferoffset,buffer->size - bufferoffset);
}


LUX_API void lxGFXTextureUnit_setCompare(lxGFXContextPTR ctx, uint imageunit, lxGLCompareMode_t cmpfunc)
{
  lxGFXTexturePTR tex = ctx->textures[imageunit];
  booln run = cmpfunc != LUXGL_COMPARE_DONTEXECUTE;

  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  glTexParameteri(VGLTARGET(tex),GL_TEXTURE_COMPARE_MODE_ARB,
    run ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);

  glTexParameteri(VGLTARGET(tex),GL_TEXTURE_COMPARE_FUNC_ARB,run ? cmpfunc : GL_LEQUAL);
  tex->sampler.cmpfunc = cmpfunc;
}

LUX_API LUX_INLINE void lxGFXTextureUnit_setSampler(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR sampler , flags32 what)
{
  lxGFXTexturePTR tex = ctx->textures[imageunit];
  GLenum minfilter;
  GLenum magfilter;

  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  
  if (what & LUXGFX_SAMPLERATTRIB_ADDRESS){
    glTexParameteri(VGLTARGET(tex),GL_TEXTURE_WRAP_S,sampler->address[0]);
    glTexParameteri(VGLTARGET(tex),GL_TEXTURE_WRAP_T,sampler->address[1]);
    glTexParameteri(VGLTARGET(tex),GL_TEXTURE_WRAP_R,sampler->address[2]);
  }

  if (what & LUXGFX_SAMPLERATTRIB_BORDER){
    glTexParameterfv(VGLTARGET(tex),GL_TEXTURE_BORDER_COLOR,sampler->border);
  }
  
  if (what & (LUXGFX_SAMPLERATTRIB_CMP)){
    if (tex->flags & LUXGFX_TEXTUREFLAG_HASCOMPARE){
      booln run = sampler->cmpfunc != LUXGL_COMPARE_DONTEXECUTE;
      glTexParameteri(VGLTARGET(tex),GL_TEXTURE_COMPARE_MODE_ARB,
         run ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);

      glTexParameteri(VGLTARGET(tex),GL_TEXTURE_COMPARE_FUNC_ARB,run ? sampler->cmpfunc : GL_LEQUAL);
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_ANISO){
    if (sampler->aniso){
      glTexParameteri(VGLTARGET(tex), GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler->aniso);
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_LOD){
    if (tex->flags & LUXGFX_TEXTUREFLAG_HASLOD){
      glTexParameterf(VGLTARGET(tex),GL_TEXTURE_MIN_LOD,sampler->lod.min);
      glTexParameterf(VGLTARGET(tex),GL_TEXTURE_MAX_LOD,sampler->lod.max);

      glTexParameterf(VGLTARGET(tex),GL_TEXTURE_LOD_BIAS_EXT,sampler->lod.bias);
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_FILTER){
    switch (sampler->filter)
    {
    case LUXGFX_SAMPLERFILTER_NEAREST:
      minfilter = magfilter = GL_NEAREST;
      break;
    case LUXGFX_SAMPLERFILTER_LINEAR:
      minfilter = magfilter = GL_LINEAR;
      break;
    case LUXGFX_SAMPLERFILTER_MIPMAP_NEAREST:
      magfilter = GL_LINEAR;
      minfilter = GL_LINEAR_MIPMAP_NEAREST;
      break;
    case LUXGFX_SAMPLERFILTER_MIPMAP_LINEAR:
      magfilter = GL_LINEAR;
      minfilter = GL_LINEAR_MIPMAP_LINEAR;
      break;
    }

    glTexParameteri(VGLTARGET(tex),GL_TEXTURE_MIN_FILTER,minfilter);
    glTexParameteri(VGLTARGET(tex),GL_TEXTURE_MAG_FILTER,magfilter);
  }

  tex->lastSampler = sampler;
  tex->lastSamplerIncarnation = sampler->incarnation;
  tex->sampler = *sampler;
  tex->sampler.vgl.id = 0;
}

LUX_API void lxGFXTextureUnit_checkedSampler(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR sampler, flags32 what){
  lxGFXTexturePTR tex = ctx->textures[imageunit];
  flags32 change = 0;

  if (tex->lastSampler == sampler 
      && sampler->incarnation == tex->lastSamplerIncarnation) return;

  if (what & LUXGFX_SAMPLERATTRIB_ANISO && sampler->aniso != tex->sampler.aniso)
  {
    change |= LUXGFX_SAMPLERATTRIB_ANISO;
  }
  if (what & LUXGFX_SAMPLERATTRIB_CMP && sampler->cmpfunc != tex->sampler.cmpfunc)
  {
    change |= LUXGFX_SAMPLERATTRIB_CMP;
  }
  if (what & LUXGFX_SAMPLERATTRIB_FILTER && sampler->filter != tex->sampler.filter)
  {
    change |= LUXGFX_SAMPLERATTRIB_FILTER;
  }
  if (what & LUXGFX_SAMPLERATTRIB_ADDRESS && 
    memcmp(tex->sampler.address,sampler->address,sizeof(sampler->address)) == 0)
  {
    change |= LUXGFX_SAMPLERATTRIB_ADDRESS;
  }
  if (what & LUXGFX_SAMPLERATTRIB_BORDER && 
    memcmp(tex->sampler.border,sampler->border,sizeof(sampler->border)) == 0)
  {
    change |= LUXGFX_SAMPLERATTRIB_BORDER;
  }
  if (what & LUXGFX_SAMPLERATTRIB_LOD && 
    memcmp(&tex->sampler.lod,&sampler->lod,sizeof(sampler->lod)) == 0)
  {
    change |= LUXGFX_SAMPLERATTRIB_LOD;
  }
  if (change){
    lxGFXTextureUnit_setSampler(ctx,imageunit,sampler,change);
  }
}




//////////////////////////////////////////////////////////////////////////
// lxGFXSampler_t

LUX_API void lxGFXSampler_init( lxGFXSamplerPTR self )
{
  self->vgl.id = 0;
  self->filter = LUXGFX_SAMPLERFILTER_LINEAR;
  self->address[0] = LUXGL_ADDRESS_REPEAT;
  self->address[1] = LUXGL_ADDRESS_REPEAT;
  self->address[2] = LUXGL_ADDRESS_REPEAT;
  self->aniso = 1;
  self->lod.min = -1000.0f;
  self->lod.max = 1000.0f;
  self->lod.bias = 0.0f;
  self->cmpfunc = LUXGL_COMPARE_LEQUAL;
  LUX_ARRAY4SET(self->border,0,0,0,0);
}

LUX_API void lxGFXSampler_initObj(lxGFXContextPTR ctx, lxGFXSamplerPTR self)
{
  lxGFXSampler_init(self);
  glGenSamplers(1,&self->vgl.id);
}

LUX_API void lxGFXSampler_setAddress(lxGFXSamplerPTR self, uint n, lxGFXSamplerAddress_t address)
{
  lxGLAddressMode_t modes[LUXGFX_SAMPLERADDRESSES] = {
    LUXGL_ADDRESS_REPEAT,
    LUXGL_ADDRESS_CLAMP,
    LUXGL_ADDRESS_BORDER,
    LUXGL_ADDRESS_MIRROR,
  };
  LUX_DEBUGASSERT(n <= 3 && address <= LUXGFX_SAMPLERADDRESSES);
  self->address[n] = modes[address];
}

LUX_API void lxGFXSampler_setCompare(lxGFXSamplerPTR self, lxGFXCompareMode_t cmp)
{
  self->cmpfunc = VGLCompareMode_get(cmp);
}

LUX_API void lxGFXSampler_changed(lxGFXSamplerPTR self)
{
  self->incarnation++;
}

LUX_API void lxGFXSampler_deinitObj(lxGFXContextPTR ctx, lxGFXSamplerPTR self)
{
  if (self->vgl.id) glDeleteSamplers(1,&self->vgl.id);
}

LUX_API void lxGFXSampler_updateObj(lxGFXContextPTR ctx, lxGFXSamplerPTR sampler)
{
  lxGLSampler_t vgl = sampler->vgl;
  GLenum minfilter;
  GLenum magfilter;
  booln runcmp = sampler->cmpfunc != LUXGL_COMPARE_DONTEXECUTE;

  glSamplerParameteri(vgl.id,GL_TEXTURE_WRAP_S,sampler->address[0]);
  glSamplerParameteri(vgl.id,GL_TEXTURE_WRAP_T,sampler->address[1]);
  glSamplerParameteri(vgl.id,GL_TEXTURE_WRAP_R,sampler->address[2]);

  glSamplerParameterfv(vgl.id,GL_TEXTURE_BORDER_COLOR,sampler->border);

  glSamplerParameteri(vgl.id,GL_TEXTURE_COMPARE_MODE_ARB,
    runcmp ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);
  glSamplerParameteri(vgl.id,GL_TEXTURE_COMPARE_FUNC_ARB,
    runcmp ? sampler->cmpfunc : GL_LEQUAL);

  glSamplerParameteri(vgl.id, GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler->aniso);

  glSamplerParameterf(vgl.id,GL_TEXTURE_MIN_LOD,sampler->lod.min);
  glSamplerParameterf(vgl.id,GL_TEXTURE_MAX_LOD,sampler->lod.max);
  glSamplerParameterf(vgl.id,GL_TEXTURE_LOD_BIAS_EXT,sampler->lod.bias);

  switch (sampler->filter)
  {
  case LUXGFX_SAMPLERFILTER_NEAREST:
    minfilter = magfilter = GL_NEAREST;
    break;
  case LUXGFX_SAMPLERFILTER_LINEAR:
    minfilter = magfilter = GL_LINEAR;
    break;
  case LUXGFX_SAMPLERFILTER_MIPMAP_NEAREST:
    magfilter = GL_LINEAR;
    minfilter = GL_LINEAR_MIPMAP_NEAREST;
    break;
  case LUXGFX_SAMPLERFILTER_MIPMAP_LINEAR:
    magfilter = GL_LINEAR;
    minfilter = GL_LINEAR_MIPMAP_LINEAR;
    break;
  }

  glSamplerParameteri(vgl.id,GL_TEXTURE_MIN_FILTER,minfilter);
  glSamplerParameteri(vgl.id,GL_TEXTURE_MAG_FILTER,magfilter);

}

LUX_API void lxGFXSampler_apply(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR self)
{
  ctx->samplers[imageunit] = self;
  if (self->vgl.id) glBindSampler(imageunit,self->vgl.id);
}

LUX_API void  lxGFXSamplers_apply(lxGFXContextPTR ctx, uint start, uint num, lxGFXSamplerPTR *samps)
{
  uint i;
  for (i = 0; i < num; i++){
    ctx->samplers[i+start] = samps[i];
    if (samps[i]->vgl.id) glBindSampler(i+start,samps[i]->vgl.id);
  }
}

//////////////////////////////////////////////////////////////////////////
// lxGFXRenderBuffer

LUX_API booln lxGFXRenderBuffer_init(lxGFXContextPTR ctx, lxGFXRenderBufferPTR rb, lxGFXTextureChannel_t format,int width, int height, int samples)
{
  rb->ctx = ctx;
  glGenRenderbuffersEXT(1,&rb->vgl.id);
  return lxGFXRenderBuffer_change(ctx,rb,format,width,height,samples);
}

LUX_API booln lxGFXRenderBuffer_change(lxGFXContextPTR ctx, lxGFXRenderBufferPTR rb,
   lxGFXTextureChannel_t format, int width, int height, int samples)
{
  if (!lxGFXTextureChannel_valid(ctx,format) || samples > ctx->capabilites.fbosamples)
    return LUX_FALSE;

  rb->formattype = format;
  rb->width = width;
  rb->height = height;
  rb->samples = samples;
  
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb->vgl.id);
  if (samples)
    glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, GetInternal(format,LUXGFX_TEXTUREDATA_BASE), width, height);
  else
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GetInternal(format,LUXGFX_TEXTUREDATA_BASE), width, height);

  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  return LUX_TRUE;
}

LUX_API void  lxGFXRenderBuffer_deinit(lxGFXContextPTR ctx, lxGFXRenderBufferPTR rb)
{
  glDeleteRenderbuffersEXT(1,&rb->vgl.id);
}

//////////////////////////////////////////////////////////////////////////

LUX_API void  lxGFXTextureImage_apply(lxGFXContextPTR ctx, uint imageunit, lxGFXTextureImagePTR img)
{
  glBindImageTextureEXT(imageunit,img->tex->vgl.id,img->level,img->layered,img->layer,img->access,img->format);
  ctx->images[imageunit] = img;
}

