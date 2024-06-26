#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class SceneNode {
public:
  SceneNode() : parent(nullptr), position(0.0f), rotation(0.0f), scale(1.0f) {}
  virtual ~SceneNode() {}

  void AddChild(const std::shared_ptr<SceneNode> &child) {
    child->parent = this;
    children.push_back(child);
  }

  void RemoveChild(const std::shared_ptr<SceneNode> &child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    child->parent = nullptr;
  }

  virtual void Initialize() {}

  virtual void Update(float deltaTime) {
    for (auto const &child: children) { child->Update(deltaTime); }
  }

  virtual void Render() {
    for (auto const &child: children) { child->Render(); }
  }

  virtual void Shutdown() {}

  virtual void SetPosition(const glm::vec2 &pos) { position = pos; }

  [[nodiscard]] virtual glm::vec2 GetPosition() const { return position; }

  glm::vec2 position;
  glm::vec3 rotation;
  glm::vec3 scale;

protected:
  SceneNode *parent;
  std::vector<std::shared_ptr<SceneNode>> children;
};