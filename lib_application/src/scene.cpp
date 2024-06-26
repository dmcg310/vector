#include "scene.h"
#include "../../lib_log/include/log.h"

void Scene::Initialize() {
  if (nodes.empty()) {
    Log::Write(Log::ERROR, "No nodes to initialize");
    return;
  }

  for (auto &node: nodes) { node->Initialize(); }
}

void Scene::Update(float deltaTime) {
  for (auto &node: nodes) { node->Update(deltaTime); }
}

void Scene::Render() {
  for (auto &node: nodes) { node->Render(); }
}

void Scene::Shutdown() {
  for (auto &node: nodes) { node->Shutdown(); }

  nodes.clear();
}

void Scene::AddNode(const std::shared_ptr<SceneNode> &node) { nodes.push_back(node); }

void Scene::RemoveNode(const std::shared_ptr<SceneNode> &node) {
  nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());

  node->Shutdown();
}