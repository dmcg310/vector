#include "../include/settings.h"

// Utility functions

std::string boolToString(bool value) { return value ? "true" : "false"; }

bool stringToBool(const std::string &str) { return str == "true"; }

bool ApplicationSettings::CheckIfConfigExists() {
#ifdef __APPLE__
  namespace fs = std::__fs::filesystem;
#else
  namespace fs = std::filesystem;
#endif

  if (!fs::exists(CONFIG_FILE_PATH)) {
    return false;
  }

  return true;
}

ApplicationSettings::Config ApplicationSettings::CreateConfig() {
  Config config;

  std::ofstream createFile(CONFIG_FILE_PATH);
  if (!createFile) {
    Log::Write(Log::ERROR, "Error creating config file");
  }

  createFile.close();

  std::ofstream configFile;
  std::ios_base::openmode mode = std::ios::out;
  configFile.open(CONFIG_FILE_PATH, mode);

  std::ostringstream oss;
  oss << "[Window]" << "\nWidth=" << DEFAULT_WIDTH
      << "\nHeight=" << DEFAULT_HEIGHT
      << "\nFullscreen=" << boolToString(DEFAULT_FULLSCREEN) << "\nTitle=\""
      << DEFAULT_TITLE << "\"" << "\n\n[Log]"
      << "\nLogToFile=" << boolToString(DEFAULT_LOG_TO_FILE)
      << "\nLogToConsole=" << boolToString(DEFAULT_LOG_TO_CONSOLE)
      << "\nResetLogFile=" << boolToString(DEFAULT_RESET_LOG_FILE);
  std::string configString = oss.str();

  configFile << configString;

  config.window.width = DEFAULT_WIDTH;
  config.window.height = DEFAULT_HEIGHT;
  config.window.fullscreen = DEFAULT_FULLSCREEN;
  config.window.title = DEFAULT_TITLE;

  config.log.logToFile = DEFAULT_LOG_TO_FILE;
  config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
  config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
  config.log.logFilePath = LOG_FILE_PATH;

  return config;
}

