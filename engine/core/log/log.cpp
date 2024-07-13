#include "log.h"

std::ofstream Log::logFile;
bool Log::logToFile = true;
bool Log::logToConsole = true;
std::mutex Log::logMutex;
std::condition_variable Log::logCondition;
std::thread Log::logThread;
bool Log::stopLogging = false;
bool Log::initialized = false;
std::queue<std::pair<Log::Level, std::string>> Log::logQueue;
#ifdef _DEBUG
std::deque<std::pair<Log::Level, std::string>> Log::logBuffer;
std::deque<std::string> Log::frameLogBuffer;
#endif

void Log::Initialize(const std::string &filename, bool fileOutput, bool consoleOutput,
                     bool resetLogFile) {
  logToFile = fileOutput;
  logToConsole = consoleOutput;

  if (logToFile) {
    CreateDirectories(filename);

    // Open file in truncate mode if resetLogFile is true, otherwise append mode
    std::ios_base::openmode mode =
            resetLogFile ? std::ios::out : (std::ios::out | std::ios::app);

    logFile.open(filename, mode);
    if (!logFile) { Log::Write(Log::ERROR, "Error opening provided log file"); }
  }

  initialized = true;
  FlushQueue();

  // Start logging thread
  stopLogging = false;
  logThread = std::thread(Log::LogThreadFunction);
}

void Log::Write(Level level, const std::string &message) {
  std::lock_guard<std::mutex> guard(logMutex);

  if (!initialized) {
    // Store logs that happen before the logger has initialized
    logQueue.emplace(level, message);

    return;
  }

  std::ostringstream oss;
  oss << GetCurrentDateTime() << " [" << LevelToString(level) << "]: " << message
      << std::endl;
  std::string logMessage = oss.str();

  if (logToFile && logFile) { logFile << logMessage; }

  if (logToConsole) { std::cout << logMessage; }

#ifdef _DEBUG
  logBuffer.emplace_back(level, logMessage);
  if (logBuffer.size() > 50) { logBuffer.pop_front(); }
#endif
}

void Log::WriteFrameLog(const std::string &message) {
#ifdef _DEBUG
  std::lock_guard<std::mutex> guard(logMutex);

  frameLogBuffer.emplace_back(message);

  if (frameLogBuffer.size() > 50) { frameLogBuffer.pop_front(); }

  logCondition.notify_one();
#endif
}

void Log::Shutdown() {
  {
    std::lock_guard<std::mutex> guard(logMutex);
    stopLogging = true;
  }

  logCondition.notify_all();
  if (logThread.joinable()) { logThread.join(); }

  if (logToFile && logFile) { logFile.close(); }
}

#ifdef _DEBUG
std::deque<std::pair<Log::Level, std::string>> Log::GetLogBuffer() { return logBuffer; }

std::deque<std::string> Log::GetFrameLogBuffer() { return frameLogBuffer; }
#endif

void Log::FlushQueue() {
  while (!logQueue.empty()) {
    auto [level, message] = logQueue.front();
    logQueue.pop();

    Write(level, message);
  }
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

void Log::CreateDirectories(const std::string &filepath) {
#ifdef __APPLE__
  namespace fs = std::__fs::filesystem;
#else
  namespace fs = std::filesystem;
#endif

  fs::path path = filepath;

  path.remove_filename();
  if (!path.empty() && !fs::exists(path)) { fs::create_directories(path); }
}

void Log::LogThreadFunction() {
  while (true) {
    std::unique_lock<std::mutex> lock(logMutex);

    logCondition.wait_for(lock, std::chrono::milliseconds(100),
                          [] { return stopLogging; });

    if (stopLogging) { break; }

    // No need to empty frameLogBuffer here since it's updated directly in the
    // main thread.
  }
}
