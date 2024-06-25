#pragma once

#ifdef _DEBUG

#include "../../lib_log/include/log.h"
#include "../../lib_renderer/include/framebuffer.h"
#include "../../lib_renderer/include/render_api_factory.h"
#include "../../lib_renderer/include/render_pass.h"
#include "../../lib_renderer/include/renderbuffer.h"
#include "../../lib_renderer/include/texture.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_framebuffer.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_render_pass.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_renderbuffer.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_texture.h"
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
  void Shutdown();

  void ToggleDebugMenu() { isDebugMenuOpen = !isDebugMenuOpen; }
  bool IsDebugMenuOpen() const { return isDebugMenuOpen; }
  bool IsInitialized() const { return initialized; }
  glm::vec2 GetViewportSize() const { return viewportSize; }
  Framebuffer *GetFramebuffer() const { return framebuffer; }

private:
  ImGuiManager()
      : framebuffer(nullptr), texture(nullptr), renderbuffer(nullptr), initialized(false),
        isDebugMenuOpen(false), renderAPI(API::None) {}
  ~ImGuiManager() {}

  bool initialized;
  bool isDebugMenuOpen;
  glm::vec2 viewportSize;
  Texture *texture;
  Framebuffer *framebuffer;
  Renderbuffer *renderbuffer;
  API renderAPI;

  bool Setup(GLFWwindow *window);

  // Disable copy constructor and assignment operator
  ImGuiManager(const ImGuiManager &) = delete;
  ImGuiManager &operator=(const ImGuiManager &) = delete;
};

#endif