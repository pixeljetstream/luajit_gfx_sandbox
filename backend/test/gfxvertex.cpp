// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "../_project/project.hpp"
#include <luxgfx/luxgfx.h>

class GfxVertex : public Project
{
private:

  RenderHelper      m_rh;
  GeometryBox       m_box;

  KeyTracker        m_keys;
  GLFWwindow        m_window;
  GLuint            m_texture;
  
  lxgContext_t      m_ctx;
  lxgBuffer_t       m_vbo;
  lxgBuffer_t       m_ibo;
  lxgVertexDecl_t   m_vdecl;
  lxgStreamHost_t   m_stream;
  flags32           m_vattribs;

public:
  GfxVertex() 
    : Project("gfxvertex","../../backend/test/")
  {

  }

  void updateGeometry(int x, int y, int z)
  {
    m_box.update(x,y,z);
  }

  void onInit(GLFWwindow win, int argc, const char** argv) {
    m_window = win;

    // scene
    updateGeometry(4,4,4);
    lxBoundingBox_t bbox;
    lxVector3Set(bbox.min,-1,-1,-1);
    lxVector3Set(bbox.max, 1, 1, 1);
    lxCVector3 up(0,1,0);
    m_rh.init(win,up);
    m_rh.cameraPerspective(&bbox, 30.0f);
    m_rh.cameraOrtho(&bbox);

    m_texture = RenderHelper::generateUVTexture(256,256);


    // context
    lxgContext_init(&m_ctx);

    // vertices
    size_t vsize = sizeof(VertexDefault) * m_box.numVertices();
    lxgBuffer_init(&m_vbo, &m_ctx, LUXGL_STATIC_DRAW, vsize, NULL);
    uint voffset = lxgBuffer_alloc(&m_vbo, vsize, sizeof(VertexDefault));
    VertexDefault* vertices = (VertexDefault*)lxgBuffer_map(&m_vbo, LUXGFX_ACCESS_WRITEDISCARDALL, NULL);
    m_box.fillVerts(vertices);
    lxgBuffer_unmap(&m_vbo);
    
    // indices
    size_t trisize  = sizeof(uint32) * m_box.numIndicesTris();
    size_t linesize = sizeof(uint32) * m_box.numIndicesLines();
    lxgBuffer_init(&m_ibo, &m_ctx, LUXGL_STATIC_DRAW, trisize + linesize, NULL);

    uint trioffset = lxgBuffer_alloc(&m_ibo, trisize, sizeof(uint32));
    uint32* indicesTris = (uint32*)lxgBuffer_mapRange(&m_ibo, trioffset, trisize,
      LUXGFX_ACCESS_WRITEDISCARDALL,0,0,NULL);
    m_box.fillIndices(LUX_MESH_INDEX_UINT32, indicesTris, voffset / sizeof(VertexDefault));
    lxgBuffer_unmap(&m_ibo);

    uint lineoffset = lxgBuffer_alloc(&m_ibo, linesize, sizeof(uint32));
    uint32* indicesLines = (uint32*)lxgBuffer_mapRange(&m_ibo, lineoffset, linesize,
      LUXGFX_ACCESS_WRITEDISCARDALL,0,0,NULL);
    m_box.fillIndices(LUX_MESH_INDEX_UINT32, indicesLines, voffset / sizeof(VertexDefault), LUX_TRUE);
    lxgBuffer_unmap(&m_ibo);

    // vertexdecl
    m_vattribs = 
      lxgVertexAttrib_bit(LUXGFX_VERTEX_ATTRIB_POS) |
      lxgVertexAttrib_bit(LUXGFX_VERTEX_ATTRIB_NORMAL) |
      lxgVertexAttrib_bit(LUXGFX_VERTEX_ATTRIB_TEXCOORD0);

    m_vdecl.available = m_vattribs;
    m_vdecl.streams = 1;
    VertexDefault* vertex = NULL;
    m_vdecl.table[LUXGFX_VERTEX_ATTRIB_POS] = 
      lxgVertexElement_set(3,LUX_SCALAR_FLOAT32,LUX_FALSE,LUX_FALSE,sizeof(VertexDefault),(size_t)&vertex->pos,0);
    m_vdecl.table[LUXGFX_VERTEX_ATTRIB_NORMAL] = 
      lxgVertexElement_set(3,LUX_SCALAR_FLOAT32,LUX_TRUE,LUX_FALSE,sizeof(VertexDefault),(size_t)&vertex->normal,0);
    m_vdecl.table[LUXGFX_VERTEX_ATTRIB_TEXCOORD0] = 
      lxgVertexElement_set(2,LUX_SCALAR_FLOAT32,LUX_FALSE,LUX_FALSE,sizeof(VertexDefault),(size_t)&vertex->uv,0);
    
    // stream
    m_stream.buffer = &m_vbo;
    m_stream.offset = 0;
    m_stream.len    = vsize;



  }

  void logic(int width, int height)
  {
    m_keys.update(m_window);

  }

  void onDraw(int width, int height) {
    logic(width,height);

    lxCVector4 lightpos(0,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    m_rh.updateProjection(width,height);
    m_rh.setCameraGL();
    lxgContext_clearVertexState(&m_ctx);

    glBindTexture(GL_TEXTURE_2D,m_texture);
    glEnable(GL_TEXTURE_2D);
    
    lxgContext_checkedVertexDecl(&m_ctx, &m_vdecl);
    lxgContext_setVertexStream(&m_ctx, 0, &m_stream);
    lxgContext_checkedVertexFIXED(&m_ctx);
    lxgContext_checkedVertexAttribFIXED(&m_ctx, m_vattribs);
    lxgBuffer_bind(&m_ibo, LUXGL_BUFFER_INDEX);
    glDrawElements(GL_TRIANGLES, m_box.numIndicesTris(), GL_UNSIGNED_INT, NULL);

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
  }

};

static GfxVertex test;

