#include "opengl_texture.h"
#include "../../../../lib_log/include/log.h"
#include "../../../../lib_application/include/settings.h" // For filesystem operations

#define STB_IMAGE_IMPLEMENTATION
#include "../../../../external/stb_image.h"

#include <filesystem>
#include <iostream>

OpenGLTexture::OpenGLTexture() {
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error creating texture: " + std::to_string(error));
  }
}

void OpenGLTexture::LoadFromFile(const std::string &filePath) {
  ApplicationSettings appSettings;
  std::string baseDirectory = appSettings.GetBaseDirectory();
  std::string fullPath = baseDirectory + "/" + filePath;

  int width, height, nrChannels;
  unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

  if (data) {
    GLenum format{};
    if (nrChannels == 1) {
      format = GL_RED;
    } else if (nrChannels == 3) {
      format = GL_RGB;
    } else if (nrChannels == 4) {
      format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      Log::Write(Log::ERROR, "Error loading texture: " + std::to_string(error));
    }
  } else {
    Log::Write(Log::ERROR, "Failed to load texture file: " + filePath);
  }
}

void OpenGLTexture::Bind(unsigned int unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, textureID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error binding texture: " + std::to_string(error));
  }
}

void OpenGLTexture::SetParameters() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

unsigned int OpenGLTexture::GetID() const {
  return textureID;
}

OpenGLTexture::~OpenGLTexture() {
  glDeleteTextures(1, &textureID);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Write(Log::ERROR, "Error deleting texture: " + std::to_string(error));
  }
}