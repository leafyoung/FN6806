#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <vector>
using namespace std;

const int TEST_RUN = 100'000'000;
const double TEST_RUN_D = static_cast<double>(TEST_RUN);

// Adapted from https://en.wikipedia.org/wiki/Expression_templates
// Non-ET implementation
#include "non_et.h"

// ET implementation
#include "et.h"

int main() {
  {
    const Vec1<12> v0{23.4, 12.5, 144.56, 23.4, 12.5, 144.56,
                      23.4, 12.5, 144.56, 23.4, 12.5, 144.56};
    const Vec1<12> v1{67.12, 34.8, 90.34,  23.4, 12.5, 144.56,
                      23.4,  12.5, 144.56, 23.4, 12.5, 144.56};
    const Vec1<12> v2{34.90, 111.9, 45.12,  23.4, 12.5, 144.56,
                      23.4,  12.5,  144.56, 23.4, 12.5, 144.56};
    const auto ab = v0 + v1 + v2;
    cout << ab << endl;

    auto c = ab;
    const auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TEST_RUN; ++i) {
      c = c + v0 + v1 + v2 + v2;
    }
    const auto end = chrono::high_resolution_clock::now();
    cout << "Time to do Vec1(): "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
         << "ns" << endl;
    cout << c << endl;
  }

  {
    const Vec<12> v0{23.4, 12.5, 144.56, 23.4, 12.5, 144.56,
                     23.4, 12.5, 144.56, 23.4, 12.5, 144.56};
    const Vec<12> v1{67.12, 34.8, 90.34,  23.4, 12.5, 144.56,
                     23.4,  12.5, 144.56, 23.4, 12.5, 144.56};
    const Vec<12> v2{34.90, 111.9, 45.12,  23.4, 12.5, 144.56,
                     23.4,  12.5,  144.56, 23.4, 12.5, 144.56};

    const Vec<12> sum_of_vec_type = v0 + v1 + v2;
    cout << sum_of_vec_type << endl;
    cout << typeid(sum_of_vec_type).name() << endl;
    // 125.42	159.2	280.02

    // cannot use auto because
    const auto sum2 = v0 + v1 + v2;
    cout << typeid(sum2).name() << endl;
    cout << sum2 << endl;

    Vec<12> c = v0 + v1 + v2;

    cout << typeid(c).name() << endl;
    const auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TEST_RUN; ++i) {
      c = c + v0 + v1 + v2;
    }
    cout << c << endl;
    const auto end = chrono::high_resolution_clock::now();
    cout << "Time to do et(): "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
         << "ns" << endl;
  }
}