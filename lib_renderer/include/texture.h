#pragma once

#include <string>

class Texture {
public:
  virtual void LoadFromFile(const std::string &filePath) = 0;
  virtual void Bind(unsigned int unit) = 0;
  virtual unsigned int GetID() const = 0;
  virtual void SetParameters() = 0;
  virtual ~Texture() = default;
};
