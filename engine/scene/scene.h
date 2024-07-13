#pragma once

#include "scene_node.h"
#include <memory>
#include <vector>

class Scene {
public:
  virtual ~Scene() = default;

  virtual void Initialize();
  virtual void Update(float deltaTime);
  virtual void Render();
  virtual void Shutdown();

  virtual void AddNode(const std::shared_ptr<SceneNode> &node);
  virtual void RemoveNode(const std::shared_ptr<SceneNode> &node);

  [[nodiscard]] virtual std::shared_ptr<SceneNode> &GetNode();
  virtual std::vector<std::shared_ptr<SceneNode>> &GetNodes();
  virtual void SelectNode(const std::shared_ptr<SceneNode> &node);
  [[nodiscard]] virtual std::shared_ptr<SceneNode> GetSelectedNode() const;

  virtual void SetViewMatrixAndZoom(const glm::mat4 &viewMatrix, float zoom);

  virtual void SetLightPosition(const glm::vec3 &lightPos);
  [[nodiscard]] virtual glm::vec3 GetLightPosition() const;

  virtual void SetCameraPosition(const glm::vec3 &cameraPos);
  [[nodiscard]] virtual glm::vec3 GetCameraPosition() const;

private:
  std::vector<std::shared_ptr<SceneNode>> nodes;
  std::shared_ptr<SceneNode> selectedNode;
};