#include "task.h"

void test_task() {
  std::cout << "=== test manual task ===\n";

  size_t result{0};

  auto t = Task(
      [&result]() {
        result++;
        std::cout << "Task is done!\n";
      },
      100);
  t.task();
  std::cout << "result: " << result << '\n';
  if (result != 1)
    throw std::logic_error("task is not executed");
}