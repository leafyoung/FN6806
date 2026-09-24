#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "task.h"

// Use one timer to count for multiple stops.
class MultipleTask {
  static const bool DEBUG = true;
  std::vector<std::unique_ptr<Task>> tasks;
  std::vector<std::unique_ptr<std::thread>> sub_tasks;

  std::thread t;
  std::mutex mtx;
  std::condition_variable cv;
  bool should_update_timer = false;
  bool can_exit = false;

  // in dtor, shall wait to finish or not
  bool wait_to_finish;

public:
  MultipleTask(bool wait_to_finish) : wait_to_finish(wait_to_finish) {
    this->t = std::thread([this]() { this->cycle(); });
  }

private:
  void cycle() {
    // start cycle
    while (true) {
      std::unique_lock<std::mutex> lock(mtx);
      while (this->tasks.empty() && !this->can_exit) {
        if (DEBUG)
          std::cout << "wait for task\n";
        cv.wait(lock);
      }
      if (this->tasks.empty() && this->can_exit)
        return;

      const auto first_wake_time = this->tasks.back()->wake_time;
      if (cv.wait_until(lock, first_wake_time,
                        [this]() { return this->should_update_timer; })) {
        // restart new timer
        this->should_update_timer = false;
        continue;
      }

      // Run the task that is at the back now (earliest wake time after sort).
      auto task_to_run = this->tasks.back()->task;
      auto first_task_start_time = this->tasks.back()->start_time;
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
    if (tasks.empty()) {
      this->tasks.emplace_back(std::make_unique<Task>(task, sleep_in_ms));
      this->should_update_timer = true;
    } else {
      auto prev_wake_time = tasks.back()->wake_time;
      this->tasks.emplace_back(std::make_unique<Task>(task, sleep_in_ms));
      auto updated_wake_time = tasks.back()->wake_time;
      if (updated_wake_time < prev_wake_time) {
        // The new task wakes earlier than the one the timer is waiting for.
        // lock ensures this
        // need to notify to cancel current.
        this->should_update_timer = true;
      }
      std::sort(tasks.begin(), tasks.end(), [](const auto &t1, const auto &t2) {
        return t1->wake_time > t2->wake_time;
      });
    }
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
    std::lock_guard<std::mutex> lock(mtx);
    if (DEBUG)
      std::cout << "try exit\n";
    this->can_exit = true;
    cv.notify_one();
  }

  ~MultipleTask() {
    // Signal the cycle thread to stop and wait for it to exit.
    // This MUST happen before touching sub_tasks: the cycle thread holds mtx
    // while pushing to sub_tasks in cycle(), so iterating sub_tasks
    // before t.join() would be a data race even with wait_to_finish = true.
    signal_exit();

    if (t.joinable()) {
      t.join();
    }

    // The cycle thread is fully done; sub_tasks is now frozen and safe to
    // iterate without a lock. Every sub-thread must be joined or detached
    // before its std::thread is destroyed, or std::terminate() is called.
    for (auto &st : this->sub_tasks) {
      if (st->joinable()) {
        if (wait_to_finish)
          st->join();
        else
          st->detach(); // detached tasks must not capture locals by reference
      }
    }
  }
};

void test_multiple_task();