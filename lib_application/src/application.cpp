#include "application.h"
#include "../scenes/test_scene/test_scene.h"
#include "renderer.h"
#include <chrono>
#include <memory>

Application::Application()
#ifdef _DEBUG
    : frameCount(0), startTime(std::chrono::steady_clock::now()), accumulatedTime(0.0),
      accumulatedFPS(0.0), sampleFrames(100), frameSamples(0), renderer(nullptr),
      camera(glm::vec3(0.0f, 0.0f, 3.0f))
#else
    : renderer(nullptr), camera(glm::vec3(0.0f, 0.0f, 3.0f))
#endif
{
}

bool Application::Initialize() {
  ApplicationSettings applicationSettings;
  ApplicationSettings::Config config;

  if (!applicationSettings.CheckIfConfigExists()) {
    Log::Write(Log::ERROR, "Config file was not found. Creating one with defaults");

    config = applicationSettings.CreateConfig();
  } else {
    config = applicationSettings.LoadConfig();
  }

  Log::Initialize(config.log.logFilePath, config.log.logToFile, config.log.logToConsole,
                  config.log.resetLogFile);

  if (!Window::Initialize(config.window.width, config.window.height,
                          config.window.windowedFullscreen,
                          config.window.borderlessFullscreen, config.window.fullscreen,
                          config.window.title)) {
    Log::Write(Log::FATAL, "Cannot initialize application window");

    return false;
  }

  if (config.window.fullscreen) { Window::SetFullscreen(true); }

  // Input event observer
  Window::RegisterObserver(this);

  Renderer::GetInstance().Initialize(Window::GetGLFWWindow());

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
  Renderer::GetInstance().Shutdown();

  Window::UnregisterObserver(this);
  Window::Shutdown();

  Log::Write(Log::INFO, "Application shut down successfully");
  Log::Shutdown();

  exit(0);
}

void Application::OnKeyPress(int key) {
#ifdef _DEBUG
  if (key == GLFW_KEY_F1) {
    ImGuiManager::GetInstance().ToggleDebugMenu();

    return;
  }
#endif

  if (key == GLFW_KEY_ESCAPE) { Shutdown(); }

  if (key == GLFW_KEY_C) {
    cameraControlEnabled = !cameraControlEnabled;
    cursorVisible = !cursorVisible;

    if (cursorVisible) {
      glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
      glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      // reset cursor position
      int width = Window::GetWidth();
      int height = Window::GetHeight();
      glfwSetCursorPos(Window::GetGLFWWindow(), width / 2.0, height / 2.0);
    }
  }

  if (cameraControlEnabled) {
    if (key == GLFW_KEY_W) camera.ProcessKeyboard(FORWARD, 0.1f);
    if (key == GLFW_KEY_S) camera.ProcessKeyboard(BACKWARD, 0.1f);
    if (key == GLFW_KEY_A) camera.ProcessKeyboard(LEFT, 0.1f);
    if (key == GLFW_KEY_D) camera.ProcessKeyboard(RIGHT, 0.1f);
  }

  pressedKeys.insert(key);
}

void Application::OnKeyRelease(int key) {
  // Handle key releases
  pressedKeys.erase(key);
}

void Application::OnMouseMove(double x, double y) {
  if (cameraControlEnabled) {
    static bool firstMouse = true;
    static float lastX = Window::GetWidth() / 2.0;
    static float lastY = Window::GetHeight() / 2.0;

    if (firstMouse) {
      lastX = x;
      lastY = y;
      firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y; // Reversed since y-coordinates go from bottom to top

    lastX = x;
    lastY = y;

    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void Application::OnMouseClick(int button) {
  // Handle mouse clicks
}

void Application::OnScroll(double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Application::Update() {
  auto currentTime = std::chrono::steady_clock::now();
  static auto lastTime = std::chrono::steady_clock::now();
  float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
  lastTime = currentTime;

  Renderer::GetInstance().Update(deltaTime);

  if (cameraControlEnabled) {
    if (pressedKeys.find(GLFW_KEY_W) != pressedKeys.end())
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if (pressedKeys.find(GLFW_KEY_S) != pressedKeys.end())
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (pressedKeys.find(GLFW_KEY_A) != pressedKeys.end())
      camera.ProcessKeyboard(LEFT, deltaTime);
    if (pressedKeys.find(GLFW_KEY_D) != pressedKeys.end())
      camera.ProcessKeyboard(RIGHT, deltaTime);
  }

#ifdef _DEBUG
  LogFrameInfo(deltaTime);
#endif
}

void Application::Render() {
  Renderer::GetInstance().SetViewMatrixAndZoom(camera.GetViewMatrix(), camera.Zoom);
  Renderer::GetInstance().Render();
}

void Application::MainLoopBody() {
  Update();
  Render();
}

void Application::LogFrameInfo(float deltaTime) {
#ifdef _DEBUG
  frameCount++;

  accumulatedTime += deltaTime;
  accumulatedFPS += 1.0 / deltaTime;

  frameSamples++;

  if (frameSamples >= sampleFrames) {
    double averageFPS = accumulatedFPS / frameSamples;
    double averageFrameTime = accumulatedTime / frameSamples;

    std::ostringstream oss;
    oss << "Frames: " << frameSamples << ", Average FPS: " << averageFPS
        << ", Average Frame Time: " << averageFrameTime << "s";

    Log::WriteFrameLog(oss.str());

    accumulatedTime = 0.0;
    accumulatedFPS = 0.0;
    frameSamples = 0;
  }
#endif
}