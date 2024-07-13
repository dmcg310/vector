#pragma once

#include "render_command_queue.h"
#include <queue>

class OpenGLRenderCommandQueue : public RenderCommandQueue {
public:
  void Submit(std::function<void()> command) override;
  void Execute() override;
private:
  std::queue<std::function<void()>> commandQueue;
};
