// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxgfx/luxgfx.h>
#include <luxplatform/debug.h>

#include "state_inl.h"



#define VERTEX_PTR(vpt)   (((byte*)ptr)+vpt->offset)
#define VERTEX_STRIDE(vpt)  (vpt->stridehalf*2)
#define VERTEX_TYPE(vpt)  (lxScalarType_to((lxScalarType_t)vpt->scalartype))
#define VERTEX_CNT(vpt)   (vpt->cnt + 1)
#define VERTEX_INT(vpt)   (vpt->integer)
#define VERTEX_NORMALIZE(vpt)   (vpt->normalize)

typedef void (lxGLVertexAttrib_fn)(const lxgVertexElement_t*, lxgVertexAttrib_t, void* ptr );
typedef void (lxGLVertexAttribState_fn)(lxgVertexAttrib_t );

static LUX_INLINE void lxGLVertexPointer(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glVertexPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void lxGLNormalPointer(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glNormalPointer(VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void lxGLColorPointer(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glColorPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void lxGLTexCoordPointer(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glClientActiveTexture(GL_TEXTURE0 + (attr - LUXGFX_VERTEX_ATTRIB_TEXCOORD0));
  glTexCoordPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}

static LUX_INLINE void lxGLAttribPointer(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  if (!VERTEX_INT(vpt)){
    glVertexAttribPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_NORMALIZE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
  else{
    glVertexAttribIPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
}

static LUX_INLINE void lxgVertexPointer_applyFIXED(const lxgVertexElement_t* elem, const lxgStreamHostPTR host, lxgVertexAttrib_t attr)
{
  static lxGLVertexAttrib_fn* funcs[] = {
    lxGLVertexPointer,
    lxGLAttribPointer,
    lxGLNormalPointer,
    lxGLColorPointer,
    lxGLAttribPointer,
    lxGLAttribPointer,
    lxGLAttribPointer,
    lxGLAttribPointer,
    lxGLTexCoordPointer,
    lxGLTexCoordPointer,
    lxGLTexCoordPointer,
    lxGLTexCoordPointer,
    lxGLAttribPointer,
    lxGLAttribPointer,
    lxGLAttribPointer,
    lxGLAttribPointer,
  };
  lxgBuffer_bind(host->buffer,LUXGL_BUFFER_VERTEX);
  funcs[attr](elem,attr,host->ptr);
}

LUX_API void lxgContext_applyVertexStateFIXED(lxgContextPTR ctx)
{
  lxgVertexState_t* vtx = &ctx->vertex;
  flags32 changed = vtx->declchange & vtx->declvalid;
  flags32 streamchanged = vtx->streamchange;

  int i;
  for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
    const lxgVertexElement_t* elem = &vtx->setup.element[i];
    uint stream = elem->stream;
    const lxgStreamHostPTR    host = &vtx->setup.streams[stream];
    if (changed & (1<<i) || streamchanged & (1<<stream)){
      lxgVertexPointer_applyFIXED(elem,host,i);
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
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

static LUX_INLINE void lxGLVertexPointerNV(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glVertexFormatNV(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt));
}
static LUX_INLINE void lxGLNormalPointerNV(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glNormalFormatNV(VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt));
}
static LUX_INLINE void lxGLColorPointerNV(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glColorFormatNV(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt));
}
static LUX_INLINE void lxGLTexCoordPointerNV(const lxgVertexElement_t* vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glClientActiveTexture(GL_TEXTURE0 + (attr - LUXGFX_VERTEX_ATTRIB_TEXCOORD0));
  glTexCoordFormatNV(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt));
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

static LUX_INLINE void lxgVertexPointer_applyFormatNV(const lxgVertexElement_t* elem, lxgVertexAttrib_t attr, booln fixed)
{
  static lxGLVertexAttrib_fn* funcs[] = {
    lxGLVertexPointerNV,
    lxGLAttribPointerNV,
    lxGLNormalPointerNV,
    lxGLColorPointerNV,
    lxGLAttribPointerNV,
    lxGLAttribPointerNV,
    lxGLAttribPointerNV,
    lxGLAttribPointerNV,
    lxGLTexCoordPointerNV,
    lxGLTexCoordPointerNV,
    lxGLTexCoordPointerNV,
    lxGLTexCoordPointerNV,
    lxGLAttribPointerNV,
    lxGLAttribPointerNV,
    lxGLAttribPointerNV,
    lxGLAttribPointerNV,
  };
  if (fixed){
    funcs[attr](elem,attr,NULL);
  }
  else{
    lxGLAttribPointerNV(elem,attr,NULL);
  }
}

static LUX_INLINE void lxgVertexPointer_applyBufferNV(const lxgVertexElement_t* elem, const lxgStreamHost_t* host, lxgVertexAttrib_t attr, booln fixed)
{
  static GLenum bindless[] = {
    GL_VERTEX_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_NORMAL_ARRAY_ADDRESS_NV,
    GL_COLOR_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_TEXTURE_COORD_ARRAY_ADDRESS_NV,
    GL_TEXTURE_COORD_ARRAY_ADDRESS_NV,
    GL_TEXTURE_COORD_ARRAY_ADDRESS_NV,
    GL_TEXTURE_COORD_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
    GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV,
  };
  static int indexcorr[] = {
    0,0,0,0,0,0,0,0,
    8,9,10,11,0,0,0,0,
  };

  int index = fixed ? indexcorr[attr] : 0;
  int bind = fixed ? bindless[attr] : GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV;

  glBufferAddressRangeNV(bind,(int)attr + index, host->buffer->address + ((GLuint64)host->ptr) + (GLuint64)elem->offset, host->len);
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
      lxgVertexPointer_applyFormatNV(elem,i,0);
    }
    if (streamchanged & (1<<stream)){
      lxgVertexPointer_applyBufferNV(elem,host,i,0);
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
}

LUX_API void lxgContext_applyVertexStateFIXEDNV(lxgContextPTR ctx)
{
  lxgVertexState_t* vtx = &ctx->vertex;
  flags32 declchanged = vtx->declchange & vtx->declvalid;
  flags32 streamchanged = vtx->streamchange;

  int i;
  for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
    const lxgVertexElement_t* elem = &vtx->setup.element[i];
    uint stream = elem->stream;
    const lxgStreamHostPTR    host = &vtx->setup.streams[stream];
    if (declchanged & (1<<i)){
      lxgVertexPointer_applyFormatNV(elem,i,1);
    }
    if (streamchanged & (1<<i)){
      lxgVertexPointer_applyBufferNV(elem,host,i,1);
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
}



#define CASE_TEST(what)   if( changed & what){
#define CASE_END      }


LUX_API void  lxgContext_applyVertexAttribsFIXED( lxgContextPTR ctx, flags32 attribs, flags32 changed)
{

#define ENABLE_VERTEX_TEX(i)    {glClientActiveTexture(GL_TEXTURE0+i); glEnableClientState(GL_TEXTURE_COORD_ARRAY);}
#define DISABLE_VERTEX_TEX(i)   {glClientActiveTexture(GL_TEXTURE0+i); glDisableClientState(GL_TEXTURE_COORD_ARRAY);}

  attribs &= ctx->vertex.declvalid;

  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_POS) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_POS) & attribs)  glEnableClientState(GL_VERTEX_ARRAY);
    else      glDisableClientState(GL_VERTEX_ARRAY);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR1) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR1) & attribs)  glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR1);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR1);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_NORMAL) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_NORMAL) & attribs) glEnableClientState(GL_NORMAL_ARRAY);
    else      glDisableClientState(GL_NORMAL_ARRAY);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_COLOR) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_COLOR) & attribs)  glEnableClientState(GL_COLOR_ARRAY);
    else      glDisableClientState(GL_COLOR_ARRAY);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR4) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR4) & attribs)  glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR4);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR4);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR5) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR5) & attribs)  glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR5);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR5);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR6) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR6) & attribs)  glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR6);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR6);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR7) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR7) & attribs)  glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR7);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR7);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD0) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD0) & attribs)  {ENABLE_VERTEX_TEX(0)}
    else      {DISABLE_VERTEX_TEX(0)}
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD1) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD1) & attribs)  {ENABLE_VERTEX_TEX(1)}
    else      {DISABLE_VERTEX_TEX(1)}
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD2) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD2) & attribs)  {ENABLE_VERTEX_TEX(2)}
    else      {DISABLE_VERTEX_TEX(2)}
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD3) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD3) & attribs)  {ENABLE_VERTEX_TEX(3)}
    else      {DISABLE_VERTEX_TEX(3)}
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR12) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR12) & attribs) glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR12);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR12);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR13) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR13) & attribs) glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR13);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR13);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR14) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR14) & attribs) glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR14);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR14);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR15) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR15) & attribs) glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR15);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR15);
  CASE_END;

