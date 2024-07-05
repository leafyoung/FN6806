#include <iostream>

// Base class
class Vehicle {
protected:
  std::string manufacturer;
  std::string model;
  int year;

  Vehicle(const std::string &m, const std::string &md, int y)
      : manufacturer(m), model(md), year(y) {}

  void getInfo() const {
    std::cout << "Manufacturer: " << manufacturer << std::endl
              << "Model: " << model << std::endl
              << "Year: " << year << std::endl;
  }
};

// Derived class
class Car : public Vehicle {
private:
  int numberOfDoors;

public:
  Car(const std::string &m, const std::string &md, int y, int doors)
      : Vehicle(m, md, y), numberOfDoors(doors) {}

  void getInfo() const {
    Vehicle::getInfo();
    std::cout << "Number of doors: " << numberOfDoors << std::endl;
  }
};

// Another derived class
class Motorcycle : public Vehicle {
private:
  std::string type;

public:
  Motorcycle(const std::string &m, const std::string &md, int y,
             const std::string &t)
      : Vehicle(m, md, y), type(t) {}

  void getInfo() const {
    Vehicle::getInfo();
    std::cout << "Type: " << type << std::endl;
  }
};

int main() {
  // We make Vehicle's constructor all as protected,
  // so we cannot create its instance.
  // Vehicle veh("Toyota", "Camry", 2020);

  Car car("Toyota", "Camry", 2020, 4);
  Motorcycle motorcycle("Honda", "CBR500R", 2021, "Sport");

  car.getInfo();
  std::cout << "----------" << std::endl;
  motorcycle.getInfo();

  // And a pointer for the Base class can not call any member function.
  // Vehicle *veh2 = new Car("Toyota", "Camry", 2020, 4);
  // veh2->getInfo();

  return 0;
}