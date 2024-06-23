#pragma once

#include "context.h"
#include "../../lib_window/include/window.h" // Workaround for now

enum class API {
  OpenGL,
};

class RenderAPIFactory {
public:
  static Context* CreateContext(API api, GLFWwindow* window);
};