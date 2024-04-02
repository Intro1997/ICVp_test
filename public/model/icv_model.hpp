#pragma once
#include "icv_material.hpp"
#include <glad/glad.h>
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

private:
  std::vector<float> positions_;
  GLuint vao_id_;
  Material material_;
  uint32_t model_id_;
};

Model CreateModel(const std::vector<float> &positions,
                  const std::string &texture_image_path);

uint32_t QueryTotalModelCount();