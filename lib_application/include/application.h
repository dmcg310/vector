#pragma once

#include "../../lib_log/include/log.h"
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

  ApplicationSettings settings;

  // We can put member variables here for other systems in the future
};
