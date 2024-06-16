#include "../lib_window/include/window.h"

int main() {
  if (!Window::Initialize(1200, 800, "Window Title")) {
    return -1;
  }

  while (!Window::ShouldClose()) {
    Window::PollEvents();

    Window::SwapBuffers();
  }

  Window::Shutdown();

  return 0;
}
