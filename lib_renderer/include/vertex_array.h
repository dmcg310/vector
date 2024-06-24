#pragma once

#include "buffer.h"

class VertexArray {
public:
  virtual void Create() = 0;
  virtual void Bind() = 0;
  virtual void AddVertexBuffer(Buffer *vertextBuffer) = 0;
  virtual void SetIndexBuffer(Buffer *indexBufferParam) = 0;
  virtual ~VertexArray() = default;
};