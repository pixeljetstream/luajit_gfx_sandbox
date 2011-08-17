// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include "../_project/project.hpp"
#include <luxgfx/luxgfx.h>

class GfxTexture : public Project
{
private:

  RenderHelper      m_rh;
  GeometryBox       m_box;

  KeyTracker    m_keys;
  GLFWwindow    m_window;
  
  lxgContext_t  m_ctx;
  lxgTexture_t  m_tex;
  lxgSampler_t  m_samplerNearest;
  lxgSampler_t  m_samplerTriLinear;
  lxgSamplerPTR m_sampler;

public:
  GfxTexture() 
    : Project("gfxtexture","../../backend/test/")
  {
      m_keys.add(GLFW_KEY_SPACE);
  }

  void updateGeometry(int x, int y, int z)
  {
    m_box.update(x,y,z);
  }

  int onInit(GLFWwindow win, int argc, const char** argv) {
    m_window = win;

    updateGeometry(4,4,4);

    lxBoundingBox_t bbox;
    lxVector3Set(bbox.min,-1,-1,-1);
    lxVector3Set(bbox.max, 1, 1, 1);
    lxCVector3 up(0,1,0);
    m_rh.init(win,up);
    m_rh.cameraPerspective(&bbox, 30.0f);
    m_rh.cameraOrtho(&bbox);

    lxgContext_init(&m_ctx);

    lxgTexture_init(&m_tex, &m_ctx);
    int w = 16;
    int h = 16;
    lxgTexture_setup(&m_tex, LUXGL_TEXTURE_2D, LUXGFX_TEXTURECHANNEL_RGB, LUXGFX_TEXTUREDATA_UNORM8,w,h,0,0,LUXGFX_TEXTUREFLAG_AUTOMIPMAP);
    size_t buffersize = w * h * sizeof(lxCVector3);
    lxCVector3* buffer = new lxCVector3[w * h];
    RenderHelper::generateUVData(w,h,buffer);
    lxgTextureUpdate_t update = {{0,0,0},{0,0,0},{w,h,0}};
    lxgTexture_readData(&m_tex,&update,0,GL_FLOAT,GL_RGB,buffer,buffersize);
    delete[] buffer;
    lxGLErrorCheck();

    lxgSampler_init(&m_samplerNearest, &m_ctx);
    m_samplerNearest.filter = LUXGFX_SAMPLERFILTER_NEAREST;
    lxgSampler_update(&m_samplerNearest);

    lxgSampler_init(&m_samplerTriLinear, &m_ctx);
    m_samplerTriLinear.filter = LUXGFX_SAMPLERFILTER_MIPMAP_LINEAR;
    lxgSampler_update(&m_samplerTriLinear);

    m_sampler = &m_samplerNearest;
    return 0;
  }

  void logic(int width, int height)
  {
    m_keys.update(m_window);
    if (m_keys.onPress(GLFW_KEY_SPACE)){
      m_sampler = m_sampler == &m_samplerNearest ? &m_samplerTriLinear : &m_samplerNearest;
    }
  }

  int onDraw(int width, int height) {
    logic(width,height);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    lxGLErrorCheck();

    lxCVector4 lightpos(0,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    lxGLErrorCheck();

    m_rh.updateProjection(width,height);
    m_rh.setCameraGL();
    lxGLErrorCheck();

    lxgContext_applyTexture( &m_ctx,&m_tex, 0);
    lxgContext_applySampler( &m_ctx,m_sampler, 0);
    glEnable(GL_TEXTURE_2D);
    m_box.drawVA();
    glDisable(GL_TEXTURE_2D);
    lxgContext_applyTexture( &m_ctx,NULL,0);
    lxgContext_applySampler( &m_ctx,NULL,0);

    return 0;
  }

};

static GfxTexture test;

