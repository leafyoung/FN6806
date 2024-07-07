// https://replit.com/@YeKunlun/31-printcontainertemplate

#include <array>
#include <iostream>
#include <list>
#include <map>
#include <tuple>
#include <type_traits>
#include <vector>
using namespace std;

struct ABC {
  void abc() const {}
};
struct DEF {
  void def() const {}
};

template <int Days> struct DayCount {
  const int day_count = Days;
};

using DayCount360 = DayCount<360>;
using DayCount365 = DayCount<365>;

template <typename T, T x> bool larger_than_x(const T &a) { return a > x; }

template <typename T> void get_abc(const T &coll) {
  static_assert(is_same<T, ABC>::value, "T must be of type ABC.");
  coll.abc();
}

template <template <typename, typename> class U, typename T, typename allocType>
T get_3rd_element(U<T, allocType> &coll) {
  return coll[2];
}

template <typename T> void print_container_basic(const T &coll) {
  for (auto const &v : coll) {
    cout << v << ", ";
  }
  cout << "\n";
}

void print_container_auto(auto const &coll) {
  for (auto const &v : coll) {
    cout << v << ", ";
  }
  cout << "\n";
}

template <template <typename, typename> class ContainerType, typename ValueType,
          typename AllocType>
void print_container(const ContainerType<ValueType, AllocType> &c) {
  for (const auto &v : c) {
    cout << v << ' ';
  }
  cout << '\n';
}

// Implement << for pairs: this is needed to print out mappings where range
// iteration goes over (key, value) pairs.
template <typename T, typename U>
ostream &operator<<(ostream &out, const pair<T, U> &p) {
  out << "[" << p.first << ", " << p.second << "]";
  return out;
}

// Using Variadic Templates (C++11)
template <template <typename, typename...> class ContainerType,
          typename ValueType, typename... Args>
void print_container_var(const ContainerType<ValueType, Args...> &c) {
  for (const auto &v : c) {
    cout << v << ' ';
  }
  cout << '\n';
}

#include <initializer_list>

class TimeSeriesNT {
  vector<double> data;

public:
  // TimeSeriesNT(initializer_list<double> data) : data(data)
  // {}
  TimeSeriesNT(const vector<double> &data) : data(data) {}

  void printData() const {
    for (const auto &value : data) {
      cout << value << " ";
    }
    cout << "\n";
  }
};

template <typename T> class TimeSeries {
  vector<T> data;

public:
  // TimeSeries(initializer_list<T> data) : data(data) {}
  TimeSeries(const vector<T> &data) : data(data) {}

  void printData() const {
    for (const auto &value : data) {
      cout << value << ", ";
    }
    cout << "\n";
  }
};

int main() {
  {
    auto t1 = TimeSeries<double>({1.1, 2.1, 3.1, 4.1});
    auto t2 = TimeSeries<int>({1, 2, 3, 4});
    t1.printData();
    t2.printData();
  }
  {
    cout << is_same<DayCount<360>, DayCount<365>>::value << "\n";
    DayCount360 dc_360;
    DayCount365 dc_365;
    cout << dc_360.day_count << "\n";
    cout << dc_365.day_count << "\n";
  }
  {
    cout << boolalpha;
    auto x0 = vector<int>{1, 2, 3};
    auto x = get_3rd_element<vector, int>(x0);
    cout << x << "\n";

    auto abc = ABC();
    get_abc(abc); // OK
                  //  get_abc(DEF()); // ERROR

    cout << larger_than_x<int, 3>(5) << "\n";
    cout << larger_than_x<int, 6>(5) << "\n";
  }
  {
    cout << "test print_container_basic()" << "\n";
    print_container_basic(vector<int>{3, 4});
    print_container_basic(vector<string>{"def", "abc"});
    print_container_basic(list<double>{3.0, 4.0});
    cout << "\n";

    cout << "test print_container_auto()" << "\n";
    print_container_auto(vector<int>{3, 4});
    print_container_auto(vector<string>{"def", "abc"});
    print_container_auto(list<double>{3.0, 4.0});
    cout << "\n";
  }
  {
    cout << "test print_container()" << "\n";
    vector<double> vd{3.14, 8.1, 3.2, 1.0};
    print_container(vd);

    list<int> li{1, 2, 3, 5};
    print_container(li);

    print_container(vector<int>{3, 4});
    print_container(vector<string>{"def", "abc"});
    print_container(list<double>{3.0, 4.0});

    for (auto p : map<int, string>{{1, "abc"}, {2, "def"}}) {
      cout << p << " ";
      // print_container(p);
    }
    cout << "\n";

    auto m = map<int, string>{{1, "abc"}, {2, "def"}};
    print_container_var(m);
  }
}