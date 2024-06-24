#pragma once

#include <vector>
#include <functional>

class RenderCommandQueue {
public:
  virtual void Submit(std::function<void()> command) = 0;
  virtual void Execute() = 0;
  virtual ~RenderCommandQueue() = default;
};