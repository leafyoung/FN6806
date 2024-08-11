#pragma once

#include <vector>
using std::vector;

template <template <typename, typename> class Interpolator, typename T1,
          typename T2>
class TermStructure {
  Interpolator<T1, T2> interpolator;

public:
  TermStructure(const vector<T1> &dates, const vector<T2> &values)
      : interpolator(dates, values) {}
  T2 interpolate(const T1 &date) const { return interpolator(date); }
};

template <typename Interpolator> class TermStructureWithoutType {
  Interpolator interpolator;

public:
  TermStructureWithoutType(const vector<typename Interpolator::T1_type> &dates,
                           const vector<typename Interpolator::T2_type> &values)
      : interpolator(dates, values) {}
  typename Interpolator::T2_type
  interpolate(const typename Interpolator::T1_type &date) const {
    return interpolator(date);
  }
};