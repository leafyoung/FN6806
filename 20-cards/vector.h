#pragma once

#include <cassert>
#include <vector>

class Matrix;

class Vector {
  friend class Matrix;
  std::vector<double> data;

public:
  Vector(std::vector<double> data) : data(data){};
  Matrix operator+(const Matrix &other);
};
