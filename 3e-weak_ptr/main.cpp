// https://replit.com/@YeKunlun/3e-weakptr

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

#include "interlock.h"
#include "nolock.h"

int main() {
  {
    cout << "object is alive"
         << "\n";
    auto x = make_shared<int>(3);
    weak_ptr<int> p = x;
    cout << "x.use_count: " << p.use_count() << "\n";
    if (auto spt = p.lock()) { // try get a share_ptr from weak_ptr
      cout << "x use_count: " << spt.use_count() << "\n";
      cout << "value" << *spt << "\n";
    }
  }
  {
    cout << "shared_ptr is no longer alive"
         << "\n";
    auto x = make_shared<int>(3);
    weak_ptr<int> p = x;
    cout << "x.use_count: " << p.use_count() << "\n";
    // de-allocate shared_ptr
    x.reset();
    if (auto spt = p.lock()) { // try get a share_ptr from weak_ptr
      cout << "x use_count: " << spt.use_count() << "\n";
      cout << "value" << *spt << "\n";
    }
  }

  {
    using namespace NoLock;
    cout << "NoLock:\n";
    for (size_t c = 0; c < 30; ++c)
      test_lock();
  }

  {
    cout << "InterLock (memory leak):\n";
    using namespace InterLock;
    while (true)
      test_lock();
  }
}