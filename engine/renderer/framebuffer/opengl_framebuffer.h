#pragma once

#include "framebuffer.h"
#include "../renderbuffer/renderbuffer.h"
#include "../texture/texture.h"

class OpenGLFramebuffer : public Framebuffer {
public:
  void Create() override;
  void Bind() override;
  void Unbind() override;
  void Resize(uint32_t width, uint32_t height) override;
  void AttachTexture(std::shared_ptr<Texture> texture) override;
  void AttachRenderbuffer(std::shared_ptr<Renderbuffer> renderbuffer) override;
  glm::vec2 GetSize(GLFWwindow *window) override;
  ~OpenGLFramebuffer() override;

private:
  unsigned int fbo;
  std::shared_ptr<Texture> texture;
  std::shared_ptr<Renderbuffer> renderbuffer;
};