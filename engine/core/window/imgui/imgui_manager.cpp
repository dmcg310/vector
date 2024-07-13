#include "imgui_manager.h"
#include "../../../renderer/renderer.h"

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
      break;

    default:
      Log::Write(Log::ERROR, "Unknown Render API provided");
      return false;
  }

  texture = Texture::CreateTexture();
  texture->Create(800, 600, GL_RGBA);

  framebuffer = Framebuffer::CreateFramebuffer();
  framebuffer->Create();
  framebuffer->AttachTexture(texture);

  renderbuffer = Renderbuffer::CreateRenderbuffer();
  renderbuffer->Create();

  switch (renderAPI) {
    case API::OpenGL:
      renderbuffer->SetStorage(GL_DEPTH24_STENCIL8, 800, 600);
      break;

    default:
      break;
  }

  framebuffer->AttachRenderbuffer(renderbuffer);

  initialized = true;
  return true;
}

void ImGuiManager::ResizeViewport(uint32_t width, uint32_t height) {
  texture = Texture::CreateTexture();
  texture->Create((int) width, (int) height, GL_RGBA);

  framebuffer->Bind();
  framebuffer->AttachTexture(texture);

  renderbuffer = Renderbuffer::CreateRenderbuffer();
  renderbuffer->Create();

  switch (renderAPI) {
    case API::OpenGL:
      renderbuffer->SetStorage(GL_DEPTH24_STENCIL8, (int) width, (int) height);
      break;

    default:
      break;
  }

  framebuffer->AttachRenderbuffer(renderbuffer);
}

void ImGuiManager::Render() {
  static bool justRenamed = false;
  static bool justDuplicated = false;
  static bool justDeleted = false;

  switch (renderAPI) {
    case API::OpenGL:
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      break;

    default:
      break;
  }

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

  framebuffer->Unbind();

  auto renderPass = RenderPass::CreateRenderPass();
  renderPass->SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  renderPass->Begin();

  ImGui::Image(reinterpret_cast<ImTextureID>(texture->GetID()), viewportScreenSize,
               ImVec2{0, 1}, ImVec2{1, 0});

  ImGui::End();
  ImGui::Render();

  switch (renderAPI) {
    case API::OpenGL:
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      break;

    default:
      break;
  }

  ImGuiIO const &io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    glfwMakeContextCurrent(backup_current_context);
  }

  if (justRenamed || justDuplicated || justDeleted) {
    Refresh();

    justRenamed = false;
    justDuplicated = false;
    justDeleted = false;
  }
}

void ImGuiManager::RenderDebugMenu() {
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(300.0f, ImGui::GetIO().DisplaySize.y),
                           ImGuiCond_FirstUseEver);
  ImGui::Begin("Options");

  auto &renderer = Renderer::GetInstance();
  auto currentScene = renderer.GetCurrentScene();
  auto nodes = currentScene->GetNodes();
  auto selectedNode = renderer.GetSelectedNode();

  if (!nodes.empty()) {
    auto rootNode = nodes[0];

    std::string nodeName =
            rootNode->GetName() + " (" + rootNode->GetNodeType() + ") [Root]";

    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow |
                                   ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                   ImGuiTreeNodeFlags_DefaultOpen;

    if (selectedNode == rootNode) { nodeFlags |= ImGuiTreeNodeFlags_Selected; }

    bool nodeOpen = ImGui::TreeNodeEx(nodeName.c_str(), nodeFlags);

    if (ImGui::IsItemClicked()) {
      if (selectedNode == rootNode) {
        currentScene->SelectNode(nullptr);
      } else {
        currentScene->SelectNode(rootNode);
      }
    }

    if (nodeOpen) {
      for (const auto &child: rootNode->GetChildren()) {
        RenderNode(child, selectedNode);
      }

      ImGui::TreePop();
    }
  }

  if (selectedNode) {
    ImGui::Separator();

    // if (auto textureNode = std::dynamic_pointer_cast<Texture2DNode>(selectedNode)) {
    //   ImGui::Text("Properties");

    //   static auto position = textureNode->GetPosition();
    //   static glm::vec2 rotation = textureNode->GetRotation();
    //   static glm::vec2 scale = textureNode->GetScale();

    //   if (ImGui::SliderFloat("X Position", &position.x, -1.0f, 1.0f, "%.3f")) {
    //     textureNode->SetPosition(position);
    //   }
    //   if (ImGui::SliderFloat("Y Position", &position.y, -1.0f, 1.0f, "%.3f")) {
    //     textureNode->SetPosition(position);
    //   }
    //   if (ImGui::SliderFloat("Rotation", &rotation.x, -360.0f, 360.0f, "%.3f")) {
    //     textureNode->SetRotation(rotation);
    //   }
    //   if (ImGui::SliderFloat("X Scale", &scale.x, 0.1f, 10.0f, "%.3f")) {
    //     textureNode->SetScale(scale);
    //   }
    //   if (ImGui::SliderFloat("Y Scale", &scale.y, 0.1f, 10.0f, "%.3f")) {
    //     textureNode->SetScale(scale);
    //   }

    //   if (ImGui::Button("Reset Position")) {
    //     position = glm::vec2(0.0f, 0.0f);
    //     textureNode->SetPosition(position);
    //   }

    //   ImGui::SameLine();
    //   if (ImGui::Button("Reset Rotation")) {
    //     rotation = glm::vec2(0.0f, 0.0f);
    //     textureNode->SetRotation(rotation);
    //   }

    //   if (ImGui::Button("Reset Scale")) {
    //     scale = glm::vec2(1.0f, 1.0f);
    //     textureNode->SetScale(scale);
    //   }
    // } else {
    //   ImGui::Text("This node is not editable.");
    // }
  }

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

