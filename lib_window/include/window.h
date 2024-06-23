#pragma once

#include "../../lib_log/include/log.h"
#include "event.h"
#include <functional>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Window {
public:
  static bool Initialize(int width, int height, const std::string &title);
  static void Run(std::function<void()> loop_body);
  static void Shutdown();

  static GLFWwindow *GetGLFWWindow();

  static void PollEvents();
  static bool ShouldClose();

  static void SetTitle(const std::string &title);

  static int GetWidth();
  static int GetHeight();

  static void SetSize(int width, int height);

  static void GetPosition(int &x, int &y);
  static void SetPosition(int x, int y);

  static bool IsFullscreen();
  static void SetFullscreen(bool fullscreen);

  static void GetMousePosition(double &x, double &y);
  static void SetMousePosition(double x, double y);

  static void RegisterObserver(IEventObserver *observer);
  static void UnregisterObserver(IEventObserver *observer);

private:
  static EventManager eventManager;
  static GLFWwindow *window;
  static int width, height;
  static bool isFullscreen;
  static int windowedPosX, windowedPosY, windowedWidth, windowedHeight;

  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void MouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void MouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);

  static void NotifyKeyPress(int key);
  static void NotifyKeyRelease(int key);
  static void NotifyMouseMove(double x, double y);
  static void NotifyMouseClick(int button);
};
