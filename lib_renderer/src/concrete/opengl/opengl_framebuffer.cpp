#include "opengl_framebuffer.h"
#include "../../../../lib_log/include/log.h"

void OpenGLFramebuffer::Create() {
  glGenFramebuffers(1, &fbo);
  Bind();
}

void OpenGLFramebuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void OpenGLFramebuffer::AttachTexture(Texture* texture) {
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0);
}

void OpenGLFramebuffer::AttachRenderbuffer(Renderbuffer* renderbuffer) {
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->GetID());
}

void OpenGLFramebuffer::CheckStatus() {
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    // Error here means we can continue running the program though there was an error
    Log::Write(
      Log::ERROR,
      "Error binding framebuffer. `glCheckFramebufferStatus()` did not return `GL_FRAMEBUFFER_COMPLETE`");
  }
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
  glDeleteFramebuffers(1, &fbo);
}