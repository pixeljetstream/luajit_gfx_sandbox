// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

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


LUX_API GLenum  lxgTextureDataType_getData(lxgTextureDataType_t data, booln rev, booln depthstencil)
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

LUX_API GLenum  lxgTextureChannel_getFormat(lxgTextureChannel_t type, booln rev, booln integer)
{
  switch(type){
  case LUXGFX_TEXTURECHANNEL_RGB:
    if (integer) return rev ? GL_BGR_INTEGER : GL_RGB_INTEGER;
    else return rev ? GL_BGR : GL_RGB;

  case LUXGFX_TEXTURECHANNEL_RGBA:
    if (integer) return rev ? GL_BGRA_INTEGER : GL_RGBA_INTEGER;
    else return rev ? GL_BGRA : GL_RGBA;

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

LUX_API GLenum  lxgTextureChannel_getInternal(lxgTextureChannel_t type, lxgTextureDataType_t data)
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
  tex->gldataformat = lxgTextureChannel_getFormat(format,LUX_FALSE,data > LUXGFX_TEXTUREDATA_FLOAT32);
  tex->gldatatype = lxgTextureDataType_getData(data,LUX_FALSE,format == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL);
  tex->glinternalformat = lxgTextureChannel_getInternal(format,data);
  if (!tex->gldataformat || !tex->gldatatype || !tex->glinternalformat)
    return LUX_FALSE;

  return LUX_TRUE;
}

//////////////////////////////////////////////////////////////////////////
// lxgTexture

static LUX_INLINE void lxgTexture_bindDefault(lxgTexturePTR tex)
{
  glBindTexture(tex->gltarget,tex->glid);
  lxGLErrorCheck();
}

static LUX_INLINE void lxgTexture_unbindDefault(lxgTexturePTR tex)
{
  glBindTexture(tex->gltarget,0);
  lxGLErrorCheck();
}

LUX_API LUX_INLINE void lxgContext_applyTexture( lxgContextPTR ctx,   lxgTexturePTR tex, uint imageunit)
{
  lxgTexturePTR old = ctx->textures[imageunit];
  LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  if (tex){
    glBindTexture(tex->gltarget,tex->glid);
  }
  else if (old){
    glBindTexture(old->gltarget,0);
  }
  lxGLErrorCheck();
  ctx->textures[imageunit] = tex;
  if (!(ctx->capbits & LUXGFX_CAP_SM3)){
    lxgContext_checkedTextureSampler(ctx,imageunit);
  }
}

LUX_API void  lxgContext_applyTextures( lxgContextPTR ctx, lxgTexturePTR *texs, uint start, uint num)
{
  int i;
  for (i = num-1; i >= 0; i--){
    lxgContext_applyTexture(ctx, texs[i], i);
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
  lxGLErrorCheck();
}
static void TexUpload_1DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage1DARB(tu->target,tu->level,tu->format,tu->size.x,tu->border,tu->compressedsize,NULL);
  lxGLErrorCheck();
}
static void TexUpload_2D(TexUpload_t*  tu)
{
  glTexImage2D(tu->target,tu->level,tu->internal,tu->size.x,tu->size.y,tu->border,tu->format,tu->type,NULL);
  lxGLErrorCheck();
}
static void TexUpload_2DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage2DARB(tu->target,tu->level,tu->format,tu->size.x,tu->size.y,tu->border,tu->compressedsize,NULL);
  lxGLErrorCheck();
}
static void TexUpload_3D(TexUpload_t*  tu)
{
  glTexImage3D(tu->target,tu->level,tu->internal,tu->size.x,tu->size.y,tu->size.z,tu->border,tu->format,tu->type,NULL);
  lxGLErrorCheck();
}
static void TexUpload_3DCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage3DARB(tu->target,tu->level,tu->format,tu->size.x,tu->size.y,tu->size.z,tu->border,tu->compressedsize,NULL);
  lxGLErrorCheck();
}
static void TexUpload_1DARRAY(TexUpload_t*  tu)
{
  glTexImage2D(tu->target,tu->level,tu->internal,tu->size.x,tu->arraysize,tu->border,tu->format,tu->type,NULL);
  lxGLErrorCheck();
}
static void TexUpload_1DARRAYCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage2DARB(tu->target,tu->level,tu->format,tu->size.x,tu->arraysize,tu->border,tu->compressedsize,NULL);
  lxGLErrorCheck();
}
static void TexUpload_2DARRAY(TexUpload_t*  tu)
{
  glTexImage3D(tu->target,tu->level,tu->internal,tu->size.x,tu->size.y,tu->arraysize,tu->border,tu->format,tu->type,NULL);
  lxGLErrorCheck();
}
static void TexUpload_2DARRAYCompressed(TexUpload_t*  tu)
{
  glCompressedTexImage3DARB(tu->target,tu->level,tu->format,tu->size.x,tu->size.y,tu->arraysize,tu->border,tu->compressedsize,NULL);
  lxGLErrorCheck();
}
static void TexUpload_2DMS(TexUpload_t*  tu)
{
  glTexImage2DMultisample(tu->target,tu->samples,tu->internal,tu->size.x,tu->size.y,tu->samplesfixed);
  lxGLErrorCheck();
}
static void TexUpload_2DMSARRAY(TexUpload_t*  tu)
{
  glTexImage3DMultisample(tu->target,tu->samples,tu->internal,tu->size.x,tu->size.y,tu->arraysize,tu->samplesfixed);
  lxGLErrorCheck();
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

static void lxgTexture_updateSizes(lxgTexturePTR tex, int width, int height, int depth, int arraysize, int samples)
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

  lxgTexture_bindDefault(tex);
  lxgBuffer_bind(NULL,LUXGL_BUFFER_PIXELREAD);
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
  lxgTexture_unbindDefault(tex);
}

