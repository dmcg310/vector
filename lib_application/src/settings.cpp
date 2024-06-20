#include "../include/settings.h"

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
    std::cout << "Error creating file"
              << std::endl; // Add to WriteLog queue, FATAL maybe
  }

  createFile.close();

  std::ofstream configFile;
  std::ios_base::openmode mode = std::ios::out;
  configFile.open(CONFIG_FILE_PATH, mode);

  std::ostringstream oss;
  oss << "[Window]" << "\nWidth=" << DEFAULT_WIDTH
      << "\nHeight=" << DEFAULT_HEIGHT << "\nFullscreen=" << DEFAULT_FULLSCREEN
      << "\nTitle=\"" << DEFAULT_TITLE << "\"" << "\n\n[Log]"
      << "\nLogToFile=" << DEFAULT_LOG_TO_FILE
      << "\nLogToConsole=" << DEFAULT_LOG_TO_CONSOLE
      << "\nResetLogFile=" << DEFAULT_RESET_LOG_FILE;
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
    std::cout << "Error reading file"
              << std::endl; // Add to WriteLog queue that config couldn't be
                            // opened so made one with defaults

    config = ApplicationSettings::CreateConfig();
    return config;
  }

  std::string line;
  std::string currentSection;
  std::unordered_map<std::string, bool> keysPresent;

  bool windowSectionExists, logSectionExists;

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
          std::cerr << "Empty value for key: " << key
                    << ". Using default value." << std::endl;
        }

        // Add below errors to WriteLog queue

        if (currentSection == "Window") {
          windowSectionExists = true;

          if (key == "Width") {
            if (value.empty() || !isNumber(value)) {
              std::cerr << "Invalid value for Width: " << value
                        << ". Using default value: " << DEFAULT_WIDTH
                        << std::endl;

              config.window.width = DEFAULT_WIDTH;
            } else {
              config.window.width = std::stoi(value);
            }

          } else if (key == "Height") {
            if (value.empty() || !isNumber(value)) {
              std::cerr << "Invalid value for Height: " << value
                        << ". Using default value: " << DEFAULT_HEIGHT
                        << std::endl;

              config.window.height = DEFAULT_HEIGHT;
            } else {
              config.window.height = std::stoi(value);
            }

          } else if (key == "Fullscreen") {
            if (value.empty() || !isBoolean(value)) {
              std::cerr << "Invalid value for Fullscreen: " << value
                        << ". Using default value: "
                        << (DEFAULT_FULLSCREEN ? "true" : "false") << std::endl;

              config.window.fullscreen = DEFAULT_FULLSCREEN;
            } else {
              config.window.fullscreen = (value == "true");
            }

          } else if (key == "Title") {
            if (value.empty() || !isQuotedString(value)) {
              std::cerr << "Invalid or empty value for Title: " << value
                        << ". Using default value: " << DEFAULT_TITLE
                        << std::endl;

              config.window.title = DEFAULT_TITLE;
            } else {
              config.window.title = value.substr(1, value.size() - 2);
            }
          }
        } else if (currentSection == "Log") {
          logSectionExists = true;

          if (key == "LogToFile") {
            if (value.empty() || !isBoolean(value)) {
              std::cerr << "Invalid value for LogToFile: " << value
                        << ". Using default value: "
                        << (DEFAULT_LOG_TO_FILE ? "true" : "false")
                        << std::endl;

              config.log.logToFile = DEFAULT_LOG_TO_FILE;
            } else {
              config.log.logToFile = (value == "true");
            }

          } else if (key == "LogToConsole") {
            if (value.empty() || !isBoolean(value)) {
              std::cerr << "Invalid value for LogToConsole: " << value
                        << ". Using default value: "
                        << (DEFAULT_LOG_TO_CONSOLE ? "true" : "false")
                        << std::endl;

              config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
            } else {
              config.log.logToConsole = (value == "true");
            }

          } else if (key == "ResetLogFile") {
            if (value.empty() || !isBoolean(value)) {
              std::cerr << "Invalid value for ResetLogFile: " << value
                        << ". Using default value: "
                        << (DEFAULT_RESET_LOG_FILE ? "true" : "false")
                        << std::endl;

              config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
            } else {
              config.log.resetLogFile = (value == "true");
            }
          } else {
            std::cout << "Key not found" << key << "Ignoring" << std::endl;
          }
        }

        config.log.logFilePath = LOG_FILE_PATH;
      } else {
        std::cerr << "Invalid line: " << line << std::endl;
      }
    }
  }

  if (!windowSectionExists) {
    std::cerr << "Window section header was not found, using default values."
              << std::endl;

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
    std::cerr << "Log section header was not found, using default values."
              << std::endl;

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
    std::cerr << "Width key missing. Using default value: " << DEFAULT_WIDTH
              << std::endl;
  }

  if (!keysPresent["Height"]) {
    config.window.height = DEFAULT_HEIGHT;
    std::cerr << "Height key missing. Using default value: " << DEFAULT_HEIGHT
              << std::endl;
  }

  if (!keysPresent["Fullscreen"]) {
    config.window.fullscreen = DEFAULT_FULLSCREEN;
    std::cerr << "Fullscreen key missing. Using default value: "
              << (DEFAULT_FULLSCREEN ? "true" : "false") << std::endl;
  }

  if (!keysPresent["Title"]) {
    config.window.title = DEFAULT_TITLE;
    std::cerr << "Title key missing. Using default value: " << DEFAULT_TITLE
              << std::endl;
  }

  if (!keysPresent["LogToFile"]) {
    config.log.logToFile = DEFAULT_LOG_TO_FILE;
    std::cerr << "LogToFile key missing. Using default value: "
              << (DEFAULT_LOG_TO_FILE ? "true" : "false") << std::endl;
  }

  if (!keysPresent["LogToConsole"]) {
    config.log.logToConsole = DEFAULT_LOG_TO_CONSOLE;
    std::cerr << "LogToConsole key missing. Using default value: "
              << (DEFAULT_LOG_TO_CONSOLE ? "true" : "false") << std::endl;
  }

  if (!keysPresent["ResetLogFile"]) {
    config.log.resetLogFile = DEFAULT_RESET_LOG_FILE;
    std::cerr << "ResetLogFile key missing. Using default value: "
              << (DEFAULT_RESET_LOG_FILE ? "true" : "false") << std::endl;
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
