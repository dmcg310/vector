#include "texture2d.h"

Texture2DNode::Texture2DNode()
    : vao(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr), texture(nullptr),
      shader(nullptr) {}

Texture2DNode::~Texture2DNode() = default;

void Texture2DNode::Initialize(const std::string &textureFile) {
  float vertices[] = {
          // positions         // texture coords
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left
          0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
          0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top-right
          -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top-left
  };

  int indices[] = {
          0, 1, 2, // first triangle
          2, 3, 0  // second triangle
  };

  vao = VertexArray::CreateVertexArray();
  vao->Create();

  vertexBuffer = Buffer::CreateBuffer(BufferType::Vertex);
  vertexBuffer->Create(sizeof(vertices), vertices);
  vao->AddVertexBuffer(vertexBuffer, 5);

  indexBuffer = Buffer::CreateBuffer(BufferType::Index);
  indexBuffer->Create(sizeof(indices), indices);
  vao->SetIndexBuffer(indexBuffer);

  shader = Shader::CreateShader();
  shader->LoadFromFile("application/assets/shaders/vertex_shader.glsl", ShaderType::Vertex);
  shader->LoadFromFile("application/assets/shaders/fragment_shader.glsl", ShaderType::Fragment);
  shader->Link();

  texture = Texture::CreateTexture();
  texture->LoadFromFile(textureFile);
  texture->SetParameters();

  renderPass = RenderPass::CreateRenderPass();
  renderPass->SetClearColor(0.8f, 0.3f, 0.3f, 1.0f);

  renderCommandQueue = RenderCommandQueue::CreateRenderCommandQueue();
}

void Texture2DNode::Update(float deltaTime) {
  // Update logic here if necessary
}

void Texture2DNode::Render() {
  bool isDebugMenuOpen = false;

#ifdef _DEBUG
  auto const &imGuiManager = ImGuiManager::GetInstance();
  isDebugMenuOpen = imGuiManager.IsDebugMenuOpen();
#endif

  std::shared_ptr<Framebuffer> framebuffer;

  float fbWidth;
  float fbHeight;

  if (isDebugMenuOpen) {
#ifdef _DEBUG
    if (!imGuiManager.IsInitialized()) {
      Log::Write(Log::ERROR, "ImGuiManager is not initialized");
      return;
    }

    framebuffer = imGuiManager.GetFramebuffer();
    framebuffer->Bind();

    fbWidth = imGuiManager.GetViewportSize().x;
    fbHeight = imGuiManager.GetViewportSize().y;
#endif
  } else {
    framebuffer = Framebuffer::CreateFramebuffer();
    framebuffer->Unbind();

    auto framebufferSize = framebuffer->GetSize(Window::GetGLFWWindow());
    fbWidth = framebufferSize.x;
    fbHeight = framebufferSize.y;
  }

  renderPass->SetViewportSize(fbWidth, fbHeight);
  renderPass->SetClearColor(0.8f, 0.3f, 0.3f, 1.0f);
  renderPass->Begin();

  if (renderCommandQueue) {
    renderCommandQueue->Submit([this]() { vao->Bind(); });

    renderCommandQueue->Submit([this, fbWidth, fbHeight]() {
      auto model = GetModelMatrix(renderPass->GetModelMatrix());
      auto view = glm::mat4(1.0f);
      auto projection = glm::ortho(0.0f, fbWidth, 0.0f, fbHeight);

      shader->Bind();
      shader->SetUniform("model", model);
      shader->SetUniform("view", view);
      shader->SetUniform("projection", projection);
      shader->SetUniform("useSolidColor", false);
    });

    renderCommandQueue->Submit([this]() { texture->Bind(0); });

    renderCommandQueue->Submit([this]() {
      renderCommand = RenderCommand::CreateDrawElementsCommand();
      renderCommand->SetDrawElementsParams(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      renderCommand->Execute();
    });

    renderCommandQueue->Execute();
  }

#ifdef _DEBUG
  if (isDebugMenuOpen) { ImGuiManager::GetInstance().Render(); }
#endif
}