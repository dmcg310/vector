#include "../include/event.h"

void EventManager::RegisterObserver(IEventObserver *observer) {
  observers.push_back(observer);
}

void EventManager::UnregisterObserver(IEventObserver *observer) {
  observers.erase(std::remove(observers.begin(), observers.end(), observer),
                  observers.end());
}

void EventManager::NotifyKeyPress(int key) {
  for (auto observer: observers) { observer->OnKeyPress(key); }
}

void EventManager::NotifyKeyRelease(int key) {
  for (auto observer: observers) { observer->OnKeyRelease(key); }
}

void EventManager::NotifyMouseMove(double x, double y) {
  for (auto observer: observers) { observer->OnMouseMove(x, y); }
}

void EventManager::NotifyMouseClick(int button) {
  for (auto observer: observers) { observer->OnMouseClick(button); }
}

void EventManager::NotifyScroll(double yoffset) {
  for (auto observer: observers) { observer->OnScroll(yoffset); }
}