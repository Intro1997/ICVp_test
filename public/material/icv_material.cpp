#include "icv_material.hpp"
#include "icv_texture.hpp"

Material::Material(const MaterialType type,
                   const std::string &texture_image_path)
    : type_(type), texture_image_path_(texture_image_path) {}

MaterialType Material::type() const { return type_; }
void Material::set_type(const MaterialType type) {}

void Material::set_texture_id(const GLuint id) { texture_id_ = id; }
GLuint Material::texture_id() const { return texture_id_; }

std::string Material::texture_image_path() const { return texture_image_path_; }

bool Material::LoadTextureId() {
  texture_id_ = CreateTextureObj(texture_image_path_.c_str());
  if (texture_id_ != 0 && glIsTexture(texture_id_)) {
    return true;
  }
  return false;
}