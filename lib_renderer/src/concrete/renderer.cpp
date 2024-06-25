#include "renderer.h"
#include "../../lib_log/include/log.h"
#include "../../lib_renderer/include/render_api_factory.h"
#include "../../lib_window/include/window.h"

Renderer::Renderer()
    : context(nullptr)
#ifdef _DEBUG
      ,
      imGuiManager(ImGuiManager::GetInstance())
#endif
{
}

Renderer::~Renderer() { Shutdown(); }

void Renderer::Initialize(GLFWwindow *window) {
  API selectedAPI = API::OpenGL;
  context = RenderAPIFactory::CreateContext(selectedAPI, window);

  try {
    context->Initialize();
  } catch (const std::runtime_error &e) {
    Log::Write(Log::FATAL, e.what());
    throw;
  }

#ifdef _DEBUG
  if (!imGuiManager.Initialize(window)) {
    Log::Write(Log::FATAL, "Failed to initialize ImGuiManager");
    throw std::runtime_error("Failed to initialize ImGuiManager");
  }
#endif
}

SceneManager &Renderer::GetSceneManager() { return sceneManager; }

void Renderer::Update(float deltaTime) {
  sceneManager.Update(deltaTime);

#ifdef _DEBUG
  if (imGuiManager.IsDebugMenuOpen()) { imGuiManager.Render(); }
#endif
}

void Renderer::Render() {
  sceneManager.Render();

#ifdef _DEBUG
  if (imGuiManager.IsDebugMenuOpen()) { imGuiManager.Render(); }
#endif

  context->SwapBuffers();
}

void Renderer::Shutdown() {
  if (context) {
    sceneManager.Shutdown();

#ifdef _DEBUG
    imGuiManager.Shutdown();
#endif

    delete context;
    context = nullptr;
  }
}