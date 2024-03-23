#include "focus.hpp"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "tools.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <thread>
#include <vector>

#include "gl_window.hpp"
#include "icv_shader.hpp"
#include "icv_texture.hpp"
#include "icv_vertex.hpp"

#define MARBLE_JPG_PATH                                                        \
  "/Users/intro/Spaces/TestSpace/ICVp_test/asset/marble.jpg"
#define METAL_PNG_PATH "/Users/intro/Spaces/TestSpace/ICVp_test/asset/metal.png"

#define VS_SHADER                                                              \
  "/Users/intro/Spaces/TestSpace/ICVp_test/test/focus/shader/vertex.vs"
#define FS_SHADER                                                              \
  "/Users/intro/Spaces/TestSpace/ICVp_test/test/focus/shader/fragment.fs"
#define FS_PURE_COLOR_SHADER                                                   \
  "/Users/intro/Spaces/TestSpace/ICVp_test/test/focus/shader/"                 \
  "fragment_pure_color.fs"

static std::vector<float> GetQuadVertices();

void RunFocusTest(GLFWwindow *window) {
  INIT_GL_FUNC_WITH_RET(window, void());
  int width = 0, height = 0;
  GetWindowSize(window, &width, &height);
  if (width == 0 || height == 0) {
    std::cerr << "error window size\n";
    return;
  }

  GLuint quad_vao = CreateVAO(GetQuadVertices());
  GLuint marble_texture = CreateTextureObj(MARBLE_JPG_PATH);
  GLuint metal_texture = CreateTextureObj(METAL_PNG_PATH);
  std::cout << marble_texture << std::endl;
  GLuint quad_tex_program = CreateProgram(VS_SHADER, FS_SHADER, width, height);
  GLuint quad_color_program =
      CreateProgram(VS_SHADER, FS_PURE_COLOR_SHADER, width, height);
  if (quad_vao == 0 || marble_texture == 0 || metal_texture == 0 ||
      quad_tex_program == 0 || quad_color_program == 0) {
    std::cerr << "gl error!\n";
    return;
  }

  glBindVertexArray(quad_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, marble_texture);
  glUseProgram(quad_tex_program);

  glUniform1i(glGetUniformLocation(quad_tex_program, "texture1"), 0);
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(16ms);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // don't forget to clear the stencil buffer!

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}

static std::vector<float> GetQuadVertices() {
  // clang-format off
  return {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
  // clang-format on
}
