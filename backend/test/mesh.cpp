// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "test.hpp"
#include <luxbackend/meshbase.h>

class Geometry {
public:

  Geometry() {}

  std::vector<lxCVector3>  pos;
  std::vector<lxCVector3>  normal;
  std::vector<lxCVector2>  uv;
  std::vector<uint32>     indicesTris;
  std::vector<uint32>     indicesLines;

  void allocMem(int numVertices, int numIndicesTris, int numIndicesLines) {
    pos.resize(numVertices);
    normal.resize(numVertices);
    uv.resize(numVertices);
    indicesTris.resize(numIndicesTris);
    indicesLines.resize(numIndicesLines);
  }

  void drawVA(booln outline){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,GL_FLOAT,0, &pos[0]);
    glNormalPointer(GL_FLOAT,0, &normal[0]);
    glTexCoordPointer(2,GL_FLOAT,0, &uv[0]);
    if (!outline){
      glDrawElements(GL_TRIANGLES, indicesTris.size(), GL_UNSIGNED_INT, &indicesTris[0]);
    }
    else{
      glDrawElements(GL_LINES, indicesLines.size(), GL_UNSIGNED_INT, &indicesLines[0]);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
};

class MeshTest : public Test
{
private:
  static const int GEOMETRIES = 5;

  RenderHelper  m_rh;
  int           m_model;
  int           m_wire;
  Geometry      m_geometries[GEOMETRIES];

  GLFWwindow    m_window;
  int           m_lastSpace;
  int           m_lastWire;
  GLuint        m_texture;

public:
  MeshTest() 
    : Test("mesh")
    , m_lastWire(0)
    , m_lastSpace(0)
    , m_wire(0)
    , m_model(0)
  {

  }

  void updateGeometry(int x, int y, int z)
  {
    int subdiv[3] = {x,y,z};

    int vertices;
    int indicesTris;
    int indicesLines;

    Geometry& Plane     = m_geometries[0];
    Geometry& Box       = m_geometries[1];
    Geometry& Disc      = m_geometries[2];
    Geometry& Cylinder  = m_geometries[3];
    Geometry& Sphere    = m_geometries[4];

    lxMeshPlane_getMemsize(subdiv,&vertices,&indicesTris,&indicesLines);
    Plane.allocMem(vertices,indicesTris,indicesLines);
    lxMeshPlane_initTriangles(subdiv,(lxVector3*)&Plane.pos[0],(lxVector3*)&Plane.normal[0],(lxVector2*)&Plane.uv[0],&Plane.indicesTris[0]);
    lxMeshPlane_initOutline(subdiv,&Plane.indicesLines[0]);

    lxMeshBox_getMemsize(subdiv,&vertices,&indicesTris,&indicesLines);
    Box.allocMem(vertices,indicesTris,indicesLines);
    lxMeshBox_initTriangles(subdiv,(lxVector3*)&Box.pos[0],(lxVector3*)&Box.normal[0],(lxVector2*)&Box.uv[0],&Box.indicesTris[0]);
    lxMeshBox_initOutline(subdiv,&Box.indicesLines[0]);

    lxMeshDisc_getMemsize(subdiv,&vertices,&indicesTris,&indicesLines);
    Disc.allocMem(vertices,indicesTris,indicesLines);
    lxMeshDisc_initTriangles(subdiv,(lxVector3*)&Disc.pos[0],(lxVector3*)&Disc.normal[0],(lxVector2*)&Disc.uv[0],&Disc.indicesTris[0]);
    lxMeshDisc_initOutline(subdiv,&Disc.indicesLines[0]);

    lxMeshCylinder_getMemsize(subdiv,&vertices,&indicesTris,&indicesLines);
    Cylinder.allocMem(vertices,indicesTris,indicesLines);
    lxMeshCylinder_initTriangles(subdiv,(lxVector3*)&Cylinder.pos[0],(lxVector3*)&Cylinder.normal[0],(lxVector2*)&Cylinder.uv[0],&Cylinder.indicesTris[0]);
    lxMeshCylinder_initOutline(subdiv,&Cylinder.indicesLines[0]);

    lxMeshSphere_getMemsize(subdiv,&vertices,&indicesTris,&indicesLines);
    Sphere.allocMem(vertices,indicesTris,indicesLines);
    lxMeshSphere_initTriangles(subdiv,(lxVector3*)&Sphere.pos[0],(lxVector3*)&Sphere.normal[0],(lxVector2*)&Sphere.uv[0],&Sphere.indicesTris[0]);
    lxMeshSphere_initOutline(subdiv,&Sphere.indicesLines[0]);
  }

  void onInit(GLFWwindow win) {
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

  void onDraw(int width, int height) {
    lxCVector4 lightpos(0,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    glDisable(GL_CULL_FACE);

    m_rh.updateProjection(width,height);
    m_rh.setCameraGL();

    int space = glfwGetKey(m_window,GLFW_KEY_SPACE);
    if (!m_lastSpace && space){
      m_model++;
      if (m_model >= GEOMETRIES*2){
        int scale = m_model/(GEOMETRIES*2);
        scale++;
        updateGeometry(4*scale,2*scale,1*scale);
      }
    }
    m_lastSpace = space;
    int wire  = glfwGetKey(m_window,GLFW_KEY_W);
    if (!m_lastWire && wire){
      m_wire = !m_wire;
    }
    m_lastWire  = wire;

    glBindTexture(GL_TEXTURE_2D,m_texture);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK,m_wire ? GL_LINE : GL_FILL);
    m_geometries[(m_model/2) % GEOMETRIES].drawVA(m_model % 2);
    glBindTexture(GL_TEXTURE_2D,0);
  }

};

MeshTest test;

