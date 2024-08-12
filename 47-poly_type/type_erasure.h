#pragma once
#include <iostream>
using namespace std;

namespace TypeErasure {

struct Car {
  void drive() {
    cout << "Car"
         << "\n";
  }
};
struct Truck {
  void drive() {
    cout << "Truck"
         << "\n";
  }
};
struct Plane {
  void drive() {
    cout << "Plane"
         << "\n";
  }
};

void test_type_erasure();
} // namespace TypeErasure