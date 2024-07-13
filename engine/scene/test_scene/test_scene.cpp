#include "test_scene.h"

void TestScene::Initialize() {
  // auto node2d = std::make_shared<Node2D>();
  // node2d->SetNodeType("Node2D");
  // node2d->SetName("Node 2D");
  // node2d->Initialize();
  // AddNode(node2d);

  // auto texture2d = std::make_shared<Texture2DNode>();
  // texture2d->SetNodeType("Texture2D");
  // texture2d->SetName("Container Texture");
  // texture2d->Initialize("assets/textures/container.jpg");

  // node2d->AddChild(texture2d);
  // AddNode(texture2d);

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