#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>

enum class ShaderType {
  Vertex,
  Fragment,
  Geometry,
  Compute,
  TessControl,
  TessEvaluation
};

class Shader {
public:
  virtual void Compile(const std::string &source, ShaderType shaderType) = 0;
  virtual void Link() = 0;
  virtual void Bind() = 0;
  virtual void SetUniform(const std::string &name, const glm::mat4 &value) = 0;
  virtual void SetUniform(const std::string &name, const glm::vec3 &value) = 0;
  virtual void SetUniform(const std::string &name, float value) = 0;
  virtual void LoadFromFile(const std::string &filePath, ShaderType shaderType) = 0;
  virtual ~Shader() = default;

  inline static std::shared_ptr<Shader> CreateShader();
};