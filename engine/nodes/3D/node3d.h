#pragma once

#include "../../scene/scene_node.h"
#include <glm/gtc/matrix_transform.hpp>

class Node3D : public SceneNode {
public:
  Node3D() : position(0.0f), rotation(0.0f), scale(1.0f) {}
  ~Node3D() override = default;

  void SetPosition(const glm::vec3 &pos) { position = pos; }
  [[nodiscard]] glm::vec3 GetPosition3D() const { return position; }

  void SetRotation(const glm::vec3 &rot) { rotation = rot; }
  [[nodiscard]] glm::vec3 GetRotation() const { return rotation; }

  void SetScale(const glm::vec3 &scl) { scale = scl; }
  [[nodiscard]] glm::vec3 GetScale() const { return scale; }

  void SetViewMatrixAndZoom(const glm::mat4 &viewMatrix, float zoom) override {
    for (auto &child: children) { child->SetViewMatrixAndZoom(viewMatrix, zoom); }
  }

  void SetLightPosition(const glm::vec3 &lightPos) override { lightPosition = lightPos; }
  [[nodiscard]] glm::vec3 GetLightPosition() const override { return lightPosition; }

private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec3 lightPosition;
};