LUX_API booln lxgTexture_resize(lxgTexturePTR tex, int width, int height, int depth, int arraysize)
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

  lxgTexture_updateSizes(tex,width,height,depth,arraysize,depth);

  return LUX_TRUE;
}

LUX_API void lxgTexture_init(lxgTexturePTR tex, lxgContextPTR ctx)
{
  memset(tex,0,sizeof(lxgTexture_t));
  tex->ctx = ctx;
  glGenTextures(1,&tex->glid);
}

LUX_API booln lxgTexture_setup(lxgTexturePTR tex,
  lxGLTextureTarget_t type, lxgTextureChannel_t format, lxgTextureDataType_t data,
  int w, int h, int d, int arraysize, flags32 flags)
{
  booln compressed = flags & LUXGFX_TEXTUREFLAG_COMPRESSED;
  lxgContextPTR ctx = tex->ctx;

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
    tex->gldatatype = lxgTextureDataType_getData(data,LUX_FALSE,LUX_FALSE);
    tex->gldataformat = lxgTextureChannel_getFormat(format,LUX_FALSE,LUX_FALSE);
  }

  // convert to GL internalformat
  if (!lxGLTextureData_init(ctx,tex,format,data)){
    return LUX_FALSE;
  }

  if (!tex->glid){
    glGenTextures(1,&tex->glid);
  }
  tex->gltarget = type;

  lxgTexture_bindDefault(tex);
  if (flags & LUXGFX_TEXTUREFLAG_AUTOMIPMAP){
    glTexParameteri(lxGLTARGET(tex),GL_GENERATE_MIPMAP,GL_TRUE);
    lxGLErrorCheck();
  }


  tex->flags = flags;
  tex->formattype = format;
  tex->datatype = data;
  tex->components = GetComponents(format);

  lxgTexture_updateSizes(tex,w,h,d,arraysize,d);

  lxgTexture_bindDefault(tex);
  tex->flags |= (ctx->capbits & LUXGFX_CAP_SM4) ? LUXGFX_TEXTUREFLAG_HASLOD : 0;
  lxgSampler_init(&tex->sampler,ctx);
  lxgSampler_deinit(&tex->sampler,ctx); // we actually don't want a gl-object for sampler state
  if (format == LUXGFX_TEXTURECHANNEL_DEPTH || format == LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL){
    glTexParameteri(lxGLTARGET(tex),GL_DEPTH_TEXTURE_MODE,GL_LUMINANCE);
    tex->sampler.filter = (lxgSamplerFilter_t)((uint)tex->sampler.filter+4);
    tex->flags |= LUXGFX_TEXTUREFLAG_HASCOMPARE;
    lxGLErrorCheck();
  }
  
  lxgTexture_boundSetSampler(tex,&tex->sampler,LUXGFX_SAMPLERATTRIB_ALL);
  lxgTexture_unbindDefault(tex);

  return LUX_TRUE;
}

LUX_API lxVec3iCPTR  lxgTexture_getMipSize(lxgTextureCPTR tex, uint mipLevel)
{
  return &tex->mipsizes[mipLevel];
}

