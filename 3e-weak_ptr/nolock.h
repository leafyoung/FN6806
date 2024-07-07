#include <iostream>
#include <memory>
#include <vector>
using namespace std;

namespace NoLock {
// Forward declaration
// allows to use the type for pointer
// without defining it.

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
  a->pointer = b;
  b->pointer = a;

  // When we use shared_ptr
  // cout << b->pointer->value[9] << endl;
  // When we use weak_ptr
  cout << b->pointer.lock()->value[9] << endl;
}
} // namespace NoLock
