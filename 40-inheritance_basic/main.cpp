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
    cout << c << endl;          // derived class' member
    cout << C::c << endl;       // base class' member
    cout << get_c() << endl;    // derived class' member
    cout << C::get_c() << endl; // base class' member
  }
};

/* ============================================== */

struct A {
  A() { cout << "A" << endl; }
  ~A() { cout << "~A" << endl; }
};

struct B : public A {
  B() { cout << "B" << endl; }
  ~B() { cout << "~B" << endl; }
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
         << K::data_public << "," << data_public << endl;
  }
};

/* ============================================== */

int main() {
  {
    cout << "Test CPP: " << endl;
    CPP cpp;
    cpp.print();
    cout << endl;
  }
  {
    cout << "Test B: " << endl;
    B b; // initialize
  }      // destroy
  cout << endl;

  {
    cout << "Test Ksub: " << endl;
    Ksub kk;
    kk.print();
    kk.access_private(30);
    kk.print();
    cout << endl;
  }
  {
    cout << "Test ZCB: " << endl;
    ZCB z{100, 0.01};
    cout << z.npv(1.5) << endl;
    CouponBond cb{100, 0.01, 0.005};
    cout << cb.npv(1.5) << endl;
    cout << endl;
  }
  {
    cout << "Test Interpolator: " << endl;
    LinearInterpolator linint({1.0, 2.0}, {1.0, 2.0});
    cout << linint.interpolate(1.5) << endl;

    SquaredInterpolator sqint({1.0, 2.0}, {1.0, 2.0});
    cout << sqint.interpolate(1.5) << endl;
    cout << endl;
  }
}