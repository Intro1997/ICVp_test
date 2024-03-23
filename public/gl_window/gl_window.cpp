#include "gl_window.hpp"

GLFWwindow *GetGlWindow(const int width, const int height, const char *titile) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
  glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  GLFWwindow *window = glfwCreateWindow(width, height, titile, NULL, NULL);
  return window;
}

void GetWindowSize(GLFWwindow *window, int *width, int *height) {
  glfwGetWindowSize(window, width, height);
}