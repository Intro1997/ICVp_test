#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

GLuint CreateProgram(const char *vertex_shader_file,
                     const char *fragment_shader_file, const int scr_width,
                     const int scr_height);
