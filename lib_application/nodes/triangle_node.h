#pragma once

#include "../../../lib_renderer/include/buffer.h"
#include "../../../lib_renderer/include/shader.h"
#include "../../../lib_renderer/include/texture.h"
#include "../../../lib_renderer/include/vertex_array.h"
#include "../include/scene_node.h"

#include <GLFW/glfw3.h>
// #include <glad/gl.h>

class TriangleNode : public SceneNode {
public:
  TriangleNode();
  ~TriangleNode();

  void Initialize();
  void Update(float deltaTime) override;
  void Render() override;

private:
  std::shared_ptr<VertexArray> vao;
  std::shared_ptr<Buffer> vertexBuffer;
  std::shared_ptr<Buffer> indexBuffer;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture> texture;
};