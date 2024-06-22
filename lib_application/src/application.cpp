#include "../include/application.h"

Application::Application() {}

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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(Window::GetGLFWWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

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
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  Window::UnregisterObserver(this);

  Window::Shutdown();
  Log::Write(Log::INFO, "Application shut down successfully");
  Log::Shutdown();

  exit(0);
}

void Application::OnKeyPress(int key) {
  if (key == GLFW_KEY_ESCAPE) {
    Shutdown();
  }

  // Handle other key presses
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
  // Update game logic
}

void Application::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Hello, World!");
  ImGui::Text("This is some text.");
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap buffers (handled by Window::SwapBuffers)
}

void Application::MainLoopBody() {
  HandleEvents();
  Update();
  Render();
}
