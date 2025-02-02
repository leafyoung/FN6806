#pragma once

#include <stdexcept>
#include <tuple>
#include <vector>
using std::logic_error;
using std::make_tuple;
using std::size_t;
using std::tuple;
using std::vector;

#include "termstructure.h"

namespace InterpolatorTemplate {

template <typename T1, typename T2> class Interpolator {
public:
  using T1_type = T1;
  using T2_type = T2;
  using Series1 = vector<T1_type>;
  using Series2 = vector<T2_type>;

  Interpolator(const Series1 &dates, const Series2 &values)
      : dates(dates), values(values) {
    if (dates.size() != values.size())
      throw logic_error("unequal length x and y");
  }

protected:
  Series1 dates;
  Series2 values;
  tuple<bool, T2> flat_extrapolate(const T1 &date) const {
    if (date <= dates.front())
      return {true, values.front()};
    if (date >= dates.back())
      return {true, values.back()};
    return {false, 0.0};
  }
};

template <typename T1, typename T2>
class LinearInterpolator : public Interpolator<T1, T2> {
public:
  using Interpolator<T1, T2>::Interpolator;

  // overload operator() to make the object to be called like a function.
  T2 operator()(const T1 &date) const {
    /*
    Instead of leaving below code in each function
    I have moved it to the base class as flat_extrapolate()

        if (date < this->dates.front())
          return this->values.front();
        if (date > this->dates.back())
          return this->values.back();
    */
    auto [b, v] = this->flat_extrapolate(date);
    if (b)
      return v;
    size_t i = 0;
    while (date > this->dates[i])
      ++i;
    return this->values[i - 1] + (this->values[i] - this->values[i - 1]) /
                                     (this->dates[i] - this->dates[i - 1]) *
                                     (date - this->dates[i - 1]);
  }
};

template <typename T1, typename T2>
class SquaredInterpolator : public Interpolator<T1, T2> {
public:
  using Interpolator<T1, T2>::Interpolator;

  T2 operator()(const T1 &date) const {
    auto [b, v] = this->flat_extrapolate(date);
    if (b)
      return v;
    size_t i = 0;
    while (date > this->dates[i])
      ++i;
    const T2 values_i1_s = this->values[i - 1] * this->values[i - 1];
    const T2 values_i_s = this->values[i] * this->values[i];
    return sqrt(values_i1_s + (values_i_s - values_i1_s) /
                                  (this->dates[i] - this->dates[i - 1]) *
                                  (date - this->dates[i - 1]));
  }
};

} // namespace InterpolatorTemplate