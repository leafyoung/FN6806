#pragma once

#include <random>
#include <stdexcept>

using namespace std;

using path = vector<double>;
using multipath = vector<path>;

multipath gbm_multipath(double S0, double mu, double sigma, double T, double dt,
                        size_t paths, mt19937 &gen);

using path_val = valarray<double>;

struct GBMParam {
  double mu;
  double sigma;
};

struct MCParam {
  double dt;
  size_t paths;
  mt19937 &gen;
};

struct Market {
  double S;
};

struct Eval {
  double T;
};

multipath gbm_multipath_opt(const GBMParam &gbm, const MCParam &mc,
                            const Market &mkt, const Eval &eval);

class Multipath {
  size_t _n_paths, _n_points;
  vector<double> data;

public:
  Multipath(size_t n_paths, size_t n_points, double default_value = 0)
      : _n_paths(n_paths), _n_points(n_points),
        data(n_paths * n_points, default_value) {}

  double &operator()(size_t idx_path, size_t idx_point) {

    // Because we can progress through time for all paths.
    // We let all paths' points of the same time to be packed together
    // (0, 0) => 0
    // (1, 0) => 1
    // (2, 0) => 2
    // (3, 0) => 3
    // ...
    return data[idx_point * _n_paths + idx_path];
  }

  double &operator[](size_t x) { return data[x]; }

  // Return iterator to the beginning
  std::vector<double>::iterator begin() { return data.begin(); }

  // Return iterator to the end
  std::vector<double>::iterator end() { return data.end(); }

  // Return iterator to the beginning of the X time point
  std::vector<double>::iterator begin(size_t x) {
    if (x >= this->_n_points)
      throw logic_error("Time point out of bound");
    return next(data.begin(), _n_paths * x);
  }

  // Return iterator to the end of the X time point
  std::vector<double>::iterator end(size_t x) {
    return next(data.begin(), _n_paths * (x + 1));
  }

  size_t n_paths() const { return this->_n_paths; }
  size_t n_points() const { return this->_n_points; }
};

Multipath gbm_multipath_opt2(const GBMParam &gbm, const MCParam &mc,
                             const Market &mkt, const Eval &eval);
