#include "gl_window.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

static double last_w_cursor_pos_x = 0;
static double last_w_cursor_pos_y = 0;
static bool first_get_cursor = true;
static glm::vec3 RotateVec3(const glm::vec3 &vec, float degree,
                            const glm::vec3 &axis);
#define MAX_WATCH_DEGREE 80.0

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

void ProcessBasicMove(GLFWwindow *window, Camera &camera, float speed) {

  int key_w_press = glfwGetKey(window, GLFW_KEY_W);
  int key_s_press = glfwGetKey(window, GLFW_KEY_S);
  int key_a_press = glfwGetKey(window, GLFW_KEY_A);
  int key_d_press = glfwGetKey(window, GLFW_KEY_D);

  if ((!(key_w_press + key_s_press + key_a_press + key_d_press)) ||
      ((key_w_press && key_s_press) || (key_a_press && key_d_press))) {
    return;
  }

  glm::vec3 lookat = camera.lookat();
  glm::vec3 pos = camera.position();
  glm::vec3 up = camera.up();
  glm::vec3 fb_direction = // forward and backward forward
      glm::normalize(lookat - pos);
  glm::vec3 lr_direction = // left and right forward
      glm::normalize(glm::cross(up, fb_direction));
  glm::vec3 fb_distance = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 lr_distance = glm::vec3(0.0, 0.0, 0.0);

  if (key_w_press) {
    fb_distance = fb_direction * speed;
  } else if (key_s_press) {
    fb_distance = -(fb_direction * speed);
  }

  if (key_a_press) {
    lr_distance = lr_direction * speed;
  } else if (key_d_press) {
    lr_distance = -(lr_direction * speed);
  }

  camera.set_position(pos + fb_distance + lr_distance);
  camera.set_lookat(lookat + fb_distance + lr_distance);

  camera.Update();
}

void ProcessBasicWatch(GLFWwindow *window, Camera &camera, float speed) {
  /*
  (0, 0)___________
        |         |
        |  window |
        |         |
        ----------- (x, y)
  */
  int w_width, w_height;
  glfwGetWindowSize(window, &w_width, &w_height);
  if (w_width == 0 || w_height == 0) {
    return;
  }

  if (first_get_cursor) {
    glfwGetCursorPos(window, &last_w_cursor_pos_x, &last_w_cursor_pos_y);
    first_get_cursor = false;
    return;
  }

  double w_cursor_pos_x, w_cursor_pos_y;
  glfwGetCursorPos(window, &w_cursor_pos_x, &w_cursor_pos_y);

  double w_cursor_offset_x =
      (w_cursor_pos_x - last_w_cursor_pos_x) * glm::abs(speed);
  double w_cursor_offset_y =
      (w_cursor_pos_y - last_w_cursor_pos_y) * glm::abs(speed);

  float w_fov = camera.horizontal_fov();
  glm::vec3 lookat = camera.lookat();
  glm::vec3 pos = camera.position();
  glm::vec3 up = glm::normalize(camera.up());
  glm::vec3 pos_to_lookat = lookat - pos;

  if (w_cursor_offset_x) {
    glm::vec3 rotate_axis = glm::vec3(0.0, 1.0, 0.0);
    float rotate_degree = (-w_cursor_offset_x / (double)w_width) * w_fov;
    pos_to_lookat = RotateVec3(pos_to_lookat, rotate_degree, rotate_axis);
    camera.set_lookat(pos + pos_to_lookat);
  }
  if (w_cursor_offset_y) {
    glm::vec3 rotate_axis = glm::normalize(glm::cross(pos_to_lookat, up));
    float rotate_degree = (-w_cursor_offset_y / (double)w_height) * w_fov;
    pos_to_lookat = RotateVec3(pos_to_lookat, rotate_degree, rotate_axis);
    if (glm::abs(glm::dot(glm::normalize(pos_to_lookat), up)) <=
        glm::cos(glm::radians(90.0 - MAX_WATCH_DEGREE))) {
      camera.set_lookat(pos + pos_to_lookat);
    }
  }

  last_w_cursor_pos_x = w_cursor_pos_x;
  last_w_cursor_pos_y = w_cursor_pos_y;
  camera.Update();
}

static glm::vec3 RotateVec3(const glm::vec3 &vec, float degree,
                            const glm::vec3 &axis) {
  glm::mat4 rotate_mat =
      glm::rotate(glm::mat4(1.0f), glm::radians(degree), axis);
  return glm::vec3(rotate_mat * glm::vec4(vec, 1.0));
}