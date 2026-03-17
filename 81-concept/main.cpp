// https://replit.com/@YeKunlun/81-concept?v=1

#include <concepts>
#include <iostream>

// Define: any type T that has a callable .npv() returning double
template <typename T>
concept Priceable = requires(T t) {
  { t.npv() } -> std::convertible_to<double>;
};

// Constrain:
template <Priceable T> double price(T instrument) { return instrument.npv(); }

struct Demo {
  double npv() { return 42; }
};

int main() {

  // Compilation error
  // std::cout << price(42);

  std::cout << price(Demo());

  return 0;
}