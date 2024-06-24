#pragma once

#include "settings.h"
#include "scene_manager.h"
#include "../scenes/test_scene/test_scene.h"
#include "../../lib_log/include/log.h"
#include "../../lib_renderer/include/context.h"
#include "../../lib_renderer/include/render_api_factory.h"
#include "../../lib_window/include/window.h"
#include <chrono>
#include <memory>

#ifdef _DEBUG
#include "imgui_manager.h"
#endif

class Application : public IEventObserver {
public:
  Application();

  bool Initialize();
  void Run();
  void Shutdown();

  void OnKeyPress(int key) override;
  void OnKeyRelease(int key) override;
  void OnMouseMove(double x, double y) override;
  void OnMouseClick(int button) override;

private:
  void Update();
  void Render();

  void MainLoopBody();

  void LogFrameInfo(float deltaTime);

#ifdef _DEBUG
  ImGuiManager imguiManager;
  bool isDebugMenuOpen = false;
  std::chrono::steady_clock::time_point startTime;
  unsigned int frameCount;
  double accumulatedTime;
  double accumulatedFPS;
  unsigned int sampleFrames;
  unsigned int frameSamples;
#endif

  ApplicationSettings settings;
  SceneManager sceneManager;
  Context *context;
};