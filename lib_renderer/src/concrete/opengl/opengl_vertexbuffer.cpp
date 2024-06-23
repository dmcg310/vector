#include "opengl_vertexbuffer.h"

void OpenGLVertexBuffer::Create(size_t size, const void *data) {
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_ARRAY_BUFFER, bufferID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void OpenGLVertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }

void OpenGLVertexBuffer::SetData(size_t size, const void *data) {
  glBindBuffer(GL_ARRAY_BUFFER, bufferID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &bufferID); }
