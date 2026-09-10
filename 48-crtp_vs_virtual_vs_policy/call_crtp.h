#pragma once
#include <memory>
#include <stdexcept>
using namespace std;

class CRTPVirtual {
public:
  virtual ~CRTPVirtual() {}
  virtual void increment() = 0;
  virtual int value() const = 0;
};

// The moment we introduce virtual as its base class, speed of CRTPBase drops to
// the same as VirtualCall.
// template <typename T> class CRTPBase : public CRTPVirtual {
template <typename T> class CRTPBase {
  // special function to resolve the function call
  // Two versions for non-const and const access.
  inline T &self() { return static_cast<T &>(*this); }
  inline const T &self() const { return static_cast<const T &>(*this); }

public:
  // A virtual destructor is required so that a
  // `unique_ptr<CRTPBase<T>>` can delete the concrete derived object.
  // It gives CRTPBase a vtable, but dispatch for increment()/value() is
  // still static via self() - only destruction is virtual.
  virtual ~CRTPBase() = default;
  // interface functions starts here.
  void increment() { return self().increment(); };
  int value() const { return self().value(); };
};

class CRTPBy1 : public CRTPBase<CRTPBy1> {
  int i = 0;

public:
  void increment() { ++i; };
  int value() const { return i; }
};

class CRTPBy10 : public CRTPBase<CRTPBy10> {
  int i = 0;

public:
  void increment() { i += 10; };
  int value() const { return i; }
};

template <typename T>
void runCRTPCall(unique_ptr<CRTPBase<T>> obj, size_t test_loop,
                 int expected_result) {
  for (size_t i = 0; i < test_loop; ++i) {
    obj->increment();
  }
  if (obj->value() != expected_result)
    throw logic_error("Failed test");
}
