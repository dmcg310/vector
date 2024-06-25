#pragma once

#include "buffer.h"
#include <memory>

class VertexArray {
public:
  virtual void Create() = 0;
  virtual void Bind() = 0;
  virtual void AddVertexBuffer(std::shared_ptr<Buffer> vertexBuffer) = 0;
  virtual void SetIndexBuffer(std::shared_ptr<Buffer> indexBufferParam) = 0;
  virtual ~VertexArray() = default;

  inline static std::shared_ptr<VertexArray> CreateVertexArray();
};