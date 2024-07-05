#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <valarray>
using namespace std;

void print_valarray(const std::string name, const std::valarray<double> &dv) {
  std::cout << name << ": ";
  for (auto v : dv) {
    std::cout << v << ", ";
  }
  std::cout << endl;
}

int main() {
  std::valarray<double> x{1.0, 2.0, 3.0, 1.5, 2.5};
  std::valarray<double> y{10.0, -20.0, 30.0, -15.0, 25.0};

  double v1_max = x.max(); // 3.0
  double v1_min = y.min(); // 1.0

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
  print_valarray("x&x", x);

  // u and w are valarray<double> types
  auto u = x.apply([](double x) { return x * x; });
  print_valarray("u", u);
  auto w = y.apply([](double x) { return std::sin(x) + std::cos(x); });
  print_valarray("v", y);

  // The result in each is a valarray<double>
  auto sine_x = std::sin(u);
  print_valarray("sine_x", sine_x);
  auto log_x = std::log(x);
  print_valarray("log_x", log_x);
  auto abs_x = std::abs(y);
  print_valarray("abs_x", abs_x);
  auto exp_x = std::exp(x);
  print_valarray("exp_x", exp_x);
  auto neg_x = -x;
  print_valarray("neg_x", neg_x);
}