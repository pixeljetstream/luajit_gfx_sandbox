// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include "../_project/project.hpp"
#include <luxinia/luxgfx/luxgfx.h>

class GfxProgram : public Project
{
  static const int PARAMSETS = 4;

  enum Params{
    PARAM_MATRICES,
    PARAM_XCOLORPTR,
    PARAM_YCOLORBUFFER,
    PARAM_ZTEX,
    PARAM_SHUFFLEFUNC,
    PARAMS,
  };

  struct ParamSet{

    lxgProgramParameterPTR params[PARAMS];
    const void*            datas[PARAMS];

    lxCVector4          vecX;
    lxCVector4          vecY;
    lxgBuffer_t         bufferX;
    lxgBuffer_t         bufferY;
    lxgBufferPTR        bufferYptr;
    lxgTexture_t        textureZ;
    lxgTexturePTR       textureZptr;
    lxCMatrix44         matrices[2];
    lxgSubroutineKey    shuffle;


    void init(lxgContextPTR ctx, RenderProgram &prog) {
      lxgTexture_init(&textureZ, ctx);
      int w = 1;
      int h = 1;
      lxgTexture_setup(&textureZ, LUXGL_TEXTURE_2D, LUXGFX_TEXTURECHANNEL_RGB, LUXGFX_TEXTUREDATA_UNORM8,w,h,0,0,LUXGFX_TEXTUREFLAG_AUTOMIPMAP);
      size_t buffersize = w * h * sizeof(lxCVector3);
      lxCVector3* buffer = new lxCVector3[w * h];
      buffer[0].Set(0,0,1.0);
      lxgTextureUpdate_t update = {{0,0,0},{0,0,0},{w,h,0}};
      lxgTexture_readData(&textureZ,&update,0,GL_FLOAT,GL_RGB,buffer,buffersize);
      delete[] buffer;
      textureZptr = &textureZ;
      lxGLErrorCheck();

      vecX.Set(1.0f,0.0f,0.0f,0.0f);
      vecY.Set(0.0f,1.0f,0.0f,0.0f);
      lxgBuffer_init(&bufferX, ctx, LUXGL_STATIC_DRAW,sizeof(lxCVector4),&vecX);
      lxgBuffer_residentNV(&bufferX,LUXGFX_ACCESS_READ);
      lxgBuffer_init(&bufferY, ctx, LUXGL_STATIC_DRAW,sizeof(lxCVector4),&vecY);
      bufferYptr = &bufferY;

      lxMatrix44Identity(matrices[0]);
      lxMatrix44Identity(matrices[1]);

      params[PARAM_MATRICES] = prog.getParameter("worldMatrix");
      params[PARAM_XCOLORPTR] = prog.getParameter("xcolor");
      params[PARAM_YCOLORBUFFER] = prog.getParameter("ybuffer");
      params[PARAM_ZTEX] = prog.getParameter("ztex");
      params[PARAM_SHUFFLEFUNC] = prog.getParameter("shuffleFunc");
      shuffle = prog.getSubRoutine("shuffle_xyz");

      datas[PARAM_MATRICES] = matrices;
      datas[PARAM_XCOLORPTR] = &bufferX.address;
      datas[PARAM_YCOLORBUFFER] = &bufferYptr;
      datas[PARAM_ZTEX] = &textureZptr;
      datas[PARAM_SHUFFLEFUNC] = &shuffle;
    }
  };


private:

  RenderHelper      m_rh;
  GeometryBox       m_box;

  KeyTracker    m_keys;

  lxgContext_t            m_ctx;
  RenderProgram           m_prog;
  ParamSet                m_paramsets[PARAMSETS];

public:
  GfxProgram() 
    : Project("gfxprogram","../../backend/test/")
  {

  }

  void updateGeometry(int x, int y, int z)
  {
    m_box.update(x,y,z);
  }



  int onInit(int argc, const char** argv) {

    updateGeometry(4,4,4);

    lxBoundingBox_t bbox;
    lxVector3Set(bbox.min,-2,-2,-2);
    lxVector3Set(bbox.max, 2, 2, 2);
    lxCVector3 up(0,1,0);
    m_rh.init(up);
    m_rh.cameraPerspective(&bbox, 30.0f);
    m_rh.cameraOrtho(&bbox);

    lxgContext_init(&m_ctx);

    m_prog.init(&m_ctx);
    m_prog.addStageProgram(LUXGFX_STAGE_VERTEX,FILENAME("resources/gfxprogram.vert").c_str(),NULL);
    m_prog.addStageProgram(LUXGFX_STAGE_FRAGMENT,FILENAME("resources/gfxprogram.frag").c_str(),NULL);
    m_prog.finish();

    // TODO init parmasets
    int i = 0;
    for (int y = 0; y < 2; y++){
      for (int x = 0; x < 2; x++,i++){
        lxCVector3 pos(-2.0f + 4.0f * (float)x, -2.0f + 4.0f * (float)y, 0.0f);
        m_paramsets[i].init(&m_ctx,m_prog);
        lxMatrix44SetTranslation(m_paramsets[i].matrices[0],pos);
        if (i == PARAMSETS-1){
          lxCVector3 rot(0,45,0);
          lxMatrix44FromEulerZYXdeg(m_paramsets[i].matrices[0],rot);
        }
        lxMatrix44Invert(m_paramsets[i].matrices[1],m_paramsets[i].matrices[0]);
        lxMatrix44Transpose1(m_paramsets[i].matrices[1]);
      }
    }
    m_paramsets[0].shuffle = m_prog.getSubRoutine("shuffle_zyx");

    return 0;
  }

  void logic(int width, int height)
  {
    m_keys.update();

  }

  int onDraw(int width, int height) {
    logic(width,height);

    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    m_rh.updateCamera();
    m_rh.setCameraGL();

    lxgContext_clearTextureState(&m_ctx);
    lxgContext_clearProgramState(&m_ctx);

    lxgContext_applyProgram(&m_ctx, &m_prog.m_program);
    for (int i = 0; i < PARAMSETS; i++){
      lxgContext_applyProgramParameters(&m_ctx,&m_prog.m_program, PARAMS, m_paramsets[i].params, m_paramsets[i].datas);
      m_box.drawVA();
    }
    lxgContext_applyProgram(&m_ctx, NULL);

    return 0;
  }

};



static GfxProgram test;

