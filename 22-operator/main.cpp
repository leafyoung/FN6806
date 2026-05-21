// https://replit.com/@YeKunlun/22-operator

#include <algorithm>
#include <array>
#include <ios>
#include <iostream>
#include <numeric>  // for std::accumulate
#include <vector>
using namespace std;

struct MultiplyAndAdd {
  int prev_result = 0;
  int operator()(int a, int b) { return prev_result += a * b; }
};

// Enable C++17
struct KeepAdd {
  inline static int sum = 0;
  void operator()(int a) const { sum += a; }
};

class MyBool {
  int x;

 public:
  MyBool(int x1) : x{x1} {}
  operator bool() const { return x == 0; }
};

// ── Explicit conversion operator ───────────────────────────────────────────
// Mark operator T() explicit to prevent silent implicit conversions.
class Price {
  double value_;

 public:
  explicit Price(double v) : value_(v) {}
  explicit operator double() const { return value_; }  // explicit: no silent conv
};

// ── Subscript operator with const/non-const overloads ───────────────────────
// Provide both overloads so const objects can be read, non-const can be written.
class TimeSeries {
  std::vector<double> data_;

 public:
  TimeSeries(std::initializer_list<double> il) : data_(il) {}
  double& operator[](size_t i) { return data_[i]; }       // write access
  double operator[](size_t i) const { return data_[i]; }  // read-only access
  size_t size() const { return data_.size(); }
};

// ── Pre/post increment ───────────────────────────────────────────────────────
// Dummy int parameter distinguishes postfix from prefix.
// Rule: prefer prefix (++it) — no temporary copy needed.
struct Counter {
  int pos = 0;
  Counter& operator++() {
    ++pos;
    return *this;
  }  // prefix:  ++c
  Counter operator++(int) {
    Counter tmp = *this;
    ++(*this);
    return tmp;
  }  // postfix: c++
  // Decrement mirrors increment:
  Counter& operator--() {
    --pos;
    return *this;
  }  // prefix:  --c
  Counter operator--(int) {
    Counter tmp = *this;
    --(*this);
    return tmp;
  }  // postfix: c--
};

int main() {
  // Stateful function with callable class
  {
    auto varr = std::array<int, 3>{{2, 3, 4}};
    int factorial = std::accumulate(varr.begin(), varr.end(), 1, MultiplyAndAdd{});
    cout << factorial << '\n';  // 40
    // accumulate: acc=1, op(1,2)=1*2, op(2,3)=2+2*3=8, op(8,4)=8+8*4=40

    KeepAdd keepadd;
    for_each(varr.begin(), varr.end(), [keepadd = keepadd](auto x) { keepadd(x); });
    cout << keepadd.sum << '\n';  // 9
    for_each(varr.begin(), varr.end(), keepadd);
    cout << keepadd.sum << '\n';  // 18
  }

  {
    MyBool my_bool{3};
    // calls operator bool(), b = false
    bool b = static_cast<bool>(my_bool);
    cout << std::boolalpha << b << '\n';  // false
  }

  // Explicit conversion operator
  {
    Price p{99.5};
    // double d = p;              // compile error: explicit required
    double d = static_cast<double>(p);  // ok
    cout << d << '\n';                  // 99.5
  }

  // Subscript with const overload
  {
    TimeSeries ts{1.0, 2.0, 3.0};
    ts[0] = 1.5;            // calls non-const overload
    cout << ts[0] << '\n';  // 1.5
    const TimeSeries cts{4.0, 5.0};
    cout << cts[1] << '\n';  // 5 (const overload)
  }

  // Pre/post increment
  {
    Counter c;
    cout << (++c).pos << '\n';  // 1  (prefix: c already incremented)
    cout << (c++).pos << '\n';  // 1  (postfix: returns old value)
    cout << c.pos << '\n';      // 2
    cout << (--c).pos << '\n';  // 1
  }
}