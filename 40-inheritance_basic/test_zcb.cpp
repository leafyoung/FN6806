#include <iostream>
using std::cout;

#include "zcb.h"

void test_zcb() {
  cout << "Test ZCB: \n";
  ZCB z{100, 0.01};
  cout << z.npv(1.5) << '\n';
  CouponBond cb{100, 0.01, 0.005};
  cout << cb.npv(1.5) << '\n';
  cout << '\n';
}