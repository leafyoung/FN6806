#include <stdexcept>
using namespace std;

namespace pricing {
template <typename T> class HasDelta {
protected:
  const double bump_size = 0.0001;

public:
  T &self() { return static_cast<T &>(*this); }
  const T &self() const { return static_cast<const T &>(*this); }

  // common method shared by all
  double delta() const {
    const double underlying = self().get_underlying();
    const double bump_up = self().pv(underlying + self().bump_size);
    const double bump_down = self().pv(underlying - self().bump_size);
    const double delta = (bump_up - bump_down) / self().bump_size / 2;
    return delta;
  };
};

class Futures : public HasDelta<Futures> {
  double amount;
  double strike;
  double underlying;
  double df;

public:
  Futures(double amount, double strike, double underlying, double df)
      : amount(amount), strike(strike), underlying(underlying), df(df) {}
  double get_underlying() const { return underlying; };
  double pv(double underlying) const {
    return amount * (underlying - strike) * df;
  };
};

class Swaps : public HasDelta<Swaps> {
  // Commodity swaps is fixed price for floating price during a perid of time.
  // Read more: https://www.investopedia.com/terms/c/commodityswap.asp
  double amount;
  double strike;
  double underlying;
  double past_average;
  double past_time_ratio;
  double df;

public:
  Swaps(double amount, double strike, double underlying, double past_average,
        double past_time_ratio, double df)
      : amount(amount), strike(strike), underlying(underlying),
        past_average(past_average), past_time_ratio(past_time_ratio), df(df) {
    if (past_time_ratio > 1)
      throw logic_error("past_time_ratio shall be < 1");
  }
  double get_underlying() const { return underlying; };
  double pv(double underlying) const {
    // assume we have half-past the pricing period, so 0.5 * underlying and
    // past_average
    return amount *
           (underlying * (1 - past_time_ratio) +
            past_average * past_time_ratio - strike) *
           df;
  };
};
} // namespace pricing