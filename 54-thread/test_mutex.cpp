#include <future>
#include <iostream>
#include <thread>
using std::cout;
using std::mutex;

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

void test_mutex() {
  cout << "test_mutex:\n";
  {
    cout << "use mutex:\n";
    std::thread th1(print_block, 50, '*', true);
    std::thread th2(print_block, 50, '$', true);
    th1.join();
    th2.join();
  }

  {
    cout << "not use mutex:\n";
    std::thread th1(print_block, 50, '*', false);
    std::thread th2(print_block, 50, '$', false);
    th1.join();
    th2.join();
  }
  cout << "\n";
}