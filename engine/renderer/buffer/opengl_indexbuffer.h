#pragma once

#include "buffer.h"
#include "../opengl_includes.h"

class OpenGLIndexBuffer : public Buffer {
public:
  void Create(size_t size, const void *data) override;
  void Bind(uint32_t bindingPoint) override;
  void SetData(size_t size, const void *data) override;
  [[nodiscard]] unsigned int GetID() const override { return bufferID; }
  ~OpenGLIndexBuffer() override;

private:
  GLuint bufferID;
};
