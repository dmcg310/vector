#include "opengl_context.h"
#include <stdexcept>

OpenGLContext::OpenGLContext(GLFWwindow *window) : window(window) {}

void OpenGLContext::Initialize() {
  // Most of the stuff that should be in here is already handled in lib_window
  // though that code will probably need to be rewritten if we want to utilise
  // other APIs from OpenGL in the future

  if (!gladLoadGL(glfwGetProcAddress)) {
    // We can catch this in lib_application and then log it properly
    throw new std::runtime_error("Failed to initialize GLAD");
  }
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(window); }
