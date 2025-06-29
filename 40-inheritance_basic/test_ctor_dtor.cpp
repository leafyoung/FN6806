#include <iostream>
using std::cout;

struct A {
  A() { cout << "A "; }
  ~A() { cout << "~A "; }
};

struct B : public A {
  B() { cout << "B "; }
  ~B() { cout << "~B "; }
};

void test_ctor_dtor() {
  cout << "Test B: \n";
  {
    B b; // initialize

  } // destroy
  cout << '\n';
}