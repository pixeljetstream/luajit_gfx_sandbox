// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/texture.h>
#include <luxgfx/luxgfx.h>
#include <luxplatform/debug.h>
#include <luxmath/misc.h>
#include <luxcore/scalarmisc.h>

#include "state_inl.h"

#define lxGLTARGET(tex)  ((tex)->gltarget)

//////////////////////////////////////////////////////////////////////////
// lxGLTextureTarget_t


typedef enum lxGLAddressMode_e{
  LUXGL_SAMPLERADDRESS_REPEAT = GL_REPEAT,
  LUXGL_SAMPLERADDRESS_CLAMP = GL_CLAMP_TO_EDGE,
  LUXGL_SAMPLERADDRESS_BORDER = GL_CLAMP_TO_BORDER,
  LUXGL_SAMPLERADDRESS_MIRROR = GL_MIRRORED_REPEAT,
}lxGLAddressMode_t;


//////////////////////////////////////////////////////////////////////////
// lxGLTextureData_t

static GLenum  GetBase(lxgTextureChannel_t type)
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

static uint  GetComponents(lxgTextureChannel_t type)
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
static GLenum  GetDataTypeSize(lxgTextureDataType_t data, booln depthstencil)
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


static GLenum  GetDataType(lxgTextureDataType_t data, booln rev, booln depthstencil)
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

static GLenum  GetDataFormat(lxgTextureChannel_t type, booln rev, booln integer)
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

static GLenum  GetCompressed(lxgTextureChannel_t type)
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

static GLenum  GetInternal(lxgTextureChannel_t type, lxgTextureDataType_t data)
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

static booln lxGLTextureData_init(lxgContextPTR ctx, lxgTexturePTR tex,
  lxgTextureChannel_t format, lxgTextureDataType_t data)
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
  tex->gldataformat = GetDataFormat(format,LUX_FALSE,data > LUXGFX_TEXTUREDATA_FLOAT32);
  tex->gldatatype = GetDataType(data,LUX_FALSE,format == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL);
  tex->glinternalformat = GetInternal(format,data);
  if (!tex->gldataformat || !tex->gldatatype || !tex->glinternalformat)
    return LUX_FALSE;

  return LUX_TRUE;
}

//////////////////////////////////////////////////////////////////////////
// lxgTexture

LUX_API void lxgTexture_apply( lxgContextPTR ctx, uint imageunit, lxgTexturePTR tex)
{
  ctx->textures[imageunit] = tex;
  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  glBindTexture(tex->gltarget,tex->glid);
}

LUX_API void  lxgTextures_apply(lxgContextPTR ctx, uint start, uint num, lxgTexturePTR *texs)
{
  int i;
  for (i = num-1; i >= 0; i--){
    ctx->textures[i+start] = texs[i];
    glActiveTexture(GL_TEXTURE0_ARB + i);
    glBindTexture(texs[i]->gltarget,texs[i]->glid);
  }
}

LUX_API booln lxgTextureTarget_valid(lxgContextPTR ctx, lxGLTextureTarget_t type)
{
  if (((type == LUXGL_TEXTURE_1DARRAY ||type == LUXGL_TEXTURE_2DARRAY ) && !(ctx->capbits & LUXGFX_CAP_SM4))||
    (type == LUXGL_TEXTURE_CUBEARRAY && !(ctx->capbits & LUXGFX_CAP_TEXCUBEARRAY))||
    (type == LUXGL_TEXTURE_3D && !(ctx->capbits & LUXGFX_CAP_TEX3D))||
    (type == LUXGL_TEXTURE_RECT && !(ctx->capbits & LUXGFX_CAP_TEXRECT))||
    ((type == LUXGL_TEXTURE_2DMS || type == LUXGL_TEXTURE_2DMSARRAY ) && !(ctx->capbits & LUXGFX_CAP_SM4))||
    (type == LUXGL_TEXTURE_BUFFER && !(ctx->capbits & LUXGFX_CAP_SM4)))
  {
    return LUX_FALSE;
  }
  return LUX_TRUE;
}

LUX_API booln lxgTextureChannel_valid(lxgContextPTR ctx, lxgTextureChannel_t channel)
{
  if ((channel == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL  && !(ctx->capbits & LUXGFX_CAP_FBO))||
    ((channel == LUXGFX_TEXTURECHANNEL_SRGB || channel == LUXGFX_TEXTURECHANNEL_SRGBA ) && !(ctx->capbits & LUXGFX_CAP_SM4)))
  {
    return LUX_FALSE;
  }
  return LUX_TRUE;
}


typedef enum TexUploadFuncType_e
{
  TEXUPLOAD_1D,
  TEXUPLOAD_1D_COMPRESSED,

  TEXUPLOAD_2D,
  TEXUPLOAD_2D_COMPRESSED,

  TEXUPLOAD_3D,
  TEXUPLOAD_3D_COMPRESSED,

  TEXUPLOAD_1DARRAY,
  TEXUPLOAD_1DARRAY_COMPRESSED,

  TEXUPLOAD_2DARRAY,
  TEXUPLOAD_2DARRAY_COMPRESSED,

  TEXUPLOAD_2DMS,
  TEXUPLOAD_2DMS_ILLEGAL,

  TEXUPLOAD_2DMSARRAY,
  TEXUPLOAD_2DMSARRAY_ILLEGAL,

  TEXUPLOAD_NONE,
}TexUploadFuncType_t;

