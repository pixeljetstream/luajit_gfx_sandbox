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

  typedef enum lxgVertexAttrib_e{
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
  }lxgVertexAttrib_t;

  #define LUXGFX_MAX_VERTEX_STREAMS 8

  //////////////////////////////////////////////////////////////////////////
  // lxgVertexElement_t
  //
  // assumes maximum vertex size of 256 bytes, and stride as multiple of 2

  typedef struct lxgVertexElement_s{
    unsigned  normalize :  1;
    unsigned  integer :  1;
    unsigned  cnt  :     2;
    unsigned  stream :     4;
    unsigned  scalartype : 8;
    unsigned  stridehalf : 8;
    unsigned  offset :  8;
  }lxgVertexElement_t;


  LUX_INLINE lxgVertexElement_t 
    lxgVertexElement_set(uint cnt, enum lxScalarType_e type, booln normalize, booln integer, uint stride,uint offset, uint stream)
  {
    lxgVertexElement_t  elem = {normalize,integer,cnt-1,stream,type,stride/2,offset};
    return elem;
  }

  //////////////////////////////////////////////////////////////////////////
  // lxgVertexPointer

  typedef struct lxgVertexDecl_s{
    flags32               available;
    uint                  streams;
    lxgVertexElement_t    table[LUXGFX_VERTEX_ATTRIBS];
  }lxgVertexDecl_t;

  //////////////////////////////////////////////////////////////////////////
  // lxgVertexPointer

  typedef struct lxgStreamHost_s{
    lxgBufferPTR        buffer;
    void*                 ptr;
  }lxgStreamHost_t;
  
  typedef struct lxgVertexPointer_s{
    lxgVertexElement_t  element[LUXGFX_VERTEX_ATTRIBS];
    lxgStreamHost_t     streams[LUXGFX_MAX_VERTEX_STREAMS];
  }lxgVertexPointer_t;

  typedef struct lxgVertexState_s{
    lxgVertexDeclPTR  decl;
    flags32             active;
    flags32             declvalid;
    flags32             streamvalid;
    flags32             declchange;
    flags32             streamchange;

    lxgVertexPointer_t  setup;
  }lxgVertexState_t;

  LUX_API void lxgVertexAttribs_apply(lxgContextPTR ctx, flags32 attribs, flags32 changed);
  LUX_API void lxgVertexAttribs_applyFIXED(lxgContextPTR ctx, flags32 attribs, flags32 changed);

  LUX_API void lxgVertexDecl_apply(lxgContextPTR ctx, lxgVertexDeclPTR decl);
  LUX_API void lxgVertexSetup_setStreams(lxgContextPTR ctx, const lxgVertexDeclPTR decl, const lxgStreamHostPTR hosts);
  LUX_API void lxgVertexSetup_setStream(lxgContextPTR ctx, uint idx, lxgStreamHostPTR host);
  LUX_API void lxgVertexSetup_apply(lxgContextPTR ctx);
  LUX_API void lxgVertexSetup_applyFIXED(lxgContextPTR ctx);

#ifdef __cplusplus
}
#endif



#endif