#include "../include/window.h"

EventManager Window::eventManager;
GLFWwindow *Window::window = nullptr;
int Window::width = 0;
int Window::height = 0;
bool Window::isFullscreen = false;
int Window::windowedPosX = 0;
int Window::windowedPosY = 0;
int Window::windowedWidth = 0;
int Window::windowedHeight = 0;

bool Window::Initialize(int width, int height, const std::string &title) {
  if (!glfwInit()) {
    Log::Write(Log::FATAL, "Failed to initialize glfw. Error running `glfwInit()`");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    Log::Write(Log::FATAL, "Failed to create window. `glfwCreateWindow()` returned NULL");
    glfwTerminate();

    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, nullptr);

  if (!gladLoadGL(glfwGetProcAddress)) {
    Log::Write(Log::FATAL,
               "Failed to initialize glad. `gladLoadGLLoader()` returned NULL");

    return false;
  }

  Window::SetSize(width, height);
  windowedWidth = width;
  windowedHeight = height;
  Window::GetPosition(windowedPosX, windowedPosY);

  glfwSetKeyCallback(window, KeyCallback);
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);

  return true;
}

void Window::Run(std::function<void()> loop_body) {
  while (!ShouldClose()) {
    PollEvents();
    loop_body();
  }

  Shutdown();
}

void Window::Shutdown() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

GLFWwindow *Window::GetGLFWWindow() { return window; }

void Window::PollEvents() { glfwPollEvents(); }

bool Window::ShouldClose() { return glfwWindowShouldClose(window); }

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
  if (fullscreen == isFullscreen) { return; }

  if (fullscreen) {
    glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
    glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
      Log::Write(Log::FATAL,
                 "Error getting primary monitor in `glfwGetPrimaryMonitor()`");

      return;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (!mode) {
      Log::Write(Log::FATAL, "Error getting video mode in `glfwGetVideoMode()`");

      return;
    }

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height,
                         mode->refreshRate);
    Window::SetSize(mode->width, mode->height);
  } else {
    glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth,
                         windowedHeight, 0);
    Window::SetSize(windowedWidth, windowedHeight);
  }

  isFullscreen = fullscreen;
}

void Window::GetMousePosition(double &x, double &y) { glfwGetCursorPos(window, &x, &y); }

void Window::SetMousePosition(double x, double y) { glfwSetCursorPos(window, x, y); }

void Window::RegisterObserver(IEventObserver *observer) {
  eventManager.RegisterObserver(observer);
}

void Window::UnregisterObserver(IEventObserver *observer) {
  eventManager.UnregisterObserver(observer);
}

void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (action == GLFW_PRESS) {
    Window::eventManager.NotifyKeyPress(key);
  } else if (action == GLFW_RELEASE) {
    Window::eventManager.NotifyKeyRelease(key);
  }
}

void Window::MouseCallback(GLFWwindow *window, double xpos, double ypos) {
  Window::eventManager.NotifyMouseMove(xpos, ypos);
}

void Window::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (action == GLFW_PRESS) { Window::eventManager.NotifyMouseClick(button); }
}
