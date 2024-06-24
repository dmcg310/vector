#include "../include/application.h"

#ifdef _DEBUG
Application::Application()
    : frameCount(0), startTime(std::chrono::steady_clock::now()), accumulatedTime(0.0),
      accumulatedFPS(0.0), sampleFrames(100), frameSamples(0), context(nullptr) {}
#else
Application::Application() : context(nullptr) {}
#endif

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
                          config.window.title)) {
    Log::Write(Log::FATAL, "Cannot initialize application window");

    return false;
  }

  if (config.window.fullscreen) { Window::SetFullscreen(true); }

  // Input event observer
  Window::RegisterObserver(this);

  API selectedAPI = API::OpenGL;
  context = RenderAPIFactory::CreateContext(selectedAPI, Window::GetGLFWWindow());

  try {
    context->Initialize();
  } catch (const std::runtime_error &e) {
    Log::Write(Log::FATAL, e.what());
    return false;
  }

  auto testScene = std::make_shared<TestScene>();
  sceneManager.AddScene("TestScene", testScene);
  sceneManager.SetActiveScene("TestScene");

#ifdef _DEBUG
  imguiManager.Initialize(Window::GetGLFWWindow());
#endif

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
#ifdef _DEBUG
  imguiManager.Shutdown();
#endif

  sceneManager.Shutdown();

  delete context;

  Window::UnregisterObserver(this);
  Window::Shutdown();

  Log::Write(Log::INFO, "Application shut down successfully");
  Log::Shutdown();

  exit(0);
}

void Application::OnKeyPress(int key) {
#ifdef _DEBUG
  if (key == GLFW_KEY_F1) {
    isDebugMenuOpen = !isDebugMenuOpen;

    return;
  }
#endif

  if (key == GLFW_KEY_ESCAPE) { Shutdown(); }
}

void Application::OnKeyRelease(int key) {
  // Handle key releases
}

void Application::OnMouseMove(double x, double y) {
  // Handle mouse move
}

void Application::OnMouseClick(int button) {
  // Handle mouse click
}

void Application::Update() {
  auto currentTime = std::chrono::steady_clock::now();
  static auto lastTime = std::chrono::steady_clock::now();
  float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
  lastTime = currentTime;

  sceneManager.Update(deltaTime);

#ifdef _DEBUG
  LogFrameInfo(deltaTime);
#endif
}

void Application::Render() {
  sceneManager.Render();

#ifdef _DEBUG
  if (isDebugMenuOpen) {
    imguiManager.Render();
  }
#endif

  context->SwapBuffers();
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