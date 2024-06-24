#pragma once

#include "../../lib_log/include/log.h"
#include "../../lib_renderer/include/context.h"
#include "../../lib_renderer/include/render_api_factory.h"
#include "../../lib_window/include/window.h"
#include "settings.h"
#include <chrono>

// TESTING PURPOSES
#include "../../lib_renderer/include/buffer.h"
#include "../../lib_renderer/include/shader.h"
#include "../../lib_renderer/include/texture.h"
#include "../../lib_renderer/src/concrete/opengl/buffers/opengl_indexbuffer.h"
#include "../../lib_renderer/src/concrete/opengl/buffers/opengl_vertexbuffer.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_shader.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_texture.h"

#ifdef _DEBUG
#include "imgui_manager.h"
#endif

class Application : public IEventObserver {
public:
  Application();

  bool Initialize();
  void Run();
  void Shutdown();

  void OnKeyPress(int key) override;
  void OnKeyRelease(int key) override;
  void OnMouseMove(double x, double y) override;
  void OnMouseClick(int button) override;

private:
  void HandleEvents();
  void Update();
  void Render();

  void MainLoopBody();

#ifdef _DEBUG
  ImGuiManager imguiManager;
  bool isDebugMenuOpen = false;
  std::chrono::steady_clock::time_point startTime;
  unsigned int frameCount;
  double accumulatedTime;
  double accumulatedFPS;
  unsigned int sampleFrames;
  unsigned int frameSamples;
#endif

  ApplicationSettings settings;
  Context *context;

  // TESTING CODE BELOW
  GLuint vao; // Can change after we get VAO API
  Buffer *buffer;
  Buffer *indexBuffer;
  Texture *texture;
  Shader *shader;
};