// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include "project.hpp"
#include <string>

int main(int argc, const char** argv)
{
  Project* test = argc > 1 ? ProjectManager::Get().getTest(argv[1]) : ProjectManager::Get().getTest();
  std::string caption = "lux backend test: " + std::string(test ? test->getName() : "no test specified");
  int status = 1;

  glfwInit();

#ifdef _DEBUG
  glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  glfwOpenWindow( 640, 480, 8,8,8,8,24,8, GLFW_WINDOW);
  glfwSetWindowTitle(caption.c_str());
  glewInit();

  glfwDisable(GLFW_AUTO_POLL_EVENTS);
  glfwSwapInterval(1);

  if (test){
    status = test->onInit(argc,argv);
  }
  if (status){
    return 0;
  }


  ProjectManager::Get().initTimer();
  while( glfwGetWindowParam(GLFW_OPENED) && glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS)
  {
    int width;
    int height;
    glfwGetWindowSize(&width, &height);
    glViewport(0,0,width,height);

    status = test->onDraw(width, height);

    if (!status){
      glfwSwapBuffers();
    }
    glFinish();
    glfwPollEvents();
    if (!status){
      if (ProjectManager::Get().updateTimer()){
        char buffer[128];
        double dur = ProjectManager::Get().getLastTime();
        sprintf_s(buffer,128," %.1f ms %5d fps", dur * 1000.0, (int)floor(1.0/dur));
        glfwSetWindowTitle((caption + std::string(buffer)).c_str());
      }
    }
  }

  if (glfwGetWindowParam(GLFW_OPENED)){
    test->onDeinit();
    glfwCloseWindow();
  }

  return 0;
}



