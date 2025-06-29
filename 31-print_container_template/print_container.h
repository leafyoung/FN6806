#pragma once

#include <iostream>
using namespace std;

template <template <typename, typename> class U, typename T, typename allocType>
T get_3rd_element(U<T, allocType> &coll) {
  return coll[2];
}

template <typename T> void print_container_basic(const T &coll) {
  for (auto const &v : coll) {
    cout << v << ", ";
  }
  cout << '\n';
}

template <template <typename, typename> class ContainerType, typename ValueType,
          typename AllocType>
void print_container(const ContainerType<ValueType, AllocType> &c) {
  for (const auto &v : c) {
    cout << v << ' ';
  }
  cout << '\n';
}

// Implement << for pairs: this is needed to print out mappings where range
// iteration goes over (key, value) pairs.
template <typename T, typename U>
ostream &operator<<(ostream &out, const pair<T, U> &p) {
  out << "[" << p.first << ", " << p.second << "]";
  return out;
}

// Using Variadic Templates (C++11)
template <template <typename, typename...> class ContainerType,
          typename ValueType, typename... Args>
void print_container_var(const ContainerType<ValueType, Args...> &c) {
  for (const auto &v : c) {
    cout << v << ' ';
  }
  cout << '\n';
}

void test_print_container();
