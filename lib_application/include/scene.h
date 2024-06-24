#pragma once

class Scene {
public:
  virtual ~Scene() = default;
  virtual void Initialize() = 0;
  virtual void Update(float deltaTime) = 0;
  virtual void Render() = 0;
  virtual void Shutdown() = 0;
};