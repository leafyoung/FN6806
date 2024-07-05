#include "07_zip_vector.h"
#include <iostream>
#include <tuple>
using namespace std;

void test_zip_vector() {
  auto zv = zip_vector<int, string>();

  zv.push_back(3, "abc"s);
  zv.push_back(5, "uvb"s);
  cout << get<0>(zv(0)) << "," << get<1>(zv(0)) << "\n";
  zv(0, 4, "def"s);
  cout << get<0>(zv(0)) << "," << get<1>(zv(0)) << "\n";

  cout << int(zv) << "\n";
  auto xxx = vector<int>(zv);
  cout << xxx.size() << "\n";
  for (auto v : xxx) {
    cout << v << "\n";
  }
  for (auto v : vector<string>{zv}) {
    cout << v << "\n";
  }
}