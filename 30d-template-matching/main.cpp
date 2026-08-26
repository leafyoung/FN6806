#include <iostream>
#include <type_traits>

template <typename T> T my_max(T x, T y) { return x > y ? x : y; }

// Entities are built by the class.
class Put {};
class Call {};

// Generic version
template <typename T> double payoff(double price, double strike) {
  // return my_max(price - strike, 0.0);

  // throw std::logic_error("Not implemented");
  static_assert(std::is_same<T, Call>::value || std::is_same<T, Put>::value,
                "Not implemented");
}

// Specialized version
template <> double payoff<Put>(double price, double strike) {
  return my_max(strike - price, 0.0);
}

template <> double payoff<Call>(double price, double strike) {
  return my_max(price - strike, 0.0);
}

// Entities are built by the class.
template <int T> class Payoff {
  double operator()(double price, double strike);
};

template <int T> double Payoff<T>::operator()(double price, double strike) {}

template <> class Payoff<0> {};
template <> class Payoff<1> {};

int main() {
  // auto r1 = payoff(3, 5);       // Failure
  // auto r1 = payoff<int>(3, 5);  // 0
  auto r2 = payoff<Call>(3, 5); // 0
  auto r3 = payoff<Put>(3, 5);  // 2

  std::cout // << r1 << "\n"
      << r2 << "\n"
      << r3 << "\n";
}