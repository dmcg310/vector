#pragma once

#include "../../../../include/buffer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class OpenGLVertexBuffer : public Buffer {
public:
  void Create(size_t size, const void *data) override;
  void Bind(uint32_t bindingPoint) override;
  void SetData(size_t size, const void *data) override;
  [[nodiscard]] unsigned int GetID() const override { return bufferID; }
  ~OpenGLVertexBuffer() override;

private:
  GLuint bufferID;
};
