#pragma once

#include <string>

// CMake will place the logs relative to the 'build' directory
#if defined(__linux__) || defined(__APPLE__)
const std::string LOG_FILE_PATH = "../logs/application.log";
#else
const std::string LOG_FILE_PATH = "../../../logs/application.log";
#endif

struct ApplicationSettings {
  // We can load this from a config file

  int windowWidth;
  int windowHeight;
  std::string windowTitle;
  bool fullscreen;

  bool logToFile;
  bool logToConsole;
  std::string logFilePath;
  bool resetLogFile; // Reset the log file on each startup

  ApplicationSettings()
      : windowWidth(1200), windowHeight(800), windowTitle("Vector Engine"),
        fullscreen(false), logToFile(true), logToConsole(true),
        logFilePath(LOG_FILE_PATH), resetLogFile(true) {}
};
