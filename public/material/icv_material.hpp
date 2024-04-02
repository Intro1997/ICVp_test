#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

enum class MaterialType { PURE_COLOR, TEXTURE_2D };

class Material {
public:
  Material(const MaterialType type, const std::string &texture_image_path);

  void set_type(const MaterialType type);
  MaterialType type() const;

  void set_texture_id(const GLuint id);
  GLuint texture_id() const;

  std::string texture_image_path() const;  

  bool LoadTextureId();

private:
  MaterialType type_;
  std::string texture_image_path_;
  GLuint texture_id_;
};