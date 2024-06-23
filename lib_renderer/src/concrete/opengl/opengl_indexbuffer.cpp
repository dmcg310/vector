#include "opengl_indexbuffer.h"
#include "../../../../lib_log/include/log.h"

void OpenGLIndexBuffer::Create(size_t size, const void *data) {
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error creating index buffer: " + std::to_string(error));
  }
}

void OpenGLIndexBuffer::Bind(uint32_t bindingPoint) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding index buffer: " + std::to_string(error));
  }
}

void OpenGLIndexBuffer::SetData(size_t size, const void *data) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error setting data for index buffer: " + std::to_string(error));
  }
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { 
  glDeleteBuffers(1, &bufferID); 

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting index buffer: " + std::to_string(error));
  }
}
