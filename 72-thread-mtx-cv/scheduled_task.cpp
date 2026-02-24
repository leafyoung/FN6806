
#include "scheduled_task.h"
#include <vector>

void test_scheduled_task() {
  std::cout << "=== test scheduled task ===\n";
  size_t result{0};
  {
    std::vector<std::unique_ptr<ScheduledTask>> tasks;

    for (auto sleep_time_in_ms : {0, 1, 3, 7}) {
      tasks.emplace_back(std::make_unique<ScheduledTask>(
          [sleep_time_in_ms, &result]() {
            result += sleep_time_in_ms;
            std::cout << "[ScheduledTask]: " << sleep_time_in_ms << "\n";
          },
          sleep_time_in_ms));
    }
  }
  std::cout << "result: " << result << "\n";
  if (result != 11)
    throw std::logic_error("scheduled task is not executed");
}