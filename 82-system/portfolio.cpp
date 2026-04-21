// portfolio.cpp
#include "portfolio.h"

#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "instrument_csv.h"
#include "logger.h"
#include "observer/risk_observer.h"

namespace {

bool is_blank_line(std::string_view line) {
  for (const char ch : line) {
    if (!std::isspace(static_cast<unsigned char>(ch))) {
      return false;
    }
  }
  return true;
}

}  // namespace

void Portfolio::prune_expired_observers() {
  // TODO(FN6806 exam): remove expired observer handles.
}

void Portfolio::register_observer(const std::shared_ptr<RiskObserver>& observer) {
  (void)observer;
  // TODO(FN6806 exam): store a non-owning observer handle.
}

void Portfolio::unregister_observer(const std::shared_ptr<RiskObserver>& observer) {
  (void)observer;
  // TODO(FN6806 exam): remove the matching observer handle.
}

void Portfolio::notify_trade_added(const Instrument& instrument, double new_total_pv) {
  (void)instrument;
  (void)new_total_pv;
  // TODO(FN6806 exam): notify all live observers after a trade is added.
}

void Portfolio::notify_portfolio_loaded() {
  // TODO(FN6806 exam): notify all live observers after loading completes.
}

void Portfolio::set_observer_rate(double rate) {
  observer_rate_ = rate;
  observer_total_pv_ = total_pv(observer_rate_);

  logging::info("Portfolio") << "event=observer_rate_update status=success rate=" << observer_rate_
                             << " observer_total_pv=" << observer_total_pv_;
}

void Portfolio::add(std::unique_ptr<Instrument> instrument) {
  if (!instrument) {
    logging::error("Portfolio") << "event=instrument_add status=failed reason=null_instrument";
    throw std::invalid_argument("Portfolio::add: instrument is null");
  }

  const double trade_pv = instrument->price(observer_rate_);
  const std::string instrument_id = instrument->id();
  const std::string instrument_type = instrument->type_name();

  instruments_.push_back(std::move(instrument));
  observer_total_pv_ += trade_pv;

  logging::debug("Portfolio") << "event=instrument_add status=success instrument_id="
                              << instrument_id << " instrument_type=" << instrument_type
                              << " trade_pv=" << trade_pv << " portfolio_size=" << size()
                              << " observer_total_pv=" << observer_total_pv_;

  notify_trade_added(*instruments_.back(), observer_total_pv_);
}

double Portfolio::total_pv(double rate) const {
  double total = 0.0;
  for (const auto& instrument : instruments_) {
    total += instrument->price(rate);
  }
  return total;
}

double Portfolio::total_dv01(double rate) const {
  double total = 0.0;
  for (const auto& instrument : instruments_) {
    total += instrument->dv01(rate);
  }
  return total;
}

PortfolioLoadResult Portfolio::load_csv(const std::string& path) {
  logging::info("Portfolio") << "event=portfolio_load status=start path=" << path
                             << " existing_instruments=" << size();

  std::ifstream file(path);
  if (!file.is_open()) {
    logging::error("Portfolio") << "event=portfolio_load status=failed reason=open_failed path="
                                << path;
    throw std::runtime_error("Portfolio::load_csv: cannot open " + path);
  }

  std::string header;
  if (!std::getline(file, header)) {
    logging::error("Portfolio") << "event=portfolio_load status=failed reason=empty_file path="
                                << path;
    throw std::runtime_error("Portfolio::load_csv: empty file " + path);
  }

  PortfolioLoadResult result;
  std::string line;
  std::size_t line_number = 1;

  while (std::getline(file, line)) {
    ++line_number;
    if (is_blank_line(line)) {
      continue;
    }

    try {
      add(instrument_csv::parse_instrument_line(line));
      ++result.loaded_count;
    } catch (const std::invalid_argument& error) {
      ++result.skipped_count;
      logging::warn("Portfolio") << "event=portfolio_load status=skipped path=" << path
                                 << " line=" << line_number << " reason=" << error.what();
    }
  }

  notify_portfolio_loaded();

  logging::info("Portfolio") << "event=portfolio_load status=complete path=" << path
                             << " loaded_count=" << result.loaded_count
                             << " skipped_count=" << result.skipped_count
                             << " total_instruments=" << size();

  return result;
}

Portfolio Portfolio::from_csv(const std::string& path) {
  Portfolio portfolio;
  portfolio.load_csv(path);
  return portfolio;
}
