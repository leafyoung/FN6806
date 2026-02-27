#pragma once

class Point {
  int x, y;

public:
  // BAD: Point() {} // default ctor
  // Point() = default;

  // Point(int x, int y) = default;

  Point(int x, int y) : x(x), y(y) {} // non-default ctor

  // T(const T& other)
  // Point(const Point& other)
  // Point(const Point& other) = default; // copy ctor

  // T& operator=(const T& rhs)
  Point &operator=(const Point &other) = default;

  // dtor
  ~Point() = default;
  // BAD: ~Point() {};

  // rhs: right hand side
  inline bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  inline bool operator!=(const Point &rhs) const { return !(*this == rhs); }
};
