#include <GL/glew.h>
#include <GL/glfw3.h>

int main(int argc, char** argv)
{
  glfwInit();
  GLFWwindow win = glfwOpenWindow(640,480,GLFW_WINDOWED,"lux backend test",0);
  glewInit();

  while( glfwIsWindow(win) && glfwGetKey(win,GLFW_KEY_ESCAPE) != GLFW_PRESS)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers();
    glfwPollEvents();
  }

  if (glfwIsWindow(win)){
    glfwCloseWindow(win);
  }

  return 0;
}



