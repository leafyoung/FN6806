#include "multiple_task.h"

using namespace std::chrono_literals;

void test_multiple_task() {
  std::cout << "=== SKIP test multiple task with wait (shall wait forever) and "
               "then signal exit  ===\n";
  {
    MultipleTask mt(true);
    std::cout << "waiting for task, let's wait for 1s before signal_exit";
    std::this_thread::sleep_for(1s);
    mt.signal_exit();
  }

  std::cout << "=== test multiple task without wait ===\n";
  {
    MultipleTask mt(false);
    mt.signal_exit();
  }

  std::cout << "=== test multiple task without wait ===\n";
  { MultipleTask mt(false); }

  size_t result{0};
  std::cout << "=== test multiple task with wait and tasks but non-shall "
               "execute ===\n";
  {
    result = 0;
    MultipleTask mt(true);
    mt.signal_exit();

    // wait for a while so the loop is completely stopped, otherwise the task
    // may execute before signal_exit
    std::this_thread::sleep_for(1s);

    for (auto sleep_time_in_ms : {0, 90, 10}) {
      mt.add(
          [sleep_time_in_ms, &result]() {
            result += sleep_time_in_ms;
            std::cout << "[MT] waited when exit: " << sleep_time_in_ms << "\n";
          },
          sleep_time_in_ms);
    }
  }
  std::cout << "result: " << result << "\n";
  if (result != 0)
    throw std::logic_error("multiple task shall not execute here");

  std::cout << "=== test multiple task with wait and tasks but all shall "
               "execute with signal_exit ===\n";
  {
    result = 0;
    MultipleTask mt(true);

    for (auto sleep_time_in_ms : {0, 90, 10}) {
      mt.add(
          [sleep_time_in_ms, &result]() {
            result += sleep_time_in_ms;
            std::cout << "[MT] waited when exit: " << sleep_time_in_ms << "\n";
          },
          sleep_time_in_ms);
    }
    mt.signal_exit();
  }
  std::cout << "result: " << result << "\n";
  if (result != 100)
    throw std::logic_error("multiple task shall execute here");

  std::cout << "=== test multiple task with wait and tasks but all shall "
               "execute without signal_exit ===\n";
  {
    result = 0;
    MultipleTask mt(true);

    for (auto sleep_time_in_ms : {0, 90, 10}) {
      mt.add(
          [sleep_time_in_ms, &result]() {
            result += sleep_time_in_ms;
            std::cout << "[MT] waited when exit: " << sleep_time_in_ms << "\n";
          },
          sleep_time_in_ms);
    }
    // mt.signal_exit();
  }
  std::cout << "result: " << result << "\n";
  if (result != 100)
    throw std::logic_error("multiple task shall execute here");
}