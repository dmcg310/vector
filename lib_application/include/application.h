#pragma once

#include "../../lib_window/include/window.h"
#include "settings.h"

class Application {
public:
  Application();

  bool Initialize();
  void Run();
  void Shutdown();

private:
  void HandleEvents();
  void Update();
  void Render();

  void MainLoopBody();

  Window window;
  ApplicationSettings settings;

  // We can put member variables here for other systems in the future
};
