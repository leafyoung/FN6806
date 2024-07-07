#include <string>
#include <vector>
using namespace std;

class PointWithStrNoCtor {
  int x, y;          // not initialized
  string name;       // s is initialized
  vector<double> vd; // vd is initialized
public:
  auto get_x() const { return x; }
  auto get_y() const { return y; }
  auto get_name() const { return std::cref(name); }
  auto get_vd() const { return std::cref(vd); }
};

class PointWithStr {
  int x, y;          // not initialized
  string name;       // s is initialized
  vector<double> vd; // vd is initialized
public:
  PointWithStr(int x, int y, string name, vector<double> vd)
      : x{x}, y{y}, name{name}, vd{vd} {} // initialization
  auto get_x() const { return x; }
  auto get_y() const { return y; }
  auto get_name() const { return std::cref(name); }
  auto get_vd() const { return std::cref(vd); }
};
