#include "../include/application.h"

#ifdef _DEBUG
Application::Application() : frameCount(0), startTime(std::chrono::steady_clock::now()), 
                             accumulatedTime(0.0), accumulatedFPS(0.0), sampleFrames(100), 
                             frameSamples(0) {}
#else
Application::Application() {}
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

#ifdef _DEBUG
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  ImGuiStyle style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  ImGui_ImplGlfw_InitForOpenGL(Window::GetGLFWWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
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
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif

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
  // Update game logic here...
}

void Application::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef _DEBUG
  if (isDebugMenuOpen) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Vector Engine Debug Menu", nullptr, windowFlags);

    ImGuiID dockspaceId = ImGui::GetID("VectorDockSpace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();

    RenderDebugMenu();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();

      glfwMakeContextCurrent(backupCurrentContext);
    }
  }
#endif
  // Swap buffers (handled by Window::SwapBuffers)
}

#ifdef _DEBUG
void Application::RenderDebugMenu() {
  static bool initialized = false;

  if (!initialized) {
    ImGui::DockBuilderRemoveNode(ImGui::GetID("VectorDockSpace")); // Clear existing layout
    ImGui::DockBuilderAddNode(ImGui::GetID("VectorDockSpace"), ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(ImGui::GetID("VectorDockSpace"), ImGui::GetIO().DisplaySize);

    ImGuiID dockMainId = ImGui::GetID("VectorDockSpace");
    ImGuiID dockLeftId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Left, 0.2f, nullptr, &dockMainId);
    ImGuiID dockBottomId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Down, 0.3f, nullptr, &dockMainId);

    ImGui::DockBuilderDockWindow("Options", dockLeftId);
    ImGui::DockBuilderDockWindow("Logs", dockBottomId);
    ImGui::DockBuilderFinish(dockMainId);

    initialized = true;
  }

  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(300.0f, ImGui::GetIO().DisplaySize.y), ImGuiCond_FirstUseEver);
  ImGui::Begin("Options");

  static float fps = 0.0f;
  fps = 0.9f * fps + 0.1f * ImGui::GetIO().Framerate;
  ImGui::Text("FPS: %.1f", fps);

  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetIO().DisplaySize.y * 0.7f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y * 0.3f), ImGuiCond_FirstUseEver);
  ImGui::Begin("Logs");

  if (ImGui::CollapsingHeader("Logs")) {
    ImGui::BeginChild("LogWindow", ImVec2(0, 200), true);

    // Display normal logs
    for (const auto& log : Log::GetLogBuffer()) {
      ImGui::TextUnformatted(log.second.c_str());
    }

    // Display frame logs
    for (const auto& frameLog : Log::GetFrameLogBuffer()) {
      ImGui::TextUnformatted(frameLog.c_str());
    }

    ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
  }

  ImGui::End();
}
#endif

void Application::MainLoopBody() {
  HandleEvents();
  Update();
  Render();
}
