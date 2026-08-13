#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "SocketLogger.hpp"
#include "StatsServer.hpp"

TEST(IntegrationTest, LoggerSendsToStats) {
  uint64_t total;

  total = 0;
  {
    Stats::StatsServer server(8002, 5, 1);
    std::thread serverThread([&server]() { server.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Logger::SocketLogger client("127.0.0.1", 8002, Logger::LogLevel::DEBUG);
    client.log("Hello World!");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    server.stop();
    serverThread.join();
    total = server.getStats().totalMessage;
  }
  EXPECT_EQ(total, 1);
}

TEST(IntegrationTest, MultipleMessages) {
  uint64_t total;

  total = 0;
  {
    Stats::StatsServer server(8001, 5, 1);
    std::thread serverThread([&server]() { server.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Logger::SocketLogger client("127.0.0.1", 8001, Logger::LogLevel::DEBUG);
    client.log("Message 1");
    client.log("Message 2");
    client.log("Message 3");
    client.log("Message 4");
    client.log("Message 5");
    server.stop();
    serverThread.join();
    total = server.getStats().totalMessage;
  }
  EXPECT_EQ(total, 5);
}

TEST(IntegrationTest, DifferentLevels) {
  uint64_t info;
  uint64_t debug;
  uint64_t error;

  info = 0;
  debug = 0;
  error = 0;
  {
    Stats::StatsServer server(8003, 5, 1);
    std::thread serverThread([&server]() { server.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Logger::SocketLogger client("127.0.0.1", 8003, Logger::LogLevel::DEBUG);
    client.log("Info", Logger::LogLevel::INFO);
    client.log("Debug", Logger::LogLevel::DEBUG);
    client.log("Error", Logger::LogLevel::ERROR);
    server.stop();
    serverThread.join();
    info = server.getStats().infoCount;
    debug = server.getStats().debugCount;
    error = server.getStats().errorCount;
    EXPECT_TRUE(client.isReady());
  }
  EXPECT_EQ(info, 0);
  EXPECT_EQ(debug, 1);
  EXPECT_EQ(error, 1);
}

TEST(IntegrationTest, LogInfoLevel) {
  Stats::StatsServer server(8005, 5, 1);
  std::thread serverThread([&server]() { server.run(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto Logger = std::make_unique<Logger::SocketLogger>("127.0.0.1", 8005,
                                                       Logger::LogLevel::INFO);
  Logger->log("Info message", Logger::LogLevel::INFO);
  server.stop();
  serverThread.join();
  EXPECT_EQ(server.getStats().infoCount, 1);
}

TEST(IntegrationTest, LogUnknownLevel) {
  Stats::StatsServer server(8006, 5, 1);
  std::thread serverThread([&server]() { server.run(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto logger = std::make_unique<Logger::SocketLogger>("127.0.0.1", 8006,
                                                       Logger::LogLevel::DEBUG);
  logger->log("Unknown", static_cast<Logger::LogLevel>(100));
  server.stop();
  serverThread.join();
  EXPECT_EQ(server.getStats().infoCount, 0);
  EXPECT_EQ(server.getStats().debugCount, 0);
  EXPECT_EQ(server.getStats().errorCount, 0);
}

TEST(SocketLoggerTest, SetLevel) {
  auto Logger = std::make_unique<Logger::SocketLogger>("127.0.0.1", 8000,
                                                       Logger::LogLevel::DEBUG);
  Logger->setLevel(Logger::LogLevel::ERROR);
  EXPECT_FALSE(Logger->isReady());
}

TEST(SocketLoggerTest, InvalidAddress) {
  auto logger = std::make_unique<Logger::SocketLogger>("invalid.host", 8000,
                                                       Logger::LogLevel::DEBUG);
  EXPECT_FALSE(logger->isReady());
}

TEST(SocketLoggerTest, InitSocketFails) {
  Stats::StatsServer server(99999999, 5, 1);
}
