#include <iostream>
using namespace std;

class Shape {
public:
  virtual double area() const = 0;
  virtual ~Shape() = default;
};

class Rectangle : public Shape {
  int x, y;

public:
  Rectangle(int x, int y) : x(x), y(y) {}
  double area() const override { return x * y; }
};
class Circle : public Shape {
  int r;

public:
  Circle(int r) : r(r) {}
  double area() const override { return 3.1415926 * r * r; }
};

void printArea(Shape const &s) { cout << s.area() << endl; }

template <typename T> void printAreaT(T const &s) { cout << s.area() << endl; }

void test_shape();