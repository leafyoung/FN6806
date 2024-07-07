#include "Shape.h"
#include <memory>
#include <vector>
using namespace std;

void test_shape() {
  // 1. same interface
  // auto will deduce the type as unique_ptr<Rectangle>
  auto s1 = make_unique<Rectangle>(2, 3);
  cout << s1->area() << "\n"; // 6
  unique_ptr<Rectangle> s2 = make_unique<Rectangle>(2, 3);
  cout << s2->area() << "\n"; // 6

  // We can use unique_ptr<Shape> to hold the instance from the derived class
  unique_ptr<Shape> s3 = make_unique<Rectangle>(2, 3);
  cout << s3->area() << "\n"; // 6
  unique_ptr<Shape> s4 = make_unique<Circle>(3);
  cout << s4->area() << "\n"; // 28.2743

  printArea(*s1);
  printArea(*s2);
  printArea(*s3);
  printArea(*s4);

  // 2. Store all derived class instances in a single vector
  // Set the type for vector as unique_ptr<BaseClass>
  vector<unique_ptr<Shape>> shapes;
  shapes.emplace_back(make_unique<Rectangle>(2, 2));
  shapes.emplace_back(make_unique<Circle>(4));

  for (const auto &x : shapes) {
    cout << x->area() << "\n";
  }

  // Template can offer this
  // But it's the pure virtual function that enforce the interface in the
  // derived classes
  {
    printAreaT(*s1);
    printAreaT(*s2);
  }
}