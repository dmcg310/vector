#include "opengl_render_command_queue.h"

void OpenGLRenderCommandQueue::Submit(std::function<void()> command) {
  commandQueue.push(command);
}

void OpenGLRenderCommandQueue::Execute() {
  while (!commandQueue.empty()) {
    auto command = commandQueue.front();
    command();

    commandQueue.pop();
  }
}