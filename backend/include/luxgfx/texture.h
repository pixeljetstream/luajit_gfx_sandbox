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

  typedef enum lxgSamplerFilter_e{
    LUXGFX_SAMPLERFILTER_NEAREST,
    LUXGFX_SAMPLERFILTER_LINEAR,
    LUXGFX_SAMPLERFILTER_MIPMAP_NEAREST,
    LUXGFX_SAMPLERFILTER_MIPMAP_LINEAR,

    LUXGFX_SAMPLERFILTERS,
  }lxgSamplerFilter_t;

  typedef enum lxgSamplerAddress_e{
    LUXGFX_SAMPLERADDRESS_REPEAT,
    LUXGFX_SAMPLERADDRESS_MIRROR,
    LUXGFX_SAMPLERADDRESS_CLAMP,
    LUXGFX_SAMPLERADDRESS_BORDER,

    LUXGFX_SAMPLERADDRESSES,
  }lxgSamplerAddress_t;

  enum lxgSamplerAttrib_e{
    LUXGFX_SAMPLERATTRIB_FILTER = 1<<0,
    LUXGFX_SAMPLERATTRIB_CMP = 1<<1,
    LUXGFX_SAMPLERATTRIB_ADDRESS = 1<<2,
    LUXGFX_SAMPLERATTRIB_ANISO = 1<<3,
    LUXGFX_SAMPLERATTRIB_LOD = 1<<4,
    LUXGFX_SAMPLERATTRIB_BORDER = 1<<5,
    LUXGFX_SAMPLERATTRIB_ALL = (1<<6)-1,
  };

  typedef struct lxgSamplerLod_s{
    float         bias;
    float         min;
    float         max;
  }lxgSamplerLod_t;

  typedef struct lxgSampler_s{
    lxGLSampler_t         vgl;
    uint32                incarnation;
    lxgCompareMode_t      cmpfunc : 8;
    lxgSamplerFilter_t    filter : 3;
    lxgSamplerAddress_t   addru : 3;
    lxgSamplerAddress_t   addrv : 3;
    lxgSamplerAddress_t   addrw : 3;
    uint                  aniso : 4;
    lxgSamplerLod_t       lod;

    float                 border[4];
  }lxgSampler_t;

  //////////////////////////////////////////////////////////////////////////

  typedef enum lxgTextureFlags_e{
    LUXGFX_TEXTUREFLAG_AUTOMIPMAP = 1<<0,
    LUXGFX_TEXTUREFLAG_MANMIPMAP  = 1<<1,
    LUXGFX_TEXTUREFLAG_COMPRESS   = 1<<2,
    LUXGFX_TEXTUREFLAG_COMPRESSED = 1<<3,
    LUXGFX_TEXTUREFLAG_SAMPLESFIXED = 1<<4,

    LUXGFX_TEXTUREFLAG_HASLOD     = 1<<30,
    LUXGFX_TEXTUREFLAG_HASCOMPARE = 1<<31,
  }lxgTextureFlags_t;

  typedef enum lxgTextureChannel_e{
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
  }lxgTextureChannel_t;

  typedef enum lxgTextureDataType_e{
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
  }lxgTextureDataType_t;

  typedef struct lxgTexture_s{
    lxGLTexture_t     vgl;
    lxgSamplerPTR     lastSampler;
    uint32            lastSamplerIncarnation;
    lxgContextPTR     ctx;

    lxgTextureChannel_t   formattype;
    lxgTextureDataType_t  datatype;
    flags32               flags;

    int           width;
    int           height;
    int           depth;
    int           arraysize;
    int           samples;

    flags32       mipsdefined;
    uint          miplevels;
    lxVec3i_t     mipsizes[LUXGFX_TEXTURE_MAXMIPMAPS];
    uint          pixelsizes[LUXGFX_TEXTURE_MAXMIPMAPS];
    size_t        nativesizes[LUXGFX_TEXTURE_MAXMIPMAPS];

    uint          components;
    uint          componentsize;

    lxgSampler_t        sampler;
    lxGLTextureData_t   vgldata;
  }lxgTexture_t;

  typedef struct lxgRenderBuffer_s{
    lxGLRenderBuffer_t    vgl;
    lxgContextPTR         ctx;

    lxgTextureChannel_t formattype;
    int           width;
    int           height;
    uint          samples;
  }lxgRenderBuffer_t;
  

    // for cubemap: z == side 
    // for cubemap array: layer = z/6, side = z%6)
  typedef struct lxgTextureUpdate_s{
    lxVec3i_t   from;
    lxVec3i_t   to;
    lxVec3i_t   size;
  }lxgTextureUpdate_t;

  //////////////////////////////////////////////////////////////////////////

  typedef struct lxgTextureImage_s{
    lxgTexturePTR             tex;
    int                       level;
    booln                     layered;
    int                       layer;
    lxGLTextureImage_t        vgl;
  }lxgTextureImage_t;

  //////////////////////////////////////////////////////////////////////////
  

  // Type checks
  LUX_API booln lxgTextureChannel_valid(lxgContextPTR ctx, lxgTextureChannel_t channel);
  LUX_API booln lxgTextureTarget_valid(lxgContextPTR ctx, lxGLTextureTarget_t type);
  
  // lxgTexture

   // for multisampled textures depth = samples
  LUX_API booln lxgTexture_init(lxgContextPTR ctx, lxgTexturePTR tex, 
    lxGLTextureTarget_t type, lxgTextureChannel_t format, lxgTextureDataType_t data,
    int width, int height, int depth, int arraysize, flags32 flags);

  LUX_API booln lxgTexture_resize(lxgContextPTR ctx, lxgTexturePTR tex, 
    int width, int height, int depth, int arraysize);

  LUX_API void  lxgTexture_deinit(lxgContextPTR ctx, lxgTexturePTR tex);
  
  LUX_API booln lxgTexture_readFrame(lxgContextPTR ctx, lxgTexturePTR tex, 
    const lxgTextureUpdatePTR update, uint miplevel);

    // if scalartype is set to invalid, we assume "native" data (matching
    // internal)
  LUX_API booln lxgTexture_readData(lxgContextPTR ctx, lxgTexturePTR tex, 
    const lxgTextureUpdatePTR update, uint miplevel,
    enum lxScalarType_e scalar, const void* buffer, uint buffersize);

  LUX_API booln lxgTexture_readBuffer(lxgContextPTR ctx, lxgTexturePTR tex, 
    const lxgTextureUpdatePTR update, uint miplevel,
    enum lxScalarType_e scalar, const lxgBufferPTR buffer, uint bufferoffset);

  LUX_API booln lxgTexture_writeData(lxgContextPTR ctx, lxgTexturePTR tex, uint side, booln ascompressed, booln onlydepth, uint mip, enum lxScalarType_e d, void* buffer, uint buffersize);
  LUX_API booln lxgTexture_writeBuffer(lxgContextPTR ctx, lxgTexturePTR tex, uint side, booln ascompressed, booln onlydepth, uint mip, enum lxScalarType_e d, lxgBufferPTR buffer, uint bufferoffset);

  LUX_API void  lxgTexture_getSampler(const lxgTexturePTR tex, lxgSamplerPTR sampler);
  LUX_API const lxVec3iPTR  lxgTexture_getMipSize(const lxgTexturePTR tex, uint mipLevel);
  
  LUX_API void  lxgTextureUnit_setCompare(lxgContextPTR ctx, uint imageunit, enum lxgCompareMode_e cmp);
  LUX_API void  lxgTextureUnit_setSampler(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR sampler, flags32 what);
  LUX_API void  lxgTextureUnit_checkedSampler(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR sampler, flags32 what);

  //////////////////////////////////////////////////////////////////////////
  // lxgSampler_t

  LUX_API void lxgSampler_init(lxgSamplerPTR self);
  LUX_API void lxgSampler_setAddress(lxgSamplerPTR self, uint n, lxgSamplerAddress_t address);
  LUX_API void lxgSampler_setCompare(lxgSamplerPTR self, enum lxgCompareMode_e cmp);
  LUX_API void lxgSampler_changed(lxgSamplerPTR self);
  
    // require SM4
  LUX_API void lxgSampler_initHW(lxgContextPTR ctx, lxgSamplerPTR self);
  LUX_API void lxgSampler_deinitHW(lxgContextPTR ctx, lxgSamplerPTR self);
  LUX_API void lxgSampler_updateHW(lxgContextPTR ctx, lxgSamplerPTR self);
  
  //////////////////////////////////////////////////////////////////////////
  // lxgRenderBuffer

  LUX_API booln lxgRenderBuffer_init(lxgContextPTR ctx, lxgRenderBufferPTR rb, lxgTextureChannel_t format,
    int width, int height, int samples);

  LUX_API booln lxgRenderBuffer_change(lxgContextPTR ctx, lxgRenderBufferPTR rb,
    lxgTextureChannel_t format,
    int width, int height, int samples);

  LUX_API void  lxgRenderBuffer_deinit(lxgContextPTR ctx, lxgRenderBufferPTR rb);

  //////////////////////////////////////////////////////////////////////////
  // lxgTextureImage

  LUX_API booln lxgTextureImage_init(lxgTextureImagePTR img, lxgTexturePTR tex, lxgAccessMode_t acces, 
    uint level, booln layered, int layer);


#ifdef __cplusplus
}
#endif


#endif
