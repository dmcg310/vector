#pragma once

#include "../../../../include/buffer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class OpenGLShaderStorageBuffer : public Buffer {
public:
  void Create(size_t size, const void *data) override;
  void Bind(uint32_t bindingPoint) override;
  void SetData(size_t size, const void *data) override;
  void BindBase(unsigned int bindingPoint);
  [[nodiscard]] unsigned int GetID() const override { return bufferID; }
  ~OpenGLShaderStorageBuffer() override;

private:
  GLuint bufferID;
};
