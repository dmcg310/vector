#pragma once

#include "../../../lib_renderer/include/render_api_factory.h"
#include "../../include/scene_node.h"
#include "node3d.h"
#include <glm/glm.hpp>

#ifdef _DEBUG
#include "../../lib_window/include/imgui_manager.h"
#endif

class Cube3DNode : public Node3D {
public:
  Cube3DNode();
  ~Cube3DNode() override;

  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;

  [[nodiscard]] std::shared_ptr<SceneNode> Clone() const override {
    auto node = std::make_shared<Cube3DNode>(*this);
    node->children.clear();

    for (const auto &child: children) { node->AddChild(child->Clone()); }

    return node;
  }

private:
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<RenderPass> renderPass;
  std::shared_ptr<RenderCommand> renderCommand;
  std::shared_ptr<RenderCommandQueue> renderCommandQueue;
  float rotationAngleX = 0.0f;
  float rotationAngleY = 0.0f;
};