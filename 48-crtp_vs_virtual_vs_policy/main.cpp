// https://replit.com/@YeKunlun/48-crtpvsvirtual

#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>

using namespace std;

#include "call_crtp.h"
#include "call_policy.h"
#include "call_virtual.h"

const int TEST_RUN = 50000;
const double TEST_RUN_D = static_cast<double>(TEST_RUN);

void runVirtualCall(unique_ptr<BaseVirtual> obj, int test_loop, int expected_result) {
  for (int i = 0; i < test_loop; ++i) {
    obj->increment();
  }
  if (obj->value() != expected_result)
    throw logic_error("Failed test");
}


int main() {
  {
    auto obj = make_unique<VirtualBy1>();
    runVirtualCall(std::move(obj), 1, 1);
  }
  {
    unique_ptr<BaseVirtual> obj = make_unique<VirtualBy1>();
    runVirtualCall(std::move(obj), 1, 1);
  }
  {
    // Test VirtualBy10
    unique_ptr<BaseVirtual> obj = make_unique<VirtualBy10>();
    runVirtualCall(std::move(obj), 1, 10);
  }

  {
    unique_ptr<CRTPBase<CRTPBy1>> obj = make_unique<CRTPBy1>();
    runCRTPCall(std::move(obj), 1, 1);
  }

  {
    unique_ptr<CRTPBase<CRTPBy10>> obj = make_unique<CRTPBy10>();
    runCRTPCall(std::move(obj), 1, 10);
  }

  {
    unique_ptr<BaseVirtual> obj = make_unique<VirtualBy1>();
    auto start = chrono::high_resolution_clock::now();
    runVirtualCall(std::move(obj), TEST_RUN, TEST_RUN);
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do runVirtualCall(): "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
         << "ns" << endl;
  }

  {
    unique_ptr<CRTPBase<CRTPBy1>> obj = make_unique<CRTPBy1>();
    auto start = chrono::high_resolution_clock::now();
    runCRTPCall(std::move(obj), TEST_RUN, TEST_RUN);
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do runCRTPCall(): "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
         << "ns" << endl;
  }

  {
    using namespace Policy;

    auto obj = make_unique<PolicyBase<Policy1>>(make_unique<Policy1>());
    auto start = chrono::high_resolution_clock::now();
    runPolicyCall(std::move(obj), TEST_RUN, TEST_RUN);
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do runPolicyCall(): "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
         << "ns" << endl;
  }

  {
    vector<unique_ptr<BaseVirtual>> vec;
    vec.emplace_back(make_unique<VirtualBy1>());
    vec.emplace_back(make_unique<VirtualBy10>());
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TEST_RUN; ++i) {
      vec[0]->increment();
      vec[1]->increment();
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do runVirtualCall->increment() in vector: "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D /
                2
         << "ns" << endl;
  }

  {
    array<function<void()>, 2> vec;
    auto obj1 = make_unique<VirtualBy1>();
    auto obj2 = make_unique<VirtualBy10>();
    vec[0] = [&obj1]() { obj1->increment(); };
    vec[1] = [&obj2]() { obj2->increment(); };
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TEST_RUN; ++i) {
      vec[0]();
      vec[1]();
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do runVirtualCall->increment() in vector: "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D /
                2
         << "ns" << endl;
  }

  {
    array<function<void()>, 2> vec;
    unique_ptr<CRTPBase<CRTPBy1>> obj1 = make_unique<CRTPBy1>();
    unique_ptr<CRTPBase<CRTPBy10>> obj2 = make_unique<CRTPBy10>();
    vec[0] = [&obj1]() { obj1->increment(); };
    vec[1] = [&obj2]() { obj2->increment(); };
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TEST_RUN; ++i) {
      vec[0]();
      vec[1]();
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do CRTPBy1->increment() in vector of function: "
         << static_cast<chrono::nanoseconds>(end - start).count() /
                static_cast<double>(TEST_RUN) / 2
         << "ns" << endl;
  }

  {
    using namespace Policy;
    array<function<void()>, 2> vec;
    auto obj1 = make_unique<PolicyBase<Policy1>>(make_unique<Policy1>());
    auto obj2 = make_unique<PolicyBase<Policy10>>(make_unique<Policy10>());
    vec[0] = [&obj1]() { obj1->increment(); };
    vec[1] = [&obj2]() { obj2->increment(); };
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TEST_RUN; ++i) {
      vec[0]();
      vec[1]();
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do PolicyBase->increment() in vector of function: "
         << static_cast<chrono::nanoseconds>(end - start).count() /
                static_cast<double>(TEST_RUN) / 2
         << "ns" << endl;
  }

  // Test using virtual to enable CRTPBy1 to be put into the
  // container.
  /*
  {
    vector<unique_ptr<CRTPVirtual>> vec;
    vec.emplace_back(make_unique<CRTPBy1>());
    vec.emplace_back(make_unique<CRTPBy1>());
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TEST_RUN; ++i) {
      vec[0]->increment();
      vec[1]->increment();
    }
    vec[0]->value();
    vec[1]->value();
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do CRTPBy1->increment() in vector: "
         << static_cast<chrono::nanoseconds>(end - start).count() /
                static_cast<double>(TEST_RUN) / 2
         << endl;
  }
  */
}