typedef struct TexUpload_s{
  TexUploadFuncType_t uptype;
  GLenum target;
  int level;
  GLenum internal;
  lxVec3i_t size;
  int border;
  GLenum format;
  GLenum type;
  int compressedsize;
  int arraysize;
  int samples;
  booln samplesfixed;
}TexUpload_t;
typedef void (*TexUpload_fn) (TexUpload_t* tu);

static void TexUpload_1D(TexUpload_t*  tu)
{
  glTexImage1D(tu->target,tu->level,tu->internal,tu->size.x,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_1DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage1DARB(tu->target,tu->level,tu->format,tu->size.x,tu->border,tu->compressedsize,NULL);
}
static void TexUpload_2D(TexUpload_t*  tu)
{
  glTexImage2D(tu->target,tu->level,tu->internal,tu->size.x,tu->size.y,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_2DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage2DARB(tu->target,tu->level,tu->format,tu->size.x,tu->size.y,tu->border,tu->compressedsize,NULL);
}
static void TexUpload_3D(TexUpload_t*  tu)
{
  glTexImage3D(tu->target,tu->level,tu->internal,tu->size.x,tu->size.y,tu->size.z,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_3DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage3DARB(tu->target,tu->level,tu->format,tu->size.x,tu->size.y,tu->size.z,tu->border,tu->compressedsize,NULL);
}
static void TexUpload_1DARRAY(TexUpload_t*  tu)
{
  glTexImage2D(tu->target,tu->level,tu->internal,tu->size.x,tu->arraysize,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_1DARRAYCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage2DARB(tu->target,tu->level,tu->format,tu->size.x,tu->arraysize,tu->border,tu->compressedsize,NULL);
}
static void TexUpload_2DARRAY(TexUpload_t*  tu)
{
  glTexImage3D(tu->target,tu->level,tu->internal,tu->size.x,tu->size.y,tu->arraysize,tu->border,tu->format,tu->type,NULL);
}
static void TexUpload_2DARRAYCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage3DARB(tu->target,tu->level,tu->format,tu->size.x,tu->size.y,tu->arraysize,tu->border,tu->compressedsize,NULL);
}
static void TexUpload_2DMS(TexUpload_t*  tu)
{
  glTexImage2DMultisample(tu->target,tu->samples,tu->internal,tu->size.x,tu->size.y,tu->samplesfixed);
}
static void TexUpload_2DMSARRAY(TexUpload_t*  tu)
{
  glTexImage3DMultisample(tu->target,tu->samples,tu->internal,tu->size.x,tu->size.y,tu->arraysize,tu->samplesfixed);
}

static TexUpload_fn l_uploadfuncs[] =
{
  TexUpload_1D,
  TexUpload_1DCompressed,
  TexUpload_2D,
  TexUpload_2DCompressed,
  TexUpload_3D,
  TexUpload_3DCompressed,
  TexUpload_1DARRAY,
  TexUpload_1DARRAYCompressed,
  TexUpload_2DARRAY,
  TexUpload_2DARRAYCompressed,
  TexUpload_2DMS,
  NULL,
  TexUpload_2DMSARRAY,
  NULL,
};

static uint GetMipSizes(uint* mippixels, lxVec3i_t* mipsizes, int w, int h, int d)
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
    h /= 2;
    d /= 2;
    w = LUX_MAX(1,w);
    h = LUX_MAX(1,h);
    d = LUX_MAX(1,d);

    mips++;
  } while(w*h*d != lastsize);

  return mips;
}

static void TexUpload_run(TexUpload_t* up, lxgTexturePTR tex, booln mipmaps)
{
  TexUploadFuncType_t type = up->uptype;
  uint levels = mipmaps ? tex->miplevels : 1;
  uint n;

  if (type == TEXUPLOAD_NONE || !l_uploadfuncs[type]) return;

  for (n = 0; n < levels; n++){
    up->level = n;
    up->size = *lxgTexture_getMipSize(tex,n);
    up->compressedsize = tex->nativesizes[n];
    l_uploadfuncs[type](up);
  }
}

static void lxgTexture_updateSizes(lxgContextPTR ctx, lxgTexturePTR tex, 
                                   int width, int height, int depth, int arraysize, int samples)
{
  TexUpload_t up;
  TexUploadFuncType_t uptype = TEXUPLOAD_NONE;

  up.size.x = 1;
  up.size.y = 1;
  up.size.z = 1;
  up.arraysize  = 0;
  up.samples = 0;

  // size correction
  switch (tex->gltarget)
  {
  case LUXGL_TEXTURE_1D:
    up.uptype = TEXUPLOAD_1D;
    up.size.x = width;
    break;
  case LUXGL_TEXTURE_2D:
  case LUXGL_TEXTURE_RECT:
    up.uptype = TEXUPLOAD_2D;
    up.size.x = width;
    up.size.y = height;
    break;
  case LUXGL_TEXTURE_3D:
    up.uptype = TEXUPLOAD_3D;
    up.size.x = width;
    up.size.y = height;
    up.size.z = depth;
    break;
  case LUXGL_TEXTURE_CUBE:
    up.uptype = TEXUPLOAD_2D;
    up.size.x = width;
    up.size.y = height;
    break;
  case LUXGL_TEXTURE_1DARRAY:
    up.uptype = TEXUPLOAD_1DARRAY;
    up.size.x = width;
    up.arraysize = arraysize;
    break;
  case LUXGL_TEXTURE_2DARRAY:
    up.uptype = TEXUPLOAD_2DARRAY;
    up.size.x = width;
    up.size.y = height;
    up.arraysize = arraysize;
    break;
  case LUXGL_TEXTURE_CUBEARRAY:
    up.uptype = TEXUPLOAD_2DARRAY;
    up.size.x = width;
    up.size.y = height;
    up.arraysize = arraysize;
    break;
  case LUXGL_TEXTURE_2DMS:
    up.uptype = LUXGL_TEXTURE_2DMS;
    up.size.x = width;
    up.size.y = height;
    up.samples = samples;
    up.samplesfixed = !!(tex->flags & LUXGFX_TEXTUREFLAG_SAMPLESFIXED);
    break;
  case LUXGL_TEXTURE_2DMSARRAY:
    up.uptype = LUXGL_TEXTURE_2DMSARRAY;
    up.size.x = width;
    up.size.y = height;
    up.samples = samples;
    up.samplesfixed = !!(tex->flags & LUXGFX_TEXTUREFLAG_SAMPLESFIXED);
    up.arraysize = arraysize;
    break;
  case LUXGL_TEXTURE_BUFFER:
  case LUXGL_TEXTURE_RENDERBUFFER:
    up.uptype = TEXUPLOAD_NONE;
    break;
  }

  tex->width = up.size.x;
  tex->height = up.size.y;
  tex->depth = up.size.z;
  tex->arraysize = up.arraysize;
  tex->samples = up.samples;
  tex->miplevels = GetMipSizes(tex->pixelsizes,tex->mipsizes,tex->width,tex->height,tex->depth);

  if (!(tex->flags & LUXGFX_TEXTUREFLAG_COMPRESSED )){
    uint i;
    for (i = 0; i < tex->miplevels; i++){
      tex->nativesizes[i] = tex->pixelsizes[i];
      if (tex->formattype != LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL){
        tex->nativesizes[i] *= tex->components;
        tex->nativesizes[i] *= GetDataTypeSize(tex->datatype,LUX_FALSE);
      }
      else{
        tex->nativesizes[i] *= GetDataTypeSize(tex->datatype,LUX_TRUE);
      }
    }
    tex->componentsize = GetDataTypeSize(tex->datatype,tex->formattype == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL);
  }
  else{
    uint i;
    for (i = 0; i < tex->miplevels; i++){
      tex->nativesizes[i] = GetCompressedSize(tex->glinternalformat,tex->pixelsizes[i]);
    }
    tex->componentsize = 0;
  }

  tex->mipsdefined = 1;
  if ((tex->flags & LUXGFX_TEXTUREFLAG_AUTOMIPMAP) ||
      (tex->flags & LUXGFX_TEXTUREFLAG_MANMIPMAP)){
    tex->mipsdefined |= (1<<tex->miplevels)-1;
  }

  // do actual specification
  up.border = 0;
  up.target = lxGLTARGET(tex);
  up.internal = tex->glinternalformat;
  up.format = tex->gldataformat;
  up.type = tex->gldatatype;

  lxgTexture_apply(ctx,0,tex);
  lxgBuffer_apply(ctx,LUXGL_BUFFER_PIXELREAD,NULL);
  if (tex->gltarget == LUXGL_TEXTURE_CUBE){
    int i;
    for (i = 0; i < 6; ++i){
      up.target = GL_TEXTURE_CUBE_MAP_POSITIVE_X+i;
      TexUpload_run(&up,tex,tex->flags & LUXGFX_TEXTUREFLAG_MANMIPMAP);
    }
  }
  else{
    TexUpload_run(&up,tex,tex->flags & LUXGFX_TEXTUREFLAG_MANMIPMAP);
  }
}

LUX_API booln lxgTexture_resize(lxgContextPTR ctx, lxgTexturePTR tex, int width, int height, int depth, int arraysize)
{
  GLenum target = tex->gltarget;
  booln compressed = (tex->flags & LUXGFX_TEXTUREFLAG_COMPRESSED);

  if (tex->width == width 
    && tex->height == height 
    && tex->depth == depth 
    && (!tex->samples || (tex->samples && tex->samples == depth))
    && tex->arraysize == arraysize) return LUX_TRUE;

  switch (target)
  {
  case LUXGL_TEXTURE_RENDERBUFFER:
  case LUXGL_TEXTURE_BUFFER:
    return LUX_FALSE;
  default:
    break;
  }

  lxgTexture_updateSizes(ctx,tex,width,height,depth,arraysize,depth);

  return LUX_TRUE;
}

LUX_API void lxgTexture_init(lxgContextPTR ctx, lxgTexturePTR tex)
{
  memset(tex,0,sizeof(lxgTexture_t));
  glGenTextures(1,&tex->glid);
}

LUX_API booln lxgTexture_setup(lxgContextPTR ctx, lxgTexturePTR tex,
  lxGLTextureTarget_t type, lxgTextureChannel_t format, lxgTextureDataType_t data,
  int w, int h, int d, int arraysize, flags32 flags)
{
  booln compressed = flags & LUXGFX_TEXTUREFLAG_COMPRESSED;

  tex->ctx = ctx;
  if (!(lxgTextureTarget_valid(ctx,type) && lxgTextureChannel_valid(ctx,format)))
    return LUX_FALSE;

  if (type == LUXGL_TEXTURE_BUFFER ||
      type == LUXGL_TEXTURE_RENDERBUFFER)
  {
    flags &= ~LUXGFX_TEXTUREFLAG_AUTOMIPMAP;
  }
  if ((!compressed) && (flags & LUXGFX_TEXTUREFLAG_COMPRESS))
  {
    tex->glinternalformat = GetCompressed(format);
    tex->gldatatype = GetDataType(data,LUX_FALSE,LUX_FALSE);
    tex->gldataformat = GetDataFormat(format,LUX_FALSE,LUX_FALSE);
  }

  // convert to GL internalformat
  if (!lxGLTextureData_init(ctx,tex,format,data)){
    return LUX_FALSE;
  }

  if (!tex->glid){
    glGenTextures(1,&tex->glid);
  }
  tex->gltarget = type;
  lxgTexture_apply(ctx,0,tex);

  if (flags & LUXGFX_TEXTUREFLAG_AUTOMIPMAP){
    glTexParameteri(lxGLTARGET(tex),GL_GENERATE_MIPMAP,GL_TRUE);
  }


  tex->flags = flags;
  tex->formattype = format;
  tex->datatype = data;
  tex->components = GetComponents(format);

  lxgTexture_updateSizes(ctx,tex,w,h,d,arraysize,d);

  tex->flags |= (ctx->capbits & LUXGFX_CAP_SM4) ? LUXGFX_TEXTUREFLAG_HASLOD : 0;
  lxgSampler_init(&tex->sampler);
  if (format == LUXGFX_TEXTURECHANNEL_DEPTH || format == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL){
    glTexParameteri(lxGLTARGET(tex),GL_DEPTH_TEXTURE_MODE,GL_LUMINANCE);
    tex->sampler.filter = (lxgSamplerFilter_t)((uint)tex->sampler.filter+4);
    tex->flags |= LUXGFX_TEXTUREFLAG_HASCOMPARE;
  }
  lxgTextureUnit_setSampler(ctx,0,&tex->sampler,LUXGFX_SAMPLERATTRIB_ALL);
  return LUX_TRUE;
}

LUX_API const lxVec3iPTR  lxgTexture_getMipSize(const lxgTexturePTR tex, uint mipLevel)
{
  return &tex->mipsizes[mipLevel];
}

LUX_API void lxgTexture_deinit(lxgContextPTR ctx,  lxgTexturePTR tex )
{
  glDeleteTextures(1,&tex->glid);
  tex->glid = 0;
}


LUX_API booln lxgTexture_readFrame(lxgContextPTR ctx, lxgTexturePTR tex,
  const lxgTextureUpdatePTR update, uint mip)
{
  GLenum target = tex->gltarget;
  int depth = tex->depth;

  lxgTexture_checked(ctx,0,tex);
  
  switch(target){
    case LUXGL_TEXTURE_1D:
      if (LUXGFX_VALIDITY &&(mip >= tex->miplevels ||
        tex->width < update->to.x + update->size.x))
      {
        return LUX_FALSE;
      }
      glCopyTexSubImage1D(target,mip,update->to.x,update->from.x,update->from.y,update->size.x);
      break;
    case LUXGL_TEXTURE_CUBE:
      target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + update->to.z;
    case LUXGL_TEXTURE_2D:
    case LUXGL_TEXTURE_RECT:
    case LUXGL_TEXTURE_2DMS:
      if (LUXGFX_VALIDITY && (mip >= tex->miplevels ||
        tex->width < update->to.x + update->size.x || 
        tex->height < update->to.y + update->size.y))
      {
        return LUX_FALSE;
      }
      glCopyTexSubImage2D(target,mip,update->to.x,update->to.y,update->from.x,update->from.y,update->size.x,update->size.y);
      break;
    case LUXGL_TEXTURE_1DARRAY:
      if (LUXGFX_VALIDITY && (mip >= tex->miplevels ||
        tex->width < update->to.x + update->size.x || 
        tex->arraysize < update->to.y + update->size.y))
      {
        return LUX_FALSE;
      }
      glCopyTexSubImage2D(target,mip,update->to.x,update->to.y,update->from.x,update->from.y,update->size.x,update->size.y);
      break;
    case LUXGL_TEXTURE_CUBEARRAY:
    case LUXGL_TEXTURE_2DARRAY:
      depth = tex->arraysize;
    case LUXGL_TEXTURE_3D:
      if (LUXGFX_VALIDITY && (mip >= tex->miplevels || 
        tex->width < update->to.x + update->size.x || 
        tex->height < update->to.y + update->size.y ||
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
LUX_API booln lxgTexture_readData(lxgContextPTR ctx, lxgTexturePTR tex,
    const lxgTextureUpdatePTR update,uint mip,
    enum lxScalarType_e d, const void* data, uint size)
{
  GLenum target = tex->gltarget;
  GLenum dataformat = tex->gldataformat;
  GLenum datatype = tex->gldatatype;
  booln compressed = (tex->flags & LUXGFX_TEXTUREFLAG_COMPRESSED);
  int depth = tex->depth;
  size_t nativesize = tex->nativesizes[mip];
  
  lxgTexture_checked(ctx,0,tex);

  if (LUXGFX_VALIDITY && (mip >= tex->miplevels) || (compressed && 
    (update->to.x%4 != 0  || update->to.y%4 != 0 ||
     update->size.x%4 != 0 || update->size.y%4 != 0))
     || !(tex->mipsdefined & (1<<mip)))
  {
    return LUX_FALSE;
  }

  switch(target){
  case LUXGL_TEXTURE_1D:
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
  case LUXGL_TEXTURE_CUBE:
    target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + update->to.z;
  case LUXGL_TEXTURE_1DARRAY:
  case LUXGL_TEXTURE_2D:
  case LUXGL_TEXTURE_RECT:
  case LUXGL_TEXTURE_2DMS:
    if (LUXGFX_VALIDITY && ( 
      tex->width < update->to.x + update->size.x || 
      tex->height < update->to.y + update->size.y))
    {
      return LUX_FALSE;
    }
    if (!compressed)
      glTexSubImage2D(target,mip,update->to.x,update->to.y,update->size.x,update->size.y,dataformat,datatype,data);
    else
      glCompressedTexSubImage2D(target,mip,update->to.x,update->to.y,update->size.x,update->size.y,dataformat,nativesize,data);
    break;
  case LUXGL_TEXTURE_CUBEARRAY:
  case LUXGL_TEXTURE_2DARRAY:
    depth = tex->arraysize;
  case LUXGL_TEXTURE_3D:
    if (LUXGFX_VALIDITY && ( 
      tex->width < update->to.x + update->size.x || 
      tex->height < update->to.y + update->size.y ||
      depth < update->to.z + update->size.z))
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

LUX_API booln lxgTexture_readBuffer(lxgContextPTR ctx, lxgTexturePTR tex, 
    const lxgTextureUpdatePTR update,uint miplevel,
    enum lxScalarType_e scalar, const lxgBufferPTR buffer, uint bufferoffset)
{
  if (LUXGFX_VALIDITY && buffer->size < bufferoffset) return LUX_FALSE;

  lxgBuffer_apply(ctx,LUXGL_BUFFER_PIXELREAD,buffer);
  return lxgTexture_readData(ctx,tex,update,miplevel,scalar,(void*)bufferoffset,buffer->size - bufferoffset);
}

LUX_API booln lxgTexture_writeData(lxgContextPTR ctx, lxgTexturePTR tex, uint side, booln ascompressed, booln onlydepth, 
    uint mip, enum lxScalarType_e d, void* buffer, uint buffersize)
{
  GLenum target = tex->gltarget;
  GLenum dataformat = tex->gldataformat;
  GLenum datatype = tex->gldatatype;
  booln compressed = (tex->flags & (LUXGFX_TEXTUREFLAG_COMPRESSED | LUXGFX_TEXTUREFLAG_COMPRESS));
  int depth = tex->depth;
  size_t nativesize = tex->nativesizes[mip]; // CORRECT BASED ON MIP

  lxgTexture_checked(ctx,0,tex);

  if (LUXGFX_VALIDITY && (mip >= tex->miplevels ))
  {
    return LUX_FALSE;
  }

  if (tex->formattype == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL){
    dataformat = onlydepth ? GL_DEPTH_COMPONENT : GL_DEPTH_STENCIL;
  }

  if (d != LUX_SCALAR_ILLEGAL){
    datatype = lxScalarType_to(d);
  }

  switch(target){
  case LUXGL_TEXTURE_CUBE:
    target = GL_TEXTURE_CUBE_MAP_POSITIVE_X+side;
  case LUXGL_TEXTURE_1DARRAY:
  case LUXGL_TEXTURE_2DARRAY:
  case LUXGL_TEXTURE_CUBEARRAY:
  case LUXGL_TEXTURE_2D:
  case LUXGL_TEXTURE_RECT:
  case LUXGL_TEXTURE_2DMS:
  case LUXGL_TEXTURE_3D:
    if (!compressed || !ascompressed)
      glGetTexImage(target,mip,dataformat,datatype,buffer);
    else
      glGetCompressedTexImage(target,mip,buffer);
    break;
  }

  return LUX_TRUE;
}

LUX_API booln lxgTexture_writeBuffer(lxgContextPTR ctx, lxgTexturePTR tex, uint side,
    booln ascompressed, booln onlydepth, uint mip, enum lxScalarType_e d, lxgBufferPTR buffer, uint bufferoffset)
{
  if (LUXGFX_VALIDITY && buffer->size < bufferoffset) return LUX_FALSE;

  lxgBuffer_apply(ctx,LUXGL_BUFFER_PIXELWRITE,buffer);
  return lxgTexture_writeData(ctx,tex,side,ascompressed,onlydepth,mip,d,(void*)bufferoffset,buffer->size - bufferoffset);
}


LUX_API void lxgTextureUnit_setCompare(lxgContextPTR ctx, uint imageunit, lxGLCompareMode_t cmpfunc)
{
  lxgTexturePTR tex = ctx->textures[imageunit];
  booln run = cmpfunc != LUXGL_COMPARE_DONTEXECUTE;

  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_MODE_ARB,
    run ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);

  glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_FUNC_ARB,run ? cmpfunc : GL_LEQUAL);
  tex->sampler.cmpfunc = cmpfunc;
}

LUX_API LUX_INLINE void lxgTextureUnit_setSampler(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR sampler , flags32 what)
{
  static GLenum address[]= {
    LUXGL_SAMPLERADDRESS_REPEAT,
    LUXGL_SAMPLERADDRESS_MIRROR,
    LUXGL_SAMPLERADDRESS_CLAMP,
    LUXGL_SAMPLERADDRESS_BORDER,
  };
  lxgTexturePTR tex = ctx->textures[imageunit];

  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  
  if (what & LUXGFX_SAMPLERATTRIB_ADDRESS){
    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_WRAP_S,address[sampler->addru]);
    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_WRAP_T,address[sampler->addrv]);
    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_WRAP_R,address[sampler->addrw]);
  }

  if (what & LUXGFX_SAMPLERATTRIB_BORDER){
    glTexParameterfv(lxGLTARGET(tex),GL_TEXTURE_BORDER_COLOR,sampler->border);
  }
  
  if (what & (LUXGFX_SAMPLERATTRIB_CMP)){
    if (tex->flags & LUXGFX_TEXTUREFLAG_HASCOMPARE){
      booln run = sampler->cmpfunc != LUXGL_COMPARE_DONTEXECUTE;
      glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_MODE_ARB,
         run ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);

      glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_FUNC_ARB,run ? sampler->cmpfunc : GL_LEQUAL);
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_ANISO){
    if (sampler->aniso){
      glTexParameteri(lxGLTARGET(tex), GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler->aniso);
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_LOD){
    if (tex->flags & LUXGFX_TEXTUREFLAG_HASLOD){
      glTexParameterf(lxGLTARGET(tex),GL_TEXTURE_MIN_LOD,sampler->lod.min);
      glTexParameterf(lxGLTARGET(tex),GL_TEXTURE_MAX_LOD,sampler->lod.max);

      glTexParameterf(lxGLTARGET(tex),GL_TEXTURE_LOD_BIAS_EXT,sampler->lod.bias);
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_FILTER){
    GLenum minfilter;
    GLenum magfilter;

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

    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_MIN_FILTER,minfilter);
    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_MAG_FILTER,magfilter);
  }

  tex->lastSampler = sampler;
  tex->lastSamplerIncarnation = sampler->incarnation;
  tex->sampler = *sampler;
  tex->sampler.glid = 0;
}

LUX_API void lxgTextureUnit_checkedSampler(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR sampler, flags32 what){
  lxgTexturePTR tex = ctx->textures[imageunit];
  flags32 change = 0;

  if (tex->lastSampler == sampler 
      && sampler->incarnation == tex->lastSamplerIncarnation) return;

  change |= LUXGFX_SAMPLERATTRIB_ANISO * !!(what & LUXGFX_SAMPLERATTRIB_ANISO && sampler->aniso != tex->sampler.aniso);
  change |= LUXGFX_SAMPLERATTRIB_CMP * !!(what & LUXGFX_SAMPLERATTRIB_CMP && sampler->cmpfunc != tex->sampler.cmpfunc);
  change |= LUXGFX_SAMPLERATTRIB_FILTER * !!(what & LUXGFX_SAMPLERATTRIB_FILTER && sampler->filter != tex->sampler.filter);
  change |= LUXGFX_SAMPLERATTRIB_ADDRESS * !!((what & LUXGFX_SAMPLERATTRIB_ADDRESS && 
    (sampler->addru != tex->sampler.addru || sampler->addrv != tex->sampler.addrv || sampler->addrw != tex->sampler.addrw)));
  change |= LUXGFX_SAMPLERATTRIB_BORDER * !!(what & LUXGFX_SAMPLERATTRIB_BORDER && 
    memcmp(tex->sampler.border,sampler->border,sizeof(sampler->border)) == 0);
  change |= LUXGFX_SAMPLERATTRIB_LOD * !!(what & LUXGFX_SAMPLERATTRIB_LOD && 
    memcmp(&tex->sampler.lod,&sampler->lod,sizeof(sampler->lod)) == 0);

  if (change){
    lxgTextureUnit_setSampler(ctx,imageunit,sampler,change);
  }
}




//////////////////////////////////////////////////////////////////////////
// lxgSampler_t

LUX_API void lxgSampler_init( lxgSamplerPTR self )
{
  self->glid = 0;
  self->filter = LUXGFX_SAMPLERFILTER_LINEAR;
  self->addru = LUXGFX_SAMPLERADDRESS_REPEAT;
  self->addrv = LUXGFX_SAMPLERADDRESS_REPEAT;
  self->addrw = LUXGFX_SAMPLERADDRESS_REPEAT;
  self->aniso = 1;
  self->lod.min = -1000.0f;
  self->lod.max = 1000.0f;
  self->lod.bias = 0.0f;
  self->cmpfunc = LUXGFX_COMPARE_LEQUAL;
  LUX_ARRAY4SET(self->border,0,0,0,0);
}


LUX_API void lxgSampler_setAddress(lxgSamplerPTR self, uint n, lxgSamplerAddress_t address)
{
  LUX_DEBUGASSERT(n <= 3 && address <= LUXGFX_SAMPLERADDRESSES);
  switch (n)
  {
  case 0: self->addru = address; return;
  case 1: self->addrv = address; return;
  case 2: self->addrw = address; return;
  }
}

LUX_API void lxgSampler_setCompare(lxgSamplerPTR self, lxgCompareMode_t cmp)
{
  self->cmpfunc = lxGLCompareMode_get(cmp);
}

LUX_API void lxgSampler_changed(lxgSamplerPTR self)
{
  self->incarnation++;
}

LUX_API void lxgSampler_initHW(lxgContextPTR ctx, lxgSamplerPTR self)
{
  lxgSampler_init(self);
  glGenSamplers(1,&self->glid);
}

LUX_API void lxgSampler_deinitHW(lxgContextPTR ctx, lxgSamplerPTR self)
{
  if (self->glid) glDeleteSamplers(1,&self->glid);
}

LUX_API void lxgSampler_updateHW(lxgContextPTR ctx, lxgSamplerPTR sampler)
{
  static GLenum address[] = {
    LUXGL_SAMPLERADDRESS_REPEAT,
    LUXGL_SAMPLERADDRESS_MIRROR,
    LUXGL_SAMPLERADDRESS_CLAMP,
    LUXGL_SAMPLERADDRESS_BORDER,
  };

  GLuint glid = sampler->glid;
  GLenum minfilter;
  GLenum magfilter;
  booln runcmp = sampler->cmpfunc != LUXGL_COMPARE_DONTEXECUTE;

  glSamplerParameteri(glid,GL_TEXTURE_WRAP_S,address[sampler->addru]);
  glSamplerParameteri(glid,GL_TEXTURE_WRAP_T,address[sampler->addrv]);
  glSamplerParameteri(glid,GL_TEXTURE_WRAP_R,address[sampler->addrw]);

  glSamplerParameterfv(glid,GL_TEXTURE_BORDER_COLOR,sampler->border);

  glSamplerParameteri(glid,GL_TEXTURE_COMPARE_MODE_ARB,
    runcmp ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);
  glSamplerParameteri(glid,GL_TEXTURE_COMPARE_FUNC_ARB,
    runcmp ? lxGLCompareMode_get(sampler->cmpfunc) : GL_LEQUAL);

  glSamplerParameteri(glid, GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler->aniso);

  glSamplerParameterf(glid,GL_TEXTURE_MIN_LOD,sampler->lod.min);
  glSamplerParameterf(glid,GL_TEXTURE_MAX_LOD,sampler->lod.max);
  glSamplerParameterf(glid,GL_TEXTURE_LOD_BIAS_EXT,sampler->lod.bias);

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

  glSamplerParameteri(glid,GL_TEXTURE_MIN_FILTER,minfilter);
  glSamplerParameteri(glid,GL_TEXTURE_MAG_FILTER,magfilter);
}

LUX_API void lxgSampler_apply(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR self)
{
  ctx->samplers[imageunit] = self;
  if (self->glid) glBindSampler(imageunit,self->glid);
}

LUX_API void  lxgSamplers_apply(lxgContextPTR ctx, uint start, uint num, lxgSamplerPTR *samps)
{
  uint i;
  for (i = 0; i < num; i++){
    ctx->samplers[i+start] = samps[i];
    if (samps[i]->glid) glBindSampler(i+start,samps[i]->glid);
  }
}

//////////////////////////////////////////////////////////////////////////
// lxgRenderBuffer

LUX_API booln lxgRenderBuffer_init(lxgContextPTR ctx, lxgRenderBufferPTR rb, lxgTextureChannel_t format,int width, int height, int samples)
{
  rb->ctx = ctx;
  glGenRenderbuffersEXT(1,&rb->glid);
  return lxgRenderBuffer_change(ctx,rb,format,width,height,samples);
}

LUX_API booln lxgRenderBuffer_change(lxgContextPTR ctx, lxgRenderBufferPTR rb,
   lxgTextureChannel_t format, int width, int height, int samples)
{
  if (!lxgTextureChannel_valid(ctx,format) || samples > ctx->capabilites.fbosamples)
    return LUX_FALSE;

  rb->formattype = format;
  rb->width = width;
  rb->height = height;
  rb->samples = samples;
  
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb->glid);
  if (samples)
    glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, GetInternal(format,LUXGFX_TEXTUREDATA_BASE), width, height);
  else
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GetInternal(format,LUXGFX_TEXTUREDATA_BASE), width, height);

  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  return LUX_TRUE;
}

