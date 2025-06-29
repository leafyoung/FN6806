
#include "gbm_multi.h"
#include <algorithm>
#include <random>
#include <valarray>

using namespace std;

using nd_double = normal_distribution<double>;

multipath gbm_multipath(double S0, double mu, double sigma, double T, double dt,
                        size_t paths, mt19937 &gen) {
  const int n_dt = round(T / dt);
  multipath v(paths, path(n_dt + 1));
  // fill in the first value
  for (auto &v_path : v)
    v_path[0] = S0;

  static nd_double nd(0.0, 1.0);
  const auto drift = (mu - sigma * sigma / 2.0) * dt;
  const auto diffusion = sqrt(dt) * sigma;
  nd.param(nd_double ::param_type(drift, diffusion));

  for (auto &v_path : v) {
    // 1. Generate random number from 2nd to the last
    generate(next(v_path.begin(), 1), v_path.end(),
             [&gen]() { return nd(gen); });
    // 2. Calculate the path successively: S_t = S_{t-1} * exp(...)
    transform(next(v_path.begin(), 1), v_path.end(), // from 2nd to the last
              v_path.begin(),                        // previous S
              next(v_path.begin(), 1),               // output
              [](const auto &z, const auto &s) { return s * exp(z); });
  }
  return v;
}

multipath gbm_multipath_opt(const GBMParam &gbm, const MCParam &mc,
                            const Market &mkt, const Eval &eval) {
  const int n_dt = round(eval.T / mc.dt);
  multipath v(mc.paths, path(n_dt + 1));
  for (auto &v_path : v)
    v_path[0] = mkt.S;

  static nd_double nd(0.0, 1.0);
  const auto drift = (gbm.mu - gbm.sigma * gbm.sigma / 2.0) * mc.dt;
  const auto diffusion = sqrt(mc.dt) * gbm.sigma;
  nd.param(nd_double::param_type(drift, diffusion));

  for (auto &v_path : v) {
    // 1. Generate random number from 2nd to the last
    generate(next(v_path.begin(), 1), v_path.end(),
             [&mc]() { return nd(mc.gen); });
    // 2. Calculate the path successively: S_t = S_{t-1} * exp(...)
    transform(next(v_path.begin(), 1), v_path.end(), // from 2nd to the last
              v_path.begin(),                        // previous S
              next(v_path.begin(), 1),               // output
              [](const auto &z, const auto &s) { return s * exp(z); });
  }
  return v;
}

/*
using path_val = valarray<double>;
using multipath_val = vector<path_val>;

auto gbm_multipath_opt(double S0, double mu, double sigma,
  const int n_dt = round(T / dt);
  multipath_val v(n_dt + 1, valarray<double>(S0, paths));

  static nd_double nd(0.0, 1.0);
  const auto drift = (mu - sigma * sigma / 2.0) * dt;
  const auto diffusion = sqrt(dt) * sigma;
  nd.param(nd_double::param_type(drift, diffusion));

  transform(next(v.begin(), 1), v.end(), // current v_n_dt
            v.begin(),                   // previous v_n_dt
            next(v.begin(), 1),          // output current v_n_dt
            [&gen](auto &v_n_dt, const auto &prev_v_n_dt) {
              // We use v_n_dt to store the random number
              // Then we calculate the path successively
              // and overwrite v_n_dt with the path
              generate(begin(v_n_dt), end(v_n_dt),
                       [&gen]() { return nd(gen); });
              return prev_v_n_dt * exp(v_n_dt);
            });
  return v;
}

void write_all_val(const multipath_val &V, string fn = "output/multiRuns.csv") {
  ofstream my_file(fn);
  for (const auto &v_nd_t : V) { // for each time point
    bool skip_first_col = true;
    for (auto v : v_nd_t) { // for each path
      if (skip_first_col) {
        my_file << v;
        skip_first_col = false;
      } else {
        my_file << "," << v; // print "," from 2nd column onwards
      }
    }
    my_file << '\n';
  }
  my_file.close();
}
*/