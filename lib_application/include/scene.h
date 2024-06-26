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

private:
  std::vector<std::shared_ptr<SceneNode>> nodes;
};