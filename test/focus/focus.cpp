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
#include "icv_program.hpp"
#include "icv_shader.hpp"
#include "icv_texture.hpp"
#include "icv_vertex.hpp"

#define MARBLE_JPG_PATH                                                        \
  "/Users/intro/Spaces/TestSpace/ICVp_test/asset/marble.jpg"
#define METAL_PNG_PATH "/Users/intro/Spaces/TestSpace/ICVp_test/asset/metal.png"

#define VS_SHADER_PATH                                                         \
  "/Users/intro/Spaces/TestSpace/ICVp_test/test/focus/shader/vertex.vs"
#define FS_SHADER_PATH                                                         \
  "/Users/intro/Spaces/TestSpace/ICVp_test/test/focus/shader/fragment.fs"
#define FS_PURE_COLOR_SHADER_PATH                                              \
  "/Users/intro/Spaces/TestSpace/ICVp_test/test/focus/shader/"                 \
  "fragment_pure_color.fs"

static std::vector<float> GetQuadVertices();

void RunFocusTest(GLFWwindow *window) {
  INIT_GL_FUNC_WITH_RET(window, void());
  int width = 0, height = 0;
  GetWindowSize(window, &width, &height);

  GLuint quad_vao = CreateVAO(GetQuadVertices());
  GLuint marble_texture = CreateTextureObj(MARBLE_JPG_PATH);
  GLuint metal_texture = CreateTextureObj(METAL_PNG_PATH);
  Shader vs_shader = CreateShader(GL_VERTEX_SHADER, VS_SHADER_PATH);
  Shader fs_shader = CreateShader(GL_FRAGMENT_SHADER, FS_SHADER_PATH);
  Shader fs_pure_shader =
      CreateShader(GL_FRAGMENT_SHADER, FS_PURE_COLOR_SHADER_PATH);

  glm::vec3 camera_pos{0.0, 0.0, 3.0};
  glm::vec3 camera_up{0.0, 1.0, 0.0};
  glm::vec3 camera_lookat{0.0, 0.0, 0.0};
  Camera curr_camera =
      CreateCamera(camera_pos, camera_up, camera_lookat, width, height);
  Program quad_tex_program = CreateProgram({vs_shader, fs_shader});

  if (quad_vao == 0 || marble_texture == 0 || metal_texture == 0 ||
      !quad_tex_program.Link()) {
    std::cerr << "gl error!\n";
    return;
  }

  glBindVertexArray(quad_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, marble_texture);

  quad_tex_program.Use();
  quad_tex_program.UseCamera(curr_camera);
  quad_tex_program.SetUniform1i("texture1", 0);

  glEnable(GL_DEPTH_TEST);

  CHECK_GL_ERROR_WITH_RET(void(0));

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  while (!glfwWindowShouldClose(window)) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(16ms);
    ProcessBasicMove(window, curr_camera, 0.1f);
    ProcessBasicWatch(window, curr_camera, 0.3f);

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
