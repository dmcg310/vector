#pragma once

#include "../../lib_application/include/scene_manager.h"
#include "../../lib_window/include/window.h"
#include "context.h"

#ifdef _DEBUG
#include "../../lib_window/include/imgui_manager.h"
#endif

class Renderer {
public:
  Renderer();
  ~Renderer();

  void Initialize(GLFWwindow *window);
  void Render();
  void Update(float deltaTime);
  void Shutdown();

  SceneManager &GetSceneManager();

private:
  Context *context;
  SceneManager sceneManager;

#ifdef _DEBUG
  ImGuiManager &imGuiManager;
#endif
};