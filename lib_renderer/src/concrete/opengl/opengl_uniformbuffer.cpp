#include "opengl_uniformbuffer.h"
#include "../../../../lib_log/include/log.h"

void OpenGLUniformBuffer::Create(size_t size, const void *data) {
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
  glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error creating uniform buffer: " + std::to_string(error));
  }
}

void OpenGLUniformBuffer::Bind(GLuint bindingPoint) {
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding uniform buffer to binding point " + std::to_string(bindingPoint) + ": " + std::to_string(error));
  }
}

void OpenGLUniformBuffer::SetData(size_t size, const void *data) {
  glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error setting data for uniform buffer: " + std::to_string(error));
  }
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() { 
  glDeleteBuffers(1, &bufferID); 

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting uniform buffer: " + std::to_string(error));
  }
}