#undef ENABLE_VERTEX_TEX
#undef DISABLE_VERTEX_TEX

  ctx->vertex.active = attribs;
}

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

LUX_API void lxgVertexAttrib_applyFloatFIXED(lxgVertexAttrib_t type, const float* vec4)
{
  switch (type){
  case LUXGFX_VERTEX_ATTRIB_POS:
    glVertex4fv(vec4);
    return;
  
  case LUXGFX_VERTEX_ATTRIB_ATTR1:
    glVertexAttrib4fv(1,vec4);
    return;
  
  case LUXGFX_VERTEX_ATTRIB_NORMAL:
    glNormal3fv(vec4);
    return;

  case LUXGFX_VERTEX_ATTRIB_COLOR:
    glColor4fv(vec4);
    return;

  case LUXGFX_VERTEX_ATTRIB_ATTR4:
  case LUXGFX_VERTEX_ATTRIB_ATTR5: 
  case LUXGFX_VERTEX_ATTRIB_ATTR6:
  case LUXGFX_VERTEX_ATTRIB_ATTR7:
    glVertexAttrib4fv(type,vec4);
    return;

  case LUXGFX_VERTEX_ATTRIB_TEXCOORD0:
  case LUXGFX_VERTEX_ATTRIB_TEXCOORD1:
  case LUXGFX_VERTEX_ATTRIB_TEXCOORD2:
  case LUXGFX_VERTEX_ATTRIB_TEXCOORD3:
    glActiveTexture(GL_TEXTURE0 + type - LUXGFX_VERTEX_ATTRIB_TEXCOORD0);
    glTexCoord4fv(vec4);
    return;

  case LUXGFX_VERTEX_ATTRIB_ATTR12:
  case LUXGFX_VERTEX_ATTRIB_ATTR13:
  case LUXGFX_VERTEX_ATTRIB_ATTR14:
  case LUXGFX_VERTEX_ATTRIB_ATTR15:
    glVertexAttrib4fv(type,vec4);
    return;
  }
}

