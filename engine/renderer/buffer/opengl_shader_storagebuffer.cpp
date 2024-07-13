#include "opengl_shader_storagebuffer.h"
#include "../../../../../lib_log/include/log.h"

void OpenGLShaderStorageBuffer::Create(size_t size, const void *data) {
  glGenBuffers(1, &bufferID);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error creating shader storage buffer: " + std::to_string(error));
  }
}

void OpenGLShaderStorageBuffer::Bind(uint32_t bindingPoint) {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding shader storage buffer: " + std::to_string(error));
  }
}

void OpenGLShaderStorageBuffer::SetData(size_t size, const void *data) {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error setting data for shader storage buffer: " + std::to_string(error));
  }
}

void OpenGLShaderStorageBuffer::BindBase(unsigned int bindingPoint) {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, bufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding base for shader storage buffer: " + std::to_string(error));
  }
}

OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer() {
  glDeleteBuffers(1, &bufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting shader storage buffer: " + std::to_string(error));
  }
}
