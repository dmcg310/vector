#pragma once

#include <cstddef>
#include <cstdint>

class Buffer {
public:
  virtual void Create(size_t size, const void *data) = 0;
  virtual void Bind(uint32_t bindingPoint) = 0;
  virtual void SetData(size_t size, const void *data) = 0;
  virtual ~Buffer() = default;
};
