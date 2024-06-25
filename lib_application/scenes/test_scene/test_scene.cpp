#include "test_scene.h"
#include "../../../lib_window/include/window.h"

void TestScene::Initialize() {
  float vertices[] = {
          // positions        // texture coords
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left
          0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
          0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top-right
          -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top-left
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  vao = std::make_shared<OpenGLVertexArray>();
  vao->Create();

  vertexBuffer = Buffer::CreateBuffer(BufferType::Vertex);
  vertexBuffer->Create(sizeof(vertices), vertices);
  vao->AddVertexBuffer(vertexBuffer);

  indexBuffer = Buffer::CreateBuffer(BufferType::Index);
  indexBuffer->Create(sizeof(indices), indices);
  vao->SetIndexBuffer(indexBuffer);

  shader = Shader::CreateShader();
  shader->LoadFromFile("assets/shaders/vertex_shader.glsl", ShaderType::Vertex);
  shader->LoadFromFile("assets/shaders/fragment_shader.glsl", ShaderType::Fragment);
  shader->Link();

  texture = Texture::CreateTexture();
  texture->LoadFromFile("assets/textures/container.jpg");
  texture->SetParameters();

  renderPass = RenderPass::CreateRenderPass();
  renderPass->SetClearColor(0.8f, 0.3f, 0.3f, 1.0f);

  renderCommandQueue = RenderCommandQueue::CreateRenderCommandQueue();
}

void TestScene::Update(float deltaTime) {
  // Update logic here if necessary
}

void TestScene::Render() {
  auto &imGuiManager = ImGuiManager::GetInstance();
  bool isDebugMenuOpen = imGuiManager.IsDebugMenuOpen();

  if (isDebugMenuOpen) {
    if (!imGuiManager.IsInitialized()) {
      Log::Write(Log::ERROR, "ImGuiManager is not initialized");
      return;
    }

    // Bind the framebuffer to render the scene
    Framebuffer *framebuffer = imGuiManager.GetFramebuffer();
    framebuffer->Bind();

    int fbWidth = static_cast<int>(imGuiManager.GetViewportSize().x);
    int fbHeight = static_cast<int>(imGuiManager.GetViewportSize().y);

    glViewport(0, 0, fbWidth, fbHeight);

    renderPass->SetClearColor(0.8f, 0.3f, 0.3f, 1.0f);
    renderPass->Begin();

    if (renderCommandQueue) {
      renderCommandQueue->Submit([this]() { vao->Bind(); });

      renderCommandQueue->Submit([this, fbWidth, fbHeight]() {
        renderPass->SetViewportSize(fbWidth, fbHeight);

        shader->Bind();
        shader->SetUniform("u_Projection", renderPass->GetProjectionMatrix());
        shader->SetUniform("u_Model", renderPass->GetModelMatrix());
      });

      renderCommandQueue->Submit([this]() { texture->Bind(0); });

      renderCommandQueue->Submit(
              []() { glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); });

      renderCommandQueue->Execute();
    }

    renderPass->End();

    ImGuiManager::GetInstance().Render();
  } else {
    // Bind the default framebuffer to render the scene directly to the window
    Framebuffer *framebuffer = new OpenGLFramebuffer();
    framebuffer->Unbind();

    int windowWidth, windowHeight;
    glfwGetFramebufferSize(Window::GetGLFWWindow(), &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    renderPass->SetClearColor(0.8f, 0.3f, 0.3f, 1.0f);
    renderPass->Begin();

    if (renderCommandQueue) {
      renderCommandQueue->Submit([this]() { vao->Bind(); });

      renderCommandQueue->Submit([this, windowWidth, windowHeight]() {
        renderPass->SetViewportSize(windowWidth, windowHeight);

        shader->Bind();
        shader->SetUniform("u_Projection", renderPass->GetProjectionMatrix());
        shader->SetUniform("u_Model", renderPass->GetModelMatrix());
      });

      renderCommandQueue->Submit([this]() { texture->Bind(0); });

      renderCommandQueue->Submit(
              []() { glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); });

      renderCommandQueue->Execute();
    }

    renderPass->End();
  }
}

void TestScene::Shutdown() {
  if (vao) { vao = nullptr; }

  if (vertexBuffer) { vertexBuffer = nullptr; }

  if (indexBuffer) { indexBuffer = nullptr; }

  if (texture) { texture = nullptr; }

  if (shader) { shader = nullptr; }
}