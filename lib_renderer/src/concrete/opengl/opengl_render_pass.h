#pragma once

#include "../../../include/render_pass.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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