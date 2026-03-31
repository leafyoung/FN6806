#include <fstream>
#include <iostream>

#include "portfolio.h"
#include "yield_curve.h"

Portfolio Portfolio::from_csv(const std::string &path,
                              std::shared_ptr<YieldCurve> curve) {
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("Portfolio::from_csv: cannot open " + path);

  Portfolio portfolio;
  std::string line;
  std::getline(file, line); // skip header

  int line_num = 1;
  while (std::getline(file, line)) {
    ++line_num;
    try {
      portfolio.add(parse_instrument_line(line, curve));
    } catch (const std::invalid_argument &e) {
      // Log and skip bad lines; do not abort the whole load
      std::cerr << "[WARN] line " << line_num << ": " << e.what() << '\n';
    }
  }
  return portfolio; // NRVO - no copy
}