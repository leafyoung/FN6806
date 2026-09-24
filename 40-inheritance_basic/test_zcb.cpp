#include <iostream>
using std::cout;

#include "zcb.h"

void test_zcb() {
  cout << "Test ZCB: \n";
  ZCB z{100, 0.01};
  cout << z.npv(1.5) << '\n';
  CouponBond cb{100, 0.01, 5}; // coupon amount 5 paid with the face value
  cout << cb.npv(1.5) << '\n';
  cout << '\n';
}