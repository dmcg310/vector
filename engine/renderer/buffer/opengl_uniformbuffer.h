#pragma once

#include "buffer.h"
#include "../opengl_includes.h"

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
