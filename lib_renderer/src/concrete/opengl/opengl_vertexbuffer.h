#pragma once

#include "../../../include/buffer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLVertexBuffer : public Buffer {
public:
  void Create(size_t size, const void* data) override;
  void Bind() override;
  void SetData(size_t size, const void* data) override;
  ~OpenGLVertexBuffer() override;
private:
  GLuint bufferID;
};
