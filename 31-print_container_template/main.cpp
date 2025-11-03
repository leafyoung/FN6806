// https://replit.com/@YeKunlun/31-printcontainertemplate

#include <iostream>
#include <type_traits>
using namespace std;

#include "gcd.h"
#include "print_container.h"
#include "time_series.h"

struct ABC {
  void abc() const {}
};
struct DEF {
  void def() const {}
};

template <typename T, T x> bool larger_than_x(const T &a) { return a > x; }

template <typename T> void get_abc(const T &coll) {
  static_assert(is_same<T, ABC>::value, "T must be of type ABC.");
  coll.abc();
}

template <int Days> struct DayCount { const int day_count = Days; };

// Each specialization is a class
// To use them conveniently, we use the following shortcut
using DayCount360 = DayCount<360>;
using DayCount365 = DayCount<365>;

int main() {
  {
    cout << boolalpha;
    cout << is_same<DayCount<360>, DayCount<365>>::value << '\n';
    DayCount360 dc_360;
    DayCount365 dc_365;
    cout << dc_360.day_count << '\n';
    cout << dc_365.day_count << '\n';
  }

  {
    cout << boolalpha;
    auto x0 = vector<int>{1, 2, 3};
    auto x = get_3rd_element<vector, int>(x0);
    cout << x << '\n';

    auto abc = ABC();
    get_abc(abc); // OK
                  //  get_abc(DEF()); // ERROR

    cout << larger_than_x<int, 3>(5) << '\n';
    cout << larger_than_x<int, 6>(5) << '\n';
  }

  test_print_container();

  {
    cout << "gcd(11, 33): " << gcd<11, 33>::value << '\n';
    cout << "gcd(33, 11): " << gcd<33, 11>::value << '\n';
    cout << "gcd(26, 65): " << gcd<26, 65>::value << '\n';
    cout << "gcd(65, 26): " << gcd<65, 26>::value << '\n';
  }
}