// https://replit.com/@YeKunlun/4c-variantvisitcxx20

#include <iostream>
#include <string>
#include <variant>
using namespace std;

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

struct PrintVisitor {
  void operator()(int &i) const { std::cout << "int: " << i; }

  void operator()(float &f) const { std::cout << "float: " << f; }

  void operator()(const std::string &s) const { std::cout << "string: " << s; }
};

int main() {
  {
    std::variant<int, float, std::string> intFloatString{"Hello"};
    std::visit(
        overload{
            [](const int &i) { std::cout << "int: " << i << '\n'; },
            [](const float &f) { std::cout << "float: " << f << '\n'; },
            [](const std::string &s) { std::cout << "string: " << s << '\n'; }},
        intFloatString);
    cout << '\n';
  }
  {
    PrintVisitor p;
    int i = 1;
    float j = 3.14;
    string s{"hello"s};
    p(i);
    p(j);
    p(s);
    cout << '\n';
  }

  {
    std::variant<Circle, Square, Triangle> shape;
    shape = Triangle{};
    auto callDraw = [](auto &sh) { sh.Draw(); };
    std::visit(callDraw, shape);
  }
}
