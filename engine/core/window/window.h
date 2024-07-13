#pragma once

#include "../log/log.h"
#include "event.h"
#include <functional>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Window {
public:
  static bool Initialize(int _width, int _height, bool windowedFullscreen,
                         bool borderlessFullscreen, bool fullscreen,
                         const std::string &title);
  static void Run(std::function<void()> loop_body);
  static void Shutdown();

  static GLFWwindow *GetGLFWWindow();

  static void PollEvents();
  static bool ShouldClose();

  static int GetWidth();
  static int GetHeight();

  static void SetSize(int _width, int _height);

  static void GetPosition(int &x, int &y);

  static void SetFullscreen(bool fullscreen, bool borderless = false);

  static void RegisterObserver(IEventObserver *observer);
  static void UnregisterObserver(IEventObserver *observer);

private:
  static EventManager eventManager;
  static GLFWwindow *window;
  static int width;
  static int height;
  static bool isFullscreen;
  static bool isBorderlessFullscreen;
  static int windowedPosX;
  static int windowedPosY;
  static int windowedWidth;
  static int windowedHeight;

  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void MouseCallback(GLFWwindow *_window, double xpos, double ypos);
  static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};