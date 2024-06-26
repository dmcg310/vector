#include "test_scene.h"
#include "../../../lib_window/include/window.h"

void TestScene::Initialize() {
  simpleNode = std::make_shared<SimpleNode>();
  simpleNode->Initialize();

  AddNode(simpleNode);
}

void TestScene::Update(float deltaTime) { Scene::Update(deltaTime); }

void TestScene::Render() { Scene::Render(); }

void TestScene::Shutdown() { Scene::Shutdown(); }