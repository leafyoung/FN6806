#include "gbm_multi_thread_eigen.h"
#include "eigen3/Eigen/Dense"
#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <thread>

using namespace std;
using namespace Eigen;

using nd_double = normal_distribution<double>;

void gbm_multipath_opt_inc_eigen(const GBMParam &gbm, const MCParam &mc,
                                 const Market &mkt, const Eval &eval,
                                 MatrixXd &v, int start, int end) {
  static nd_double nd(0.0, 1.0);
  const auto drift = (gbm.mu - gbm.sigma * gbm.sigma / 2.0) * mc.dt;
  const auto diffusion = sqrt(mc.dt) * gbm.sigma;
  nd.param(nd_double::param_type(drift, diffusion));
  const auto rows = v.rows();
  double prev_val = 0;
  for (auto row = start; row != end; ++row) {
    v(0, row) = prev_val = mkt.S;
    for (auto x = 1; x < rows; ++x) {
      v(x, row) = prev_val = prev_val * exp(nd(mc.gen));
    }
  }
}

MatrixXd gbm_multipath_opt_thread_eigen(const GBMParam &gbm, const MCParam &mc,
                                        const Market &mkt, const Eval &eval,
                                        const int &n_thread) {
  uniform_int_distribution<unsigned int> uid;

  seed_seq seed{uid(mc.gen)};
  std::vector<std::uint32_t> seeds(n_thread);
  seed.generate(seeds.begin(), seeds.end());
  vector<mt19937> mts;
  for (auto seed : seeds)
    mts.emplace_back(seed);

  vector<MCParam> mcs(n_thread, mc);
  const int n_dt = round(eval.T / mc.dt);
  MatrixXd vs(n_dt + 1, mc.paths);

  vector<packaged_task<void()>> tasks;
  vector<future<void>> futures;
  vector<thread> threads;

  int end = 0, start = 0;
  for (int i = 0; i < n_thread; ++i) {
    mcs[i].gen = mts[i];
    // cout << mts[i] << ", " << uid(mcs[i].gen) << endl;
    start = end;
    if (i < n_thread - 1) {
      mcs[i].paths = mcs[i].paths / n_thread;
    } else {
      mcs[i].paths = mc.paths - end;
    }
    end += mcs[i].paths;

    tasks.emplace_back([gbm, mc = mcs[i], mkt, eval, &vs, start, end]() {
      gbm_multipath_opt_inc_eigen(gbm, mc, mkt, eval, vs, start, end);
    });

    // if we use packaged_task/future
    futures.emplace_back(tasks.back().get_future());
    threads.emplace_back(move(tasks.back()));
  }

  for (auto &f : futures) {
    f.wait();
  }

  for (auto &t : threads) {
    t.join();
  }

  return vs;
}
