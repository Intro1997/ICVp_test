#include "icv_vertex.hpp"
#include "tools.hpp"

GLuint CreateVAO(const std::vector<float> &quad_vec) {
  GLint origin_vao;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &origin_vao);

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, quad_vec.size() * sizeof(float),
               quad_vec.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                        (void *)(3 * sizeof(float)));

  glBindVertexArray(origin_vao);

  CHECK_GL_ERROR_WITH_RET(0);
  return VAO;
}