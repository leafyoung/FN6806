#include "gbm_single.h"
#include <algorithm>
#include <random>
#include <vector>

#include <iostream>

using namespace std;

path gbm_single_path_demo(double S0, double mu, double sigma, double T,
                          double dt, mt19937 &gen) {
  path results(100, S0);
  return results;
}

path gbm_single_path(double S0, double mu, double sigma, double T, double dt,
                     mt19937 &gen) {
  static normal_distribution<double> nd(0.0, 1.0);

  const int n_dt = round(T / dt);
  path v(n_dt + 1, S0);

  for (int i = 1; i <= n_dt; ++i) {
    v[i] = v[i - 1] * (1 + mu * dt + sigma * sqrt(dt) * nd(gen));
  }
  return v;
}

path gbm_single_path_v2(double S0, double mu, double sigma, double T, double dt,
                        mt19937 &gen) {
  const int n_dt = round(T / dt);
  path v(n_dt + 1, S0);

  const double drift = 1 + mu * dt;
  const double diffusion = sigma * sqrt(dt);
  static normal_distribution<double> nd(0.0, 1.0);

  transform(v.begin(), prev(v.end(), 1), // previous S
            next(v.begin(), 1),          // current S
            [&gen, &drift, &diffusion](auto v) {
              return v * (drift + diffusion * nd(gen));
            });
  return v;
}

path gbm_single_path_exp(double S0, double mu, double sigma, double T,
                         double dt, mt19937 &gen) {
  const int n_dt = round(T / dt);
  path v(n_dt + 1, S0);

  const double drift = (mu - sigma * sigma / 2) * dt;
  const double diffusion = sigma * sqrt(dt);
  static normal_distribution<double> nd(0.0, 1.0);
  nd.param(normal_distribution<double>::param_type(drift, diffusion));

  transform(v.begin(), prev(v.end(), 1), next(v.begin(), 1),
            [&gen](const auto v0) { return v0 * exp(nd(gen)); });
  return v;
}

// === optional

path gbm_single_path_exp_cumsum(double S0, double mu, double sigma, double T,
                                double dt, mt19937 &gen) {
  const int n_dt = round(T / dt);
  path v(n_dt + 1, S0);

  static normal_distribution<double> nd(0.0, 1.0);
  nd.param(normal_distribution<double>::param_type(
      (mu - sigma * sigma / 2.0) * dt, sqrt(dt) * sigma));
  generate(next(v.begin(), 1), v.end(), // from 2nd to the last
           [&gen]() { return nd(gen); });
  double cumsum = 0;
  transform(next(v.begin(), 1), v.end(), // from 2nd to the last
            next(v.begin(), 1),          // output
            [&cumsum, S0](const auto z) {
              cumsum += z;
              return S0 * exp(cumsum);
            });
  return v;
}
