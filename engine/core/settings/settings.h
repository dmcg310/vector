#pragma once

#include "core/log/log.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

const int DEFAULT_WIDTH = 1400;
const int DEFAULT_HEIGHT = 900;
const bool DEFAULT_WINDOWED_FULLSCREEN = true;
const bool DEFAULT_BORDERLESS_FULLSCREEN = false;
const bool DEFAULT_FULLSCREEN = false;
const std::string DEFAULT_TITLE = "Vector Engine";
const bool DEFAULT_LOG_TO_FILE = true;
const bool DEFAULT_LOG_TO_CONSOLE = true;
const bool DEFAULT_RESET_LOG_FILE = true;

class ApplicationSettings {
public:
  ApplicationSettings();

  struct Config {
    struct Window {
      int width;
      int height;
      bool windowedFullscreen;
      bool borderlessFullscreen;
      bool fullscreen;
      std::string title;
    };

    struct Log {
      bool logToFile;
      bool logToConsole;
      bool resetLogFile;
      std::string logFilePath;
    };

    Window window;
    Log log;
  };

  static bool CheckIfConfigExists();
  static Config CreateConfig();
  static Config LoadConfig();

  static std::string GetBaseDirectory();

private:
  Config config;

  static std::string trim(const std::string &str);
  static bool isNumber(const std::string &str);
  static bool isBoolean(const std::string &str);
  static bool isQuotedString(const std::string &str);

  static std::string GetAbsoluteLogFilePath();
  static std::string GetAbsoluteConfigFilePath();
};
