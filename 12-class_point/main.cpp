// https://replit.com/@YeKunlun/12-classpoint

#include <iostream>
using namespace std;

#include "Point.h"
#include "PointWithStr.h"

class Rectangle {
  double width;
  double height;
  mutable double _area;
  mutable double _perimeter;
  mutable bool b_area{false};
  mutable bool b_perimeter{false};

public:
  Rectangle(double w, double h)
      : width(w), height(h), _area(0), _perimeter(0) {}

  inline void set_width(double w) {
    if (width != w) {
      width = w;
      invalid_calc();
    }
  }

  inline void set_height(double h) {
    if (height != h) {
      height = h;
      invalid_calc();
    }
  }

  inline double get_width() const { return width; }
  inline double get_height() const { return height; }
  inline double area() const {
    if (b_area)
      return _area;
    b_area = true;
    return _area = height * width;
  };
  inline double perimeter() const {
    if (b_perimeter)
      return _perimeter;
    b_perimeter = true;
    return _perimeter = 2 * (height + width);
  }

private:
  inline void invalid_calc() { b_area = b_perimeter = false; }
};
int main() {
  Rectangle(3, 4);

  cout << boolalpha;
  {
    Point p(3, 4); // 1. constructor
    auto p2{p};    // 2. copy constructor
    if (p2 != p) { // comparison operator!=
      p2 = p;      // 3. copy assignment operator
      cout << "not equal\n";
    } else {
      cout << "equal\n";
    }
    Point p3(3, 5);
    cout << (p3 == p) << "\n"; // comparison operator==
  }                            // 4. p and p2 are out of scope, destroyed

  {
    cout << "PointWithStrNoCtor\n";
    PointWithStrNoCtor p;
    cout << p.get_x() << ", " << p.get_y() << ", " << p.get_name() << "\n";

    for (auto v : p.get_vd()) {
      cout << v << ", ";
    }
    cout << "\n";
  }

  {
    cout << "PointWithStr(...)\n";
    PointWithStr p(42, 43, "a. name", {1, 2});
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
}