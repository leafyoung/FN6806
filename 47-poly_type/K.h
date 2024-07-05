#pragma once
#include <iostream>
#include <vector>
using namespace std;

class K {
public:
  // virtual ~K() = default;
  // For demo, we make some print here and not virtual.
  ~K() { cout << "~K" << endl; };
};

class Ksub : public K {
  vector<int> x;

public:
  // allocate large memory
  Ksub() : x(1'000'000'000, 0) {}
  ~Ksub() { cout << "~Ksub" << endl; };
};

void test_k();

void test_k_loop();