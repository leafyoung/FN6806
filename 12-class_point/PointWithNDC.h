#pragma once

class PointWithNDC {
  int x;

public:
  // GOOD: use explicit for single-argument constructor.
  explicit PointWithNDC(int x) : x(x) {}
  int get_x() const { return x; }
  // explicit conversion operator: needs static_cast<int>(p)
  explicit operator int() const { return x; }
};
