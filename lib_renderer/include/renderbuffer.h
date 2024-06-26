#pragma once

#include <memory>

class Renderbuffer {
public:
  virtual void Create() = 0;
  virtual void Bind() = 0;
  virtual void SetStorage(unsigned int internalFormat, int width, int height) = 0;
  virtual unsigned int GetID() const = 0;
  virtual ~Renderbuffer() = default;

  inline static std::shared_ptr<Renderbuffer> CreateRenderbuffer();
};