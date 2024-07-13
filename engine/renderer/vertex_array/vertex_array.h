#pragma once

#include "../buffer/buffer.h"
#include <memory>
#include <vector>

class VertexArray {
public:
  virtual void Create() = 0;
  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void AddVertexBuffer(std::shared_ptr<Buffer> vertexBuffer, int stride) = 0;
  virtual void SetIndexBuffer(std::shared_ptr<Buffer> indexBufferParam) = 0;
  [[nodiscard]] virtual unsigned int GetID() const = 0;
  virtual ~VertexArray() = default;

  inline static std::shared_ptr<VertexArray> CreateVertexArray();
};