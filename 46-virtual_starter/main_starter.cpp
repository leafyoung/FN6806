// https://github.com/leafyoung/FN6806/tree/main/46-virtual

#include <iostream>
#include <memory>

using namespace std;

// Make dtor virtual as long as you need to use the pointer of base class to
// access the instance of derived class.

namespace virtual_dtor {
class A {
public:
  virtual ~A() { cout << "~A()\n"; };
};

class B {
public:
  // virtual is a must here.
  virtual ~B() { cout << "~B()\n"; }
};

class C : public B {
public:
  A a;
};
} // namespace virtual_dtor

int main() {
  {
    using namespace virtual_dtor;
    cout << "Test cases for Virtual Destructor\n";

    auto a = make_unique<A>();
    auto b = make_unique<B>();
    auto c = make_unique<C>();
    [[maybe_unused]] auto d = c->a;

    c.reset();

    cout << "====\n";
  }

  return 0;
}