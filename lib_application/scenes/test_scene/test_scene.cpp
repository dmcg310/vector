#include "test_scene.h"
#include "../../lib_log/include/log.h"

void TestScene::Initialize() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  buffer = new OpenGLVertexBuffer();

  float vertices[] = {
          // positions        // texture coords
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left
          0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
          0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top-right
          -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top-left
  };

  buffer->Create(sizeof(vertices), vertices);
  buffer->Bind(0);

  // Position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

  // Texture coordinate attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

  indexBuffer = new OpenGLIndexBuffer();
  indexBuffer->Create(sizeof(indices), indices);

  texture = new OpenGLTexture();
  texture->LoadFromFile("assets/textures/container.jpg");
  texture->SetParameters();

  shader = new OpenGLShader();
  shader->LoadFromFile("assets/shaders/vertex_shader.glsl", ShaderType::Vertex);
  shader->LoadFromFile("assets/shaders/fragment_shader.glsl", ShaderType::Fragment);
  shader->Link();
}

void TestScene::Update(float deltaTime) {
  // Update logic here if necessary
}

void TestScene::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(vao);

  if (shader) {
    shader->Bind();
  }

  if (texture) {
    texture->Bind(0);
  }

  if (buffer) {
    buffer->Bind(0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  }

  if (indexBuffer) {
    indexBuffer->Bind(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  glBindVertexArray(0);
}

void TestScene::Shutdown() {
  if (vao) {
    glDeleteVertexArrays(1, &vao);
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
}