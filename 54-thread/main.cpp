#include <chrono>
#include <future>
#include <iostream>
#include <thread>
using namespace std;

void func1() { std::cout << "thread func without params" << '\n'; }

void func2(int const i, double const d, std::string const s) {
  std::cout << i << ", " << d << ", " << s << '\n';
}

void func3(int &i) { i *= 2; }

std::mutex mtx; // mutex for critical section

void print_block(int n, char c, bool use_guard) {
  // critical section (exclusive access to std::cout signaled by locking mtx):
  if (use_guard) {
    std::lock_guard<std::mutex> guard(mtx);
    for (int i = 0; i < n; ++i) {
      std::cout << c;
      cout.flush();
    }
    std::cout << "\n";
  } else {
    for (int i = 0; i < n; ++i) {
      std::cout << c;
      cout.flush();
    }
    std::cout << "\n";
  }
}

class ForThread2 {
  int input, output;

public:
  ForThread2(int input) : input(input), output(0) {}
  int add(int add) { return output = input + add; }
  int get_output() { return output; }
};

class ForThread {
  inline static int id = 0;
  int input, output;

public:
  ForThread(int input) : input(input), output(0) { id++; }
  int operator()(int add) {
    cout << "id: " << id << "\n";
    output = input + add;
    cout << output << "\n";
    return output;
  }
  int get_output() { return output; }
};

void launch_thread() {
  std::thread t(func1);
  if (t.joinable())
    t.join();
}

int main() {
  {
    std::thread t(func1);
    if (t.joinable())
      t.join();
    // t.detach();
  }
  return 0;
  {
    int i = 0;
    std::thread t;
    t = thread([&i]() {
      // std::cout << "thread func without params using lambda" << '\n';
      i += 1001;
      // cout << i << "\n";
    });

    if (t.joinable())
      t.join();
    cout << i << "\n";
    return 0;
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
    std::cout << n << '\n'; // 84
  }
  { cout << thread::hardware_concurrency() << "\n"; }

  {
    // 1. wrap the function
    std::packaged_task<int()> task([] { return 7; });
    // 2. get a future
    std::future<int> f1 = task.get_future();
    // 3. launch on a thread
    std::thread t(std::move(task));

    f1.wait(); // wait for the task to finish}
    cout << f1.get() << "\n";

    if (t.joinable())
      t.join();
  }

  {
    cout << "ForThread2"
         << "\n";
    ForThread2 for_thread2(3);
    thread t2(&ForThread2::add, &for_thread2, 4);
    t2.join();
    cout << for_thread2.get_output() << "\n";
  }
  /*
{
  cout << "ForThread" << "\n";
  ForThread for_thread(3);
  // It will run a copy of for_thread, can not update for_thread
  thread t(&for_thread, 4);
  t.join();
  cout << "g: " << for_thread.get_output() << "\n";
}
*/

  {
    std::thread th1(print_block, 50, '*', true);
    std::thread th2(print_block, 50, '$', true);

    th1.join();
    th2.join();
  }

  {
    std::thread th1(print_block, 50, '*', false);
    std::thread th2(print_block, 50, '$', false);

    th1.join();
    th2.join();
  }
  {
    auto x = async(launch::deferred, [] { return 7; });
    if (x.wait(), true) {
      cout << x.get();
    }
    auto y = async(launch::async, [] { return 9; });
    if (y.wait(), true) {
      cout << y.get();
    }
    cout << "\n";
  }
}