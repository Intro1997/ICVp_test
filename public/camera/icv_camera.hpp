#pragma once
#include <glm/glm.hpp>
#include <string>

enum class LookAtStyle {
  kDefault = 0x0,
  kGltf = 0x1, // gltf has no lookat info, and default lookat stright forward
};

class Camera {
public:
  Camera();
  void set_position(const glm::vec3 &position);
  const glm::vec3 &position() const;
  void set_up(const glm::vec3 &up);
  const glm::vec3 &up() const;
  void set_lookat(const glm::vec3 &lookat);
  const glm::vec3 &lookat() const;
  const glm::mat4 &view_mat() const;
  const glm::mat4 &projection_mat() const;

  void set_horizontal_fov(const float &horizontal_fov);
  float horizontal_fov() const;
  void set_clip_plane_near(const float &clip_plane_near);
  float clip_plane_near() const;
  void set_clip_plane_far(const float &clip_plane_far);
  float clip_plane_far() const;
  void set_aspect(const float &aspect);
  float aspect() const;
  void set_orthographic_width(const float &orthographic_width);
  float orthographic_width() const;
  LookAtStyle lookat_style() const;
  void set_lookat_style(LookAtStyle style);

  void GenViewMat();
  void GenProjectionMat();
  void Update();
  void AddViewUpdateCb(
      const std::function<void(const glm::mat4 &, const glm::mat4 &)> &cb);

private:
  glm::vec3 position_;
  glm::vec3 up_;
  glm::vec3 lookat_;
  glm::mat4 view_mat_;
  glm::mat4 projection_mat_;
  float horizontal_fov_;
  float clip_plane_near_;
  float clip_plane_far_;
  float aspect_;
  float orthographic_width_;
  LookAtStyle lookat_style_;
  std::vector<std::function<void(const glm::mat4 &, const glm::mat4 &)>>
      view_update_cbs_;
};

Camera CreateCamera(const glm::vec3 &pos, const glm::vec3 &up,
                    const glm::vec3 &lookat, const int width, const int height);