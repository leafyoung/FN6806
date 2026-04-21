#pragma once
#include <string>

class Portfolio;

void generate_risk_report(const Portfolio& portfolio,
                          double base_rate,
                          const std::string& output_path);
