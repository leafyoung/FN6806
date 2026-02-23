#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <stdexcept>

struct Task {
public:
  const std::function<void(void)> task;
  const std::chrono::milliseconds sleep_in_milliseconds;
  const std::chrono::steady_clock::time_point start_time, wake_time;

  Task(std::function<void(void)> task, std::size_t sleep_in_ms)
      : task(task),
        sleep_in_milliseconds(std::chrono::milliseconds(sleep_in_ms)),
        start_time(std::chrono::steady_clock::now()),
        wake_time(std::chrono::steady_clock::now() + sleep_in_milliseconds) {

    if (sleep_in_milliseconds.count() < 0)
      throw std::logic_error("sleep_in_ms shall be > 0");
  }

  friend std::ostream &operator<<(std::ostream &out, const Task &task) {
    out << "sleep: " << task.sleep_in_milliseconds.count() << ", time range: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(
               task.start_time.time_since_epoch())
               .count()
        << '-'
        << std::chrono::duration_cast<std::chrono::milliseconds>(
               task.wake_time.time_since_epoch())
               .count();

    return out;
  }
};

void test_task();