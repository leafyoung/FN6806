#include <functional>
#include <string>
#include <vector>
using namespace std;

class PointWithNDC {
  int x;

public:
  // GOOD: use explicit for single-argument constructor.
  explicit PointWithNDC(int x) : x(x) {}
  int get_x() const { return x; }
  operator int() const { return x; }
};
