// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include "../_project/project.hpp"
#include <luxmath/bounding.h>
#include <luxbackend/meshbase.h>
#include <string>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////

std::string ReadFile(const char* filename)
{
  string str;
  size_t filesize;

  ifstream file (filename,ios::in|ios::end);

  if (!file.is_open()){
    printf("error: could not open file %s\n",filename);
    return str;
  }

  filesize=file.tellg();

  str.reserve(filesize);

  file.seekg(0);
  while (!file.eof())
  {
    str += file.get();
  }
  str[str.size()-1] = 0;
  return str;
}



//////////////////////////////////////////////////////////////////////////

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
  float length = lxVector3Length(m_sceneSize);
  m_ortho = length * 1.25f;
  m_fov   = 0.0f;
  lxCVector3  offset(-1,1,-1);
  lxCVector3  campos;

  lxMatrix44Identity(m_viewMatrix);
  //m_viewmatrix[14] = -length * 4.0f;

  lxVector3Normalized(offset);
  lxVector3ScaledAdd(campos,m_sceneCenter,length * 4.0f, offset);
  lxMatrix44LookAt(m_viewMatrix,campos,m_sceneCenter,m_up);
}

void RenderHelper::cameraPerspective( lxBoundingBox_t* bbox, float fovdeg )
{
  lxBoundingBox_toCenter(bbox,m_sceneCenter,m_sceneSize);
  float length = lxVector3Length(m_sceneSize) * 0.5f;
  lxCVector3  offset(-0.33f,0.33f,-0.66f);
  lxCVector3  campos;
  m_ortho = 0.0f;
  m_fov   = fovdeg;

  // tan = opp/adj
  length = length / tanf(LUX_DEG2RAD(fovdeg) * 0.5f);
  
  lxMatrix44Identity(m_viewMatrix);

  lxVector3Normalized(offset);
  lxVector3ScaledAdd(campos,m_sceneCenter,length, offset);
  lxMatrix44LookAt(m_viewMatrix,campos,m_sceneCenter,m_up);
}

void RenderHelper::updateProjection( int width, int height )
{
  float size = lxVector3Length(m_sceneSize);
  if (m_ortho){
    lxMatrix44Ortho(m_projMatrix, m_ortho, size / 4096.0f, size * 16.0f, (float)width / (float) height);
  }
  else{
    lxMatrix44Perspective(m_projMatrix, m_fov, size / 4096.0f, size * 16.0f, (float)width / (float) height);
  }
  m_screenSize[0] = width;
  m_screenSize[1] = height;
}

void RenderHelper::setCameraGL()
{
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(m_projMatrix);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(m_viewMatrix);
}

void RenderHelper::doArcBall(int w, int h )
{
  int mb1 = glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_1);
  int mb2 = glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_2);
  if (!mb1 && !mb2) return;

  
  if (mb1 && !mb2){
    // rotate

  }
  else if(mb2 && !mb1){
    // pan

  }
  else{
    // zoom

  }

}

void  RenderHelper::generateUVData( int w, int h, lxCVector3 *pixels)
{
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
}

GLuint RenderHelper::generateUVTexture( int w, int h )
{
  GLuint tex;
  glGenTextures(1,&tex);
  glBindTexture(GL_TEXTURE_2D,tex);
  glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);

  lxCVector3* pixels = new lxCVector3[w * h];
  generateUVData(w,h,pixels);

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_FLOAT,pixels);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_ARB);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT_ARB);
  glBindTexture(GL_TEXTURE_2D,0);
  delete [] pixels;

  return tex;
}

GLuint RenderHelper::loadShader(GLenum type, const char* filename, const char* prepend )
{
  string content = ReadFile(filename);
  content = string(prepend) + content;
  GLuint obj = glCreateShader(type);
  const GLchar* strings[] = {
    content.c_str(),
  };
  GLsizei sizes[] = {
    strlen(content.c_str()),
  };
  glShaderSource(obj,1,strings,sizes);
  return obj;
}

