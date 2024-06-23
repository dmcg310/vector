#pragma once

#ifdef _DEBUG

#include "../../lib_log/include/log.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

class ImGuiManager {
public:
  void Initialize(GLFWwindow *window);
  void Render();
  void RenderDebugMenu();
  void Shutdown();

private:
  bool initialized = false;
};

#endif
