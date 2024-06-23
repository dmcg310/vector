#include "opengl_vertexbuffer.h"
#include "../../../../../lib_log/include/log.h"

void OpenGLVertexBuffer::Create(size_t size, const void *data) {
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_ARRAY_BUFFER, bufferID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error creating vertex buffer: " + std::to_string(error));
  }
}

void OpenGLVertexBuffer::Bind(uint32_t bindingPoint) { 
  glBindBuffer(GL_ARRAY_BUFFER, bufferID); 

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding vertex buffer: " + std::to_string(error));
  }
}

void OpenGLVertexBuffer::SetData(size_t size, const void *data) {
  glBindBuffer(GL_ARRAY_BUFFER, bufferID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error setting data for vertex buffer: " + std::to_string(error));
  }
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { 
  glDeleteBuffers(1, &bufferID); 

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting vertex buffer: " + std::to_string(error));
  }
}