void RenderHelper::updateCamera()
{
  lxMatrix44MultiplyFull(m_viewprojMatrix,m_projMatrix,m_viewMatrix);
}


//////////////////////////////////////////////////////////////////////////

void Geometry::allocMem( int numVertices, int numIndicesTris, int numIndicesLines )
{
  pos.resize(numVertices);
  normal.resize(numVertices);
  uv.resize(numVertices);
  indicesTris.resize(numIndicesTris);
  indicesLines.resize(numIndicesLines);
}


void Geometry::fillVerts( VertexDefault* vertices )
{
  for (size_t i = 0; i < pos.size(); ++i){
    vertices[i].pos = pos[i];
    vertices[i].normal = normal[i];
    vertices[i].uv = uv[i];
  }
}


void Geometry::fillIndices( lxMeshIndexType_t type, void* indices, uint32 vertexoffset, booln outline )
{
  size_t num = outline ? indicesLines.size() : indicesTris.size() ;
  uint32* src = outline ? &indicesLines[0] : &indicesTris[0];
  if (type == LUX_MESH_INDEX_UINT16){
    uint16* LUX_RESTRICT dst = (uint16*)indices;
    for (size_t i = 0; i < num; ++i){
      dst[i] = src[i] + vertexoffset;
    }
  }
  else{
    memcpy(indices,src,sizeof(uint32)*num);
  }
}


void Geometry::updateBO()
{
  if (!vbo){
    glGenBuffers(1,&vbo);
  }
  if (!ibo){
    glGenBuffers(1,&ibo);
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDefault) * pos.size(), NULL, GL_STATIC_DRAW);
  VertexDefault* vertices = (VertexDefault*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  fillVerts(vertices);
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * (indicesTris.size() + indicesLines.size()), NULL, GL_STATIC_DRAW);
  uint32* indices = (uint32*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
  fillIndices(LUX_MESH_INDEX_UINT32,indices);
  fillIndices(LUX_MESH_INDEX_UINT32,indices+ indicesTris.size(),0,LUX_TRUE);
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool Geometry::useGenericAttribs = false;

void Geometry::drawVBO( booln outline, booln bind)
{
  if (bind){
    VertexDefault*  ptr = (VertexDefault*)0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if (useGenericAttribs){
      glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(VertexDefault), (const void*)&ptr->pos);
      glVertexAttribPointer(1,3,GL_FLOAT,GL_TRUE , sizeof(VertexDefault), (const void*)&ptr->normal);
      glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, sizeof(VertexDefault), (const void*)&ptr->uv);
    }
    else{
      glVertexPointer(  3,GL_FLOAT, sizeof(VertexDefault), (const void*)&ptr->pos);
      glNormalPointer(    GL_FLOAT, sizeof(VertexDefault), (const void*)&ptr->normal);
      glTexCoordPointer(2,GL_FLOAT, sizeof(VertexDefault), (const void*)&ptr->uv);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  }
  if (!outline){
    glDrawElements(GL_TRIANGLES, indicesTris.size(), GL_UNSIGNED_INT, NULL);
  }
  else{
    glDrawElements(GL_LINES, indicesLines.size(), GL_UNSIGNED_INT, (void*)(sizeof(uint32) * indicesTris.size()));
  }
}

void Geometry::drawVA( booln outline )
{
  if (useGenericAttribs){
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 0, (const void*)&pos[0]);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_TRUE , 0, (const void*)&normal[0]);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 0, (const void*)&uv[0]);
  }
  else{
    glVertexPointer(3,GL_FLOAT,0, &pos[0]);
    glNormalPointer(GL_FLOAT,0, &normal[0]);
    glTexCoordPointer(2,GL_FLOAT,0, &uv[0]);

  }
  if (!outline){
    glDrawElements(GL_TRIANGLES, indicesTris.size(), GL_UNSIGNED_INT, &indicesTris[0]);
  }
  else{
    glDrawElements(GL_LINES, indicesLines.size(), GL_UNSIGNED_INT, &indicesLines[0]);
  }
}

