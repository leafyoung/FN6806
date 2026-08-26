#include <iostream>
#include <string>
using namespace std;

template <typename T> class Widget {
  T t;

public:
  Widget(T t);
  T get() const;
  // we can omit <T> in below line
  // Widget<T> copy() const;
  Widget copy() const;
};

// Add template type for functions.
template <typename T> Widget<T>::Widget(T t) : t(t) {}

template <typename T> T Widget<T>::get() const { return t; }

template <typename T> Widget<T> Widget<T>::copy() const { return Widget(t); }

template <unsigned M, unsigned N> struct gcd {
  static int const value = gcd<N, M % N>::value;
};

// partial specialization
template <unsigned M> struct gcd<M, 0> {
  static_assert(M != 0); // gcd<0, 0> is not defined
  static int const value = M;
};

int main() {
  cout << gcd<33, 11>::value << endl;

  auto w = Widget<double>(3.0);
  cout << w.get() << endl;
}