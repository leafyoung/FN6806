#include <iostream>
#include <memory>
#include <vector>
using namespace std;

namespace NoLock {

// Forward declaration
// It allows to use B type for the use of pointer in class A.
struct A;
struct B;

struct A {
  vector<int> value;
  // Without forward declaration
  // this line cannot compile
  shared_ptr<B> pointer;
  A() : value(100'000'000, 42) {} // to speed up the memory leak
  ~A() = default;
};

struct B {
  weak_ptr<A> pointer; // weak_ptr avoids

  ~B() = default;
};

void test_lock() {
  auto a = make_shared<A>();
  auto b = make_shared<B>();
  // use debugger to observe for _M_refcount -> _M_use_count and _M_weak_count
  // a: 1, 1; b: 2, 1
  a->pointer = b; // B is used in A and b
  // a: 1, 2; b: 2, 1
  b->pointer = a; // A is used in B and a

  // When we use shared_ptr
  // cout << b->pointer->value[9] << "\n";
  // When we use weak_ptr
  // The use_count() returns total count including the use of weak_ptr.
  // A: has two use_count:
  // with lock, a: 2, 2; b: 2, 1
  cout << b->pointer.lock()->value[9] << ": " << a.use_count() << ", "
       << b.use_count() << "\n";
  // after lock, a: 1, 2; b: 2, 1
}
} // namespace NoLock
