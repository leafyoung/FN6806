// european_option.h
#pragma once
#include "instrument.h"

class EuropeanOption : public Instrument {
  double spot_, strike_, vol_, T_;
  bool is_call_;

  // Helper: standard normal CDF
  static double N(double x);
  static double n(double x);  // PDF

 public:
  EuropeanOption(std::string id, double spot, double strike, double vol, double T, bool is_call);

  std::string type_name() const override { return "EuropeanOption"; }
  double price(const YieldCurve& curve) const override;
  double duration(const YieldCurve& curve) const override;

  double delta(const YieldCurve& curve) const;  // ∂P/∂S
  double vega(const YieldCurve& curve) const;   // ∂P/∂σ
  double gamma(const YieldCurve& curve) const;  // ∂²P/∂S²
};