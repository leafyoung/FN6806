// risk_observer.h
#pragma once

#include <cstddef>
#include <string_view>

class Portfolio;
class Instrument;

class RiskObserver {
 public:
  virtual ~RiskObserver() = default;

  virtual void on_trade_added(const Portfolio& portfolio, const Instrument& instrument,
                              double new_total_pv) = 0;
  virtual void on_portfolio_loaded(const Portfolio& portfolio, std::size_t instrument_count) = 0;
  virtual std::string_view name() const = 0;
};
