#include "gl_window.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

static glm::vec3 GetVec3ByYawPitch(double yaw, double pitch);

GLFWwindow *GetGlWindow(const int width, const int height, const char *title) {
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

  GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
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

static double last_w_cursor_pos_x = 0;
static double last_w_cursor_pos_y = 0;
static double yaw = 0;
static double pitch = 0;
static bool reset_last_pos = true;
#define MAX_WATCH_DEGREE 80.0
#define MIN_WATCH_DEGREE (-80.0)

void ProcessBasicWatch(GLFWwindow *window, Camera &camera, float speed) {
  if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
    reset_last_pos = true;
    return;
  }

  if (reset_last_pos) {
    glfwGetCursorPos(window, &last_w_cursor_pos_x, &last_w_cursor_pos_y);
    reset_last_pos = false;
    return;
  }

  double w_cursor_pos_x, w_cursor_pos_y;
  glfwGetCursorPos(window, &w_cursor_pos_x, &w_cursor_pos_y);

  double w_cursor_offset_x =
      (w_cursor_pos_x - last_w_cursor_pos_x) * glm::abs(speed);
  double w_cursor_offset_y =
      (w_cursor_pos_y - last_w_cursor_pos_y) * glm::abs(speed);

  yaw += w_cursor_offset_x;
  pitch -= w_cursor_offset_y;

  if (pitch > MAX_WATCH_DEGREE) {
    pitch = MAX_WATCH_DEGREE;
  } else if (pitch < MIN_WATCH_DEGREE) {
    pitch = MIN_WATCH_DEGREE;
  }

  glm::vec3 pos = camera.position();
  glm::vec3 pos_to_lookat = GetVec3ByYawPitch(yaw, pitch);
  camera.set_lookat(pos + pos_to_lookat);

  last_w_cursor_pos_x = w_cursor_pos_x;
  last_w_cursor_pos_y = w_cursor_pos_y;
  camera.Update();
}

static glm::vec3 GetVec3ByYawPitch(double yaw_degree, double pitch_degree) {
  double pitch_radians = glm::radians(pitch_degree);
  double yaw_radians = glm::radians(yaw_degree - 90);
  // double x = glm::cos(pitch_radians) * glm::sin(glm::radians(yaw_degree));
  double x = glm::cos(pitch_radians) * glm::cos(yaw_radians);
  double y = glm::sin(pitch_radians);
  // double z = glm::cos(pitch_radians) * glm::cos(glm::radians(yaw_degree +
  // 180));
  double z = glm::cos(pitch_radians) * glm::sin(yaw_radians);
  return glm::normalize(glm::vec3(x, y, z));
}