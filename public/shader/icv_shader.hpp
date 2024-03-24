#pragma once
#include "icv_camera.hpp"
#include "icv_file.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class Shader {
public:
  Shader(GLenum type, const std::string &path);

  const File &file() const;
  const GLenum type() const;
  const GLuint gl_obj() const;
  bool IsCompiled() const;

  bool Compile();


private:
  GLenum type_;
  File file_;
  GLuint gl_obj_;
  bool is_compiled_;
};

Shader CreateShader(GLenum type, const std::string& path);