// https://github.com/leafyoung/FN6806/tree/main/50-valarray

#include <cmath>
#include <iostream>
#include <string>
#include <valarray>

void print_valarray(const std::string name, const std::valarray<double>& dv) {
  std::cout << name << ": ";
  for (auto v : dv) {
    std::cout << v << ", ";
  }
  std::cout << '\n';
}

int main() {
  std::valarray<double> x{1.0, 2.0, 3.0, 1.5, 2.5};
  std::valarray<double> y{10.0, -20.0, 30.0, -15.0, 25.0};

  const double v1_max = x.max();  // 3.0
  const double v1_min = y.min();  // -20.0
  std::cout << "max(x): " << v1_max << '\n';
  std::cout << "min(y): " << v1_min << '\n';

  print_valarray("x", x);
  x = +x;
  print_valarray("+x", x);
  x += 3.0;
  print_valarray("x+3.0", x);
  x = -x;
  print_valarray("-x", x);
  x -= 3.0;
  print_valarray("x-3.0", x);

  x = x.apply([](double x) { return x * x; });
  print_valarray("x*x", x);

  // apply() and the maths functions may return an expression-template type
  // (libstdc++ does), so store results as explicit valarray<double>, not auto.
  std::valarray<double> u = x.apply([](double x) { return x * x; });
  print_valarray("u", u);
  std::valarray<double> w = y.apply([](double x) { return std::sin(x) + std::cos(x); });
  print_valarray("w", w);
  print_valarray("y", y);

  std::valarray<double> sine_u = std::sin(u);
  print_valarray("sine_u", sine_u);
  std::valarray<double> log_x = std::log(x);
  print_valarray("log_x", log_x);
  std::valarray<double> abs_y = std::abs(y);
  print_valarray("abs_y", abs_y);
  std::valarray<double> exp_x = std::exp(x);
  print_valarray("exp_x", exp_x);
  std::valarray<double> neg_x = -x;
  print_valarray("neg_x", neg_x);
}