#pragma once

#include "../../../lib_renderer/include/render_api_factory.h"
#include "../../include/scene_node.h"
#include <glm/glm.hpp>

class Node2D : public SceneNode {
public:
  Node2D() = default;
  ~Node2D() override = default;

  void SetPosition(const glm::vec2 &pos) override {
    position = glm::vec3(pos, 0.0f); // Set the z-component to 0 for 2D
  }

  [[nodiscard]] glm::vec2 GetPosition2D() const { return {position.x, position.y}; }

  [[nodiscard]] glm::mat4 GetModelMatrix(const glm::mat4 &renderPassModelMatrix) const {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
    model = renderPassModelMatrix * model;
    return model;
  }
};