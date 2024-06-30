#include "lighting_cube3d.h"

LightingCubeNode::LightingCubeNode()
    : viewMatrix(), vao(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr),
      shader(nullptr) {}

LightingCubeNode::~LightingCubeNode() = default;

void LightingCubeNode::Initialize() {
  SetScale(glm::vec3(0.2f, 0.2f, 0.2f)); // Smaller scale for the light cube

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

  auto scaleX = GetScale().x;
  auto scaleY = GetScale().y;
  auto scaleZ = GetScale().z;

  for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 3) {
    vertices[i] *= scaleX;
    vertices[i + 1] *= scaleY;
    vertices[i + 2] *= scaleZ;
  }

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
  shader->LoadFromFile("assets/shaders/fragment_shader_light.glsl", ShaderType::Fragment);
  shader->Link();

  renderPass = RenderPass::CreateRenderPass();
  renderCommandQueue = RenderCommandQueue::CreateRenderCommandQueue();

  vao->Bind();
  vao->AddVertexBuffer(vertexBuffer, 3);
  vao->SetIndexBuffer(indexBuffer);
  vao->Unbind();

  glEnable(GL_DEPTH_TEST);
}

void LightingCubeNode::Update(float deltaTime) {}

void LightingCubeNode::Render() {
  bool isDebugMenuOpen = false;

#ifdef _DEBUG
  auto const &imGuiManager = ImGuiManager::GetInstance();
  isDebugMenuOpen = imGuiManager.IsDebugMenuOpen();
#endif

  float fbWidth;
  float fbHeight;
  auto framebuffer = Framebuffer::CreateFramebuffer();
  Renderer::GetInstance().SetupFramebuffer(framebuffer, fbWidth, fbHeight);

  renderPass->SetViewportSize(fbWidth, fbHeight);

  if (renderCommandQueue) {
    renderCommandQueue->Submit([this]() { vao->Bind(); });

    renderCommandQueue->Submit([this]() { shader->Bind(); });

    renderCommandQueue->Submit([this, fbWidth, fbHeight]() {
      float aspectRatio = fbWidth / fbHeight;

      glm::mat4 projection =
              glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
      glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 1.0f, -2.0f));

      shader->SetUniform("projection", projection);
      shader->SetUniform("view", viewMatrix);
      shader->SetUniform("model", model);
      shader->SetUniform("lightColor", lightColor);
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