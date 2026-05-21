// Demonstrates why the `override` specifier is mandatory.
//
// Without `override`, a typo in the function signature silently creates a
// NEW, non-virtual function instead of overriding the base-class one.
// With `override`, the compiler catches the mismatch immediately.

#include <iostream>
#include <memory>

using std::cout;
using std::make_unique;
using std::unique_ptr;

struct Vehicle {
  virtual void drive() const { cout << "Vehicle::drive\n"; }
  virtual void stop() const { cout << "Vehicle::stop\n"; }
  virtual ~Vehicle() = default;
};

struct Car : Vehicle {
  void drive() const override { cout << "Car::drive\n"; }  // OK: overrides base
  void stop() const override { cout << "Car::stop\n"; }    // OK: overrides base
};

// ── Pitfall: typo in function name silently creates a new function ──────────
//
// Suppose the developer wrote Drive() (capital D) instead of drive().
// Without `override`, the compiler accepts it as a brand-new function.
// The vtable for Vehicle still points drive() → Vehicle::drive().
// A base-class pointer calling v->drive() dispatches to Vehicle::drive(),
// NOT CarBuggy::Drive() — the intent is silently lost.
struct CarBuggy : Vehicle {
  // Missing `drive() override` — Drive() does NOT override anything
  void Drive() const { cout << "CarBuggy::Drive\n"; }  // new function, NOT an override
  void stop() const override { cout << "CarBuggy::stop\n"; }
  //
  // If this had been written as:
  //   void Drive() const override { ... }
  // the compiler would immediately error: 'Drive' does not override any base class member.
  // With override, the typo is caught at declaration — not silently at runtime.
};

void test_override_virtual() {
  cout << "test_override_virtual:\n";

  // Static binding (compile-time type known)
  Car car;
  car.drive();  // Car::drive

  // Dynamic binding (virtual dispatch through base pointer)
  unique_ptr<Vehicle> v = make_unique<Car>();
  v->drive();  // Car::drive  (virtual dispatch)
  v->stop();   // Car::stop   (virtual dispatch)

  // Pitfall demo: CarBuggy::Drive() does NOT override Vehicle::drive()
  unique_ptr<Vehicle> vb = make_unique<CarBuggy>();
  vb->drive();  // Vehicle::drive  ← silent bug: Drive() was intended but not called
  vb->stop();   // CarBuggy::stop  ← correct: properly marked override
  CarBuggy cb;
  cb.Drive();  // CarBuggy::Drive ← reachable only through static type

  cout << '\n';
}
