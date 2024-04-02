#pragma once
#include "icv_camera.hpp"
#include "icv_model.hpp"
#include <memory>
#include <GLFW/glfw3.h>

class RayCaster {
public:
  RayCaster() = default;

  void set_camera_ref(const std::shared_ptr<Camera> camera);
  const std::weak_ptr<Camera> &camera_ref() const;

  void AddModelRef(const std::shared_ptr<Model> &model);

private:
  std::weak_ptr<Camera> camera_ref_;
  std::vector<std::weak_ptr<Model>> model_ref_vec_;
};

RayCaster CreateRayCaster(const std::shared_ptr<Camera> &camera,
                          const std::vector<std::shared_ptr<Model>> &models);

std::vector<std::weak_ptr<Model>> ProcessRayCaster(GLFWwindow* window, RayCaster& ray_caster, const glm::vec2& cursor_pos);