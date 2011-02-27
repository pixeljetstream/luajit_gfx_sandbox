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

typedef void (lxGLVertexAttrib_fn)(const lxGFXVertexElementPTR, lxGFXVertexAttrib_t, void* ptr );
typedef void (lxGLVertexAttribState_fn)(lxGFXVertexAttrib_t );

static LUX_INLINE void VGLVertexPointer(const lxGFXVertexElementPTR vpt, lxGFXVertexAttrib_t attr, void* ptr)
{
  glVertexPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void VGLNormalPointer(const lxGFXVertexElementPTR vpt, lxGFXVertexAttrib_t attr, void* ptr)
{
  glNormalPointer(VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void VGLColorPointer(const lxGFXVertexElementPTR vpt, lxGFXVertexAttrib_t attr, void* ptr)
{
  glColorPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}
static LUX_INLINE void VGLTexCoordPointer(const lxGFXVertexElementPTR vpt, lxGFXVertexAttrib_t attr, void* ptr)
{
  glClientActiveTexture(GL_TEXTURE0 + (attr - LUXGFX_VERTEX_ATTRIB_TEXCOORD0));
  glTexCoordPointer(VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
}

static LUX_INLINE void VGLAttribPointer(const lxGFXVertexElementPTR vpt, lxGFXVertexAttrib_t attr, void* ptr)
{
  if (!VERTEX_INT(vpt)){
    glVertexAttribPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_NORMALIZE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
  else{
    glVertexAttribIPointer((uint)attr,VERTEX_CNT(vpt),VERTEX_TYPE(vpt),VERTEX_STRIDE(vpt),VERTEX_PTR(vpt));
  }
}

static void lxGFXVertexPointer_apply(const lxGFXVertexElementPTR elem, const lxGFXStreamHostPTR host, lxGFXVertexAttrib_t attr, lxGFXContextPTR ctx)
{
  lxGFXBuffer_apply(ctx,LUXGL_BUFFER_VERTEX,host->buffer);
#ifdef LUX_RENDERBACKEND_OPENGL3
  VGLAttribPointer(elem,attr,host->ptr);
#else
  {
    static lxGLVertexAttrib_fn* funcs[] = {
      VGLVertexPointer,
      VGLAttribPointer,
      VGLNormalPointer,
      VGLColorPointer,
      VGLAttribPointer,
      VGLAttribPointer,
      VGLAttribPointer,
      VGLAttribPointer,
      VGLTexCoordPointer,
      VGLTexCoordPointer,
      VGLTexCoordPointer,
      VGLTexCoordPointer,
      VGLAttribPointer,
      VGLAttribPointer,
      VGLAttribPointer,
      VGLAttribPointer,
    };
    funcs[attr](elem,attr,host->ptr);
  }
#endif
}

static LUX_INLINE void VGLVertexAttribEnable(lxGFXVertexAttrib_t attr){
  glEnableVertexAttribArray(attr);
}
static LUX_INLINE void VGLVertexAttribDisable(lxGFXVertexAttrib_t attr){
  glDisableVertexAttribArray(attr);
}
static LUX_INLINE void VGLVertexPosEnable(lxGFXVertexAttrib_t attr){
  glEnableClientState(GL_VERTEX_ARRAY);
}
static LUX_INLINE void VGLVertexPosDisable(lxGFXVertexAttrib_t attr){
  glDisableClientState(GL_VERTEX_ARRAY);
}
static LUX_INLINE void VGLVertexNormalEnable(lxGFXVertexAttrib_t attr){
  glEnableClientState(GL_NORMAL_ARRAY);
}
static LUX_INLINE void VGLVertexNormalDisable(lxGFXVertexAttrib_t attr){
  glDisableClientState(GL_NORMAL_ARRAY);
}
static LUX_INLINE void VGLVertexColorEnable(lxGFXVertexAttrib_t attr){
  glEnableClientState(GL_COLOR_ARRAY);
}
static LUX_INLINE void VGLVertexColorDisable(lxGFXVertexAttrib_t attr){
  glDisableClientState(GL_COLOR_ARRAY);
}
static LUX_INLINE void VGLVertexTexEnable(lxGFXVertexAttrib_t attr){
  glClientActiveTexture(GL_TEXTURE0+attr-LUXGFX_VERTEX_ATTRIB_TEXCOORD0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
static LUX_INLINE void VGLVertexTexDisable(lxGFXVertexAttrib_t attr){
  glClientActiveTexture(GL_TEXTURE0+attr-LUXGFX_VERTEX_ATTRIB_TEXCOORD0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

#define CASE_TEST(what)   if( changed & what){
#define CASE_END      }

#ifdef LUX_RENDERBACKEND_OPENGL3

#define ENABLE_VERTEX_POS()     glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_POS);
#define DISABLE_VERTEX_POS()    glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_POS)

#define ENABLE_VERTEX_NORMAL()    glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_NORMAL)
#define DISABLE_VERTEX_NORMAL()   glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_NORMAL)

#define ENABLE_VERTEX_COLOR()   glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_COLOR)
#define DISABLE_VERTEX_COLOR()    glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_COLOR)

#define ENABLE_VERTEX_TEX(i)    glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_TEXCOORD0+i)
#define DISABLE_VERTEX_TEX(i)   glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_TEXCOORD0+i)

#else

#define ENABLE_VERTEX_POS()     glEnableClientState(GL_VERTEX_ARRAY)
#define DISABLE_VERTEX_POS()    glDisableClientState(GL_VERTEX_ARRAY)

#define ENABLE_VERTEX_NORMAL()    glEnableClientState(GL_NORMAL_ARRAY)
#define DISABLE_VERTEX_NORMAL()   glDisableClientState(GL_NORMAL_ARRAY)

#define ENABLE_VERTEX_COLOR()   glEnableClientState(GL_COLOR_ARRAY)
#define DISABLE_VERTEX_COLOR()    glDisableClientState(GL_COLOR_ARRAY)

#define ENABLE_VERTEX_TEX(i)    {glClientActiveTexture(GL_TEXTURE0+i); glEnableClientState(GL_TEXTURE_COORD_ARRAY);}
#define DISABLE_VERTEX_TEX(i)   {glClientActiveTexture(GL_TEXTURE0+i); glDisableClientState(GL_TEXTURE_COORD_ARRAY);}

#endif


LUX_API void lxGFXVertexAttribs_setArray(lxGFXContextPTR ctx, uint numattribs, lxGFXVertexAttrib_t* attribs, booln state)
{

#ifndef LUX_RENDERBACKEND_OPENGL3
  static lxGLVertexAttribState_fn* funcsenable[] = {
    VGLVertexPosEnable,
    VGLVertexAttribEnable,
    VGLVertexNormalEnable,
    VGLVertexColorEnable,
    VGLVertexAttribEnable,
    VGLVertexAttribEnable,
    VGLVertexAttribEnable,
    VGLVertexAttribEnable,
    VGLVertexTexEnable,
    VGLVertexTexEnable,
    VGLVertexTexEnable,
    VGLVertexTexEnable,
    VGLVertexAttribEnable,
    VGLVertexAttribEnable,
    VGLVertexAttribEnable,
    VGLVertexAttribEnable,
  };

  static lxGLVertexAttribState_fn* funcsdisable[] = {
    VGLVertexPosDisable,
    VGLVertexAttribDisable,
    VGLVertexNormalDisable,
    VGLVertexColorDisable,
    VGLVertexAttribDisable,
    VGLVertexAttribDisable,
    VGLVertexAttribDisable,
    VGLVertexAttribDisable,
    VGLVertexTexDisable,
    VGLVertexTexDisable,
    VGLVertexTexDisable,
    VGLVertexTexDisable,
    VGLVertexAttribDisable,
    VGLVertexAttribDisable,
    VGLVertexAttribDisable,
    VGLVertexAttribDisable,
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
  
#else
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

#endif
}

LUX_API void  lxGFXVertexAttribs_apply(lxGFXContextPTR ctx, flags32 attribs, flags32 changed)
{

  LUX_DEBUGASSERT((attribs & ~(ctx->vertex.declvalid)) == 0);

  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_POS) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_POS) & attribs)  ENABLE_VERTEX_POS();
    else      DISABLE_VERTEX_POS();
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_ATTR1) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_ATTR1) & attribs)  glEnableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR1);
    else      glDisableVertexAttribArray(LUXGFX_VERTEX_ATTRIB_ATTR1);
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_NORMAL) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_NORMAL) & attribs) ENABLE_VERTEX_NORMAL();
    else      DISABLE_VERTEX_NORMAL();
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_COLOR) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_COLOR) & attribs)  ENABLE_VERTEX_COLOR();
    else      DISABLE_VERTEX_COLOR();
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
    else      {DISABLE_VERTEX_TEX(GL_TEXTURE0)}
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD1) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD1) & attribs)  {ENABLE_VERTEX_TEX(1)}
    else      {DISABLE_VERTEX_TEX(GL_TEXTURE1)}
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD2) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD2) & attribs)  {ENABLE_VERTEX_TEX(2)}
    else      {DISABLE_VERTEX_TEX(GL_TEXTURE2)}
  CASE_END;
  CASE_TEST( (1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD3) )
    if ((1<<LUXGFX_VERTEX_ATTRIB_TEXCOORD3) & attribs)  {ENABLE_VERTEX_TEX(3)}
    else      {DISABLE_VERTEX_TEX(GL_TEXTURE3)}
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

  ctx->vertex.active = attribs;
}

