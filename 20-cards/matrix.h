#pragma once

#include <vector>

class Vector;

class Matrix {
  friend class Vector;
  std::vector<double> data;

public:
  Matrix(std::vector<double> data) : data(data){};
  Matrix operator+(const Vector &other);
};
