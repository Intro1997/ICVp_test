#include "icv_ray_caster.hpp"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

RayCaster::RayCaster(const glm::vec3 &direction) : direction_(direction) {}

void RayCaster::set_camera_ref(const std::shared_ptr<Camera> camera) {
  camera_ref_ = camera;
}
const std::weak_ptr<Camera> &RayCaster::camera_ref() const {
  return camera_ref_;
}

std::vector<std::weak_ptr<Model>> RayCaster::model_ref_vec() {
  return model_ref_vec_;
}

glm::vec3 RayCaster::direction() const { return direction_; }

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
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  auto win_width = (float)width;
  auto win_height = (float)height;

  if (cursor_pos.x < 0 || cursor_pos.y < 0 || cursor_pos.x > win_width ||
      cursor_pos.y > win_height) {
    return {};
  }
  float half_win_width = win_width / 2.0f;
  float half_win_height = win_height / 2.0f;
  glm::vec2 trans_cursor_pos;
  trans_cursor_pos.x = (cursor_pos.x - half_win_width) / half_win_width;
  trans_cursor_pos.y = (cursor_pos.y - half_win_height) / half_win_height;
  trans_cursor_pos.y = -trans_cursor_pos.y;

  std::weak_ptr<Camera> camera_weak = ray_caster.camera_ref();
  std::vector<std::weak_ptr<Model>> aimed_models;
  if (!camera_weak.expired()) {
    std::shared_ptr<Camera> camera_lock = camera_weak.lock();
    glm::vec3 O = camera_lock->position();
    glm::vec3 D = glm::vec3(trans_cursor_pos,
                            -0.1); // z-axis will be useless because of inverse
                                   // of perspective matrix
    glm::vec3 D_test = glm::vec3(trans_cursor_pos, -1.0);
    D = camera_lock->inverse_projection_mat() * glm::vec4(D, 1.0);
    D = camera_lock->inverse_view_mat() * glm::vec4(D, 1.0);
    D = D - O;
    D = glm::normalize(D);

    for (const auto &model_ref : ray_caster.model_ref_vec()) {
      if (!model_ref.expired()) {
        std::shared_ptr<Model> model = model_ref.lock();
        uint32_t model_position_size = model->positions().size() / 5;
        for (int i = 0; i < model_position_size; i += 3) {
          glm::vec3 A = model->GetPointPositionByIdx(i + 0);
          glm::vec3 B = model->GetPointPositionByIdx(i + 1);
          glm::vec3 C = model->GetPointPositionByIdx(i + 2);

          glm::vec3 AB = B - A;
          glm::vec3 AC = C - A;
          glm::vec3 AO = O - A;

          //                      [u]
          // O-A = [B-A C-A -D] * |v|
          //                      [t]
          //            ^(-1)          [u]
          // [B-A C-A -D]      * O-A = |v|
          //                           [t]
          glm::mat3 inverse_mat = glm::inverse(glm::mat3(AB, AC, -D));
          glm::vec3 uvt = inverse_mat * AO;
          float u = uvt.x;
          float v = uvt.y;
          float t = uvt.z;
          float u_v = u + v;

          // clang-format off
          if (0 <= u && u <= 1 && 
              0 <= v && v <= 1 && 
              0 <= u_v && u_v <= 1 &&
              0 <= t) {
            aimed_models.push_back(model);
            break;
          }
          // clang-format on
        }
      }
    }
  }
  return aimed_models;
}