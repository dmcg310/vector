#pragma once

#include "../../../include/context.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class OpenGLContext : public Context {
public:
  OpenGLContext(GLFWwindow *window);

  void Initialize() override;
  void SwapBuffers() override;

private:
  GLFWwindow *window;
};
