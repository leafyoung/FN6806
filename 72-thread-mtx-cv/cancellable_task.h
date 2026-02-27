#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "task.h"

class CancellableTask : public Task {
  static const bool DEBUG = false;

private:
  bool wait_to_finish;
  std::thread t;
  std::mutex cancel_signal_mtx;
  std::condition_variable cv;
  bool cancel_signal = false;
  bool done = false;

public:
  CancellableTask(std::function<void(void)> task, size_t sleep_in_ms,
                  bool wait_to_finish)
      : Task(task, sleep_in_ms), wait_to_finish(wait_to_finish) {

    t = std::thread([this]() {
      std::unique_lock<std::mutex> lock(this->cancel_signal_mtx);
      if (DEBUG)
        std::cout << "locked\n";
      // check condition till reaching wake_time
      // returns cancel_signal when finished waiting
      // cv.wait_until unlocks, get locked when done.
      if (cv.wait_until(lock, this->wake_time,
                        [this]() { return this->cancel_signal; })) {
        // early stopped
        if (DEBUG) {
          const auto diff = std::chrono::steady_clock::now() - this->start_time;
          const auto diff_ms =
              std::chrono::duration_cast<std::chrono::milliseconds>(diff)
                  .count();
          std::cout << "stopped early: " << diff_ms << '\n';
        }
        return;
      }
      this->task();
      this->done = true;
      if (DEBUG) {
        const auto diff = std::chrono::steady_clock::now() - this->start_time;
        const auto diff_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        std::cout << "executed with wait: " << diff_ms << '\n';
      }
    });
  }

  void cancel() {
    // locks to avoid another calling and sync the reading of cancel_signal
    std::lock_guard<std::mutex> lock(this->cancel_signal_mtx);
    if (DEBUG)
      std::cout << "try enter\n";

    this->cancel_signal = true;
    cv.notify_one();
  }

  ~CancellableTask() {
    {
      // Read `done` under the mutex: the thread writes `done = true` while
      // holding cancel_signal_mtx (it is still locked after wait_until
      // returns). Reading it here without the lock would be a data race.
      std::lock_guard<std::mutex> lock(this->cancel_signal_mtx);
      if (done) {
        if (DEBUG)
          std::cout << "done when exit!\n";
      } else {
        if (!wait_to_finish) {
          if (DEBUG)
            std::cout << "cancelling\n";
          // cancel_signal_mtx is already held; signal directly to avoid
          // the double-lock that cancel() would cause.
          this->cancel_signal = true;
          cv.notify_one();
        }
      }
    } // release lock before join

    // Always join to ensure thread cleanup
    if (t.joinable()) {
      t.join();
    }
  }
};

void test_cancellable_task();