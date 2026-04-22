// instrument.h
#pragma once
#include <memory>
#include <string>

#include "yield_curve.h"

class Instrument {
  std::string id_;

 public:
  explicit Instrument(std::string id) : id_(std::move(id)) {}
  const std::string& id() const { return id_; }
  virtual ~Instrument() = default;  // MUST be virtual (Rule: Lec04)

  virtual std::string type_name() const = 0;
  virtual double price(const YieldCurve& curve) const = 0;
  virtual double duration(const YieldCurve& curve) const = 0;

  // Non-virtual DV01: finite difference using virtual price()
  double dv01(const YieldCurve& curve, double bump = 1e-4) const {
    YieldCurve bumped_up = curve;
    YieldCurve bumped_down = curve;
    bumped_up.parallel_shift(bump);
    bumped_down.parallel_shift(-bump);
    return (price(bumped_up) - price(bumped_down)) / (2.0 * bump);
  }
};
