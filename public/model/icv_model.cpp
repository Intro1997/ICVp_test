#include "icv_model.hpp"
#include "icv_vertex.hpp"
#include "tools.hpp"
#include <iostream>

Model::Model(const std::vector<float> &positions, const Material &material,
             const uint32_t model_id)
    : positions_(positions), material_(material), model_id_(model_id) {}

GLuint Model::vao_id() const { return vao_id_; }
void Model::set_vao_id(GLuint id) { vao_id_ = id; }

void Model::set_material(const Material &material) { material_ = material; }
Material Model::material() const { return material_; }

const std::vector<float> Model::positions() const { return positions_; }

bool Model::LoadVaoId() {
  vao_id_ = CreateVAO(positions_);
  if (vao_id_ != 0 && glIsVertexArray(vao_id_)) {
    return true;
  }
  return false;
}

static uint32_t total_model_cnt = 0;
Model CreateModel(const std::vector<float> &positions,
                  const std::string &texture_image_path,
                  const glm::mat4 trans_mat) {
  Material material(MaterialType::TEXTURE_2D, texture_image_path);
  if (!material.LoadTextureId()) {
    std::cerr << "Load \"" << texture_image_path << "\" failed.\n";
  }

  Model model(positions, material, total_model_cnt++);
  model.set_trans_mat(trans_mat);

  if (!model.LoadVaoId()) {
    std::cerr << "Load model position failed.\n";
  }

  return model;
}

uint32_t Model::model_id() const { return model_id_; }

void Model::set_trans_mat(const glm::mat4 trans_mat) { trans_mat_ = trans_mat; }

glm::mat4 Model::trans_mat() const { return trans_mat_; }

uint32_t QueryTotalModelCount() { return total_model_cnt; }