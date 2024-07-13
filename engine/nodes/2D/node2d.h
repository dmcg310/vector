#pragma once

#include "../../scene/scene_node.h"
#include <glm/gtc/matrix_transform.hpp>

class Node2D : public SceneNode {
public:
  Node2D() : position(0.0f), rotation(0.0f), scale(1.0f) {}
  ~Node2D() override = default;

  void SetPosition(const glm::vec2 &pos) {
    position = glm::vec3(pos, 0.0f); // Set the z-component to 0 for 2D
  }

  [[nodiscard]] glm::vec2 GetPosition2D() const { return {position.x, position.y}; }

  void SetRotation(float rot) {
    rotation = glm::vec3(0.0f, 0.0f, rot); // Only use z-component for 2D rotation
  }

  [[nodiscard]] float GetRotation2D() const { return rotation.z; }

  void SetScale(const glm::vec2 &scl) {
    scale = glm::vec3(scl, 1.0f); // Set the z-component to 1 for 2D
  }

  [[nodiscard]] glm::vec2 GetScale2D() const { return {scale.x, scale.y}; }

  [[nodiscard]] glm::mat4 GetModelMatrix(const glm::mat4 &renderPassModelMatrix) const {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    model = renderPassModelMatrix * model;
    return model;
  }

  [[nodiscard]] std::shared_ptr<SceneNode> Clone() const override {
    auto node = std::make_shared<Node2D>(*this);
    node->children.clear();

    for (const auto &child: children) { node->AddChild(child->Clone()); }

    return node;
  }

private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
};