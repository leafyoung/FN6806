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

    // ++acnt uses the default memory_order_seq_cst. For a plain counter,
    // relaxed order is sufficient: acnt.fetch_add(1, std::memory_order_relaxed);
    ++acnt;

    // Release order: a thread that later reads acnt_release with an acquire
    // load is guaranteed to see everything written before this store. There is
    // no acquire load here; join() already synchronizes the final read.
    // https://en.cppreference.com/w/cpp/atomic/memory_order.html#Release-Acquire_ordering
    acnt_release.fetch_add(1, std::memory_order_release);
  }
}

int main() {
  {
    vector<std::thread> pool;
    for (int n = 0; n < 100; ++n)
      pool.emplace_back(f);

    for (auto& f : pool)
      f.join();
  }

  std::cout << "The non-atomic counter is " << cnt << '\n'
            << "The atomic counter (seq_cst) is " << acnt << '\n'
            << "The atomic counter (release) is " << acnt_release << '\n';
}