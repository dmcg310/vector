#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <functional>
#include <string>

class Window {
public:
  static bool Initialize(int width, int height, const std::string &title);
  static void Shutdown();

  static void Run(std::function<void()> loop_body);

  static void PollEvents();
  static bool ShouldClose();
  static void SwapBuffers();

  static void SetTitle(const std::string &title);

  static int GetWidth();
  static int GetHeight();

  static void SetSize(int width, int height);

  static void GetPosition(int &x, int &y);
  static void SetPosition(int x, int y);

  static bool IsFullscreen();
  static void SetFullscreen(bool fullscreen);

  /* Input related functions */

  static bool IsKeyPressed(int key);
  static bool IsKeyReleased(int key);

  static bool IsMouseButtonPressed(int button);
  static bool IsMouseButtonReleased(int button);

  static void GetMousePosition(double &x, double &y);
  static void SetMousePosition(double x, double y);

  static void SetScrollXOffset(double xOffset);
  static void SetScrollYOffset(double xOffset);

  static void GetMouseScroll(double &xOffset, double &yOffset);

private:
  static int width;
  static int height;
  static bool isFullscreen;
  static int windowedPosX, windowedPosY, windowedWidth, windowedHeight;
  static GLFWmonitor *monitor;
  static const GLFWvidmode *mode;
  static GLFWwindow *window;
  static double scrollXOffset, scrollYOffset;
};
#endif // !WINDOW_H
