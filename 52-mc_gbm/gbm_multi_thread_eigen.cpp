#include "gbm_multi_thread_eigen.h"

#ifdef HAS_EIGEN

#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <thread>
#include "split_mix64.h"

using namespace std;
using namespace Eigen;

using nd_double = normal_distribution<double>;

void gbm_multipath_opt_inc_eigen(const GBMParam& gbm, const MCParam& mc, const Market& mkt,
                                 const Eval& eval, MatrixXd& v, int start, int end) {
  const auto drift = (gbm.mu - gbm.sigma * gbm.sigma / 2.0) * mc.dt;
  const auto diffusion = sqrt(mc.dt) * gbm.sigma;
  // nd is local (not static) so each thread owns its own instance.
  // The single-threaded version (gbm_multi.cpp) uses `static nd_double nd`
  // which is safe there (one caller), but would be a data race here since
  // all threads would share and mutate the same object concurrently.
  nd_double nd(drift, diffusion);
  const auto rows = v.rows();
  double prev_val = 0;
  for (auto row = start; row != end; ++row) {
    v(0, row) = prev_val = mkt.S;
    for (auto x = 1; x < rows; ++x) {
      v(x, row) = prev_val = prev_val * exp(nd(mc.gen));
    }
  }
}

MatrixXd gbm_multipath_opt_thread_eigen(const GBMParam& gbm, const MCParam& mc, const Market& mkt,
                                        const Eval& eval, const int& n_thread) {
  uniform_int_distribution<uint64_t> uid;
  SplitMix64 sm(uid(mc.gen));

  // create a vector of mt19937_64 from different seeds
  vector<mt19937_64> mts;
  for (size_t i = 0; i < n_thread; ++i) {
    mts.emplace_back(sm());
  }

  vector<MCParam> mcs(n_thread, mc);
  const size_t n_dt = round(eval.T / mc.dt);
  MatrixXd vs(n_dt + 1, mc.paths);

  vector<packaged_task<void()>> tasks;
  vector<future<void>> futures;
  vector<thread> threads;

  const size_t base_paths = mc.paths / static_cast<size_t>(n_thread);
  const size_t remainder = mc.paths % static_cast<size_t>(n_thread);
  size_t end = 0, start = 0;
  for (int i = 0; i < n_thread; ++i) {
    mcs[i].gen = mts[i];
    // cout << mts[i] << ", " << uid(mcs[i].gen) << '\n';
    start = end;
    mcs[i].paths = base_paths + (i == n_thread - 1 ? remainder : 0);
    end += mcs[i].paths;

    tasks.emplace_back([gbm, mc = mcs[i], mkt, eval, &vs, start, end]() {
      gbm_multipath_opt_inc_eigen(gbm, mc, mkt, eval, vs, start, end);
    });

    // if we use packaged_task/future
    futures.emplace_back(tasks.back().get_future());
    threads.emplace_back(std::move(tasks.back()));
  }

  for (auto& f : futures) {
    f.wait();
  }

  for (auto& t : threads) {
    t.join();
  }

  return vs;
}

#endif  // HAS_EIGEN
