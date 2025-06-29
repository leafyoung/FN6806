// https://replit.com/@YeKunlun/52-mcgbm

#include "gbm_multi.h"
#include "gbm_multi_thread.h"
#include "gbm_multi_thread_eigen.h"
#include "gbm_single.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <valarray>
#include <vector>

using namespace std::chrono;
using namespace std;

void writeToFile(const path &v, string fn = "output/singlefn.csv") {
  ofstream my_file(fn);
  for_each(v.begin(), v.end(), [&my_file](auto x) { my_file << x << '\n'; });
  my_file.close();
}

void write_all(const multipath &V, string fn = "output/multiRuns.csv") {
  ofstream my_file(fn);
  for (int t = 0; t < V[0].size(); ++t) {
    int p = 0;
    // write the 1st column without comma
    my_file << V[p++][t];
    // To write CSV file, we need to loop over all series' i element
    for (; p < V.size(); ++p) {
      my_file << "," << V[p][t]; // print "," from 2nd column onwards
    }
    my_file << '\n';
  }
  my_file.close();
}

void test_end_values(valarray<double> vals, double expected_mean,
                     double expected_stdev) {
  auto mean = vals.sum() / vals.size();
  auto stdev = sqrt(pow(vals - mean, 2.0).sum() / vals.size());
  auto mean_diff = (mean - expected_mean) / expected_mean;
  auto stdev_diff = (stdev - expected_stdev) / expected_stdev;
  cout << "[n]: " << vals.size() << " mean: " << mean << " (" << mean_diff
       << "), stdev: " << stdev << " (" << stdev_diff << ")\n";
}

