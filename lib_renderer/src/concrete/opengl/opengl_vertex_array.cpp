#include "opengl_vertex_array.h"
#include "../../../../lib_log/include/log.h"

void OpenGLVertexArray::Create() {
  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error creating vertex array object: " + std::to_string(error));
  }
}

void OpenGLVertexArray::Bind() {
  glBindVertexArray(vaoID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding vertex array object: " + std::to_string(error));
  }
}

void OpenGLVertexArray::Unbind() {
  glBindVertexArray(0);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error unbinding vertex array object: " + std::to_string(error));
  }
}

void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<Buffer> vertexBuffer,
                                        int stride) {
  Bind();
  vertexBuffer->Bind(0);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                        (void *) nullptr);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                        (void *) (3 * sizeof(float)));

  vertexBuffers.push_back(vertexBuffer);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error adding vertex buffer object: " + std::to_string(error));
  }
}

void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<Buffer> indexBufferParam) {
  Bind();
  indexBufferParam->Bind(0);

  indexBuffer = indexBufferParam;

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error setting index buffer object: " + std::to_string(error));
  }
}

OpenGLVertexArray::~OpenGLVertexArray() {
  glDeleteVertexArrays(1, &vaoID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error deleting vertex array object: " + std::to_string(error));
  }
}