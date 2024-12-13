// https://replit.com/@YeKunlun/12-classpoint

#include <iomanip>
#include <iostream>
using namespace std;

#include "Point.h"
#include "PointWithNDC.h"
#include "PointWithStr.h"

void f(PointWithNDC p) { cout << "called f(PointWithNDC p)\n"; }

int main() {
  cout << boolalpha;
  {
    Point p(3, 4);
    Point p2{p};
    p2 = p;
    cout << "p2 == p: " << (p2 == p) << "\n";
  }

  {
    Point p(3, 4); // 1. constructor
    auto p2{p};    // 2. copy constructor
    if (p2 != p) { // comparison operator!=
      p2 = p;      // 3. copy assignment operator
      cout << "p2 != p: not equal\n";
    } else {
      cout << "p2 == p: equal\n";
    }
    Point p3(3, 5);
    cout << "p3 == p: " << (p3 == p) << "\n\n"; // comparison operator==
  } // 4. p and p2 are out of scope, destroyed

  {
    cout << "PointWithStrNoCtor\n";
    PointWithStrNoCtor p;
    cout << p.get_x() << ", " << p.get_y() << ", " << p.get_x() + p.get_y()
         << p.get_name() << "\n";

    for (auto v : p.get_vd()) {
      cout << v << ", ";
    }
    cout << "\n";
  }

  {
    cout << "PointWithStr(...)\n";
    PointWithStr p(42, 43, "a point", {1, 2});
    cout << p.get_x() << ", " << p.get_y() << ", " << p.get_name() << "\n";

    for (auto v : p.get_vd()) {
      cout << v << ", ";
    }
    cout << "\n";
  }

  {
    cout << "PointWithStr(empty)\n";
    PointWithStr p;
    cout << p.get_x() << ", " << p.get_y() << ", " << p.get_name() << "\n";

    for (auto v : p.get_vd()) {
      cout << v << ", ";
    }
    cout << "\n";
  }
  {
    // f(1); // implicit conversion, compiler error
    // Below will work with or without explicit copy ctor.
    f(PointWithNDC(1)); // also, explicit conversion
    auto x = static_cast<PointWithNDC>(13);
    cout << x.get_x() << "\n";           // int => PointWithNDC
    cout << static_cast<int>(x) << "\n"; // PointWithNDC => int
  }
}