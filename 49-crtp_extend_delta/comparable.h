#pragma once
#include <iostream>
#include <tuple>
using namespace std;

namespace Vanilla {
class Point {
  int x, y;

public:
  Point(int x, int y) : x(x), y(y) {}
  bool operator==(Point const &rhs) const { return x == rhs.x && y == rhs.y; }
  bool operator!=(Point const &rhs) const { return !((*this) == rhs); }
};

class Rectangle {
  Point x, y;

public:
  Rectangle(Point x, Point y) : x(x), y(y) {}
  bool operator==(Rectangle const &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  bool operator!=(Rectangle const &rhs) const { return !((*this) == rhs); }
};

} // namespace Vanilla

namespace CRTP {
template <class T> class Comparable {
  inline T &self() { return static_cast<T &>(*this); }
  inline const T &self() const { return static_cast<const T &>(*this); }

public:
  bool operator==(T const &rhs) const { return self().is_equal(rhs); }
  bool operator!=(T const &rhs) const { return !((*this) == rhs); }
};

class Point : public Comparable<Point> {
  int x, y;

public:
  Point(int x, int y) : x(x), y(y) {}
  bool is_equal(Point const &rhs) const { return x == rhs.x && y == rhs.y; }
};

class Rectangle : public Comparable<Rectangle> {
  Point x, y;

public:
  Rectangle(Point x, Point y) : x(x), y(y) {}
  bool is_equal(Rectangle const &rhs) const { return x == rhs.x && y == rhs.y; }
};

} // namespace CRTP

void test_comparable() {
  cout << boolalpha;
  {
    using namespace Vanilla;
    cout << "Using Vanilla" << "\n";

    Point p1(1, 1), p2(1, 2);
    cout << (p1 == p1) << ", " << (p1 != p1) << "\n";
    cout << (p1 == p2) << ", " << (p1 != p2) << "\n";

    Rectangle r1(p1, p2), r2({1, 2}, p2);
    cout << (r1 == r1) << ", " << (r1 != r1) << "\n";
    cout << (r1 == r2) << ", " << (r1 != r2) << "\n";
    cout << "\n";
  }

  {
    using namespace CRTP;
    cout << "Using CRTP" << "\n";

    Point p1(1, 1), p2(1, 2);
    cout << (p1 == p1) << ", " << (p1 != p1) << "\n";
    cout << (p1 == p2) << ", " << (p1 != p2) << "\n";

    Rectangle r1(p1, p2), r2({1, 2}, p2);
    cout << (r1 == r1) << ", " << (r1 != r1) << "\n";
    cout << (r1 == r2) << ", " << (r1 != r2) << "\n";
    cout << "\n";
  }
}