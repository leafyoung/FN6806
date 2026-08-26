#pragma once
#include <algorithm>
#include <array>
#include <initializer_list>
#include <iostream>

using namespace std;

template <size_t N, typename T = double> class Vec1 {
  std::array<T, N> elems;

public:
  Vec1() : elems() {}
  Vec1(const std::array<T, N> &arr) : elems(arr) {}

  Vec1(initializer_list<T> init) {
    copy(init.begin(), init.end(), elems.begin());
  }

  T &operator[](size_t index) { return elems[index]; }
  const double &operator[](size_t index) const { return elems[index]; }
  size_t size() const { return N; }

  friend std::ostream &operator<<(std::ostream &os, const Vec1 &obj) {
    for (size_t i = 0; i < N; ++i) {
      os << i << ": ";
      os << obj[i] << ", ";
    }
    return os;
  }
};

template <size_t N> Vec1<N> operator+(Vec1<N> const &u, Vec1<N> const &v) {
  Vec1<N> sum;
  for (size_t i = 0; i < u.size(); i++) {
    sum[i] = u[i] + v[i];
  }
  return sum;
}
./