// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXTEST_PROJECT_H__
#define __LUXTEST_PROJECT_H__

#include <GL/glew.h>
#include <GL/glfw3.h>

#include <luxinia/luxmath/luxmath.h>
#include <luxinia/luxmath/basetypes.hpp>
#include <luxinia/luxscene/meshbase.h>
#include <luxinia/luxgfx/luxgfx.h>

#include <string.h>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////

inline float randomFloat(){
  return (float) rand()/RAND_MAX;
}

inline float randomFloat(float max){
  return ((float) rand()/RAND_MAX) * max;
}

inline float randomFloat(float min, float max){
  return (((float) rand()/RAND_MAX) * (max - min)) + min;
}

//////////////////////////////////////////////////////////////////////////
class KeyTracker
{
public:
  struct KeyState{
    int   last;
    int   toggled;
  };

  KeyTracker() {
    memset(m_states,0,sizeof(m_states));
  }
  void update(GLFWwindow win){
    for (size_t i = 0; i < m_tracked.size(); i++){
      int key = m_tracked[i];
      int state = glfwGetKey(win,key);
      m_states[key].toggled = m_states[key].last != state;
      m_states[key].last = state;
    }
  }

  void add(int key){
    m_tracked.push_back(key);
  }
  inline int pressed(int key) const {
    return m_states[key].last;
  }
  inline int toggled(int key) const {
    return m_states[key].toggled;
  }
  inline int onPress(int key) const {
    return m_states[key].last && m_states[key].toggled;
  }
  inline const KeyState& get(int key) const {
    return m_states[key];
  }

private:
  KeyState          m_states[GLFW_KEY_LAST];
  std::vector<int>  m_tracked;
};

//////////////////////////////////////////////////////////////////////////

enum VertexDefaultAttribs{
  VERTEX_ATTRIB_POS,
  VERTEX_ATTRIB_NORMAL,
  VERTEX_ATTRIB_UV,
};

struct VertexDefault{
  lxCVector2  uv;
  lxCVector3  normal;
  lxCVector3  pos;
};

class Geometry {
public:
  static bool useGenericAttribs;


  Geometry() : vbo(0), ibo(0), dltris(0), dlline(0) {}
  ~Geometry();

  std::vector<lxCVector3>   pos;
  std::vector<lxCVector3>   normal;
  std::vector<lxCVector2>   uv;
  std::vector<uint32>       indicesTris;
  std::vector<uint32>       indicesLines;
  GLuint                    vbo;
  GLuint                    ibo;
  GLuint                    dltris;
  GLuint                    dlline;

  inline size_t  numVertices() const {
    return pos.size();
  }
  inline size_t numIndicesTris() const {
    return indicesTris.size();
  }
  inline size_t numIndicesLines() const {
    return indicesLines.size();
  }

  void fillVerts(VertexDefault* verts);
  void fillIndices(lxMeshIndexType_t  type, void* indices, uint32 vertexoffset=0, booln outline=0);

  void updateBO();
  void updateDL();

  void clearBO();
  void clearDL();

  void drawVBO(booln outline=0,booln bind=1);
  void drawDL(booln outline=0);
  void drawVA(booln outline=0);

protected:
  void allocMem(int numVertices, int numIndicesTris, int numIndicesLines);

};

class GeometryBox : public Geometry 
{
public:
  GeometryBox() { update(4,4,4); }
  GeometryBox(int x, int y, int z) { update(x,y,z); }
  void update(int x, int y, int z);
};

class GeometryDisc : public Geometry 
{
public:
  GeometryDisc() { update(32,4); }
  GeometryDisc(int x, int y){ update(x,y); }
  void update(int x, int y);
};

class GeometryPlane : public Geometry 
{
public:
  GeometryPlane() { update(4,4); }
  GeometryPlane(int x, int y) { update(x,y); }
  void update(int x, int y);
};

class GeometrySphere : public Geometry 
{
public:
  GeometrySphere() { update(32,16); }
  GeometrySphere(int x, int y) { update(x,y); }
  void update(int x, int y);
};

class GeometryCylinder : public Geometry 
{
public:
  GeometryCylinder() { update(32,4,4); }
  GeometryCylinder(int x, int y, int z) { update(x,y,z); }
  void update(int x, int y, int z);
};


//////////////////////////////////////////////////////////////////////////

class RenderProgram{
public:
  lxgProgram_t                        m_program;
private:
  std::vector<lxgStageProgram_t>      m_stages;
  std::vector<lxgProgramStage_t>      m_stageTypes;
  std::vector<lxgProgramParameter_t>  m_params;
  std::vector<lxgProgramParameterPTR> m_paramPtrs;
  std::vector<lxgProgramSubroutine_t> m_compats;
  std::vector<char*>                  m_subroutines;
  std::string                         m_namebuffer;


public:
  void    init(lxgContextPTR ctx);
  booln   addStageProgram(lxgProgramStage_t type, const char * filename, const char* prepend);
  booln   finish();
  void    deinit(lxgContextPTR ctx);