void Geometry::clearBO()
{
  if (vbo){
    glDeleteBuffers(1,&vbo);
    vbo = 0;
  }
  if (ibo){
    glDeleteBuffers(1,&ibo);
    ibo = 0;
  }
}

void Geometry::clearDL()
{
  if (dltris){
    glDeleteLists(dltris,1);
    dltris = 0;
  }
  if (dlline){
    glDeleteLists(dlline,1);
    dlline = 0;
  }
}

Geometry::~Geometry()
{
  clearDL();
  clearBO();
}

void Geometry::drawDL( booln outline )
{
  if (!outline){
    glCallList(dltris);
  }
  else{
    glCallList(dlline);
  }
}

void Geometry::updateDL()
{
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

  if (!dlline){
    dlline = glGenLists(1);
  }
  if (!dltris){
    dltris = glGenLists(1);
  }
  glNewList(dlline,GL_COMPILE);
  drawVA(1);
  glEndList();

  glNewList(dltris,GL_COMPILE);
  drawVA(0);
  glEndList();
}

void GeometryBox::update( int x, int y, int z )
{
  int numVerts, numTris, numLines;
  int subdiv[] = {x,y,z};

  lxMeshBox_getCounts(subdiv,&numVerts,&numTris,&numLines);
  allocMem(numVerts,numTris,numLines);
  lxMeshBox_initTriangles(subdiv,(lxVector3*)&pos[0],(lxVector3*)&normal[0],(lxVector2*)&uv[0],&indicesTris[0]);
  lxMeshBox_initOutline(subdiv,&indicesLines[0]);
}

void GeometryCylinder::update( int x, int y, int z )
{
  int numVerts, numTris, numLines;
  int subdiv[] = {x,y,z};

  lxMeshCylinder_getCounts(subdiv,&numVerts,&numTris,&numLines);
  allocMem(numVerts,numTris,numLines);
  lxMeshCylinder_initTriangles(subdiv,(lxVector3*)&pos[0],(lxVector3*)&normal[0],(lxVector2*)&uv[0],&indicesTris[0]);
  lxMeshCylinder_initOutline(subdiv,&indicesLines[0]);
}

void GeometryPlane::update( int x, int y)
{
  int numVerts, numTris, numLines;
  int subdiv[] = {x,y};

  lxMeshPlane_getCounts(subdiv,&numVerts,&numTris,&numLines);
  allocMem(numVerts,numTris,numLines);
  lxMeshPlane_initTriangles(subdiv,(lxVector3*)&pos[0],(lxVector3*)&normal[0],(lxVector2*)&uv[0],&indicesTris[0]);
  lxMeshPlane_initOutline(subdiv,&indicesLines[0]);
}

void GeometryDisc::update( int x, int y)
{
  int numVerts, numTris, numLines;
  int subdiv[] = {x,y};

  lxMeshDisc_getCounts(subdiv,&numVerts,&numTris,&numLines);
  allocMem(numVerts,numTris,numLines);
  lxMeshDisc_initTriangles(subdiv,(lxVector3*)&pos[0],(lxVector3*)&normal[0],(lxVector2*)&uv[0],&indicesTris[0]);
  lxMeshDisc_initOutline(subdiv,&indicesLines[0]);
}

void GeometrySphere::update( int x, int y)
{
  int numVerts, numTris, numLines;
  int subdiv[] = {x,y};

  lxMeshSphere_getCounts(subdiv,&numVerts,&numTris,&numLines);
  allocMem(numVerts,numTris,numLines);
  lxMeshSphere_initTriangles(subdiv,(lxVector3*)&pos[0],(lxVector3*)&normal[0],(lxVector2*)&uv[0],&indicesTris[0]);
  lxMeshSphere_initOutline(subdiv,&indicesLines[0]);
}


//////////////////////////////////////////////////////////////////////////

Project* ProjectManager::getTest(const char* name){
  for (size_t i = 0; i < m_tests.size(); ++i){
    if (strcmp(name,m_tests[i]->getName())==0){
      return m_tests[i];
    }
  }
  return NULL;
}

