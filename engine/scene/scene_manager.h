#pragma once

#include "scene.h"
#include "../core/log/log.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

class SceneManager {
public:
  void SetActiveScene(const std::string &sceneName);
  void AddScene(const std::string &sceneName, std::shared_ptr<Scene> scene);
  std::shared_ptr<Scene> GetActiveScene();

  void Update(float deltaTime);
  void Render();

  void Shutdown();

private:
  std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
  std::shared_ptr<Scene> activeScene;
};