LUX_API void lxgTexture_deinit( lxgTexturePTR tex,lxgContextPTR ctx )
{
  glDeleteTextures(1,&tex->glid);
  tex->glid = 0;
}


LUX_API booln lxgTexture_readFrame(lxgTexturePTR tex, lxgContextPTR ctx,
  const lxgTextureUpdate_t* update, uint mip)
{
  GLenum target = tex->gltarget;
  int depth = tex->depth;

  lxgTexture_bindDefault(tex);
  
  switch(target){
    case LUXGL_TEXTURE_1D:
      if (LUXGFX_VALIDITY &&(mip >= tex->miplevels ||
        tex->width < update->to.x + update->size.x))
      {
        lxgTexture_unbindDefault(tex);
        return LUX_FALSE;
      }
      glCopyTexSubImage1D(target,mip,update->to.x,update->from.x,update->from.y,update->size.x);
      lxGLErrorCheck();
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
        lxgTexture_unbindDefault(tex);
        return LUX_FALSE;
      }
      glCopyTexSubImage2D(target,mip,update->to.x,update->to.y,update->from.x,update->from.y,update->size.x,update->size.y);
      lxGLErrorCheck();
      break;
    case LUXGL_TEXTURE_1DARRAY:
      if (LUXGFX_VALIDITY && (mip >= tex->miplevels ||
        tex->width < update->to.x + update->size.x || 
        tex->arraysize < update->to.y + update->size.y))
      {
        lxgTexture_unbindDefault(tex);
        return LUX_FALSE;
      }
      glCopyTexSubImage2D(target,mip,update->to.x,update->to.y,update->from.x,update->from.y,update->size.x,update->size.y);
      lxGLErrorCheck();
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
        lxgTexture_unbindDefault(tex);
        return LUX_FALSE;
      }
      glCopyTexSubImage3D(target,mip,update->to.x,update->to.y,update->to.z,update->from.x,update->from.y,update->size.x,update->size.y);
      lxGLErrorCheck();
      break;
  }

  lxgTexture_unbindDefault(tex);
  return LUX_TRUE;
}

// if scalartype is set to invalid, we assume "native" data (matching
// internal)
LUX_API booln lxgTexture_readData(lxgTexturePTR tex,
    const lxgTextureUpdate_t* update,uint mip,
    GLenum datatype, GLenum dataformat, const void* data, uint size)
{
  GLenum target = tex->gltarget;
  booln compressed = (tex->flags & LUXGFX_TEXTUREFLAG_COMPRESSED);
  int depth = tex->depth;
  size_t nativesize = tex->nativesizes[mip];
  
  if (LUXGFX_VALIDITY && (mip >= tex->miplevels) || (compressed && 
    (update->to.x%4 != 0  || update->to.y%4 != 0 ||
     update->size.x%4 != 0 || update->size.y%4 != 0))
     || !(tex->mipsdefined & (1<<mip)))
  {
    return LUX_FALSE;
  }

  lxgTexture_bindDefault(tex);
  switch(target){
  case LUXGL_TEXTURE_1D:
    if (LUXGFX_VALIDITY && (
      tex->width < update->to.x+update->size.x))
    {
      lxgTexture_unbindDefault(tex);
      return LUX_FALSE;
    }
    if (!compressed)
      glTexSubImage1D(target,mip,update->to.x,update->size.x,dataformat,datatype,data);
    else
      glCompressedTexSubImage1D(target,mip,update->to.x,update->size.x,dataformat,nativesize,data);
    lxGLErrorCheck();
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
      lxgTexture_unbindDefault(tex);
      return LUX_FALSE;
    }
    if (!compressed)
      glTexSubImage2D(target,mip,update->to.x,update->to.y,update->size.x,update->size.y,dataformat,datatype,data);
    else
      glCompressedTexSubImage2D(target,mip,update->to.x,update->to.y,update->size.x,update->size.y,dataformat,nativesize,data);
    lxGLErrorCheck();
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
      lxgTexture_unbindDefault(tex);
      return LUX_FALSE;
    }
    if (!compressed)
      glTexSubImage3D(target,mip,update->to.x,update->to.y,update->to.z,update->size.x,update->size.y,update->size.z,dataformat,datatype,data);
    else
      glCompressedTexSubImage3D(target,mip,update->to.x,update->to.y,update->to.z,update->size.x,update->size.y,update->size.z,dataformat,nativesize,data);
    lxGLErrorCheck();
    break;
  }

  lxgTexture_unbindDefault(tex);

  return LUX_TRUE;

}

