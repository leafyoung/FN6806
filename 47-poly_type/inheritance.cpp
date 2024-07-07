#include "inheritance.h"

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

namespace Inheritance {

void test_drive(Vehicle &v) { v.drive(); }

// Not compilable as Vehicle is an abstract class
// void test_drive2(Vehicle v) { v.drive(); }

// This is the factory function to help us create object.
unique_ptr<Vehicle> getVehicle(const string &choice) {
  if (choice == "car") {
    return make_unique<Car>();
  } else if (choice == "truck") {
    return make_unique<Truck>();
  } else if (choice == "plane") {
    return make_unique<Plane>();
  }
  throw logic_error("unknown choice: "s + choice);
}

void operateVehicle(unique_ptr<Vehicle> v) {
  v->drive();
  v->speedup();
  v->stop();
}

void operateVehicle(Vehicle &v) {
  v.drive();
  v.speedup();
  v.stop();
}

void test_inheritance() {
  cout << "Call from inheritance/reference" << "\n";

  Car c;
  Truck tr;
  Plane pl;
  Vehicle &v1{c}, &v2{tr}, &v3{pl};
  v1.drive();
  v2.drive();
  v3.drive();

  cout << "Call from inheritance/pointer" << "\n";

  unique_ptr<Vehicle> vp1 = make_unique<Car>();
  test_drive(*vp1);

  // test_drive2(*vp1);

  vector<unique_ptr<Vehicle>> vehicles;
  /*
   // With factory function, we don't need to know the detail of how to
   // create object

   // replaced following code with call to getVehicle()
    vehicles.emplace_back(make_unique<Car>());
    vehicles.emplace_back(make_unique<Truck>());
    vehicles.emplace_back(make_unique<Plane>());
  */

  vehicles.emplace_back(getVehicle("car"s));
  vehicles.emplace_back(getVehicle("truck"s));
  vehicles.emplace_back(getVehicle("plane"s));

  for (auto &v : vehicles) {
    v->drive();
    v->speedup();
    v->stop();
  }

  cout << "Call from inheritance/template" << "\n";
  operateVehicle(getVehicle("car"s));

  cout << "Return from virtual method" << "\n";
  auto &r1 = vp1->self();
  r1.drive();
  Vehicle &v = vp1->self();
  v.drive();

  cout << "\n";
}

} // namespace Inheritance
