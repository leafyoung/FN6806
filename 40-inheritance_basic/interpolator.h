#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
using std::cout;
using std::logic_error;
using std::vector;

using Series = vector<double>;

class Interpolator {
protected:
  Series x, y;

public:
  Interpolator(const Series &x, const Series &y) : x(x), y(y) {
    if (x.size() != y.size())
      throw logic_error("unequal length x and y");
  }
};

class LinearInterpolator : public Interpolator {
public:
  // 1. ctor in derived class can call the base class' ctor
  // LinearInterpolator(const Series &x, const Series &y) : Interpolator(x, y)
  // {}
  // 2. When the ctor is the same as base class
  // Use `using` to bring base class's ctor into derived class.
  using Interpolator::Interpolator;

  double operator()(double d) const {
    cout << "linint: " << x.size() << ", " << y.size() << "\n";
    // 3. We need to access x and y here so they are protected.
    // dummy return value. not real implementation
    return 0.1;
  }
};
