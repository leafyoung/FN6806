#include <future>
#include <iostream>
#include <vector>
using std::cout;
using std::launch;

void test_async() {
  cout << "test_async:\n";

  // Basic deferred / async
  {
    cout << "Basic deferred / async:\n";
    // deferred: runs lazily on the calling thread at get()/wait()
    auto x = std::async(launch::deferred, [] { return 7; });
    cout << "deferred: " << x.get() << '\n';
    // async: runs on a new thread right away
    auto y = std::async(launch::async, [] { return 9; });
    cout << "async: " << y.get() << '\n';
  }

  // Multiple futures: parallel sum (the std::async pattern from OOP2
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