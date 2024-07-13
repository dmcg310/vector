#pragma once

#include "../../../include/render_command.h"

class OpenGLDrawElementsCommand : public RenderCommand {
public:
  OpenGLDrawElementsCommand()
      : mode(GL_TRIANGLES), count(0), type(GL_UNSIGNED_INT), indices(nullptr) {}

  void Execute() override { glDrawElements(mode, count, type, indices); }

  void SetDrawElementsParams(GLenum _mode, GLsizei _count, GLenum _type,
                             const void *_indices) override {
    mode = _mode;
    count = _count;
    type = _type;
    indices = _indices;
  }

private:
  GLenum mode;
  GLsizei count;
  GLenum type;
  const void *indices;
};
