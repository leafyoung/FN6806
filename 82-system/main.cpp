// https://replit.com/@YeKunlun/82-system

#include <iostream>

#include "portfolio.h"
#include "risk_report.h"
#include "yield_curve.h"

using namespace std;

int main() {
  try {
    // 1. Load shared curve (factory method)
    auto curve = YieldCurve::from_csv("data/curve.csv");

    // 2. Load portfolio (instruments share the curve via shared_ptr)
    auto portfolio = Portfolio::from_csv("data/instruments.csv", curve);

    // 3. Print summary
    const double rate = curve->rate_at(5.0);
    std::cout << "Portfolio PV:   " << portfolio.total_pv(rate) << '\n';
    std::cout << "Portfolio DV01: " << portfolio.total_dv01(rate) << '\n';

    // 4. Write risk report (RAII + exceptions inside)
    generate_risk_report(portfolio, curve, "output/risk_report.csv");
  } catch (const std::runtime_error &e) {
    std::cerr << "[ERROR] " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "[ERROR] unexpected exception\n";
    return 1;
  }
  return 0;
}