#pragma once

#include <string>

class Log {
public:
  enum Level { INFO, WARNING, ERROR, FATAL };

  static bool Initialize();
  static void Write(Level level, const std::string &message);
  static void Shutdown();

private:
  static std::string GetCurrentDateTime();
  static std::string LevelToString(Level level);
};
