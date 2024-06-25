#pragma once

#include "../../include/scene.h"
#include "../../lib_renderer/include/buffer.h"
#include "../../lib_renderer/include/texture.h"
#include "../../lib_renderer/include/shader.h"
#include "../../lib_renderer/include/vertex_array.h"
#include "../../lib_renderer/include/render_command_queue.h"
#include "../../lib_renderer/src/concrete/opengl/buffers/opengl_indexbuffer.h"
#include "../../lib_renderer/src/concrete/opengl/buffers/opengl_vertexbuffer.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_vertex_array.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_shader.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_texture.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_render_pass.h"
#include "../../lib_renderer/src/concrete/opengl/opengl_render_command_queue.h"
#include "../../lib_window/include/imgui_manager.h"
#include <glad/gl.h>

class TestScene : public Scene {
public:
  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;
  void Shutdown() override;

private:
  VertexArray* vao;
  Buffer* vertexBuffer;
  Buffer* indexBuffer;
  Texture* texture;
  Shader* shader;
  RenderPass* renderPass;
  RenderCommandQueue* renderCommandQueue;
};