#pragma once

#include "../../../lib_renderer/include/buffer.h"
#include "../../../lib_renderer/include/render_api_factory.h"
#include "../../../lib_renderer/include/render_pass.h"
#include "../../../lib_renderer/include/shader.h"
#include "../../../lib_renderer/include/texture.h"
#include "../../../lib_renderer/include/vertex_array.h"
#include "../../lib_window/include/imgui_manager.h"

// TEMPORARY
#include "../../../lib_renderer/src/concrete/opengl/opengl_framebuffer.h"

#include <glm/glm.hpp>

class SimpleNode {
public:
  SimpleNode();
  ~SimpleNode();

  void Initialize();
  void Update(float deltaTime);
  void Render();
  void SetPosition(const glm::vec2 &position);
  [[nodiscard]] glm::vec2 GetPosition() const;
  [[nodiscard]] glm::mat4 GetModelMatrix(const glm::mat4 &renderPassModelMatrix) const;

private:
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Texture> texture;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<RenderPass> renderPass;
  std::shared_ptr<RenderCommandQueue> renderCommandQueue;
  glm::vec2 position;
};