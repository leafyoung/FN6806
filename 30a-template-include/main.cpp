#include "template_int.h"

#include <iostream>

int main() {
  std::cout << std::boolalpha;

  A<int> a(3);

  A<double> b(3.14);

  A<bool> c(true);
  if (42) {
    std::cout << 1 << '\n';
  }
  if (!0) {
    std::cout << 0 << '\n';
  }

  std::cout << a.a << '\n';
  std::cout << b.a << '\n';
  std::cout << c.a << '\n';
}