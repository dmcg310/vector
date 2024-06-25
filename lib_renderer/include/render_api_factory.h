#pragma once

#include "../../lib_window/include/window.h" // Workaround for now
#include "context.h"

enum class API { OpenGL, None };

class RenderAPIFactory {
public:
  static Context *CreateContext(API api, GLFWwindow *window);
};