LUX_API void lxgContext_setFeedbackStreams(lxgContextPTR ctx, lxgStreamHostCPTR streams, int numStreams)
{
  lxgFeedbackStatePTR xfb = &ctx->feedback;
  flags32 changed = 0;
  flags32 valid = 0;
  int i;

  for (i = 0; i < numStreams; i++){
    LUX_DEBUGASSERT(lxgStreamHost_valid(&streams[i]));

    changed |= (lxgStreamHost_unequal(&xfb->streams[i],&streams[i])) << i;
    xfb->streams[i] = streams[i];
    valid |= 1 << i;
  }
  xfb->streamchange |= changed;
  xfb->streamvalid  |= valid;
}


LUX_API void lxgContext_setFeedbackStream(lxgContextPTR ctx, uint idx, lxgStreamHostCPTR host)
{
  lxgFeedbackStatePTR xfb = &ctx->feedback;
  
  if (!lxgStreamHost_valid(host)){
    xfb->streamvalid &= ~(1 << idx);
    return;
  }

  xfb->streamchange |= lxgStreamHost_unequal(&xfb->streams[idx],host) << idx;
  xfb->streamvalid  |= (1 << idx);
  xfb->streams[idx] = *host;
}

LUX_API LUX_INLINE void lxgContext_applyFeedbackStreams(lxgContextPTR ctx)
{
  lxgFeedbackStatePTR xfb = &ctx->feedback;
  flags32 streamchanged = xfb->streamchange;

  LUX_DEBUGASSERT((xfb->usedvalid & xfb->streamvalid) == xfb->usedvalid);
  if (streamchanged){
    int i;
    for (i = 0; i < xfb->active; i++){
      const lxgStreamHostPTR    host = &xfb->streams[i];
      if (streamchanged & (1<<i)){
        lxgBuffer_bindRanged(host->buffer,LUXGL_BUFFER_FEEDBACK,i,(size_t)host->ptr,host->len);
      }
    }
  }

  xfb->streamchange = 0;
}

LUX_API void lxgContext_clearFeedbackState(lxgContextPTR ctx)
{
  ctx->feedback.streamvalid = 0;
  ctx->feedback.streamchange = 0;
  memset(ctx->feedback.streams,0,sizeof(ctx->feedback.streams));
}

LUX_API void lxgContext_enableFeedback(lxgContextPTR ctx, lxGLPrimitiveType_t type, int numStreams)
{
  lxgFeedbackStatePTR xfb = &ctx->feedback;

  LUX_DEBUGASSERT(!xfb->active);

  xfb->capture = type;
  xfb->active = numStreams;
  xfb->usedvalid = (1<<numStreams) - 1;

  lxgContext_applyFeedbackStreams(ctx);
  glBeginTransformFeedback(type);
}

LUX_API void lxgContext_disableFeedback(lxgContextPTR ctx)
{
  lxgFeedbackStatePTR xfb = &ctx->feedback;

  LUX_DEBUGASSERT(xfb->active); 
  glEndTransformFeedback();
  xfb->active = 0;
}




