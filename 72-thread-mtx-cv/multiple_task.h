#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "task.h"

// Use one timer to count for multipler stops.

// Start
// wake me in 10 seconds.
// wake me in 3 seconds.
// wake me in 2 seconds.
// wake me in 5 seconds.

// After 4 seconds
// wake me in 6 seconds - updated
// wake me in 1 seconds - updated
// wake me in 15 seconds - new
// wake me in 4 seconds - new

class MultipleTask {
  static const bool DEBUG = true;
  std::vector<std::unique_ptr<Task>> tasks;
  std::vector<std::unique_ptr<std::thread>> sub_tasks;

  std::thread t;
  std::mutex mtx;
  std::condition_variable cv;
  bool wait_to_finish;
  bool should_update_timer = false;
  bool can_exit = false;
  bool done = false;

public:
  MultipleTask(bool wait_to_finish) : wait_to_finish(wait_to_finish) {
    this->t = std::thread([this]() { this->cycle(); });
  }

private:
  void cycle() {
    // start cycle

    while (true) {
      std::unique_lock<std::mutex> lock(mtx);
      if (this->tasks.empty()) {
        while (this->tasks.empty() && !this->can_exit) {
          if (DEBUG)
            std::cout << "wait for task\n";
          cv.wait(lock);
        }
        if (this->can_exit)
          return;
      }

      auto &first_task = *(this->tasks.back());
      auto first_task_start_time = first_task.start_time;
      if (cv.wait_until(lock, first_task.wake_time,
                        [this]() { return this->should_update_timer; })) {
        // restart new timer
        this->should_update_timer = false;
        continue;
      }

      auto task_to_run = first_task.task;
      this->tasks.pop_back();

      sub_tasks.emplace_back(
          std::make_unique<std::thread>([task_to_run]() { task_to_run(); }));

      if (DEBUG) {
        const auto diff =
            std::chrono::steady_clock::now() - first_task_start_time;
        const auto diff_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        std::cout << "waited: " << diff_ms << '\n';
      }
    }
  }

public:
  void add(std::function<void(void)> task, size_t sleep_in_ms) {
    std::lock_guard<std::mutex> lock(mtx);
    this->tasks.emplace_back(std::make_unique<Task>(task, sleep_in_ms));
    if (tasks.back()->wake_time < tasks.front()->wake_time) {
      // need to check is in processing and not yet run.
      // lock ensures this
      // need to notify to cancel current.
      this->should_update_timer = true;
    }
    std::sort(tasks.begin(), tasks.end(), [](const auto &t1, const auto &t2) {
      return t1->wake_time > t2->wake_time;
    });

    if (DEBUG)
      std::cout << "added task with sleep: " << sleep_in_ms << "\n";

    for (const auto &t : tasks) {
      if (DEBUG) {
        std::cout << "task: " << *t << '\n';
      }
    }
    cv.notify_one();
  }

  void signal_exit() {
    if (DEBUG)
      std::cout << "try exit when empty\n";

    std::lock_guard<std::mutex> lock(mtx);
    this->can_exit = true;
    cv.notify_one();
  }

  ~MultipleTask() {
    if (done) {
      if (DEBUG)
        std::cout << "done when exit!\n";
    } else {
      if (!wait_to_finish) {
        if (DEBUG)
          std::cout << "cancelling\n";
        signal_exit();
      }
    }

    // Always join to ensure thread cleanup
    if (t.joinable()) {
      signal_exit();
      t.join();
    }

    for (auto &t : this->sub_tasks) {
      if (t->joinable()) {
        t->join();
      }
    }
  }
};

void test_multiple_task();