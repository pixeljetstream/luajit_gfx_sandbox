// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LUXTEST_TEST_H__
#define __LUXTEST_TEST_H__

#include <GL/glew.h>
#include <GL/glfw3.h>

#include <luxmath/luxmath.h>
#include <luxmath/basetypes.hpp>

#include <string.h>
#include <vector>

class Test;
class TestManager {
public:
  static TestManager& Get(){
    static TestManager mgr;
    return mgr;
  }

  void addTest(Test* test){
    m_tests.push_back(test);
  }

  Test* getTest(const char* name);

private:
  std::vector<Test*>  m_tests;

};

class RenderHelper {
public:

  static GLuint generateUVTexture(int w, int h);

  void init(GLFWwindow win, const lxCVector3& up);
  void cameraOrtho(lxBoundingBox_t* bbox);
  void cameraPerspective(lxBoundingBox_t* bbox, float fovdeg);
  void updateProjection(int width, int height);
  void setCameraGL();
  void doArcBall();

private:

  GLFWwindow  m_window;

  float       m_ortho;
  float       m_fov;
  lxCMatrix44 m_viewmatrix;
  lxCMatrix44 m_projmatrix;
  lxCVector3  m_sceneCenter;
  lxCVector3  m_sceneSize;

  lxCVector3  m_lastmpos;
  float       m_movespeed;
  float       m_rotspeed;
  lxCVector3  m_up;
};

class Test
{
public:
  Test(const char* name) : m_name(name){
    TestManager::Get().addTest(this);
  }

  const char* getName() const {
    return m_name;
  }
  virtual void onDraw(int width, int height) {}
  virtual void onInit(GLFWwindow window) {}
  virtual void onDeInit() {}

private:
  const char*   m_name;
};

inline Test* TestManager::getTest(const char* name){
  for (size_t i = 0; i < m_tests.size(); ++i){
    if (strcmp(name,m_tests[i]->getName())==0){
      return m_tests[i];
    }
  }
  return NULL;
}




#endif

