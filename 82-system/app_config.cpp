// app_config.cpp
#include "app_config.h"

#include <stdexcept>

namespace app_config {
namespace {

bool starts_with(std::string_view value, std::string_view prefix) {
  return value.rfind(prefix, 0) == 0;
}

LogLevel parse_log_level(std::string_view value) {
  if (value == "debug") {
    return LogLevel::DEBUG;
  }
  if (value == "info") {
    return LogLevel::INFO;
  }
  if (value == "warn") {
    return LogLevel::WARN;
  }
  if (value == "error") {
    return LogLevel::ERROR;
  }

  throw std::invalid_argument("unknown log level: " + std::string(value));
}

double parse_double_argument(std::string_view name, std::string_view value) {
  try {
    return std::stod(std::string(value));
  } catch (...) {
    throw std::invalid_argument("cannot parse " + std::string(name) + ": " + std::string(value));
  }
}

}  // namespace

std::vector<std::string_view> command_line_args(int argc, char* argv[]) {
  std::vector<std::string_view> args;
  args.reserve(static_cast<std::size_t>(argc));
  for (int index = 0; index < argc; ++index) {
    args.emplace_back(argv[index]);
  }
  return args;
}

AppConfig parse_args(const std::vector<std::string_view>& args) {
  AppConfig config;

  for (std::size_t index = 1; index < args.size(); ++index) {
    const std::string_view arg = args[index];

    if (arg == "debug" || arg == "info" || arg == "warn" || arg == "error") {
      config.log_level = parse_log_level(arg);
      continue;
    }

    if (starts_with(arg, "--log-level=")) {
      config.log_level = parse_log_level(arg.substr(std::string_view("--log-level=").size()));
      continue;
    }

    if (starts_with(arg, "--curve=")) {
      config.curve_path = std::string(arg.substr(std::string_view("--curve=").size()));
      continue;
    }

    if (starts_with(arg, "--instruments=")) {
      config.instruments_path = std::string(arg.substr(std::string_view("--instruments=").size()));
      continue;
    }

    if (starts_with(arg, "--report=")) {
      config.report_path = std::string(arg.substr(std::string_view("--report=").size()));
      continue;
    }

    if (starts_with(arg, "--summary-tenor=")) {
      config.summary_tenor = parse_double_argument(
          "summary tenor", arg.substr(std::string_view("--summary-tenor=").size()));
      continue;
    }

    throw std::invalid_argument("unknown argument: " + std::string(arg));
  }

  return config;
}

}  // namespace app_config
