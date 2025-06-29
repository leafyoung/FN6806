#include <iostream>
#include <memory>
#include <vector>
using namespace std;

namespace InterLock {
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
  shared_ptr<A> pointer; // Cross-reference
  // weak_ptr<A> pointer; // weak_ptr avoids

  ~B() = default;
};

void test_lock() {
  auto a = make_shared<A>();
  auto b = make_shared<B>();
  a->pointer = b; // B is used in A and b
  cout << a.use_count() << ", " << b.use_count() << '\n';
  b->pointer = a; // A is used in B and a
  cout << b->pointer.use_count() << '\n';

  // When we use shared_ptr
  cout << b->pointer->value[9] << ": " << a.use_count() << ", " << b.use_count()
       << '\n';
  // a: 2, 1; b: 2, 1

  // When we use weak_ptr
  // cout << b->pointer.lock()->value[9] << '\n';
}
} // namespace InterLock
