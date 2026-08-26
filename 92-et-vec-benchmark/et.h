#pragma once
#include <array>
#include <initializer_list>
#include <iostream>
using namespace std;

template <typename E> class VecExpression {
public:
  static constexpr bool is_leaf = false;

  E const &self() const { return static_cast<const E &>(*this); }
  // E &self() { return static_cast<E &>(*this); }
  double operator[](size_t i) const { return self()[i]; }
  size_t size() const { return self().size(); }
};

template <size_t N> class Vec : public VecExpression<Vec<N>> {
  array<double, N> elems;

public:
  static constexpr bool is_leaf = true;

  auto operator[](size_t i) const { return elems[i]; }
  auto &operator[](size_t i) { return elems[i]; }
  size_t size() const { return elems.size(); }
  Vec() : elems{} {}
  Vec(initializer_list<double> init) {
    copy(init.begin(), init.end(), elems.begin());
  }
  template <typename E> Vec(VecExpression<E> const &expr) {
    if (expr.size() != N) {
      throw logic_error("Not the same size");
    }
    for (size_t i = 0; i != expr.size(); ++i) {
      elems[i] = expr[i];
    }
  }

  friend ostream &operator<<(ostream &os, const Vec<N> &obj) {
    for (size_t i = 0; i < N; ++i) {
      os << i << ": ";
      os << obj.elems[i] << ", ";
    }
    return os;
  }
};

template <typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1, E2>> {
  // cref if leaf, copy otherwise
  std::conditional_t<E1::is_leaf, const E1 &, const E1> _u;
  std::conditional_t<E2::is_leaf, const E2 &, const E2> _v;
  // const E1 &_u;
  // const E2 &_v;

public:
  static constexpr bool is_leaf = false;
  VecSum(E1 const &u, E2 const &v) : _u(u), _v(v) {
    // cout << _u.size() << ", " << _v.size() << endl;
    if (_u.size() != _v.size()) {
      throw logic_error("Not the same size");
    }
  }
  auto operator[](size_t i) const {
    // cout << "[" << i << "]: " << _u[i] << "," << _v[i] << endl;
    return _u[i] + _v[i];
  }
  size_t size() const { return _u.size(); }
};

template <typename E1, typename E2>
VecSum<E1, E2> operator+(VecExpression<E1> const &u,
                         VecExpression<E2> const &v) {
  return VecSum<E1, E2>(*static_cast<const E1 *>(&u),
                        *static_cast<const E2 *>(&v));
  // return VecSum<E1, E2>(u.self(), v.self());
}
