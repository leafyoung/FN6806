#pragma once

#include <vector>
using namespace std;

template <class T, class U> class zip_vector {
  vector<T> _vs;
  vector<U> _us;

public:
  void push_back(T &&v, U &&u) {
    _vs.push_back(move(v));
    _us.push_back(move(u));
  }
  auto operator()(size_t i) { return make_tuple(_vs[i], _us[i]); }
  auto operator()(size_t i, const T &v, const U &u) {
    _vs[i] = v;
    _us[i] = u;
    return make_tuple(_vs[i], _us[i]);
  }
  explicit operator int() { return _vs.size(); }
  explicit operator vector<int>() { return _vs; }
  explicit operator vector<U>() { return _us; }
};

void test_zip_vector();