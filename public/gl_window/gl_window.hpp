#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

GLFWwindow *GetGlWindow(const int width, const int height, const char *titile);
void GetWindowSize(GLFWwindow *window, int *width, int *height);
