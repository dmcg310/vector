#pragma once

#include <string>

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
        logFilePath("../logs/application.log"), resetLogFile(true) {}
};
