#include "FileLogger.hpp"

#include <chrono>
#include <iostream>
#include <sstream>

namespace Logger {

FileLogger::FileLogger(std::string const &filename, LogLevel defaultLevel)
    : file_(filename, std::ios::app), minLevel_(defaultLevel) {
  if (!file_.is_open()) std::cerr << "Error: cannot open log file" << std::endl;
}

void FileLogger::setLevel(LogLevel level) { minLevel_ = level; }

bool FileLogger::log(std::string const &message, LogLevel level) {
  std::string levelStr;
  std::string timeStr;
  std::ostringstream oss;

  if (level < minLevel_) return (true);
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  timeStr = std::ctime(&time_t);
  timeStr.pop_back();
  levelStr = getStrLogLevel(level);
  oss << timeStr << " [" << levelStr << "]: " << message << std::endl;
  std::lock_guard<std::mutex> lock(mtx_);
  if (!file_.is_open()) return (false);
  file_ << oss.str();
  return (file_.good());
}

bool FileLogger::log(std::string const &message) {
  return (log(message, minLevel_));
}

std::string_view FileLogger::getStrLogLevel(LogLevel level) const {
  switch (level) {
    case LogLevel::DEBUG:
      return ("DEBUG");
    case LogLevel::ERROR:
      return ("ERROR");
    case LogLevel::INFO:
      return ("INFO");
    default:
      return ("UNKNOWN");
  }
}

bool FileLogger::isReady(void) const { return (file_.is_open()); }

}  // namespace Logger
