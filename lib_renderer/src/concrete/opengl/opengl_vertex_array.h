#pragma once

#include "../../../include/vertex_array.h"
#include <glad/gl.h>
#include <vector>

class OpenGLVertexArray : public VertexArray {
public:
  void Create() override;
  void Bind() override;
  void AddVertexBuffer(std::shared_ptr<Buffer> vertexBuffer) override;
  void SetIndexBuffer(std::shared_ptr<Buffer> indexBufferParam) override;

  ~OpenGLVertexArray() override;

private:
  GLuint vaoID;
  std::vector<std::shared_ptr<Buffer>> vertexBuffers;
  std::shared_ptr<Buffer> indexBuffer;
};