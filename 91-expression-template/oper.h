#pragma once
#include "base_expr.h"

struct plus_ {
  template <typename T, typename U>
  auto operator()(T const &t, U const &u) const {
    return t + u;
  }
};

struct minus_ {
  template <typename T, typename U>
  auto operator()(T const &t, U const &u) const {
    return t - u;
  }
};

struct multiply_ {
  template <typename T, typename U>
  auto operator()(T const &t, U const &u) const {
    return t * u;
  }
};

struct divide_ {
  template <typename T, typename U>
  auto operator()(T const &t, U const &u) const {
    return t / u;
  }
};

struct negate_ {
  template <typename T> auto operator()(T const &t) const { return -t; }
};

template <typename E1> auto operator-(BaseExpr<E1> &e1) {
  return UnaryExpr<negate_, E1>(e1.self());
}

template <typename E1, typename E2>
auto operator+(BaseExpr<E1> const &e1, BaseExpr<E2> const &e2) {
  return BinaryExpr<plus_, E1, E2>{e1.self(), e2.self()};
};

template <typename E1, typename E2>
auto operator-(BaseExpr<E1> const &e1, BaseExpr<E2> const &e2) {
  return BinaryExpr<minus_, E1, E2>{e1.self(), e2.self()};
};

template <typename E1, typename E2>
auto operator*(BaseExpr<E1> const &e1, BaseExpr<E2> const &e2) {
  return BinaryExpr<multiply_, E1, E2>{e1.self(), e2.self()};
};

template <typename E1, typename E2>
auto operator/(BaseExpr<E1> const &e1, BaseExpr<E2> const &e2) {
  return BinaryExpr<divide_, E1, E2>{e1.self(), e2.self()};
};
