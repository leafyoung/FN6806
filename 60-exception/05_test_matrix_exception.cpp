#include "05_test_matrix_exception.h"
using std::cout;

void test_matrix_exception() {
  cout << __FUNCTION__ << ":\n";
  try {
    throw MatrixExceptionStandalone(
        "Out of bound from MatrixExceptionStandalone");
  } catch (const MatrixExceptionStandalone &e) {
    cout << e.what() << '\n';
  }

  try {
    throw MatrixException("Out of bound from MatrixException - logic_error");
  } catch (const MatrixException &e) {
    std::cout << e.what() << '\n';
  }

  try {
    throw MatrixOutOfBoundsException(3, 56);
  } catch (const MatrixOutOfBoundsException &e) {
    cout << e.what() << '\n';
  }

  try {
    throw MatrixException17("Out of bound from 17");
  } catch (const MatrixException17 &e) {
    cout << e.what() << '\n';
  }
  cout << '\n';
}