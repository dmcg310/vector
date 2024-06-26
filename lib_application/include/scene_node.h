#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <string>
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

  virtual void Initialize() {
    for (const auto &child: children) { child->Initialize(); }
  }

  virtual void Update(float deltaTime) {
    for (const auto &child: children) { child->Update(deltaTime); }
  }

  virtual void Render() {
    for (const auto &child: children) { child->Render(); }
  }

  virtual void Shutdown() {
    for (const auto &child: children) { child->Shutdown(); }
  }

  virtual void SetScale(const glm::vec2 &s) { scale = s; }
  [[nodiscard]] virtual glm::vec2 GetScale() const { return scale; }

  virtual void SetRotation(const glm::vec2 &rot) { rotation = rot; }
  [[nodiscard]] virtual glm::vec2 GetRotation() const { return rotation; }

  virtual void SetPosition(const glm::vec2 &pos) { position = pos; }
  [[nodiscard]] virtual glm::vec2 GetPosition() const { return position; }

  [[nodiscard]] std::vector<std::shared_ptr<SceneNode>> GetChildren() const {
    return children;
  }

  [[nodiscard]] std::string GetName() const {
    if (name.empty()) {
      return GetNodeType();
    } else {
      return name;
    }
  }

  void SetName(const std::string &_name) { name = _name; }

  [[nodiscard]] std::string GetNodeType() const { return nodeType; }
  void SetNodeType(const std::string &_nodeType) { nodeType = _nodeType; }

protected:
  std::string name;
  std::string nodeType;
  glm::vec2 position;
  glm::vec2 rotation;
  glm::vec2 scale;
  SceneNode *parent;
  std::vector<std::shared_ptr<SceneNode>> children;
};