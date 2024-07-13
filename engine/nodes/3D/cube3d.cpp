#include "cube3d.h"

Cube3DNode::Cube3DNode()
    : viewMatrix(), vao(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr),
      shader(nullptr) {}

Cube3DNode::~Cube3DNode() = default;

void Cube3DNode::Initialize() {
  SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

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
  shader->LoadFromFile("application/assets/shaders/vertex_shader.glsl", ShaderType::Vertex);
  shader->LoadFromFile("application/assets/shaders/fragment_shader_cube.glsl", ShaderType::Fragment);
  shader->Link();

  renderPass = RenderPass::CreateRenderPass();
  renderCommandQueue = RenderCommandQueue::CreateRenderCommandQueue();

  vao->Bind();
  vao->AddVertexBuffer(vertexBuffer, 6); // 6 for positions and normals
  vao->SetIndexBuffer(indexBuffer);
  vao->Unbind();

  glEnable(GL_DEPTH_TEST);
}

void Cube3DNode::Update(float deltaTime) {}

void Cube3DNode::Render() {
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

  auto lightPos = Renderer::GetInstance().GetCurrentScene()->GetLightPosition();
  auto cameraPos = Renderer::GetInstance().GetCurrentScene()->GetCameraPosition();

  if (renderCommandQueue) {
    renderCommandQueue->Submit([this]() { vao->Bind(); });

    renderCommandQueue->Submit([this]() { shader->Bind(); });

    renderCommandQueue->Submit([this, fbWidth, fbHeight, lightPos, cameraPos]() {
      float aspectRatio = fbWidth / fbHeight;

      glm::mat4 projection =
              glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);

      shader->SetUniform("projection", projection);
      shader->SetUniform("view", viewMatrix);
      shader->SetUniform("model", glm::mat4(1.0f));
      shader->SetUniform("useSolidColor", true);
      shader->SetUniform("solidColor", solidColor);
      shader->SetUniform("lightColor", lightColor);
      shader->SetUniform("objectColor", objectColor);
      shader->SetUniform("lightPos", lightPos);
      shader->SetUniform("viewPos", cameraPos);
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