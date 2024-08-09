// https://replit.com/@YeKunlun/40-inheritancebasic

#include "interpolator.h"
#include "zcb.h"
#include <iostream>
using namespace std;

void test_override();
void test_ctor_dtor();
void test_delegation();

/* ============================================== */

int main() {
  test_override();
  test_ctor_dtor();
  test_delegation();

  {
    cout << "Test ZCB: "
         << "\n";
    ZCB z{100, 0.01};
    cout << z.npv(1.5) << "\n";
    CouponBond cb{100, 0.01, 0.005};
    cout << cb.npv(1.5) << "\n";
    cout << "\n";
  }
  {
    cout << "Test Interpolator: "
         << "\n";
    LinearInterpolator linint({1.0, 2.0}, {1.0, 2.0});
    cout << linint.interpolate(1.5) << "\n";

    SquaredInterpolator sqint({1.0, 2.0}, {1.0, 2.0});
    cout << sqint.interpolate(1.5) << "\n";
    cout << "\n";
  }
}