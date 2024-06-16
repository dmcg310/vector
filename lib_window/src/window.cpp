#include "../include/window.h"

int Window::width = 0;
int Window::height = 0;

static GLFWwindow *window = nullptr;
static bool isFullscreen = false;
static int windowedWidth = 0, windowedHeight = 0;
static int windowedPosX = 0, windowedPosY = 0;

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

  windowedWidth = width;
  windowedHeight = height;
  Window::GetPosition(windowedPosX, windowedPosY);

  return true;
}

void Window::Shutdown() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::Run(std::function<void()> loop_body) {
  while (!ShouldClose()) {
    PollEvents();
    loop_body();
    SwapBuffers();
  }

  Shutdown();
}

bool Window::Create(int width, int height, const std::string &title,
                    std::function<void()> loop_body) {
  if (!Initialize(width, height, title)) {
    return false;
  }

  Run(loop_body);

  return true;
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

bool Window::IsFullscreen() { return isFullscreen; }

void Window::SetFullscreen(bool fullscreen) {
  if (fullscreen == isFullscreen) {
    return;
  }

  if (fullscreen) {
    glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
    glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
      return;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (!mode) {
      return;
    }

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height,
                         mode->refreshRate);

    Window::SetSize(mode->width, mode->height);
  } else {
    glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY,
                         windowedWidth, windowedHeight, 0);

    Window::SetSize(windowedWidth, windowedHeight);
  }

  isFullscreen = fullscreen;
}
