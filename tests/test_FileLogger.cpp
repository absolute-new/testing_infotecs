#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <string>

#include "FileLogger.hpp"

#define LOGFILE "test.log"

TEST(FileLoggerTest, LogsMessage) {
  std::string line;
  bool found;
  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::DEBUG);
    Logger->log("Hello");
  }
  std::ifstream file(LOGFILE);
  found = false;
  while (std::getline(file, line)) {
    if (line.find("Hello") != std::string::npos) found = true;
  }
  std::remove(LOGFILE);
  EXPECT_TRUE(found);
}

TEST(FileLoggerTest, FiltersByLevel) {
  std::string line;
  bool foundNotAppear;
  bool foundAppear;

  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::DEBUG);
    Logger->log("Should not appear", Logger::LogLevel::INFO);
    Logger->log("Should appear", Logger::LogLevel::ERROR);
  }

  std::ifstream file(LOGFILE);
  foundAppear = false;
  foundNotAppear = false;
  while (std::getline(file, line)) {
    if (line.find(" Should not appear") != std::string::npos)
      std::cout << "  FOUND 'Should appear' at pos "
                << line.find("Should appear") << std::endl;
    if (line.find("Should appear") != std::string::npos) foundAppear = true;
  }
  std::remove(LOGFILE);
  EXPECT_TRUE(foundAppear);
  EXPECT_FALSE(foundNotAppear);
}

TEST(FileLoggerTest, SetLevelWorks) {
  std::string line;
  bool foundNotAppear;
  bool foundAppear;

  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::DEBUG);
    Logger->setLevel(Logger::LogLevel::ERROR);
    Logger->log("Should not appear", Logger::LogLevel::INFO);
    Logger->log("Should appear", Logger::LogLevel::ERROR);
  }
  std::ifstream file(LOGFILE);
  foundAppear = false;
  foundNotAppear = false;
  while (std::getline(file, line)) {
    if (line.find("Should not appear") != std::string::npos)
      foundNotAppear = true;
    if (line.find("Should appear") != std::string::npos) foundAppear = true;
  }
  std::remove(LOGFILE);
  EXPECT_TRUE(foundAppear);
  ;
  EXPECT_FALSE(foundNotAppear);
}

TEST(FileLoggerTest, IsReadyReturns) {
  auto Logger =
      std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::DEBUG);
  EXPECT_TRUE(Logger->isReady());
  std::remove(LOGFILE);
}

TEST(FileLoggerTest, LogsInfoLevel) {
  std::string line;
  bool found;

  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::INFO);
    Logger->log("Info message", Logger::LogLevel::INFO);
  }
  std::ifstream file(LOGFILE);
  found = false;
  while (std::getline(file, line)) {
    if (line.find("Info message") != std::string::npos) found = true;
  }
  std::remove(LOGFILE);
  EXPECT_TRUE(found);
}

TEST(FileLoggerTest, UnknownLevel) {
  std::string line;
  bool found;

  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::INFO);
    Logger->log("Should be unknown", static_cast<Logger::LogLevel>(100));
  }
  std::ifstream file(LOGFILE);
  found = false;
  while (std::getline(file, line)) {
    if (line.find("UNKNOWN") != std::string::npos) found = true;
  }
  std::remove(LOGFILE);
  EXPECT_TRUE(found);
}

TEST(FileLoggerTest, LevelBelowMin) {
  std::string line;
  bool found;

  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::ERROR);
    Logger->log("Should not appear", Logger::LogLevel::DEBUG);
  }
  std::ifstream file(LOGFILE);
  found = false;
  while (std::getline(file, line)) {
    if (line.find("Should not appear") != std::string::npos) found = true;
  }
  std::remove(LOGFILE);
  EXPECT_FALSE(found);
}

TEST(FileLoggerTest, EmptyMessage) {
  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::ERROR);
    Logger->log("");
  }
  std::ifstream file(LOGFILE);
  file.seekg(0, std::ios::end);
  std::remove(LOGFILE);
  EXPECT_TRUE(file.tellg() > 0);
}

TEST(FileLoggerTest, LongMessage) {
  std::string line;
  bool found;

  std::string longMessage(100000, 'a');
  {
    auto Logger =
        std::make_unique<Logger::FileLogger>(LOGFILE, Logger::LogLevel::ERROR);
    Logger->log(longMessage);
  }
  std::ifstream file(LOGFILE);
  found = false;
  while (std::getline(file, line)) {
    if (line.find(longMessage.substr(0, 50)) != std::string::npos) found = true;
  }
  std::remove(LOGFILE);
  EXPECT_TRUE(found);
}