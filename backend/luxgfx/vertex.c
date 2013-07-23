// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxgfx/luxgfx.h>
#include <luxinia/luxplatform/debug.h>

#include "state_inl.h"



#define VERTEX_PTR(vpt)   (((byte*)ptr)+vpt->offset)
#define VERTEX_STRIDE(vpt)  (vpt->stridehalf*2)
#define VERTEX_TYPE(vpt)  (lxScalarType_to((lxScalarType_t)vpt->scalartype))
#define VERTEX_CNT(vpt)   (vpt->cnt + 1)
#define VERTEX_INT(vpt)   (vpt->integer)
#define VERTEX_NORMALIZE(vpt)   (vpt->normalize)

typedef void (lxGLVertexAttrib_fn)(const lxgVertexElement_t*, lxgVertexAttrib_t, void* ptr );
typedef void (lxGLVertexAttribState_fn)(lxgVertexAttrib_t );


static LUX_INLINE void lxGLAttribPointer(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  if (!VERTEX_INT(vpt)){
    glVertexAttribPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_NORMALIZE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
  else{
    glVertexAttribIPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
}

static LUX_INLINE void lxgVertexPointer_apply(const lxgVertexElement_t* elem, const lxgStreamHost_t* host, lxgVertexAttrib_t attr)
{
  lxgBuffer_bind(host->buffer,LUXGL_BUFFER_VERTEX);
  lxGLAttribPointer(elem,attr,host->ptr);
}

LUX_API void lxgContext_applyVertexState(lxgContextPTR ctx)
{
  lxgVertexState_t* vtx = &ctx->vertex;
  flags32 changed = vtx->declchange & vtx->declvalid;
  flags32 streamchanged = vtx->streamchange & vtx->declstreams;

  int i;
  for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
    const lxgVertexElement_t* elem = &vtx->setup.element[i];
    uint stream = elem->stream;
    const lxgStreamHost_t*    host = &vtx->setup.streams[stream];
    if (changed & (1<<i) || streamchanged & (1<<stream)){
      lxgVertexPointer_apply(elem,host,i);
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
}


static LUX_INLINE void lxGLAttribPointerNV(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  if (!VERTEX_INT(vpt)){
    glVertexAttribFormatNV((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_NORMALIZE(vpt),VERTEX_STRIDE(vpt));
  }
  else{
    glVertexAttribIFormatNV((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt));
  }
}

static LUX_INLINE void lxgVertexPointer_applyFormatNV(const lxgVertexElement_t* elem, lxgVertexAttrib_t attr)
{
  lxGLAttribPointerNV(elem,attr,NULL);
}

static LUX_INLINE void lxgVertexPointer_applyBufferNV(const lxgVertexElement_t* elem, const lxgStreamHost_t* host, lxgVertexAttrib_t attr)
{

  glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,(int)attr, host->buffer->address + ((GLuint64)host->ptr) + (GLuint64)elem->offset, host->len);
}

LUX_API void lxgContext_applyVertexStateNV(lxgContextPTR ctx)
{
  lxgVertexState_t* vtx = &ctx->vertex;
  flags32 declchanged = vtx->declchange & vtx->declvalid;
  flags32 streamchanged = vtx->streamchange & vtx->declstreams;

  int i;
  for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
    const lxgVertexElement_t* elem = &vtx->setup.element[i];
    uint stream = elem->stream;
    const lxgStreamHost_t*    host = &vtx->setup.streams[stream];
    if (declchanged & (1<<i)){
      lxgVertexPointer_applyFormatNV(elem,i);
    }
    if (streamchanged & (1<<stream)){
      lxgVertexPointer_applyBufferNV(elem,host,i);
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
}



#define CASE_TEST(what)   if( changed & what){
#define CASE_END      }



LUX_API void  lxgContext_applyVertexAttribs( lxgContextPTR ctx, flags32 attribs, flags32 changed)
{
  int i = 0;

  attribs &= ctx->vertex.declvalid;

  for (; i < LUXGFX_VERTEX_ATTRIBS; i++){
    uint bit = (1<<i);
    if (changed & bit){
      if (attribs & bit)  glEnableVertexAttribArray(i);
      else                glDisableVertexAttribArray(i);
    }
  }

  ctx->vertex.active = attribs;
}

LUX_API void lxgContext_setVertexDecl( lxgContextPTR ctx,  lxgVertexDeclCPTR decl )
{
  lxgVertexState_t* vtx = &ctx->vertex;
  int i;
  vtx->decl = decl;
  vtx->declvalid = decl->available;
  for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
    if (decl->available & (1<<i) && (memcmp(&vtx->setup.element[i],&decl->table[i],sizeof(lxgVertexElement_t)))){
      vtx->setup.element[i] = decl->table[i];
      vtx->declchange |= (1<<i);
    }
  }
  vtx->declstreams = (1 << decl->streams ) - 1;
}

static LUX_INLINE booln lxgStreamHost_valid(const lxgStreamHostCPTR host)
{
  return (host && (host->buffer || host->ptr));
}

static LUX_INLINE booln lxgStreamHost_unequal(const lxgStreamHostCPTR a, lxgStreamHostCPTR b)
{
  return !!memcmp(a,b,sizeof(lxgStreamHost_t));
}

LUX_API void lxgContext_setVertexDeclStreams(lxgContextPTR ctx, lxgVertexDeclCPTR decl, lxgStreamHostCPTR streams)
{
  lxgVertexState_t* vtx = &ctx->vertex;
  flags32 changed = 0;
  uint i;

  LUX_DEBUGASSERT(vtx->decl == decl);

  for (i = 0; i < decl->streams; i++){
    LUX_DEBUGASSERT(lxgStreamHost_valid(&streams[i]));

    changed |= (lxgStreamHost_unequal(&vtx->setup.streams[i],&streams[i])) << i;
    vtx->setup.streams[i] = streams[i];
  }
  vtx->streamchange |= changed;
  vtx->streamvalid = vtx->declvalid;
}


LUX_API void lxgContext_setVertexStream(lxgContextPTR ctx, uint idx, lxgStreamHostCPTR host)
{
  lxgVertexState_t* vtx = &ctx->vertex;
  booln changed = 0;
  
  if (!lxgStreamHost_valid(host)){
    vtx->streamvalid &= ~(1 << idx);
    return;
  }

  vtx->streamchange |= lxgStreamHost_unequal(&vtx->setup.streams[idx],host) << idx;
  vtx->streamvalid |= (1 << idx);
  vtx->setup.streams[idx] = *host;
}

LUX_API void lxgContext_clearVertexState( lxgContextPTR ctx )
{
  lxgVertexState_t* vtx = &ctx->vertex;
  memset(vtx,0,sizeof(lxgVertexState_t));
  lxgContext_applyVertexAttribsFIXED(ctx,0,0xFFFFFFFF);
}

LUX_API void lxgVertexAttrib_applyFloat(lxgVertexAttrib_t type, const float* vec4)
{
  glVertexAttrib4fv(type, vec4);
}

LUX_API void lxgVertexAttrib_applyInteger(lxgVertexAttrib_t type, const int* vec4)
{
  glVertexAttribI4iv(type, vec4);
}


LUX_INLINE LUX_API void lxgContext_applyFeedbackStream(lxgContextPTR ctx, uint idx, lxgStreamHostCPTR host)
{
  lxgFeedbackStatePTR xfb = &ctx->feedback;

  LUX_DEBUGASSERT(idx <= LUXGFX_MAX_VERTEX_STREAMS);
  LUX_DEBUGASSERT(lxgStreamHost_valid(host));

  if (lxgStreamHost_unequal(&xfb->streams[idx],host)){
    lxgBuffer_bindRanged(host->buffer,LUXGL_BUFFER_FEEDBACK,idx,(size_t)host->ptr,host->len);
  }

  xfb->streams[idx] = *host;
}

LUX_API void lxgContext_applyFeedbackStreams(lxgContextPTR ctx, lxgStreamHostCPTR streams, int numStreams)
{
  int i;

  for (i = 0; i < numStreams; i++){
    lxgContext_applyFeedbackStream(ctx,i,&streams[i]);
  }
}

LUX_API void lxgContext_clearFeedbackState(lxgContextPTR ctx )
{
  memset(&ctx->feedback,0,sizeof(lxgFeedbackState_t));
}





