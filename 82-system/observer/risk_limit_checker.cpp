// risk_limit_checker.cpp
#include "risk_limit_checker.h"

#include <stdexcept>

#include "../instrument.h"
#include "../logger.h"
#include "../portfolio.h"

RiskLimitChecker::RiskLimitChecker(double max_pv, double max_dv01, double warning_thresh)
    : max_portfolio_pv_(max_pv), max_portfolio_dv01_(max_dv01), warning_threshold_(warning_thresh) {
  if (max_portfolio_pv_ <= 0.0) {
    throw std::invalid_argument("RiskLimitChecker: max_pv must be positive");
  }

  if (max_portfolio_dv01_ <= 0.0) {
    throw std::invalid_argument("RiskLimitChecker: max_dv01 must be positive");
  }

  if (warning_threshold_ <= 0.0 || warning_threshold_ > 1.0) {
    throw std::invalid_argument("RiskLimitChecker: warning threshold must be in (0, 1]");
  }

  logging::info("RiskLimitChecker")
      << "event=observer_construct status=success max_pv=" << max_portfolio_pv_
      << " max_dv01=" << max_portfolio_dv01_ << " warning_threshold=" << warning_threshold_;
}

void RiskLimitChecker::on_trade_added(const Portfolio& portfolio, const Instrument& instrument,
                                      double new_total_pv) {
  (void)portfolio;
  (void)instrument;
  (void)new_total_pv;
  // TODO(FN6806 exam): evaluate PV and DV01 limits after a trade is added.
}

void RiskLimitChecker::on_portfolio_loaded(const Portfolio& portfolio,
                                           std::size_t instrument_count) {
  (void)portfolio;
  (void)instrument_count;
  // TODO(FN6806 exam): report initial monitoring state after portfolio load.
}
