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
#include "icv_time.hpp"
#include "icv_vertex.hpp"

#define FPS_INTERVAL_MICROSECOND_TIME 16667

#define MARBLE_JPG_PATH "./asset/marble.jpg"
#define METAL_PNG_PATH "./asset/metal.png"

#define VS_SHADER_PATH "./test/focus/shader/vertex.vs"
#define FS_SHADER_PATH "./test/focus/shader/fragment.fs"
#define FS_PURE_COLOR_SHADER_PATH "./test/focus/shader/fragment_pure_color.fs"

static std::vector<float> GetQuadVertices();
static void ProcessKeyInput(GLFWwindow *window);

void RunFocusTest(GLFWwindow *window) {
  INIT_GL_FUNC_WITH_RET(window, void());
  int width = 0, height = 0;
  GetWindowSize(window, &width, &height);

  GLuint quad_vao = CreateVAO(GetQuadVertices());
  GLuint marble_texture = CreateTextureObj(MARBLE_JPG_PATH);
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

  if (quad_vao == 0 || marble_texture == 0 || !quad_tex_program.Link()) {
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
  uint64_t current_time = 0, last_time = 0;

  while (!glfwWindowShouldClose(window)) {
    current_time = GetCurrentTime(TimeUnit::MICORSECOND);
    ProcessKeyInput(window);
    ProcessBasicMove(window, curr_camera, 0.1f);
    ProcessBasicWatch(window, curr_camera, 0.1f);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // don't forget to clear the stencil buffer!

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwPollEvents();
    glfwSwapBuffers(window);
    last_time = GetCurrentTime(TimeUnit::MICORSECOND);
    WAIT_UNTIL_INTERVAL_LARGE_THAN_FPS_TIME(current_time, last_time,
                                            FPS_INTERVAL_MICROSECOND_TIME);
  }
}

static bool first_press_cmd = true;
static void ProcessKeyInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  int ctrl_key_state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
  if (ctrl_key_state == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else if (ctrl_key_state == GLFW_RELEASE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
