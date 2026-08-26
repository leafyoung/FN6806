#include <iostream>
#include <typeinfo>
using namespace std;

template <typename T> T sum(T v) { return v; }
template <typename T, typename... Ts> T sum(T v, Ts... vs) {
  return v + sum<Ts...>(vs...);
}

template <typename T, typename... Ts> void show_types() {
  std::cout << typeid(T).name() << std::endl;
  if constexpr (sizeof...(Ts) != 0) {
    return show_types<Ts...>();
  }
}
int main() {
  // sum2<double>();
  show_types<double, double, string>();
}