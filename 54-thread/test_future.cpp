#include <future>
#include <iostream>

using std::cout;
using std::future;
using std::packaged_task;
using std::thread;

void test_future() {
  cout << "test_future:\n";
  // 1. wrap the function
  packaged_task<int()> task([] { return 7; });
  // 2. get a future
  future<int> f1 = task.get_future();
  // 3. launch on a thread
  thread t(std::move(task));

  f1.wait(); // wait for the task to finish}
  cout << f1.get() << '\n';

  if (t.joinable())
    t.join();
  cout << '\n';
}
