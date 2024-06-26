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

void OpenGLFramebuffer::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error unbinding framebuffer: " + std::to_string(error));
  }
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
  glDeleteFramebuffers(1, &fbo);

  Create();

  texture->Create(width, height, GL_RGBA);
  AttachTexture(texture);

  renderbuffer->Create();
  renderbuffer->SetStorage(GL_DEPTH24_STENCIL8, width, height);
  AttachRenderbuffer(renderbuffer);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::AttachTexture(std::shared_ptr<Texture> texture) {
  this->texture = texture;

  Bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture->GetID(), 0);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error attaching texture to framebuffer: " + std::to_string(error));
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::AttachRenderbuffer(std::shared_ptr<Renderbuffer> renderbuffer) {
  this->renderbuffer = renderbuffer;

  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                            renderbuffer->GetID());

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error attaching renderbuffer to framebuffer: " + std::to_string(error));
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::vec2 OpenGLFramebuffer::GetSize(GLFWwindow *window) {
  int width;
  int height;

  glfwGetFramebufferSize(window, &width, &height);

  return glm::vec2(width, height);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
  glDeleteFramebuffers(1, &fbo);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting framebuffer: " + std::to_string(error));
  }
}