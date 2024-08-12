#include <iostream>
using std::cout;

#include "interpolator.h"

void test_interpolator() {
  cout << "Test Interpolator: \n";
  LinearInterpolator linint({1.0, 2.0}, {1.0, 2.0});
  cout << linint(1.5) << "\n";
}