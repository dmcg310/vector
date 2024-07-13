#pragma once

#include "context.h"
#include "../opengl_includes.h"

class OpenGLContext : public Context {
public:
  OpenGLContext(GLFWwindow *window);

  void Initialize() override;
  void SwapBuffers() override;

private:
  GLFWwindow *window;
};