  lxgProgramParameterPTR  getParameter( const char* name);
  lxgSubroutineKey        getSubRoutine( const char* name);

  inline size_t numParams() {
    return m_paramPtrs.size();
  }

  inline lxgProgramCPTR getProgram() const {
    return &m_program;
  }
  inline const lxgProgramParameter_t* getParams() const{
    return &m_params[0];
  }
  inline char** getSubs() {
    return &m_subroutines[0];
  }
  inline lxgProgramParameterPTR* getPtrs() {
    return &m_paramPtrs[0];
  }
};

class ParameterContainer{
public:
  ParameterContainer() : m_prog(0) {}

  inline void setProgram(lxgProgramCPTR prog) {
    m_prog = prog;
  }
  inline void apply(lxgContextPTR ctx){
    lxgContext_applyProgramParameters(ctx, m_prog, m_paramPtrs.size(), &m_paramPtrs[0], &m_paramData[0]);
  }
  void add(lxgProgramParameterPTR param, const void* data);
private:
  lxgProgramCPTR                      m_prog;
  std::vector<lxgProgramParameterPTR> m_paramPtrs;
  std::vector<const void*>            m_paramData;
};

//////////////////////////////////////////////////////////////////////////

class RenderHelper {
public:

  static GLuint generateUVTexture(int w, int h);
  static void   generateUVData( int w, int h, lxCVector3 *pixels);
  static GLuint loadShader(GLenum type, const char* filename, const char* prepend );

  void init(GLFWwindow win, const lxCVector3& up);
  void update(int width, int height);

  void cameraOrtho(lxBoundingBox_t* bbox, const lxCVector3* delta=NULL);
  void cameraPerspective(lxBoundingBox_t* bbox, float fovdeg, const lxCVector3* delta=NULL);
  void updateProjection(int width, int height);
  void updateCamera();
  void setCameraGL();
  void doCameraControl();

  inline float getSceneDimension() const {
    return lxVector3Length(m_sceneSize);
  }
  inline bool getZooming() const {
    return m_zooming;
  }
  inline bool getPanning() const {
    return m_panning;
  }
  inline bool getRotating() const {
    return m_rotating;
  }

  union {
    struct {
      int     m_winwidth;
      int     m_winheight;
    };
    int       m_screenSize[2];
  };
  lxCMatrix44 m_viewTM;
  lxCMatrix44 m_projectionTM;
  lxCMatrix44 m_viewProjTM;
  float       m_ortho;
  float       m_fov;
  lxCVector3  m_mpos;
  bool        m_useOrtho;
  float       m_zoomsense;
  float       m_rotsense;
  float       m_pansense;

private:

  GLFWwindow  m_window;

  lxCVector3  m_orbitCenter;
  lxCVector3  m_sceneCenter;
  lxCVector3  m_sceneSize;
  lxCVector3  m_campos;
  lxCVector3  m_up;

  bool            m_panning;
  bool            m_zooming;
  bool            m_rotating;

  int             m_buttons[3];
  int             m_buttonsToggle[3];
  lxCVector3      m_panpos;
  lxCVector3      m_zoompos;
  lxCVector3      m_rotpos;
  lxCMatrix44     m_panrotcam;
  float           m_zoomcam;
};

//////////////////////////////////////////////////////////////////////////

class Project;
class ProjectManager {
public:
  static ProjectManager& Get(){
    static ProjectManager mgr;
    return mgr;
  }

  void addTest(Project* test){
    m_tests.push_back(test);
  }

  Project* getTest(const char* name);
  Project* getTest();

  void    initTimer();
  bool    updateTimer();
  inline double  getLastTime() const {
    return m_time;
  }
  inline uint32  getLastTimeID() const {
    return m_timeID;
  }

private:
  std::vector<Project*>   m_tests;
  double                  m_frames;
  double                  m_begin;
  double                  m_time;
  uint32                  m_timeID;
};


class Project
{
public:
  Project(const char* name, const char* dirname) : m_name(name), m_dirname(dirname){
    ProjectManager::Get().addTest(this);
  }

  std::string FILENAME(const char* name);

  const char* getName() const {
    return m_name;
  }
    const char* getDirName() const {
    return m_dirname;
  }
  virtual int  onDraw(int width, int height) { return 0; }
  virtual int  onInit(GLFWwindow window, int argc, const char** argv) { return 0; }
  virtual void onDeinit() {}

private:
  const char*   m_name;
  const char*   m_dirname;
};

std::string ReadFileContent(const char* filename);

//////////////////////////////////////////////////////////////////////////





#endif


