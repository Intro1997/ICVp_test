#pragma once
#include "icv_material.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Model {
public:
  Model(const std::vector<float> &positions, const Material &material,
        const uint32_t model_id);

  void set_vao_id(GLuint id);
  GLuint vao_id() const;

  void set_material(const Material &material);
  Material material() const;

  const std::vector<float> positions() const;
  bool LoadVaoId();

  uint32_t model_id() const;

  void set_trans_mat(const glm::mat4 trans_mat);
  glm::mat4 trans_mat() const;

  glm::vec3 GetPointPositionByIdx(const uint32_t idx);

private:
  std::vector<float> positions_;
  GLuint vao_id_;
  Material material_;
  uint32_t model_id_;
  glm::mat4 trans_mat_;
};

Model CreateModel(const std::vector<float> &positions,
                  const std::string &texture_image_path,
                  const glm::mat4 trans_mat4 = glm::mat4(1.0));

uint32_t QueryTotalModelCount();