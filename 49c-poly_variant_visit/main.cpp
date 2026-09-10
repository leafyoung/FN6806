// https://github.com/leafyoung/FN6806/tree/main/49c-poly_variant_visit

// Need --std=c++17 or c++20

#include <iostream>
#include <variant>
#include <vector>
using namespace std;

// The alternatives carry data, so the variant stores a value of one of these
// types - no common base class, no pointers, no virtual functions.
struct Circle {
  double radius;
};

struct Square {
  double side;
};

// A visitor: one overload per alternative.
class Draw {
public:
  void operator()(Circle const &c) const { cout << "Draw circle r=" << c.radius << '\n'; }
  void operator()(Square const &s) const { cout << "Draw square a=" << s.side << '\n'; }
};

using Shape = variant<Circle, Square>;

void drawAllShapes(vector<Shape> const &shapes) {
  for (auto const &s : shapes) {
    visit(Draw{}, s);
  }
}

int main() { drawAllShapes(vector<Shape>{Circle{1.0}, Square{2.0}, Circle{3.0}}); }
