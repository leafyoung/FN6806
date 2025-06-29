// https://replit.com/@YeKunlun/47-polytype

#include <functional>
#include <iostream>
#include <vector>
using namespace std;

#include "K.h"
#include "crtp.h"
#include "inheritance.h"
#include "type_erasure.h"

int main() {
  TypeErasure::test_type_erasure();

  Inheritance::test_inheritance();

  {
    using namespace Inheritance;
    cout << "More tests from inheritance\n";
    unique_ptr<Vehicle> v = make_unique<Car>();
    v->drive();
    v->speedup();
    v->stop();

    if (false) {
      unique_ptr<Vehicle> v2;
      string input;
      cout << "input your choice: \n";
      if (cin >> input) {
        v2 = getVehicle(input);
        v2->drive();
      }
    }
  }

  {
    using namespace CRTP;
    test_crtp();
  }

  cout << "Test K/Ksub\n";
  test_k();

  if (false) {
    cout << "Test K/Ksub: waiting to crash\n";
    test_k_loop();
  }
}