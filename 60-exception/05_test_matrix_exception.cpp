#include "05_test_matrix_exception.h"

void test_matrix_exception() {
  try {
    throw MatrixExceptionStandalone(
        "Out of bound from MatrixExceptionStandalone");
  } catch (const MatrixExceptionStandalone &e) {
    std::cout << e.what() << "\n";
  }

  try {
    throw MatrixException("Out of bound from MatrixException - logic_error");
  } catch (const MatrixException &e) {
    std::cout << e.what() << "\n";
  }

  try {
    throw MatrixOutOfBoundsException(3, 56);
  } catch (const MatrixOutOfBoundsException &e) {
    std::cout << e.what() << "\n";
  }

  try {
    throw MatrixException17("Out of bound from 17");
  } catch (const MatrixException17 &e) {
    std::cout << e.what() << "\n";
  }
}