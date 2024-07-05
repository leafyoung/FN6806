#include <functional>
#include <iostream>
#include <vector>
using namespace std;

#include "type_erasure.h"

namespace TypeErasure {

template <typename Vehicle> void call_drive(Vehicle v) { v.drive(); }

void test_type_erasure() {
  cout << "Call from function<void()>" << endl;
  vector<function<void()>> vehicles_drive;
  vehicles_drive.emplace_back([]() { Car().drive(); });
  vehicles_drive.emplace_back([]() { Truck().drive(); });
  vehicles_drive.emplace_back([]() { Plane().drive(); });

  for (auto &v : vehicles_drive) {
    v();
  }
  cout << endl;

  cout << "Call from function template" << endl;
  call_drive(Car());
  call_drive(Truck());
  call_drive(Plane());
  cout << endl;
}
} // namespace TypeErasure