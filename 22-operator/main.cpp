// https://replit.com/@YeKunlun/22-operator?v=1

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric> // for std::accumulate
using namespace std;

struct Multiply {
  int operator()(int a, int b) const { return a * b; }
};

// Enable C++17
struct KeepAdd {
  inline static int sum = 0;
  void operator()(int a) const { sum += a; }
};

int main() {
  auto varr = std::array<int, 3>{{2, 3, 4}};
  int factorial = std::accumulate(varr.begin(), varr.end(), 1, Multiply{});
  cout << factorial << endl; // 24

  KeepAdd keepadd;
  for_each(varr.begin(), varr.end(),
           [keepadd = keepadd](auto x) { keepadd(x); });
  cout << keepadd.sum << "\n"; // 9
  for_each(varr.begin(), varr.end(), keepadd);
  cout << keepadd.sum << "\n"; // 18
}