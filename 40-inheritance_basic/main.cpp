// https://replit.com/@YeKunlun/40-inheritancebasic

#include "interpolator.h"
#include "zcb.h"
#include <iostream>
using namespace std;

/* ============================================== */

struct C {
  int c{0};
  int get_c() { return c; }
};

struct CPP : public C {
  int c{1};
  int get_c() { return c; }
  void print() {
    cout << c << "\n";          // derived class' member
    cout << C::c << "\n";       // base class' member
    cout << get_c() << "\n";    // derived class' member
    cout << C::get_c() << "\n"; // base class' member
  }
};

/* ============================================== */

struct A {
  A() { cout << "A" << "\n"; }
  ~A() { cout << "~A" << "\n"; }
};

struct B : public A {
  B() { cout << "B" << "\n"; }
  ~B() { cout << "~B" << "\n"; }
};

/* ============================================== */

class K {
  int data_private;

protected:
  int data_protected;

public:
  int data_public;
  K() : data_private(1), data_protected(2), data_public(3) {}
  int access_private() const { return data_private; }
  void access_private(int v) { data_private = v; }
};

class Ksub : public K { // 1. use public inheritance
                        // 2. Override the base class' member
                        // Base's member can be accessed by K::data_public
  int data_public;

public:
  // 3. Delegate to base class' ctor (preferred)
  // There are no access to data_private anyway.
  Ksub() : K(), data_public(4) {}
  void print() {
    // 4. Access private member in base class via member function
    cout << access_private() << "," << data_protected
         << ","
         // Access base class' member with the same name
         << K::data_public << "," << data_public << "\n";
  }
};

/* ============================================== */

int main() {
  {
    cout << "Test CPP: " << "\n";
    CPP cpp;
    cpp.print();
    cout << "\n";
  }
  {
    cout << "Test B: " << "\n";
    B b; // initialize
  } // destroy
  cout << "\n";

  {
    cout << "Test Ksub: " << "\n";
    Ksub kk;
    kk.print();
    kk.access_private(30);
    kk.print();
    cout << "\n";
  }
  {
    cout << "Test ZCB: " << "\n";
    ZCB z{100, 0.01};
    cout << z.npv(1.5) << "\n";
    CouponBond cb{100, 0.01, 0.005};
    cout << cb.npv(1.5) << "\n";
    cout << "\n";
  }
  {
    cout << "Test Interpolator: " << "\n";
    LinearInterpolator linint({1.0, 2.0}, {1.0, 2.0});
    cout << linint.interpolate(1.5) << "\n";

    SquaredInterpolator sqint({1.0, 2.0}, {1.0, 2.0});
    cout << sqint.interpolate(1.5) << "\n";
    cout << "\n";
  }
}