#pragma once

#include "../../../include/texture.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class OpenGLTexture : public Texture {
public:
  OpenGLTexture();
  void LoadFromFile(const std::string &filePath) override;
  void Create(int width, int height, GLenum format) override;
  void Bind(unsigned int unit) override;
  void SetParameters() override;
  unsigned int GetID() const override;
  ~OpenGLTexture() override;

private:
  unsigned int textureID;
};
