#pragma once

class RenderPass {
public:
  virtual void Begin() = 0;
  virtual void End() = 0;
  virtual void SetClearColor(float r, float g, float b, float a) = 0;
  virtual ~RenderPass() = default;
};