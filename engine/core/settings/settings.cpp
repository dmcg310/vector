#include "settings.h"

std::string boolToString(bool value) { return value ? "true" : "false"; }

bool stringToBool(const std::string_view &str) { return str == "true"; }

std::string ApplicationSettings::GetBaseDirectory() {
  std::filesystem::path executablePath = std::filesystem::current_path();
  std::filesystem::path basePath = executablePath;

  // Traverse upwards to find the 'vector' directory
  while (basePath.has_parent_path() && basePath.filename() != "vector") {
    basePath = basePath.parent_path();
  }

  if (basePath.filename() == "vector") {
    return basePath.string();
  } else {
    // Should never happen unless the folder is renamed
    Log::Write(Log::ERROR, "Base directory not found");
    return "";
  }
}

std::string ApplicationSettings::GetAbsoluteLogFilePath() {
  std::string baseDirectory = GetBaseDirectory();
  std::filesystem::path logFilePath =
          std::filesystem::path(baseDirectory) / "logs" / "application.log";

  return logFilePath.string();
}

std::string ApplicationSettings::GetAbsoluteConfigFilePath() {
  std::string baseDirectory = GetBaseDirectory();
  std::filesystem::path configFilePath =
          std::filesystem::path(baseDirectory) / "config.vector";

  return configFilePath.string();
}

ApplicationSettings::ApplicationSettings() {
  config.log.logFilePath = GetAbsoluteLogFilePath();
}

bool ApplicationSettings::CheckIfConfigExists() {
#ifdef __APPLE__
  namespace fs = std::__fs::filesystem;
#else
  namespace fs = std::filesystem;
#endif

  if (std::string configFilePath = GetAbsoluteConfigFilePath();
      !fs::exists(configFilePath)) {
    return false;
  }

  return true;
}

ApplicationSettings::Config ApplicationSettings::CreateConfig() {
  Config _config;

  std::string configFilePath = GetAbsoluteConfigFilePath();
  std::ofstream createFile(configFilePath);
  if (!createFile) { Log::Write(Log::ERROR, "Error creating config file"); }

  createFile.close();

  std::ofstream configFile;
  std::ios_base::openmode mode = std::ios::out;
  configFile.open(configFilePath, mode);

  std::ostringstream oss;
  oss << "[Window]" << "\nWidth=" << DEFAULT_WIDTH << "\nHeight=" << DEFAULT_HEIGHT
      << "\nWindowedFullscreen=" << boolToString(DEFAULT_WINDOWED_FULLSCREEN)
      << "\nBorderlessFullscreen=" << boolToString(DEFAULT_BORDERLESS_FULLSCREEN)
      << "\nFullscreen=" << boolToString(DEFAULT_FULLSCREEN) << "\nTitle=\""
      << DEFAULT_TITLE << "\"" << "\n\n[Log]"
      << "\nLogToFile=" << boolToString(DEFAULT_LOG_TO_FILE)
      << "\nLogToConsole=" << boolToString(DEFAULT_LOG_TO_CONSOLE)
      << "\nResetLogFile=" << boolToString(DEFAULT_RESET_LOG_FILE);
  std::string configString = oss.str();

  configFile << configString;

  _config.window.width = DEFAULT_WIDTH;
  _config.window.height = DEFAULT_HEIGHT;
  _config.window.fullscreen = DEFAULT_FULLSCREEN;
  _config.window.windowedFullscreen = DEFAULT_WINDOWED_FULLSCREEN;
  _config.window.borderlessFullscreen = DEFAULT_BORDERLESS_FULLSCREEN;
  _config.window.title = DEFAULT_TITLE;

  _config.log.logToFile = DEFAULT_LOG_TO_FILE;
  _config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
  _config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
  _config.log.logFilePath = GetAbsoluteLogFilePath();

  return _config;
}

