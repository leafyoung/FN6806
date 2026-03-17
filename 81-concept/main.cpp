// https://replit.com/@YeKunlun/81-concept?v=1

#include <concepts>
#include <iostream>

// Define: any type T that has a callable .npv() returning double
template <typename T>
concept Priceable = requires(T t) {
  { t.npv() } -> std::convertible_to<double>;
};

// Constrain:
template <Priceable T> double price_constraint(T instrument) {
  return instrument.npv();
}

struct Demo {
  double npv() { return 42; }
};

// Verbose form (explicit requires clause)
template <typename T>
requires Priceable<T>
double price_verbose(T instrument) { return instrument.npv(); }

// Shorthand (abbreviated function template — C++20)
double price_short(Priceable auto instrument) { return instrument.npv(); }

int main() {

  // Compilation error
  // std::cout << price_constraint(42);

  std::cout << price_constraint(Demo()) << '\n';

  std::cout << price_verbose(Demo()) << '\n';

  std::cout << price_short(Demo()) << '\n';

  return 0;
}