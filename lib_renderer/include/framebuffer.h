#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <memory>

class Texture;
class Renderbuffer;

class Framebuffer {
public:
  virtual void Create() = 0;
  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void Resize(uint32_t width, uint32_t height) = 0;
  virtual void AttachTexture(Texture *texture) = 0;
  virtual void AttachRenderbuffer(Renderbuffer *renderbuffer) = 0;
  virtual glm::vec2 GetSize(GLFWwindow *window) = 0;
  virtual ~Framebuffer() = default;

  inline static std::shared_ptr<Framebuffer> CreateFramebuffer();
};
