#pragma once

#include "../../../include/context.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLContext : public Context {
public:
  OpenGLContext(GLFWwindow* window);

  void Initialize() override;
  void SwapBuffers() override;

private:
  GLFWwindow* window;
};