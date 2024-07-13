#pragma once

#include <glad/gl.h>
#include <memory>

class RenderCommand {
public:
  virtual ~RenderCommand() = default;
  virtual void Execute() = 0;

  virtual void SetDrawElementsParams(GLenum mode, GLsizei count, GLenum type,
                                     const void *indices) = 0;

  inline static std::shared_ptr<RenderCommand> CreateDrawElementsCommand();
};