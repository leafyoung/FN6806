// https://replit.com/@YeKunlun/22-operator

#include <algorithm>
#include <array>
#include <ios>
#include <iostream>
#include <numeric> // for std::accumulate
#include <vector>
using namespace std;

struct MultiplyAndAdd {
  int prev_result = 0;
  int operator()(int a, int b) { return prev_result += a * b; }
};

struct KeepAdd {
  inline static int sum = 0; // Enable C++17 to use inline
  void operator()(int a) const { sum += a; }
};

// conversion via bool() to bool, via int() to int
class MyBool {
  int x;

public:
  explicit MyBool(int x1) : x{x1} {}
  operator bool() const { return x == 0; }
  explicit operator int() const { return x; }
};

// ── Subscript operator with const/non-const overloads ───────────────────────
// Provide both overloads so const objects can be read, non-const can be
// written.
class TimeSeries {
  std::vector<double> data_;

public:
  TimeSeries(std::initializer_list<double> il) : data_(il) {}
  double &operator[](size_t i) { return data_[i]; }      // write access
  double operator[](size_t i) const { return data_[i]; } // read-only access
  size_t size() const { return data_.size(); }
};

// ── Pre/post increment ───────────────────────────────────────────────────────
// Dummy int parameter distinguishes postfix from prefix.
// Rule: prefer prefix (++it) — no temporary copy needed.
struct Counter {
  int pos = 0;
  Counter &operator++() {
    ++pos;
    return *this;
  } // prefix:  ++c
  Counter operator++(int) {
    Counter tmp = *this;
    ++(*this);
    return tmp;
  } // postfix: c++
  // Decrement mirrors increment:
  Counter &operator--() {
    --pos;
    return *this;
  } // prefix:  --c
  Counter operator--(int) {
    Counter tmp = *this;
    --(*this);
    return tmp;
  } // postfix: c--
};

int main() {
  // Stateful function with callable class
  {
    auto varr = std::array<int, 3>{{2, 3, 4}};
    int factorial =
        std::accumulate(varr.begin(), varr.end(), 1, MultiplyAndAdd{});
    cout << "factorial: " << factorial << '\n'; // 40
    // accumulate: acc=1, op(1,2)=1*2, op(2,3)=2+2*3=8, op(8,4)=8+8*4=40

    KeepAdd keepadd;
    for_each(varr.begin(), varr.end(),
             [keepadd = keepadd](auto x) { keepadd(x); });
    cout << "sum: " << keepadd.sum << '\n'; // 9
    for_each(varr.begin(), varr.end(), keepadd);
    cout << "sum: " << keepadd.sum << '\n'; // 18
  }

  {
    cout << "\nMyBool:\n";
    MyBool my_bool = 3;
    // calls operator bool(), b = false
    bool b = my_bool;
    cout << std::boolalpha << "bool: " << b << '\n';      // implicit, false
    cout << "int: " << static_cast<int>(my_bool) << '\n'; // explicit, 3
  }

  // Subscript with const overload
  {
    cout << "\nTimeSeries:\n";
    TimeSeries ts{1.0, 2.0, 3.0};
    cout << "initial: " << ts[0]
         << '\n'; // 1.0 (non-const overload; ts is not const)
    ts[0] = 1.5;  // calls non-const overload
    cout << "updated: " << ts[0] << '\n'; // 1.5
  }

  // Pre/post increment
  {
    cout << "\nCounter:\n";
    Counter c;
    cout << "initial: " << c.pos << '\n';
    cout << "(++c): " << (++c).pos
         << '\n'; // 1  (prefix: c already incremented)
    cout << "(c++): " << (c++).pos << '\n'; // 1  (postfix: returns old value)
    cout << "c: " << c.pos << '\n';         // 2
    cout << "(--c): " << (--c).pos << '\n'; // 1
    cout << "(c--): " << (c--).pos << '\n'; // 1
    cout << "c: " << c.pos << '\n';         // 0
  }
}