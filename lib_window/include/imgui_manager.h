#pragma once

#ifdef _DEBUG

#include "../../lib_log/include/log.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_texture.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_framebuffer.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_renderbuffer.h"
#include "../../lib_renderer/include/render_pass.h"
#include "../../lib_renderer/include/texture.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <glm/glm.hpp>

class ImGuiManager {
public:
  static ImGuiManager& GetInstance() {
    static ImGuiManager instance;
    return instance;
  }

  bool Initialize(GLFWwindow *window);
  void ResizeViewport(uint32_t width, uint32_t height);
  void Render();
  void RenderDebugMenu();
  void Shutdown();

  void ToggleDebugMenu() { isDebugMenuOpen = !isDebugMenuOpen; }
  bool IsDebugMenuOpen() const { return isDebugMenuOpen; }
  bool IsInitialized() const { return initialized; }

  OpenGLFramebuffer* framebuffer;

private:
  ImGuiManager() : framebuffer(nullptr), texture(nullptr), renderbuffer(nullptr),
                   initialized(false), isDebugMenuOpen(false) {}
  ~ImGuiManager() {}

  bool initialized;
  bool isDebugMenuOpen;
  glm::vec2 viewportSize;
  Texture* texture;
  Renderbuffer* renderbuffer;
  RenderPass* renderPass;

  // Disable copy constructor and assignment operator
  ImGuiManager(const ImGuiManager&) = delete;
  ImGuiManager& operator=(const ImGuiManager&) = delete;
};

#endif