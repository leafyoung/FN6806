// yield_curve.cpp
#include "yield_curve.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "logger.h"

namespace {

struct CurvePoint {
  double tenor = 0.0;
  double rate = 0.0;
};

CurvePoint parse_curve_point(const std::string& line, std::size_t line_number) {
  std::string normalized = line;
  std::replace(normalized.begin(), normalized.end(), ',', ' ');

  std::istringstream input(normalized);
  CurvePoint point;
  if (!(input >> point.tenor >> point.rate)) {
    throw std::invalid_argument("invalid curve row at line " + std::to_string(line_number) + ": " +
                                line);
  }

  return point;
}

}  // namespace

YieldCurve::YieldCurve(std::vector<double> tenors, std::vector<double> rates)
    : tenors_(std::move(tenors)), rates_(std::move(rates)) {
  if (tenors_.size() != rates_.size()) {
    logging::error("YieldCurve")
        << "event=curve_construct status=failed reason=size_mismatch tenor_count=" << tenors_.size()
        << " rate_count=" << rates_.size();
    throw std::runtime_error("YieldCurve: tenors and rates must have same size");
  }

  if (tenors_.empty()) {
    logging::error("YieldCurve") << "event=curve_construct status=failed reason=empty_curve";
    throw std::runtime_error("YieldCurve: cannot be empty");
  }

  for (std::size_t index = 1; index < tenors_.size(); ++index) {
    if (tenors_[index] <= tenors_[index - 1]) {
      logging::error("YieldCurve")
          << "event=curve_construct status=failed reason=unsorted_tenors index=" << index
          << " previous_tenor=" << tenors_[index - 1] << " tenor=" << tenors_[index];
      throw std::runtime_error("YieldCurve: tenors must be strictly increasing");
    }
  }

  logging::info("YieldCurve") << "event=curve_construct status=success point_count=" << size()
                              << " first_tenor=" << tenors_.front()
                              << " last_tenor=" << tenors_.back();
}

std::shared_ptr<YieldCurve> YieldCurve::from_csv(const std::string& path) {
  logging::info("YieldCurve") << "event=curve_load status=start path=" << path;

  std::ifstream file(path);
  if (!file.is_open()) {
    logging::error("YieldCurve") << "event=curve_load status=failed reason=open_failed path="
                                 << path;
    throw std::runtime_error("YieldCurve::from_csv: cannot open " + path);
  }

  std::string header;
  if (!std::getline(file, header)) {
    logging::error("YieldCurve") << "event=curve_load status=failed reason=empty_file path="
                                 << path;
    throw std::runtime_error("YieldCurve::from_csv: empty file " + path);
  }

  std::vector<double> tenors;
  std::vector<double> rates;
  std::string line;
  std::size_t line_number = 1;

  while (std::getline(file, line)) {
    ++line_number;
    if (line.empty()) {
      continue;
    }

    const CurvePoint point = parse_curve_point(line, line_number);
    tenors.push_back(point.tenor);
    rates.push_back(point.rate);
  }

  if (tenors.empty()) {
    logging::error("YieldCurve") << "event=curve_load status=failed reason=no_points path=" << path;
    throw std::runtime_error("YieldCurve::from_csv: no data in " + path);
  }

  logging::info("YieldCurve") << "event=curve_load status=complete path=" << path
                              << " point_count=" << tenors.size();

  return std::make_shared<YieldCurve>(std::move(tenors), std::move(rates));
}

double YieldCurve::rate_at(double tenor) const {
  if (tenor <= tenors_.front()) {
    logging::debug("YieldCurve") << "event=curve_rate status=flat_left tenor=" << tenor
                                 << " rate=" << rates_.front();
    return rates_.front();
  }

  if (tenor >= tenors_.back()) {
    logging::debug("YieldCurve") << "event=curve_rate status=flat_right tenor=" << tenor
                                 << " rate=" << rates_.back();
    return rates_.back();
  }

  const auto upper = std::lower_bound(tenors_.begin(), tenors_.end(), tenor);
  const std::size_t upper_index = static_cast<std::size_t>(upper - tenors_.begin());
  const std::size_t lower_index = upper_index - 1;

  const double lower_tenor = tenors_[lower_index];
  const double upper_tenor = tenors_[upper_index];
  const double lower_rate = rates_[lower_index];
  const double upper_rate = rates_[upper_index];
  const double weight = (tenor - lower_tenor) / (upper_tenor - lower_tenor);
  const double interpolated_rate = lower_rate + weight * (upper_rate - lower_rate);

  logging::debug("YieldCurve") << "event=curve_rate status=interpolated tenor=" << tenor
                               << " lower_tenor=" << lower_tenor << " upper_tenor=" << upper_tenor
                               << " rate=" << interpolated_rate;
  return interpolated_rate;
}

void YieldCurve::parallel_shift(double spread) {
  for (double& rate : rates_) {
    rate += spread;
  }

  logging::debug("YieldCurve") << "event=curve_shift status=success spread=" << spread
                               << " point_count=" << size();
}
