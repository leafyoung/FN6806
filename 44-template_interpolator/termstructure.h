#pragma once

#include <vector>
using std::vector;

// Improved version of internalize the types
template <typename Interpolator> class TermStructure {
  Interpolator interpolator;

public:
  TermStructure(const typename Interpolator::Series1 &dates,
                const typename Interpolator::Series2 &values)
      : interpolator(dates, values) {}
  typename Interpolator::T2_type
  interpolate(const typename Interpolator::T1_type &date) const {
    return interpolator(date);
  }
};

template <template <typename, typename> class Interpolator, typename T1,
          typename T2>
class TermStructureWithType {
  Interpolator<T1, T2> interpolator;

public:
  TermStructureWithType(const typename Interpolator<T1, T2>::Series1 &dates,
                        const typename Interpolator<T1, T2>::Series2 &values)
      : interpolator(dates, values) {}
  T2 interpolate(const T1 &date) const { return interpolator(date); }
};
