#include <signal.h>

#include <iostream>
#include <thread>

#include "StatsServer.hpp"

static volatile sig_atomic_t stop = 0;

static void handleSignal(int) { stop = 1; }

int main(int ac, char *av[]) {
  int port;
  int n;
  int timeout;

  if (ac != 4) {
    std::cerr << "Usage: " << av[0] << " <port> <N> <T>" << std::endl;
    return (1);
  }
  port = std::stoi(av[1]);
  n = std::stoi(av[2]);
  timeout = std::stoi(av[3]);
  signal(SIGINT, handleSignal);
  signal(SIGTERM, handleSignal);
  Stats::StatsServer server(port, n, timeout);
  std::thread serverThread([&server]() { server.run(); });
  while (stop == 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  serverThread.join();

  return (0);
}
