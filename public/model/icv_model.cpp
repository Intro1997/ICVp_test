#include "icv_model.hpp"
#include "icv_vertex.hpp"
#include "tools.hpp"
#include <iostream>

Model::Model(const std::vector<float> &positions, const Material &material)
    : positions_(positions), material_(material) {}

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

Model CreateModel(const std::vector<float> &positions,
                  const std::string &texture_image_path) {
  Material material(MaterialType::TEXTURE_2D, texture_image_path);
  if (!material.LoadTextureId()) {
    std::cerr << "Load \"" << texture_image_path << "\" failed.\n";
  }

  Model model(positions, material);
  if (!model.LoadVaoId()) {
    std::cerr << "Load model position failed.\n";
  }

  return model;
}
