#pragma once

#include "settings.h"
#include "../../lib_log/include/log.h"
#include "../../lib_window/include/window.h"
#include "../../lib_window/include/imgui_impl_glfw.h"
#include "../../lib_window/include/imgui_impl_opengl3.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <chrono>

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
  void HandleEvents();
  void Update();
  void Render();

  void MainLoopBody();

#ifdef _DEBUG
  void RenderDebugMenu();
  bool isDebugMenuOpen = false;
  std::chrono::steady_clock::time_point startTime;
  unsigned int frameCount;
#endif

  ApplicationSettings settings;
};
