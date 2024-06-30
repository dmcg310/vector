#pragma once

#include "../../../include/vertex_array.h"
#include <glad/gl.h>
#include <vector>

class OpenGLVertexArray : public VertexArray {
public:
  void Create() override;
  void Bind() override;
  void Unbind() override;
  void AddVertexBuffer(std::shared_ptr<Buffer> vertexBuffer, int stride) override;
  void SetIndexBuffer(std::shared_ptr<Buffer> indexBufferParam) override;
  [[nodiscard]] unsigned int GetID() const override { return vaoID; }

  ~OpenGLVertexArray() override;

private:
  GLuint vaoID;
  std::vector<std::shared_ptr<Buffer>> vertexBuffers;
  std::shared_ptr<Buffer> indexBuffer;
};