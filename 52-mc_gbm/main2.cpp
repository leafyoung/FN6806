// ponytail: minimal harness — calls gbm_multipath_opt_thread only, for validating
// the SplitMix64 64-bit seed tweak without running the full main.cpp demo suite.
#include "gbm_multi_thread.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <valarray>

using namespace std;
using namespace std::chrono;

static void test_end_values(valarray<double> vals, double expected_mean, double expected_stdev) {
  auto mean = vals.sum() / vals.size();
  auto stdev = sqrt(pow(vals - mean, 2.0).sum() / vals.size());
  cout << "[n]: " << vals.size() << " mean: " << mean
       << " (" << (mean - expected_mean) / expected_mean << "), stdev: " << stdev
       << " (" << (stdev - expected_stdev) / expected_stdev << ")\n";
}

int main() {
  const double mu = 0.5;
  const double sigma = 0.3;
  const size_t paths = 40'000;
  const double dt = 1.0 / 365.0;
  const double S0 = 100.0;
  const double T = dt * 120;

  const auto expected_mean = exp(T * mu) * S0;
  const auto expected_stdev = S0 * exp(T * mu) * sqrt(exp(sigma * sigma * T) - 1);
  cout << "expected mean: " << expected_mean << '\n';
  cout << "expected stdev: " << expected_stdev << '\n';

  seed_seq s{1238982123178};
  mt19937_64 gen{s};

  auto start = high_resolution_clock::now();
  auto traj = gbm_multipath_opt_thread(
      {.mu = mu, .sigma = sigma}, {.dt = dt, .paths = paths, .gen = gen}, {.S = S0}, {.T = T}, 4);
  auto end = high_resolution_clock::now();

  cout << duration_cast<nanoseconds>(end - start).count() / 1e9 << "s\n";
  cout << "gbm_multipath_opt_thread\n";

  valarray<double> end_values(paths);
  transform(traj.begin(), traj.end(), begin(end_values),
            [](const auto& v) { return v.back(); });
  test_end_values(end_values, expected_mean, expected_stdev);
  return 0;
}
