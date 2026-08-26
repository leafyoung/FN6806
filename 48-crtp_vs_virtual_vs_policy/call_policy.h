#include <iostream>
#include <memory>
#include <string>
using namespace std;

namespace Policy {

template <typename T> class PolicyBase {
  unique_ptr<T> t;

public:
  PolicyBase(unique_ptr<T> t) : t(std::move(t)) {}
  // interface functions starts here.
  void increment() { return t->increment(); };
  int value() const { return t->value(); };
};

class Policy1 {
  int i = 0;

public:
  void increment() { ++i; };
  int value() const { return i; }
};

class Policy10 {
  int i = 0;

public:
  void increment() { i += 10.0; };
  int value() const { return i; }
};

template <typename T>
void runPolicyCall(unique_ptr<PolicyBase<T>> obj, int test_loop,
                   int expected_result) {
  for (int i = 0; i < test_loop; ++i) {
    obj->increment();
  }
  if (obj->value() != expected_result)
    throw logic_error("Failed test");
}

} // namespace Policy