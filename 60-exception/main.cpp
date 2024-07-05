#include <fstream>
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
  sqrt(-1.0);
  1 / 0;
  1.0 / 0.0;

  char x = 3000;
  cout << x << endl;

  vector<int> xs = {1, 2, 3};
  cout << xs[3] << "\n";

  string ss = "123";
  cout << ss.at(0) << "\n";
  cout << ss.c_str()[10] << "\n";
  cout << ss[10] << "\n";
  cout << ss.at(10) << "\n";
}

int main() {
  // { test_exp(); }
  { test_read_file(3); }
  { test_stream(); }
  { test_vector_exception_demo(); }
  { test_vector_exception(); }
  { test_zip_vector(); }
  { test_user_exception(); }
  { test_dtor(); }
  { test_matrix_exception(); }
  { test_user_input(); }

  return 0;
}