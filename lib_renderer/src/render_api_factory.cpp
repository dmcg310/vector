#include "../include/render_api_factory.h"
#include "concrete/opengl/opengl_context.h"

Context* RenderAPIFactory::CreateContext(API api, GLFWwindow* window) {
  switch (api) {
    case API::OpenGL: return new OpenGLContext(window);
    
    default: return nullptr;
  }
}