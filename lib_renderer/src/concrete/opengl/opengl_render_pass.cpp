#include "opengl_render_pass.h"

void OpenGLRenderPass::Begin() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderPass::End() {
  // Additional steps to end the render pass if necessary
}

void OpenGLRenderPass::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}