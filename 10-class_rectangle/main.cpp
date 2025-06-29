// https://replit.com/@YeKunlun/10-classrectangle

#include <iostream>
using namespace std;

#include "rectangle_inside.h"
// Or
// #include "rectangle.h"
// #include "rectangle_atomic.h"

struct Point {
private: // like private
  int x, y;

public:
  int get_x() const { return x; } // getter
  void set_x(int _x) { x = _x; }  // setter
  int get_y() const { return y; } // getter
  void set_y(int _y) { y = _y; }  // setter

  /*
  int set_x(int _x) {
      x = _x;
      return x;
    }
  */
};

int main() {
  Rectangle rec;
  rec.set_height(3.0);
  rec.set_width(4.0);
  cout << rec.get_height() << ", " << rec.get_width() << '\n';
  cout << rec.area() << ", " << rec.perimeter() << '\n';

  Rectangle rec2;
  rec2.set_height(2.0);
  rec2.set_width(5.0);
  cout << rec2.area() << ", " << rec2.perimeter() << '\n';

  /*
  {
    using namespace RectangleAtomic;
    RectangleAtomic::Rectangle rec(3, 44);
    cout << rec.area() << '\n';
  }
  */
}