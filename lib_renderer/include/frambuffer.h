#pragma once

class Texture;
class Renderbuffer;

class Framebuffer {
public:
  virtual void Create() = 0;
  virtual void Bind() = 0;
  virtual void AttachTexture(Texture *texture) = 0;
  virtual void AttachRenderbuffer(Renderbuffer *renderbuffer) = 0;
  virtual void CheckStatus() = 0;
  virtual ~Framebuffer() = default;
};
