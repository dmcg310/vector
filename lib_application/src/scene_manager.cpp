#include "scene_manager.h"
#include "../../lib_log/include/log.h"

void SceneManager::SetActiveScene(const std::string& sceneName) {
  if (auto it = scenes.find(sceneName); it != scenes.end()) {
    if (activeScene) {
      activeScene->Shutdown();
    }

    activeScene = it->second;
    activeScene->Initialize();

    Log::Write(Log::INFO, "Scene: " + sceneName + " loaded");
  }
}

void SceneManager::AddScene(const std::string& sceneName, std::shared_ptr<Scene> scene) {
  scenes[sceneName] = std::move(scene);
}

void SceneManager::Update(float deltaTime) {
  if (activeScene) {
    activeScene->Update(deltaTime);
  }
}

void SceneManager::Render() {
  if (activeScene) {
    activeScene->Render();
  }
}

void SceneManager::Shutdown() {
  if (activeScene) {
    activeScene->Shutdown();
    activeScene = nullptr;
  }

  for (auto& pair : scenes) {
    pair.second->Shutdown();
  }

  scenes.clear();

  Log::Write(Log::INFO, "Scene Manager shutdown complete");
}
