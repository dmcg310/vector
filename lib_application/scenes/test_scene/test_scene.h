#pragma once

#include "../../include/scene.h"
#include "../../lib_renderer/include/buffer.h"
#include "../../lib_renderer/include/render_command_queue.h"
#include "../../lib_renderer/include/render_pass.h"
#include "../../lib_renderer/include/shader.h"
#include "../../lib_renderer/include/texture.h"
#include "../../lib_renderer/include/vertex_array.h"
#include "../../lib_window/include/imgui_manager.h"
#include <glad/gl.h>
#include <memory>

class TestScene : public Scene {
public:
  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;
  void Shutdown() override;

private:
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Texture> texture;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<RenderPass> renderPass;
  std::shared_ptr<RenderCommandQueue> renderCommandQueue;
};