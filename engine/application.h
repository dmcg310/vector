#pragma once

#include "../../lib_renderer/include/context.h"
#include "camera.h"
#include "event.h"
#include "renderer.h"
#include "settings.h"
#include <unordered_set>

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
  void OnScroll(double yoffset) override;

private:
  void Update();
  void Render();
  void MainLoopBody();
  void LogFrameInfo(float deltaTime);

#ifdef _DEBUG
  bool isDebugMenuOpen = false;
  std::chrono::steady_clock::time_point startTime;
  unsigned int frameCount;
  double accumulatedTime;
  double accumulatedFPS;
  unsigned int sampleFrames;
  unsigned int frameSamples;
#endif

  Renderer *renderer;
  Camera camera;
  bool cameraControlEnabled = false;
  bool cursorVisible = true;
  std::unordered_set<int> pressedKeys;
};