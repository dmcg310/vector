#pragma once

#include <algorithm>
#include <vector>

enum class EventType { KeyPress, KeyRelease, MouseMove, MouseClick };

class IEventObserver {
public:
  virtual void OnKeyPress(int key) = 0;
  virtual void OnKeyRelease(int key) = 0;
  virtual void OnMouseMove(double x, double y) = 0;
  virtual void OnMouseClick(int button) = 0;
  virtual void OnScroll(double yoffset) = 0;

  virtual ~IEventObserver() = default;

private:
};

class EventManager {
public:
  void RegisterObserver(IEventObserver *observer);
  void UnregisterObserver(IEventObserver *observer);

  void NotifyKeyPress(int key);
  void NotifyKeyRelease(int key);
  void NotifyMouseMove(double x, double y);
  void NotifyMouseClick(int button);
  void NotifyScroll(double yoffset);

private:
  std::vector<IEventObserver *> observers;
};
