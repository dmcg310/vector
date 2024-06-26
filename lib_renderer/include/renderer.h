#pragma once

#include "../../lib_application/include/scene_manager.h"
#include "../../lib_window/include/window.h"
#include "context.h"

#ifdef _DEBUG
#include "../../lib_window/include/imgui_manager.h"
#endif

class Renderer {
public:
  static Renderer &GetInstance() {
    static Renderer instance;
    return instance;
  }

  void Initialize(GLFWwindow *window);
  void Render();
  void Update(float deltaTime);
  void Shutdown();

  SceneManager &GetSceneManager();
  SceneNode &GetNode();

  std::shared_ptr<Scene> GetCurrentScene();
  std::shared_ptr<SceneNode> GetSelectedNode();

private:
  Renderer();
  ~Renderer();

  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;

  Context *context;
  SceneManager sceneManager;

#ifdef _DEBUG
  ImGuiManager &imGuiManager;
#endif
};