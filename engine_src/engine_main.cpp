#include "../lib_window/include/window.h"
#include <iostream>

int main() {
  if (!Window::Initialize(1200, 800, "Test Window")) {
    std::cerr << "[FATAL] Failed to initialize window." << std::endl;
    return -1;
  }

  while (!Window::ShouldClose()) {
    Window::PollEvents();

    Window::SwapBuffers();

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      break;
    }
  }

  Window::Shutdown();

  return 0;
}
