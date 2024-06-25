#pragma once

#include <glm/glm.hpp>

class RenderPass {
public:
  virtual void Begin() = 0;
  virtual void End() = 0;
  virtual void SetClearColor(float r, float g, float b, float a) = 0;
  virtual void SetViewportSize(int width, int height) = 0;
  virtual glm::mat4 GetProjectionMatrix() const = 0;
  virtual glm::mat4 GetModelMatrix() const = 0;
  virtual ~RenderPass() = default;
};