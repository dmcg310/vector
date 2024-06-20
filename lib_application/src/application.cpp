#include "../include/application.h"
#include "../../lib_log/include/log.h"

Application::Application() {}

bool Application::Initialize() {
  if (!Window::Initialize(settings.windowWidth, settings.windowHeight,
                          settings.windowTitle)) {
    Log::Write(Log::FATAL, "Cannot initialize application window");

    return false;
  }

  if (settings.fullscreen) {
    Window::SetFullscreen(true);
  }

  // Initialize other systems here

  return true;
}

void Application::Run() {
  /*
   * The Run method initializes the main loop of the application.
   * It delegates the loop control to the Window::Run method,
   * passing a bound member function (MainLoopBody) which encapsulates
   * the core loop logic.
   *
   * The MainLoopBody function is called repeatedly within the
   * Window::Run loop, ensuring that the application continuously:
   * 1. Handles input events.
   * 2. Updates game logic.
   * 3. Renders the current frame.
   *
   * The std::bind function is used to create a callable object that
   * ties the MainLoopBody function to the current instance of the
   * Application class, allowing it to access member variables and
   * other member functions.
   */

  Log::Write(Log::INFO, "Application started successfully");

  Window::Run(std::bind(&Application::MainLoopBody, this));
}

void Application::Shutdown() {
  // Shutdown other systsems here
  Window::Shutdown();

  Log::Write(Log::INFO, "Application shut down successfully");

  exit(0);
}

void Application::HandleEvents() {
  // Handle events, like key presses etc
  if (Window::IsKeyPressed(GLFW_KEY_ESCAPE)) {
    Application::Shutdown();
  }
}

void Application::Update() {
  // Update game logic
}

void Application::Render() {
  // Render the frame
}

void Application::MainLoopBody() {
  HandleEvents();
  Update();
  Render();
}
