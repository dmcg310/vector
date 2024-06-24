#pragma once

#include "scene.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <utility>

class SceneManager {
public:
  void SetActiveScene(const std::string& sceneName);
  void AddScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

  void Update(float deltaTime);
  void Render();

private:
  std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
  std::shared_ptr<Scene> activeScene;
};