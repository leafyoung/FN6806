#include <iostream>
using namespace std;

#include "oper.h"

int main() {
  auto d = Value<int>{4};
  cout << (-d)() << '\n';
  auto e1 = d + d * d + Value<int>{5} / d;
  cout << d() << '\n';
  cout << e1() << '\n';

  auto x = Value<double>{3.2};
  auto y = Value<double>{-2.3};

  // works fine!
  auto z = x + Value<double>{0};
  cout << z() << '\n';

  // works fine!
  z = x + y;
  cout << z() << '\n';

  // works fine!
  auto w = z + y;
  cout << w() << '\n';

  auto z2 = z + x;
  cout << z2() << '\n';

  Value<float> u;
  u = d * Value<int>{2} + d * d;
  cout << u() << '\n';
}