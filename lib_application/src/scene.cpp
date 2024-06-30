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
  auto it = std::find(nodes.begin(), nodes.end(), node);
  if (it != nodes.end()) {
    (*it)->Shutdown();
    nodes.erase(it);
  }
}

std::shared_ptr<SceneNode> &Scene::GetNode() { return nodes[0]; }

std::vector<std::shared_ptr<SceneNode>> &Scene::GetNodes() { return nodes; }

void Scene::SelectNode(const std::shared_ptr<SceneNode> &node) { selectedNode = node; }

std::shared_ptr<SceneNode> Scene::GetSelectedNode() const { return selectedNode; }

void Scene::SetViewMatrixAndZoom(const glm::mat4 &viewMatrix, float zoom) {
  for (auto &node: nodes) { node->SetViewMatrixAndZoom(viewMatrix, zoom); }
}
