// https://replit.com/@YeKunlun/46-virtualdtor

#include <iostream>
#include <memory>

using namespace std;

// Make dctor as virtual as long as you need to use the pointer of base class to
// access the instance of derived class.

namespace virtual_dtor {
class A {
public:
  virtual ~A() { cout << "~A()\n"; };
};

class B {
public:
  // virtual is a must here.
  virtual ~B() {}
};

class C : public B {
  A a;
};
} // namespace virtual_dtor

namespace virtual_function {
class A {
public:
  virtual void a() { std::cout << "a" << std::endl; }
};

class B : public A {
public:
  virtual void a() { std::cout << "b" << std::endl; }
};
} // namespace virtual_function

int main() {
  {
    using namespace virtual_dtor;
    cout << "Test cases for Virtual Destructor\n";

    cout << "Case 1\n";
    // With the pointer type the same as the actual type
    // correct destructor will be selected always.
    C *c = new C;
    delete c;

    cout << "Case 2\n";
    B *b = new C;
    // if we don't make ~B as virtual, b will be destructed as class B.
    // a is not destructed so no print of "A::~A()"
    delete b;

    // unique_ptr will automatically call delete when object is destroyed.
    // RAII
    cout << "Case 3\n";
    unique_ptr<B> b1{make_unique<C>()};

    cout << "Case 4\n";
    unique_ptr<C> c1{make_unique<C>()};
    cout << "====\n";
  } // b1 and c1 were destructed here.

  {
    using namespace virtual_function;

    cout << "Test cases for Virtual Function with Reference\n";

    std::cout << "From value\n";

    A a;
    a.a();

    B b;
    b.a();

    std::cout << "From reference\n";
    A &a_ref = a;
    // static typing makes the binding during compilation.
    // need virtual to make dynamic lookup during runtime.
    A &b_ref = b;
    B &b_ref2 = b;

    a_ref.a();
    b_ref.a();
    b_ref2.a();
    cout << "====\n";
  }

  return 0;
}