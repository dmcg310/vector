#pragma once

#include <glm/glm.hpp>
#include <memory>

class RenderPass {
public:
  virtual void Begin() = 0;
  virtual void End() = 0;
  virtual void SetClearColor(float r, float g, float b, float a) = 0;
  virtual void SetViewportSize(float width, float height) = 0;
  virtual glm::mat4 GetProjectionMatrix() const = 0;
  virtual glm::mat4 GetModelMatrix() const = 0;
  virtual ~RenderPass() = default;

  inline static std::shared_ptr<RenderPass> CreateRenderPass();
};