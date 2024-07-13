#pragma once

#include <algorithm>
#include <vector>

class IEventObserver {
public:
  virtual void OnKeyPress(int key) = 0;
  virtual void OnKeyRelease(int key) = 0;
  virtual void OnMouseMove(double x, double y) = 0;
  virtual void OnMouseClick(int button) = 0;
  virtual void OnScroll(double xoffset, double yoffset) = 0;

  virtual ~IEventObserver() = default;
};

class EventManager {
public:
  void RegisterObserver(IEventObserver *observer);
  void UnregisterObserver(IEventObserver *observer);

  void NotifyKeyPress(int key) const;
  void NotifyKeyRelease(int key) const;
  void NotifyMouseMove(double x, double y) const;
  void NotifyMouseClick(int button) const;
  void NotifyScroll(double xoffset, double yoffset) const;

private:
  std::vector<IEventObserver *> observers;
};
