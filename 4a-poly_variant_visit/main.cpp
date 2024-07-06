// https://replit.com/@YeKunlun/4a-polyvariantvisit

// Need --std=c++17 or c++20

#include <iostream>
#include <variant>
#include <vector>
using namespace std;

class Circle {};

class Square {};

class Draw {
public:
  void operator()(Circle const &c) const {
    std::cout << "Draw circle"
              << "\n";
  }
  void operator()(Square const &c) const {
    std::cout << "Draw square"
              << "\n";
  }
};

using Shapes = std::variant<Circle, Square>;
void drawAllShapes(std::vector<Shapes> const &shapes) {
  for (auto const &s : shapes) {
    std::visit(Draw{}, s);
  }
}

int main() { drawAllShapes(vector<Shapes>{Circle{}, Square{}, Circle{}}); }