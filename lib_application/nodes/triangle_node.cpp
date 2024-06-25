#include "triangle_node.h"

TriangleNode::TriangleNode()
    : vao(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr), shader(nullptr),
      texture(nullptr) {}

TriangleNode::~TriangleNode() {}

void TriangleNode::Initialize() {
  float vertices[] = {
          // positions        // texture coords
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left
          0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
          0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top-right
          -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top-left
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  vao = VertexArray::CreateVertexArray();
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
}

void TriangleNode::Update(float deltaTime) {
  // Update logic here if necessary
}

void TriangleNode::Render() {
  shader->Bind();
  texture->Bind(0);
  vao->Bind();

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}