Project* ProjectManager::getTest(){
  if (m_tests.size()){
    return m_tests[0];
  }

  return NULL;
}


std::string Project::FILENAME(const char* name )
{
  return std::string(m_dirname) + std::string(name);
}

//////////////////////////////////////////////////////////////////////////

void RenderProgram::init( lxgContextPTR ctx )
{
  lxgProgram_init(&m_program, ctx);
}

booln RenderProgram::addStageProgram(lxgProgramStage_t type, const char * filename, const char* prepend )
{
  lxgStageProgram_t stage;
  lxgStageProgram_init(&stage, m_program.ctx, type);
  std::string src = ReadFile(filename);
  if (prepend){
    src = std::string(prepend) + src;
  }
  int errlen = lxgStageProgram_compile(&stage,src.c_str(), strlen(src.c_str()));
  if (errlen){
    std::string error;
    error.resize(errlen,0);
    lxgStageProgram_error(&stage, &error[0], errlen);
    printf("stage program compile error:\n%s\n\n",error.c_str());
    return LUX_FALSE;
  }
  m_stages.push_back(stage);
  m_stageTypes.push_back(type);
  return LUX_TRUE;
}

booln RenderProgram::finish()
{
  // attach
  for (size_t i = 0; i < m_stages.size(); i++){
    lxgProgram_setStage(&m_program, m_stageTypes[i], &m_stages[i]);
  }

  // link
  int errlen = lxgProgram_link(&m_program);
  if (errlen){
    std::string error;
    error.resize(errlen,0);
    lxgProgram_log(&m_program, &error[0], errlen);
    printf("program link error:\n%s\n\n",error.c_str());
    return LUX_FALSE;
  }

  // build parameter info
  int numCompat = 0;
  int numNameSize = 0;
  int numParams = lxgProgram_getParameterCount(&m_program, &numNameSize, &numCompat);
  int numSubNameSize = 0;
  int numSubs = 0;
  if (numCompat){
    numSubs = lxgProgram_getSubroutineCount(&m_program, &numSubNameSize);
    m_subroutines.resize(numSubs,NULL);
    m_compats.resize(numCompat);
  }
  m_namebuffer.resize(numNameSize + numSubNameSize, 0);
  m_params.resize(numParams);
  m_paramPtrs.resize(numParams);
  lxgProgram_initParameters(&m_program, numParams, &m_params[0], numNameSize, &m_namebuffer[0]);
  if (numCompat){
    lxgProgram_initSubroutineParameters(&m_program, numParams, &m_params[0], 
      numSubNameSize, &m_namebuffer[numNameSize], &m_subroutines[0],
      numCompat, &m_compats[0]);
  }
  for (int i = 0; i < numParams; i++){
    m_paramPtrs[i] = &m_params[i];
  }

  return LUX_TRUE;
}

void RenderProgram::deinit( lxgContextPTR ctx )
{
  for (size_t i = 0; i < m_stages.size(); ++i){
    lxgStageProgram_deinit(&m_stages[i],ctx);
  }
  lxgProgram_deinit(&m_program,ctx);
}

lxgProgramParameterPTR RenderProgram::getParameter( const char* name )
{
  for (size_t i = 0; i < m_params.size(); i++){
    if (strcmp(m_params[i].name,name)==0){
      return &m_params[i];
    }
  }
  return NULL;
}

lxgSubroutineKey RenderProgram::getSubRoutine( const char* name )
{
  for (size_t i = 0; i < m_subroutines.size(); i++){
    if (strcmp(m_subroutines[i],name)==0){
      return m_subroutines[i] - m_subroutines[0];
    }
  }
  return NULL;
}

//////////////////////////////////////////////////////////////

void ParameterContainer::add(lxgProgramParameterPTR param, const void *data)
{
  if (param){
    m_paramPtrs.push_back(param);
    m_paramData.push_back(data);
  }
}

