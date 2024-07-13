#pragma once

#ifdef _DEBUG

#include "../../../nodes/2D/node2d.h"
#include "../../../nodes/2D/texture2d.h"
#include "../../log/log.h"
#include "../../../renderer/framebuffer/framebuffer.h"
#include "../../../renderer/render_api_factory.h"
#include "../../../renderer/render_pass/render_pass.h"
#include "../../../renderer/renderbuffer/renderbuffer.h"
#include "../../../renderer/texture/texture.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

  void Refresh();

  void ToggleDebugMenu();
  [[nodiscard]] bool IsDebugMenuOpen() const { return isDebugMenuOpen; }
  [[nodiscard]] bool IsInitialized() const { return initialized; }
  [[nodiscard]] glm::vec2 GetViewportSize() const { return viewportSize; }
  [[nodiscard]] std::shared_ptr<Framebuffer> GetFramebuffer() const {
    return framebuffer;
  }

private:
  ImGuiManager()
      : framebuffer(nullptr), texture(nullptr), renderbuffer(nullptr),
        isDebugMenuOpen(true), renderAPI(API::None) {}
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