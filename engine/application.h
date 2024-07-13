#pragma once

#include "core/camera/camera.h"
#include "core/settings/settings.h"
#include "core/window/event.h"
#include "renderer/context/context.h"
#include "renderer/renderer.h"
#include "scene/test_scene/test_scene.h"
#include <chrono>
#include <memory>
#include <unordered_set>

class Application : public IEventObserver {
public:
  Application();
  bool Initialize();
  void Run();
  [[noreturn]] void Shutdown();

  void OnKeyPress(int key) override;
  void OnKeyRelease(int key) override;
  void OnMouseMove(double x, double y) override;
  void OnMouseClick(int button) override;
  void OnScroll(double xoffset, double yoffset) override;

private:
  void Update();
  void Render();
  void MainLoopBody();
  void LogFrameInfo(float deltaTime);
  void ToggleCameraControl();
  static void ResetCursorPosition();
  void HandleCameraMovement(float deltaTime);

  unsigned int frameCount{0};
  double accumulatedTime{0.0};
  double accumulatedFPS{0.0};
  const unsigned int sampleFrames{100};
  unsigned int frameSamples{0};

  Camera camera;
  bool cameraControlEnabled{false};
  bool cursorVisible{true};
  std::unordered_set<int> pressedKeys;
};