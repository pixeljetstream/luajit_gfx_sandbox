// Copyright (C) 2010-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include "../_project/project.hpp"
#include <string>

int main(int argc, const char** argv)
{
  Project* test = argc > 1 ? ProjectManager::Get().getTest(argv[1]) : ProjectManager::Get().getTest();
  std::string caption = "lux backend test: " + std::string(test ? test->getName() : "no test specified");
  int status = 1;

  glfwInit();
  glfwOpenWindowHint(GLFW_DEPTH_BITS,24);
  glfwOpenWindowHint(GLFW_STENCIL_BITS,8);
  GLFWwindow win = glfwOpenWindow(1024,768,GLFW_WINDOWED,caption.c_str(),0);
  glewInit();

  if (test){
    status = test->onInit(win,argc,argv);
  }
  if (status){
    return 0;
  }


  ProjectManager::Get().initTimer();
  while( glfwIsWindow(win) && glfwGetKey(win,GLFW_KEY_ESCAPE) != GLFW_PRESS)
  {
    int width;
    int height;
    glfwGetWindowSize(win, &width, &height);
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
        glfwSetWindowTitle(win, (caption + std::string(buffer)).c_str());
      }
    }
  }

  if (glfwIsWindow(win)){
    test->onDeinit();
    glfwCloseWindow(win);
  }

  return 0;
}



