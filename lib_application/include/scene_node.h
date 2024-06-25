#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class SceneNode {
public:
  SceneNode() : parent(nullptr) {}
  virtual ~SceneNode() {}

  void AddChild(const std::shared_ptr<SceneNode> &child) {
    child->parent = this;
    children.push_back(child);
  }

  void RemoveChild(const std::shared_ptr<SceneNode> &child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    child->parent = nullptr;
  }

  virtual void Update(float deltaTime) {
    for (auto const &child: children) { child->Update(deltaTime); }
  }

  virtual void Render() {
    for (auto const &child: children) { child->Render(); }
  }

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

protected:
  SceneNode *parent;
  std::vector<std::shared_ptr<SceneNode>> children;
};
