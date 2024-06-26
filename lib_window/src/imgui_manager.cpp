#include "imgui_manager.h"
#include "../../lib_renderer/include/renderer.h"

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

      framebuffer = Framebuffer::CreateFramebuffer();
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

  auto &renderer = Renderer::GetInstance();
  auto currentScene = renderer.GetCurrentScene();
  auto nodes = currentScene->GetNodes();
  auto selectedNode = renderer.GetSelectedNode();

  if (ImGui::CollapsingHeader("Node Hierarchy")) {
    for (size_t i = 0; i < nodes.size(); ++i) {
      bool isRootNode = (i == 0); // Assuming the first node is the root node

      std::string nodeName =
              "Node " + std::to_string(i) + " (" + typeid(*nodes[i]).name() + ")";

      if (isRootNode) { nodeName += " [Root]"; }

      if (ImGui::TreeNode(nodeName.c_str())) {
        if (!isRootNode &&
            ImGui::Selectable(nodeName.c_str(), selectedNode == nodes[i])) {
          if (selectedNode == nodes[i]) {
            currentScene->SelectNode(nullptr);
          } else {
            currentScene->SelectNode(nodes[i]);
          }
        }

        for (const auto &child: nodes[i]->GetChildren()) {
          RenderNode(child, selectedNode);
        }

        ImGui::TreePop();
      }
    }
  }

  if (selectedNode) {
    ImGui::Separator();
    ImGui::Text("Selected Node Properties");

    if (auto textureNode = std::dynamic_pointer_cast<Texture2DNode>(selectedNode)) {
      static glm::vec2 position = textureNode->GetPosition();
      static glm::vec2 rotation = textureNode->GetRotation();
      static glm::vec2 scale = textureNode->GetScale();

      if (ImGui::SliderFloat("X Position", &position.x, -1.0f, 1.0f, "%.3f")) {
        textureNode->SetPosition(position);
      }
      if (ImGui::SliderFloat("Y Position", &position.y, -1.0f, 1.0f, "%.3f")) {
        textureNode->SetPosition(position);
      }
      if (ImGui::SliderFloat("Rotation", &rotation.x, -360.0f, 360.0f, "%.3f")) {
        textureNode->SetRotation(rotation);
      }
      if (ImGui::SliderFloat("X Scale", &scale.x, 0.1f, 10.0f, "%.3f")) {
        textureNode->SetScale(scale);
      }
      if (ImGui::SliderFloat("Y Scale", &scale.y, 0.1f, 10.0f, "%.3f")) {
        textureNode->SetScale(scale);
      }

      if (ImGui::Button("Reset Position")) {
        position = glm::vec2(0.0f, 0.0f);
        textureNode->SetPosition(position);
      }

      ImGui::SameLine();
      if (ImGui::Button("Reset Rotation")) {
        rotation = glm::vec2(0.0f, 0.0f);
        textureNode->SetRotation(rotation);
      }

      if (ImGui::Button("Reset Scale")) {
        scale = glm::vec2(1.0f, 1.0f);
        textureNode->SetScale(scale);
      }
    } else {
      ImGui::Text("This node is not editable.");
    }
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
  for (size_t i = 0; i < children.size(); ++i) {
    std::string nodeName =
            "Child Node " + std::to_string(i) + " (" + typeid(*children[i]).name() + ")";

    if (ImGui::TreeNode(nodeName.c_str())) {
      if (ImGui::Selectable(nodeName.c_str(), selectedNode == children[i])) {
        if (selectedNode == children[i]) {
          Renderer::GetInstance().GetCurrentScene()->SelectNode(nullptr);
        } else {
          Renderer::GetInstance().GetCurrentScene()->SelectNode(children[i]);
        }
      }

      RenderNode(children[i], selectedNode);

      ImGui::TreePop();
    } else {
      if (ImGui::Selectable(nodeName.c_str(), selectedNode == children[i])) {
        if (selectedNode == children[i]) {
          Renderer::GetInstance().GetCurrentScene()->SelectNode(nullptr);
        } else {
          Renderer::GetInstance().GetCurrentScene()->SelectNode(children[i]);
        }
      }
    }
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

  delete texture;
  delete renderbuffer;
}

#endif