int main(int argc, char **argv) {
  // try: mu = 0.03, sigma = 3
  // try: mu = 1, sigma = 0.3
  // gbm
  const double mu = 0.5;
  const double sigma = 0.3;
  // mc simulation
  const size_t paths = 10'000;
  const double dt{1.0 / 365.0};
  // market data
  const double S0{100.0};
  // time to maturity
  const double T{dt * 120};

  const auto expected_mean = exp(T * mu) * S0;
  const auto expected_stdev =
      S0 * exp(T * mu) * sqrt(exp(sigma * sigma * T) - 1);

  cout << "expected mean: " << expected_mean << '\n';
  cout << "expected stdev: " << expected_stdev << '\n';

  seed_seq seed{1238982};
  mt19937 gen{seed};

  valarray<double> end_values(0.0, paths);
  multipath traj;

  {
    auto start = high_resolution_clock::now();
    gen.seed(seed);
    for (int n = 0; n < paths; n++) {
      traj.emplace_back(gbm_single_path_demo(S0, mu, sigma, T, dt, gen));
      end_values[n] = traj[n].back();
    }
    auto end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
         << '\n';
    write_all(traj, "output/gbm_single_path_demo.csv");
    cout << "gbm_single_path_demo\n";
    test_end_values(end_values, expected_mean, expected_stdev);
  }

  auto start = high_resolution_clock::now();
  traj.clear();
  gen.seed(seed);
  for (int n = 0; n < paths; n++) {
    traj.emplace_back(gbm_single_path(S0, mu, sigma, T, dt, gen));
    end_values[n] = traj[n].back();
  }
  auto end = high_resolution_clock::now();
  cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
       << '\n';
  write_all(traj, "output/gbm_single_path.csv");
  cout << "gbm_single_path\n";
  test_end_values(end_values, expected_mean, expected_stdev);

  gen.seed(seed);
  traj.clear();
  start = high_resolution_clock::now();
  for (int n = 0; n < paths; n++) {
    traj.emplace_back(gbm_single_path_v2(S0, mu, sigma, T, dt, gen));
    end_values[n] = traj[n].back();
  }
  end = high_resolution_clock::now();
  cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
       << '\n';
  write_all(traj, "output/gbm_single_path_v2.csv");
  cout << "gbm_single_path_v2\n";
  test_end_values(end_values, expected_mean, expected_stdev);

  gen.seed(seed);
  traj.clear();
  start = high_resolution_clock::now();
  for (int n = 0; n < paths; n++) {
    traj.emplace_back(gbm_single_path_exp(S0, mu, sigma, T, dt, gen));
    end_values[n] = traj[n].back();
  }
  end = high_resolution_clock::now();
  cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
       << '\n';
  write_all(traj, "output/gbm_single_path_exp.csv");
  cout << "gbm_single_path_exp\n";
  test_end_values(end_values, expected_mean, expected_stdev);

  gen.seed(seed);
  traj.clear();
  start = high_resolution_clock::now();
  traj = gbm_multipath(S0, mu, sigma, T, dt, paths, gen);
  end = high_resolution_clock::now();
  cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
       << '\n';
  transform(traj.begin(), traj.end(), begin(end_values),
            [](const auto &v) { return v.back(); });
  // write_all(traj, "output/gbm_multipath.csv");
  cout << "gbm_multipath\n";
  test_end_values(end_values, expected_mean, expected_stdev);

  gen.seed(seed);
  traj.clear();
  start = high_resolution_clock::now();
  traj = gbm_multipath_opt({.mu = mu, .sigma = sigma},
                           {.dt = 1.0 / 365.0, .paths = paths, .gen = gen},
                           {.S = S0}, {.T = T});
  end = high_resolution_clock::now();
  cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
       << '\n';
  transform(traj.begin(), traj.end(), begin(end_values),
            [](const auto &v) { return v.back(); });
  // write_all(traj, "output/gbm_multipath.csv");
  cout << "gbm_multipath_opt\n";
  test_end_values(end_values, expected_mean, expected_stdev);

  const int multiplier = 10;

  {
    gen.seed(seed);
    start = high_resolution_clock::now();
    auto traj1 = gbm_multipath_opt(
        {.mu = mu, .sigma = sigma},
        {.dt = 1.0 / 365.0, .paths = paths * multiplier, .gen = gen}, {.S = S0},
        {.T = T});
    end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
         << '\n';
    cout << "gbm_multipath_opt(struct) for " << paths * multiplier << '\n';
    end_values.resize(paths * multiplier);
    transform(traj1.begin(), traj1.end(), begin(end_values),
              [](const auto &v) { return v.back(); });
    test_end_values(end_values, expected_mean, expected_stdev);

    gen.seed(seed);
    start = high_resolution_clock::now();
    auto traj2 = gbm_multipath_opt_thread(
        {.mu = mu, .sigma = sigma},
        {.dt = 1.0 / 365.0, .paths = paths * multiplier, .gen = gen}, {.S = S0},
        {.T = T}, 4);

    end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
         << '\n';
    cout << "gbm_multipath_opt_thread for " << paths * multiplier << '\n';
    end_values.resize(paths * multiplier);
    transform(traj2.begin(), traj2.end(), begin(end_values),
              [](const auto &v) { return v.back(); });
    test_end_values(end_values, expected_mean, expected_stdev);

    gen.seed(seed);
    start = high_resolution_clock::now();
    auto traj3 = gbm_multipath_opt_thread_eigen(
        {.mu = mu, .sigma = sigma},
        {.dt = 1.0 / 365.0, .paths = paths * multiplier, .gen = gen}, {.S = S0},
        {.T = T}, 4);

    end = high_resolution_clock::now();
    cout << duration_cast<nanoseconds>((end - start)).count() / 1e9 << "s"
         << '\n';
    cout << "gbm_multipath_opt_thread_eigen for " << paths * multiplier << '\n';
    end_values.resize(paths * multiplier);
    Eigen::VectorXd last_row = traj3.row(traj3.rows() - 1);
    end_values = valarray<double>(last_row.data(), last_row.size());
    test_end_values(end_values, expected_mean, expected_stdev);
    // writeToCSVfile("output/test.csv", traj3);
  }

  return 0;
}