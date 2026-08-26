#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
using namespace std;

const int TEST_RUN = 50000;
const double TEST_RUN_D = static_cast<double>(TEST_RUN);

class Shape {
  function<void()> f_incr;

public:
  Shape() = delete;
  Shape(function<void()> f_incr) : f_incr(f_incr) {}
  virtual ~Shape() = default;
  void incr() { return f_incr(); }
};

class Square : public Shape {
  int s = 3;

public:
  Square() : Shape([this]() { this->incr(); }) {}
  void incr() { ++s; }
};

class Circle : public Shape {
  int c = 4;

public:
  Circle() : Shape([this]() { return this->incr(); }) {}
  void incr() { ++c; }
};

int main() {
  auto s = Square();
  s.incr();
  auto c = Circle();
  c.incr();

  unique_ptr<Shape> obj = make_unique<Square>();
  auto start = chrono::high_resolution_clock::now();
  for (size_t i = 0; i < TEST_RUN; ++i) {
    obj->incr();
  }
  auto end = chrono::high_resolution_clock::now();
  cout << "Time to call Function pointer(): "
       << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
       << "ns" << endl;
}