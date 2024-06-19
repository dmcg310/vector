#include "../include/log.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

bool Log::Initialize() {
  // Here we can open the logfile for writing, if the user specifies file based
  // logging

  return true;
}

void Log::Write(Level level, const std::string &message) {
  std::ostringstream oss;

  oss << "(" << GetCurrentDateTime() << ")"
      << "[" << LevelToString(level) << "]"
      << ": " << message;

  std::string logMsg = oss.str();
  std::cout << logMsg << std::endl;

  // Or write to file
}

void Log::Shutdown() {
  // Here we can close the logfile
}

std::string Log::GetCurrentDateTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time_t);

  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");

  return oss.str();
}

std::string Log::LevelToString(Level level) {
  switch (level) {
  case INFO:
    return "INFO";
  case WARNING:
    return "WARNING";
  case ERROR:
    return "ERROR";
  case FATAL:
    return "FATAL";

  default:
    return "UNKNOWN";
  }
}
