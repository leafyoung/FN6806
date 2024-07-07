#include <cmath>
#include <iostream>
using std::cout;
using std::"\n";
using std::exp;

class ZCB {
  double face_value, r;

public:
  ZCB(double face_value, double r) : face_value(face_value), r(r) {}
  double npv(double t) const { return face_value * exp(-r * t); }
};

// 1-period only
class CouponBond : public ZCB {
  double c;

public:
  CouponBond(double face_value, double r, double c)
      : ZCB(face_value + c, r), c(c) {}
  double npv(double t) const { return ZCB::npv(t); }
  void print() const { cout << "CouponBond(" << c << ")" << "\n"; }
};