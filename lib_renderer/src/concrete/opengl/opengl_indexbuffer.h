#pragma once

#include "../../../include/buffer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class OpenGLIndexBuffer : public Buffer {
public:
  void Create(size_t size, const void *data) override;
  void Bind() override;
  void SetData(size_t size, const void *data) override;
  ~OpenGLIndexBuffer() override;

private:
  GLuint bufferID;
};
