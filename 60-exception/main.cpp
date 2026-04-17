// https://replit.com/@YeKunlun/60-exception

#include <iostream>
using namespace std;

#include "01_test_vector.h"
#include "02_test_stream.h"
#include "03_test_user.h"
#include "04_test_input.h"
#include "05_test_matrix_exception.h"
#include "06_test_read_file.h"
#include "07_zip_vector.h"

#include <cmath>
using std::isnan;
using std::sqrt;

int test_exp() {
  const double root = sqrt(-1.0);
  cout << "sqrt(-1.0) is NaN: " << isnan(root) << '\n';

  cout << "1.0 / 0.0 -> " << (1.0 / 0.0) << '\n';

  const char x = 3000;
  cout << x << '\n';

  vector<int> xs = {1, 2, 3};
  string ss = "123";
  cout << ss.at(0) << '\n';

  // Intentionally cause out-of-range exceptions to demonstrate undefined
  // behaviors
  cout << xs[3] << '\n';           // undefined behavior: unchecked access
  cout << ss.c_str()[10] << '\n';  // undefined behavior: unchecked access
  cout << ss[10] << '\n';          // undefined behavior: unchecked access
  cout << ss.at(10) << '\n';       // throws std::out_of_range
  return 0;
}

int main() {
  try {
    // test_exp();
  } catch (const std::exception& e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }
  test_dtor();
  test_read_file(3);
  test_stream();
  test_vector_exception_demo();
  test_vector_exception();
  test_zip_vector();
  test_user_exception();
  test_matrix_exception();
  test_user_input();

  return 0;
}