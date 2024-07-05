#pragma once

#include <stdexcept>
#include <string>

class MyIntException : public std::runtime_error {
public:
  int x;
  explicit MyIntException(int x)
      : std::runtime_error("Got problem with " + std::to_string(x)), x(x) {}
};

void test_user_exception();
