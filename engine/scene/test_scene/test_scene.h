#pragma once

#include "../../core/window/imgui/imgui_manager.h"
#include "../../core/window/window.h"
#include "../../nodes/3D/cube3d.h"
#include "../../nodes/3D/lighting_cube3d.h"
#include "../../nodes/3D/node3d.h"
#include "../scene.h"
#include <glad/glad.h>
#include <memory>

class TestScene : public Scene {
public:
  void Initialize() override;
  void Update(float deltaTime) override;
  void Render() override;
  void Shutdown() override;
};