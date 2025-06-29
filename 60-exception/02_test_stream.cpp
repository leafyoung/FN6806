#include "02_test_stream.h"
#include <fstream>
#include <iostream>
using std::cout;

void test_stream() {
  cout << __FUNCTION__ << ":\n";
  try {
    // Default behavior of stream object is silent of no throwing the exception.
    // After a failure, it will set the failbit data member to a value
    std::ifstream f("doesn't exist");
    std::cout << "failbit: " << f.failbit << '\n';

    // We can throw an exception according to failure with .exceptions()
    f.exceptions(f.failbit);
  } catch (const std::ios_base::failure &e) {
    std::cout << "Caught an ios_base::failure.\n"
              << "Explanatory string: " << e.what() << '\n'
              << "Error code: " << e.code() << '\n';
  }
  cout << '\n';
}
