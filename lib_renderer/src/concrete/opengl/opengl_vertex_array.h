#pragma once

#include "../../../include/vertex_array.h"
#include <glad/gl.h>
#include <vector>

class OpenGLVertexArray : public VertexArray {
public:
  void Create() override;
  void Bind() override;
  void AddVertexBuffer(Buffer* vertexBuffer) override;
  void SetIndexBuffer(Buffer* indexBufferParam) override;

  ~OpenGLVertexArray() override;
private:
  GLuint vaoID;
  std::vector<Buffer*> vertexBuffers;
  Buffer* indexBuffer;
};