LUX_API booln lxgTexture_readBuffer(lxgTexturePTR tex, 
    const lxgTextureUpdate_t* update,uint miplevel,
    GLenum datatype, GLenum dataformat, const lxgBuffer_t* buffer, uint bufferoffset)
{
  if (LUXGFX_VALIDITY && buffer->size < bufferoffset) return LUX_FALSE;

  lxgBuffer_bind(buffer,LUXGL_BUFFER_PIXELREAD);
  return lxgTexture_readData(tex,update,miplevel,datatype,dataformat,(void*)bufferoffset,buffer->size - bufferoffset);
}

LUX_API booln lxgTexture_writeData(lxgTexturePTR tex, uint side, booln ascompressed, 
    uint mip, GLenum datatype, GLenum dataformat, void* buffer, uint buffersize)
{
  GLenum target = tex->gltarget;
  booln compressed = (tex->flags & (LUXGFX_TEXTUREFLAG_COMPRESSED | LUXGFX_TEXTUREFLAG_COMPRESS));
  int depth = tex->depth;
  size_t nativesize = tex->nativesizes[mip]; // CORRECT BASED ON MIP

  if (LUXGFX_VALIDITY && (mip >= tex->miplevels ))
  {
    return LUX_FALSE;
  }

  lxgTexture_bindDefault(tex);

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
    lxGLErrorCheck();
    break;
  }

  lxgTexture_unbindDefault(tex);

  return LUX_TRUE;
}

LUX_API booln lxgTexture_writeBuffer(lxgTexturePTR tex, uint side,
    booln ascompressed, uint mip, GLenum datatype, GLenum dataformat, lxgBufferPTR buffer, uint bufferoffset)
{
  if (LUXGFX_VALIDITY && buffer->size < bufferoffset) return LUX_FALSE;

  lxgBuffer_bind(buffer,LUXGL_BUFFER_PIXELWRITE);
  return lxgTexture_writeData(tex,side,ascompressed,mip,datatype,dataformat,(void*)bufferoffset,buffer->size - bufferoffset);
}

LUX_API LUX_INLINE void lxgTexture_boundSetSampler(lxgTexturePTR tex, lxgSamplerCPTR sampler, flags32 what)
{
  static GLenum address[]= {
    LUXGL_SAMPLERADDRESS_REPEAT,
    LUXGL_SAMPLERADDRESS_MIRROR,
    LUXGL_SAMPLERADDRESS_CLAMP,
    LUXGL_SAMPLERADDRESS_BORDER,
  };

  if (what & LUXGFX_SAMPLERATTRIB_ADDRESS){
    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_WRAP_S,address[sampler->addru]);
    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_WRAP_T,address[sampler->addrv]);
    glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_WRAP_R,address[sampler->addrw]);
    lxGLErrorCheck();
  }

  if (what & LUXGFX_SAMPLERATTRIB_BORDER){
    glTexParameterfv(lxGLTARGET(tex),GL_TEXTURE_BORDER_COLOR,sampler->border);
    lxGLErrorCheck();
  }

  if (what & (LUXGFX_SAMPLERATTRIB_CMP)){
    if (tex->flags & LUXGFX_TEXTUREFLAG_HASCOMPARE){
      booln run = sampler->cmpfunc != LUXGL_COMPARE_DONTEXECUTE;
      glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_MODE_ARB,
        run ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);

      glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_FUNC_ARB,run ? sampler->cmpfunc : GL_LEQUAL);
      lxGLErrorCheck();
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_ANISO){
    if (sampler->aniso){
      glTexParameteri(lxGLTARGET(tex), GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler->aniso);
      lxGLErrorCheck();
    }
  }

  if (what & LUXGFX_SAMPLERATTRIB_LOD){
    if (tex->flags & LUXGFX_TEXTUREFLAG_HASLOD){
      glTexParameterf(lxGLTARGET(tex),GL_TEXTURE_MIN_LOD,sampler->lod.min);
      glTexParameterf(lxGLTARGET(tex),GL_TEXTURE_MAX_LOD,sampler->lod.max);

      glTexParameterf(lxGLTARGET(tex),GL_TEXTURE_LOD_BIAS_EXT,sampler->lod.bias);
      lxGLErrorCheck();
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
    lxGLErrorCheck();
  }

  tex->lastSampler = sampler;
  tex->lastSamplerIncarnation = sampler->incarnation;
  tex->sampler = *sampler;
  tex->sampler.glid = 0;

}

