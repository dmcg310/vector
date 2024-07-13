#pragma once

#include "renderbuffer.h"
#include "../opengl_includes.h"

class OpenGLRenderbuffer : public Renderbuffer {
public:
  void Create() override;
  void Bind() override;
  void SetStorage(GLenum internalFormat, int width, int height) override;
  unsigned int GetID() const override;
  ~OpenGLRenderbuffer() override;

private:
  GLuint renderbufferID;
};
