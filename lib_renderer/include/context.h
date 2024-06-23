#pragma once

class Context {
public:
  virtual void Initialize() = 0;
  virtual void SwapBuffers() = 0;
  virtual ~Context() = default;
};