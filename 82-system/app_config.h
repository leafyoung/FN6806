// app_config.h
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "logger.h"

namespace app_config {

struct AppConfig {
  std::string curve_path = "data/curve.csv";
  std::string instruments_path = "data/instruments.csv";
  std::string report_path = "output/risk_report.csv";
  double summary_tenor = 5.0;
  LogLevel log_level = LogLevel::INFO;
};

std::vector<std::string_view> command_line_args(int argc, char* argv[]);
AppConfig parse_args(const std::vector<std::string_view>& args);

}  // namespace app_config
