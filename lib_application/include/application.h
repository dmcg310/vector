#pragma once

#include "../../lib_log/include/log.h"
#include "../../lib_window/include/window.h"
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
  void HandleEvents();
  void Update();
  void Render();

  void MainLoopBody();

  ApplicationSettings settings;

  // We can put member variables here for other systems in the future
};