void ImGuiManager::RenderNode(const std::shared_ptr<SceneNode> &node,
                              const std::shared_ptr<SceneNode> &selectedNode) {
  auto children = node->GetChildren();
  static std::string renameNodeName;
  static bool renameActive = false;
  static std::shared_ptr<SceneNode> nodeToRename = nullptr;
  static char newNameBuffer[128] = "";
  static bool justRenamed = false;
  static bool justDuplicated = false;
  static bool justDeleted = false;

  if (children.empty()) {
    std::string nodeName = node->GetName() + " (" + node->GetNodeType() + ")";
    nodeName += " [Child]";

    ImGuiTreeNodeFlags nodeFlags =
            ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if (selectedNode == node) { nodeFlags |= ImGuiTreeNodeFlags_Selected; }

    ImGui::TreeNodeEx(nodeName.c_str(), nodeFlags);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) { ImGui::OpenPopup("NodePopup"); }

    if (ImGui::BeginPopup("NodePopup")) {
      if (ImGui::MenuItem("Delete Node")) {
        if (auto parent = node->GetParent()) {
          parent->RemoveChild(node);

          Renderer::GetInstance().GetCurrentScene()->RemoveNode(node);
          Renderer::GetInstance().GetCurrentScene()->SelectNode(nullptr);

          justDeleted = true;

          // Odd case, if we delete the last child of the root node normally,
          // the viewport won't refresh, so we have to force it
          if (Renderer::GetInstance().GetCurrentScene()->GetNodes().size() == 1) {
            Refresh();
          }
        }
      }

      if (ImGui::MenuItem("Rename")) {
        renameActive = true;
        nodeToRename = node;

        renameNodeName = node->GetName();
        strncpy(newNameBuffer, renameNodeName.c_str(), sizeof(newNameBuffer));
        newNameBuffer[sizeof(newNameBuffer) - 1] = '\0';
      }

      ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked()) {
      if (selectedNode == node) {
        Renderer::GetInstance().GetCurrentScene()->SelectNode(nullptr);
      } else {
        Renderer::GetInstance().GetCurrentScene()->SelectNode(node);
      }
    }
  } else {
    std::string nodeName = node->GetName() + " (" + node->GetNodeType() + ")";
    nodeName += " [Parent]";

    ImGuiTreeNodeFlags nodeFlags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (selectedNode == node) { nodeFlags |= ImGuiTreeNodeFlags_Selected; }

    bool nodeOpen = ImGui::TreeNodeEx(nodeName.c_str(), nodeFlags);

    if (ImGui::IsItemClicked()) {
      if (selectedNode == node) {
        Renderer::GetInstance().GetCurrentScene()->SelectNode(nullptr);
      } else {
        Renderer::GetInstance().GetCurrentScene()->SelectNode(node);
      }
    }

    if (nodeOpen) {
      for (const auto &child: children) { RenderNode(child, selectedNode); }

      ImGui::TreePop();
    }
  }

  if (renameActive && nodeToRename == node) {
    ImGui::OpenPopup("Rename Node");
    renameActive = false;
  }

  if (ImGui::BeginPopupModal("Rename Node", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Enter new name for the node:");
    ImGui::InputText("##newname", newNameBuffer, IM_ARRAYSIZE(newNameBuffer));

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      if (nodeToRename) {
        nodeToRename->SetName(std::string(newNameBuffer));
        justRenamed = true;
      }

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

    ImGui::EndPopup();
  }
}

void ImGuiManager::Shutdown() {
  switch (renderAPI) {
    case API::OpenGL:
      ImGui_ImplOpenGL3_Shutdown();
      break;
  }

  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiManager::Refresh() {
  // Sometimes we have to refresh the viewport size after scene changes
  auto size = framebuffer->GetSize(Window::GetGLFWWindow());
  ResizeViewport(static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
}

void ImGuiManager::ToggleDebugMenu() {
  if (Renderer::GetInstance().GetCurrentScene()->GetNodes().size() == 1) { return; }

  isDebugMenuOpen = !isDebugMenuOpen;
}

#endif