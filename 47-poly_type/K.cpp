#include <memory>
using namespace std;
#include "K.h"

void test_k() {
  unique_ptr<K> k = make_unique<Ksub>();
} // The output is ~K, not ~Ksub

void test_k_loop() {
  // causing memory leak
  while (true)
    unique_ptr<K> k = make_unique<Ksub>();
}