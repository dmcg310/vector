#include "opengl_pipeline_state.h"

void OpenGLPipelineState::SetBlendState(bool enable) {
  if (enable) {
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }
}

void OpenGLPipelineState::SetDepthTest(bool enable) {
  if (enable) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void OpenGLPipelineState::SetCullFace(bool enable) {
  if (enable) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}
