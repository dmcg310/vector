#pragma once

#include <cstddef>

class Buffer {
public:
  virtual void Create(size_t size, const void* data) = 0;
  virtual void Bind() = 0;
  virtual void SetData(size_t size, const void* data) = 0;
  virtual ~Buffer() = default;
};