#pragma once

#include "../../lib_renderer/include/context.h"
#include "event.h"
#include "renderer.h"
#include "settings.h"

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
  bool isDebugMenuOpen = false;
  std::chrono::steady_clock::time_point startTime;
  unsigned int frameCount;
  double accumulatedTime;
  double accumulatedFPS;
  unsigned int sampleFrames;
  unsigned int frameSamples;
#endif

  Renderer *renderer;
};