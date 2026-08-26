#include <iostream>
using namespace std;

#include "oper.h"

int main() {
  auto d = Value<int>{4};
  cout << (-d)() << endl;
  auto e1 = d + d * d + Value<int>{5} / d;
  cout << d() << endl;
  cout << e1() << endl;

  auto x = Value<double>{3.2};
  auto y = Value<double>{-2.3};

  // works fine!
  auto z = x + Value<double>{0};
  cout << z() << endl;

  // works fine!
  z = x + y;
  cout << z() << endl;

  // works fine!
  auto w = z + y;
  cout << w() << endl;

  auto z2 = z + x;
  cout << z2() << endl;

  Value<float> u;
  u = d * Value<int>{2} + d * d;
  cout << u() << endl;
}