//////////////////////////////////////////////////////////////////////////


LUX_API void lxgContext_clearTextureState(lxgContextPTR ctx)
{
  memset((void*)ctx->textures,0,sizeof(lxgTexturePTR)*LUXGFX_MAX_TEXTURE_IMAGES);
  memset((void*)ctx->images,0,sizeof(lxgTextureImageCPTR)*LUXGFX_MAX_TEXTURE_IMAGES);
  memset((void*)ctx->samplers,0,sizeof(lxgSamplerCPTR)*LUXGFX_MAX_TEXTURE_IMAGES);
}

LUX_API void lxgContext_setTextureCompare(lxgContextPTR ctx, uint imageunit, lxGLCompareMode_t cmp)
{
  lxgTexturePTR tex = ctx->textures[imageunit];
  booln run = cmp != LUXGL_COMPARE_DONTEXECUTE;

  LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
  LUX_DEBUGASSERT(tex);

  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_MODE_ARB,
    run ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);

  glTexParameteri(lxGLTARGET(tex),GL_TEXTURE_COMPARE_FUNC_ARB,run ? cmp : GL_LEQUAL);
  tex->sampler.cmpfunc = cmp;
  tex->sampler.incarnation++;
  tex->lastSampler = &tex->sampler;
  tex->lastSamplerIncarnation = tex->sampler.incarnation;
}


LUX_API void lxgContext_setTextureSampler(lxgContextPTR ctx, uint imageunit, flags32 what)
{
  lxgTexturePTR tex = ctx->textures[imageunit];
  lxgSamplerCPTR sampler = ctx->samplers[imageunit];

  LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
  LUX_DEBUGASSERT(tex && sampler);

  glActiveTexture(GL_TEXTURE0_ARB + imageunit);
  lxgTexture_boundSetSampler(tex,sampler,what);
}

LUX_API void lxgContext_changedTextureSampler(lxgContextPTR ctx, uint imageunit, flags32 what){
  lxgTexturePTR tex = ctx->textures[imageunit];
  lxgSamplerCPTR sampler = ctx->samplers[imageunit];
  flags32 change = 0;

  LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
  LUX_DEBUGASSERT(tex && sampler);

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
    glActiveTexture(GL_TEXTURE0_ARB + imageunit);
    lxgTexture_boundSetSampler(tex,sampler,change);
  }
}

LUX_API void lxgTexture_getSampler( lxgTextureCPTR tex, lxgSamplerPTR sampler )
{
  GLuint glid = sampler->glid;
  uint32 incarnation = sampler->incarnation;
  memcpy(sampler, &tex->sampler, sizeof(lxgSampler_t));
  sampler->glid = glid;
  sampler->incarnation = incarnation;
  lxgSampler_update(sampler);
}



//////////////////////////////////////////////////////////////////////////
// lxgSampler_t

LUX_API void lxgSampler_init( lxgSamplerPTR self, lxgContextPTR ctx )
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
  self->cmpfunc = LUXGL_COMPARE_LEQUAL;
  LUX_ARRAY4SET(self->border,0,0,0,0);
  if (ctx->capbits & LUXGFX_CAP_SM3){
    glGenSamplers(1,&self->glid);
  }
}

LUX_API void lxgSampler_deinit(lxgSamplerPTR self, lxgContextPTR ctx)
{
  if (self->glid) glDeleteSamplers(1,&self->glid);
  self->glid = 0;
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

LUX_API void lxgSampler_setCompare(lxgSamplerPTR self, lxGLCompareMode_t cmp)
{
  self->cmpfunc = (cmp);
}

static void lxgSampler_updateHW(lxgSamplerPTR sampler)
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

  lxGLErrorCheck();
  glSamplerParameteri(glid,GL_TEXTURE_WRAP_S,address[sampler->addru]);
  glSamplerParameteri(glid,GL_TEXTURE_WRAP_T,address[sampler->addrv]);
  glSamplerParameteri(glid,GL_TEXTURE_WRAP_R,address[sampler->addrw]);
  
  glSamplerParameterfv(glid,GL_TEXTURE_BORDER_COLOR,sampler->border);

  glSamplerParameteri(glid,GL_TEXTURE_COMPARE_MODE_ARB,
    runcmp ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);
  glSamplerParameteri(glid,GL_TEXTURE_COMPARE_FUNC_ARB,
    runcmp ? (sampler->cmpfunc) : GL_LEQUAL);

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
  lxGLErrorCheck();
}

