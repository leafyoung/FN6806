#pragma once

#include <functional>
#include <tuple>
#include <vector>
using std::function;
using std::make_tuple;
using std::size_t;
using std::tuple;
using std::vector;

namespace T1op {

template <typename T1, typename T2> class Interpolator {
public:
  using T1_type = T1;
  using T2_type = T2;
  Interpolator(const vector<T1> &dates, const vector<T2> &values)
      : dates(dates), values(values) {}

protected:
  vector<T1> dates;
  vector<T2> values;
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
  // Same as parent class's constructor, use using.
  // LinearInterpolator(const vector<T1> &dates, const vector<T1> &values)
  //    : dates(dates), values(values) {}
  using Interpolator<T1, T2>::Interpolator;

  // overload operator() to make the object to be called like a function.
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

template <template <typename, typename> class Interpolator, typename T1,
          typename T2>
class TermStructure {
  Interpolator<T1, T2> interpolator;

public:
  TermStructure(const vector<T1> &dates, const vector<T2> &values)
      : interpolator(dates, values) {}
  T2 interpolate(const T1 &date) const { return interpolator(date); }
};

} // namespace T1op