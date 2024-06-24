#include "opengl_shader.h"
#include "../../../../lib_log/include/log.h"

OpenGLShader::OpenGLShader() {
  shaderProgram = glCreateProgram();
  if (shaderProgram == 0) { Log::Write(Log::ERROR, "Failed to create shader program"); }
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

    Log::Write(Log::ERROR, "Error compiling shader: " +
                                   std::string(errorLog.begin(), errorLog.end()));

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

    Log::Write(Log::ERROR, "Error linking shader program: " +
                                   std::string(errorLog.begin(), errorLog.end()));

    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
  }
}

void OpenGLShader::Bind() {
  glUseProgram(shaderProgram);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding shader: " + std::to_string(error));
  }
}

void OpenGLShader::SetUniform(const std::string &name, const glm::mat4 &value) {
  GLint location = GetUniformLocation(name);
  if (location == -1) { return; }

  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error setting uniform '" + name + "': " + std::to_string(error));
  }
}

void OpenGLShader::SetUniform(const std::string &name, const glm::vec3 &value) {
  GLint location = GetUniformLocation(name);
  if (location == -1) { return; }

  glUniform3fv(location, 1, glm::value_ptr(value));

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error setting uniform '" + name + "': " + std::to_string(error));
  }
}

void OpenGLShader::SetUniform(const std::string &name, float value) {
  GLint location = GetUniformLocation(name);
  if (location == -1) { return; }

  glUniform1f(location, value);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error setting uniform '" + name + "': " + std::to_string(error));
  }
}

GLint OpenGLShader::GetUniformLocation(const std::string &name) {
  // We can cache the uniform location for performance
  if (auto it = uniformLocations.find(name); it != uniformLocations.end()) {
    return it->second;
  }

  GLint location = glGetUniformLocation(shaderProgram, name.c_str());
  if (location == -1) {
    Log::Write(Log::WARNING, "Uniform '" + name + "' not found in shader program.");
  }

  uniformLocations[name] = location;
  return location;
}

OpenGLShader::~OpenGLShader() {
  if (shaderProgram != 0) { glDeleteProgram(shaderProgram); }
}