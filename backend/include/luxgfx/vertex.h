// Copyright (C) 2010-2011 Christoph Kubisch
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
    union{
      void*             ptr;
      size_t            offset;
    };
    size_t              len;
  }lxgStreamHost_t;
  
  typedef struct lxgVertexPointer_s{
    lxgVertexElement_t  element[LUXGFX_VERTEX_ATTRIBS];
    lxgStreamHost_t     streams[LUXGFX_MAX_VERTEX_STREAMS];
  }lxgVertexPointer_t;

  typedef struct lxgVertexState_s{
    const lxgVertexDecl_t* decl;
    flags32             active;
    flags32             declvalid;
    flags32             declstreams;
    flags32             streamvalid;
    flags32             declchange;
    flags32             streamchange;

    lxgVertexPointer_t  setup;
  }lxgVertexState_t;

  typedef struct lxgFeedbackState_s{
    lxGLPrimitiveType_t capture;
    int                 active;
    flags32             usedvalid;
    flags32             streamvalid;
    flags32             streamchange;
    
    lxgStreamHost_t     streams[LUXGFX_MAX_VERTEX_STREAMS];
  }lxgFeedbackState_t;


  LUX_API flags32 lxgVertexAttrib_bit(lxgVertexAttrib_t attrib);

  LUX_API lxgVertexElement_t lxgVertexElement_set(
    uint cnt, enum lxScalarType_e type, booln normalize, 
    booln integer, uint stride,uint offset, uint stream);

  LUX_API void lxgVertexAttrib_applyFloat(lxgVertexAttrib_t attrib, const float* vec4);
  LUX_API void lxgVertexAttrib_applyInteger(lxgVertexAttrib_t attrib, const int* vec4);
  LUX_API void lxgVertexAttrib_applyFloatFIXED(lxgVertexAttrib_t attrib, const float* vec4);

  LUX_API void lxgContext_applyVertexAttribs(lxgContextPTR ctx, flags32 attribs, flags32 changed);
  LUX_API void lxgContext_applyVertexAttribsFIXED(lxgContextPTR ctx, flags32 attribs, flags32 changed);

  LUX_API void lxgContext_clearVertexState(lxgContextPTR ctx);
  LUX_API void lxgContext_setVertexDecl(lxgContextPTR ctx, lxgVertexDeclPTR decl);
  LUX_API void lxgContext_setVertexDeclStreams(lxgContextPTR ctx, lxgVertexDeclPTR decl, lxgStreamHostPTR hosts );
  LUX_API void lxgContext_setVertexStream(lxgContextPTR ctx, uint idx, lxgStreamHostPTR host);
  LUX_API void lxgContext_invalidateVertexStreams(lxgContextPTR ctx);

  LUX_API void lxgContext_applyVertexState(lxgContextPTR ctx);
  LUX_API void lxgContext_applyVertexStateFIXED(lxgContextPTR ctx);
  LUX_API void lxgContext_applyVertexStateNV(lxgContextPTR ctx);
  LUX_API void lxgContext_applyVertexStateFIXEDNV(lxgContextPTR ctx);

  LUX_API void lxgContext_clearFeedbackState(lxgContextPTR ctx);
  LUX_API void lxgContext_applyFeedbackStreams(lxgContextPTR ctx);
  LUX_API void lxgContext_setFeedbackStreams(lxgContextPTR ctx, lxgStreamHostPTR hosts, int numStreams);
  LUX_API void lxgContext_setFeedbackStream(lxgContextPTR ctx, uint idx, lxgStreamHostPTR host );
  LUX_API void lxgContext_enableFeedback(lxgContextPTR ctx, lxGLPrimitiveType_t type, int numStreams);
  LUX_API void lxgContext_disableFeedback(lxgContextPTR ctx);

  //////////////////////////////////////////////////////////////////////////

  LUX_INLINE lxgVertexElement_t 
    lxgVertexElement_set(uint cnt, enum lxScalarType_e type, booln normalize, booln integer, uint stride,uint offset, uint stream)
  {
    lxgVertexElement_t  elem = {normalize,integer,cnt-1,stream,type,stride/2,offset};
    return elem;
  }

  LUX_INLINE flags32 lxgVertexAttrib_bit(lxgVertexAttrib_t attrib){
    return 1 << attrib;
  }
  


#ifdef __cplusplus
}
#endif



#endif