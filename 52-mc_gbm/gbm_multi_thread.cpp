#include "gbm_multi_thread.h"
#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <random>
#include <thread>
#include <valarray>

using namespace std;

using nd_double = normal_distribution<double>;

void gbm_multipath_opt_inc(const GBMParam &gbm, const MCParam &mc,
                           const Market &mkt, const Eval &eval, multipath &v,
                           int start, int end) {
  static nd_double nd(0.0, 1.0);
  const auto drift = (gbm.mu - gbm.sigma * gbm.sigma / 2.0) * mc.dt;
  const auto diffusion = sqrt(mc.dt) * gbm.sigma;
  nd.param(nd_double::param_type(drift, diffusion));

  for (auto it = next(v.begin(), start); it != next(v.begin(), end); ++it) {
    it->front() = mkt.S;
    generate(next(it->begin(), 1), it->end(), // from 2nd to the last
             [&mc]() { return nd(mc.gen); });

    transform(next(it->begin(), 1), it->end(), // from 2nd to the last
              it->begin(),                     // previous S
              next(it->begin(), 1),            // output
              [](const auto &z, const auto &s) { return s * exp(z); });
  }
}

multipath gbm_multipath_opt_thread(const GBMParam &gbm, const MCParam &mc,
                                   const Market &mkt, const Eval &eval,
                                   const int &n_thread) {
  // use incoming seed to initialize the seed
  uniform_int_distribution<unsigned int> uid;
  seed_seq seed{uid(mc.gen)};
  // generate a vector of seeds
  std::vector<std::uint32_t> seeds(n_thread);
  seed.generate(seeds.begin(), seeds.end());
  // create a vector of mt19937 from different seeds
  vector<mt19937> mts;
  for (auto seed : seeds)
    mts.emplace_back(seed);

  // allocate for all the MCParam for each thread
  vector<MCParam> mcs(n_thread, mc);
  const int n_dt = round(eval.T / mc.dt);

  // allocate for all the paths data
  multipath vs(mc.paths, path(n_dt + 1, 0.0));

  vector<packaged_task<void()>> tasks;
  vector<future<void>> futures;
  vector<thread> threads;

  int end = 0, start = 0;
  for (int i = 0; i < n_thread; ++i) {
    mcs[i].gen = mts[i];
    // cout << mts[i] << ", " << uid(mcs[i].gen) << "\n";
    start = end;
    if (i < n_thread - 1) {
      mcs[i].paths = mcs[i].paths / n_thread;
    } else {
      mcs[i].paths = mc.paths - end;
    }
    end += mcs[i].paths;

    tasks.emplace_back([gbm, mc = mcs[i], mkt, eval, &vs, start, end]() {
      gbm_multipath_opt_inc(gbm, mc, mkt, eval, vs, start, end);
    });

    // if we use packaged_task/future
    futures.emplace_back(tasks.back().get_future());
    threads.emplace_back(move(tasks.back()));

    // if we only use thread
    /* threads.emplace_back(
        [&gbm, mc = mcs[i], &mkt, &eval, &vs, start, end]() {
      gbm_multipath_opt_inc(gbm, mc, mkt, eval, vs, start, end);
        }); */
    // cout << mc.paths << ": " << mcs[i].paths << ", " << &mcs[i] << "\n";
  }

  for (auto &f : futures) {
    f.wait();
  }

  for (auto &t : threads) {
    t.join();
  }

  return vs;
}
