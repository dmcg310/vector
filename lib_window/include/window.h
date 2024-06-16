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

  /*
   * This function encapsulates the entire process of setting up a window,
   * initializing the necessary components, and running the main loop until the
   * window should close. It accepts a user-defined function (loop_body) that
   * will be called repeatedly during the main loop to handle rendering and
   * other per-frame logic.
   */
  static bool Create(int width, int height, const std::string &title,
                     std::function<void()> loop_body);

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
