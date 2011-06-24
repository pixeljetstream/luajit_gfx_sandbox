// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "test.hpp"

class MeshTest : public Test
{
private:

  RenderHelper  m_rh;
  int           m_model;
  int           m_wire;
  GeometryPlane     m_plane;
  GeometryBox       m_box;
  GeometryDisc      m_disc;
  GeometryCylinder  m_cylinder;
  GeometrySphere    m_sphere;
  std::vector<Geometry*>   m_geometries;
  int                      m_numGeometries;

  KeyTracker    m_keys;
  GLFWwindow    m_window;
  GLuint        m_texture;

public:
  MeshTest() 
    : Test("mesh")
    , m_wire(0)
    , m_model(0)
  {
    m_geometries.push_back(&m_plane);
    m_geometries.push_back(&m_box);
    m_geometries.push_back(&m_disc);
    m_geometries.push_back(&m_cylinder);
    m_geometries.push_back(&m_sphere);
    m_numGeometries = (int)m_geometries.size();
    m_keys.add(GLFW_KEY_SPACE);
    m_keys.add(GLFW_KEY_W);
  }

  void updateGeometry(int x, int y, int z)
  {
    m_plane.update(x,y);
    m_box.update(x,y,z);
    m_disc.update(x,y);
    m_cylinder.update(x,y,z);
    m_sphere.update(x,y);
  }

  void onInit(GLFWwindow win, int argc, const char** argv) {
    m_window = win;

    updateGeometry(4,2,1);

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
    if (m_keys.onPress(GLFW_KEY_SPACE)){
      m_model++;
      if (m_model >= m_numGeometries*2){
        int scale = m_model/(m_numGeometries*2);
        scale++;
        updateGeometry(4*scale,2*scale,1*scale);
      }
    }
    if (m_keys.onPress(GLFW_KEY_W)){
      m_wire = !m_wire;
    }
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
    glPolygonMode(GL_FRONT_AND_BACK,m_wire ? GL_LINE : GL_FILL);
    m_geometries[(m_model/2) % m_numGeometries]->drawVA(m_model % 2);
    glBindTexture(GL_TEXTURE_2D,0);
  }

};

static MeshTest test;

