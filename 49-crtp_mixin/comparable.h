#pragma once
#include <iostream>
#include <tuple>
#include <utility>
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
template <typename T, typename U> class ComparableXY {
  U x, y;
  inline T &self() { return static_cast<T &>(*this); }
  inline const T &self() const { return static_cast<const T &>(*this); }

public:
  ComparableXY(const U &x, const U &y) : x(x), y(y) {}
  bool operator==(T const &rhs) const {
    // `tie` from tuple can replace `x == rhs.x && y == rhs.y;`
    // `tie` can be used to compare any number of members at once.
    return std::tie(x, y) == std::tie(rhs.x, rhs.y);
  }
  bool operator!=(T const &rhs) const { return !((*this) == rhs); }
};

class Point : public ComparableXY<Point, double> {
  using ComparableXY<Point, double>::ComparableXY;
};

class Rectangle : public ComparableXY<Rectangle, Point> {
  using ComparableXY<Rectangle, Point>::ComparableXY;
};

} // namespace CRTP

void test_comparable() {
  cout << boolalpha;
  {
    using namespace Vanilla;
    cout << "Using Vanilla\n";

    Point p1(1, 1), p2(1, 2);
    cout << (p1 == p1) << ", " << (p1 != p1) << '\n';
    cout << (p1 == p2) << ", " << (p1 != p2) << '\n';

    Rectangle r1(p1, p2), r2({1, 2}, p2);
    cout << (r1 == r1) << ", " << (r1 != r1) << '\n';
    cout << (r1 == r2) << ", " << (r1 != r2) << '\n';
    cout << '\n';
  }

  {
    using namespace CRTP;
    cout << "Using CRTP\n";

    Point p1(1., 1.), p2(1., 2.);
    cout << (p1 == p1) << ", " << (p1 != p1) << '\n';
    cout << (p1 == p2) << ", " << (p1 != p2) << '\n';

    Rectangle r1(p1, p2), r2({1., 2.}, p2);
    cout << (r1 == r1) << ", " << (r1 != r1) << '\n';
    cout << (r1 == r2) << ", " << (r1 != r2) << '\n';
    cout << '\n';
  }
}