#pragma once

#include "../../lib_window/include/window.h"
#include "context.h"

#include "../include/buffer.h"
#include "../include/framebuffer.h"
#include "../include/render_command.h"
#include "../include/render_command_queue.h"
#include "../include/render_pass.h"
#include "../include/renderbuffer.h"
#include "../include/shader.h"
#include "../include/texture.h"
#include "../include/vertex_array.h"

#include "../src/concrete/opengl/opengl_framebuffer.h"
#include "../src/concrete/opengl/opengl_render_command.h"
#include "../src/concrete/opengl/opengl_render_command_queue.h"
#include "../src/concrete/opengl/opengl_render_pass.h"
#include "../src/concrete/opengl/opengl_renderbuffer.h"
#include "../src/concrete/opengl/opengl_shader.h"
#include "../src/concrete/opengl/opengl_texture.h"
#include "../src/concrete/opengl/opengl_vertex_array.h"

#include "../src/concrete/opengl/buffers/opengl_indexbuffer.h"
#include "../src/concrete/opengl/buffers/opengl_shader_storagebuffer.h"
#include "../src/concrete/opengl/buffers/opengl_uniformbuffer.h"
#include "../src/concrete/opengl/buffers/opengl_vertexbuffer.h"

enum class API { OpenGL, None };

class RenderAPIFactory {
public:
  static Context *CreateContext(API api, GLFWwindow *window);
};

const API currentAPI = API::OpenGL;

std::shared_ptr<VertexArray> VertexArray::CreateVertexArray() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLVertexArray>();
    default:
      return nullptr;
  }
}

std::shared_ptr<Framebuffer> Framebuffer::CreateFramebuffer() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLFramebuffer>();
    default:
      return nullptr;
  }
}

std::shared_ptr<Renderbuffer> Renderbuffer::CreateRenderbuffer() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLRenderbuffer>();
    default:
      return nullptr;
  }
}

std::shared_ptr<Buffer> Buffer::CreateBuffer(BufferType type) {
  switch (currentAPI) {
    case API::OpenGL:
      switch (type) {
        case BufferType::Vertex:
          return std::make_shared<OpenGLVertexBuffer>();
        case BufferType::Index:
          return std::make_shared<OpenGLIndexBuffer>();
        case BufferType::Uniform:
          return std::make_shared<OpenGLUniformBuffer>();
        case BufferType::ShaderStorage:
          return std::make_shared<OpenGLShaderStorageBuffer>();
        default:
          return nullptr;
      }
    default:
      return nullptr;
  }
}

std::shared_ptr<Shader> Shader::CreateShader() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLShader>();
    default:
      return nullptr;
  }
}

std::shared_ptr<Texture> Texture::CreateTexture() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLTexture>();
    default:
      return nullptr;
  }
}

std::shared_ptr<RenderPass> RenderPass::CreateRenderPass() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLRenderPass>();
    default:
      return nullptr;
  }
}

std::shared_ptr<RenderCommandQueue> RenderCommandQueue::CreateRenderCommandQueue() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLRenderCommandQueue>();
    default:
      return nullptr;
  }
}

std::shared_ptr<RenderCommand> RenderCommand::CreateDrawElementsCommand() {
  switch (currentAPI) {
    case API::OpenGL:
      return std::make_shared<OpenGLDrawElementsCommand>();
    default:
      return nullptr;
  }
}