#pragma once

#include "../core/window/window.h"
#include "context/context.h"

#include "buffer/buffer.h"
#include "framebuffer/framebuffer.h"
#include "render_command/render_command.h"
#include "render_command/render_command_queue.h"
#include "render_pass/render_pass.h"
#include "renderbuffer/renderbuffer.h"
#include "shader/shader.h"
#include "texture/texture.h"
#include "vertex_array/vertex_array.h"

#include "framebuffer/opengl_framebuffer.h"
#include "render_command/opengl_render_command.h"
#include "render_command/opengl_render_command_queue.h"
#include "render_pass/opengl_render_pass.h"
#include "renderbuffer/opengl_renderbuffer.h"
#include "shader/opengl_shader.h"
#include "texture/opengl_texture.h"
#include "vertex_array/opengl_vertex_array.h"

#include "buffer/opengl_indexbuffer.h"
#include "buffer/opengl_shader_storagebuffer.h"
#include "buffer/opengl_uniformbuffer.h"
#include "buffer/opengl_vertexbuffer.h"

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