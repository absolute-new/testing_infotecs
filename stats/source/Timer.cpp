#include "Timer.hpp"

#include <iostream>

namespace Stats {

Timer::Timer(std::chrono::seconds interval, std::function<void()> callback)
    : running_(false), interval_(interval), callback_(callback) {}

Timer::~Timer(void) { stop(); }

void Timer::start() {
  if (running_.load()) return;
  if (!callback_) {
    std::cerr << "Warning: Timer started with empty callback" << std::endl;
    return;
  }
  running_ = true;
  timerThread_ = std::thread([this]() {
    while (running_.load()) {
      for (int i = 0; i < interval_.count() * 10; ++i) {
        if (!running_.load()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      if (!running_.load()) break;
      if (callback_) callback_();
    }
  });
}

void Timer::stop() {
  running_ = false;
  if (timerThread_.joinable()) timerThread_.join();
}

}  // namespace Stats