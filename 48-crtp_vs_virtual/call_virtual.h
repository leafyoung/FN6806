#pragma once

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
                    int expected_result) {
  for (int i = 0; i < test_loop; ++i) {
    obj->increment();
  }
  if (obj->value() != expected_result)
    throw logic_error("Failed test");
}
