#include "cancellable_task.h"
#include <vector>

using namespace std::chrono_literals;

void test_cancellable_task() {
  std::cout << "=== test cancellable task without cancel ===\n";
  size_t result{0};
  {
    std::vector<std::unique_ptr<CancellableTask>> tasks;

    for (auto sleep_time_in_ms : {0, 1, 3, 7}) {
      tasks.emplace_back(std::make_unique<CancellableTask>(
          [sleep_time_in_ms, &result]() {
            result += sleep_time_in_ms;
            std::cout << "CancellableTask without cancel, waited externally: "
                      << sleep_time_in_ms << "\n";
          },
          sleep_time_in_ms, false));
    }
    // sleep is necessary to let task finish
    std::this_thread::sleep_for(1s);
  }
  std::cout << "result: " << result << "\n";
  if (result != 11)
    throw std::logic_error("cancellable task is not executed");

  std::cout
      << "=== test cancellable task with cancel (running out of scope) ===\n";
  {
    result = 0;

    std::vector<std::unique_ptr<CancellableTask>> tasks;

    for (auto sleep_time_in_ms : {4}) {
      tasks.emplace_back(std::make_unique<CancellableTask>(
          [sleep_time_in_ms, &result]() {
            result += sleep_time_in_ms;
            std::cout << "CancellableTask with cancel, not waited when exit: "
                      << sleep_time_in_ms << "\n";
          },
          // wait_to_finish = false, so it will not wait to finish and
          // exit immediately.
          // You won't see any update.
          sleep_time_in_ms, false));
    }
  }
  std::cout << "result: " << result << "\n";
  if (result != 0)
    throw std::logic_error("cancellable task is not cancelled");

  std::cout << "=== test cancellable task with wait ===\n";
  {
    result = 0;
    std::vector<std::unique_ptr<CancellableTask>> tasks;

    for (auto sleep_time_in_ms : {0, 1, 3, 7}) {
      tasks.emplace_back(std::make_unique<CancellableTask>(
          [sleep_time_in_ms, &result]() {
            result += sleep_time_in_ms;
            std::cout << "CancellableTask with wait " << sleep_time_in_ms
                      << "\n";
          },
          sleep_time_in_ms, true));
    }
  }
  std::cout << "result: " << result << "\n";
  if (result != 11)
    throw std::logic_error("cancellable task is not executed properly");
}