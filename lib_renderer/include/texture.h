#pragma once

class Texture {
public:
  viurtal void LoadFromFile(const std::string& filePath);
  viurtal void Bind(unsigned int unit);
  virtual unsigned int GetID() const = 0;
  virtual void SetParameters();
  virtual ~Texture() = default;
};