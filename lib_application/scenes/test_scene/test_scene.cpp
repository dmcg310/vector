#include "test_scene.h"
#include "../../lib_log/include/log.h"

void TestScene::Initialize() {
  float vertices[] = {
          // positions        // texture coords
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left
          0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
          0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top-right
          -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top-left
  };

  unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

  vao = new OpenGLVertexArray();
  vao->Create();

  buffer = new OpenGLVertexBuffer();
  buffer->Create(sizeof(vertices), vertices);
  vao->AddVertexBuffer(buffer);

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
}

void TestScene::Update(float deltaTime) {
  // Update logic here if necessary
}

void TestScene::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (vao) {
    vao->Bind();
  }

  if (shader) {
    shader->Bind();
  }

  if (texture) {
    texture->Bind(0);
  }

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  renderPass->End();
}

void TestScene::Shutdown() {
  if (vao) {
    delete vao;
    vao = nullptr;
  }

  if (buffer) {
    delete buffer;
    buffer = nullptr;
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
}