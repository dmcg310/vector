#include "renderer.h"
#include "../../lib_application/scenes/test_scene/test_scene.h"
#include "../../lib_log/include/log.h"
#include "../../lib_window/include/window.h"
#include "render_api_factory.h"

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

  // Maybe we can make any scene not just TestScene
  auto testScene = std::make_shared<TestScene>();
  sceneManager.AddScene("TestScene", testScene);
  sceneManager.SetActiveScene("TestScene");

#ifdef _DEBUG
  if (!imGuiManager.Initialize(window, selectedAPI)) {
    Log::Write(Log::FATAL, "Failed to initialize ImGuiManager");
  }
#endif
}

SceneNode &Renderer::GetNode() {
  return reinterpret_cast<SceneNode &>(sceneManager.GetActiveScene()->GetNode());
}

SceneManager &Renderer::GetSceneManager() { return sceneManager; }

std::shared_ptr<Scene> Renderer::GetCurrentScene() {
  return sceneManager.GetActiveScene();
}

std::shared_ptr<SceneNode> Renderer::GetSelectedNode() {
  return GetCurrentScene()->GetSelectedNode();
}

void Renderer::Update(float deltaTime) {
  sceneManager.Update(deltaTime);

#ifdef _DEBUG
  if (imGuiManager.IsDebugMenuOpen()) { imGuiManager.Render(); }
#endif
}

void Renderer::Render() {
  auto renderPass = RenderPass::CreateRenderPass();

  float fbWidth;
  float fbHeight;
  auto framebuffer = Framebuffer::CreateFramebuffer();
  SetupFramebuffer(framebuffer, fbWidth, fbHeight);

  renderPass->SetViewportSize(fbWidth, fbHeight);
  renderPass->SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  renderPass->Begin();

  sceneManager.Render();

#ifdef _DEBUG
  if (imGuiManager.IsDebugMenuOpen()) {
    framebuffer->Unbind();
    imGuiManager.Render();
  }
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

void Renderer::SetupFramebuffer(std::shared_ptr<Framebuffer> &framebuffer, float &fbWidth,
                                float &fbHeight) {
#ifdef _DEBUG
  if (imGuiManager.IsDebugMenuOpen()) {
    framebuffer = imGuiManager.GetFramebuffer();
    framebuffer->Bind();

    fbWidth = imGuiManager.GetViewportSize().x;
    fbHeight = imGuiManager.GetViewportSize().y;
  } else {
    framebuffer = Framebuffer::CreateFramebuffer();
    framebuffer->Unbind();

    auto framebufferSize = framebuffer->GetSize(Window::GetGLFWWindow());
    fbWidth = framebufferSize.x;
    fbHeight = framebufferSize.y;
  }
#else
  framebuffer = Framebuffer::CreateFramebuffer();
  framebuffer->Unbind();

  auto framebufferSize = framebuffer->GetSize(Window::GetGLFWWindow());
  fbWidth = framebufferSize.x;
  fbHeight = framebufferSize.y;
#endif
}

void Renderer::SetViewMatrixAndZoom(const glm::mat4 &viewMatrix, float zoom) {
  this->viewMatrix = viewMatrix;
  this->zoom = zoom;

  GetCurrentScene()->SetViewMatrixAndZoom(viewMatrix, zoom);
}