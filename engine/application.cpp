#include "application.h"

Application::Application() : camera(glm::vec3(0.0f, 0.0f, 3.0f)) {}

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

  Window::RegisterObserver(this);
  Renderer::GetInstance().Initialize(Window::GetGLFWWindow());

#ifdef _RELEASE
  cameraControlEnabled = true;
#endif

  return true;
}

void Application::Run() {
  Log::Write(Log::INFO, "Application started successfully");
  Window::Run([this] { MainLoopBody(); });
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
  switch (key) {
    case GLFW_KEY_F1:
      ImGuiManager::GetInstance().ToggleDebugMenu();
      return;
    case GLFW_KEY_ESCAPE:
      Shutdown();
    case GLFW_KEY_C:
      ToggleCameraControl();
      return;
    case GLFW_KEY_W:
    case GLFW_KEY_S:
    case GLFW_KEY_A:
    case GLFW_KEY_D:
      if (cameraControlEnabled) {
        camera.ProcessKeyboard(static_cast<Camera_Movement>(key), 0.1f);
      }
      break;
    default:
      break;
  }

  pressedKeys.insert(key);
}

void Application::OnKeyRelease(int key) { pressedKeys.erase(key); }

void Application::OnMouseMove(double x, double y) {
  if (cameraControlEnabled) {
    static bool firstMouse = true;
    static float lastX = static_cast<float>(Window::GetWidth()) / 2.0f;
    static float lastY = static_cast<float>(Window::GetHeight()) / 2.0f;

    if (firstMouse) {
      lastX = static_cast<float>(x);
      lastY = static_cast<float>(y);
      firstMouse = false;
    }

    auto xoffset = static_cast<float>(x - lastX);
    auto yoffset = static_cast<float>(
            lastY - y); // Reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(x);
    lastY = static_cast<float>(y);

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

  if (cameraControlEnabled) { HandleCameraMovement(deltaTime); }

  LogFrameInfo(deltaTime);
}

void Application::Render() {
  Renderer::GetInstance().SetViewMatrixAndZoom(camera.GetViewMatrix(), camera.Zoom);
  Renderer::GetInstance().SetCameraPosition(camera.Position);
  Renderer::GetInstance().Render();
}

void Application::MainLoopBody() {
  Update();
  Render();
}

void Application::LogFrameInfo(float deltaTime) {
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
}

void Application::ToggleCameraControl() {
  cameraControlEnabled = !cameraControlEnabled;
  cursorVisible = !cursorVisible;

  if (cursorVisible) {
    glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else {
    glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ResetCursorPosition();
  }
}

void Application::ResetCursorPosition() {
  int width = Window::GetWidth();
  int height = Window::GetHeight();
  glfwSetCursorPos(Window::GetGLFWWindow(), width / 2.0, height / 2.0);
}

void Application::HandleCameraMovement(float deltaTime) {
  if (pressedKeys.find(GLFW_KEY_W) != pressedKeys.end()) {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }

  if (pressedKeys.find(GLFW_KEY_S) != pressedKeys.end()) {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }

  if (pressedKeys.find(GLFW_KEY_A) != pressedKeys.end()) {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }

  if (pressedKeys.find(GLFW_KEY_D) != pressedKeys.end()) {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}