#pragma once

#include "../../../include/shader.h"
#include <unordered_map>

class OpenGLShader : public Shader {
public:
  OpenGLShader();
  ~OpenGLShader();

  void Compile(const std::string &source, GLenum shaderType) override;
  void Link() override;
  void Bind() override;
  void SetUniform(const std::string &name, const glm::mat4 &value) override;
  void SetUniform(const std::string &name, const glm::vec3 &value) override;
  void SetUniform(const std::string &name, float value) override;

private:
  GLuint shaderProgram;
  std::unordered_map<std::string, GLint> uniformLocations;

  GLint GetUniformLocation(const std::string& name);
};