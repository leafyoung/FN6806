// fixed_rate_bond.h
#pragma once
#include "instrument.h"
#include "yield_curve.h"
#include <memory>

class FixedRateBond : public Instrument {
  double face_, coupon_;
  int maturity_;
  std::shared_ptr<YieldCurve> curve_; // shared_ptr: many bonds share one curve

public:
  FixedRateBond(std::string id, double face, double coupon, int maturity,
                std::shared_ptr<YieldCurve> curve);

  std::string type_name() const override { return "FixedRateBond"; }
  double price(double rate) const override;
  double duration(double rate) const override;
};