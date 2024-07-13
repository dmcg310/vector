#include "opengl_shader.h"
#include "../../core/settings/settings.h"

OpenGLShader::OpenGLShader() {
  shaderProgram = glCreateProgram();
  if (shaderProgram == 0) { Log::Write(Log::ERROR, "Failed to create shader program"); }
}

void OpenGLShader::Compile(const std::string &source, ShaderType shaderType) {
  GLenum glShaderType = ConvertShaderType(shaderType);
  GLuint shaderID = glCreateShader(glShaderType);

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

void OpenGLShader::SetUniform(const std::string &name, const glm::vec4 &value) {
  GLint location = GetUniformLocation(name);
  if (location == -1) { return; }

  glUniform4fv(location, 1, glm::value_ptr(value));

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

void OpenGLShader::SetUniform(const std::string &name, int value) {
  GLint location = GetUniformLocation(name);
  if (location == -1) { return; }

  glUniform1i(location, value);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error setting uniform '" + name + "': " + std::to_string(error));
  }
}

void OpenGLShader::SetUniformArray(const std::string &name,
                                   const std::vector<int> &values) {
  GLint location = GetUniformLocation(name);
  if (location == -1) { return; }

  glUniform1iv(location, values.size(), values.data());

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR,
               "Error setting uniform array '" + name + "': " + std::to_string(error));
  }
}

GLint OpenGLShader::GetUniformLocation(const std::string &name) {
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

void OpenGLShader::LoadFromFile(const std::string &filePath, ShaderType shaderType) {
  ApplicationSettings appSettings;
  std::string baseDirectory = appSettings.GetBaseDirectory();
  std::string fullPath = baseDirectory + "/" + filePath;

  std::ifstream file(fullPath);
  if (!file.is_open()) {
    Log::Write(Log::ERROR, "Failed to open shader file: " + fullPath);
    return;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string source = buffer.str();

  file.close();

  Compile(source, shaderType);
}

OpenGLShader::~OpenGLShader() {
  if (shaderProgram != 0) { glDeleteProgram(shaderProgram); }
}

GLenum OpenGLShader::ConvertShaderType(ShaderType shaderType) {
  switch (shaderType) {
    case ShaderType::Vertex:
      return GL_VERTEX_SHADER;
    case ShaderType::Fragment:
      return GL_FRAGMENT_SHADER;
    case ShaderType::Geometry:
      return GL_GEOMETRY_SHADER;
    case ShaderType::Compute:
      return GL_COMPUTE_SHADER;
    case ShaderType::TessControl:
      return GL_TESS_CONTROL_SHADER;
    case ShaderType::TessEvaluation:
      return GL_TESS_EVALUATION_SHADER;
    default:
      Log::Write(Log::ERROR, "Invalid shader type provided");
  }
}