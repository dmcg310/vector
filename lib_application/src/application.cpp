#include "../include/application.h"

#ifdef _DEBUG
Application::Application()
    : frameCount(0), startTime(std::chrono::steady_clock::now()),
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

  API selectedAPI =
      API::OpenGL; // This could definitely be loaded from our config.vector
  context =
      RenderAPIFactory::CreateContext(selectedAPI, Window::GetGLFWWindow());

  try {
    context->Initialize();
  } catch (const std::runtime_error &e) {
    Log::Write(Log::FATAL, e.what());

    return false;
  }

  // TESTING CODE
  if (selectedAPI == API::OpenGL) {
    buffer = new OpenGLVertexBuffer();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Vertex 0: Bottom-left
        0.5f,  -0.5f, 0.0f, // Vertex 1: Bottom-right
        0.5f,  0.5f,  0.0f, // Vertex 2: Top-right
        -0.5f, 0.5f,  0.0f  // Vertex 3: Top-left
    };

    buffer->Create(sizeof(vertices), vertices);
    buffer->Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    indexBuffer = new OpenGLIndexBuffer();

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    indexBuffer->Create(sizeof(indices), indices);
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

  if (buffer) {
    delete buffer;
  }

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
  auto deltaTime =
      std::chrono::duration<double>(currentTime - lastTime).count();
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
    oss << "Frames: " << frameSamples << ", Average FPS: " << averageFPS
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

  // TESTING CODE
  if (buffer) {
    buffer->Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
  }

  if (indexBuffer) {
    indexBuffer->Bind();
  }

#ifdef _DEBUG
  if (isDebugMenuOpen) {
    imguiManager.Render();
  } else {
    // Normal rendering when debug menu is closed

    // TESTING CODE BELOW
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
