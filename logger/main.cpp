#include <iostream>

#include "Config.hpp"
#include "LoggerClient.hpp"
#include "LoggerConfig.hpp"

static void printUsage(char const *progName) {
  std::cerr << "Usage: " << progName << " <log_file> <log_level> [--socket]"
            << std::endl;
  std::cerr << "Log levels: INFO, DEBUG, ERROR" << std::endl;
}

static bool isSocketMode(int ac, char *av[]) {
  if (ac == 4) {
    if (std::string(av[3]) == "--socket") return (true);
    std::cerr << "Error: unknown flag '" << av[3] << "'" << std::endl;
    printUsage(av[0]);
    exit(1);
  }
  return (false);
}

static bool parseArgs(int ac, char *av[]) {
  if (ac < 3 || ac > 4) {
    printUsage(av[0]);
    return (false);
  }
  return (true);
}

static Logger::LoggerConfig createConfig(int ac, char *av[]) {
  Logger::LoggerConfig appConfig;

  appConfig.useSocket = isSocketMode(ac, av);
  appConfig.logFile = av[1];
  if (appConfig.useSocket) {
    Logger::Config fileConfig;
    if (!fileConfig.load("logger.conf"))
      std::cerr << "Warning: cannot load logger.conf, using defaults"
                << std::endl;
    appConfig.address = fileConfig.get("address", "127.0.0.1");
    appConfig.port = fileConfig.getInt("port", 9000);
  }
  auto level = Logger::LoggerClient::parseLogLevel(av[2]);
  if (!level.has_value()) {
    std::cerr << "Error: invalid log level: " << av[2] << std::endl;
    exit(1);
  }
  appConfig.level = level.value();
  return (appConfig);
}

int main(int ac, char *av[]) {
  Logger::LoggerConfig appConfig;

  if (!parseArgs(ac, av)) return (1);
  appConfig = createConfig(ac, av);
  Logger::LoggerClient client(appConfig);
  if (!client.isReady()) return (1);
  client.run();
  return (0);
}