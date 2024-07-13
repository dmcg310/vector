#pragma once

#include "../core/log/log.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class SceneNode {
public:
  SceneNode() : parent(nullptr) {}
  virtual ~SceneNode() = default;

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

  virtual void SetViewMatrixAndZoom(const glm::mat4 &viewMatrix, float zoom) {
    for (const auto &child: children) { child->SetViewMatrixAndZoom(viewMatrix, zoom); }
  }

  [[nodiscard]] std::vector<std::shared_ptr<SceneNode>> GetChildren() const {
    return children;
  }

  [[nodiscard]] virtual SceneNode *GetParent() const { return parent; }

  [[nodiscard]] std::string GetName() const {
    if (name.empty()) {
      return GetNodeType();
    } else {
      return name;
    }
  }
  void SetName(const std::string_view &_name) { name = _name; }

  [[nodiscard]] std::string GetNodeType() const { return nodeType; }
  void SetNodeType(const std::string_view &_nodeType) { nodeType = _nodeType; }

  [[nodiscard]] virtual glm::vec3 GetLightPosition() const { return lightPos; }
  virtual void SetLightPosition(const glm::vec3 &lightPos) {
    for (const auto &child: children) { child->SetLightPosition(lightPos); }
  }

  [[nodiscard]] virtual glm::vec3 GetCameraPosition() const { return cameraPos; }
  virtual void SetCameraPosition(const glm::vec3 &cameraPos) {
    this->cameraPos = cameraPos;
    for (const auto &child: children) { child->SetCameraPosition(cameraPos); }
  }

  void AddChild(const std::shared_ptr<SceneNode> &child) {
    child->parent = this;
    children.push_back(child);
  }

  void RemoveChild(const std::shared_ptr<SceneNode> &child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
      (*it)->Shutdown();
      children.erase(it);
      child->parent = nullptr;
    }
  }

protected:
  std::string name;
  std::string nodeType;
  SceneNode *parent;
  std::vector<std::shared_ptr<SceneNode>> children;
  glm::vec3 lightPos{};
  glm::vec3 cameraPos{};
};