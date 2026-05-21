#include <future>
#include <iostream>
#include <vector>
using std::cout;
using std::launch;

void test_async() {
  cout << "test_async:\n";

  // Basic deferred / async
  {
    cout << "Basic deferred:\n";
    auto x = std::async(launch::deferred, [] { return 7; });
    if (x.wait(), true) {
      cout << x.get();
    }
    auto y = std::async(launch::async, [] { return 9; });
    if (y.wait(), true) {
      cout << y.get();
    }
    cout << '\n';
  }

  // Multiple futures: parallel sum (demonstrates the N6 pattern from OOP2
  // Lec05). Each task returns a partial sum; the caller collects via
  // future.get(). Any exception thrown inside a task is re-thrown by
  // future.get().
  {
    cout << "Multiple futures:\n";

    const int n_threads = 4;
    const int n_per_thread = 25;
    std::vector<std::future<int>> futures;
    futures.reserve(n_threads);

    for (int t = 0; t < n_threads; ++t) {
      int start = t * n_per_thread + 1;
      futures.push_back(std::async(launch::async, [start]() {
        int sum = 0;
        for (int i = start; i < start + n_per_thread; ++i)
          sum += i;
        return sum;
      }));
    }

    int total = 0;
    for (auto &f : futures)
      total += f.get(); // collect; exceptions propagate here
    cout << "sum(1..100) = " << total << '\n'; // 5050
  }
  cout << '\n';
}