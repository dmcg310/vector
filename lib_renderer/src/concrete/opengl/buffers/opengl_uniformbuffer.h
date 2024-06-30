#pragma once

#include "../../../../include/buffer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class OpenGLUniformBuffer : public Buffer {
public:
  void Create(size_t size, const void *data) override;
  void Bind(GLuint bindingPoint) override;
  void SetData(size_t size, const void *data) override;
  [[nodiscard]] unsigned int GetID() const override { return bufferID; }
  ~OpenGLUniformBuffer() override;

private:
  GLuint bufferID;
};
