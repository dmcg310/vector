#include "opengl_indexbuffer.h"

void OpenGLIndexBuffer::Create(size_t size, const void *data) {
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void OpenGLIndexBuffer::Bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void OpenGLIndexBuffer::SetData(size_t size, const void *data) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &bufferID); }
