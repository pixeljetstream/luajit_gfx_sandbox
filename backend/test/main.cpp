// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "test.hpp"
#include <string>

int main(int argc, char** argv)
{
  Test* test = argc > 1 ? TestManager::Get().getTest(argv[1]) : NULL;
  std::string caption = "lux backend test: " + std::string(test ? test->getName() : "no test specified");


  glfwInit();
  glfwOpenWindowHint(GLFW_DEPTH_BITS,24);
  glfwOpenWindowHint(GLFW_STENCIL_BITS,8);
  GLFWwindow win = glfwOpenWindow(640,480,GLFW_WINDOWED,caption.c_str(),0);
  glewInit();

  if (test)
    test->onInit(win);

  while( glfwIsWindow(win) && glfwGetKey(win,GLFW_KEY_ESCAPE) != GLFW_PRESS)
  {
    int width;
    int height;
    glfwGetWindowSize(win, &width, &height);
    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (test)
      test->onDraw(width, height);

    glfwSwapBuffers();
    glFinish();
    glfwPollEvents();
  }

  if (glfwIsWindow(win)){
    if (test)
      test->onDeInit();
    glfwCloseWindow(win);
  }

  return 0;
}



