#pragma once

#include "../../renderer/render_api_factory.h"
#include "../../scene/scene_node.h"
#include "node3d.h"
#include "../../renderer/renderer.h"
#include <glm/glm.hpp>

#ifdef _DEBUG
#include "../../core/window/imgui/imgui_manager.h"
#endif

class Cube3DNode : public Node3D {
public:
  Cube3DNode();
  ~Cube3DNode() override;

  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;

  void SetViewMatrixAndZoom(const glm::mat4 &_viewMatrix, float _zoom) override {
    this->viewMatrix = _viewMatrix;
    this->zoom = _zoom;
  }

  [[nodiscard]] std::shared_ptr<SceneNode> Clone() const override {
    auto node = std::make_shared<Cube3DNode>(*this);
    node->children.clear();

    for (const auto &child: children) { node->AddChild(child->Clone()); }

    return node;
  }

private:
  glm::mat4 viewMatrix;
  float zoom{};

  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<RenderPass> renderPass;
  std::shared_ptr<RenderCommand> renderCommand;
  std::shared_ptr<RenderCommandQueue> renderCommandQueue;

  glm::vec4 solidColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
};