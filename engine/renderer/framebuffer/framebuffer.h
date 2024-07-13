#pragma once

#include "../renderbuffer/renderbuffer.h"
#include "../texture/texture.h"
#include "../opengl_includes.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>

class Framebuffer {
public:
  virtual void Create() = 0;
  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void Resize(uint32_t width, uint32_t height) = 0;
  virtual void AttachTexture(std::shared_ptr<Texture> texture) = 0;
  virtual void AttachRenderbuffer(std::shared_ptr<Renderbuffer> renderbuffer) = 0;
  virtual glm::vec2 GetSize(GLFWwindow *window) = 0;
  virtual ~Framebuffer() = default;

  inline static std::shared_ptr<Framebuffer> CreateFramebuffer();
};
