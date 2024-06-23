#include "opengl_shader.h"
#include "../../../../lib_log/include/log.h"

OpenGLShader::OpenGLShader() {
  shaderProgram = glCreateProgram();
  if (shaderProgram == 0) {
    Log::Write(Log::ERROR, "Failed to create shader program");
  }
}

void OpenGLShader::Compile(const std::string &source, GLenum shaderType) {
  GLuint shaderID = glCreateShader(shaderType);

  const char *sourceCStr = source.c_str();
  glShaderSource(shaderID, 1, &sourceCStr, nullptr);
  glCompileShader(shaderID);

  GLint compileStatus;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus != GL_TRUE) {
    GLint logLength;

    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<char> errorLog(logLength);
    glGetShaderInfoLog(shaderID, logLength, &logLength, errorLog.data());

    Log::Write(Log::ERROR, "Error compiling shader: " + std::string(errorLog.begin(), errorLog.end()));

    glDeleteShader(shaderID);

    return;
  }

  glAttachShader(shaderProgram, shaderID);

  glDeleteShader(shaderID);
}

void OpenGLShader::Link() {
  glLinkProgram(shaderProgram);

  GLint linkStatus;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
  if (linkStatus != GL_TRUE) {
    GLint logLength;

    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<char> errorLog(logLength);
    glGetProgramInfoLog(shaderProgram, logLength, &logLength, errorLog.data());

    Log::Write(Log::ERROR, "Error linking shader program: " + std::string(errorLog.begin(), errorLog.end()));

    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
  }
}

void OpenGLShader::Bind() {
}

void OpenGLShader::SetUniform(const std::string &name, const glm::mat4 &value) {
}

void OpenGLShader::SetUniform(const std::string &name, const glm::vec3 &value) {
}

void OpenGLShader::SetUniform(const std::string &name, float value) {
}

GLint OpenGLShader::GetUniformLocation(const std::string &name) {
  return GLint();
}

OpenGLShader::~OpenGLShader() {
  if (shaderProgram != 0) {
    glDeleteProgram(shaderProgram);
  }
}
