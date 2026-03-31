// european_option.h
#pragma once
#include "instrument.h"

class EuropeanOption : public Instrument {
  double spot_, strike_, rate_, vol_, T_;
  bool is_call_;

  // Helper: standard normal CDF
  static double N(double x);
  static double n(double x); // PDF

public:
  EuropeanOption(std::string id, double spot, double strike, double rate,
                 double vol, double T, bool is_call);

  std::string type_name() const override { return "EuropeanOption"; }
  double price(double rate) const override;    // Black-Scholes
  double duration(double rate) const override; // dP/dr (rho)

  double delta() const; // ∂P/∂S - non-virtual, specific to options
  double vega() const;  // ∂P/∂σ
  double gamma() const; // ∂²P/∂S²
};