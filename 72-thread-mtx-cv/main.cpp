// https://replit.com/@YeKunlun/72-thread-mtx-cv

#include <thread>
#include <vector>

#include "cancellable_task.h"
#include "multiple_task.h"
#include "scheduled_task.h"
#include "task.h"

class Timer {
  std::vector<Task> stoppers;
  std::thread t;

 public:
  Timer() {
    stoppers.reserve(1000);
    this->t = std::thread([this]() { this->reeval(); });
  }
  void add(int /* x */) {}

  void reeval() {}
};

int main() {
  test_task();

  test_scheduled_task();

  test_cancellable_task();

  test_multiple_task();

  return 0;
}