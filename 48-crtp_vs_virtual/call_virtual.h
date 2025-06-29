#pragma once
#include <memory>
#include <stdexcept>

using std::logic_error;
using std::unique_ptr;

class BaseVirtual {
public:
  virtual ~BaseVirtual() = default;
  virtual void increment() = 0;
  virtual int value() const = 0;
};

class VirtualBy1 : public BaseVirtual {
  int i = 0;

public:
  virtual void increment() override { ++i; }
  virtual int value() const override { return i; }
};

class VirtualBy10 : public BaseVirtual {
  int i = 0;

public:
  virtual void increment() override { i += 10; }
  virtual int value() const override { return i; }
};

void runVirtualCall(unique_ptr<BaseVirtual> obj, int test_loop,
                    int expected_result);