#pragma once

#include "scene_node.h"
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

  [[nodiscard]] std::shared_ptr<SceneNode> Clone() const override {
    auto node = std::make_shared<Node3D>(*this);
    node->children.clear();

    for (const auto &child: children) { node->AddChild(child->Clone()); }

    return node;
  }

  virtual void SetViewMatrix(const glm::mat4 &viewMatrix) override {
    for (auto &child: children) { child->SetViewMatrix(viewMatrix); }
  }

private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
};