#include "StatsServer.hpp"

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <iostream>

namespace Stats {

StatsServer::StatsServer(int port, int n, int timeoutSec)
    : port_(port), n_(n), timeoutSec_(timeoutSec), running_(true) {
  if (port < 1 || port > 65535) {
    std::cerr << "Error: invalid port: " << port << std::endl;
    return;
  }
  if (!initSocket()) {
    running_ = false;
    return;
  }
  startTimer();
}

StatsServer::~StatsServer(void) {
  if (timer_) timer_->stop();
  if (serverSocket_ != -1) {
    while (close(serverSocket_) == -1) {
      if (errno != EINTR) {
        std::cerr << "Error: close failed" << std::endl;
        break;
      }
    }
  }
}

void StatsServer::run(void) {
  int clientSocket;

  std::cout << "Listening on port " << port_ << "..." << std::endl;
  while (running_.load()) {
    clientSocket = acceptClient();
    if (clientSocket == -1) {
      continue;
    }
    handleClient(clientSocket);
  }
}

void StatsServer::printStats(void) const {
  std::cout << "\n---------- STATISTICS ----------\n";
  std::cout << "Total messages:\t" << stats_.totalMessage << std::endl;
  std::cout << "INFO:\t\t" << stats_.infoCount << std::endl;
  std::cout << "DEBUG:\t\t" << stats_.debugCount << std::endl;
  std::cout << "ERROR:\t\t" << stats_.errorCount << std::endl;
  std::cout << "Last hour:\t" << stats_.lastHourCount << std::endl;
  std::cout << "Min length:\t" << stats_.minLength << std::endl;
  std::cout << "Max length:\t" << stats_.maxLength << std::endl;
  std::cout << "Avg length:\t" << stats_.avgLength << std::endl;
  std::cout << "--------------------------------" << std::endl;
}

bool StatsServer::initSocket(void) {
  int opt;
  sockaddr_in6 addr6;

  serverSocket_ = socket(AF_INET6, SOCK_STREAM, 0);
  if (serverSocket_ == -1) {
    std::cerr << "Error: cannot create socket" << std::endl;
    return (false);
  }
  opt = 0;
  setsockopt(serverSocket_, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
  opt = 1;
  setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  addr6.sin6_family = AF_INET6;
  addr6.sin6_port = htons(port_);
  addr6.sin6_addr = in6addr_any;
  if (bind(serverSocket_, (sockaddr *)&addr6, sizeof(addr6)) == -1) {
    std::cerr << "Error: cannot bind to port " << port_ << std::endl;
    close(serverSocket_);
    serverSocket_ = -1;
    return (false);
  }
  if (listen(serverSocket_, 1) == -1) {
    std::cerr << "Error: cannot listen on socket" << std::endl;
    close(serverSocket_);
    serverSocket_ = -1;
    return (false);
  }
  return (true);
}

void StatsServer::startTimer(void) {
  timer_ = std::make_unique<Timer>(std::chrono::seconds(timeoutSec_), [this]() {
    if (stats_.totalMessage != stats_.lastTotal) {
      printStats();
      stats_.lastTotal = stats_.totalMessage;
    }
  });
  timer_->start();
}

int StatsServer::acceptClient(void) {
  sockaddr_in6 clientAddr;
  socklen_t clientLen;
  int clientSocket;
  int ret;
  fd_set readfds;

  FD_ZERO(&readfds);
  FD_SET(serverSocket_, &readfds);
  timeval tv = {1, 0};
  ret = select(serverSocket_ + 1, &readfds, nullptr, nullptr, &tv);
  if (ret <= 0) {
    return (-1);
  }
  clientLen = sizeof(clientAddr);
  clientSocket = accept(serverSocket_, (sockaddr *)&clientAddr, &clientLen);
  if (clientSocket == -1) {
    std::cerr << "Error: accept failed" << std::endl;
    return (-1);
  }
  return (clientSocket);
}

void StatsServer::processMessage(std::string const &msg) {
  uint64_t len;

  stats_.totalMessage++;
  if (msg.find("[INFO]") != std::string::npos)
    stats_.infoCount++;
  else if (msg.find("[DEBUG]") != std::string::npos)
    stats_.debugCount++;
  else if (msg.find("[ERROR]") != std::string::npos)
    stats_.errorCount++;
  stats_.lastHourCount++;
  len = static_cast<uint64_t>(msg.length());
  stats_.sumLengths += len;
  stats_.minLength = std::min(stats_.minLength, len);
  stats_.maxLength = std::max(stats_.maxLength, len);
  stats_.avgLength =
      static_cast<double>(stats_.sumLengths) / stats_.totalMessage;
  if (stats_.totalMessage % n_ == 0) printStats();
}

void StatsServer::handleClient(int clientSocket) {
  char buf[1024];
  int bytesRead;

  const timeval tv = {1, 0};
  if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1)
    std::cerr << "Warning: cannot set timeout on client socket" << std::endl;
  while (1) {
    memset(buf, 0, sizeof(buf));
    bytesRead = recv(clientSocket, buf, 1024, 0);
    if (bytesRead <= 0) break;
    std::string data(buf, bytesRead);
    size_t pos = 0;
    while ((pos = data.find('\n')) != std::string::npos) {
      std::string msg = data.substr(0, pos);
      data.erase(0, pos + 1);
      std::cout << "Received:> " << msg << std::endl;
      processMessage(msg);
    }
  }
  close(clientSocket);
}

void StatsServer::stop(void) {
  running_ = false;
  if (timer_) timer_->stop();
}

Stats StatsServer::getStats(void) const { return (stats_); }

}  // namespace Stats