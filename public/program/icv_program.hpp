#pragma once
#include "icv_shader.hpp"
#include <vector>

class Program {
public:
  Program() = default;

  GLuint gl_obj() const;
  const std::vector<Shader> &shaders() const;

  void AddShader(const Shader &shader);

  bool CompileAllShader();
  bool Link();
  void Use() const;
  void SetUniform1f(const std::string &location_name, float value);
  void SetUniform1i(const std::string &location_name, int value);
  bool SetUniformMatrix4Array(const char *name, const size_t count,
                              const float *pointer, bool transpose);
  bool SetUniformBool(const char *name, bool use);
  bool SetUniformMatrix4(const char *name, const glm::mat4 &trs_matrix) const;
  void UseCamera(Camera &camera) const;

private:
  std::vector<Shader> shaders_;
  GLuint gl_obj_; // TODO: use virtual base class?
};

Program CreateProgram(const std::vector<Shader> &shaders);