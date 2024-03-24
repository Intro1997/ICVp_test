#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <memory>

#include "icv_camera.hpp"

GLFWwindow *GetGlWindow(const int width, const int height, const char *titile);
void GetWindowSize(GLFWwindow *window, int *width, int *height);
void ProcessBasicMove(GLFWwindow *window, Camera &camera, float speed);
void ProcessBasicWatch(GLFWwindow *window, Camera &camera, float speed);
