// https://replit.com/@YeKunlun/3e-weakptr

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

#include "interlock.h"
#include "nolock.h"

int main() {

  {
    cout << "object is alive" << endl;
    auto x = make_shared<int>(3);
    weak_ptr<int> p = x;
    cout << "x.use_count: " << p.use_count() << endl;
    if (auto spt = p.lock()) { // try get a share_ptr from weak_ptr
      cout << "x use_count: " << spt.use_count() << endl;
      cout << "value" << *spt << endl;
    }
  }
  {
    cout << "shared_ptr is no longer alive" << endl;
    auto x = make_shared<int>(3);
    weak_ptr<int> p = x;
    cout << "x.use_count: " << p.use_count() << endl;
    // de-allocate shared_ptr
    x.reset();
    if (auto spt = p.lock()) { // try get a share_ptr from weak_ptr
      cout << "x use_count: " << spt.use_count() << endl;
      cout << "value" << *spt << endl;
    }
  }

  {
    using namespace InterLock;
    while (true)
      test_lock();
  }

  {
    using namespace NoLock;
    while (true)
      test_lock();
  }
}