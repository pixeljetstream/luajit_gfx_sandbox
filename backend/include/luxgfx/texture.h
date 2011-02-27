// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_TEXTURE_H__
#define __LUXLUXGFX_TEXTURE_H__

#include <luxplatform/luxplatform.h>
#include <luxmath/basetypes.h>

#include "glconfig.h"

#ifdef __cplusplus
extern "C"{
#endif

  #define LUXGFX_TEXTURE_MAXMIPMAPS 16

  typedef enum lxGFXSamplerFilter_e{
    LUXGFX_SAMPLERFILTER_NEAREST,
    LUXGFX_SAMPLERFILTER_LINEAR,
    LUXGFX_SAMPLERFILTER_MIPMAP_NEAREST,
    LUXGFX_SAMPLERFILTER_MIPMAP_LINEAR,

    LUXGFX_SAMPLERFILTERS,
    LUXGFX_SAMPLERFILTER_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXSamplerFilter_t;

  typedef enum lxGFXSamplerAddress_e{
    LUXGFX_SAMPLERADDRESS_REPEAT,
    LUXGFX_SAMPLERADDRESS_MIRROR,
    LUXGFX_SAMPLERADDRESS_CLAMP,
    LUXGFX_SAMPLERADDRESS_BORDER,

    LUXGFX_SAMPLERADDRESSES,
    LUXGFX_SAMPLERADDRESSES_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXSamplerAddress_t;

  enum lxGFXSamplerAttrib_e{
    LUXGFX_SAMPLERATTRIB_FILTER = 1<<0,
    LUXGFX_SAMPLERATTRIB_CMP = 1<<1,
    LUXGFX_SAMPLERATTRIB_ADDRESS = 1<<2,
    LUXGFX_SAMPLERATTRIB_ANISO = 1<<3,
    LUXGFX_SAMPLERATTRIB_LOD = 1<<4,
    LUXGFX_SAMPLERATTRIB_BORDER = 1<<5,
    LUXGFX_SAMPLERATTRIB_ALL = (1<<6)-1,
  };

  typedef struct lxGFXSamplerLod_s{
    float         bias;
    float         min;
    float         max;
  }lxGFXSamplerLod_t;

  typedef struct lxGFXSampler_s{
    lxGLSampler_t         vgl;
    uint32                incarnation;
    lxGFXSamplerFilter_t  filter;
    lxGLCompareMode_t     cmpfunc;
    lxGLAddressMode_t     address[3];
    uint                  aniso;
    lxGFXSamplerLod_t     lod;

    float                 border[4];
  }lxGFXSampler_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxGFXTextureType_e{
    LUXGFX_TEXTURE_1D,
    LUXGFX_TEXTURE_2D,
    LUXGFX_TEXTURE_RECT,
    LUXGFX_TEXTURE_3D,
    LUXGFX_TEXTURE_CUBE,
    LUXGFX_TEXTURE_2DMS,
    LUXGFX_TEXTURE_1DARRAY,
    LUXGFX_TEXTURE_2DARRAY,
    LUXGFX_TEXTURE_CUBEARRAY,
    LUXGFX_TEXTURE_2DMSARRAY,
    LUXGFX_TEXTURE_BUFFER,

    LUXGFX_TEXTURE_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXTextureType_t;

  typedef enum lxGFXTextureFlags_e{
    LUXGFX_TEXTUREFLAG_AUTOMIPMAP = 1<<0,
    LUXGFX_TEXTUREFLAG_MANMIPMAP  = 1<<1,
    LUXGFX_TEXTUREFLAG_COMPRESS   = 1<<2,
    LUXGFX_TEXTUREFLAG_COMPRESSED = 1<<3,
    LUXGFX_TEXTUREFLAG_SAMPLESFIXED = 1<<4,

    LUXGFX_TEXTUREFLAG_HASLOD     = 1<<30,
    LUXGFX_TEXTUREFLAG_HASCOMPARE = 1<<31,

    LUXGFX_TEXTUREFLAG_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXTextureFlags_t;

  typedef enum lxGFXTextureChannel_e{
    LUXGFX_TEXTURECHANNEL_RGB,
    LUXGFX_TEXTURECHANNEL_RGBA,
  
    LUXGFX_TEXTURECHANNEL_R,
    LUXGFX_TEXTURECHANNEL_RG,

    LUXGFX_TEXTURECHANNEL_SRGB,
    LUXGFX_TEXTURECHANNEL_SRGBA,

    LUXGFX_TEXTURECHANNEL_DEPTH,
    LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL,

#ifdef LUX_RENDERBACKEND_OPENGL1
    LUXGFX_TEXTURECHANNEL_ALPHA,
    LUXGFX_TEXTURECHANNEL_INTENSITY,
    LUXGFX_TEXTURECHANNEL_LUMINANCE,
    LUXGFX_TEXTURECHANNEL_LUMINANCEALPHA,
#endif

    LUXGFX_TEXTURECHANNEL_CUSTOM,
    LUXGFX_TEXTURECHANNEL_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXTextureChannel_t;

  typedef enum lxGFXTextureDataType_e{
    LUXGFX_TEXTUREDATA_BASE,

    LUXGFX_TEXTUREDATA_UNORM8,
    LUXGFX_TEXTUREDATA_UNORM16,
    LUXGFX_TEXTUREDATA_SNORM8,
    LUXGFX_TEXTUREDATA_SNORM16,
    
    LUXGFX_TEXTUREDATA_FLOAT16,
    LUXGFX_TEXTUREDATA_FLOAT32,

    LUXGFX_TEXTUREDATA_SINT8,
    LUXGFX_TEXTUREDATA_UINT8,
    LUXGFX_TEXTUREDATA_SINT16,
    LUXGFX_TEXTUREDATA_UINT16,
    LUXGFX_TEXTUREDATA_SINT32,
    LUXGFX_TEXTUREDATA_UINT32,


    LUXGFX_TEXTUREDATAS,

    // for depth
    LUXGFX_TEXTUREDATA_DEPTH16,
    LUXGFX_TEXTUREDATA_DEPTH24,
    LUXGFX_TEXTUREDATA_DEPTH32,
    LUXGFX_TEXTUREDATA_DEPTH32F,
    // for RGBA
    LUXGFX_TEXTUREDATA_UNORM1010102,
    LUXGFX_TEXTUREDATA_UINT1010102,
    LUXGFX_TEXTUREDATA_FLOAT111110,
    // for RGB
    LUXGFX_TEXTUREDATA_EXP999,

    LUXGFX_TEXTUREDATA_COMPRESSED,
    LUXGFX_TEXTUREDATA_COMPRESSED_DXT1,
    LUXGFX_TEXTUREDATA_COMPRESSED_DXT3,
    LUXGFX_TEXTUREDATA_COMPRESSED_DXT5,
    LUXGFX_TEXTUREDATA_COMPRESSED_TC,
    LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC,
    LUXGFX_TEXTUREDATA_COMPRESSED_UNORM_BPTC,
    LUXGFX_TEXTUREDATA_COMPRESSED_FLOAT_BPTC,
    LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_FLOAT_BPTC,
    LUXGFX_TEXTUREDATA_CUSTOM,

    LUXGFX_TEXTUREDATA_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXTextureDataType_t;

  typedef struct lxGFXTexture_s{
    lxGLTexture_t     vgl;
    lxGFXSamplerPTR   lastSampler;
    uint32            lastSamplerIncarnation;
    lxGFXContextPTR   ctx;

    lxGFXTextureType_t    type;
    lxGFXTextureChannel_t formattype;
    lxGFXTextureDataType_t  datatype;
    flags32         flags;

    int           width;
    int           height;
    int           depth;
    int           arraysize;

    flags32       mipsdefined;
    uint          miplevels;
    lxVec3i_t     mipsizes[LUXGFX_TEXTURE_MAXMIPMAPS];
    uint          pixelsizes[LUXGFX_TEXTURE_MAXMIPMAPS];
    size_t        nativesizes[LUXGFX_TEXTURE_MAXMIPMAPS];

    uint          components;

    lxGFXSampler_t      sampler;
    lxGLTextureData_t   vgldata;
  }lxGFXTexture_t;

  typedef struct lxGFXRenderBuffer_s{
    lxGLRenderBuffer_t    vgl;
    lxGFXContextPTR       ctx;

    lxGFXTextureChannel_t formattype;
    int           width;
    int           height;
    uint          samples;
  }lxGFXRenderBuffer_t;
  

    // for cubemap: z == side 
    // for cubemap array: layer = z/6, side = z%6)
  typedef struct lxGFXTextureUpdate_s{
    lxVec3i_t   from;
    lxVec3i_t   to;
    lxVec3i_t   size;
  }lxGFXTextureUpdate_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxGFXTextureAccessType_e{
    LUXGFX_TEXACCESS_READ,
    LUXGFX_TEXACCESS_WRITE,
    LUXGFX_TEXACCESS_READWRITE,
  }lxGFXTextureAccessType_t;

  typedef struct lxGFXTextureImage_s{
    lxGFXTexturePTR           tex;
    int                       level;
    booln                     layered;
    int                       layer;
    lxGLAccess_t              access;
    lxGLAccessFormat_t        format;
  }lxGFXTextureImage_t;

  //////////////////////////////////////////////////////////////////////////
  

  // Type checks
  LUX_API booln lxGFXTextureChannel_valid(lxGFXContextPTR ctx, lxGFXTextureChannel_t channel);
  LUX_API booln lxGFXTextureType_valid(lxGFXContextPTR ctx, lxGFXTextureType_t type);
  
  // lxGFXTexture

   // for multisampled textures depth = samples
  LUX_API booln lxGFXTexture_init(lxGFXContextPTR ctx, lxGFXTexturePTR tex, 
    lxGFXTextureType_t type, lxGFXTextureChannel_t format, lxGFXTextureDataType_t data,
    int width, int height, int depth, int arraysize, flags32 flags);

  LUX_API booln lxGFXTexture_resize(lxGFXContextPTR ctx, lxGFXTexturePTR tex, int width, int height);

  LUX_API void  lxGFXTexture_deinit(lxGFXContextPTR ctx, lxGFXTexturePTR tex);
  
  LUX_API booln lxGFXTexture_readFrame(lxGFXContextPTR ctx, lxGFXTexturePTR tex, 
    const lxGFXTextureUpdatePTR update, uint miplevel);

    // if scalartype is set to invalid, we assume "native" data (matching
    // internal)
  LUX_API booln lxGFXTexture_readData(lxGFXContextPTR ctx, lxGFXTexturePTR tex, 
    const lxGFXTextureUpdatePTR update, uint miplevel,
    enum lxScalarType_e scalar, const void* buffer, uint buffersize);

  LUX_API booln lxGFXTexture_readBuffer(lxGFXContextPTR ctx, lxGFXTexturePTR tex, 
    const lxGFXTextureUpdatePTR update, uint miplevel,
    enum lxScalarType_e scalar, const lxGFXBufferPTR buffer, uint bufferoffset);

  LUX_API booln lxGFXTexture_writeData(lxGFXContextPTR ctx, lxGFXTexturePTR tex, uint side, booln ascompressed, booln onlydepth, uint mip, enum lxScalarType_e d, void* buffer, uint buffersize);
  LUX_API booln lxGFXTexture_writeBuffer(lxGFXContextPTR ctx, lxGFXTexturePTR tex, uint side, booln ascompressed, booln onlydepth, uint mip, enum lxScalarType_e d, lxGFXBufferPTR buffer, uint bufferoffset);

  LUX_API void  lxGFXTexture_getSampler(const lxGFXTexturePTR tex, lxGFXSamplerPTR sampler);
  LUX_API const lxVec3iPTR  lxGFXTexture_getMipSize(const lxGFXTexturePTR tex, uint mipLevel);
  
  LUX_API void  lxGFXTextureUnit_setCompare(lxGFXContextPTR ctx, uint imageunit, enum lxGFXCompareMode_e cmp);
  LUX_API void  lxGFXTextureUnit_setSampler(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR sampler, flags32 what);
  LUX_API void  lxGFXTextureUnit_checkedSampler(lxGFXContextPTR ctx, uint imageunit, lxGFXSamplerPTR sampler, flags32 what);

  //////////////////////////////////////////////////////////////////////////
  // lxGFXSampler_t
  LUX_API void lxGFXSampler_init(lxGFXSamplerPTR self);
  LUX_API void lxGFXSampler_setAddress(lxGFXSamplerPTR self, uint n, lxGFXSamplerAddress_t address);
  LUX_API void lxGFXSampler_setCompare(lxGFXSamplerPTR self, enum lxGFXCompareMode_e cmp);
  LUX_API void lxGFXSampler_changed(lxGFXSamplerPTR self);
  
    // require SM4
  LUX_API void lxGFXSampler_initObj(lxGFXContextPTR ctx, lxGFXSamplerPTR self);
  LUX_API void lxGFXSampler_deinitObj(lxGFXContextPTR ctx, lxGFXSamplerPTR self);
  LUX_API void lxGFXSampler_updateObj(lxGFXContextPTR ctx, lxGFXSamplerPTR self);
  
  //////////////////////////////////////////////////////////////////////////
  // lxGFXRenderBuffer

  LUX_API booln lxGFXRenderBuffer_init(lxGFXContextPTR ctx, lxGFXRenderBufferPTR rb, lxGFXTextureChannel_t format,
    int width, int height, int samples);

  LUX_API booln lxGFXRenderBuffer_change(lxGFXContextPTR ctx, lxGFXRenderBufferPTR rb,
    lxGFXTextureChannel_t format,
    int width, int height, int samples);

  LUX_API void  lxGFXRenderBuffer_deinit(lxGFXContextPTR ctx, lxGFXRenderBufferPTR rb);


#ifdef __cplusplus
}
#endif


#endif