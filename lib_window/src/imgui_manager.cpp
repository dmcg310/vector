#ifdef _DEBUG

#include "../include/imgui_manager.h"

void ImGuiManager::Initialize(GLFWwindow *window) {
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

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiManager::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
  ImGui::DockSpaceOverViewport(dockspaceID, ImGui::GetMainViewport(),
                               ImGuiDockNodeFlags_PassthruCentralNode);

  RenderDebugMenu();

  // Viewport window
  ImGui::Begin("Viewport");
  ImGui::Text("This is where the viewport rendering will go.");
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO &io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiManager::RenderDebugMenu() {
  if (!initialized) {
    ImGui::DockBuilderRemoveNode(
        ImGui::GetID("VectorDockSpace")); // Clear existing layout
    ImGui::DockBuilderAddNode(ImGui::GetID("VectorDockSpace"),
                              ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(ImGui::GetID("VectorDockSpace"),
                                  ImGui::GetIO().DisplaySize);

    ImGuiID dockMainId = ImGui::GetID("VectorDockSpace");
    ImGuiID dockLeftId = ImGui::DockBuilderSplitNode(
        dockMainId, ImGuiDir_Left, 0.2f, nullptr, &dockMainId);
    ImGuiID dockBottomId = ImGui::DockBuilderSplitNode(
        dockMainId, ImGuiDir_Down, 0.3f, nullptr, &dockMainId);

    ImGui::DockBuilderDockWindow("Options", dockLeftId);
    ImGui::DockBuilderDockWindow("Logs", dockBottomId);
    ImGui::DockBuilderFinish(dockMainId);

    initialized = true;
  }

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
    for (const auto &log : Log::GetLogBuffer()) {
      ImGui::TextUnformatted(log.second.c_str());
    }

    // Display frame logs
    for (const auto &frameLog : Log::GetFrameLogBuffer()) {
      ImGui::TextUnformatted(frameLog.c_str());
    }

    ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
  }

  ImGui::End();
}

void ImGuiManager::Shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

#endif