ApplicationSettings::Config ApplicationSettings::LoadConfig() {
  Config config;

  std::ifstream file(CONFIG_FILE_PATH);
  if (!file.is_open()) {
    Log::Write(Log::ERROR,
               "Error reading config file, creating one with defaults");

    return ApplicationSettings::CreateConfig();
  }

  std::string line;
  std::string currentSection;
  std::unordered_map<std::string, bool> keysPresent;

  bool windowSectionExists = false, logSectionExists = false;

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    if (line[0] == '[' && line.back() == ']') {
      currentSection = line.substr(1, line.size() - 2);
    } else {
      std::istringstream lineStream(line);
      std::string key, value;

      if (std::getline(lineStream, key, '=')) {
        key = trim(key);

        if (!std::getline(lineStream, value)) {
          value = "";
        }

        value = trim(value);

        keysPresent[key] = true;

        if (value.empty()) {
          Log::Write(Log::ERROR,
                     "Empty value found in config file, using default");
        }

        if (currentSection == "Window") {
          windowSectionExists = true;

          if (key == "Width") {
            if (value.empty() || !isNumber(value)) {
              Log::Write(
                  Log::ERROR,
                  "Invalid window width value in config file, using default");

              config.window.width = DEFAULT_WIDTH;
            } else {
              config.window.width = std::stoi(value);
            }

          } else if (key == "Height") {
            if (value.empty() || !isNumber(value)) {
              Log::Write(
                  Log::ERROR,
                  "Invalid window height value in config file, using default");

              config.window.height = DEFAULT_HEIGHT;
            } else {
              config.window.height = std::stoi(value);
            }

          } else if (key == "Fullscreen") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(Log::ERROR, "Invalid window fullscreen value in "
                                     "config file, using default");

              config.window.fullscreen = DEFAULT_FULLSCREEN;
            } else {
              config.window.fullscreen = stringToBool(value);
            }

          } else if (key == "Title") {
            if (value.empty() || !isQuotedString(value)) {
              Log::Write(
                  Log::ERROR,
                  "Invalid window title value in config file, using default");

              config.window.title = DEFAULT_TITLE;
            } else {
              config.window.title = value.substr(1, value.size() - 2);
            }
          }
        } else if (currentSection == "Log") {
          logSectionExists = true;
          if (key == "LogToFile") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(
                  Log::ERROR,
                  "Invalid log to file value in config file, using default");

              config.log.logToFile = DEFAULT_LOG_TO_FILE;
            } else {
              config.log.logToFile = stringToBool(value);
            }

          } else if (key == "LogToConsole") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(
                  Log::ERROR,
                  "Invalid log to console value in config file, using default");

              config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
            } else {
              config.log.logToConsole = stringToBool(value);
            }

          } else if (key == "ResetLogFile") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(
                  Log::ERROR,
                  "Invalid reset log file value in config file, using default");

              config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
            } else {
              config.log.resetLogFile = stringToBool(value);
            }

          } else {
            Log::Write(Log::ERROR, "Unknown key found in config file");
          }

        } else {
          Log::Write(Log::ERROR, "Unknown section found in config file");
        }

      } else {
        Log::Write(Log::ERROR, "Invalid line found in config file");
      }
    }

    config.log.logFilePath = LOG_FILE_PATH;
  }

  if (!windowSectionExists) {
    Log::Write(
        Log::ERROR,
        "Window section was not found in config file, using default values");

    config.window.width = DEFAULT_WIDTH;
    keysPresent["Width"] = true;

    config.window.height = DEFAULT_HEIGHT;
    keysPresent["Height"] = true;

    config.window.fullscreen = DEFAULT_FULLSCREEN;
    keysPresent["Fullscreen"] = true;

    config.window.title = DEFAULT_TITLE;
    keysPresent["Title"] = true;
  }

  if (!logSectionExists) {
    Log::Write(
        Log::ERROR,
        "Log section was not found in config file, using default values");

    config.log.logToFile = DEFAULT_LOG_TO_FILE;
    keysPresent["LogToFile"] = true;

    config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
    keysPresent["LogToConsole"] = true;

    config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
    keysPresent["ResetLogFile"] = true;

    config.log.logFilePath = LOG_FILE_PATH;
    keysPresent["LogFilePath"] = true;
  }

  if (!keysPresent["Width"]) {
    config.window.width = DEFAULT_WIDTH;
    Log::Write(Log::ERROR,
               "Width key missing from config file, using default value");
  }

  if (!keysPresent["Height"]) {
    config.window.height = DEFAULT_HEIGHT;
    Log::Write(Log::ERROR,
               "Height key missing from config file, using default value");
  }

  if (!keysPresent["Fullscreen"]) {
    config.window.fullscreen = DEFAULT_FULLSCREEN;
    Log::Write(Log::ERROR,
               "Fullscreen key missing from config file, using default value");
  }

  if (!keysPresent["Title"]) {
    config.window.title = DEFAULT_TITLE;
    Log::Write(Log::ERROR,
               "Title key missing from config file, using default value");
  }

  if (!keysPresent["LogToFile"]) {
    config.log.logToFile = DEFAULT_LOG_TO_FILE;
    Log::Write(Log::ERROR,
               "Log to file key missing from config file, using default value");
  }

  if (!keysPresent["LogToConsole"]) {
    config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
    Log::Write(
        Log::ERROR,
        "Log to console key missing from config file, using default value");
  }

  if (!keysPresent["ResetLogFile"]) {
    config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
    Log::Write(
        Log::ERROR,
        "Reset log file key missing from config file, using default value");
  }

  file.close();

  return config;
}

std::string ApplicationSettings::trim(const std::string &str) {
  size_t first = str.find_first_not_of(' ');
  if (first == std::string::npos) {
    return "";
  }

  size_t last = str.find_last_not_of(' ');

  return str.substr(first, last - first + 1);
}

bool ApplicationSettings::isNumber(const std::string &str) {
  return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool ApplicationSettings::isBoolean(const std::string &str) {
  return str == "true" || str == "false";
}

bool ApplicationSettings::isQuotedString(const std::string &str) {
  return str.size() >= 2 && str.front() == '"' && str.back() == '"';
}
