#pragma once

#include <random>
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

multipath gbm_multipath_opt(const GBMParam &gbm, const MCParam &mc, const Market &mkt, const Eval &eval);