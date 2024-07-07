#include <iostream>
#include <string>
#include <type_traits>
using namespace std;

template <typename T> class Widget {
  T t;

public:
  Widget(T t);
  T get() const;
  // we can omit <T> in below line
  // Widget<T> copy() const;
  Widget<T> copy() const;

  // Use member function template to return
  template <typename S> Widget<S> copy() const;
};

// Add template type for functions.
template <typename T> Widget<T>::Widget(T t) : t(t) {}

template <typename T> T Widget<T>::get() const { return t; }

// base case: int -> double, double -> int
template <typename T> template <typename S> Widget<S> Widget<T>::copy() const {
  return Widget<S>(static_cast<S>(t));
}

// specialize to string and double/int
template <> template <> Widget<string> Widget<double>::copy() const {
  return Widget<string>(to_string(t));
}

template <> template <> Widget<string> Widget<int>::copy() const {
  return Widget<string>(to_string(t));
}

/* C++17 enables below to replace above.
template <typename T> template <typename S>
Widget<S> Widget<T>::copy() const {
  if constexpr (is_same<S, string>::value) {
    return Widget<S>(to_string(t));
  } else {
    return Widget<S>(static_cast<S>(t));
  }
}
*/

int main() {
  auto w_double = Widget<double>(3.14);
  cout << "Widget<double>: " << w_double.get() << "\n";

  // Copy from <double> to <int>
  Widget<int> w_int = w_double.copy<int>();
  cout << "Widget<int>: " << w_int.get() << "\n";

  // Copy from <double> to <string>
  Widget<string> w_str = w_double.copy<string>();
  cout << "Widget<string> copy from double: " << w_str.get() << "\n";

  // Copy from <int> to <string>
  Widget<string> w_str2 = w_int.copy<string>();
  cout << "Widget<string> copy from int: " << w_str2.get() << "\n";

  Widget<double> w_double2 = w_int.copy<double>();
  cout << "Widget<double> copy from int: " << w_double2.get() << "\n";
}