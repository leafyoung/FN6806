#pragma once

template <typename Derived> struct BaseExpr {
  Derived const &self() const { return static_cast<const Derived &>(*this); }
  auto operator()() const { return self()(); }
};

template <typename Tag, typename A>
struct UnaryExpr : public BaseExpr<UnaryExpr<Tag, A>> {
  explicit UnaryExpr(const A &a) : ca{a} {}
  auto operator()() const { return op(ca()); }

private:
  A ca;
  Tag op;
};

template <typename Tag, typename A, typename B>
struct BinaryExpr : public BaseExpr<BinaryExpr<Tag, A, B>> {
  BinaryExpr(const A &a, const B &b) : ca{a}, cb{b} {}
  auto operator()() const { return op(ca(), cb()); }

private:
  A ca;
  B cb;
  Tag op;
};

template <typename V> struct Value : public BaseExpr<Value<V>> {
  Value() = default;
  explicit Value(const V &v) : val{v} {}

  template <typename X> Value operator=(BaseExpr<X> const &x) {
    val = x();
    return *this;
  }

  operator Value() const { return val; }
  auto operator()() const { return val; }

private:
  V val;
};
