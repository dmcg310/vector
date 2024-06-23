#include "opengl_uniformbuffer.h"

void OpenGLUniformBuffer::Create(size_t size, const void *data) {
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
  glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
}

void OpenGLUniformBuffer::Bind(GLuint bindingPoint) {
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferID);
}

void OpenGLUniformBuffer::SetData(size_t size, const void *data) {
  glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() { glDeleteBuffers(1, &bufferID); }
