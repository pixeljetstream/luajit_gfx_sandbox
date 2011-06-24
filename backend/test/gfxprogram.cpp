// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "test.hpp"


class GfxProgram : public Test
{
private:

  RenderHelper      m_rh;
  GeometryBox       m_box;

  KeyTracker    m_keys;
  GLFWwindow    m_window;
  GLuint        m_texture;

public:
  GfxProgram() 
    : Test("gfxprogram")
  {

  }

  void updateGeometry(int x, int y, int z)
  {
    m_box.update(x,y,z);
  }

  void onInit(GLFWwindow win, int argc, const char** argv) {
    m_window = win;

    updateGeometry(4,4,4);

    lxBoundingBox_t bbox;
    lxVector3Set(bbox.min,-1,-1,-1);
    lxVector3Set(bbox.max, 1, 1, 1);
    lxCVector3 up(0,1,0);
    m_rh.init(win,up);
    m_rh.cameraPerspective(&bbox, 30.0f);
    m_rh.cameraOrtho(&bbox);

    m_texture = RenderHelper::generateUVTexture(256,256);
  }

  void logic(int width, int height)
  {
    m_keys.update(m_window);

  }

  void onDraw(int width, int height) {
    logic(width,height);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    lxCVector4 lightpos(0,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    m_rh.updateProjection(width,height);
    m_rh.setCameraGL();

    glBindTexture(GL_TEXTURE_2D,m_texture);
    glEnable(GL_TEXTURE_2D);
    m_box.drawVA();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
  }

};

static GfxProgram test;

