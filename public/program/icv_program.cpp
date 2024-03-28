#include "icv_program.hpp"
#include "tools.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#define PROGRAM_COMPILE_ERROR_BUFFER_LEN 1024

GLuint Program::gl_obj() const { return gl_obj_; }
const std::vector<Shader> &Program::shaders() const { return shaders_; }

void Program::AddShader(const Shader &shader) { shaders_.push_back(shader); }

bool Program::CompileAllShader() {
  for (auto &shader : shaders_) {
    if (shader.IsCompiled()) {
      continue;
    } else if (!shader.Compile()) {
      std::cerr << "Compile " << shader.file().current_path() << " failed\n";
      return false;
    }
  }

  return true;
}

bool Program::Link() {
  if (glIsProgram(gl_obj_) != GL_TRUE) {
    gl_obj_ = glCreateProgram();
  }

  if (!CompileAllShader()) {
    std::cerr << "Compile shader error\n";
    return false;
  }

  for (auto &shader : shaders_) {
    glAttachShader(gl_obj_, shader.gl_obj());
  }

  GLint ret;
  glLinkProgram(gl_obj_);
  glGetProgramiv(gl_obj_, GL_LINK_STATUS, &ret);

  if (ret != GL_TRUE) {
    std::unique_ptr<char> buf(new char[PROGRAM_COMPILE_ERROR_BUFFER_LEN]());
    GLint info_len;
    glGetProgramInfoLog(gl_obj_, 1024, &info_len, buf.get());
    std::cerr << "Link program failed.\nErrMsg: " << buf.get() << std::endl;
    return false;
  }

  CHECK_GL_ERROR_WITH_RET(false);

  return true;
}

void Program::Use() const {
  glUseProgram(gl_obj_);
  // remove these
  SetUniformMatrix4("model", glm::mat4(1.0));
  SetUniformMatrix4("view", glm::mat4(1.0));
  SetUniformMatrix4("projection", glm::mat4(1.0));
}

// TODO: use function generator
void Program::SetUniform1f(const std::string &location_name, float value) {
  glUniform1f(glGetUniformLocation(gl_obj_, location_name.c_str()), value);
}
void Program::SetUniform1i(const std::string &location_name, int value) {
  glUniform1i(glGetUniformLocation(gl_obj_, location_name.c_str()), value);
}

bool Program::SetUniformMatrix4Array(const char *name, const size_t count,
                                     const float *pointer, bool transpose) {

  if (!pointer || !name) {
    return false;
  }

  glUniformMatrix4fv(glGetUniformLocation(gl_obj_, name), count, transpose,
                     pointer);

  CHECK_GL_ERROR_WITH_RET(false);

  return true;
}

bool Program::SetUniformBool(const char *name, bool use) {
  if (!name) {
    return false;
  }

  const int int_use = use ? 1 : 0;
  glUniform1i(glGetUniformLocation(gl_obj_, name), int_use);

  CHECK_GL_ERROR_WITH_RET(false);

  return true;
}

bool Program::SetUniformMatrix4(const char *name,
                                const glm::mat4 &trs_matrix) const {
  if (!name) {
    return false;
  }
  auto location = glGetUniformLocation(gl_obj_, name);

  glUniformMatrix4fv(location, 1, GL_FALSE,
                     (const GLfloat *)glm::value_ptr(trs_matrix));

  CHECK_GL_ERROR_WITH_RET(false);

  return true;
}

void Program::UseCamera(Camera &camera) const {
  camera.AddViewUpdateCb(
      [this](const glm::mat4 &view_mat, const glm::mat4 &projection) {
        this->Use();
        this->SetUniformMatrix4("view", view_mat);
        this->SetUniformMatrix4("projection", projection);
      });
  camera.Update();
}

// TODO: add move version
Program CreateProgram(const std::vector<Shader> &shaders) {
  Program p;
  for (const auto &shader : shaders) {
    p.AddShader(shader);
  }
  return p;
}