#pragma once

#include "../../renderer/render_api_factory.h"
#include "../../renderer/renderer.h"
#include "../../scene/scene_node.h"
#include "node3d.h"
#include <glm/glm.hpp>

#ifdef _DEBUG
#include "../../core/window/imgui/imgui_manager.h"
#endif

class LightingCubeNode : public Node3D {
public:
  LightingCubeNode();
  ~LightingCubeNode() override;

  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;

  void SetViewMatrixAndZoom(const glm::mat4 &_viewMatrix, float _zoom) override {
    this->viewMatrix = _viewMatrix;
    this->zoom = _zoom;
  }

private:
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<RenderPass> renderPass;
  std::shared_ptr<RenderCommand> renderCommand;
  std::shared_ptr<RenderCommandQueue> renderCommandQueue;

  float zoom;
  float angle = 0.0f;
  glm::mat4 viewMatrix;
  glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
};