#include "../../../include/renderbuffer.h"

class OpenGLRenderbuffer : public Renderbuffer {
public:
  OpenGLRenderbuffer();
  void Create() override;
  void Bind() override;
  unsigned int GetID() const override;

private:
  unsigned int renderbufferID;
};
