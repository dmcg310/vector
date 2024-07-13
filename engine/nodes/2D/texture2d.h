#pragma once

#include "../../renderer/render_api_factory.h"
#include "../../renderer/render_command/render_command.h"
#include "../../renderer/render_command/render_command_queue.h"
#include "../../renderer/render_pass/render_pass.h"
#include "../../renderer/shader/shader.h"
#include "../../renderer/texture/texture.h"
#include "../../renderer/vertex_array/vertex_array.h"
#include "node2d.h"

#ifdef _DEBUG
#include "../../core/window/imgui/imgui_manager.h"
#endif

class Texture2DNode : public Node2D {
public:
  Texture2DNode();
  ~Texture2DNode() override;

  void Initialize(const std::string &textureFile);
  void Update(float deltaTime) override;
  void Render() override;

private:
  std::shared_ptr<Texture> texture;
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<RenderPass> renderPass;
  std::shared_ptr<RenderCommand> renderCommand;
  std::shared_ptr<RenderCommandQueue> renderCommandQueue;
};