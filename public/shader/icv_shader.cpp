#include "icv_shader.hpp"
#include "icv_file.hpp"
#include "tools.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

static bool CompileShader(GLuint shader, const File &shader_file);
template <size_t n>
static bool LinkShaders(GLuint program, const GLuint (&shader_array)[n]);

GLuint CreateProgram(const char *vertex_file, const char *fragment_file,
                     const int scr_width, const int scr_height) {
  // prepare shaders
  File vertex_shader_file(vertex_file, "vertex_file");
  File fragment_shader_file(fragment_file, "fragment_file");

  vertex_shader_file.LoadContent();
  fragment_shader_file.LoadContent();

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  if (vertex_shader == 0) {
    std::cerr << "create vertex shader error\n";
    return -1;
  }
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  if (fragment_shader == 0) {
    std::cerr << "create fragment shader error\n";
    return -1;
  }

  if (!CompileShader(vertex_shader, vertex_shader_file) ||
      !CompileShader(fragment_shader, fragment_shader_file)) {
    std::cerr << "Compile shader failed\n";
    return -1;
  }

  GLuint program = glCreateProgram();
  if (program == 0) {
    std::cerr << "Create program failed\n";
    return -1;
  }

  if (!LinkShaders(program, {vertex_shader, fragment_shader})) {
    std::cerr << "link shader failed\n";
    return -1;
  }

  glUseProgram(program);

  glm::mat4 model = glm::mat4(1.0);

  glm::mat4 view = glm::mat4(1.0);
  // view = glm::translate(view, glm::vec3(0, 0, 0));

  glm::vec3 camera_position = glm::vec3(2, 0, 0);
  glm::mat4 lookat_mat =
      glm::lookAt(camera_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  view = lookat_mat * view;
  std::cout << glm::to_string(glm::transpose(view)) << std::endl;

  glm::mat4 projection = glm::mat4(1.0);
  projection = glm::perspective(
      glm::radians(90.0f), (float)scr_width / (float)scr_height, 0.1f, 10.0f);

  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
  std::cout << glGetError() << std::endl;
  glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE,
                     glm::value_ptr(view));
  std::cout << glGetError() << std::endl;
  glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE,
                     glm::value_ptr(projection));
  std::cout << glGetError() << std::endl;

  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  CHECK_GL_ERROR_WITH_RET(0);

  return program;
}

static bool CompileShader(GLuint shader, const File &shader_file) {
  std::string shader_content = shader_file.GetContent();
  const GLchar *shader_string = shader_content.c_str();
  const GLint shader_string_length = shader_content.length();

  glShaderSource(shader, 1, &shader_string, nullptr);

  glCompileShader(shader);

  GLint ret;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
  if (ret == GL_FALSE) {
    std::cerr << "Compile shader " << shader_file.filename() << " failed\n";
    GLint info_len, info_size;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
    std::unique_ptr<char> info_string(new char[info_len + 1]());

    glGetShaderInfoLog(shader, info_len, &info_size, info_string.get());
    std::cerr << "Detail: " << info_string << std::endl;
    return false;
  }
  return true;
}

template <size_t n>
bool LinkShaders(GLuint program, const GLuint (&shader_array)[n]) {
  for (int i = 0; i < n; i++) {
    glAttachShader(program, shader_array[i]);
  }

  glLinkProgram(program);

  GLint ret;
  glGetProgramiv(program, GL_LINK_STATUS, &ret);
  if (ret == GL_FALSE) {
    std::cerr << "Link program failed\n";
    GLint info_len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
    std::unique_ptr<char> info_str(new char[info_len]());
    GLint get_info_len;
    glGetProgramInfoLog(program, info_len, &get_info_len, info_str.get());
    std::cerr << "Detail: " << info_str << "\n";
    return false;
  }
  return true;
}