LUX_API void lxgSampler_update(lxgSamplerPTR sampler)
{
  sampler->incarnation++;
  if (sampler->glid){
    lxgSampler_updateHW(sampler);
  }
}


LUX_API LUX_INLINE void lxgContext_applySampler( lxgContextPTR ctx, lxgSamplerCPTR sampler, uint imageunit)
{
  LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_TEXTURE_IMAGES);
  ctx->samplers[imageunit] = sampler;
  
  if (sampler){
    if (sampler->glid){
      glBindSampler(imageunit,sampler->glid);
    }
  }
  else if (ctx->capbits & LUXGFX_CAP_SM4){
    glBindSampler(imageunit,0);
  }
  lxGLErrorCheck();
}

LUX_API void  lxgContext_applySamplers( lxgContextPTR ctx, lxgSamplerCPTR *samps, uint start, uint num)
{
  uint i;
  for (i = 0; i < num; i++){
    lxgContext_applySampler(ctx, samps[i], start + i);
  }
}

//////////////////////////////////////////////////////////////////////////
// lxgRenderBuffer

LUX_API booln lxgRenderBuffer_init(lxgRenderBufferPTR rb, lxgContextPTR ctx, lxgTextureChannel_t format,int width, int height, int samples)
{
  memset(rb,0,sizeof(lxgRenderBuffer_t));
  rb->ctx = ctx;
  glGenRenderbuffersEXT(1,&rb->glid);
  return lxgRenderBuffer_change(rb,format,width,height,samples);
}

LUX_API booln lxgRenderBuffer_change(lxgRenderBufferPTR rb,
   lxgTextureChannel_t format, int width, int height, int samples)
{
  if (!lxgTextureChannel_valid(rb->ctx,format) || samples > rb->ctx->capabilites.fbosamples)
    return LUX_FALSE;

  rb->formattype = format;
  rb->width = width;
  rb->height = height;
  rb->samples = samples;
  
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb->glid);
  if (samples)
    glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, lxgTextureChannel_getInternal(format,LUXGFX_TEXTUREDATA_BASE), width, height);
  else
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, lxgTextureChannel_getInternal(format,LUXGFX_TEXTUREDATA_BASE), width, height);

  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  return LUX_TRUE;
}

LUX_API void  lxgRenderBuffer_deinit(lxgRenderBufferPTR rb,lxgContextPTR ctx)
{
  glDeleteRenderbuffersEXT(1,&rb->glid);
}

//////////////////////////////////////////////////////////////////////////

LUX_API LUX_INLINE void  lxgContext_applyTextureImage( lxgContextPTR ctx, lxgTextureImageCPTR img, uint imageunit)
{
  LUX_DEBUGASSERT(imageunit < LUXGFX_MAX_RWTEXTURE_IMAGES);
  glBindImageTextureEXT(imageunit,img->tex->glid,img->level,img->layered,img->layer,img->glaccess,img->glformat);
  ctx->images[imageunit] = img;
}


LUX_API void  lxgContext_applyTextureImages( lxgContextPTR ctx, lxgTextureImageCPTR *imgs, uint start, uint num)
{
  int i;
  for (i = num-1; i >= 0; i--){
    lxgContext_applyTextureImage(ctx, imgs[i], i);
  }
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




LUX_API booln lxgTextureImage_init( lxgTextureImagePTR img, lxgContextPTR ctx, lxgTexturePTR tex, lxgAccessMode_t access, uint level, booln layered, int layer )
{
  static lxGLAccessMode_t modes[] = {
    LUXGL_ACCESS_READ_ONLY,
    LUXGL_ACCESS_WRITE_ONLY,
    LUXGL_ACCESS_READ_WRITE,

    LUXGL_ACCESS_WRITE_ONLY,
    LUXGL_ACCESS_WRITE_ONLY,
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

  switch(tex->componentsize * tex->components * 8){
  case 8:
    base = LUXGFX_ACCESSFORMAT_R8UI;
    break;
  case 16:
    base = LUXGFX_ACCESSFORMAT_R16UI;
    break;
  case 32:
    base = LUXGFX_ACCESSFORMAT_R32UI;
    break;
  case 64:
    base = LUXGFX_ACCESSFORMAT_RG32UI;
    break;
  case 128:
    base = LUXGFX_ACCESSFORMAT_RGBA32UI;
    break;
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


