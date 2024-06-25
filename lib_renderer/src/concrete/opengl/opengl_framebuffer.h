#pragma once

#include "../../../include/framebuffer.h"
#include "../../../include/renderbuffer.h"
#include "../../../include/texture.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class OpenGLFramebuffer : public Framebuffer {
public:
  void Create() override;
  void Bind() override;
  void Unbind() override;
  void Resize(uint32_t width, uint32_t height) override;
  void AttachTexture(Texture *texture) override;
  void AttachRenderbuffer(Renderbuffer *renderbuffer) override;
  ~OpenGLFramebuffer() override;

private:
  unsigned int fbo;
  Texture *texture;
  Renderbuffer *renderbuffer;
};