LUX_API void lxGFXVertexDecl_apply(lxGFXContextPTR ctx, lxGFXVertexDeclPTR decl)
{
  lxGFXVertexStatePTR vtx = &ctx->vertex;
  int i;
  vtx->decl = decl;
  vtx->declvalid = decl->available;
  for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
    if (decl->available & (1<<i) && (memcmp(&vtx->setup.element[i],&decl->table[i],sizeof(lxGFXVertexElement_t)))){
      vtx->setup.element[i] = decl->table[i];
      vtx->declchange |= (1<<i);
    }
  }
}

LUX_API void lxGFXVertexSetup_setStreams(lxGFXContextPTR ctx, const lxGFXVertexDeclPTR decl, const lxGFXStreamHostPTR streams)
{
  lxGFXVertexStatePTR vtx = &ctx->vertex;
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


LUX_API void lxGFXVertexSetup_setStream(lxGFXContextPTR ctx, uint idx, lxGFXStreamHostPTR host)
{
  lxGFXVertexStatePTR vtx = &ctx->vertex;
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

LUX_API void lxGFXVertexSetup_apply(lxGFXContextPTR ctx)
{
  lxGFXVertexStatePTR vtx = &ctx->vertex;
  flags32 changed = vtx->declchange & vtx->declvalid;
  flags32 streamchanged = vtx->streamchange;

  if (changed || vtx->streamchange){
    int i;
    for (i = 0; i < LUXGFX_VERTEX_ATTRIBS; i++){
      const lxGFXVertexElementPTR elem = &vtx->setup.element[i];
      uint stream = elem->stream;
      const lxGFXStreamHostPTR    host = &vtx->setup.streams[stream];
      if (changed & (1<<i) || streamchanged & (1<<stream)){
        lxGFXVertexPointer_apply(elem,host,i,ctx);
      }
    }
  }

  vtx->declchange = 0;
  vtx->streamchange = 0;
}
