template <unsigned M, unsigned N> struct gcd {
  static int const value = gcd<N, M % N>::value;
};

// partial specialization
template <unsigned M> struct gcd<M, 0> {
  static_assert(M != 0); // gcd<0, 0> is not defined
  static int const value = M;
};
