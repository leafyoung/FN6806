#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

class MatrixExceptionStandalone {
private:
  const std::string info;

public:
  MatrixExceptionStandalone(const char *s) : info(s) {}
  const char *what() const noexcept { return info.c_str(); }
};

class MatrixException : public std::logic_error {
public:
  MatrixException() : MatrixException("") {};
  explicit MatrixException(const std::string &message)
      : std::logic_error(message) {}
};

class MatrixOutOfBoundsException : public MatrixException {
public:
  MatrixOutOfBoundsException(int row, int col)
      : MatrixException("Matrix out of bounds exception: Row index = " +
                        std::to_string(row) +
                        ", Column index = " + std::to_string(col) + ".") {}
};

//=================

// Use C++17 string_view
class MatrixException17 {
  // : public exception // for handling errors in the class Matrix
private:
  std::string_view info;

public:
  MatrixException17() = default;

  explicit MatrixException17(std::string_view message) : info(message) {}

  const char *what() const noexcept { return info.data(); }
};

//=================

void test_matrix_exception();