#include "../include/application.h"

#ifdef _DEBUG
Application::Application() : frameCount(0), startTime(std::chrono::steady_clock::now()), 
                             accumulatedTime(0.0), accumulatedFPS(0.0), sampleFrames(100), 
                             frameSamples(0), context(nullptr), isDebugMode(true) {}
#else
Application::Application() : context(nullptr) {}
#endif

bool Application::Initialize() {
  ApplicationSettings applicationSettings;
  ApplicationSettings::Config config;

  if (!applicationSettings.CheckIfConfigExists()) {
    Log::Write(Log::ERROR,
               "Config file was not found. Creating one with defaults");

    config = applicationSettings.CreateConfig();
  } else {
    config = applicationSettings.LoadConfig();
  }

  Log::Initialize(config.log.logFilePath, config.log.logToFile,
                  config.log.logToConsole, config.log.resetLogFile);

  if (!Window::Initialize(config.window.width, config.window.height,
                          config.window.title)) {
    Log::Write(Log::FATAL, "Cannot initialize application window");

    return false;
  }

  if (config.window.fullscreen) {
    Window::SetFullscreen(true);
  }

  Window::RegisterObserver(this);

  API selectedAPI = API::OpenGL; // This could definitely be loaded from our config.vector
  context = RenderAPIFactory::CreateContext(selectedAPI, Window::GetGLFWWindow());

  try {
    context->Initialize();
  } catch (const std::runtime_error& e) {
    Log::Write(Log::FATAL, e.what());
    
    return false;
  }

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

  if (key == GLFW_KEY_ESCAPE) {
    Shutdown();
  }
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

void Application::HandleEvents() {
  // Handle events
}

void Application::Update() {
#ifdef _DEBUG
  // Increment the frame count
  frameCount++;

  // Calculate the elapsed time since the application started
  auto currentTime = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsedTime = currentTime - startTime;

  // Calculate frame time (delta time)
  static auto lastTime = std::chrono::steady_clock::now();
  auto deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
  lastTime = currentTime;

  // Accumulate values
  accumulatedTime += deltaTime;
  accumulatedFPS += 1.0 / deltaTime;
  frameSamples++;

  if (frameSamples >= sampleFrames) {
    double averageFPS = accumulatedFPS / frameSamples;
    double averageFrameTime = accumulatedTime / frameSamples;

    // Log average frame information to ImGui buffer
    std::ostringstream oss;
    oss << "Frames: " << frameSamples
      << ", Average FPS: " << averageFPS
      << ", Average Frame Time: " << averageFrameTime << "s";
    Log::WriteFrameLog(oss.str());

    // Reset accumulators
    accumulatedTime = 0.0;
    accumulatedFPS = 0.0;
    frameSamples = 0;
  }
#endif

  // Update game logic here
}

void Application::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef _DEBUG
  if (isDebugMenuOpen) {
    imguiManager.Render();
  } else {
    // Normal rendering when debug menu is closed

    // Below is some testing code
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex2f(0.0f, 0.5f);
    glEnd();
  }

#else
  // Normal rendering for release mode
#endif
  context->SwapBuffers();
}

void Application::MainLoopBody() {
  HandleEvents();
  Update();
  Render();
}
