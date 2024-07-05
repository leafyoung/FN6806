#pragma once
#include <iostream>
using namespace std;

namespace TypeErasure {

struct Car {
  void drive() { cout << "Car" << endl; }
};
struct Truck {
  void drive() { cout << "Truck" << endl; }
};
struct Plane {
  void drive() { cout << "Plane" << endl; }
};

void test_type_erasure();
} // namespace TypeErasure