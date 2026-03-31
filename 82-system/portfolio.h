// portfolio.h
#pragma once
#include <memory>
#include <vector>

#include "instrument.h"
#include "trade_observer.h"
#include "yield_curve.h"

class Portfolio {
  std::vector<std::unique_ptr<Instrument>> instruments_;
  std::vector<RiskObserver *> observers_; // non-owning

  void notify_observers(double old_pv, double new_pv);

public:
  // Load instruments from CSV; throws on bad data
  static Portfolio from_csv(const std::string &path,
                            std::shared_ptr<YieldCurve> curve);
  // Observer pattern: notify listeners when PV changes materially
  void register_observer(RiskObserver *obs);

  void add(std::unique_ptr<Instrument> instr); // portfolio owns instruments
  double total_pv(double rate) const;
  double total_dv01(double rate) const;
};
