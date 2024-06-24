#pragma once

#include "../../../include/render_pass.h"
#include <glad/gl.h>

class OpenGLRenderPass : public RenderPass {
public:
  void Begin() override;
  void End() override;
  void SetClearColor(float r, float g, float b, float a) override;
};