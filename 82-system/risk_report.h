#pragma once
#include <string>

#include "portfolio.h"
#include "yield_curve.h"

void generate_risk_report(const Portfolio &portfolio,
                          std::shared_ptr<YieldCurve> curve,
                          const std::string &output_path);