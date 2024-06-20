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
      << "\nTitle=" << DEFAULT_TITLE << "\n\n[Log]"
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

  // Add to WriteLog queue that a config wasnt provided and was made with
  // defaults

  return config;
}

ApplicationSettings::Config ApplicationSettings::LoadConfig() {
  Config config;

  std::ifstream file(CONFIG_FILE_PATH);
  if (!file.is_open()) {
    std::cout << "Error reading file"
              << std::endl; // Add to WriteLog queue that config couldnt be
                            // opened so made one with defaults

    config = ApplicationSettings::CreateConfig();
    return config;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Parse the config file
    std::cout << line << std::endl;
  }

  file.close();

  return config;
}
