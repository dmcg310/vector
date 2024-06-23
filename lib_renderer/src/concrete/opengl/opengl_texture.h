#include "../../../include/texture.h"

class OpenGLTexture : public Texture {
public:
  OpenGLTexture();
  void LoadFromFile(const std::string &filePath) override;
  void Bind(unsigned int unit) override;
  void SetParameters() override;
  unsigned int GetID() const override;

private:
  unsigned int textureID;
};
