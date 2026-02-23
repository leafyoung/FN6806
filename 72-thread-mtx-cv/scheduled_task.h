#pragma once

#include <functional>
#include <iostream>
#include <thread>

#include "task.h"

class ScheduledTask : public Task {
  static const auto DEBUG = false;
  std::thread t;

public:
  ScheduledTask(std::function<void(void)> task, size_t sleep_in_ms)
      : Task(task, sleep_in_ms) {
    t = std::thread([this]() {
      std::this_thread::sleep_for(this->sleep_in_milliseconds);
      this->task();
      if (DEBUG) {
        const auto diff = std::chrono::steady_clock::now() - this->start_time;
        const auto diff_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        std::cout << "waited: " << diff_ms << '\n';
      }
    });
  }

  ~ScheduledTask() { t.join(); }
};

void test_scheduled_task();