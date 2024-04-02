#include "icv_ray_caster.hpp"

void RayCaster::set_camera_ref(const std::shared_ptr<Camera> camera) {
  camera_ref_ = camera;
}
const std::weak_ptr<Camera> &RayCaster::camera_ref() const {
  return camera_ref_;
}

void RayCaster::AddModelRef(const std::shared_ptr<Model> &model) {
  model_ref_vec_.push_back(model);
}

RayCaster CreateRayCaster(const std::shared_ptr<Camera> &camera,
                          const std::vector<std::shared_ptr<Model>> &models) {
  RayCaster ray_caster;
  ray_caster.set_camera_ref(camera);
  for (const auto &model : models) {
    ray_caster.AddModelRef(model);
  }
  return ray_caster;
}

std::vector<std::weak_ptr<Model>>
ProcessRayCaster(GLFWwindow *window, RayCaster &ray_caster,
                 const glm::vec2 &cursor_pos) {

  return {};
}