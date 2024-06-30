#pragma once

#include "../../include/scene.h"
#include "../../lib_application/nodes/2D/node2d.h"
#include "../../lib_application/nodes/2D/texture2d.h"
#include "../../lib_application/nodes/3D/cube3d.h"
#include "../../lib_application/nodes/3D/node3d.h"
#include "../../lib_window/include/imgui_manager.h"
#include <glad/gl.h>
#include <memory>

class TestScene : public Scene {
public:
  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;
  void Shutdown() override;
};