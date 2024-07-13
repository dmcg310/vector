#pragma once

#include "../../../include/renderbuffer.h"
#include <glad/gl.h>

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
