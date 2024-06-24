#include "opengl_renderbuffer.h"
#include "../../../../lib_log/include/log.h"

void OpenGLRenderbuffer::Create() {
  glGenRenderbuffers(1, &renderbufferID);
  glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error creating renderbuffer: " + std::to_string(error));
  }
}

void OpenGLRenderbuffer::Bind() {
  glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding renderbuffer: " + std::to_string(error));
  }
}

void OpenGLRenderbuffer::SetStorage(GLenum internalFormat, int width, int height) {
  glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error setting renderbuffer storage: " + std::to_string(error));
  }
}

unsigned int OpenGLRenderbuffer::GetID() const {
  return renderbufferID;
}

OpenGLRenderbuffer::~OpenGLRenderbuffer() {
  glDeleteRenderbuffers(1, &renderbufferID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting renderbuffer: " + std::to_string(error));
  }
}