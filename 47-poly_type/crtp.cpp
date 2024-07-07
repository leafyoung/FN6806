#include <iostream>
#include <memory>
using namespace std;
#include "crtp.h"

namespace CRTP {

void test_crtp() {

  cout << "Call from CRTP" << "\n";

  {
    Car car;
    car.drive();

    Truck truck;
    truck.drive();

    unique_ptr<Vehicle<Car>> c1 = make_unique<Car>();
    unique_ptr<Vehicle<Truck>> t1 = make_unique<Truck>();
    unique_ptr<Vehicle<Plane>> p1 = make_unique<Plane>();

    operateVehicle(move(c1));
    operateVehicle(move(t1));
    operateVehicle(move(p1));
  }
}

} // namespace CRTP