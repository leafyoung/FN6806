// https://github.com/leafyoung/FN6806/tree/main/49d-variant_visit_cxx20

// C++20 required: the template lambda []<typename T>(...) below.
// (The `overload` idiom in the first block is C++17.)

#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
using namespace std;

// C++17: build one visitor out of an overload set of lambdas.
template <class... Ts> struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overload(Ts...) -> overload<Ts...>;

struct Circle {
  void Draw() const { cout << "Circle\n"; }
};
struct Square {
  void Draw() const { cout << "Square\n"; }
};
struct Triangle {
  void Draw() const { cout << "Triangle\n"; }
};

int main() {
  {
    // C++17: overload set of lambdas as the visitor.
    variant<int, float, std::string> intFloatString{"Hello"};
    visit(
        overload{
            [](const int &i) { cout << "int: " << i << '\n'; },
            [](const float &f) { cout << "float: " << f << '\n'; },
            [](const std::string &s) { cout << "string: " << s << '\n'; }},
        intFloatString);
    cout << '\n';
  }

  {
    // C++20: a template lambda ([]<typename T>) is itself a visitor, so it can
    // be passed straight to std::visit without an overload set.
    variant<int, float, std::string> v{3.14f};
    visit(
        []<typename T>(const T &x) {
          if constexpr (is_same_v<T, int>)
            cout << "int: " << x << '\n';
          else if constexpr (is_same_v<T, float>)
            cout << "float: " << x << '\n';
          else
            cout << "string: " << x << '\n';
        },
        v);
    cout << '\n';
  }

  {
    // Visiting user-defined alternatives: the visitor calls Draw() on each.
    variant<Circle, Square, Triangle> shape;
    shape = Triangle{};
    auto callDraw = [](auto &sh) { sh.Draw(); };
    visit(callDraw, shape);
  }
}
