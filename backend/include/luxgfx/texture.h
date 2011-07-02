// Copyright (C) 2010-2011 Christoph Kubisch
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
    GLuint                glid;
    uint32                incarnation;
    lxGLCompareMode_t     cmpfunc;
    lxgSamplerFilter_t    filter;
    lxgSamplerAddress_t   addru;
    lxgSamplerAddress_t   addrv;
    lxgSamplerAddress_t   addrw;
    uint                  aniso;
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
    LUXGFX_TEXTURECHANNEL_NATIVE,
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
    lxGLTextureTarget_t   gltarget;
    GLuint                glid;
    lxgSamplerPTR         lastSampler;
    uint32                lastSamplerIncarnation;
    lxgContextPTR         ctx;

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
    lxVec3i_t     mipsizes[LUXGFX_MAX_TEXTURE_MIPMAPS];
    uint          pixelsizes[LUXGFX_MAX_TEXTURE_MIPMAPS];
    size_t        nativesizes[LUXGFX_MAX_TEXTURE_MIPMAPS];

    uint          components;
    uint          componentsize;

    lxgSampler_t  sampler;
    GLenum        glinternalformat;
    GLenum        gldatatype;
    GLenum        gldataformat;
  }lxgTexture_t;

  typedef struct lxgRenderBuffer_s{
    GLuint                glid;
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
    lxgTexturePTR     tex;
    int               level;
    booln             layered;
    int               layer;
    GLenum            glformat;
    GLenum            glaccess;
  }lxgTextureImage_t;

  //////////////////////////////////////////////////////////////////////////


  LUX_API void  lxgTextureContext_reset(lxgContextPTR ctx);
  LUX_API void  lxgTextureContext_setCompare(lxgContextPTR ctx, uint imageunit, enum lxgCompareMode_e cmp);
  LUX_API void  lxgTextureContext_setSampler(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR sampler, flags32 what);
  LUX_API void  lxgTextureContext_checkedSampler(lxgContextPTR ctx, uint imageunit, lxgSamplerPTR sampler, flags32 what);
  
  

  // Type checks
  LUX_API booln lxgTextureChannel_valid(lxgContextPTR ctx, lxgTextureChannel_t channel);
  LUX_API booln lxgTextureTarget_valid(lxgContextPTR ctx, lxGLTextureTarget_t type);

  LUX_API GLenum lxgTextureChannel_getFormat(lxgTextureChannel_t type, booln rev, booln integer);
  LUX_API GLenum lxgTextureDataType_getData(lxgTextureDataType_t data, booln rev, booln depthstencil);
  LUX_API GLenum lxgTextureChannel_getInternal(lxgTextureChannel_t type, lxgTextureDataType_t data);
  
  
  // lxgTexture

   // for multisampled textures depth = samples
  LUX_API void lxgTexture_init(lxgTexturePTR tex, lxgContextPTR ctx);
  LUX_API booln lxgTexture_setup(lxgTexturePTR tex, 
    lxGLTextureTarget_t type, lxgTextureChannel_t format, lxgTextureDataType_t data,
    int width, int height, int depth, int arraysize, flags32 flags);

  LUX_API booln lxgTexture_resize(lxgTexturePTR tex, 
    int width, int height, int depth, int arraysize);

  LUX_API void  lxgTexture_deinit(lxgTexturePTR tex, lxgContextPTR ctx);
  
  LUX_API booln lxgTexture_readFrame(lxgTexturePTR tex, lxgContextPTR ctx, 
    const lxgTextureUpdate_t* update, uint miplevel);

    // if scalartype is set to invalid, we assume "native" data (matching
    // internal)
  LUX_API booln lxgTexture_readData(lxgTexturePTR tex, 
    const lxgTextureUpdate_t* update, uint miplevel,
    GLenum datatype, GLenum dataformat, const void* buffer, uint buffersize);

  LUX_API booln lxgTexture_readBuffer(lxgTexturePTR tex, 
    const lxgTextureUpdate_t* update, uint miplevel,
    GLenum datatype, GLenum dataformat, const struct lxgBuffer_s* buffer, uint bufferoffset);

  LUX_API booln lxgTexture_writeData(lxgTexturePTR tex, uint side, booln ascompressed, uint mip, 
    GLenum datatype, GLenum dataformat, void* buffer, uint buffersize);
  LUX_API booln lxgTexture_writeBuffer(lxgTexturePTR tex, uint side, booln ascompressed, uint mip, 
    GLenum datatype, GLenum dataformat, lxgBufferPTR buffer, uint bufferoffset);

  LUX_API void  lxgTexture_getSampler(lxgTexturePTR tex, lxgSamplerPTR sampler);
  LUX_API void  lxgTexture_boundSetSampler(lxgTexturePTR tex, lxgSamplerPTR sampler, flags32 what);
  LUX_API const lxVec3iPTR  lxgTexture_getMipSize(lxgTexturePTR tex, uint mipLevel);
  
  
  //////////////////////////////////////////////////////////////////////////
  // lxgSampler_t

  LUX_API void lxgSampler_init(lxgSamplerPTR self);
  LUX_API void lxgSampler_setAddress(lxgSamplerPTR self, uint n, lxgSamplerAddress_t address);
  LUX_API void lxgSampler_setCompare(lxgSamplerPTR self, enum lxGLCompareMode_t cmp);
  LUX_API void lxgSampler_changed(lxgSamplerPTR self);
  
    // require SM4
  LUX_API void lxgSampler_initHW(lxgSamplerPTR self, lxgContextPTR ctx);
  LUX_API void lxgSampler_deinitHW(lxgSamplerPTR self, lxgContextPTR ctx);
  LUX_API void lxgSampler_updateHW(lxgSamplerPTR self);
  
  //////////////////////////////////////////////////////////////////////////
  // lxgRenderBuffer

  LUX_API booln lxgRenderBuffer_init(lxgRenderBufferPTR rb, lxgContextPTR ctx, lxgTextureChannel_t format,
    int width, int height, int samples);

  LUX_API booln lxgRenderBuffer_change(lxgRenderBufferPTR rb,
    lxgTextureChannel_t format,
    int width, int height, int samples);

  LUX_API void  lxgRenderBuffer_deinit(lxgRenderBufferPTR rb, lxgContextPTR ctx);

  //////////////////////////////////////////////////////////////////////////
  // lxgTextureImage

  LUX_API booln lxgTextureImage_init(lxgTextureImagePTR img, lxgContextPTR ctx, lxgTexturePTR tex, lxgAccessMode_t acces, 
    uint level, booln layered, int layer);
  




#ifdef __cplusplus
}
#endif


#endif
