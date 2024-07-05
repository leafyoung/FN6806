#include <iostream>
#include <memory>
using namespace std;

namespace CRTP {

class AbstractVehicle {
  public:
    virtual void drive() = 0;
    virtual void speedup() = 0;
    virtual void stop() = 0;
};
    
template <typename Derived> class Vehicle {
  inline Derived &self() { return static_cast<Derived &>(*this); }
  inline const Derived &self() const { return static_cast<const Derived &>(*this); }

public:
  void drive() { self().drive_impl(); };
  void speedup() { self().speedup_impl(); };
  void stop() { self().stop_impl(); }

  // little overhead in
  virtual ~Vehicle() = default;
};

class Car : public Vehicle<Car> {
public:
  void drive_impl() { cout << "Car~" << endl; }
  void speedup_impl() { cout << "Car++~" << endl; }
  void stop_impl() { cout << "Car--|" << endl; }
};

class Truck : public Vehicle<Truck> {
public:
  void drive_impl() { cout << "Truck>" << endl; }
  void speedup_impl() { cout << "Truck++>" << endl; }
  void stop_impl() { cout << "Truck--<" << endl; }
};
class Plane : public Vehicle<Plane> {
public:
  void drive_impl() { cout << "Plane/" << endl; }
  void speedup_impl() { cout << "Plane++/" << endl; }
  void stop_impl() { cout << "Plane--\\" << endl; }
};

void test_crtp();

template <typename T> void operateVehicle(unique_ptr<Vehicle<T>> v) {
  v->drive();
  v->speedup();
  v->stop();
}


template <typename T> void operateVehicle(Vehicle<T> &v) {
  v.drive();
  v.speedup();
  v.stop();
}

}; // namespace CRTP