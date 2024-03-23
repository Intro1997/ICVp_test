#pragma once
#include <iostream>
#include <string>

#ifndef __FILE_NAME__
// TODO: consider windows
#define __FILE_NAME__                                                          \
  strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#endif

#define INIT_GL_FUNC_WITH_RET(window, ret_value)                               \
  do {                                                                         \
    glfwMakeContextCurrent(window);                                            \
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {                 \
      std::cerr << "Failed to initialize GLAD\n";                              \
      return ret_value;                                                        \
    }                                                                          \
  } while (false);

#define CHECK_GL_ERROR_WITH_RET(ret_value)                                     \
  do {                                                                         \
    GLenum err = glGetError();                                                 \
    if (err != GL_NO_ERROR) {                                                  \
      std::cerr << "GL error occurs! Error code is " << err << ", at "         \
                << std::string(__FILE_NAME__) << ":" << __LINE__ << "\n";                   \
      return ret_value;                                                        \
    }                                                                          \
  } while (false);
