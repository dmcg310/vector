#include "opengl_render_pass.h"

void OpenGLRenderPass::Begin() {
  glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderPass::End() {
  // Additional steps to end the render pass if necessary
}

void OpenGLRenderPass::SetClearColor(float r, float g, float b, float a) {
  clearColor[0] = r;
  clearColor[1] = g;
  clearColor[2] = b;
  clearColor[3] = a;
}

void OpenGLRenderPass::SetViewportSize(float width, float height) {
  viewportWidth = width;
  viewportHeight = height;

  glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

glm::mat4 OpenGLRenderPass::GetProjectionMatrix() const {
  return glm::ortho(0.0f, viewportWidth, 0.0f, viewportHeight);
}

glm::mat4 OpenGLRenderPass::GetModelMatrix() const {
  float aspectRatio = viewportWidth / viewportHeight;
  float scale = std::min(viewportWidth, viewportHeight) / 2.0f;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model,
                         glm::vec3(viewportWidth / 2.0f, viewportHeight / 2.0f, 0.0f));
  model = glm::scale(model, glm::vec3(scale * aspectRatio, scale, 1.0f));

  return model;
}