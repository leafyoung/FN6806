#pragma once
#include <string>

class Portfolio;
class YieldCurve;

void generate_risk_report(const Portfolio& portfolio, const YieldCurve& curve,
                          const std::string& output_path, double scenario_shift_bp);
