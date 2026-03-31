// instrument.h
#pragma once
#include <memory>
#include <string>

class Instrument {
  std::string id_;

public:
  explicit Instrument(std::string id) : id_(std::move(id)) {}
  const std::string &id() const { return id_; }
  virtual ~Instrument() = default; // MUST be virtual (Rule: Lec04)

  virtual std::string type_name() const = 0;
  virtual double price(double rate) const = 0;
  virtual double duration(double rate) const = 0;

  // Non-virtual DV01: finite difference using virtual price()
  double dv01(double rate, double bump = 1e-4) const {
    return (price(rate + bump) - price(rate - bump)) / (2.0 * bump);
  }
};
