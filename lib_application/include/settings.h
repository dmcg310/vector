#pragma once

#include "../../lib_log/include/log.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

// CMake will place the logs relative to the 'build' directory
#if defined(__linux__) || defined(__APPLE__)
const std::string LOG_FILE_PATH = "../logs/application.log";
const std::string CONFIG_FILE_PATH = "../config.vector";
#else
const std::string LOG_FILE_PATH = "../../../logs/application.log";
const std::string CONFIG_FILE_PATH = "../../../config.vector";
#endif

const int DEFAULT_WIDTH = 1200;
const int DEFAULT_HEIGHT = 800;
const bool DEFAULT_FULLSCREEN = false;
const std::string DEFAULT_TITLE = "Vector Engine";
const bool DEFAULT_LOG_TO_FILE = true;
const bool DEFAULT_LOG_TO_CONSOLE = true;
const bool DEFAULT_RESET_LOG_FILE = true;

/*
  config.vector defaults
  ----------------------
  [Window]
  Width=1200
  Height=800
  Fullscreen=false
  Title="Vector Engine"

  [Log]
  LogToFile=true
  LogToConsole=true
  ResetLogFile=true
*/

class ApplicationSettings {
public:
  ApplicationSettings() = default;

  struct Config {
    struct Window {
      int width;
      int height;
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

  bool CheckIfConfigExists();
  Config CreateConfig();
  Config LoadConfig();

private:
  Config config;

  std::string trim(const std::string &str);
  bool isNumber(const std::string &str);
  bool isBoolean(const std::string &str);
  bool isQuotedString(const std::string &str);
};
