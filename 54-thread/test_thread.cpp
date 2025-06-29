#include <iostream>
#include <thread>

using std::cout;
using std::thread;

void func1() { std::cout << "thread func without params\n"; }

void func2(int const i, double const d, std::string const s) {
  std::cout << i << ", " << d << ", " << s << '\n';
}

void func3(int &i) { i *= 2; }

void test_thread() {
  cout << "test_thread:\n";
  cout << "CPU: " << thread::hardware_concurrency() << '\n';
  {
    std::thread t(func1);
    // try either .join() or .detach()
    // if (t.joinable()) t.join();
    t.detach();
  } // must call t.join()/t.detach() before `t` goes out of scope, crash
    // otherwise
  {
    int i = 0;
    std::thread t;
    t = thread([&i]() {
      std::cout << "thread func without params using lambda\n";
      i += 1001;
    });
    if (t.joinable())
      t.join();
    cout << "After thread: " << i << '\n';
  }

  {
    std::thread t(func2, 42, 42.0, "42");
    if (t.joinable())
      t.join();
  }

  {
    int n = 42;
    std::thread t(func3, std::ref(n));
    if (t.joinable())
      t.join();
    std::cout << "After func3: " << n << '\n'; // 84
  }
  cout << '\n';
}