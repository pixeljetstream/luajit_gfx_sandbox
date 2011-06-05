// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "test.hpp"
#include <luxmath/bounding.h>

void RenderHelper::init(GLFWwindow win, const lxCVector3& up)
{
  m_window = win;
  m_up = up;

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glStencilMask(255);
  glClearStencil(0);


  float mat_specular[4] = {0.2f, 0.2f, 0.2f, 1.0f};
  float mat_shininess[1] = {50.0f};
  float mat_ambient[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  float mat_diffuse[4] = {0.7f, 0.7f, 0.7f, 1.0f};

  float light_ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
  float light_diffuse[4] = {0.95f, 0.95f, 0.95f, 1.0f};

  glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);

  glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);

  glEnable(GL_COLOR_MATERIAL);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0);
}

void RenderHelper::cameraOrtho( lxBoundingBox_t* bbox )
{
  lxBoundingBox_toCenter(bbox,m_sceneCenter,m_sceneSize);
  float length = lxVector3LengthA(m_sceneSize);
  m_ortho = length * 1.25f;
  m_fov   = 0.0f;
  lxCVector3  offset(-1,1,-1);
  lxCVector3  campos;

  lxMatrix44Identity(m_viewmatrix);
  //m_viewmatrix[14] = -length * 4.0f;

  lxVector3NormalizedA(offset);
  lxVector3ScaledAdd(campos,m_sceneCenter,length * 4.0f, offset);
  lxMatrix44LookAt(m_viewmatrix,campos,m_sceneCenter,m_up);
}

void RenderHelper::cameraPerspective( lxBoundingBox_t* bbox, float fovdeg )
{
  lxBoundingBox_toCenter(bbox,m_sceneCenter,m_sceneSize);
  float length = lxVector3LengthA(m_sceneSize) * 0.5f;
  lxCVector3  offset(-0.33f,0.33f,-0.66f);
  lxCVector3  campos;
  m_ortho = 0.0f;
  m_fov   = fovdeg;

  // tan = opp/adj
  length = length / tanf(LUX_DEG2RAD(fovdeg) * 0.5f);
  
  lxMatrix44Identity(m_viewmatrix);

  lxVector3NormalizedA(offset);
  lxVector3ScaledAdd(campos,m_sceneCenter,length, offset);
  lxMatrix44LookAt(m_viewmatrix,campos,m_sceneCenter,m_up);
}

void RenderHelper::updateProjection( int width, int height )
{
  float size = lxVector3LengthA(m_sceneSize);
  if (m_ortho){
    lxMatrix44Ortho(m_projmatrix, m_ortho, size / 4096.0f, size * 16.0f, (float)width / (float) height);
  }
  else{
    lxMatrix44Perspective(m_projmatrix, m_fov, size / 4096.0f, size * 16.0f, (float)width / (float) height);
  }
}

void RenderHelper::setCameraGL()
{
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(m_projmatrix);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(m_viewmatrix);
}

void RenderHelper::doArcBall()
{

}

GLuint RenderHelper::generateUVTexture( int w, int h )
{
  GLuint tex;
  glGenTextures(1,&tex);
  glBindTexture(GL_TEXTURE_2D,tex);
  glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);

  lxCVector3* pixels = new lxCVector3[w * h];

  float hdiv = 1.0f/(float)(LUX_MAX(1,h-1));
  float wdiv = 1.0f/(float)(LUX_MAX(1,w-1));

  for (int y = 0; y < h; y++){
    for (int x = 0; x < w; x++){
      float xpos = (float)x * wdiv;
      float ypos = (float)y * hdiv;
      float len  = sqrtf(xpos*xpos + ypos*ypos);
      float add  = 1.0f - LUX_MIN(1.0f,len);
      pixels[y*w +x].Set( xpos + add, ypos + add, add);
    }
  }

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_FLOAT,pixels);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_ARB);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT_ARB);
  glBindTexture(GL_TEXTURE_2D,0);
  delete [] pixels;

  return tex;
}
