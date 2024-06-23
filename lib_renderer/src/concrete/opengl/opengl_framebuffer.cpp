#include "opengl_framebuffer.h"
#include "../../../../lib_log/include/log.h"

void OpenGLFramebuffer::Create() {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error creating framebuffer: " + std::to_string(error));
  }
}

void OpenGLFramebuffer::Bind() { 
  glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding framebuffer: " + std::to_string(error));
  }
}

void OpenGLFramebuffer::AttachTexture(Texture *texture) {
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture->GetID(), 0);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error attaching texture to framebuffer: " + std::to_string(error));
  }
}

void OpenGLFramebuffer::AttachRenderbuffer(Renderbuffer *renderbuffer) {
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, renderbuffer->GetID());

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error attaching renderbuffer to framebuffer: " + std::to_string(error));
  }
}

void OpenGLFramebuffer::CheckStatus() {
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    Log::Write(Log::ERROR,
               "Error binding framebuffer. `glCheckFramebufferStatus()` did "
               "not return `GL_FRAMEBUFFER_COMPLETE`");
  }
}

OpenGLFramebuffer::~OpenGLFramebuffer() { 
  glDeleteFramebuffers(1, &fbo); 

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting framebuffer: " + std::to_string(error));
  }
}
