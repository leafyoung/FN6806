// portfolio.h
#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "instrument.h"

class RiskObserver;
class YieldCurve;

struct PortfolioLoadResult {
  std::size_t loaded_count = 0;
  std::size_t skipped_count = 0;
};

class Portfolio {
  std::vector<std::unique_ptr<Instrument>> instruments_;
  std::vector<std::weak_ptr<RiskObserver>> observers_;
  std::shared_ptr<YieldCurve> observer_curve_;
  double observer_total_pv_ = 0.0;

  // Observer extension points used in the final exam.
  void prune_expired_observers();
  // Notifies observers when a trade is added.
  void notify_trade_added(const Instrument& instrument, double new_total_pv);
  // Notifies observers when portfolio loading completes.
  void notify_portfolio_loaded();

 public:
  Portfolio() = default;

  template <typename Func>
  void for_each(Func&& f) const {
    for (const auto& instrument : instruments_) {
      f(*instrument);
    }
  }

  static Portfolio from_csv(const std::string& path,
                            const std::shared_ptr<YieldCurve>& observer_curve);
  PortfolioLoadResult load_csv(const std::string& path);

  // Registers a non-owning observer handle.
  void register_observer(const std::shared_ptr<RiskObserver>& observer);
  void unregister_observer(const std::shared_ptr<RiskObserver>& observer);
  std::size_t observer_count() const {
    return std::count_if(observers_.begin(), observers_.end(),
                         [](const auto& observer) { return !observer.expired(); });
  }

  void set_observer_curve(const std::shared_ptr<YieldCurve>& curve);
  const std::shared_ptr<YieldCurve>& observer_curve() const { return observer_curve_; }
  double observer_total_pv() const { return observer_total_pv_; }

  void add(std::unique_ptr<Instrument> instrument);
  double total_pv(const YieldCurve& curve) const;
  double total_dv01(const YieldCurve& curve) const;

  std::size_t size() const { return instruments_.size(); }
};
