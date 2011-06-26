// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "test.hpp"
#include <luxgfx/luxgfx.h>

class GfxProgram : public Test
{
  static const int PARAMSETS = 4;

  struct ParamSet{
    lxgTexture_t  texture;
    lxgBuffer_t   buffer;
    lxCMatrix44   matrix;
    GLuint        subroutine[2];
    void**        datas;
  };


private:

  RenderHelper      m_rh;
  GeometryBox       m_box;

  KeyTracker    m_keys;
  GLFWwindow    m_window;

  lxgContext_t            m_ctx;
  RenderProgram           m_prog;
  ParamSet                m_paramsets[PARAMSETS];

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

    lxgContext_init(&m_ctx);

    m_prog.init(&m_ctx);
    m_prog.addStageProgram(LUXGFX_STAGE_VERTEX,RESFILENAME("gfxprogram.vert").c_str(),NULL);
    m_prog.addStageProgram(LUXGFX_STAGE_FRAGMENT,RESFILENAME("gfxprogram.frag").c_str(),NULL);
    m_prog.finish();

    // TODO init parmasets
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

    lxgProgram_apply(&m_prog.m_program, &m_ctx);
    for (int i = 0; i < PARAMSETS; i++){
      lxgProgram_applyParameters(&m_prog.m_program, &m_ctx, m_prog.m_params.size(), &m_prog.m_paramPtrs[0], m_paramsets[i].datas);
      m_box.drawVA();
    }
    lxgProgram_apply(NULL, &m_ctx);
  }

};

static GfxProgram test;

