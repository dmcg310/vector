#pragma once

#include <functional>
#include <memory>
#include <vector>

class RenderCommandQueue {
public:
  virtual void Submit(std::function<void()> command) = 0;
  virtual void Execute() = 0;
  virtual ~RenderCommandQueue() = default;

  inline static std::shared_ptr<RenderCommandQueue> CreateRenderCommandQueue();
};