ApplicationSettings::Config ApplicationSettings::LoadConfig() {
  Config _config;

  std::string configFilePath = GetAbsoluteConfigFilePath();
  std::ifstream file(configFilePath);
  if (!file.is_open()) {
    Log::Write(Log::ERROR, "Error reading config file, creating one with defaults");

    return ApplicationSettings::CreateConfig();
  }

  std::string line;
  std::string currentSection;
  std::unordered_map<std::string, bool> keysPresent;

  bool windowSectionExists = false;
  bool logSectionExists = false;

  while (std::getline(file, line)) {
    if (line.empty()) { continue; }

    if (line[0] == '[' && line.back() == ']') {
      currentSection = line.substr(1, line.size() - 2);
    } else {
      std::istringstream lineStream(line);
      std::string key;
      std::string value;

      if (std::getline(lineStream, key, '=')) {
        key = trim(key);

        if (!std::getline(lineStream, value)) { value = ""; }

        value = trim(value);

        keysPresent[key] = true;

        if (value.empty()) {
          Log::Write(Log::ERROR, "Empty value found in config file, using default");
        }

        if (currentSection == "Window") {
          windowSectionExists = true;

          if (key == "Width") {
            if (value.empty() || !isNumber(value)) {
              Log::Write(Log::ERROR,
                         "Invalid window width value in config file, using default");

              _config.window.width = DEFAULT_WIDTH;
            } else {
              _config.window.width = std::stoi(value);
            }

          } else if (key == "Height") {
            if (value.empty() || !isNumber(value)) {
              Log::Write(Log::ERROR,
                         "Invalid window height value in config file, using default");

              _config.window.height = DEFAULT_HEIGHT;
            } else {
              _config.window.height = std::stoi(value);
            }

          } else if (key == "WindowedFullscreen") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(Log::ERROR, "Invalid window windowed fullscreen value in "
                                     "config file, using default");

              _config.window.windowedFullscreen = DEFAULT_WINDOWED_FULLSCREEN;
            } else {
              _config.window.windowedFullscreen = stringToBool(value);
            }

          } else if (key == "BorderlessFullscreen") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(Log::ERROR, "Invalid window borderless fullscreen value in "
                                     "config file, using default");

              _config.window.borderlessFullscreen = DEFAULT_BORDERLESS_FULLSCREEN;
            } else {
              _config.window.borderlessFullscreen = stringToBool(value);
            }

          } else if (key == "Fullscreen") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(Log::ERROR, "Invalid window fullscreen value in "
                                     "config file, using default");

              _config.window.fullscreen = DEFAULT_FULLSCREEN;
            } else {
              _config.window.fullscreen = stringToBool(value);
            }

          } else if (key == "Title") {
            if (value.empty() || !isQuotedString(value)) {
              Log::Write(Log::ERROR,
                         "Invalid window title value in config file, using default");

              _config.window.title = DEFAULT_TITLE;
            } else {
              _config.window.title = value.substr(1, value.size() - 2);
            }
          }
        } else if (currentSection == "Log") {
          logSectionExists = true;
          if (key == "LogToFile") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(Log::ERROR,
                         "Invalid log to file value in config file, using default");

              _config.log.logToFile = DEFAULT_LOG_TO_FILE;
            } else {
              _config.log.logToFile = stringToBool(value);
            }

          } else if (key == "LogToConsole") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(Log::ERROR,
                         "Invalid log to console value in config file, using default");

              _config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
            } else {
              _config.log.logToConsole = stringToBool(value);
            }

          } else if (key == "ResetLogFile") {
            if (value.empty() || !isBoolean(value)) {
              Log::Write(Log::ERROR,
                         "Invalid reset log file value in config file, using default");

              _config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
            } else {
              _config.log.resetLogFile = stringToBool(value);
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

    _config.log.logFilePath = GetAbsoluteLogFilePath();
  }

  if (!windowSectionExists) {
    Log::Write(Log::ERROR,
               "Window section was not found in config file, using default values");

    _config.window.width = DEFAULT_WIDTH;
    keysPresent["Width"] = true;

    _config.window.height = DEFAULT_HEIGHT;
    keysPresent["Height"] = true;

    _config.window.windowedFullscreen = DEFAULT_WINDOWED_FULLSCREEN;
    keysPresent["WindowedFullscreen"] = true;

    _config.window.borderlessFullscreen = DEFAULT_BORDERLESS_FULLSCREEN;
    keysPresent["BorderlessFullscreen"] = true;

    _config.window.fullscreen = DEFAULT_FULLSCREEN;
    keysPresent["Fullscreen"] = true;

    _config.window.title = DEFAULT_TITLE;
    keysPresent["Title"] = true;
  }

  if (!logSectionExists) {
    Log::Write(Log::ERROR,
               "Log section was not found in config file, using default values");

    _config.log.logToFile = DEFAULT_LOG_TO_FILE;
    keysPresent["LogToFile"] = true;

    _config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
    keysPresent["LogToConsole"] = true;

    _config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
    keysPresent["ResetLogFile"] = true;

    _config.log.logFilePath = GetAbsoluteLogFilePath();
    keysPresent["LogFilePath"] = true;
  }

  if (!keysPresent["Width"]) {
    _config.window.width = DEFAULT_WIDTH;
    Log::Write(Log::ERROR, "Width key missing from config file, using default value");
  }

  if (!keysPresent["Height"]) {
    _config.window.height = DEFAULT_HEIGHT;
    Log::Write(Log::ERROR, "Height key missing from config file, using default value");
  }

  if (!keysPresent["WindowedFullscreen"]) {
    _config.window.windowedFullscreen = DEFAULT_WINDOWED_FULLSCREEN;
    Log::Write(Log::ERROR,
               "Windowed fullscreen key missing from config file, using default value");
  }

  if (!keysPresent["BorderlessFullscreen"]) {
    _config.window.borderlessFullscreen = DEFAULT_BORDERLESS_FULLSCREEN;
    Log::Write(Log::ERROR,
               "Borderless fullscreen key missing from config file, using default value");
  }

  if (!keysPresent["Fullscreen"]) {
    _config.window.fullscreen = DEFAULT_FULLSCREEN;
    Log::Write(Log::ERROR,
               "Fullscreen key missing from config file, using default value");
  }

  if (!keysPresent["Title"]) {
    _config.window.title = DEFAULT_TITLE;
    Log::Write(Log::ERROR, "Title key missing from config file, using default value");
  }

  if (!keysPresent["LogToFile"]) {
    _config.log.logToFile = DEFAULT_LOG_TO_FILE;
    Log::Write(Log::ERROR,
               "Log to file key missing from config file, using default value");
  }

  if (!keysPresent["LogToConsole"]) {
    _config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
    Log::Write(Log::ERROR,
               "Log to console key missing from config file, using default value");
  }

  if (!keysPresent["ResetLogFile"]) {
    _config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
    Log::Write(Log::ERROR,
               "Reset log file key missing from config file, using default value");
  }

  file.close();

  return _config;
}

std::string ApplicationSettings::trim(const std::string &str) {
  size_t first = str.find_first_not_of(' ');
  if (first == std::string::npos) { return ""; }

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
