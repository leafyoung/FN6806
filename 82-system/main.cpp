// FN6806 Capstone: Portfolio Pricing Engine
// https://replit.com/@YeKunlun/82-system

#include <iostream>

#include "app_config.h"
#include "logger.h"
#include "portfolio.h"
#include "risk_report.h"
#include "yield_curve.h"

namespace {

struct PortfolioSummary {
  double base_rate = 0.0;
  double total_pv = 0.0;
  double total_dv01 = 0.0;
  std::size_t instrument_count = 0;
};

PortfolioSummary compute_summary(const YieldCurve& curve, const Portfolio& portfolio,
                                 double summary_tenor) {
  PortfolioSummary summary;
  summary.base_rate = curve.rate_at(summary_tenor);
  summary.total_pv = portfolio.total_pv(curve);
  summary.total_dv01 = portfolio.total_dv01(curve);
  summary.instrument_count = portfolio.size();
  return summary;
}

void print_summary(const PortfolioSummary& summary, double summary_tenor) {
  std::cout << "=== Portfolio Summary ===\n";
  std::cout << "Base Rate (" << summary_tenor << "y): " << summary.base_rate * 100.0 << "%\n";
  std::cout << "Instruments:      " << summary.instrument_count << '\n';
  std::cout << "Portfolio PV:     " << summary.total_pv << '\n';
  std::cout << "Portfolio DV01:   " << summary.total_dv01 << '\n';
}

int run_pricing_engine(const app_config::AppConfig& config) {
  set_log_level(config.log_level);

  logging::info("Main") << "event=pricing_engine status=start curve_path=" << config.curve_path
                        << " instruments_path=" << config.instruments_path
                        << " report_path=" << config.report_path
                        << " summary_tenor=" << config.summary_tenor;

  // Extension point for later work:
  // an observer-based workflow may register portfolio observers before loading trades.
  const auto curve = YieldCurve::from_csv(config.curve_path);

  // Extension point for later work:
  // an observer-based workflow may register portfolio observers before loading trades.
  Portfolio portfolio = Portfolio::from_csv(config.instruments_path, curve);

  // Baseline lecture version:
  // summary is computed manually here rather than by an observer.
  const PortfolioSummary summary = compute_summary(*curve, portfolio, config.summary_tenor);
  print_summary(summary, config.summary_tenor);

  logging::info("Main") << "event=portfolio_summary status=ready base_rate=" << summary.base_rate
                        << " instrument_count=" << summary.instrument_count
                        << " total_pv=" << summary.total_pv << " total_dv01=" << summary.total_dv01;

  generate_risk_report(portfolio, *curve, config.report_path, 100);

  logging::info("Main") << "event=pricing_engine status=complete report_path="
                        << config.report_path;
  return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    const app_config::AppConfig config =
        app_config::parse_args(app_config::command_line_args(argc, argv));
    return run_pricing_engine(config);
  } catch (const std::exception& error) {
    logging::error("Main") << "event=pricing_engine status=failed reason=" << error.what();
    std::cerr << "[ERROR] " << error.what() << '\n';
    return 1;
  }
}
