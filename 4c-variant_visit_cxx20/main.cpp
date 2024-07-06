#include <iostream>
#include <string>
#include <variant>
using namespace std;

template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> overload(Ts...) -> overload<Ts...>;

struct Circle {
  void Draw() const { cout << "Circle" << endl; }
};
struct Square {
  void Draw() const { cout << "Square" << endl; }
};
struct Triangle {
  void Draw() const { cout << "Triangle" << endl; }
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
            [](const int &i) { std::cout << "int: " << i << endl; },
            [](const float &f) { std::cout << "float: " << f << endl; },
            [](const std::string &s) { std::cout << "string: " << s << endl; }},
        intFloatString);
    cout << endl;
  }
  {
    PrintVisitor p;
    int i = 1;
    float j = 3.14;
    string s{"hello"s};
    p(i);
    p(j);
    p(s);
    cout << endl;
  }

  {
    std::variant<Circle, Square, Triangle> shape;
    shape = Triangle{};
    auto callDraw = [](auto &sh) { sh.Draw(); };
    std::visit(callDraw, shape);
  }
}
