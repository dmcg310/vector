#include "opengl_shader.h"
#include "../../../../lib_log/include/log.h"

OpenGLShader::OpenGLShader() {
}

OpenGLShader::~OpenGLShader() {
}

void OpenGLShader::Compile(const std::string &source, GLenum shaderType) {
}

void OpenGLShader::Link() {
}

void OpenGLShader::Bind() {
}

void OpenGLShader::SetUniform(const std::string &name, const glm::mat4 &value) {
}

void OpenGLShader::SetUniform(const std::string &name, const glm::vec3 &value) {
}

void OpenGLShader::SetUniform(const std::string &name, float value) {
}
