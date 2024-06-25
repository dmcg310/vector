#include "imgui_manager.h"

#ifdef _DEBUG

bool ImGuiManager::Initialize(GLFWwindow *window, API api) {
  renderAPI = api;
  return Setup(window);
}

bool ImGuiManager::Setup(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  switch (renderAPI) {
    case API::OpenGL:
      if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        Log::Write(Log::FATAL, "Failed to initialize ImGui GLFW implementation");
        return false;
      }

      if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        Log::Write(Log::FATAL, "Failed to initialize ImGui OpenGL3 implementation");
        return false;
      }

      texture = new OpenGLTexture();
      texture->Create(800, 600, GL_RGBA);

      framebuffer = new OpenGLFramebuffer();
      framebuffer->Create();
      framebuffer->AttachTexture(texture);

      renderbuffer = new OpenGLRenderbuffer();
      renderbuffer->Create();
      renderbuffer->SetStorage(GL_DEPTH24_STENCIL8, 800, 600);
      framebuffer->AttachRenderbuffer(renderbuffer);

      break;

    default:
      Log::Write(Log::ERROR, "Unknown Render API provided");
      return false;
  }

  initialized = true;
  return true;
}

void ImGuiManager::ResizeViewport(uint32_t width, uint32_t height) {
  delete texture;
  delete renderbuffer;

  switch (renderAPI) {
    case API::OpenGL:
      texture = new OpenGLTexture();
      texture->Create(width, height, GL_RGBA);

      framebuffer->Bind();
      framebuffer->AttachTexture(texture);

      renderbuffer = new OpenGLRenderbuffer();
      renderbuffer->Create();
      renderbuffer->SetStorage(GL_DEPTH24_STENCIL8, width, height);

      framebuffer->AttachRenderbuffer(renderbuffer);

      break;
  }
}

void ImGuiManager::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
  ImGui::DockSpaceOverViewport(dockspaceID, ImGui::GetMainViewport(),
                               ImGuiDockNodeFlags_PassthruCentralNode);

  if (isDebugMenuOpen) { RenderDebugMenu(); }

  ImGui::Begin("Viewport");

  ImVec2 viewportScreenSize = ImGui::GetContentRegionAvail();
  if (glm::vec2 newViewportSize = {viewportScreenSize.x, viewportScreenSize.y};
      newViewportSize != viewportSize) {
    ResizeViewport(static_cast<uint32_t>(newViewportSize.x),
                   static_cast<uint32_t>(newViewportSize.y));
    viewportSize = newViewportSize;
  }

  switch (renderAPI) {
    case API::OpenGL:
      framebuffer->Unbind();

      RenderPass *renderPass = new OpenGLRenderPass();
      renderPass->SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      renderPass->Begin();

      break;
  }

  ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(texture->GetID())),
               viewportScreenSize, ImVec2{0, 1}, ImVec2{1, 0});

  ImGui::End();
  ImGui::Render();

  switch (renderAPI) {
    case API::OpenGL:
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      break;
  }

  ImGuiIO const &io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiManager::RenderDebugMenu() {
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(300.0f, ImGui::GetIO().DisplaySize.y),
                           ImGuiCond_FirstUseEver);
  ImGui::Begin("Options");

  static float fps = 0.0f;
  fps = 0.9f * fps + 0.1f * ImGui::GetIO().Framerate;
  ImGui::Text("FPS: %.1f", fps);

  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetIO().DisplaySize.y * 0.7f),
                          ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(
          ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y * 0.3f),
          ImGuiCond_FirstUseEver);
  ImGui::Begin("Logs");

  if (ImGui::CollapsingHeader("Logs")) {
    ImGui::BeginChild("LogWindow", ImVec2(0, 200), true);

    // Display normal logs
    for (const auto &log: Log::GetLogBuffer()) {
      ImGui::TextUnformatted(log.second.c_str());
    }

    // Display frame logs
    for (const auto &frameLog: Log::GetFrameLogBuffer()) {
      ImGui::TextUnformatted(frameLog.c_str());
    }

    ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
  }

  ImGui::End();
}

void ImGuiManager::Shutdown() {
  switch (renderAPI) {
    case API::OpenGL:
      ImGui_ImplOpenGL3_Shutdown();
      break;
  }

  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  delete texture;
  delete framebuffer;
  delete renderbuffer;
}

#endif