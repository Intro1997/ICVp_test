#pragma once
#include "icv_camera.hpp"
#include "icv_model.hpp"
#include <GLFW/glfw3.h>
#include <memory>

class RayCaster {
public:
  RayCaster(const glm::vec3 &direction = glm::vec3(0.0, 0.0, -1.0));

  void set_camera_ref(const std::shared_ptr<Camera> camera);
  const std::weak_ptr<Camera> &camera_ref() const;

  void AddModelRef(const std::shared_ptr<Model> &model);

  std::vector<std::weak_ptr<Model>> model_ref_vec();

  glm::vec3 direction() const;

private:
  std::weak_ptr<Camera> camera_ref_;
  std::vector<std::weak_ptr<Model>> model_ref_vec_;
  glm::vec3 direction_;
};

RayCaster CreateRayCaster(const std::shared_ptr<Camera> &camera,
                          const std::vector<std::shared_ptr<Model>> &models);

std::vector<std::weak_ptr<Model>> ProcessRayCaster(GLFWwindow *window,
                                                   RayCaster &ray_caster,
                                                   const glm::vec2 &cursor_pos);