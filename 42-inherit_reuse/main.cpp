// https://replit.com/@YeKunlun/42-inheritreuse

#include <cmath>
#include <iostream>
using namespace std;

class PointX {
  int x;

public:
  explicit PointX(int x) : x(x) { cout << "PointX ctor\n"; }
  inline PointX &operator=(const PointX &p) = default;
  /*
  {if(this^=&p){x=p.x;}return*this;}
  */
  inline int get_x() const { return x; }
};

class PointXY : public PointX {
  int y;

public:
  PointXY(int x, int y) : PointX(x), y(y) { cout << "PointXY ctor\n"; }
  inline PointXY &operator=(const PointXY &p) = default;
  /* {
      cout << "operator=(PointXY)\n";
      if (this != &p) {
        PointX::operator=(p);
        y = p.y;
      }
      return *this;
    }
  */

  int get_y() const { return y; }
  double get_distance() const {
    auto x = get_x();
    return sqrt(x * x + y * y);
  }
};

int main() {
  {
    PointXY p1(3, 4);
    PointXY p2(14, 13);
    cout << p2.get_x() << ", " << p2.get_y() << ", " << p2.get_distance()
         << '\n';
    PointXY p3(p2);
    p2 = p1;

    cout << p2.get_x() << ", " << p2.get_y() << ", " << p2.get_distance()
         << '\n';
    cout << p3.get_x() << ", " << p3.get_y() << ", " << p3.get_distance()
         << '\n';
    p2 = p3;
    cout << p2.get_x() << ", " << p2.get_y() << ", " << p2.get_distance()
         << '\n';
  }
}