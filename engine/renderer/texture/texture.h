#pragma once

#include <string>
#include <memory>

class Texture {
public:
  virtual void LoadFromFile(const std::string &filePath) = 0;
  virtual void Create(int width, int height, unsigned int format) = 0;
  virtual void Bind(unsigned int unit) = 0;
  virtual unsigned int GetID() const = 0;
  virtual void SetParameters() = 0;
  virtual ~Texture() = default;

  inline static std::shared_ptr<Texture> CreateTexture();
};
