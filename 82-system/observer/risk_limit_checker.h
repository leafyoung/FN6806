// risk_limit_checker.h
#pragma once

#include <cstddef>
#include <string_view>

#include "risk_observer.h"

class Portfolio;
class Instrument;

// Concrete observer used as an exam extension point.
class RiskLimitChecker : public RiskObserver {
  double max_portfolio_pv_;
  double max_portfolio_dv01_;
  double warning_threshold_;
  bool limit_breached_ = false;

 public:
  RiskLimitChecker(double max_pv = 1e9, double max_dv01 = 1e6, double warning_thresh = 0.8);

  void on_trade_added(const Portfolio& portfolio, const Instrument& instrument,
                      double new_total_pv) override;
  void on_portfolio_loaded(const Portfolio& portfolio, std::size_t instrument_count) override;

  std::string_view name() const override { return "RiskLimitChecker"; }

  bool was_limit_breached() const { return limit_breached_; }
  void reset() { limit_breached_ = false; }
};
