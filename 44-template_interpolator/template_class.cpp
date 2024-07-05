#include <iostream>
using namespace std;

template <typename X> struct X2 { X x; };

struct X3 : public X2<int> {
  int x3() { return x; }
};

template <typename X> struct X4 : public X2<X> {
  X x4() { return this->x; }
};

int main2() {
  {
    X3 x3;
    cout << x3.x3() << "\\n";

    X4<int> x4;
    cout << x4.x4() << "\\n";
  }
  return 0;
}