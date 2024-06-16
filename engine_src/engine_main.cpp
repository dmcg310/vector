#include "../lib_window/include/window.h"
#include <iostream>

void main_loop_body() {
  // We can do some input handling here or something else
}

int main() {
  if (!Window::Create(1200, 800, "Test Window", main_loop_body)) {
    std::cerr << "[FATAL] Failed to initialize window." << std::endl;
    return -1;
  }

  return 0;
}
