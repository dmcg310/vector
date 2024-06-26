#pragma once

#ifdef _DEBUG

#include "../../lib_application/nodes/2D/node2d.h"
#include "../../lib_application/nodes/2D/texture2d.h"
#include "../../lib_log/include/log.h"
#include "../../lib_renderer/include/framebuffer.h"
#include "../../lib_renderer/include/render_api_factory.h"
#include "../../lib_renderer/include/render_pass.h"
#include "../../lib_renderer/include/renderbuffer.h"
#include "../../lib_renderer/include/texture.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

class ImGuiManager {
public:
  static ImGuiManager &GetInstance() {
    static ImGuiManager instance;
    return instance;
  }

  bool Initialize(GLFWwindow *window, API api);
  void ResizeViewport(uint32_t width, uint32_t height);
  void Render();
  void RenderDebugMenu();
  void RenderNode(const std::shared_ptr<SceneNode> &node,
                  const std::shared_ptr<SceneNode> &selectedNode);
  void Shutdown();

  void ToggleDebugMenu() { isDebugMenuOpen = !isDebugMenuOpen; }
  [[nodiscard]] bool IsDebugMenuOpen() const { return isDebugMenuOpen; }
  [[nodiscard]] bool IsInitialized() const { return initialized; }
  [[nodiscard]] glm::vec2 GetViewportSize() const { return viewportSize; }
  [[nodiscard]] std::shared_ptr<Framebuffer> GetFramebuffer() const {
    return framebuffer;
  }

private:
  ImGuiManager()
      : framebuffer(nullptr), texture(nullptr), renderbuffer(nullptr),
        isDebugMenuOpen(false), renderAPI(API::None) {}
  ~ImGuiManager() {}

  bool initialized;
  bool isDebugMenuOpen;
  glm::vec2 viewportSize;
  std::shared_ptr<Texture> texture;
  std::shared_ptr<Framebuffer> framebuffer;
  std::shared_ptr<Renderbuffer> renderbuffer;
  API renderAPI;

  bool Setup(GLFWwindow *window);

  // Disable copy constructor and assignment operator
  ImGuiManager(const ImGuiManager &) = delete;
  ImGuiManager &operator=(const ImGuiManager &) = delete;
};

#endif