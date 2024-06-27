#pragma once

#include "../../lib_renderer/include/render_command.h"
#include "../../lib_renderer/include/render_command_queue.h"
#include "../../lib_renderer/include/render_pass.h"
#include "../../lib_renderer/include/shader.h"
#include "../../lib_renderer/include/texture.h"
#include "../../lib_renderer/include/vertex_array.h"
#include "node2d.h"

#ifdef _DEBUG
#include "../../lib_window/include/imgui_manager.h"
#endif

class Texture2DNode : public Node2D {
public:
  Texture2DNode();
  ~Texture2DNode() override;

  void Initialize(const std::string &textureFile);
  void Update(float deltaTime) override;
  void Render() override;

  [[nodiscard]] std::shared_ptr<SceneNode> Clone() const override {
    auto node = std::make_shared<Texture2DNode>(*this);
    node->children.clear();

    for (const auto &child: children) { node->AddChild(child->Clone()); }

    return node;
  }

private:
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Texture> texture;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<RenderPass> renderPass;
  std::shared_ptr<RenderCommand> renderCommand;
  std::shared_ptr<RenderCommandQueue> renderCommandQueue;
};