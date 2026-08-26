#include <array>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

template <typename T, std::size_t N> class Arr : public std::array<T, N> {
public:
  using array<T, N>::array;
  // std::array's constructor is implicit.
  Arr(std::initializer_list<T> l) : array<T, N>::array() {
    copy(l.begin(), l.end(), this->begin());
  }
};

template <typename T> class Vec : public std::vector<T> {
  using vector<T>::vector;
};

template <typename T> class List : public std::list<T> {
  using list<T>::list;
};

int main() {
  // Arr<int, 5> a{1, 2, 3, 4, 5};
  Arr<int, 5> a;
  Arr<int, 3> b{1, 2, 3};
  array<int, 3> c{1, 2, 3};
  vector<int> d{1, 2, 3};
  Vec<int> e{1, 2, 3};
  list<int> f{1, 2, 3};
  List<int> g{1, 2, 3};

  for (auto v : a) {
    cout << v << endl;
  }
}