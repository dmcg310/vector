#pragma once

#include "render_pass.h"
#include "../opengl_includes.h"

class OpenGLRenderPass : public RenderPass {
public:
  void Begin() override;
  void End() override;
  void SetClearColor(float r, float g, float b, float a) override;
  void SetViewportSize(float width, float height) override;
  glm::mat4 GetProjectionMatrix() const override;
  glm::mat4 GetModelMatrix() const override;

private:
  float viewportWidth;
  float viewportHeight;
  float clearColor[4];
};