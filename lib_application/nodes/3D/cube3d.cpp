#include "cube3d.h"

Cube3DNode::Cube3DNode()
    : vao(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr), shader(nullptr),
      viewMatrix() {}

Cube3DNode::~Cube3DNode() = default;

void Cube3DNode::Initialize() {
  float vertices[] = {
          -1.0f, -1.0f, 1.0f,  // Front-bottom-left
          1.0f,  -1.0f, 1.0f,  // Front-bottom-right
          1.0f,  1.0f,  1.0f,  // Front-top-right
          -1.0f, 1.0f,  1.0f,  // Front-top-left
          -1.0f, -1.0f, -1.0f, // Back-bottom-left
          1.0f,  -1.0f, -1.0f, // Back-bottom-right
          1.0f,  1.0f,  -1.0f, // Back-top-right
          -1.0f, 1.0f,  -1.0f  // Back-top-left
  };

  int indices[] = {
          0, 1, 2, 2, 3, 0, // Front face
          1, 5, 6, 6, 2, 1, // Right face
          7, 6, 5, 5, 4, 7, // Back face
          4, 0, 3, 3, 7, 4, // Left face
          4, 5, 1, 1, 0, 4, // Bottom face
          3, 2, 6, 6, 7, 3  // Top face
  };

  vao = VertexArray::CreateVertexArray();
  vao->Create();

  vertexBuffer = Buffer::CreateBuffer(BufferType::Vertex);
  vertexBuffer->Create(sizeof(vertices), vertices);
  vao->AddVertexBuffer(vertexBuffer, 3);

  indexBuffer = Buffer::CreateBuffer(BufferType::Index);
  indexBuffer->Create(sizeof(indices), indices);
  vao->SetIndexBuffer(indexBuffer);

  shader = Shader::CreateShader();
  shader->LoadFromFile("assets/shaders/vertex_shader.glsl", ShaderType::Vertex);
  shader->LoadFromFile("assets/shaders/fragment_shader.glsl", ShaderType::Fragment);
  shader->Link();

  renderPass = RenderPass::CreateRenderPass();
  renderPass->SetClearColor(0.8f, 0.3f, 0.3f, 1.0f);

  renderCommandQueue = RenderCommandQueue::CreateRenderCommandQueue();

  vao->Bind();
  vao->AddVertexBuffer(vertexBuffer, 3);
  vao->SetIndexBuffer(indexBuffer);

  vao->Unbind();

  glEnable(GL_DEPTH_TEST);
}

void Cube3DNode::Update(float deltaTime) {}

void Cube3DNode::Render() {
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

    renderCommandQueue->Submit([this]() { shader->Bind(); });

    renderCommandQueue->Submit([this, fbWidth, fbHeight]() {
      float aspectRatio = fbWidth / fbHeight;

      glm::mat4 projection =
              glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

      shader->SetUniform("projection", projection);
      shader->SetUniform("view", viewMatrix);

      shader->SetUniform("model", glm::mat4(1.0f));
      shader->SetUniform("useSolidColor", true);
      shader->SetUniform("solidColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    });

    renderCommandQueue->Submit([this]() {
      renderCommand = RenderCommand::CreateDrawElementsCommand();
      renderCommand->SetDrawElementsParams(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
      renderCommand->Execute();
    });

    renderCommandQueue->Submit([this]() { vao->Unbind(); });

    renderCommandQueue->Execute();
  }

#ifdef _DEBUG
  if (isDebugMenuOpen) { ImGuiManager::GetInstance().Render(); }
#endif
}