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

  vao = new OpenGLVertexArray();
  vao->Create();

  vertexBuffer = new OpenGLVertexBuffer();
  vertexBuffer->Create(sizeof(vertices), vertices);
  vao->AddVertexBuffer(vertexBuffer);

  indexBuffer = new OpenGLIndexBuffer();
  indexBuffer->Create(sizeof(indices), indices);
  vao->SetIndexBuffer(indexBuffer);

  texture = new OpenGLTexture();
  texture->LoadFromFile("assets/textures/container.jpg");
  texture->SetParameters();

  shader = new OpenGLShader();
  shader->LoadFromFile("assets/shaders/vertex_shader.glsl", ShaderType::Vertex);
  shader->LoadFromFile("assets/shaders/fragment_shader.glsl", ShaderType::Fragment);
  shader->Link();

  renderPass = new OpenGLRenderPass();
  renderPass->SetClearColor(0.8f, 0.3f, 0.3f, 1.0f);

  renderCommandQueue = new OpenGLRenderCommandQueue();
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
    imGuiManager.framebuffer->Bind();

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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
  if (vao) {
    delete vao;
    vao = nullptr;
  }

  if (vertexBuffer) {
    delete vertexBuffer;
    vertexBuffer = nullptr;
  }

  if (indexBuffer) {
    delete indexBuffer;
    indexBuffer = nullptr;
  }

  if (texture) {
    delete texture;
    texture = nullptr;
  }

  if (shader) {
    delete shader;
    shader = nullptr;
  }

  if (renderPass) {
    delete renderPass;
    renderPass = nullptr;
  }

  if (renderCommandQueue) {
    delete renderCommandQueue;
    renderCommandQueue = nullptr;
  }
}