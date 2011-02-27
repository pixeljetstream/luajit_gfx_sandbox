// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXLUXGFX_VERTEX_H__
#define __LUXLUXGFX_VERTEX_H__

#include <luxplatform/luxplatform.h>

#include "glconfig.h"

#ifdef __cplusplus
extern "C"{
#endif

  //////////////////////////////////////////////////////////////////////////
  // VertexAttrib

  typedef enum lxGFXVertexAttrib_e{
    LUXGFX_VERTEX_ATTRIB_POS,
    LUXGFX_VERTEX_ATTRIB_ATTR1,   // Cg:  blendweights
    LUXGFX_VERTEX_ATTRIB_NORMAL,
    LUXGFX_VERTEX_ATTRIB_COLOR,
    LUXGFX_VERTEX_ATTRIB_ATTR4,
    LUXGFX_VERTEX_ATTRIB_ATTR5,   // Cg: fogcoord,tessfactor
    LUXGFX_VERTEX_ATTRIB_ATTR6,   // Cg: pointsize
    LUXGFX_VERTEX_ATTRIB_ATTR7,   // Cg: blendindices 
    LUXGFX_VERTEX_ATTRIB_TEXCOORD0,
    LUXGFX_VERTEX_ATTRIB_TEXCOORD1,
    LUXGFX_VERTEX_ATTRIB_TEXCOORD2,
    LUXGFX_VERTEX_ATTRIB_TEXCOORD3,
    LUXGFX_VERTEX_ATTRIB_ATTR12,
    LUXGFX_VERTEX_ATTRIB_ATTR13,
    LUXGFX_VERTEX_ATTRIB_ATTR14,    // Cg: tangent
    LUXGFX_VERTEX_ATTRIB_ATTR15,    // Cg: binormal
    LUXGFX_VERTEX_ATTRIBS,
    LUXGFX_VERTEX_ATTRIB_FORCE_DWORD = 0x7FFFFFFF,
  }lxGFXVertexAttrib_t;

  #define LUXGFX_MAX_VERTEX_STREAMS 8

  //////////////////////////////////////////////////////////////////////////
  // lxGFXVertexElement_t
  //
  // assumes maximum vertex size of 256 bytes, and stride as multiple of 2

  typedef struct lxGFXVertexElement_s{
    unsigned  normalize :  1;
    unsigned  integer :  1;
    unsigned  cnt  :     2;
    unsigned  stream :     4;
    unsigned  scalartype : 8;
    unsigned  stridehalf : 8;
    unsigned  offset :  8;
  }lxGFXVertexElement_t;


  LUX_INLINE lxGFXVertexElement_t 
    lxGFXVertexElement_set(uint cnt, enum lxScalarType_e type, booln normalize, booln integer, uint stride,uint offset, uint stream)
  {
    lxGFXVertexElement_t  elem = {normalize,integer,cnt-1,stream,type,stride/2,offset};
    return elem;
  }

  //////////////////////////////////////////////////////////////////////////
  // lxGFXVertexPointer

  typedef struct lxGFXVertexDecl_s{
    flags32                 available;
    uint                    streams;
    lxGFXVertexElement_t    table[LUXGFX_VERTEX_ATTRIBS];
  }lxGFXVertexDecl_t;

  //////////////////////////////////////////////////////////////////////////
  // lxGFXVertexPointer

  typedef struct lxGFXStreamHost_s{
    lxGFXBufferPTR        buffer;
    void*                 ptr;
  }lxGFXStreamHost_t;
  
  typedef struct lxGFXVertexPointer_s{
    lxGFXVertexElement_t  element[LUXGFX_VERTEX_ATTRIBS];
    lxGFXStreamHost_t     streams[LUXGFX_MAX_VERTEX_STREAMS];
  }lxGFXVertexPointer_t;

  typedef struct lxGFXVertexState_s{
    lxGFXVertexDeclPTR  decl;
    flags32             active;
    flags32             declvalid;
    flags32             streamvalid;
    flags32             declchange;
    flags32             streamchange;

    lxGFXVertexPointer_t  setup;
  }lxGFXVertexState_t;

  LUX_API void lxGFXVertexAttribs_apply(lxGFXContextPTR ctx, flags32 attribs, flags32 changed);
  LUX_API void lxGFXVertexAttribs_setArray(lxGFXContextPTR ctx, uint numattribs, lxGFXVertexAttrib_t* attribs, booln state);

  LUX_API void lxGFXVertexDecl_apply(lxGFXContextPTR ctx, lxGFXVertexDeclPTR decl);
  LUX_API void lxGFXVertexSetup_setStreams(lxGFXContextPTR ctx, const lxGFXVertexDeclPTR decl, const lxGFXStreamHostPTR hosts);
  LUX_API void lxGFXVertexSetup_setStream(lxGFXContextPTR ctx, uint idx, lxGFXStreamHostPTR host);
  LUX_API void lxGFXVertexSetup_apply(lxGFXContextPTR ctx);

#ifdef __cplusplus
}
#endif



#endif