LUX_API void  lxgRenderBuffer_deinit(lxgContextPTR ctx, lxgRenderBufferPTR rb)
{
  glDeleteRenderbuffersEXT(1,&rb->glid);
}

//////////////////////////////////////////////////////////////////////////

LUX_API void  lxgTextureImage_apply(lxgContextPTR ctx, uint imageunit, lxgTextureImagePTR img)
{
  glBindImageTextureEXT(imageunit,img->tex->glid,img->level,img->layered,img->layer,img->glaccess,img->glformat);
  ctx->images[imageunit] = img;
}


typedef enum lxgAccessFormat_e{
  LUXGFX_ACCESSFORMAT_R8UI,
  LUXGFX_ACCESSFORMAT_R8I,
  LUXGFX_ACCESSFORMAT_R8F,

  LUXGFX_ACCESSFORMAT_R16UI,
  LUXGFX_ACCESSFORMAT_R16I,
  LUXGFX_ACCESSFORMAT_R16F,

  LUXGFX_ACCESSFORMAT_R32UI,
  LUXGFX_ACCESSFORMAT_R32I,
  LUXGFX_ACCESSFORMAT_R32F,

  LUXGFX_ACCESSFORMAT_RG32UI,
  LUXGFX_ACCESSFORMAT_RG32I,
  LUXGFX_ACCESSFORMAT_RG32F,

  LUXGFX_ACCESSFORMAT_RGBA32UI,
  LUXGFX_ACCESSFORMAT_RGBA32I,
  LUXGFX_ACCESSFORMAT_RGBA32F,
}lxgAccessFormat_t;


