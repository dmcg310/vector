#pragma once

#include "../scene.h"
#include "../../nodes/2D/node2d.h"
#include "../../nodes/2D/texture2d.h"
#include "../../nodes/3D/cube3d.h"
#include "../../nodes/3D/lighting_cube3d.h"
#include "../../nodes/3D/node3d.h"
#include "../../core/window/imgui/imgui_manager.h"
#include "../../core/window/window.h"
#include <glad/gl.h>
#include <memory>

class TestScene : public Scene {
public:
  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;
  void Shutdown() override;
};