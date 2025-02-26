#pragma once

#include "shader.h"
#include "../opengl_includes.h"
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

class OpenGLShader : public Shader {
public:
  OpenGLShader();
  ~OpenGLShader() override;

  void Compile(const std::string &source, ShaderType shaderType) override;
  void Link() override;
  void Bind() override;
  void SetUniform(const std::string &name, const glm::mat4 &value) override;
  void SetUniform(const std::string &name, const glm::vec4 &value) override;
  void SetUniform(const std::string &name, const glm::vec3 &value) override;
  void SetUniform(const std::string &name, float value) override;
  void SetUniform(const std::string &name, int value) override;
  void SetUniformArray(const std::string &name, const std::vector<int> &values) override;
  GLuint GetProgramID() const override { return shaderProgram; }
  void LoadFromFile(const std::string &filePath, ShaderType shaderType) override;

private:
  GLuint shaderProgram;
  std::unordered_map<std::string, GLint> uniformLocations;

  GLint GetUniformLocation(const std::string &name);
  static GLenum ConvertShaderType(ShaderType shaderType);
};