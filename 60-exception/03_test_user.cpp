#include "03_test_user.h"
#include <iostream>
#include <stdexcept>

void test_user_exception() {
  try {
    throw 1; // throw an integer
  } catch (const double &i) {
    std::cout << "Caught double: " << i << "\n";
  } catch (const int &i) {
    std::cout << "Caught int: " << i << "\n";
  }

  try {
    // better with an object to include the data and also describe the error
    throw MyIntException(1);
  } catch (const MyIntException &e) {
    std::cout << e.what() << "\n";
  } catch (const std::runtime_error &e) {
    // MyIntException is derived from runtime_error
    // it can also be caught with it.
    std::cout << "runtime_error: " << e.what() << "\n";
  }
}