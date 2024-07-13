#pragma once

#include "pipeline_state.h"
#include "../opengl_includes.h"

class OpenGLPipelineState : public PipelineState {
public:
  void SetBlendState(bool enable) override;
  void SetDepthTest(bool enable) override;
  void SetCullFace(bool enable) override;
};