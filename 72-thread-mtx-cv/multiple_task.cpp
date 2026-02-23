#include "multiple_task.h"

void test_multiple_task() {
  std::cout << "=== test multiple task without wait ===\n";
  { MultipleTask mt(true); }

  std::cout << "=== test multiple task without wait but signal exit ===\n";
  {
    MultipleTask mt(true);
    mt.signal_exit();
  }

  std::cout << "=== test multiple task with wait ===\n";
  {
    MultipleTask mt(false);
    mt.signal_exit();
  }

  {
    MultipleTask mt(false);

    for (auto sleep_time_in_ms : {0, 90, 10}) {
      mt.add(
          [sleep_time_in_ms]() {
            std::cout << "[MT] waited when exit: " << sleep_time_in_ms << "\n";
          },
          sleep_time_in_ms);
    }
  }
}