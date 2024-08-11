#pragma once

#include <functional>
#include <stdexcept>
#include <tuple>
#include <vector>

using std::function;
using std::logic_error;
using std::make_tuple;
using std::size_t;
using std::tuple;
using std::vector;

namespace InterpolatorTemplateWithOp {

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

  T2 with_op(const T1 &date, function<T2(T2)> op_up,
             function<T2(T2)> op_down) const {
    auto [b, v] = flat_extrapolate(date);
    if (b)
      return v;
    size_t i = 0;
    while (date > dates[i])
      ++i;
    const T2 values_i1_up = op_up(values[i - 1]);
    const T2 values_i_up = op_up(values[i]);
    const T2 ratio = (date - dates[i - 1]) / (this->dates[i] - dates[i - 1]);
    return op_down(values_i1_up + (values_i_up - values_i1_up) * ratio);
  }
};

template <typename T1, typename T2>
class LinearInterpolator : public Interpolator<T1, T2> {
public:
  using Interpolator<T1, T2>::Interpolator;

  T2 operator()(const T1 &date) const {
    return this->with_op(
        date, [](auto x) { return x; }, [](auto x) { return x; });
  }
};

template <typename T1, typename T2>
class SquaredInterpolator : public Interpolator<T1, T2> {
public:
  using Interpolator<T1, T2>::Interpolator;

  T2 operator()(const T1 &date) const {
    return this->with_op(
        date, [](auto x) { return x * x; }, [](auto x) { return sqrt(x); });
  }
};

} // namespace InterpolatorTemplateWithOp