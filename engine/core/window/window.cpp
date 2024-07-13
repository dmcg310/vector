#include "window.h"

EventManager Window::eventManager;
GLFWwindow *Window::window = nullptr;
int Window::width = 0;
int Window::height = 0;
bool Window::isFullscreen = false;
bool Window::isBorderlessFullscreen = false;
int Window::windowedPosX = 0;
int Window::windowedPosY = 0;
int Window::windowedWidth = 0;
int Window::windowedHeight = 0;

bool Window::Initialize(int _width, int _height, bool windowedFullscreen,
                        bool borderlessFullscreen, bool fullscreen,
                        const std::string &title) {
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

  window = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
  if (!window) {
    Log::Write(Log::FATAL, "Failed to create window. `glfwCreateWindow()` returned NULL");
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, nullptr);
  glfwSwapInterval(0);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    Log::Write(Log::FATAL,
               "Failed to initialize glad. `gladLoadGLLoader()` returned NULL");
    return false;
  }

  Window::SetSize(_width, _height);
  windowedWidth = _width;
  windowedHeight = _height;
  Window::GetPosition(windowedPosX, windowedPosY);

  glfwSetKeyCallback(window, KeyCallback);
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glfwSetScrollCallback(window, ScrollCallback);

  if (fullscreen) {
    SetFullscreen(true, false);
  } else if (borderlessFullscreen) {
    SetFullscreen(true, true);
  } else if (windowedFullscreen) {
    glfwMaximizeWindow(window);
  }

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

int Window::GetWidth() { return Window::width; }

int Window::GetHeight() { return Window::height; }

void Window::SetSize(int _width, int _height) {
  glfwSetWindowSize(window, _width, _height);
  Window::width = _width;
  Window::height = _height;
}

void Window::GetPosition(int &x, int &y) { glfwGetWindowPos(window, &x, &y); }

void Window::SetFullscreen(bool fullscreen, bool borderless) {
  if (fullscreen == isFullscreen && borderless == isBorderlessFullscreen) { return; }

  isBorderlessFullscreen = borderless;

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

    if (borderless) {
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
      glfwSetWindowSize(window, mode->width, mode->height);
      glfwSetWindowPos(window, 0, 0);
    } else {
      glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height,
                           mode->refreshRate);
      Window::SetSize(mode->width, mode->height);
    }
  } else {
    glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth,
                         windowedHeight, 0);
    Window::SetSize(windowedWidth, windowedHeight);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
  }

  isFullscreen = fullscreen;
}

void Window::RegisterObserver(IEventObserver *observer) {
  eventManager.RegisterObserver(observer);
}

void Window::UnregisterObserver(IEventObserver *observer) {
  eventManager.UnregisterObserver(observer);
}

void Window::KeyCallback(GLFWwindow *_window, int key, int scancode, int action,
                         int mods) {
  (void) _window;
  (void) scancode;
  (void) mods;

  if (action == GLFW_PRESS) {
    Window::eventManager.NotifyKeyPress(key);
  } else if (action == GLFW_RELEASE) {
    Window::eventManager.NotifyKeyRelease(key);
  }
}

void Window::MouseCallback(GLFWwindow *_window, double xpos, double ypos) {
  (void) _window;

  Window::eventManager.NotifyMouseMove(xpos, ypos);
}

void Window::MouseButtonCallback(GLFWwindow *_window, int button, int action, int mods) {
  (void) _window;
  (void) mods;

  if (action == GLFW_PRESS) { Window::eventManager.NotifyMouseClick(button); }
}

void Window::ScrollCallback(GLFWwindow *_window, double xoffset, double yoffset) {
  (void) _window;

  Window::eventManager.NotifyScroll(xoffset, yoffset);
}
