#include "icv_shader.hpp"
#include "tools.hpp"
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <sstream>

#define SHADER_COMPILE_ERROR_BUFFER_LEN 1024

static const glm::mat4 kGlmIdentityMat4 = glm::mat4(1.0);

#define GL_INFO_LOG_BUFFER_SIZE 1024

Shader::Shader(GLenum type, const std::string &path)
    : type_(type), file_(path.c_str()), is_compiled_(false) {}

const File &Shader::file() const { return file_; }
const GLenum Shader::type() const { return type_; }
const GLuint Shader::gl_obj() const { return gl_obj_; }

bool Shader::Compile() {
  std::string file_string = file_.GetFileContent();
  if (file_string.empty()) {
    return false;
  }

  if (glIsShader(gl_obj_) != GL_TRUE) {
    gl_obj_ = glCreateShader(type_);
  }

  GLint file_string_size;
  GLint ret;

  const char *file_string_cstr;
  file_string_cstr = file_string.c_str();
  file_string_size = file_string.size();

  CHECK_GL_ERROR_WITH_RET(false);

  glShaderSource(gl_obj_, 1, &file_string_cstr, &file_string_size);
  glCompileShader(gl_obj_);
  glGetShaderiv(gl_obj_, GL_COMPILE_STATUS, &ret);

  if (ret != GL_TRUE) {
    GLsizei info_len;
    std::unique_ptr<char> buf(new char[GL_INFO_LOG_BUFFER_SIZE]());
    glGetShaderInfoLog(gl_obj_, SHADER_COMPILE_ERROR_BUFFER_LEN, &info_len,
                       buf.get());
    std::cerr << "Compile " << file_.current_path()
              << " failed.\nErrMsg: " << buf << std::endl;
    return false;
  }

  return true;
}

bool Shader::IsCompiled() const { return is_compiled_; }

// TODO: add move version
Shader CreateShader(GLenum type, const std::string &path) {
  Shader s(type, path);
  return s;
}