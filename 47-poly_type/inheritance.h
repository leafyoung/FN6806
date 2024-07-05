#pragma once

#include <iostream>
#include <memory>
using namespace std;

namespace Inheritance {
class Vehicle {
public:
  virtual void drive() = 0;
  virtual void speedup() = 0;
  // FIXME:
  void stop() { cout << "stop" << endl; }
  virtual Vehicle &self() { return *this; }
  virtual ~Vehicle() = default;
};

class Car : public Vehicle {
public:
  void drive() override { cout << "Car~" << endl; }
  void speedup() override { cout << "Car++~" << endl; }
  void stop() { cout << "Car--|" << endl; }
  Car &self() override { return *this; }
};
class Truck : public Vehicle {
public:
  void drive() override { cout << "Truck>" << endl; }
  void speedup() override { cout << "Truck++>" << endl; }
  void stop() { cout << "Truck--<" << endl; }
  Truck &self() override { return *this; }
};
class Plane : public Vehicle {
public:
  void drive() override { cout << "Plane/" << endl; }
  void speedup() override { cout << "Plane++/" << endl; }
  void stop() { cout << "Plane--\\" << endl; }
  Plane &self() override { return *this; }
};

// This is the factory function to help us create object.
unique_ptr<Vehicle> getVehicle(const string &choice);

class VehicleFactory {
public:
  static std::unique_ptr<Vehicle> create(const std::string &choice) {
    if (choice == "car") {
      return make_unique<Car>();
    } else if (choice == "truck") {
      return make_unique<Truck>();
    } else if (choice == "plane") {
      return make_unique<Plane>();
    }
    throw logic_error("unknown choice: "s + choice);
  }
};

void test_inheritance();

} // namespace Inheritance
