#include "event.h"

void EventManager::RegisterObserver(IEventObserver *observer) {
  observers.push_back(observer);
}

void EventManager::UnregisterObserver(IEventObserver *observer) {
  observers.erase(std::remove(observers.begin(), observers.end(), observer),
                  observers.end());
}

void EventManager::NotifyKeyPress(int key) const {
  for (auto observer: observers) { observer->OnKeyPress(key); }
}

void EventManager::NotifyKeyRelease(int key) const {
  for (auto observer: observers) { observer->OnKeyRelease(key); }
}

void EventManager::NotifyMouseMove(double x, double y) const {
  for (auto observer: observers) { observer->OnMouseMove(x, y); }
}

void EventManager::NotifyMouseClick(int button) const {
  for (auto observer: observers) { observer->OnMouseClick(button); }
}

void EventManager::NotifyScroll(double xoffset, double yoffset) const {
  for (auto observer: observers) { observer->OnScroll(xoffset, yoffset); }
}