#include <iostream>
#include <random>
#include <string>

using namespace std;

namespace {
auto &prng() {
  thread_local mt19937 prng{random_device{}()};
  return prng;
}
constexpr int floor_division(int dividend, int divisor) {
  return (dividend - (dividend < 0)) / divisor;
}
} // namespace

bool luhn_valid(const string &str) {
  int sum = 0, digit_count = 0, factor = 2;
  for (int i = str.size() - 1; i >= 0; --i) {
    if (str[i] == ' ')
      continue;
    if (str[i] < '0' || '9' < str[i])
      return false;
    ++digit_count;
    factor = 3 - factor;
    int digit = (str[i] - '0') * factor;
    if (digit > 9)
      digit -= 9;
    sum += digit;
  }
  return digit_count > 1 && sum % 10 == 0;
}

int main() {
  cout << boolalpha;

  // roll 4 dice and discard the smallest
  int ability = 10;
  auto x = floor_division(ability - 10, 2);
  cout << x << endl;

  uniform_int_distribution<int> dist(1, 6);
  cout << dist(prng()) << endl;
  cout << luhn_valid("123") << endl;
}
