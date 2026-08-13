#include "SocketLogger.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sstream>

namespace Logger {

SocketLogger::SocketLogger(std::string const &address, int port,
                           LogLevel defaultLevel)
    : socketFd_(-1), minLevel_(defaultLevel) {
  if (!connectToServer(address, port))
    std::cerr << "Error: cannot connect to " << address << ":" << port
              << std::endl;
}

SocketLogger::~SocketLogger(void) {
  if (socketFd_ != -1) {
    while (close(socketFd_) == -1) {
      if (errno != EINTR) {
        std::cerr << "Error: close failed" << std::endl;
        break;
      }
    }
  }
}

void SocketLogger::setLevel(LogLevel level) { minLevel_ = level; }

bool SocketLogger::log(std::string const &message, LogLevel level) {
  std::string logMessage;
  ssize_t bytesSent;

  if (level < minLevel_) return (true);
  if (socketFd_ == -1) {
    std::cerr << "Error: socket is not connected" << std::endl;
    return (false);
  }
  logMessage = formatLogMessage(message, level);
  std::lock_guard<std::mutex> lock(mtx_);
  bytesSent = send(socketFd_, logMessage.c_str(), logMessage.size(), 0);
  if (bytesSent == -1) {
    std::cerr << "Error: send failed" << std::endl;
    return (false);
  }
  if (static_cast<size_t>(bytesSent) != logMessage.size()) {
    std::cerr << "Warning: only " << bytesSent << " bytes sent" << std::endl;
    return (false);
  }
  return (true);
}

bool SocketLogger::log(std::string const &message) {
  return (log(message, minLevel_));
}

std::string_view SocketLogger::getStrLogLevel(LogLevel level) const {
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

bool SocketLogger::tryConnect(addrinfo *p, int port) {
  socketFd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
  if (socketFd_ == -1) return (false);
  if (p->ai_family == AF_INET)
    ((sockaddr_in *)p->ai_addr)->sin_port = htons(port);
  else if (p->ai_family == AF_INET6)
    ((sockaddr_in6 *)p->ai_addr)->sin6_port = htons(port);
  if (connect(socketFd_, p->ai_addr, p->ai_addrlen) == 0) return (true);
  close(socketFd_);
  socketFd_ = -1;
  return (false);
}

bool SocketLogger::connectToServer(std::string const &address, int port) {
  auto [addresses, res] = getAddresses(address);
  for (auto p : addresses) {
    if (tryConnect(p, port)) {
      freeaddrinfo(res);
      return (true);
    }
  }
  freeaddrinfo(res);
  return (false);
}

bool SocketLogger::isReady(void) const { return (socketFd_ != -1); }

std::string const SocketLogger::formatLogMessage(std::string const &message,
                                                 LogLevel level) const {
  std::string timeStr;
  std::string levelStr;
  std::ostringstream oss;

  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  timeStr = std::ctime(&time_t);
  timeStr.pop_back();
  levelStr = getStrLogLevel(level);
  oss << timeStr << " [" << levelStr << "]: " << message + "\n";
  return (oss.str());
}

std::pair<std::vector<addrinfo *>, addrinfo *> SocketLogger::getAddresses(
    std::string const &address) {
  addrinfo hints;
  addrinfo *res;
  std::vector<addrinfo *> result;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if (getaddrinfo(address.c_str(), nullptr, &hints, &res) != 0) {
    std::cerr << "Error: cannot resolve address" << std::endl;
    return {};
  }
  for (addrinfo *p = res; p != nullptr; p = p->ai_next) {
    result.push_back(p);
  }
  return {result, res};
}

}  // namespace Logger
