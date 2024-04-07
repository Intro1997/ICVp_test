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
#include "icv_model.hpp"
#include "icv_program.hpp"
#include "icv_ray_caster.hpp"
#include "icv_shader.hpp"
#include "icv_time.hpp"

#define FPS_INTERVAL_MICROSECOND_TIME 16667

#define MARBLE_JPG_PATH "./asset/marble.jpg"
#define METAL_PNG_PATH "./asset/metal.png"

#define VS_SHADER_PATH "./test/focus/shader/vertex.vs"
#define FS_SHADER_PATH "./test/focus/shader/fragment.fs"
#define FS_PURE_COLOR_SHADER_PATH "./test/focus/shader/fragment_pure_color.fs"

static std::vector<float> GetQuadVertices();
static void ProcessKeyInput(GLFWwindow *window);
static void MouseButtonCallback(GLFWwindow *window, int button, int action,
                                int mods);
static std::vector<std::shared_ptr<Model>> all_models;

static RayCaster ray_caster;
static std::vector<bool> model_is_selected_table;

void RunFocusTest(GLFWwindow *window) {
  INIT_GL_FUNC_WITH_RET(window, void());
  int width = 0, height = 0;
  GetWindowSize(window, &width, &height);

  all_models.push_back(
      std::make_shared<Model>(CreateModel(GetQuadVertices(), MARBLE_JPG_PATH)));
  // all_models.push_back(std::make_shared<Model>(
  //     CreateModel(GetQuadVertices(), MARBLE_JPG_PATH,
  //                 glm::translate(glm::mat4(1.0), glm::vec3(1.5, 0.0, 0.0)))));
  // all_models.push_back(std::make_shared<Model>(
  //     CreateModel(GetQuadVertices(), MARBLE_JPG_PATH,
  //                 glm::translate(glm::mat4(1.0), glm::vec3(0.0, 1.5, 0.0)))));

  model_is_selected_table.resize(QueryTotalModelCount(), false);

  Shader vs_shader = CreateShader(GL_VERTEX_SHADER, VS_SHADER_PATH);
  Shader fs_shader = CreateShader(GL_FRAGMENT_SHADER, FS_SHADER_PATH);
  Shader vs_pure_shader = CreateShader(GL_VERTEX_SHADER, VS_SHADER_PATH);
  Shader fs_pure_shader =
      CreateShader(GL_FRAGMENT_SHADER, FS_PURE_COLOR_SHADER_PATH);

  glm::vec3 camera_pos{0.0, 0.0, 3.0};
  glm::vec3 camera_up{0.0, 1.0, 0.0};
  glm::vec3 camera_lookat{0.0, 0.0, 0.0};
  std::shared_ptr<Camera> curr_camera = std::make_shared<Camera>(
      CreateCamera(camera_pos, camera_up, camera_lookat, width, height));

  ray_caster = CreateRayCaster(curr_camera, all_models);

  Program quad_tex_program = CreateProgram({vs_shader, fs_shader});

  Program select_program = CreateProgram({vs_pure_shader, fs_pure_shader});

  for (const auto &model : all_models) {
    if (model->vao_id() == 0 || model->material().texture_id() == 0) {
      std::cerr << "gl mmodel error!\n";
      return;
    }
  }

  if (!quad_tex_program.Link() || !select_program.Link()) {
    std::cerr << "gl program error!\n";
    return;
  }

  quad_tex_program.Use();
  quad_tex_program.SetUniformMatrix4("model", glm::mat4(1.0));
  quad_tex_program.SetUniformMatrix4("view", glm::mat4(1.0));
  quad_tex_program.SetUniformMatrix4("projection", glm::mat4(1.0));

  select_program.Use();
  select_program.SetUniformMatrix4("model", glm::mat4(1.0));
  select_program.SetUniformMatrix4("view", glm::mat4(1.0));
  select_program.SetUniformMatrix4("projection", glm::mat4(1.0));

  quad_tex_program.Use();
  quad_tex_program.UseCamera(*curr_camera);
  quad_tex_program.SetUniform1i("texture1", 0);

  select_program.Use();
  select_program.UseCamera(*curr_camera);
  select_program.SetUniformMatrix4(
      "model", glm::scale(glm::mat4(1.0), glm::vec3(1.2, 1.2, 1.2)));

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);

  CHECK_GL_ERROR_WITH_RET(void(0));
  uint64_t current_time = 0, last_time = 0;
  double cursor_x, cursor_y;
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  while (!glfwWindowShouldClose(window)) {
    current_time = GetCurrentTime(TimeUnit::MICORSECOND);
    ProcessKeyInput(window);
    ProcessBasicMove(window, *curr_camera, 0.1f);
    ProcessBasicWatch(window, *curr_camera, 0.1f);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (const auto &model : all_models) {
      glBindVertexArray(model->vao_id());
      quad_tex_program.Use();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, model->material().texture_id());

      quad_tex_program.SetUniformMatrix4("model", model->trans_mat());

      glStencilMask(0xFF);
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      if (model_is_selected_table[model->model_id()]) {
        select_program.Use();
        select_program.SetUniformMatrix4(
            "model",
            model->trans_mat() * glm::scale(glm::mat4(1.0), glm::vec3(1.2)));
        glStencilMask(0xFF);
        glStencilFunc(GL_EQUAL, 0, 0xFF);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

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

static void MouseButtonCallback(GLFWwindow *window, int button, int action,
                                int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    glm::dvec2 cursor_pos;
    glfwGetCursorPos(window, &cursor_pos.x, &cursor_pos.y);
    std::vector<std::weak_ptr<Model>> selected_models =
        ProcessRayCaster(window, ray_caster, cursor_pos);

    for (const auto &selected_model : selected_models) {
      if (!selected_model.expired()) {
        std::shared_ptr<Model> selected_model_ref = selected_model.lock();
        uint32_t m_id = selected_model_ref->model_id();
        model_is_selected_table[m_id] = !model_is_selected_table[m_id];
      }
    }
  }
}

static std::vector<float> GetQuadVertices() {
  // clang-format off
  return {
        // positions          // texture Coords
        // back
  /*0*/ -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // front
 /*6*/  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // left
/*12*/  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         // right
/*18*/   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        // bottom
/*24*/  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // top
/*30*/  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
  // clang-format on
}
