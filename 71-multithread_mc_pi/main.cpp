// https://replit.com/@YeKunlun/71-multithreadmcpi

#include <future>
#include <iostream>
#include <mutex>
#include <random>
#include <syncstream>
#include <thread>
#include <vector>

using namespace std;

auto counter = 0; // Counter will be protected by counter_mutex
std::mutex counter_mutex;

void increment_counter(int n) {
  for (int i = 0; i < n; ++i) {
    auto lock = std::scoped_lock<mutex>{counter_mutex};
    ++counter;
  }
}

long random_circle_sampling(long n_samples) {
  std::random_device
      rd; // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  auto thread_id = std::this_thread::get_id();
  std::cout << "running thread "
            << *static_cast<unsigned int *>(static_cast<void *>(&thread_id))
            << '\n';
  std::uniform_real_distribution<> dis(0.0, 1.0);
  long points_inside = 0;
  for (long i = 0; i < n_samples; ++i) {
    double x = dis(gen);
    double y = dis(gen);
    if (x * x + y * y <= 1.0) {
      ++points_inside;
    }
  }
  return points_inside;
}

double approximate_pi(long tot_samples, int n_threads) {
  long samples_per_thread = tot_samples / n_threads;

  // Used to store the future results
  vector<future<long>> futures;
  for (int t = 0; t < n_threads; ++t) {
    // Start a new asynchronous task
    futures.emplace_back(
        async(launch::async, random_circle_sampling, samples_per_thread));
  }

  long tot_points_inside = 0;
  for (future<long> &f : futures) {
    // Wait for the result to be ready
    tot_points_inside += f.get();
  }

  double pi = 4.0 * (double)tot_points_inside / (double)tot_samples;
  return pi;
}

int main() {
  cout.precision(32);

  long tot_samples = 1e6;
  int n_threads = 8;
  double pi = 3.14159265358979323846;
  double approx_pi = approximate_pi(tot_samples, n_threads);
  double abs_diff = abs(pi - approx_pi);
  cout << "pi\t\t" << pi << '\n';
  cout << "approx_pi\t" << approx_pi << '\n';
  cout << "abs_diff\t" << abs_diff << '\n';

  return 0;
}