#include "test_scene.h"
#include "../../../lib_window/include/window.h"

void TestScene::Initialize() {
  auto node2d = std::make_shared<Node2D>();
  node2d->SetNodeType("Node2D");
  node2d->SetName("Node 2D");
  node2d->Initialize();
  AddNode(node2d);

  auto texture2d = std::make_shared<Texture2DNode>();
  texture2d->SetNodeType("Texture2D");
  texture2d->SetName("Container Texture");
  texture2d->Initialize("assets/textures/container.jpg");

  node2d->AddChild(texture2d);
  AddNode(texture2d);
}

void TestScene::Update(float deltaTime) { Scene::Update(deltaTime); }

void TestScene::Render() { Scene::Render(); }

void TestScene::Shutdown() { Scene::Shutdown(); }