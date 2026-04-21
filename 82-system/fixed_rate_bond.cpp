// fixed_rate_bond.cpp
#include "fixed_rate_bond.h"

#include <cmath>
#include <stdexcept>
#include <utility>

#include "logger.h"

namespace {

struct BondAnalytics {
  double present_value = 0.0;
  double weighted_time = 0.0;
};

double discount_factor(double rate, int time_step) {
  return std::exp(-rate * static_cast<double>(time_step));
}

BondAnalytics calculate_bond_analytics(double face, double coupon, int maturity, double rate) {
  BondAnalytics analytics;

  for (int time_step = 1; time_step <= maturity; ++time_step) {
    const double coupon_cashflow = coupon * face;
    const double discounted_coupon = coupon_cashflow * discount_factor(rate, time_step);

    analytics.present_value += discounted_coupon;
    analytics.weighted_time += static_cast<double>(time_step) * discounted_coupon;
  }

  const double discounted_face = face * discount_factor(rate, maturity);
  analytics.present_value += discounted_face;
  analytics.weighted_time += static_cast<double>(maturity) * discounted_face;

  return analytics;
}

}  // namespace

FixedRateBond::FixedRateBond(std::string id, double face, double coupon, int maturity)
    : Instrument(std::move(id)), face_(face), coupon_(coupon), maturity_(maturity) {
  if (face_ <= 0.0) {
    logging::error("FixedRateBond") << "event=bond_construct status=failed reason=invalid_face face=" << face_;
    throw std::invalid_argument("FixedRateBond: face must be positive");
  }

  if (maturity_ <= 0) {
    logging::error("FixedRateBond") << "event=bond_construct status=failed reason=invalid_maturity maturity=" << maturity_;
    throw std::invalid_argument("FixedRateBond: maturity must be positive");
  }

  logging::info("FixedRateBond") << "event=bond_construct status=success instrument_id="
                                << this->id() << " face=" << face_ << " coupon=" << coupon_
                                << " maturity=" << maturity_;
}

double FixedRateBond::price(double rate) const {
  const BondAnalytics analytics = calculate_bond_analytics(face_, coupon_, maturity_, rate);

  logging::debug("FixedRateBond") << "event=bond_price status=success instrument_id="
                                 << id() << " rate=" << rate << " pv=" << analytics.present_value;
  return analytics.present_value;
}

double FixedRateBond::duration(double rate) const {
  const BondAnalytics analytics = calculate_bond_analytics(face_, coupon_, maturity_, rate);
  const double duration = analytics.weighted_time / analytics.present_value;

  logging::debug("FixedRateBond") << "event=bond_duration status=success instrument_id="
                                 << id() << " rate=" << rate << " duration=" << duration;
  return duration;
}
