#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
  static bool Initialize(int width, int height, const std::string &title);
  static void Shutdown();

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

private:
  static int width;
  static int height;
};

#endif // !WINDOW_H
