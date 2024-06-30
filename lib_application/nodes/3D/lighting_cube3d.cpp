#include "lighting_cube3d.h"

LightingCubeNode::LightingCubeNode()
    : viewMatrix(), vao(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr),
      shader(nullptr) {}

LightingCubeNode::~LightingCubeNode() = default;

void LightingCubeNode::Initialize() {
  SetLightPosition(glm::vec3(1.2f, 1.0f, 2.0f));
  SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

  float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f,
                      -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,
                      0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,

                      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
                      0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
                      0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,
                      -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
                      0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,

                      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,
                      -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
                      0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,

                      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f,
                      -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
                      -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,

                      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
                      -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
                      1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f};

  unsigned int indices[] = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
                            8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
                            16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

  auto scaleX = GetScale().x;
  auto scaleY = GetScale().y;
  auto scaleZ = GetScale().z;

  for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 6) {
    vertices[i] *= scaleX;
    vertices[i + 1] *= scaleY;
    vertices[i + 2] *= scaleZ;
  }

  vao = VertexArray::CreateVertexArray();
  vao->Create();

  vertexBuffer = Buffer::CreateBuffer(BufferType::Vertex);
  vertexBuffer->Create(sizeof(vertices), vertices);
  vao->AddVertexBuffer(vertexBuffer, 6);

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
  vao->AddVertexBuffer(vertexBuffer, 6);
  vao->SetIndexBuffer(indexBuffer);
  vao->Unbind();

  glEnable(GL_DEPTH_TEST);
}

void LightingCubeNode::Update(float deltaTime) {
  float radius = 2.0f;
  angle += deltaTime;
  Renderer::GetInstance().GetCurrentScene()->SetLightPosition(
          glm::vec3(radius * cos(angle), 1.0f, radius * sin(angle)));
}

void LightingCubeNode::Render() {
#ifdef _DEBUG
  bool isDebugMenuOpen = false;
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
      glm::mat4 model = glm::translate(glm::mat4(1.0f), GetLightPosition());

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