#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader {
public:
  virtual void Compile(const std::string &source, GLenum shaderType) = 0;
  virtual void Link() = 0;
  virtual void Bind() = 0;
  virtual void SetUniform(const std::string &name, const glm::mat4 &value) = 0;
  virtual void SetUniform(const std::string &name, const glm::vec3 &value) = 0;
  virtual void SetUniform(const std::string &name, float value) = 0;
  virtual ~Shader() = default;
};
