#include <functional>
#include <iostream>
#include <vector>
using namespace std;

#include "type_erasure.h"

namespace TypeErasure {

template <typename Vehicle> void call_drive(Vehicle v) { v.drive(); }

void test_type_erasure() {
  cout << "Call from function<void()>" << "\n";
  vector<function<void()>> vehicles_drive;
  vehicles_drive.emplace_back([]() { Car().drive(); });
  vehicles_drive.emplace_back([]() { Truck().drive(); });
  vehicles_drive.emplace_back([]() { Plane().drive(); });

  for (auto &v : vehicles_drive) {
    v();
  }
  cout << "\n";

  cout << "Call from function template" << "\n";
  call_drive(Car());
  call_drive(Truck());
  call_drive(Plane());
  cout << "\n";
}
} // namespace TypeErasure