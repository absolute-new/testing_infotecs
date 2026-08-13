#include "LoggerClient.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <unordered_map>

#include "FileLogger.hpp"
#include "SocketLogger.hpp"

namespace Logger {

LoggerClient::LoggerClient(LoggerConfig const &config)
    : config_(config), running_(true), ready_(true) {
  if (config.useSocket)
    Logger_ = std::make_unique<SocketLogger>(config_.address, config_.port,
                                             config.level);
  else
    Logger_ = std::make_unique<FileLogger>(config.logFile, config.level);
  if (!Logger_->isReady()) ready_ = false;
  writeThrd_ = std::thread(&LoggerClient::writeLoop, this);
}

LoggerClient::~LoggerClient(void) {
  running_ = false;
  queueCV_.notify_all();
  if (writeThrd_.joinable()) writeThrd_.join();
}

void LoggerClient::run(void) {
  std::string message;
  std::string levelInput;
  static std::string const separator(50, '-');
  LogEntry logEntry;

  while (1) {
    std::cout << "Enter log message (or 'exit' to quit):> ";

    if (!std::getline(std::cin, message)) break;
    if (isExit(message)) break;
    std::cout << "Enter log level (or press Enter for default) ["
              << logLevelToString(config_.level) << "]> ";

    if (!std::getline(std::cin, levelInput)) break;
    if (levelInput.empty())
      logEntry.level = config_.level;
    else {
      auto level = parseLogLevel(levelInput);
      if (level.has_value())
        logEntry.level = level.value();
      else {
        std::cerr << "Invalid log level. Using default level: "
                  << logLevelToString(config_.level) << std::endl;
        logEntry.level = config_.level;
      }
    }
    logEntry.message = message;
    {
      std::lock_guard<std::mutex> lock(queueMtx_);
      messageQ_.push(logEntry);
    }
    queueCV_.notify_one();
    std::cout << separator << std::endl;
  }
}

bool LoggerClient::isExit(std::string const &input) const {
  std::string lower;

  lower = input;
  std::transform(lower.begin(), lower.end(), lower.begin(),
                 [](unsigned char ch) { return (std::tolower(ch)); });
  return (lower == "exit");
}

std::optional<LogLevel> LoggerClient::parseLogLevel(std::string const &level) {
  std::string upper;

  upper = level;
  std::transform(upper.begin(), upper.end(), upper.begin(),
                 [](unsigned char ch) { return (std::toupper(ch)); });
  static const std::unordered_map<std::string, LogLevel> levelMap = {
      {"INFO", LogLevel::INFO},
      {"DEBUG", LogLevel::DEBUG},
      {"ERROR", LogLevel::ERROR}};
  auto it = levelMap.find(upper);
  if (it == levelMap.end()) return (std::nullopt);
  return (it->second);
}

std::string_view LoggerClient::logLevelToString(LogLevel level) const {
  switch (level) {
    case LogLevel::INFO:
      return ("INFO");
    case LogLevel::DEBUG:
      return ("DEBUG");
    case LogLevel::ERROR:
      return ("ERROR");
    default:
      return ("UNKNOWN");
  }
}

void LoggerClient::writeLoop(void) {
  LogEntry logEntry;

  while (running_ || !messageQ_.empty()) {
    {
      std::unique_lock<std::mutex> lock(queueMtx_);
      queueCV_.wait(
          lock, [this]() { return (!messageQ_.empty() || !running_.load()); });
      if (!running_.load() && messageQ_.empty()) break;
      logEntry = messageQ_.front();
      messageQ_.pop();
    }
    if (!Logger_->log(logEntry.message, logEntry.level))
      std::cerr << "Error: failed to log message" << std::endl;
  }
}

bool LoggerClient::isReady() const { return (ready_); }

}  // namespace Logger