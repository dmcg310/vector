#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>

class Log {
public:
  enum Level { INFO, WARNING, ERROR, FATAL };

  static void Initialize(const std::string &filename, bool logToFile,
                         bool logToConsole, bool resetLogFile);
  static void Write(Level level, const std::string &message);
  static void WriteFrameLog(const std::string& message);
  static void Shutdown();

#ifdef _DEBUG
  static std::deque<std::pair<Level, std::string>> GetLogBuffer(); // For forwarding logs to ImGui debug menu
  static std::deque<std::string> GetFrameLogBuffer();
#endif

private:
  static std::ofstream logFile;
  static bool logToFile;
  static bool logToConsole;

  static std::mutex logMutex;

  static bool initialized;

  static std::queue<std::pair<Level, std::string>> logQueue;
#ifdef _DEBUG
  static std::deque<std::pair<Level, std::string>> logBuffer;
  static std::deque<std::string> frameLogBuffer;
#endif

  static void FlushQueue();

  static std::string GetCurrentDateTime();
  static std::string LevelToString(Level level);

  static void CreateDirectories(const std::string &filepath);
};
