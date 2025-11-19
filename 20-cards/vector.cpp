#include "vector.h"
#include "matrix.h"

Matrix Vector::operator+(const Matrix &other) {
  assert(other.data.size() == data.size());
  return Matrix({1, 2});
}