typedef enum lxGLAccessFormat_e{
  LUXGL_ACCESSFORMAT_R8UI     = GL_R8UI,
  LUXGL_ACCESSFORMAT_R8I      = GL_R8I,

  LUXGL_ACCESSFORMAT_R16UI    = GL_R16UI,
  LUXGL_ACCESSFORMAT_R16I     = GL_R16I,
  
  LUXGL_ACCESSFORMAT_R32UI    = GL_R32UI,
  LUXGL_ACCESSFORMAT_R32I     = GL_R32I,
  LUXGL_ACCESSFORMAT_R32F     = GL_R32F,

  LUXGL_ACCESSFORMAT_RG32UI   = GL_RG32UI,
  LUXGL_ACCESSFORMAT_RG32I    = GL_RG32I,
  LUXGL_ACCESSFORMAT_RG32F    = GL_RG32F,
  
  LUXGL_ACCESSFORMAT_RGBA32UI = GL_RGBA32UI,
  LUXGL_ACCESSFORMAT_RGBA32I  = GL_RGBA32I,
  LUXGL_ACCESSFORMAT_RGBA32F  = GL_RGBA32F,
}lxGLAccessFormat_t;

LUX_API booln lxgTextureImage_init( lxgTextureImagePTR img, lxgTexturePTR tex, lxgAccessMode_t access, uint level, booln layered, int layer )
{
  static GLenum modes[] = {
    LUXGL_ACCESS_READ,
    LUXGL_ACCESS_WRITE,
    LUXGL_ACCESS_READWRITE,

    LUXGL_ACCESS_WRITEDISCARD,
    LUXGL_ACCESS_WRITEDISCARDALL,
  };

  static lxGLAccessFormat_t formats[] = {
    LUXGL_ACCESSFORMAT_R8UI,
    LUXGL_ACCESSFORMAT_R8I,
    0,

    LUXGL_ACCESSFORMAT_R16UI,
    LUXGL_ACCESSFORMAT_R16I,
    0,

    LUXGL_ACCESSFORMAT_R32UI,
    LUXGL_ACCESSFORMAT_R32I,
    LUXGL_ACCESSFORMAT_R32F,

    LUXGL_ACCESSFORMAT_RG32UI,
    LUXGL_ACCESSFORMAT_RG32I,
    LUXGL_ACCESSFORMAT_RG32F,

    LUXGL_ACCESSFORMAT_RGBA32UI, 
    LUXGL_ACCESSFORMAT_RGBA32I,
    LUXGL_ACCESSFORMAT_RGBA32F,
  };
  lxgAccessFormat_t base;

  if (level >= tex->miplevels){
    LUX_DEBUGASSERT( 0 && "illegal texture level");
    return LUX_FALSE;
  }

  if ((layered || layer) && !tex->arraysize){
    LUX_DEBUGASSERT( 0 && "texture has no layers");
    return LUX_FALSE;
  }

  switch(tex->formattype){
  case LUXGFX_TEXTURECHANNEL_R:
  case LUXGFX_TEXTURECHANNEL_RG:
  case LUXGFX_TEXTURECHANNEL_RGBA:
  case LUXGFX_TEXTURECHANNEL_SRGBA:
  case LUXGFX_TEXTURECHANNEL_DEPTH:
    break;
  default:
    LUX_DEBUGASSERT( 0 && "illegal texture channel format");
    return LUX_FALSE;
  }

  switch(tex->componentsize * tex->components){
  case 8:
    base = LUXGFX_ACCESSFORMAT_R8UI;
  case 16:
    base = LUXGFX_ACCESSFORMAT_R16UI;
  case 32:
    base = LUXGFX_ACCESSFORMAT_R32UI;
  case 64:
    base = LUXGFX_ACCESSFORMAT_RG32UI;
  case 128:
    base = LUXGFX_ACCESSFORMAT_RGBA32UI;
  default:
    LUX_DEBUGASSERT( 0 && "illegal texel size");
    return LUX_FALSE;
  }

  switch(tex->datatype){
  case LUXGFX_TEXTUREDATA_UNORM8:
  case LUXGFX_TEXTUREDATA_UNORM16:
  case LUXGFX_TEXTUREDATA_UINT8:
  case LUXGFX_TEXTUREDATA_UINT16:
  case LUXGFX_TEXTUREDATA_UINT32:
  case LUXGFX_TEXTUREDATA_FLOAT16:
    // unsigned int
    break;
  case LUXGFX_TEXTUREDATA_SNORM8:
  case LUXGFX_TEXTUREDATA_SNORM16:
  case LUXGFX_TEXTUREDATA_SINT8:
  case LUXGFX_TEXTUREDATA_SINT16:
  case LUXGFX_TEXTUREDATA_SINT32:
    // signed int
    base += 1;
    break;
  case LUXGFX_TEXTUREDATA_FLOAT32:
    // float
    base += 2;
    break;
  default:
    LUX_DEBUGASSERT( 0 && "illegal datatype");
    return LUX_FALSE;
  }
  
  img->tex = tex;
  img->level = level;
  img->layered = layered;
  img->layer = layer;
  img->glaccess = modes[access];
  img->glformat = formats[base];

  return !!(img->glformat);
}

