#include "../include/window.h"

int Window::width = 0;
int Window::height = 0;

static GLFWwindow *window = nullptr;

bool Window::Initialize(int width, int height, const std::string &title) {
  if (!glfwInit()) {
    return false;
  }

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  Window::SetSize(width, height);

  return true;
}

void Window::Shutdown() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::PollEvents() { glfwPollEvents(); }

bool Window::ShouldClose() { return glfwWindowShouldClose(window); }

void Window::SwapBuffers() { return glfwSwapBuffers(window); }

void Window::SetTitle(const std::string &title) {
  glfwSetWindowTitle(window, title.c_str());
}

int Window::GetWidth() { return Window::width; }

int Window::GetHeight() { return Window::height; }

void Window::SetSize(int width, int height) {
  glfwSetWindowSize(window, width, height);

  Window::width = width;
  Window::height = height;
}

void Window::GetPosition(int &x, int &y) { glfwGetWindowPos(window, &x, &y); }

void Window::SetPosition(int x, int y) { glfwSetWindowPos(window, x, y); }

bool Window::IsFullscreen() { return true; }

void Window::SetFullscreen(bool fullscreen) {}
