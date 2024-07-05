#pragma once

#include <tuple>
#include <vector>
using std::size_t;
using std::tuple;
using std::vector;

#include "inter1.h"

namespace T2 {

// Saves some types from T1

template <typename Interpolator> class TermStructure {
  Interpolator interpolator;

public:
  TermStructure(const vector<typename Interpolator::T1_type> &dates,
                const vector<typename Interpolator::T2_type> &values)
      : interpolator(dates, values) {}
  typename Interpolator::T2_type
  interpolate(const typename Interpolator::T1_type &date) const {
    return interpolator(date);
  }
};

} // namespace T2