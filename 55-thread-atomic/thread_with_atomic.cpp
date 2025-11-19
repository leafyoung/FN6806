#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

std::atomic_int acnt;
std::atomic_int acnt_release;
int cnt;

void f() {
  for (auto n{100000}; n; --n) {
    // race condition
    ++cnt;

    // Note: for this example, relaxed memory order is sufficient,
    // e.g. acnt.fetch_add(1, std::memory_order_relaxed);
    ++acnt;

    //  That is, once the atomic load is completed, thread B is guaranteed to
    //  see everything thread A wrote to memory.
    // https://en.cppreference.com/w/cpp/atomic/memory_order.html#Release-Acquire_ordering
    acnt_release.fetch_add(1, std::memory_order_release);
  }
}

int main() {
  {
    vector<std::thread> pool;
    for (int n = 0; n < 100; ++n)
      pool.emplace_back(f);

    for (auto &f : pool)
      f.join();
  }

  std::cout

      << "The non-atomic counter is " << cnt << '\n'
      << "The atomic counter (relaxedx) is " << acnt << '\n'
      << "The atomic counter (acquire-release sync) is " << acnt_release
      << '\n';
  ;
}