// yield_curve.h
#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class YieldCurve {
  std::vector<double> tenors_;
  std::vector<double> rates_;

public:
  YieldCurve(std::vector<double> tenors, std::vector<double> rates);

  int size() const { return static_cast<int>(tenors_.size()); }
  // Load from CSV file - throws std::runtime_error on failure
  static std::shared_ptr<YieldCurve> from_csv(const std::string &path);
  double rate_at(double tenor) const; // linear interpolation
  void parallel_shift(double spread); // modifies all rates
};
