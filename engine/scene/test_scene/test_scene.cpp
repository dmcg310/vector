#include "test_scene.h"

void TestScene::Initialize() {
  auto rootNode = std::make_shared<Node3D>();
  rootNode->SetNodeType("Node3D");
  rootNode->SetName("Node 3D");
  rootNode->Initialize();
  AddNode(rootNode);

  auto cube3d = std::make_shared<Cube3DNode>();
  cube3d->SetNodeType("Cube3DNode");
  cube3d->SetName("Cube 3D");
  cube3d->Initialize();

  rootNode->AddChild(cube3d);
  AddNode(cube3d);

  auto lightingCube = std::make_shared<LightingCubeNode>();
  lightingCube->SetNodeType("LightingCubeNode");
  lightingCube->SetName("Lighting Cube");
  lightingCube->Initialize();

  rootNode->AddChild(lightingCube);
  AddNode(lightingCube);
}

void TestScene::Update(float deltaTime) { Scene::Update(deltaTime); }

void TestScene::Render() { Scene::Render(); }

void TestScene::Shutdown() { Scene::Shutdown(); }