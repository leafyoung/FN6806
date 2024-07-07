// https://replit.com/@YeKunlun/48-crtpvsvirtual

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

#include "call_crtp.h"
#include "call_virtual.h"

const int TEST_RUN = 50000;
const double TEST_RUN_D = static_cast<double>(TEST_RUN);

int main() {
  {
    auto obj = make_unique<VirtualBy1>();
    runVirtualCall(move(obj), 1, 1);
  }
  {
    unique_ptr<BaseVirtual> obj1 = make_unique<VirtualBy1>();
    runVirtualCall(move(obj1), 1, 1);
    unique_ptr<BaseVirtual> obj10 = make_unique<VirtualBy10>();
    runVirtualCall(move(obj10), 1, 10);
  }
  {
    unique_ptr<CRTPBase<CRTPBy1>> obj1 = make_unique<CRTPBy1>();
    runCRTPCall(move(obj1), 1, 1);
    unique_ptr<CRTPBase<CRTPBy10>> obj10 = make_unique<CRTPBy10>();
    runCRTPCall(move(obj10), 1, 10);
  }
  {
    unique_ptr<BaseVirtual> obj = make_unique<VirtualBy1>();
    auto start = chrono::high_resolution_clock::now();
    runVirtualCall(move(obj), TEST_RUN, TEST_RUN);
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do runVirtualCall(): "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
         << "ns" << "\n";
  }
  {
    unique_ptr<CRTPBase<CRTPBy1>> obj = make_unique<CRTPBy1>();
    auto start = chrono::high_resolution_clock::now();
    runCRTPCall(move(obj), TEST_RUN, TEST_RUN);
    auto end = chrono::high_resolution_clock::now();
    cout << "Time to do runCRTPCall(): "
         << static_cast<chrono::nanoseconds>(end - start).count() / TEST_RUN_D
         << "ns" << "\n";
  }
}