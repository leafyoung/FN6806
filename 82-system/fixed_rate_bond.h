// fixed_rate_bond.h
#pragma once

#include <string>

#include "instrument.h"

class FixedRateBond : public Instrument {
  double face_;
  double coupon_;
  int maturity_;

 public:
  FixedRateBond(std::string id, double face, double coupon, int maturity);

  std::string type_name() const override { return "FixedRateBond"; }
  double price(const YieldCurve& curve) const override;
  double duration(const YieldCurve& curve) const override;
};
