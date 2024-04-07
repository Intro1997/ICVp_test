#include "icv_camera.hpp"
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const glm::mat4 kGlmIdentityMat4 = glm::mat4(1.0);

Camera::Camera()
    : position_(glm::vec3(0.0, 0.0, 0.0)), up_(glm::vec3(0.0, 1.0, 0.0)),
      lookat_(glm::vec3(0.0, 0.0, -1.0)), view_mat_(glm::mat4(1.0)),
      inverse_view_mat_(glm::mat4(1.0)), projection_mat_(glm::mat4(1.0)),
      inverse_projection_mat_(glm::mat4(1.0)), horizontal_fov_(45.0f),
      clip_plane_near_(0.1f), clip_plane_far_(10.0f), aspect_(1.0f),
      orthographic_width_(0.0f), lookat_style_(LookAtStyle::kDefault) {}

void Camera::set_position(const glm::vec3 &position) { position_ = position; }

const glm::vec3 &Camera::position() const { return position_; }

void Camera::set_up(const glm::vec3 &up) { up_ = up; }

const glm::vec3 &Camera::up() const { return up_; }

void Camera::set_lookat(const glm::vec3 &lookat) { lookat_ = lookat; }

const glm::vec3 &Camera::lookat() const { return lookat_; }

const glm::mat4 &Camera::view_mat() const { return view_mat_; }

const glm::mat4 &Camera::inverse_view_mat() const { return inverse_view_mat_; }

const glm::mat4 &Camera::projection_mat() const { return projection_mat_; }

const glm::mat4 &Camera::inverse_projection_mat() const {
  return inverse_projection_mat_;
}

void Camera::set_horizontal_fov(const float &horizontal_fov) {
  horizontal_fov_ = horizontal_fov;
}

float Camera::horizontal_fov() const { return horizontal_fov_; }

void Camera::set_clip_plane_near(const float &clip_plane_near) {
  clip_plane_near_ = clip_plane_near;
}

float Camera::clip_plane_near() const { return clip_plane_near_; }

void Camera::set_clip_plane_far(const float &clip_plane_far) {
  clip_plane_far_ = clip_plane_far;
}

float Camera::clip_plane_far() const { return clip_plane_far_; }

void Camera::set_aspect(const float &aspect) { aspect_ = aspect; }

float Camera::aspect() const { return aspect_; }

void Camera::set_orthographic_width(const float &orthographic_width) {
  orthographic_width_ = orthographic_width;
}

float Camera::orthographic_width() const { return orthographic_width_; }

LookAtStyle Camera::lookat_style() const { return lookat_style_; }

void Camera::set_lookat_style(LookAtStyle style) { lookat_style_ = style; }

void Camera::GenViewMat() {
  if (lookat_style_ == LookAtStyle::kGltf) {
    view_mat_ = glm::lookAt(position_, lookat_ + position_, up_);
  } else {
    view_mat_ = glm::lookAt(position_, lookat_, up_);
  }
  inverse_view_mat_ = glm::inverse(view_mat_);
}

void Camera::Update() {
  GenViewMat();
  GenProjectionMat();
  for (const auto &cb : view_update_cbs_) {
    cb(view_mat_, projection_mat_);
  }
}

void Camera::AddViewUpdateCb(
    const std::function<void(const glm::mat4 &, const glm::mat4 &)> &cb) {
  view_update_cbs_.push_back(cb);
}

void Camera::GenProjectionMat() {
  if (orthographic_width_ == 0) {
    projection_mat_ = glm::perspective(horizontal_fov_, aspect_,
                                       clip_plane_near_, clip_plane_far_);
  } else {
    if (aspect_ == 0) {
      projection_mat_ = glm::mat4(1.0);
      printf("Error: aspect is zero!\n");
    }

    float right = orthographic_width_;
    float up = orthographic_width_ / aspect_;
    projection_mat_ =
        glm::ortho(-right, right, up, -up, clip_plane_near_, clip_plane_far_);
  }
  inverse_projection_mat_ = glm::inverse(projection_mat_);
}

Camera CreateCamera(const glm::vec3 &pos, const glm::vec3 &up,
                    const glm::vec3 &lookat, const int width,
                    const int height) {
  Camera camera;
  camera.set_position(pos);
  camera.set_up(up);
  camera.set_lookat(lookat);
  camera.set_aspect((float)width / (float)height);
  return camera;
}