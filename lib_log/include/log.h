#pragma once

#include <mutex>
#include <string>

class Log {
public:
  enum Level { INFO, WARNING, ERROR, FATAL };

  static void Initialize(const std::string &filename, bool logToFile,
                         bool logToConsole, bool resetLogFile);
  static void Write(Level level, const std::string &message);
  static void Shutdown();

private:
  static std::ofstream logFile;
  static bool logToFile;
  static bool logToConsole;

  static std::mutex logMutex;

  static std::string GetCurrentDateTime();
  static std::string LevelToString(Level level);

  static void CreateDirectories(const std::string &filepath);
};
