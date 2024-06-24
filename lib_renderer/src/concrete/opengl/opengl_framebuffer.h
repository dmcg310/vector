#pragma once

#include "../../../include/frambuffer.h"
#include "../../../include/renderbuffer.h"
#include "../../../include/texture.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class OpenGLFramebuffer : public Framebuffer {
public:
  void Create() override;
  void Bind() override;
  void AttachTexture(Texture *texture) override;
  void AttachRenderbuffer(Renderbuffer *renderbuffer) override;
  void CheckStatus() override;
  ~OpenGLFramebuffer() override;

private:
  unsigned int fbo;
};
