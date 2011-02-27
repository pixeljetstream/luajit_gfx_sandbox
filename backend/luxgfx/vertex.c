// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <luxgfx/luxgfx.h>
#include <luxplatform/debug.h>

#include "state_inl.h"



#define VERTEX_PTR(vpt)   (((byte*)ptr)+vpt->offset)
#define VERTEX_STRIDE(vpt)  (vpt->stridehalf*2)
#define VERTEX_TYPE(vpt)  (lxScalarType_to((lxScalarType_t)vpt->scalartype))
#define VERTEX_CNT(vpt)   (vpt->cnt + 1)
#define VERTEX_INT(vpt)   (vpt->integer)
#define VERTEX_NORMALIZE(vpt)   (vpt->normalize)

typedef void (lxGLVertexAttrib_fn)(const lxgVertexElementPTR, lxgVertexAttrib_t, void* ptr );
typedef void (lxGLVertexAttribState_fn)(lxgVertexAttrib_t );

static LUX_INLINE void lxGLVertexPointer(const lxgVertexElementPTR vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glVertexPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void lxGLNormalPointer(const lxgVertexElementPTR vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glNormalPointer(VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void lxGLColorPointer(const lxgVertexElementPTR vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glColorPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void lxGLTexCoordPointer(const lxgVertexElementPTR vpt, lxgVertexAttrib_t attr, void* ptr)
{
  glClientActiveTexture(GL_TEXTURE0 + (attr - LUXGFX_VERTEX_ATTRIB_TEXCOORD0));
  glTexCoordPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}

static LUX_INLINE void lxGLAttribPointer(const lxgVertexElementPTR vpt, lxgVertexAttrib_t attr, void* ptr)
{
  if (!VERTEX_INT(vpt)){
    glVertexAttribPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_NORMALIZE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
  else{
    glVertexAttribIPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
}

static LUX_INLINE void lxgVertexPointer_applyFIXED(const lxgVertexElementPTR elem, const lxgStreamHostPTR host, lxgVertexAttrib_t attr, lxgContextPTR ctx)
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
  lxgBuffer_apply(ctx,LUXGL_BUFFER_VERTEX,host->buffer);
  funcs[attr](elem,attr,host->ptr);
}

LUX_API void lxgVertexSetup_applyFIXED(lxgContextPTR ctx)
{
  lxgVertexStatePTR vtx = &ctx->vertex;
  flags32 changed = vtx->declchange & vtx->declvalid;
  flags32 streamchanged = vtx->streamchange;

  if (changed || vtx->streamchange){
    int i;
    for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
      const lxgVertexElementPTR elem = &vtx->setup.element[i];
      uint stream = elem->stream;
      const lxgStreamHostPTR    host = &vtx->setup.streams[stream];
      if (changed & (1<<i) || streamchanged & (1<<stream)){
        lxgVertexPointer_applyFIXED(elem,host,i,ctx);
      }
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
}

static LUX_INLINE void lxgVertexPointer_apply(const lxgVertexElementPTR elem, const lxgStreamHostPTR host, lxgVertexAttrib_t attr, lxgContextPTR ctx)
{
  lxgBuffer_apply(ctx,LUXGL_BUFFER_VERTEX,host->buffer);
  lxGLAttribPointer(elem,attr,host->ptr);
}

LUX_API void lxgVertexSetup_apply(lxgContextPTR ctx)
{
  lxgVertexStatePTR vtx = &ctx->vertex;
  flags32 changed = vtx->declchange & vtx->declvalid;
  flags32 streamchanged = vtx->streamchange;

  if (changed || vtx->streamchange){
    int i;
    for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
      const lxgVertexElementPTR elem = &vtx->setup.element[i];
      uint stream = elem->stream;
      const lxgStreamHostPTR    host = &vtx->setup.streams[stream];
      if (changed & (1<<i) || streamchanged & (1<<stream)){
        lxgVertexPointer_apply(elem,host,i,ctx);
      }
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
}


static LUX_INLINE void lxGLVertexAttribEnable(lxgVertexAttrib_t attr){
  glEnableVertexAttribArray(attr);
}
static LUX_INLINE void lxGLVertexAttribDisable(lxgVertexAttrib_t attr){
  glDisableVertexAttribArray(attr);
}
static LUX_INLINE void lxGLVertexPosEnable(lxgVertexAttrib_t attr){
  glEnableClientState(GL_VERTEX_ARRAY);
}
static LUX_INLINE void lxGLVertexPosDisable(lxgVertexAttrib_t attr){
  glDisableClientState(GL_VERTEX_ARRAY);
}
static LUX_INLINE void lxGLVertexNormalEnable(lxgVertexAttrib_t attr){
  glEnableClientState(GL_NORMAL_ARRAY);
}
static LUX_INLINE void lxGLVertexNormalDisable(lxgVertexAttrib_t attr){
  glDisableClientState(GL_NORMAL_ARRAY);
}
static LUX_INLINE void lxGLVertexColorEnable(lxgVertexAttrib_t attr){
  glEnableClientState(GL_COLOR_ARRAY);
}
static LUX_INLINE void lxGLVertexColorDisable(lxgVertexAttrib_t attr){
  glDisableClientState(GL_COLOR_ARRAY);
}
static LUX_INLINE void lxGLVertexTexEnable(lxgVertexAttrib_t attr){
  glClientActiveTexture(GL_TEXTURE0+attr-LUXGFX_VERTEX_ATTRIB_TEXCOORD0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
static LUX_INLINE void lxGLVertexTexDisable(lxgVertexAttrib_t attr){
  glClientActiveTexture(GL_TEXTURE0+attr-LUXGFX_VERTEX_ATTRIB_TEXCOORD0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

#define CASE_TEST(what)   if( changed & what){
#define CASE_END      }


LUX_API void lxgVertexAttribs_setArrayFIXED(lxgContextPTR ctx, uint numattribs, lxgVertexAttrib_t* attribs, booln state)
{
  static lxGLVertexAttribState_fn* funcsenable[] = {
    lxGLVertexPosEnable,
    lxGLVertexAttribEnable,
    lxGLVertexNormalEnable,
    lxGLVertexColorEnable,
    lxGLVertexAttribEnable,
    lxGLVertexAttribEnable,
    lxGLVertexAttribEnable,
    lxGLVertexAttribEnable,
    lxGLVertexTexEnable,
    lxGLVertexTexEnable,
    lxGLVertexTexEnable,
    lxGLVertexTexEnable,
    lxGLVertexAttribEnable,
    lxGLVertexAttribEnable,
    lxGLVertexAttribEnable,
    lxGLVertexAttribEnable,
  };

  static lxGLVertexAttribState_fn* funcsdisable[] = {
    lxGLVertexPosDisable,
    lxGLVertexAttribDisable,
    lxGLVertexNormalDisable,
    lxGLVertexColorDisable,
    lxGLVertexAttribDisable,
    lxGLVertexAttribDisable,
    lxGLVertexAttribDisable,
    lxGLVertexAttribDisable,
    lxGLVertexTexDisable,
    lxGLVertexTexDisable,
    lxGLVertexTexDisable,
    lxGLVertexTexDisable,
    lxGLVertexAttribDisable,
    lxGLVertexAttribDisable,
    lxGLVertexAttribDisable,
    lxGLVertexAttribDisable,
  };
  uint i;
  if (state){
    for (i = 0; i < numattribs; i++){
      uint attr = (uint)attribs[i];
      funcsenable[attr](attr);
      ctx->vertex.active |= (1<<attribs[i]);
    }
  }
  else{
    flags32 stateflag = 0;
    for (i = 0; i < numattribs; i++){
      uint attr = (uint)attribs[i];
      funcsdisable[attr](attr);
      stateflag |= (1<<attribs[i]);
    }
    ctx->vertex.active &= ~stateflag;
  }
  
}
LUX_API void lxgVertexAttribs_setArray(lxgContextPTR ctx, uint numattribs, lxgVertexAttrib_t* attribs, booln state)
{
  uint i;
  if (state){
    for (i = 0; i < numattribs; i++){
      glEnableVertexAttribArray(attribs[i]);
      ctx->vertex.active |= (1<<attribs[i]);
    }
  }
  else{
    flags32 stateflag = 0;
    for (i = 0; i < numattribs; i++){
      glDisableVertexAttribArray(attribs[i]);
      stateflag |= (1<<attribs[i]);
    }
    ctx->vertex.active &= ~stateflag;
  }
}




LUX_API void  lxgVertexAttribs_applyFIXED(lxgContextPTR ctx, flags32 attribs, flags32 changed)
{

#define ENABLE_VERTEX_TEX(i)    {glClientActiveTexture(GL_TEXTURE0+i); glEnableClientState(GL_TEXTURE_COORD_ARRAY);}
#define DISABLE_VERTEX_TEX(i)   {glClientActiveTexture(GL_TEXTURE0+i); glDisableClientState(GL_TEXTURE_COORD_ARRAY);}


  LUX_DEBUGASSERT((attribs & ~(ctx->vertex.declvalid)) == 0);

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

LUX_API void  lxgVertexAttribs_apply(lxgContextPTR ctx, flags32 attribs, flags32 changed)
{
  int i = 0;

  LUX_DEBUGASSERT((attribs & ~(ctx->vertex.declvalid)) == 0);

  for (; i < LUXGFX_VERTEX_ATTRIBS; i++){
    uint bit = (1<<i);
    if (changed & bit){
      if (attribs & bit)  glEnableVertexAttribArray(i);
      else                glDisableVertexAttribArray(i);
    }
  }
}

LUX_API void lxgVertexDecl_apply(lxgContextPTR ctx, lxgVertexDeclPTR decl)
{
  lxgVertexStatePTR vtx = &ctx->vertex;
  int i;
  vtx->decl = decl;
  vtx->declvalid = decl->available;
  for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
    if (decl->available & (1<<i) && (memcmp(&vtx->setup.element[i],&decl->table[i],sizeof(lxgVertexElement_t)))){
      vtx->setup.element[i] = decl->table[i];
      vtx->declchange |= (1<<i);
    }
  }
}

LUX_API void lxgVertexSetup_setStreams(lxgContextPTR ctx, const lxgVertexDeclPTR decl, const lxgStreamHostPTR streams)
{
  lxgVertexStatePTR vtx = &ctx->vertex;
  flags32 changed = 0;
  uint i;

  LUX_DEBUGASSERT(vtx->decl == decl);

  for (i = 0; i < decl->streams; i++){
    changed |= ((vtx->setup.streams[i].buffer == streams[i].buffer &&
                 vtx->setup.streams[i].ptr == streams[i].ptr)<<i);
    vtx->setup.streams[i] = streams[i];
  }
  vtx->streamchange |= changed;
  vtx->streamvalid = vtx->declvalid;
}


LUX_API void lxgVertexSetup_setStream(lxgContextPTR ctx, uint idx, lxgStreamHostPTR host)
{
  lxgVertexStatePTR vtx = &ctx->vertex;
  booln changed = 0;
  
  if (!host || (!host->buffer && !host->ptr)){
    vtx->streamvalid &= ~(1<<idx);
    return;
  }

  vtx->streamchange |= (host->buffer == vtx->setup.streams[idx].buffer &&
                        host->ptr == vtx->setup.streams[idx].ptr)<<idx;
  vtx->streamvalid |= (1<<idx);
  vtx->setup.streams[idx] = *host;
}


